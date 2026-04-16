# External_Interrupt_Handler - Gestione Interrupt da Pulsanti

## Obiettivo
Il progetto illustra le metodologie per il rilevamento di **eventi da input hardware** (pulsanti) tramite la gestione delle interruzioni su piattaforma **STM32G474RE**. L'approccio mira a ottimizzare le risorse della CPU evitando il polling continuo e attivando l'elaborazione solo in corrispondenza di un evento reale.

## 🎯 Funzionamento
Il sistema prevede la configurazione di **8 ingressi GPIO** per la generazione di interrupt su evento di pressione. Ogni occorrenza attiva una specifica callback non bloccante. È prevista una gestione software del debouncing (rimbalzi meccanici) per garantire la stabilità del segnale.

### Specifiche Hardware
* **MCU:** STM32G474RETx (NUCLEO-G474RE)
* **Input:** 8 pulsanti attestati su GPIOB.
* **Configurazione:** EXTI (External Interrupt) su fronte di salita (RISING edge).
* **Debouncing:** Temporizzazione software di 50 ms.
* **Output:** Esecuzione di callback dedicate per ogni canale.

## 🔧 Analisi Tecnica
Il contatto meccanico di un pulsante è caratterizzato da transitori oscillatori (rimbalzi) della durata di alcuni millisecondi. In assenza di contromisure, una singola pressione causerebbe molteplici attivazioni spurie dell'interrupt.

Il meccanismo implementato prevede una finestra di guardia di 50 ms per la validazione del segnale: eventuali variazioni di stato all'interno di questo intervallo vengono ignorate. Solo al raggiungimento della stabilità elettrica viene confermata la pressione e invocata la funzione di callback per l'esecuzione della logica applicativa (es. attivazione di un'uscita o invio di comandi).

## 💡 Concetti Chiave

**EXTI (External Interrupt):** Periferica dedicata al monitoraggio dei pin GPIO e alla generazione di eccezioni hardware in corrispondenza di eventi configurati (fronte di salita, discesa o entrambi).

**Edge Detection:** Rilevamento del fronte di salita (*Rising Edge*), corrispondente alla transizione dello stato logico da LOW a HIGH al momento della pressione.



**Debouncing:** Processo di filtraggio dei rimbalzi meccanici. La stabilità del segnale è garantita attraverso un ritardo software che assicura l'univocità dell'evento registrato.

**Callback:** Funzione di servizio invocata dal sistema a seguito del riconoscimento di un interrupt validato. Rappresenta il punto di inserimento della logica di controllo specifica (`HAL_GPIO_EXTI_Callback`).

## 🎓 Applicazioni Pratiche
* **Interfacce utente:** Gestione di tastiere, switch di sicurezza e pannelli di controllo.
* **Sensori digitali:** Interfacciamento con sensori di movimento (PIR) o finecorsa magnetici.
* **Sincronizzazione:** Triggering di operazioni software basate su segnali esterni asincroni.

## 🚀 Modalità di Utilizzo
Dopo la compilazione e il caricamento del firmware sulla scheda NUCLEO-G474RE, il sistema rimane in attesa di eventi. La pressione dei pulsanti genera una risposta immediata da parte della MCU. È possibile mappare azioni specifiche (come l'accensione di un LED di stato) all'interno delle callback di ciascun pin nel file `main.c`.

## 🎓 Competenze Acquisite
* Implementazione di logiche *event-driven* per la gestione degli input.
* Tecniche di filtraggio software per segnali impulsivi.
* Utilizzo efficiente delle risorse di sistema tramite interrupt.
* Gestione della priorità e delle routine di servizio (ISR) su architettura ARM Cortex-M4.

---
**Esercitazione realizzata per il corso di Sistemi Embedded - Università degli Studi di Salerno (UNISA).**
