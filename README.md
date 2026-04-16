# STM32G474RE Embedded System Exercises

Raccolta di progetti **embedded system** realizzati durante il corso di **Embedded System** presso l'**Università degli Studi di Salerno (UNISA)**, usando la scheda **NUCLEO-G474RE**.

## 📚 Progetti

| # | Nome | Livello | Descrizione |
|----|------|---------|-------------|
| 1 | Timer_Interrupt_Basic | 🟢 Base | Timer interrupt con TIM6 |
| 2 | LED_PWM_Fade_Comparison | 🟡 Intermedio | PWM: interrupt vs polling |
| 3 | Timer_Multichannel_PWM | 🟡 Intermedio | PWM su 4 canali |
| 4 | External_Interrupt_Handler | 🟡 Intermedio | Interrupt EXTI da GPIO |
| 5 | LED_Queue_Control_System | 🔴 Avanzato | Sistema interattivo con coda |
| 6 | Traffic_Light_Queue_System | 🔴 Avanzato | Semaforo con state machine |

## 🚀 Quick Start

```bash
# Clonare il repository
git clone <repository-url>
cd STM32CubeMX

# Entrare in un progetto
cd Timer_Interrupt_Basic

# Compilare
cd Debug && make clean && make
```

## 🛠️ Hardware & Requisiti

**Hardware:**
- STM32G474RE (ARM Cortex-M4, 170 MHz)
- NUCLEO-G474RE board

**Software:**
- ARM GCC Embedded Toolchain
- STM32CubeProgrammer (per flashare)
- STM32CubeMX (opzionale, per rigenerare da .ioc)

## 📖 Concetti Insegnati

GPIO • Timer • PWM • EXTI • Interrupt Handler • Debouncing • Queue/FIFO • State Machine • Callback • HAL Library

## 📝 Note

- Usa **HAL Library STM32** (portabile tra MCU)
- I file di build sono nel `.gitignore`
- Ogni progetto ha **README.md dettagliato** con configurazioni e codice

---

**Per dettagli completi su un progetto, vedi il suo README.md.**
