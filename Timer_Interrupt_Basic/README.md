# Timer_Interrupt_Basic - Introduzione ai Timer Interrupt

## Obiettivo

Questo progetto insegna i **fondamenti dei timer interrupt** su STM32. Un timer è un contatore hardware che può generare interrupt a intervalli regolari. Invece di usare `HAL_Delay()` che blocca il programma, un timer interrupt permette al main loop di continuare libero mentre il sistema esegue azioni periodiche.

## 🎯 Cosa Fa

Il progetto configura il **TIM6** per generare un interrupt ogni millisecondo (configurabile). Ogni volta che scatta l'interrupt, il microcontrollore esegue automaticamente una callback senza bloccare il main loop.

### Hardware Utilizzato
- **Timer:** TIM6 (timer base a 16-bit)
- **Frequenza:** ~1 kHz (1 interrupt ogni 1 ms)
- **Output:** Callback non-bloccante

## 🔧 Come Funziona

Il timer è una periferica hardware che conta autonomamente. La frequenza scelta (1 kHz) significa che ogni 1 millisecondo il timer "scatta" e avvisa il processore. Il processore allora interrompe quello che sta facendo, esegue la callback (una piccola funzione), e torna al main loop.

**Vantaggi rispetto a `HAL_Delay()`:**
- Il main loop **non è bloccato** - può fare altre cose
- Il timing è **preciso e sincronizzato al hardware**, non dipende da SysTick
- Pattern ideale per applicazioni real-time

## 💡 Concetti Interessanti

**Prescaler:** Il clock del microcontrollore è molto veloce (16 MHz). Il prescaler lo "ralenta" a una frequenza utile. Ad esempio, un prescaler di 16 divide il clock per 16, ottenendo 1 MHz.

**Periodo (ARR - Auto Reload Register):** Quante volte il timer deve contare prima di generare l'interrupt. Se il timer conta a 1 MHz e deve scattare ogni 1 ms, deve contare 1000 volte.

**Callback:** È una funzione che il sistema chiama automaticamente quando scatta l'interrupt. È il posto dove mettere la logica che vuoi eseguire periodicamente (accendere un LED, leggere un sensore, etc.).

## 🚀 Come Usare

1. Compila il progetto
2. Flashalo su NUCLEO-G474RE
3. Se colleghi un LED a PA5, vedrà accendersi e spegnersi periodicamente (il timer lo controlla dalla callback)
4. Puoi modificare il periodo del timer per cambiar la frequenza

## 📝 Configurazioni Principali

Puoi modificare la **frequenza del timer** cambiando due parametri nel codice di configurazione del timer:
- **Prescaler:** Come dividi il clock principale
- **Periodo (ARR):** Quante volte il timer conta prima di interrupt

La formula è: `Frequenza_Interrupt = Clock / (Prescaler + 1) / (Periodo + 1)`

## 🎓 Cosa Impari

Questo è il progetto base per capire:
- Come i timer hardware funzionano
- Come generare interrupt periodici precisi
- Il pattern callback (fondamentale per embedded system)
- Perché gli interrupt sono meglio di delay bloccanti

---

**Progetto essenziale per imparare timer interrupt!**
