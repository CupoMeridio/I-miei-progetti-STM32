# LED_PWM_Fade_Comparison - Analisi di Architetture Blocking vs Non-Blocking

## Obiettivo
Il progetto analizza e confronta due diverse metodologie di controllo della luminosità (fading) di un LED tramite segnale **PWM (Pulse Width Modulation)** su microcontrollore **STM32G474RE**. L'analisi si focalizza sulla differenza strutturale tra un approccio basato su ritardi software (bloccante) e uno basato su interrupt hardware (non bloccante).

## 🎯 Funzionalità
Il sistema gestisce due canali PWM indipendenti con un pattern di fading identico, implementati tramite due architetture distinte:

* **Canale 1 (PA5):** Controllo tramite **Main Loop**. L'aggiornamento del Duty Cycle avviene in modo sequenziale, intervallato da funzioni di ritardo software.
* **Canale 2 (PA1):** Controllo tramite **Timer Interrupt**. L'aggiornamento del Duty Cycle è delegato a una ISR (Interrupt Service Routine), rendendo l'operazione asincrona rispetto al flusso principale.

### Specifiche Tecniche
* **MCU:** STM32G474RETx (NUCLEO-G474RE).
* **Timer:** TIM2 (Canali 1 e 2).
* **Frequenza PWM:** ~100 Hz (configurata per eliminare lo sfarfallio visivo).
* **Risoluzione:** Modulazione del Duty Cycle su una scala di 1000 step.

## 🔧 Analisi dei Metodi

### 1. Approccio Blocking (Main Loop)
In questa configurazione, il firmware utilizza la funzione `HAL_Delay()` per temporizzare l'incremento della luminosità.
* **Comportamento:** Il processore entra in uno stato di attesa attiva durante il ritardo, sospendendo l'esecuzione di qualsiasi altra logica nel ciclo principale.
* **Limitazione:** L'architettura non è scalabile; l'aggiunta di task simultanei (es. campionamento sensori o gestione comunicazioni) risulterebbe compromessa dai tempi di attesa del loop.

### 2. Approccio Non-Blocking (Timer Interrupt)
L'aggiornamento della luminosità è gestito tramite la callback di overflow del timer (`HAL_TIM_PeriodElapsedCallback`).
* **Comportamento:** Il Timer genera un evento periodico che attiva l'esecuzione della logica di aggiornamento. Terminata la callback, la CPU riprende immediatamente le attività del Main Loop.
* **Vantaggio:** Il Main Loop rimane disponibile per l'elaborazione di task in tempo reale. Questa architettura garantisce determinismo e precisione temporale hardware.


## 💡 Concetti Dimostrati

**Pulse Width Modulation (PWM):** Tecnica di modulazione della potenza applicata a un carico variando il rapporto tra tempo di accensione (ON) e tempo di spegnimento (OFF) del segnale.

**Gestione delle Risorse CPU:** Utilizzo delle periferiche hardware e delle eccezioni per implementare logiche multitasking efficienti senza l'ausilio di un sistema operativo real-time (RTOS).



**Determinismo:** Differenza tra temporizzazione software (soggetta a jitter e latenze del loop) e temporizzazione hardware sincronizzata con il clock di sistema.

## 🎓 Conclusioni
L'analisi evidenzia come l'approccio **Non-Blocking** sia lo standard per applicazioni embedded professionali. Mentre il risultato visivo è comparabile, solo l'architettura basata su interrupt permette la gestione simultanea e reattiva di più periferiche, requisito fondamentale per sistemi complessi.

---
**Esercitazione realizzata per il corso di Sistemi Embedded - Università degli Studi di Salerno (UNISA).**
