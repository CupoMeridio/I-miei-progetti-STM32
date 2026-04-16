# Traffic_Light_Queue_System - Semaforo Intelligente con Coda

## Obiettivo

Questo progetto dimostra un **sistema complete di controllo** che combina state machine, queue, e timer interrupt in un'applicazione realistica: un semaforo stradale che cambia colore automaticamente e può ricevere comandi in coda.

## 🎯 Cosa Fa

Il progetto simula un semaforo intelligente con 3 LED (Rosso, Giallo, Verde):
- Cambia automaticamente ogni X secondi (Rosso 30s → Giallo 5s → Verde 25s)
- I comandi di cambio colore possono essere accodati
- Un timer interrupt gestisce il timing deterministico
- Il main loop rimane libero

### Hardware
- **LED:** 3 LED (Rosso, Giallo, Verde) su GPIOA
- **Timer:** Per timing automatico tra stati
- **Queue:** Coda FIFO per buffering comandi
- **Application:** Semaforo stradale realistico

## 🔧 Come Funziona

Il sistema usa una **state machine** (macchina a stati). Un semaforo ha 3 stati: Rosso, Giallo, Verde. Lo stato attuale determina quale LED è acceso. Ogni stato ha una durata fissa (es. Rosso 30 secondi). Quando la durata scade, il sistema transisce al prossimo stato.

Il timer interrupt monitora il tempo trascorso. Quando la durata dello stato attuale è terminata, l'interrupt triggera la transizione al prossimo stato.

La **queue** permette al sistema di ricevere comandi di cambio colore che verranno processati in sequenza. Ad esempio: "Vai a Giallo, poi Verde, poi Rosso" viene enqueued e processato sequenzialmente.

## 💡 Concetti Interessanti

**State Machine:** Un sistema che ha stati discreti e transizioni deterministiche tra di essi. Un semaforo è l'esempio perfetto: hai esattamente 3 stati e le transizioni sono scritte nella logica (Rosso → Giallo → Verde → Rosso).

**Timing Deterministico:** Usa un timer hardware per il timing, non delay software. Questo garantisce che i tempi siano precisi e prevedibili, essenziale per sistemi reali (il semaforo deve essere sincronizzato con altri semafori sulla strada).

**Queue Asincrona:** I comandi arrivano indipendentemente dal timing del semaforo. La queue li bufferizza e li processa in ordine, permettendo al sistema di rimanere reattivo.

## 🎓 Applicazioni Pratiche Similari

- **Controllo ascensori:** Piano attuale, coda di piani richiesti
- **Sequenza di operazioni:** Qualsiasi processo multi-step determinato
- **Automazione industriale:** Sequenze di azionamento
- **Sistemi di traffico intelligente:** Sincronizzazione tra multiple semafori

## 🚀 Come Usare

Compila il progetto. Vedrai il LED Rosso acceso per 30 secondi, poi Giallo per 5, poi Verde per 25, poi ripete. Il ciclo è completamente automatico.

Se aggiungi pulsanti per enqueue comandi, potrai mandare il semaforo immediatamente di Rosso (ad esempio per emergenza), mantenendo la coda di comandi.

## 🎓 Cosa Impari

- State machine: come strutturare logica sequenziale
- Timing deterministico con timer hardware
- Queue per buffering asincrono
- Architettura di sistemi reali (production-grade)
- Sincronizzazione tra componenti

## 💡 Estensioni Possibili

- **Sensore di traffico:** Adatta la durata del verde in base al numero di auto
- **Pulsante emergenza:** Override immediato per ambulanza
- **Sincronizzazione:** Coordinamento con altri semafori vicini
- **UART debug:** Log dello stato attuale su terminale

---

**Progetto avanzato che dimostra un sistema production-ready!**
