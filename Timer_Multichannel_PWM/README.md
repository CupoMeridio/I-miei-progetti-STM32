# Timer_Multichannel_PWM - Generazione PWM su Canali Multipli

## Obiettivo
Il progetto illustra le modalità di controllo di **molteplici dispositivi indipendenti** utilizzando le uscite PWM (Pulse Width Modulation) generate da una singola periferica timer. L'implementazione permette di gestire diverse intensità luminose su ciascun canale, mantenendo una frequenza di commutazione comune.

## 🎯 Funzionamento
Il sistema prevede la configurazione del timer **TIM2 con 4 canali PWM** per il controllo indipendente della luminosità di 4 LED. Ciascun canale opera con un Duty Cycle specifico, permettendo di impostare livelli di intensità differenziati (es. 25%, 75%, ecc.) per ogni uscita.

### Specifiche Hardware
- **Periferica:** TIM2 (configurato con 4 canali di Output Compare).
- **Output:** 4 LED collegati a pin dedicati (es. PA0-PA3).
- **Frequenza PWM:** ~1 kHz (uniforme per tutti i canali).
- **Duty Cycle:** Regolabile indipendentemente per ogni canale (0-100%).

## 🔧 Analisi Tecnica
Un timer hardware opera con una frequenza di clock e un periodo (Auto-Reload Register) definiti a livello di periferica. Tuttavia, la presenza di più registri di confronto (**Capture/Compare Registers - CCRx**) consente di modulare le singole uscite in modo autonomo.

Il segnale viene generato confrontando il valore del contatore principale con il valore impostato nel registro del singolo canale:
1. Il contatore incrementa fino al valore massimo stabilito.
2. L'uscita del canale rimane nello stato logico HIGH finché il contatore è inferiore al valore di soglia del registro CCRx.
3. Al superamento della soglia, l'uscita passa allo stato logico LOW fino al termine del periodo.



## 💡 Applicazioni Pratiche
* **LED RGB:** Gestione delle tre componenti cromatiche (Rosso, Verde, Blu) tramite canali indipendenti per la sintesi dei colori.
* **Motori in corrente continua (DC):** Controllo simultaneo e indipendente della velocità di più attuatori.
* **Sistemi di ventilazione:** Regolazione della velocità di rotazione di più ventole con profili di raffreddamento diversificati.

## 🎓 Concetti Chiave

**Frequenza Comune:** Poiché i canali condividono la medesima base dei tempi del timer, la frequenza del segnale PWM è identica per tutte le uscite associate a quella specifica periferica.

**Duty Cycle Indipendente:** È possibile variare la frazione di tempo "ON" di ciascun canale senza interferire con lo stato degli altri.

**Modifica a Runtime:** Il Duty Cycle può essere aggiornato dinamicamente durante l'esecuzione del firmware. Questa caratteristica consente al sistema di reagire istantaneamente a input esterni (es. variazioni rilevate da sensori di luminosità o temperatura).



## 🚀 Modalità di Utilizzo
Dopo la compilazione e il caricamento del firmware sulla scheda NUCLEO-G474RE, le uscite mostreranno intensità luminose predefinite. Tramite l'uso di un oscilloscopio è possibile verificare la sincronia dei fronti di salita (frequenza comune) e la differente ampiezza temporale degli impulsi (duty cycle variabile).

La modifica dei valori nei registri di confronto permette di osservare variazioni istantanee dell'intensità luminosa o della velocità dei carichi collegati.

## 🎓 Competenze Acquisite
- Configurazione e gestione di periferiche timer multicanale.
- Comprensione dei vantaggi del controllo PWM rispetto alla regolazione di tensione lineare (efficienza energetica e precisione).
- Implementazione di logiche di controllo asincrone per attuatori multipli.
- Gestione dei registri di configurazione in tempo reale.
