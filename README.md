# Orquesta de Motores

Estos motores cantan y bailan.

## Materiales

- 1x Arduino Uno (o similar)
- 1x DRV8825
- 1x Motor paso a paso
- 1x Capacitor electrolítico (100uF recomendado)
- 1x Fuente de alimentación externa (12V recomendado)

## Conexiones

Una buena referencia para entender las conexiones entre el DRV8825 y el Arduino es este [enlace](https://lastminuteengineers.com/drv8825-stepper-motor-driver-arduino-tutorial/).

Este es el esquema de conexiones entre el DRV8825 y el Arduino:

Mapa DRV8825

![Mapa DRV8825](https://lastminuteengineers.com/wp-content/uploads/arduino/DRV8825-Stepper-Motor-Driver-Pinout.png)

O visto al reves:

| A     | B  |
|-------|----------|
| DIR   | GND      |
| STP   | FLT      |
| SLP   | 2A      |
| RST   | 1A      |
| M2    | 1B      |
| M1    | 2B      |
| M0    | GND     |
| EN    | VMOT    |


Conexiones entre el controlador DRV8825 y el Arduino:
| DRV8825 | Arduino |
|---------|---------|
| RST     | 5V     |
| SLP     | 5V     |
| GND LOGIC | GND   |
| STEP    | 3       |
| DIR     | 2       |

Conexiones entre el controlador DRV8825 y el motor paso a paso. Revisar con un multímetro las parejas de cables que tienen continuidad para identificar las bobinas del motor. Luego, cada pareja de cables se conecta a una de las dos salidas del DRV8825:

| DRV8825 | Motor Paso a Paso |
|---------|-------------------|
| 1A      | A+                |
| 1B      | A-                |
| 2A      | B+                |
| 2B      | B-                |

Conexiones entre el controlador DRV8825 y la fuente de alimentación externa:

:warning: **Es importante usar un capacitor electrolítico (100uF recomendado) entre VMOT y GND para evitar picos de voltaje que puedan dañar el controlador.** Ver imagen de referencia.

| DRV8825 | Fuente de Alimentación |
|---------|-----------------------|
| VMOT    | +12V                  |
| GND     | GND                   |

![Conexiones](https://lastminuteengineers.com/wp-content/uploads/arduino/Wiring-Nema-17-Stepper-Motor-to-DRV8825-driver-Arduino.png)

## Calibrar el DRV8825

Es importante calibrar el DRV8825 para que entregue la corriente adecuada al motor paso a paso. Esto se hace ajustando el potenciómetro en el DRV8825 mientras se mide el voltaje de referencia (Vref). 

Con el motor desconectado pero la corriente del Arduino y la fuente de alimentación encendidas, mide el voltaje entre el pin Vref (el pin central del potenciómetro) y GND.

Para el motor PM35S-048 estamos usando un valor de 0.15V que nos dió un buen rendimiento sin que el motor se caliente demasiado.

Con un Nema 17 - 36.8 (0.25Nm) de 200 pasos por revolución, usamos un valor de 0.3V.

