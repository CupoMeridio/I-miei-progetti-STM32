# Faiding_RGB - Controllo LED RGB con Transizioni Smooth

## Obiettivo
Il progetto implementa un sistema di controllo per LED RGB che esegue transizioni cromatiche continue e fluide. Il microcontrollore **STM32G474RE** gestisce tre canali PWM indipendenti, variando dinamicamente il duty cycle di ciascun canale per produrre effetti di fading tra colori adiacenti nello spazio RGB.

## 🎯 Funzionalità
Il sistema realizza un ciclo cromatico continuo che attraversa sei fasi di transizione, passando attraverso tutti i colori primari e secondari in modo fluido e senza interruzioni.

* **Transizioni:** Il firmware implementa una macchina a stati che gestisce il passaggio graduale tra colori consecutivi, aumentando o diminuendo progressivamente le componenti R, G e B.
* **Ciclo Cromatico:** Rosso → Giallo → Verde → Ciano → Blu → Magenta → (ritorno a Rosso)

### Specifiche Tecniche
* **MCU:** STM32G474RETx (NUCLEO-G474RE).
* **Timer PWM:** TIM2 (Canali 1, 2, 3) per le tre componenti RGB.
* **Timer Interrupt:** TIM6 per la temporizzazione delle transizioni.
* **Risoluzione PWM:** 8 bit (256 livelli per canale).
* **Uscite (cablatura):**
  * LED RGB - Anodo comune collegato a **PA0** (Componente Rosso)
  * LED RGB - Anodo comune collegato a **PA1** (Componente Verde)
  * LED RGB - Anodo comune collegato a **PB10** (Componente Blu)

## 🔧 Architettura del Sistema

### Gestione PWM
Il Timer 2 è configurato in modalità PWM con tre canali indipendenti, ciascuno collegato a una componente del LED RGB. Il duty cycle di ogni canale determina l'intensità della rispettiva componente cromatica, permettendo la sintesi di qualsiasi colore nello spazio RGB.

### Macchina a Stati per le Transizioni
Il cuore del sistema è una macchina a stati finiti implementata nella callback del Timer 6. Ogni stato rappresenta una fase di transizione tra due colori:

* **Stato 0 (Rosso→Giallo):** Aumenta progressivamente la componente verde.
* **Stato 1 (Giallo→Verde):** Diminuisce progressivamente la componente rossa.
* **Stato 2 (Verde→Ciano):** Aumenta progressivamente la componente blu.
* **Stato 3 (Ciano→Blu):** Diminuisce progressivamente la componente verde.
* **Stato 4 (Blu→Magenta):** Aumenta progressivamente la componente rossa.
* **Stato 5 (Magenta→Rosso):** Diminuisce progressivamente la componente blu.

### Sincronizzazione
Il Timer 6 genera un interrupt periodico che attiva l'aggiornamento delle componenti cromatiche. Questo approccio garantisce transizioni uniformi e indipendenti dal carico del main loop, grazie alla natura non-bloccante degli interrupt hardware.

## ⚙️ Configurazione dei Timer

Il microcontrollore è alimentato dall'oscillatore interno HSI a **16 MHz** senza PLL attivo, quindi tutti i timer ricevono un clock di $F_{clock} = 16\,000\,000 \text{ Hz}$.

La formula generale per calcolare la frequenza di un timer è:

$$F_{timer} = \frac{F_{clock}}{(PSC + 1) \times (ARR + 1)}$$

dove **PSC** è il Prescaler e **ARR** è l'Auto-Reload Register (Counter Period).

---

### TIM2 — Generazione PWM

TIM2 produce i tre segnali PWM per le componenti R, G e B. I parametri sono scelti per ottenere una **risoluzione di 8 bit** (256 livelli distinti) e una frequenza abbastanza alta da essere invisibile all'occhio umano.

**Perché ARR = 255?**

La risoluzione del PWM è determinata dall'ARR: il contatore va da $0$ a $ARR$, producendo $ARR + 1$ livelli distinti. Con $ARR = 255$ si ottengono esattamente **256 livelli** $(0$–$255)$, che corrispondono alla codifica standard a 8 bit dei colori RGB. I valori delle variabili `r`, `g`, `b` nel firmware possono quindi essere scritti direttamente nel registro $CCR$ senza alcuna conversione.

**Calcolo del Prescaler per ~1 kHz:**

Una frequenza PWM di circa 1 kHz è sufficiente per eliminare qualsiasi sfarfallio visibile. Fissato $ARR = 255$, si ricava il valore teorico di $PSC$ invertendo la formula:

$$PSC = \frac{F_{clock}}{F_{pwm} \times (ARR + 1)} - 1 = \frac{16\,000\,000}{1000 \times 256} - 1 = 62{,}5 - 1 = 61{,}5$$

Poiché $PSC$ deve essere un intero, si arrotonda a $PSC = 62$. Verifica:

$$F_{pwm} = \frac{16\,000\,000}{(62 + 1) \times 256} = \frac{16\,000\,000}{63 \times 256} = \frac{16\,000\,000}{16\,128} \approx 992 \text{ Hz} \quad \checkmark$$

| Parametro TIM2 | Valore | Descrizione |
|----------------|--------|-------------|
| **Prescaler (PSC)** | 62 | Divide il clock a ~254 kHz |
| **Counter Period (ARR)** | 255 | 256 livelli → risoluzione 8 bit |
| **CCR1 / CCR2 / CCR3** | $0$–$255$ | Duty cycle per R, G, B rispettivamente |

---

### TIM6 — Interrupt di aggiornamento cromatico

TIM6 non genera segnali PWM: si limita a generare un **interrupt periodico** che incrementa o decrementa di 1 unità la componente colore corrente. La frequenza di questo interrupt determina la velocità delle transizioni.

**Quanti interrupt servono per un ciclo completo?**

La macchina a stati percorre 6 fasi, ognuna da 255 step:

$$\text{Step totali} = 255 \times 6 = 1530 \text{ interrupt/ciclo}$$

**Calcolo della frequenza effettiva con $PSC = 15999$, $ARR = 9$:**

$$F_{tim6} = \frac{16\,000\,000}{(15999 + 1) \times (9 + 1)} = \frac{16\,000\,000}{16\,000 \times 10} = 100 \text{ Hz}$$

Il prescaler divide il clock a $\frac{16\,000\,000}{16\,000} = 1\,000 \text{ Hz}$, che il contatore ulteriormente divide per $ARR + 1 = 10$, producendo un interrupt ogni $10 \text{ ms}$.

**Durata del ciclo cromatico completo:**

$$T_{ciclo} = \frac{\text{Step totali}}{F_{tim6}} = \frac{1530}{100} = 15{,}3 \text{ s}$$

| Parametro TIM6 | Valore | Descrizione |
|----------------|--------|-------------|
| **Prescaler (PSC)** | 15999 | Divide il clock a 1 kHz |
| **Counter Period (ARR)** | 9 | Interrupt a 100 Hz (ogni 10 ms) |
| **Ciclo cromatico completo** | 15,3 secondi | 1530 step a 100 Hz |

> 💡 **Modificare la velocità è semplice:** la durata del ciclo scala linearmente con $ARR$.
> Ad esempio, $ARR = 4 \Rightarrow F_{tim6} = 200 \text{ Hz} \Rightarrow T_{ciclo} \approx 7{,}65 \text{ s}$; $\quad ARR = 19 \Rightarrow F_{tim6} = 50 \text{ Hz} \Rightarrow T_{ciclo} \approx 30{,}6 \text{ s}$.

---

### Flusso temporale complessivo

```
16 MHz (HSI)
  │
  ├─► TIM2 (PSC=62, ARR=255)
  │     └─► PWM ~992 Hz su CH1, CH2, CH3
  │           ├─► PA0  → LED Rosso   (CCR1 = r)
  │           ├─► PA1  → LED Verde   (CCR2 = g)
  │           └─► PB10 → LED Blu     (CCR3 = b)
  │
  └─► TIM6 (PSC=15999, ARR=9)
        └─► Interrupt 100 Hz (ogni 10 ms)
              └─► Callback: r/g/b ±1 → __HAL_TIM_SET_COMPARE()
```

---

## 💡 Concetti Dimostrati

**Modulazione PWM Multi-Canale:** Utilizzo di un singolo timer per controllare più uscite PWM indipendenti, ottimizzando l'uso delle risorse periferiche del microcontrollore.

**Macchine a Stati:** Pattern di progettazione che permette di gestire sequenze complesse di comportamenti in modo deterministico e manutenibile.

**Transizioni Smooth:** Tecnica di interpolazione lineare tra due valori, ottenuta variando progressivamente un parametro nel tempo anziché commutarlo bruscamente.

**Sincronizzazione Temporale:** Utilizzo degli interrupt per garantire aggiornamenti periodici precisi, indipendenti dall'esecuzione del codice principale.

## 🎓 Conclusioni
Il progetto dimostra come combinare tecniche di modulazione PWM e gestione degli interrupt per creare effetti visivi complessi. L'approccio basato su macchina a stati rende il codice modulare e facilmente estendibile: aggiungere nuove transizioni o modificare la sequenza cromatica richiede solo modifiche alla logica di transizione, senza alterare la struttura del sistema.