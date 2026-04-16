# STM32G474RE Embedded System Exercises

Raccolta di progetti **embedded system** realizzati durante il corso di **Embedded System** presso l'**Università degli Studi di Salerno (UNISA)**, usando la scheda **NUCLEO-G474RE**.

## 📚 Progetti

| # | Nome | Descrizione |
|----|------|-------------|
| 1 | Timer_Interrupt_Basic | Timer interrupt con TIM6 |
| 2 | LED_PWM_Fade_Comparison | PWM: interrupt vs polling |
| 3 | Timer_Multichannel_PWM | PWM su 4 canali |
| 4 | External_Interrupt_Handler | Interrupt EXTI da GPIO |
| 5 | LED_Queue_Control_System | Sistema interattivo con coda |
| 6 | Traffic_Light_Queue_System | Semaforo con state machine |

## 🛠️ Hardware & Requisiti

**Hardware:**
- [NUCLEO-G474RE board](https://www.st.com/en/evaluation-tools/nucleo-g474re.html)

**Software:**
- [ARM GCC Embedded Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
- [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
- [STLinkServer](https://www.st.com/en/development-tools/stlink-server.html)

## 📖 Documentazione Ufficiale

Per lo sviluppo su questa scheda, è fondamentale consultare i seguenti documenti:

- [**User Manual (UM2505)**](https://www.st.com/resource/en/user_manual/um2505-getting-started-with-stm32g4-series-nucleo64-board-stmicroelectronics.pdf): Guida hardware alla scheda Nucleo-64.
- [**Reference Manual (RM0440)**](https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf): Descrizione dettagliata di tutti i registri e periferiche.
- [**STM32G474RE Datasheet**](https://www.st.com/resource/en/datasheet/stm32g474re.pdf): Specifiche tecniche e pinout del microcontrollore.

## 🧠 Concetti affrontati negli esercizi

GPIO • Timer • PWM • EXTI • Interrupt Handler • Debouncing • Queue/FIFO • State Machine • Callback • HAL Library

## 📝 Note

**Per dettagli completi su un progetto, vedi il suo README.md specifico all'interno della cartella dedicata.**
