# LED_Queue_Control_System - Sistema Interattivo di Controllo LED

## Obiettivo

Questo progetto dimostra un **sistema completo e interattivo** dove l'utente (tramite pulsanti) accoda comandi di LED in una coda, che vengono processati sequenzialmente da interrupt timer. È una applicazione realistica di queue, interrupt EXTI, debouncing, e multi-timer.

## 🎯 Cosa Fa

Il progetto ha **8 pulsanti su GPIOB** e **4 LED su GPIOA** (Verde, Giallo, Rosso, Blu). L'utente preme i pulsanti per:
- Aggiungere colori di LED alla coda
- Controllare la velocità di esecuzione
- Accendere/spegnere i LED globalmente

I comandi accodati vengono eseguiti sequenzialmente da un timer interrupt, il main loop rimane sempre libero.

### Hardware
- **LED:** 4 LED (Verde, Giallo, Rosso, Blu) su GPIOA
- **Pulsanti:** 8 pulsanti + 1 pulsante execute su GPIOB (EXTI interrupt)
- **Timer:** 4 timer (TIM6, TIM7, TIM16, TIM17) per controllo velocità e esecuzione
- **Queue:** Coda FIFO thread-safe per buffering comandi LED

## 🔧 Come Funziona

Ogni volta che premi un pulsante (es. "Green"), il sistema aggiunge il comando alla coda. La coda rimane in memoria finché non viene processata.

Un timer interrupt (TIM17) opera periodicamente. Ogni scatto, controlla se c'è un comando in coda. Se sì, lo estrae ed esegue (accende il LED corrispondente). Se no, attende il prossimo interrupt.

Il **debouncing** elimina i falsi trigger dai rimbalzi meccanici dei pulsanti. Il sistema attende 50 millisecondi prima di considerare stabile la pressione.

La **sezione critica** protegge la coda da race condition. Quando il main loop o un interrupt accede alla coda, disabilita gli altri interrupt per garantire che nessuno modifichi la coda simultaneamente.

## 💡 Concetti Interessanti

**Queue FIFO:** First-In-First-Out. I comandi vengono processati nell'ordine in cui sono stati aggiunti. Come una coda al supermercato.

**Thread-safe Queue:** La coda è usata sia dal main loop (che aggiunge comandi) che dall'interrupt (che li estrae). Per evitare conflitti, quando uno accede alla coda, disabilita gli interrupt globali (critical section).

**Multi-Timer:** Il sistema usa 4 timer diversi. TIM17 scandisce il processing della coda. Gli altri timer possono controllare velocità, debouncing, etc. È un esempio di coordinamento complesso tra periferiche.

**Debouncing Software:** Ogni pulsante ha un timestamp dell'ultimo interrupt. Se un nuovo interrupt arriva meno di 50 ms dopo, viene ignorato (rimbalzo).

## 🎓 Applicazioni Pratiche Similari

- **Menu interattivo:** Scegli operazione premendo pulsanti, vengono eseguite in sequenza
- **Automazione con feedback utente:** Premendo pulsanti accodai task che il sistema esegue
- **Sistemi di controllo industriale:** Operatori premono pulsanti, macchina esegue sequenze
- **Giochi embedded:** Input da giocatore accodato e processato

## 🚀 Come Usare

Compila il progetto. Poi:
1. Premi i pulsanti dei colori (Green, Red, Yellow) per aggiungere colori alla coda
2. Premi il pulsante "Execute" (o simile) per iniziare a processare
3. I LED si accenderanno in sequenza (colore accodato per il tempo impostato)
4. Premi Speed Up/Down per aumentare/diminuire la velocità di esecuzione

## 🎓 Cosa Impari

- Architettura event-driven (pulsanti generano interrupt)
- Queue per buffering asincrono
- Sincronizzazione tra interrupt e main loop
- Debouncing hardware+software
- Thread-safety in embedded (critical section)
- Multi-timer coordinamento
- Pattern realistica di sistema interattivo

## 💡 Estensioni Possibili

- **Sequenze memorizzate:** Salva sequenze di comandi e ripetile
- **Display feedback:** Mostra quanti comandi sono in coda su LCD
- **UART logging:** Log delle operazioni su terminale serial
- **Sensore feedback:** Verifica che il LED sia effettivamente acceso

---

**Progetto avanzato e realistico, simile a sistemi veri!**
# LED_Queue_Control_System - Sistema Interattivo con Queue e Interrupt

## Obiettivo

Implementare un **sistema completo di controllo LED** che dimostra architettura embedded production-grade:
- **Queue (FIFO)** per buffering comandi di LED
- **Interrupt EXTI** su 8 pulsanti con debouncing 50 ms
- **Multi-timer** per controllo della velocità e sincronizzazione
- **Thread-safe queue** con sezioni critiche
- **Pattern event-driven** asincrono

## 🎯 Funzionalità Principale

### Hardware
- **Microcontrollore:** STM32G474RE
- **LED:** 4 LED (verde, giallo, rosso, blu) su GPIOA (PA5-7, PA-X)
- **Pulsanti:** 8 pulsanti + 1 Execute button su GPIOB
- **Timer:** TIM6, TIM7, TIM16, TIM17 (controllo velocità multi-timer)
- **Applicazione:** Sistema interattivo controllato da pulsanti

### Controlli Disponibili

| Pulsante | Funzione | Output |
|----------|----------|--------|
| **Button_on** | Abilita LED output | Tutti OFF → accensione abilitata |
| **Button_off** | Disabilita LED output | Disattiva tutti i LED |
| **Button_green** | Accoda LED verde | Aggiunge GREEN alla queue |
| **Button_red** | Accoda LED rosso | Aggiunge RED alla queue |
| **Button_yellow** | Accoda LED giallo | Aggiunge YELLOW alla queue |
| **Button_speed_up** | Aumenta velocità | Decrementa timer ARR (più veloce) |
| **Button_speed_down** | Diminuisci velocità | Incrementa timer ARR (più lento) |
| **Button_queue_execute** | Esegui sequenza | Processa queue accodata |

## 🔧 Architettura Software

### Queue Structure

```c
// Definito in queue.h
typedef struct queue_s {
    uint16_t qsize;       // Capacità massima (es. 10 item)
    uint16_t length;      // Numero item attuali
    uint16_t head;        // Indice lettura
    uint16_t tail;        // Indice scrittura
    uint16_t isize;       // Size di ogni item (es. 1 byte per LED_ID)
    uint8_t* items;       // Buffer dati allocato esternamente
} queue_t;
```

### LED Command Type

```c
// In main.h
typedef enum {
    LED_GREEN = 0,
    LED_YELLOW = 1,
    LED_RED = 2,
    LED_BLUE = 3
} led_id_t;

#define QUEUE_CAPACITY 10
```

### Queue Operations

```c
// Inizializzazione
queue_t led_queue;
uint8_t queue_buffer[QUEUE_CAPACITY * sizeof(led_id_t)];
queue_init(&led_queue, queue_buffer, sizeof(led_id_t), QUEUE_CAPACITY);

// Aggiungere comando (in interrupt EXTI)
led_id_t color = LED_GREEN;
queue_enqueue(&led_queue, &color);  // Thread-safe (disabilita IRQ)

// Estrarre comando (in main loop o TIM17 callback)
led_id_t cmd;
if (queue_extract(&led_queue, &cmd) == QUEUE_OK) {
    turn_on_led(cmd);
}

// Verificare stato
if (queue_is_empty(&led_queue)) {
    // Coda vuota
}

if (queue_is_full(&led_queue)) {
    // Coda piena - rifiuta ulteriori comandi
}
```

## 📊 Flow Architetturale

```
[PULSANTE PREMUTO]
        ↓
 [EXTI INTERRUPT]
        ↓
[DEBOUNCE CHECK]  ← 50 ms timeout
        ↓
[queue_enqueue]   ← Aggiunge comando in coda
  (Thread-safe)
        ↓
[TIM17 INTERRUPT] ← Periodico (configurable)
        ↓
[queue_extract]   ← Estrae comando se disponibile
        ↓
[turn_on_led]     ← Accende LED corrispondente
        ↓
[MAIN LOOP]       ← Sempre libero, asincrono
```

## 💻 Implementazione Dettagliata

### 1. EXTI Interrupt Handler

```c
// In stm32g4xx_it.c

#define DEBOUNCE_DELAY_MS 50
static uint32_t last_interrupt_time[16] = {0};

extern queue_t led_queue;
extern uint8_t queue_buffer[];

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint32_t current_time = HAL_GetTick();
    uint16_t pin_idx = __builtin_ctz(GPIO_Pin);  // Find bit position
    
    // Debounce check
    if (current_time - last_interrupt_time[pin_idx] < DEBOUNCE_DELAY_MS) {
        return;  // Rimbalzo, rigetta
    }
    
    last_interrupt_time[pin_idx] = current_time;
    
    // Switch su pulsante premuto
    switch (GPIO_Pin) {
        case Button_green_Pin:
        {
            led_id_t color = LED_GREEN;
            queue_enqueue(&led_queue, &color);
            break;
        }
        
        case Button_red_Pin:
        {
            led_id_t color = LED_RED;
            queue_enqueue(&led_queue, &color);
            break;
        }
        
        case Button_yellow_Pin:
        {
            led_id_t color = LED_YELLOW;
            queue_enqueue(&led_queue, &color);
            break;
        }
        
        case Button_speed_up_Pin:
            increase_speed();  // Decrementa ARR
            break;
            
        case Button_speed_down_Pin:
            decrease_speed();  // Incrementa ARR
            break;
        
        // ... altri pulsanti ...
    }
}
```

### 2. TIM17 Interrupt Handler

```c
// In stm32g4xx_it.c

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM17) {
        // Prova a estrarre comando dalla coda
        led_id_t color;
        
        if (queue_extract(&led_queue, &color) == QUEUE_OK) {
            // Comando disponibile - eseguilo
            turn_on_led(color);
            
            // Salva timestamp per timing successivo
            static uint32_t cmd_start_time = 0;
            cmd_start_time = HAL_GetTick();
        }
    }
}
```

### 3. Main Loop (Non-blocking)

```c
// In main.c

int main(void)
{
    // Initialization
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM17_Init();  // Timer per esecuzione coda
    
    // Queue initialization
    queue_init(&led_queue, queue_buffer, sizeof(led_id_t), QUEUE_CAPACITY);
    
    // Avvia timer interrupt
    HAL_TIM_Base_Start_IT(&htim17);
    
    while (1)
    {
        // Main loop rimane COMPLETAMENTE LIBERO!
        // Nessun blocking, nessun delay
        
        // Opzionale: puoi fare altre cose qui
        // - Leggere sensori
        // - Logging su UART
        // - Update display
        // - Etc.
        
        // Tutto il controllo LED è gestito da interrupt!
    }
}
```

### 4. LED Control Functions

```c
// In gpio.c o separato

void turn_on_led(led_id_t color)
{
    // Spegni tutti i LED
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);
    
    // Accendi quello selezionato
    switch (color) {
        case LED_GREEN:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);  // PA7
            break;
        case LED_YELLOW:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);  // PA6
            break;
        case LED_RED:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);  // PA5
            break;
        default:
            break;
    }
}

void turn_off_all_leds(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);
}
```

## ⚙️ Configurazioni Importanti

```c
// In main.h
#define QUEUE_CAPACITY 10

// In stm32g4xx_it.c
#define DEBOUNCE_DELAY_MS 50           // Debounce per pulsanti
#define TIMER_STEP 500                 // Step timer in µs
#define MIN_ARR 100                    // Minimo value ARR (veloce)
#define MAX_ARR 65535                  // Massimo value ARR (lento)
```

## 📝 Critical Section Protection

```c
// Queue è thread-safe grazie a questo pattern in queue.c:

uint8_t queue_enqueue(queue_t* queue, const void* item)
{
    // Disabilita interrupt globali
    __disable_irq();
    
    // Sezione critica - accedo al queue senza race condition
    size_t offset = queue->tail * queue->isize;
    memcpy(queue->items + offset, item, queue->isize);
    queue->tail = (queue->tail + 1) % queue->qsize;
    
    if (queue->length < queue->qsize - 1) {
        queue->length++;
    }
    
    // Abilita interrupt nuovamente
    __enable_irq();
    
    return QUEUE_OK;
}
```

## 🔌 Pin Mapping

| Funzione | Pin | GPIO | Edge |
|----------|-----|------|------|
| Button_green | PB-X | GPIOB | RISING |
| Button_red | PB-X | GPIOB | RISING |
| Button_yellow | PB-X | GPIOB | RISING |
| Button_speed_up | PB-X | GPIOB | RISING |
| Button_speed_down | PB-X | GPIOB | RISING |
| Button_on/off | PB-X | GPIOB | RISING |
| LED_GREEN | PA7 | GPIOA | OUTPUT |
| LED_YELLOW | PA6 | GPIOA | OUTPUT |
| LED_RED | PA5 | GPIOA | OUTPUT |

## 🚀 Come Usare

1. **Compilare:**
   ```bash
   cd Debug && make clean && make
   ```

2. **Flashare su NUCLEO-G474RE**

3. **Interagire:**
   - Premi pulsanti per aggiungere colori alla coda
   - Sistema esegue automaticamente la sequenza
   - Regola velocità con speed up/down

## 💡 Estensioni Avanzate

**Logging stato coda:**
```c
void log_queue_status(void)
{
    uint16_t len;
    queue_get_length(&led_queue, &len);
    printf("Queue items: %d\r\n", len);
}
```

**Visualizzazione sequenza:**
```c
void show_queue_sequence(void)
{
    led_id_t cmd;
    queue_t temp = led_queue;  // Copia temporanea
    
    while (queue_extract(&temp, &cmd) == QUEUE_OK) {
        printf("LED: %s\r\n", color_names[cmd]);
    }
}
```

**UART Debug Interface:**
```c
// Inviare stato su UART per monitoring remoto
HAL_UART_Transmit(&huart2, (char*)buffer, len, 100);
```

## 🎓 Concetti Avanzati Dimostrati

✅ **FIFO Queue** - Buffering asincrono comandi  
✅ **Event-driven** - Basato su interrupt, non polling  
✅ **Thread-safety** - Critical sections con IRQ disable  
✅ **Multi-timer** - Coordinamento tra TIM6, TIM7, TIM16, TIM17  
✅ **Debouncing** - Filtro rimbalzi con SysTick  
✅ **Non-blocking** - Main loop completamente libero  
✅ **Production pattern** - Architettura usata in sistemi reali  

## 📚 File Importanti

- `Core/Inc/queue.h` - Interfaccia FIFO
- `Core/Src/queue.c` - Implementazione thread-safe
- `Core/Src/main.c` - Application logic
- `Core/Src/stm32g4xx_it.c` - Interrupt handlers (EXTI, TIM17)
- `Core/Src/gpio.c` - GPIO configuration

---

**Progetto avanzato e realistico per embedded system production!**
