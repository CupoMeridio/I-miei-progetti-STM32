# LED_Queue_Control_System - Sistema Interattivo con Gestione Code e Interrupt

## Obiettivo
Il progetto illustra l'implementazione di un sistema di controllo embedded basato su un'architettura **event-driven**. L'obiettivo principale è la gestione asincrona di comandi utente tramite l'integrazione di code FIFO (First-In-First-Out), interrupt esterni (EXTI) e temporizzatori multipli, garantendo la thread-safety e la massima reattività del sistema.

## 🎯 Funzionalità del Sistema
Il firmware permette l'interazione con l'utente attraverso una serie di pulsanti per la creazione di sequenze luminose dinamiche:

* **Buffering dei Comandi:** Ogni pressione dei pulsanti dedicati ai colori (Verde, Giallo, Rosso) inserisce un comando in una coda di esecuzione.
* **Gestione Asincrona:** L'esecuzione dei comandi avviene tramite un timer dedicato che processa la coda in background, mantenendo il Main Loop libero per altre attività.
* **Controllo Dinamico:** Il sistema permette di variare in tempo reale la velocità di esecuzione della sequenza e di abilitare/disabilitare l'output globale.

### Specifiche Hardware
* **MCU:** STM32G474RETx (NUCLEO-G474RE).
* **Input (Pulsanti con EXTI):**
  * **PB0** (Button_on) → EXTI0
  * **PB1** (Button_red) → EXTI1
  * **PB3** (Button_yellow) → EXTI3
  * **PB4** (Button_speed_up) → EXTI4
  * **PB5** (Button_off) → EXTI5
  * **PB6** (Button_green) → EXTI6
  * **PB9** (Button_queue_execute) → EXTI9
  * **PB10** (Button_speed_down) → EXTI10
* **Output (LED):**
  * **PA0** → Led_green
  * **PA1** → Led_yellow
  * **PA4** → Led_red
  * **PA7** → Led_blu
* **Timer:** TIM6, TIM7, TIM16, TIM17 per la scansione della coda e la gestione dei tempi di sistema.

## 🔧 Architettura Software

### 1. Gestione della Coda (FIFO)
Il nucleo del sistema è una coda circolare che funge da buffer tra gli input dell'utente (produttori) e il timer di esecuzione (consumatore). 
* **Thread-Safety:** L'accesso alla coda è protetto da **sezioni critiche** (disabilitazione temporanea degli interrupt) per prevenire race condition tra il contesto EXTI e quello del timer.



### 2. Flusso di Esecuzione
L'architettura segue un modello a eventi coordinato come segue:

1.  **Evento Input:** La pressione di un pulsante genera un interrupt EXTI.
2.  **Debouncing:** Un filtro software basato su timestamp ignora i segnali spuri derivanti dai rimbalzi meccanici (finestra di 50 ms).
3.  **Enqueuing:** Il comando viene validato e inserito nella coda FIFO.
4.  **Processing:** Un timer periodico (TIM17) interroga la coda ad intervalli regolari. Se disponibile, un comando viene estratto ed eseguito, attivando il LED corrispondente.



## 💡 Concetti Dimostrati

**Pattern Produttore-Consumatore:** Implementazione di un sistema in cui l'input hardware genera dati (comandi LED) che vengono consumati in modo asincrono da un processo temporizzato.

**Sezioni Critiche:** Gestione della concorrenza in sistemi embedded bare-metal per garantire l'integrità dei dati condivisi tra diversi contesti di esecuzione.

**Modularità Hardware:** Coordinamento di periferiche multiple per compiti specializzati (Timer per il timing, EXTI per l'interattività, GPIO per l'output).

## 🎓 Competenze Acquisite
* Sviluppo di applicazioni asincrone non bloccanti.
* Implementazione di strutture dati thread-safe per sistemi microcontrollati.
* Tecniche avanzate di debouncing e sincronizzazione tra interrupt.
* Ottimizzazione del Main Loop tramite delega dei task alle periferiche hardware.
