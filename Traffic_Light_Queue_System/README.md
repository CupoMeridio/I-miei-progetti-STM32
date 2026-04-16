# Traffic_Light_Queue_System - Sistema Semaforico Intelligente con Coda

## Obiettivo
Il progetto illustra l'integrazione di **Macchine a Stati Finiti (FSM)**, code di messaggi (Queue) e interrupt hardware per la realizzazione di un sistema di controllo realistico. L'applicazione simula un semaforo stradale automatizzato in grado di gestire transizioni temporizzate deterministiche e di processare comandi asincroni tramite buffering.

## 🎯 Funzionamento
Il sistema simula il comportamento di un semaforo tramite l'utilizzo di 3 LED (Rosso, Giallo, Verde) e segue una logica sequenziale predefinita:

* **Ciclo Automatico:** Transizione temporizzata tra gli stati (es. Rosso 30s → Giallo 5s → Verde 25s).
* **Gestione Comandi:** Possibilità di accodare richieste di cambio stato tramite una coda FIFO.
* **Architettura Non Bloccante:** Un timer interrupt gestisce il timing e le transizioni, mantenendo il Main Loop disponibile per la ricezione di input o altre attività di monitoraggio.

### Specifiche Hardware
* **Output:** 3 LED configurati su GPIOA (Rosso, Giallo, Verde).
* **Timer:** Periferica dedicata alla scansione dei tempi di permanenza in ogni stato.
* **Struttura Dati:** Coda FIFO per il buffering dei comandi di override o di sequenza.

## 🔧 Analisi Tecnica

### 1. Macchina a Stati Finiti (FSM)
La logica di controllo è basata su stati discreti. Lo stato attuale determina quale periferica di output attivare. Il passaggio allo stato successivo è regolato da eventi temporali (scadenza del timer) o da eventi esterni (estrazione di un comando dalla coda).



### 2. Timing Deterministico
A differenza dei ritardi software, il sistema utilizza un timer hardware per conteggiare la durata di ogni fase. Questo approccio garantisce che la durata degli stati sia precisa e costante, requisito fondamentale per la sincronizzazione stradale reale.

### 3. Buffering Asincrono
L'integrazione di una coda permette al sistema di ricevere istruzioni (es. "Passa a Rosso per emergenza") in modo asincrono. I comandi vengono memorizzati e processati sequenzialmente senza interferire con la stabilità temporale della ISR (Interrupt Service Routine).

## 💡 Concetti Chiave

**Stati e Transizioni:** Rappresentazione della logica tramite stati (es. `STATE_RED`, `STATE_GREEN`) e regole di transizione definite, che garantiscono un comportamento prevedibile del sistema.

**Sincronizzazione Hardware-Software:** Coordinamento tra il clock periferico e la logica applicativa per ottenere un'esecuzione in tempo reale.



**Priority Handling:** Capacità del sistema di gestire transizioni standard e comandi prioritari (come chiamate di emergenza) attraverso la manipolazione della coda o degli stati della FSM.

## 🎓 Applicazioni Pratiche
* **Sistemi di Trasporto:** Controllo di incroci intelligenti e coordinamento di flussi di traffico.
* **Logistica:** Gestione di nastri trasportatori e sequenze di smistamento merci.
* **Automazione Industriale:** Controllo di processi multi-fase con tempistiche rigorose (es. cicli di lavorazione meccanica).
* **Ascensori:** Gestione delle code di prenotazione piani e transizioni tra livelli.

## 🚀 Modalità di Utilizzo
Al caricamento del firmware sulla scheda NUCLEO-G474RE, il sistema avvia il ciclo automatico. È possibile osservare la commutazione sequenziale dei LED secondo i tempi prestabiliti. Tramite l'invio di segnali esterni (pulsanti o messaggi UART, se implementati), è possibile accodare nuovi stati e osservare la deviazione controllata dal ciclo standard verso la nuova sequenza inserita.

## 🎓 Competenze Acquisite
- Progettazione e implementazione di Macchine a Stati Finiti (FSM) in ambiente embedded.
- Utilizzo di timer hardware per la gestione di tempistiche deterministiche.
- Integrazione di code per la comunicazione asincrona tra contesti diversi (Interrupt/Main Loop).
- Sviluppo di architetture di controllo scalabili e robuste (production-grade).

---
**Progetto avanzato che dimostra l'integrazione di logica sequenziale e gestione asincrona degli eventi.**
