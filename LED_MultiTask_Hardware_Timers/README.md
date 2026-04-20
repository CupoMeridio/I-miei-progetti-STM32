# LED_Queue_Control_System - Sistema Multi-Tasking per il Controllo LED

## Obiettivo
Il progetto è un'applicazione didattica avanzata volta a dimostrare l'integrazione di molteplici periferiche hardware su STM32G474RE. L'obiettivo è gestire diversi comportamenti dei LED (lampeggio automatico, controllo dinamico della velocità e registrazione di sequenze tramite code) in modo asincrono e non bloccante.

## 🎯 Funzionalità del Sistema
Il firmware implementa tre sottosistemi indipendenti che operano simultaneamente:

### 1. Sistema di Blinking in Background (TIM6, TIM7, TIM16)
Gestisce il lampeggio automatico di tre LED con frequenze diverse. Questo sistema può essere attivato o disattivato globalmente tramite pulsanti dedicati.
*   **LED Rosso (PA4):** Controllato da TIM6 (~1 Hz).
*   **LED Giallo (PA1):** Controllato da TIM7 (~2 Hz).
*   **LED Verde (PA0):** Controllato da TIM16 (~5 Hz).
*   **Comandi:**
    *   **Button_on (PB0):** Avvia i timer e inizia il lampeggio.
    *   **Button_off (PB5):** Ferma i timer e interrompe il lampeggio.

### 2. Controllo Dinamico della Velocità (TIM17)
Un quarto LED (Blu) opera indipendentemente dagli altri. La sua frequenza di lampeggio può essere regolata in tempo reale dall'utente.
*   **LED Blu (PA7):** Controllato da TIM17.
*   **Comandi:**
    *   **Button_speed_up (PB4):** Aumenta la velocità di lampeggio (riduce il periodo del timer).
    *   **Button_speed_down (PB10):** Diminuisce la velocità di lampeggio (aumenta il periodo del timer).

### 3. Sistema di Registrazione ed Esecuzione Sequenze (Queue FIFO)
Permette all'utente di "registrare" una sequenza di colori e di eseguirla manualmente in un secondo momento.
*   **Registrazione:** Premendo i pulsanti dei colori (Rosso PB1, Giallo PB3, Verde PB6), l'identificativo del LED viene inserito in una coda circolare (FIFO).
*   **Esecuzione:** Premendo il pulsante **Button_queue_execute (PA9)**, il comando più vecchio viene estratto dalla coda e lo stato del LED corrispondente viene invertito (Toggle).

---

## 🔧 Architettura Tecnica

### Gestione degli Interrupt (EXTI)
Tutti gli input utente sono gestiti tramite interrupt esterni (EXTI). È implementato un meccanismo di **debouncing software** (50ms) per garantire la stabilità dei segnali dai pulsanti meccanici.

### Coda Thread-Safe
Il sistema utilizza una struttura dati `queue_t` custom per la gestione dei comandi. Per prevenire race condition tra il contesto EXTI (produttore) e il contesto dell'esecuzione (consumatore), le operazioni sulla coda sono protette da **sezioni critiche** tramite la disabilitazione temporanea degli interrupt (`__disable_irq()`).

### Configurazione Hardware (NUCLEO-G474RE)
*   **MCU:** STM32G474RETx
*   **Output (LED - cablaggio):**
  * LED Rosso - Pin **PA4**
  * LED Giallo - Pin **PA1**
  * LED Verde - Pin **PA0**
  * LED Blu - Pin **PA7**
*   **Input (Pulsanti):**
    *   PB0 (ON), PB5 (OFF)
    *   PB4 (Speed+), PB10 (Speed-)
    *   PB1 (Red), PB3 (Yellow), PB6 (Green)
    *   PA9 (Execute)

## 💡 Concetti Dimostrati
*   **Multitasking Cooperativo:** Gestione di più processi indipendenti senza l'uso di un RTOS.
*   **Manipolazione dei Registri a Runtime:** Modifica dei valori ARR (Auto-Reload Register) per variare le frequenze dei timer durante l'esecuzione.
*   **Sincronizzazione tra Interrupt:** Uso di sezioni critiche per garantire l'integrità dei dati condivisi.
*   **Architettura Event-Driven:** Risposta immediata agli input hardware tramite ISR (Interrupt Service Routines).
