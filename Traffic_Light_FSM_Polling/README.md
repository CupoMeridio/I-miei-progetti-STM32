# Traffic_Light_Queue_System - FSM e Gestione Code via Polling

## Obiettivo
Il progetto illustra l'implementazione di una **Macchina a Stati Finiti (FSM)** centralizzata per la gestione di un semaforo intelligente. L'obiettivo principale è mostrare come coordinare una sequenza temporizzata deterministica con un sistema di "override" asincrono basato su code, utilizzando un approccio software basato su **Polling**.

## 🎯 Funzionamento
Il sistema simula un semaforo (Rosso, Giallo, Verde) che segue un ciclo automatico, ma che può essere influenzato da comandi esterni:

*   **Ciclo Automatico (FSM):** Il sistema transita sequenzialmente tra gli stati (Rosso 5s → Giallo 2s → Verde 1s).
*   **Gestione Override (Queue):** Tramite pulsanti esterni, l'utente può inserire nella coda un "cambio stato prioritario". Il sistema, nel suo ciclo di controllo, verifica la coda: se trova un comando, interrompe il ciclo attuale per passare immediatamente allo stato richiesto.
*   **Timing Non Bloccante:** Il tempo di permanenza in ogni stato è gestito tramite la funzione `HAL_GetTick()`, evitando l'uso di `HAL_Delay()` e mantenendo il sistema reattivo.

---

## 🔧 Architettura Software: Polling vs Hardware Timers
Questo progetto è progettato per essere studiato insieme a **[LED_Queue_Control_System](../LED_Queue_Control_System)**. Sebbene entrambi utilizzino LED e code, seguono filosofie architetturali opposte:

| Caratteristica | Traffic_Light_Queue_System | LED_Queue_Control_System |
| :--- | :--- | :--- |
| **Modello Logico** | **FSM Centrale:** Un'unica logica decide quale LED accendere. | **Parallelismo:** Ogni LED è gestito da un processo indipendente. |
| **Gestione Tempo** | **Software Polling:** Usa `HAL_GetTick()` nel loop principale. | **Hardware Timers:** Usa gli interrupt dei timer (TIM6, 7, 16, 17). |
| **Uso della Coda** | **Interruzione Sequenza:** La coda modifica il flusso della FSM. | **Registratore di Eventi:** La coda memorizza sequenze manuali. |
| **Vantaggio** | Semplice da implementare e debuggare per logiche sequenziali. | Massima precisione e indipendenza tra i task. |

---

## 💡 Analisi Tecnica

### Macchina a Stati (FSM)
La logica risiede interamente nel `while(1)` del `main.c`. Questo approccio permette di avere un unico punto di controllo per tutte le transizioni, rendendo facile l'implementazione di logiche condizionali complesse (es. "se la coda è piena, accelera il ciclo").

### Buffering Asincrono
I pulsanti (PB3, PB4, PB5) generano interrupt **EXTI** che popolano la coda. Poiché l'estrazione avviene nel loop principale (`queue_extract`), è garantita una separazione netta tra il momento in cui l'evento accade e il momento in cui viene elaborato, garantendo la stabilità del sistema.

## 🔌 Configurazione Hardware
*   **Output (LED - cablaggio):**
  * LED Rosso - Pin **PA0**
  * LED Giallo - Pin **PA1**
  * LED Verde - Pin **PA4**
*   **Input (Pulsanti su GPIOB):** PB3 (Richiesta Rosso), PB4 (Richiesta Giallo), PB5 (Richiesta Verde).
*   **MCU:** STM32G474RETx (NUCLEO-G474RE).

## 🎓 Competenze Acquisite
- Progettazione di FSM centralizzate in ambiente embedded.
- Gestione del tempo non bloccante via software (`HAL_GetTick`).
- Integrazione di code per l'override di sequenze predefinite.
- Analisi critica tra architetture software-based e hardware-based.
