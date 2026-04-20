# SG90 Servo Control

Controllo di un servomotore SG90 tramite PWM generato da TIM2 del microcontroller STM32G474RET6.

## Panoramica del Progetto

Questo progetto utilizza il Timer 2 in modalità PWM per controllare la posizione di un servomotore SG90. Il controllo avviene tramite interrupt esterni su pin GPIO che impostano diversi angoli di rotazione.

### Caratteristiche

- **Microcontroller**: STM32G474RET6  
- **Frequenza clock**: 16 MHz (HSI interno)  
- **Timer**: TIM2 in modalità PWM  
- * **Pin di output (servo)**: PA0 (TIM2_CH1) – segnale PWM per il servo SG90
* **Pulsanti di selezione angolo**: 
  * Button 0° – Pin **PB0**
  * Button 45° – Pin **PB1**
  * Button 90° – Pin **PB2**
  * Button 135° – Pin **PB3**
  * Button 180° – Pin **PB4**  
- **Angoli supportati**: 0°, 45°, 90°, 135°, 180°  

---

## Configurazione del Timer

### Parametri TIM2

| Parametro | Valore | Descrizione |
|----------|--------|------------|
| Prescaler (PSC) | 159 | Divide il clock per 160 |
| Periodo (ARR) | 1999 | Counter massimo |
| Clock Division | DIV1 | Nessuna divisione aggiuntiva |
| Modalità | PWM1 | PWM edge-aligned |

---

## Calcolo della Frequenza PWM

La frequenza del PWM è determinata da:

$$
f_{PWM} = \frac{f_{TIMER}}{(PSC + 1)\cdot(ARR + 1)}
$$

Dove:

- $f_{TIMER} = 16\ \text{MHz}$
- $PSC = 159$
- $ARR = 1999$

### Calcolo passo passo

1. **Frequenza del timer dopo il prescaler**

$$
f_{timer} = \frac{16\ \text{MHz}}{159 + 1} = \frac{16\ \text{MHz}}{160} = 100\ \text{kHz}
$$

2. **Frequenza PWM finale**

$$
f_{PWM} = \frac{100\ \text{kHz}}{1999 + 1} = \frac{100\ \text{kHz}}{2000} = 50\ \text{Hz}
$$

3. **Periodo del PWM**

$$
T = \frac{1}{f_{PWM}} = \frac{1}{50} = 20\ \text{ms}
$$

> **Nota**: Il periodo di 20 ms (50 Hz) è lo standard per i servomotori SG90.

---

## Calcolo degli Angoli

Il servomotore SG90 utilizza la larghezza dell’impulso PWM per determinare la posizione.

### Formula generale

$$
\text{Pulse Width} = \frac{CCR}{ARR + 1} \cdot T
$$

Nel nostro caso:

$$
\text{Pulse Width} = \frac{CCR}{2000} \cdot 20\ \text{ms}
$$

---

### Tabella valori

| Angolo | CCR | Larghezza impulso |
|--------|-----|------------------|
| 0° | 50 | 0.5 ms |
| 45° | 100 | 1.0 ms |
| 90° | 150 | 1.5 ms |
| 135° | 200 | 2.0 ms |
| 180° | 240 | 2.4 ms |

---

### Range del Servo SG90

Valori tipici:

- 0° → 0.5 ms  
- 90° → 1.5 ms  
- 180° → 2.5 ms  

---

## Risoluzione del PWM

$$
\text{Resolution} = \frac{T}{ARR + 1} = \frac{20\ \text{ms}}{2000} = 10\ \mu s
$$

---

## Configurazione del Clock

Il sistema utilizza il clock HSI interno:

```c
RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
RCC_OscInitStruct.HSIState = RCC_HSI_ON;
RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;