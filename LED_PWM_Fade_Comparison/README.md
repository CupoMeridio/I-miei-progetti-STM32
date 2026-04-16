# LED_PWM_Fade_Comparison - Confronto di Due Approcci al PWM

## Obiettivo

Questo progetto mostra **due modi diversi** di controllare il brightness (luminosità) di LED tramite PWM (Pulse Width Modulation). Un LED fa fade (aumenta e diminuisce luminosità) usando il main loop, l'altro usando un interrupt. L'obiettivo è capire le differenze tra approccio "bloccante" e "non-bloccante".

## 🎯 Cosa Fa

Il progetto ha **due LED che fanno fade identico** ma con architetture diverse:
- **LED 1 (PA5):** Fade controllato dal main loop con delay
- **LED 2 (PA1):** Fade controllato da interrupt timer

Visivamente fanno lo stesso (luminosità sale e scende), ma internamente il programma si comporta molto diversamente.

### Hardware
- **Timer:** TIM2 con 2 canali PWM
- **LED 1:** Comandato dal main loop (bloccante)
- **LED 2:** Comandato da interrupt (non-bloccante)
- **Frequenza PWM:** ~100 Hz (100 volte al secondo)

## 🔧 Come Funziona

**Metodo 1 (Bloccante):**
Il main loop incrementa il duty cycle del LED 1, poi chiama `HAL_Delay(60)` che ferma tutto per 60 millisecondi. Durante questo tempo il processore non fa nulla, solo aspetta.

**Metodo 2 (Non-bloccante):**
Il main loop rimane completamente libero e fa quello che vuole. Nel frattempo, un timer interrupt genera un evento periodico che incrementa il duty cycle del LED 2. Il main può fare altre cose mentre questo accade automaticamente.

## 💡 Concetti Interessanti

**PWM (Pulse Width Modulation):** È una tecnica per controllare la luminosità di un LED. Invece di variare la tensione (complicato), il LED viene acceso e spento molto rapidamente. Se lo accendi 50% del tempo e lo spegni 50% del tempo, sembra mezzo acceso. Se lo accendi 75% del tempo, sembra più luminoso.

**Duty Cycle:** La percentuale di tempo in cui il LED è acceso. 50% = metà luminosità, 100% = massima luminosità.

**HAL_Delay() è cattivo:** Blocca il processore. Se il tuo programma ha altri compiti (leggere sensori, comunicare con periferiche), non può farli mentre aspetta il delay.

## 🎓 Differenze Chiave

| Aspetto | Metodo 1 | Metodo 2 |
|---------|----------|----------|
| **Bloccante?** | Sì - è male | No - è bene |
| **Precisione** | Discreta | Ottima |
| **Main loop libero?** | No | Sì |
| **Semplicità** | Facile | Un po' più complesso |
| **Applicazioni reali** | Quasi mai | Sempre |

## 🚀 Come Usare

Compila, flashalo, e guarda i due LED. Visivamente sembrano identici, ma uno blocca il processore mentre l'altro no. Se aggiungessi altre operazioni (leggere sensori, comunicare), il Metodo 1 le perderebbe, il Metodo 2 no.

## 🎓 Cosa Impari

- Perché non usare `HAL_Delay()` in applicazioni reali
- Come usare interrupt per timing preciso
- La differenza tra "bloccante" e "non-bloccante"
- Pattern PWM + interrupt

---

**Progetto ideale per capire perché gli interrupt sono meglio!**
# LED_PWM_Fade_Comparison - Confronto di due Approcci al PWM

## Obiettivo

Insegnare come **due metodi diversi** di controllare PWM diano risultati differenti in precisione e architettura:
- **Metodo 1 (Blocking):** Aggiornamento PWM nel main loop con `HAL_Delay()`
- **Metodo 2 (Non-blocking):** Aggiornamento PWM via interrupt timer callback

Entrambi i LED fanno un **fade ciclico** (luminosità che aumenta e diminuisce), ma con dinamiche diverse.

## 🎯 Funzionalità

### Hardware
- **Microcontrollore:** STM32G474RE
- **Timer:** TIM2 con 2 canali PWM
- **LED 1 (PA5):** Fade controllato dal main loop
- **LED 2 (PA1):** Fade controllato da interrupt timer
- **Frequenza PWM:** ~100 Hz

### Concetti Dimostrati

✅ PWM Fade pattern  
✅ Blocking loop pattern (semplice, meno efficiente)  
✅ Non-blocking interrupt pattern (efficiente, preciso)  
✅ Confronto tra due approcci  
✅ HAL_Delay vs Timer interrupt  
✅ Callback non-bloccante  

## 🔧 Configurazione Hardware

```c
// TIM2 Configuration (tim.c)
void MX_TIM2_Init(void)
{
    // ...
    htim2.Init.Prescaler = 39;   // Clock divider per ottenere ~1 MHz
    htim2.Init.Period = 999;     // 1000 count @ 1 MHz = 10 ms → 100 Hz
    // ...
    
    // Channel 1 (PA5) e Channel 2 (PA1)
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 250;       // PWM iniziale 25%
    
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);
}
```

## 💻 Metodo 1: Approccio Blocking (HAL_Delay)

```c
// In main.c - main loop
uint32_t duty_cycle = 0;

int main(void)
{
    // ... init ...
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);  // Start PWM su CH1
    
    while (1)
    {
        // Incrementa duty cycle
        duty_cycle += DUTY_INCREMENT;  // DUTY_INCREMENT = 20
        
        if (duty_cycle > 1000) {
            duty_cycle = 0;  // Reset al 0%
        }
        
        // Aggiorna il PWM (blocking)
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty_cycle);
        
        // Delay blocking: il programma ASPETTA qui
        HAL_Delay(60);  // 60 ms tra aggiornamenti
    }
}
```

### Caratteristiche Metodo 1
❌ **Bloccante** - main loop aspetta durante `HAL_Delay()`  
❌ **Meno preciso** - dipende da SysTick (1 ms granularità)  
✅ **Semplice** - facile da capire e implementare  
✅ **Basso overhead** - nessun interrupt  
⚠️ **Non-deterministic** - timing soggetto a jitter  

## 💻 Metodo 2: Approccio Non-Blocking (Interrupt)

```c
// In stm32g4xx_it.c - Interrupt handler TIM2
static uint32_t duty_cycle_ch2 = 0;
static uint32_t interrupt_counter = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        // Esegui callback ogni NUM_INTERRUPTS_PER_CYCLE interrupt
        interrupt_counter++;
        
        if (interrupt_counter >= NUM_INTERRUPTS_PER_CYCLE) {
            interrupt_counter = 0;
            
            // Incrementa duty cycle
            duty_cycle_ch2 += DUTY_INCREMENT;
            
            if (duty_cycle_ch2 > 1000) {
                duty_cycle_ch2 = 0;
            }
            
            // Aggiorna il PWM (non-blocking)
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty_cycle_ch2);
        }
    }
}

// In main.c
int main(void)
{
    // ... init ...
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);  // Start PWM su CH2
    HAL_TIM_Base_Start_IT(&htim2);             // Start timer interrupt
    
    while (1)
    {
        // Main loop rimane libero - nessun Delay!
        // Può fare altre cose...
    }
}
```

### Caratteristiche Metodo 2
✅ **Non-bloccante** - main loop continua liberamente  
✅ **Preciso** - timing controllato da timer hardware (µs granularità)  
✅ **Deterministico** - interrupt sincronizzato con hardware  
❌ **Più complesso** - richiede configurazione interrupt  
✅ **Buon overhead** - manageable per applicazioni semplici  

## 📊 Confronto Visivo

```
METODO 1 (Blocking with HAL_Delay):
TIME:     |---60ms delay---|---60ms delay---|---60ms delay---|
ACTION:   update PWM        update PWM       update PWM       
MAIN:     [WAITING]         [WAITING]        [WAITING]

METODO 2 (Interrupt-driven):
TIME:     |10ms|10ms|10ms|10ms|10ms|10ms|
TIMER:    [INT][   ][INT][   ][INT][   ]
PWM_UPD:  [   ][UPD][   ][UPD][   ][UPD]   (ogni 6 interrupt)
MAIN:     [RUNNING FREE]   [RUNNING FREE]   (non bloccato)
```

## 📐 Fade Pattern

```
Duty Cycle:
100% ┌──────────────┐
     │     ╱╲       │
  50%│    ╱  ╲      │
     │   ╱    ╲     │
  0% └──╱──────╲────┘
     t=0  t=T   t=2T

Entrambi i canali mostrano lo stesso pattern,
solo con tempistiche diverse.
```

## ⚙️ Parametri Configurabili

```c
// In main.c
#define DUTY_INCREMENT 20              // Incremento duty cycle ogni ciclo
#define CYCLE_DELAY    60              // Delay tra cicli (Metodo 1) - ms
#define NUM_INTERRUPTS_PER_CYCLE 6     // Interrupt prima di aggiornare (Metodo 2)
```

**Formula durata ciclo completo:**
```
Metodo 1: DUTY_INCREMENT=20, cicli=50, per completare: 50 * 60ms = 3 secondi
Metodo 2: NUM_INTERRUPTS_PER_CYCLE=6, interrupt ogni 10ms: 50*6*10ms = 3 secondi
```

## 🎓 Lezioni Apprese

| Aspetto | Metodo 1 | Metodo 2 |
|---------|----------|----------|
| **Blocking** | ❌ Sì | ✅ No |
| **Precisione** | ⚠️ Media | ✅ Alta |
| **Semplicità** | ✅ Alta | ⚠️ Media |
| **Real-time** | ❌ No | ✅ Sì |
| **Overhead** | ✅ Basso | ⚠️ Medio |
| **Scalabilità** | ❌ Limitata | ✅ Buona |

## 🚀 Come Usare

1. **Compilare:**
   ```bash
   cd Debug && make clean && make
   ```

2. **Flashare su NUCLEO-G474RE**

3. **Osservare:**
   - LED su PA5 (Metodo 1): Fade bloccante
   - LED su PA1 (Metodo 2): Fade non-bloccante
   - Visivamente simili, ma con architetture diverse

4. **Con oscilloscopio:**
   - Verifica frequenza PWM ~100 Hz
   - Nota il duty cycle che aumenta gradualmente

## 💡 Estensioni

**Aggiungere più azioni nel main loop (Metodo 2 only):**
```c
while (1)
{
    // Queste operazioni non vengono bloccate!
    
    // Lettura sensore
    uint16_t sensor = read_adc();
    
    // Controllo logica
    if (sensor > THRESHOLD) {
        // Fai qualcosa
    }
    
    // Altra operazione
    process_data();
    
    // NO DELAY! - tutto è asincrono
}
```

**Confronto preciso:**
```c
volatile uint32_t metodo1_updates = 0;
volatile uint32_t metodo2_updates = 0;

// Nel Metodo 1 callback:
metodo1_updates++;

// Nel main loop (Metodo 2):
if (metodo2_updates != last_update) {
    last_update = metodo2_updates;
    // Update PWM
}
```

## 🔌 Pin Utilizzo

| LED | Metodo | Pin | Canale |
|-----|--------|-----|--------|
| LED1 | Blocking | PA5 | TIM2_CH1 |
| LED2 | Interrupt | PA1 | TIM2_CH2 |

## 📝 Note Importanti

- Entrambi i LED mostrano lo **stesso pattern visuale**
- La differenza è **architetturale**, non visiva
- Metodo 2 è **preferibile** per applicazioni reali
- Metodo 1 utile solo per **demo/test semplici**

---

**Progetto didattico fondamentale per capire blocking vs non-blocking patterns!**
