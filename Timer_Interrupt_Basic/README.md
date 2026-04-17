# Timer_Interrupt_Basic - Introduzione ai Timer Interrupt

## Obiettivo
Il progetto illustra i **fondamenti dei timer interrupt** su piattaforma STM32. Un timer è un contatore hardware in grado di generare interruzioni a intervalli regolari. Rispetto all'utilizzo della funzione `HAL_Delay()`, che comporta il blocco dell'esecuzione del programma, l'uso degli interrupt permette al flusso principale (Main Loop) di procedere senza interruzioni, demandando al sistema l'esecuzione di azioni periodiche basate su eventi hardware.

## 🎯 Funzionamento
Il sistema prevede la configurazione del timer **TIM6** per la generazione di un interrupt con cadenza periodica (es. ogni millisecondo). Al verificarsi dell'evento di overflow del timer, il microcontrollore esegue automaticamente una funzione di callback, garantendo un'operatività non bloccante.

### Specifiche Tecniche
- **Periferica:** TIM6 (timer di base a 16-bit).
- **Frequenza di Interrupt:** Configurazione standard a ~1 kHz (1 interrupt ogni 1 ms).
- **Architettura:** Esecuzione asincrona tramite callback.

## 🔧 Analisi Tecnica
Il timer opera come una periferica hardware indipendente che incrementa un contatore in base al clock di sistema. Al raggiungimento del valore di soglia prestabilito, viene generata un'eccezione che induce il processore a sospendere temporaneamente il task corrente per eseguire la routine di servizio associata (ISR).

**Vantaggi dell'approccio basato su Timer:**
- **Esecuzione Non Bloccante:** Il Main Loop rimane disponibile per l'elaborazione di altri processi.
- **Determinismo:** Il timing è garantito dall'hardware e sincronizzato con il clock di sistema, risultando indipendente dal carico software.
- **Efficienza:** Rappresenta il pattern standard per lo sviluppo di applicazioni real-time.



## 💡 Concetti Chiave

**Prescaler (PSC):** Registro utilizzato per dividere la frequenza del clock principale del microcontrollore. Permette di rallentare il conteggio del timer per ottenere risoluzioni temporali utili all'applicazione.

**Auto-Reload Register (ARR):** Definisce il valore massimo raggiungibile dal contatore prima del reset e della generazione dell'interrupt. Determina, insieme al prescaler, il periodo dell'evento.

**Callback:** Funzione software invocata automaticamente dall'Hardware Abstraction Layer (HAL) al verificarsi dell'interruzione. Costituisce il punto di inserimento per la logica periodica (es. toggling di un GPIO o campionamento dati).

**Calcolo della Frequenza**: La frequenza di generazione dell'interrupt è determinata dalla seguente formula:

$$F_{interrupt} = \frac{Clock_{source}}{(Prescaler + 1) \cdot (Periodo + 1)}$$

## 🎓 Competenze Acquisite
- Comprensione del funzionamento dei timer hardware.
- Generazione di interrupt periodici con alta precisione.
- Implementazione del pattern basato su callback per sistemi embedded.
- Analisi dei vantaggi della programmazione orientata agli eventi rispetto ai ritardi bloccanti.
