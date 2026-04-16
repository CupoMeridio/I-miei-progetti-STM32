# External_Interrupt_Handler - Gestione Interrupt da Pulsanti

## Obiettivo

Questo progetto insegna come rilevare **eventi da input hardware** (pulsanti) e reagire istantaneamente. Invece di leggere continuamente "il pulsante è premuto?", il sistema si sveglia solo quando accade davvero.

## 🎯 Cosa Fa

Il progetto configura **8 pulsanti su GPIO** per generare interrupt quando vengono premuti. Ogni pressione di pulsante triggerera un interrupt che esegue una callback non-bloccante. Il debouncing (eliminazione rimbalzi) è gestito automaticamente.

### Hardware
- **Input:** 8 pulsanti su GPIOB
- **Tipo:** EXTI (External Interrupt) su RISING edge
- **Debouncing:** 50 ms software
- **Output:** Callback per ogni pulsante

## 🔧 Come Funziona

Quando premi un pulsante, non è una transizione netta: il contatto meccanico rimbalza per qualche millisecondo. Senza debouncing, un singola pressione genererebbe decine di interrupt falsi.

Il sistema attende 50 millisecondi prima di considerare la pressione "valida". Se in quei 50 ms il pulsante rimbalza, viene ignorato. Solo dopo i 50 ms puliti il sistema registra la pressione.

Una volta registrata la pressione, il sistema esegue la callback - una funzione personalizzata che puoi usare per accendere un LED, mandare un comando, etc.

## 💡 Concetti Interessanti

**EXTI (External Interrupt):** È una periferica che monitora i pin GPIO e genera interrupt quando accade un evento (fronte salita, fronte discesa, entrambi).

**Edge Detection:** Il "fronte salita" (RISING) è il momento in cui il pulsante passa da LOW (non premuto) a HIGH (premuto). Il sistema scatta solo su questo evento.

**Debouncing:** Il rimbalzo meccanico è un problema reale. Se non lo gestisci, un singola pressione viene vista come 10-20 pressioni separate. Il software attende 50 ms prima di considerare stabile la lettura.

**Callback:** Quando scatta l'interrupt, il sistema chiama automaticamente una funzione che hai scritto. È il posto dove puoi mettere la logica per reagire alla pressione.

## 🎓 Applicazioni Pratiche

**Bottoni di controllo:** Telecomandi, switch, tastiere.

**Sensori:** Rilevatori di movimento (PIR), sensori di contatto, etc.

**Sincronizzazione:** Attendere un evento prima di iniziare un'operazione.

## 🚀 Come Usare

Compila il progetto e flashalo. Premi i pulsanti e osserva come il sistema reagisce istantaneamente. Se colleghi un LED alla callback di un pulsante, vedrai accendersi quando premi.

## 🎓 Cosa Impari

- Rilevamento event-driven di input (meglio di polling)
- Gestione rimbalzi e debouncing
- Pattern interrupt per reazioni veloci
- Sincronizzazione hardware con software

---

**Progetto fondamentale per input interattivo!**
