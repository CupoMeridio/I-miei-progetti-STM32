# Timer_Multichannel_PWM - PWM su Multipli Canali

## Obiettivo

Questo progetto insegna come controllare **più dispositivi indipendenti** usando PWM dalla stessa periferica timer. Ogni canale può avere luminosità diversa, ma tutti hanno la stessa frequenza.

## 🎯 Cosa Fa

Il progetto configura il **TIM2 con 4 canali PWM** e controlla la luminosità di 4 LED indipendentemente. Ogni LED può essere a brightness diverso: uno può essere al 25%, un altro al 75%, etc.

### Hardware
- **Timer:** TIM2 (4 canali)
- **LED:** 4 LED su pin diversi (PA0-PA3)
- **Frequenza PWM:** ~1 kHz (uguale per tutti i canali)
- **Duty Cycle:** Indipendente per ogni canale (0-100%)

## 🔧 Come Funziona

Un timer ha una velocità (frequenza) e un periodo fissato. Però ha **più uscite** che puoi controllare indipendentemente. Immagina un metronomo che batte 1000 volte al secondo: 4 musicisti ascoltano lo stesso metronomo (stessa frequenza) ma suonano note diverse (duty cycle diverso).

Sul microcontrollore funziona così: un timer incrementa un contatore fino a un valore massimo (es. 1000). Ogni canale può generare un'uscita HIGH quando il contatore è sotto un valore specifico. Se il contatore massimo è 1000 e il canale 1 è impostato a 250, allora sarà HIGH per i primi 250 count (25%) e LOW per gli ultimi 750 count (75%).

## 💡 Applicazioni Pratiche

**LED RGB:** Usi 3 canali per controllare le componenti Rosso, Verde, Blu. Combinandoli puoi ottenere qualsiasi colore.

**Motori DC:** Usi più canali per controllare la velocità di più motori indipendentemente.

**Fan Speed:** Controlli la velocità di più ventole con duty cycle diversi.

## 🎓 Concetti Interessanti

**Frequenza Comune:** Tutti i canali hanno la stessa frequenza perché condividono lo stesso timer. Non puoi averne uno a 1 kHz e uno a 2 kHz.

**Duty Cycle Indipendente:** Puoi cambiare il duty cycle di ogni canale senza influenzare gli altri. Puoi anche cambiarlo "al volo" (runtime) senza fermare il timer.

**Modifica Runtime:** Uno dei vantaggi di PWM è che puoi cambiare il brightness in real-time. Ad esempio, se leggi un sensore di luminosità ambientale, puoi adattare il brightness del tuo schermo istantaneamente.

## 🚀 Come Usare

Compila il progetto. I 4 LED si accenderanno a luminosità diverse. Se colleghi un oscilloscopio, vedrai 4 segnali PWM con la stessa frequenza ma duty cycle diversi.

Puoi modificare il duty cycle di ogni LED dinamicamente e vedrai il cambio di luminosità istantaneo.

## 🎓 Cosa Impari

- Come usare multi-canali di un timer
- Perché PWM è meglio di controllo tensione (più efficiente, controllabile)
- Applicazioni realistiche (LED RGB, motori, etc.)
- Come modificare PWM in runtime

---

**Progetto essenziale per controllare più dispositivi!**
