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
* **Uscite:** PA0 (R), PA1 (G), PA2 (B).

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

## 💡 Concetti Dimostrati

**Modulazione PWM Multi-Canale:** Utilizzo di un singolo timer per controllare più uscite PWM indipendenti, ottimizzando l'uso delle risorse periferiche del microcontrollore.

**Macchine a Stati:** Pattern di progettazione che permette di gestire sequenze complesse di comportamenti in modo deterministico e manutenibile.

**Transizioni Smooth:** Tecnica di interpolazione lineare tra due valori, ottenuta variando progressivamente un parametro nel tempo anziché commutarlo bruscamente.

**Sincronizzazione Temporale:** Utilizzo degli interrupt per garantire aggiornamenti periodici precisi, indipendenti dall'esecuzione del codice principale.

## 🎓 Conclusioni
Il progetto dimostra come combinare tecniche di modulazione PWM e gestione degli interrupt per creare effetti visivi complessi. L'approccio basato su macchina a stati rende il codice modulare e facilmente estendibile: aggiungere nuove transizioni o modificare la sequenza cromatica richiede solo modifiche alla logica di transizione, senza alterare la struttura del sistema.