# Timer_Multichannel_OC - Generazione Segnali Sfasati via Output Compare

## Obiettivo
Il progetto illustra come utilizzare la modalità **Output Compare (OC)** di un timer per generare segnali periodici sincronizzati ma sfasati temporalmente su più canali. A differenza del PWM, che modula la potenza, questo approccio si focalizza sulla coordinazione temporale di eventi indipendenti che condividono la stessa base dei tempi.

## 🎯 Funzionamento
Il sistema utilizza il timer **TIM2** configurato in modalità **Toggle**. In questa configurazione, l'uscita di ciascun canale inverte il proprio stato logico ogni volta che il contatore del timer raggiunge il valore impostato nel rispettivo registro di confronto (**CCR - Capture/Compare Register**).

### Specifiche Tecniche
- **Periferica:** TIM2 (3 canali configurati in modalità TOGGLE).
- **Frequenza Segnale:** ~0.5 Hz (Il toggle avviene ogni secondo, completando un ciclo ON/OFF ogni 2 secondi).
- **Duty Cycle:** Fisso al 50% per tutti i canali (caratteristica intrinseca della modalità Toggle).

## 🔧 Analisi Tecnica
Tutti i canali condividono lo stesso registro **ARR (Auto-Reload Register)**, fissato a 999, che determina la frequenza comune. La differenza tra i valori dei registri CCR determina lo **sfasamento temporale** (fase) tra le accensioni dei LED:

1.  **Canale 2 (CCR 299):** È il primo a commutare durante il conteggio.
2.  **Canale 1 (CCR 499):** Commuta quando il contatore è circa a metà del suo percorso.
3.  **Canale 3 (CCR 799):** È l'ultimo a commutare prima del reset del timer.

Questo comportamento permette di creare sequenze temporizzate precise dove l'inizio di un evento è sfasato rispetto a un altro, pur mantenendo una sincronizzazione perfetta data dal clock comune.

## 🔌 Configurazione Hardware
*   **MCU:** STM32G474RETx (NUCLEO-G474RE)
*   **Output (LED - cablaggio):**
  * LED Rosso - Pin **PA4**
  * LED Giallo - Pin **PA1**
  * LED Verde - Pin **PA0**
*   **Interfaccia di Debug:** ST-LINK (USB)

## 💡 Concetti Chiave

**Output Compare Toggle:** Una modalità operativa in cui l'hardware inverte automaticamente lo stato del pin al match del contatore. Elimina la necessità di gestire il toggle via software (interrupt), riducendo il carico della CPU.

**Sfasamento (Phase Shift):** La sfasatura temporale tra due segnali periodici. In questo progetto, lo sfasamento è ottenuto impostando livelli di soglia (CCR) differenti su canali che condividono lo stesso periodo.

**Sincronizzazione Hardware:** Poiché tutti i segnali dipendono dallo stesso contatore, non esiste "drift" (deriva temporale) tra i canali; la loro relazione temporale rimane costante nel tempo.

## 🎓 Competenze Acquisite
- Configurazione di timer in modalità Output Compare.
- Comprensione della differenza tra modulazione PWM e commutazione di fase OC.
- Gestione di segnali multicanale sincronizzati per l'automazione di sequenze temporali.
