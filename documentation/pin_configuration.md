
# Pin Configuration

## Overview

This document describes the pin connections between the Arduino, L298N motor driver, and TCRT5000L five-channel IR sensor array.

The Arduino acts as the main controller. It receives digital signals from the IR sensors, processes the sensor information using the PID algorithm, and sends control signals to the motor driver.

## 1. L298N Motor Driver Connections

The L298N dual H-bridge motor driver controls the two N20 DC motors.

The following pin configuration is used in the Arduino program:

| L298N Pin | Arduino Pin | Function |
|---|---|---|
| ENA | D3 | Right motor speed control (PWM) |
| IN1 | D4 | Right motor direction control |
| IN2 | D5 | Right motor direction control |
| ENB | D9 | Left motor speed control (PWM) |
| IN3 | D6 | Left motor direction control |
| IN4 | D7 | Left motor direction control |

### Motor Connections

| Motor Driver Output | Connected Component |
|---|---|
| OUT1 and OUT2 | Right N20 DC Motor |
| OUT3 and OUT4 | Left N20 DC Motor |

The ENA and ENB pins receive PWM signals to control motor speed, while IN1, IN2, IN3, and IN4 determine the direction of rotation.

## 2. IR Sensor Array Connections

The TCRT5000L five-channel sensor array is connected to the Arduino digital pins.

| Sensor Channel | Arduino Pin | Sensor Position |
|---|---|---|
| IR1 | D13 | Leftmost |
| IR2 | D12 | Left |
| IR3 | D11 | Middle |
| IR4 | D10 | Right |
| IR5 | D8 | Rightmost |

The sensor array provides five digital signals to the Arduino.

In the implemented program:

- LOW indicates an active sensor detecting the line.
- HIGH indicates an inactive sensor.

## 3. Arduino Pin Configuration Summary

| Arduino Pin | Connected Component | Purpose |
|---|---|---|
| D3 | L298N ENA | Right motor PWM |
| D4 | L298N IN1 | Right motor direction |
| D5 | L298N IN2 | Right motor direction |
| D6 | L298N IN3 | Left motor direction |
| D7 | L298N IN4 | Left motor direction |
| D8 | IR5 | Rightmost sensor input |
| D9 | L298N ENB | Left motor PWM |
| D10 | IR4 | Right sensor input |
| D11 | IR3 | Middle sensor input |
| D12 | IR2 | Left sensor input |
| D13 | IR1 | Leftmost sensor input |

## 4. Power Connections

The power system consists of three 18650 lithium-ion batteries, a battery holder, and a KCD11 power switch.

The battery supply provides power to the robot's electronic and motor-driving system according to the actual wiring configuration.

The following connections should be verified during assembly:

- The battery supply is connected to the appropriate power input of the motor driver.
- The Arduino receives power through its intended power input.
- The IR sensor array receives its required supply voltage.
- The Arduino, sensor array, and motor driver share a common ground reference.
- The power switch is connected in series with the intended supply line.

**Important:** The exact battery voltage, series/parallel configuration, Arduino power input, and regulator connections depend on the actual circuit assembly. Verify these before reproducing the hardware.

## 5. PWM Configuration

The Arduino uses two PWM-capable pins to control the motor speeds.

| PWM Pin | Motor |
|---|---|
| D3 | Right Motor |
| D9 | Left Motor |

The `analogWrite()` function generates PWM signals with values ranging from 0 to 255.

The program uses a maximum motor speed command of 200.

## 6. Serial Communication

The Arduino serial communication interface is initialized at:

```cpp
Serial.begin(9600);
```

The Serial Monitor displays the following debugging information:

- Calculated position error.
- PID correction output.
- Left motor speed command.
- Right motor speed command.

This information is useful for observing the controller's behavior and tuning the PID parameters.

## 7. Important Hardware Notes

1. Confirm the motor polarity and rotation direction before operating the robot.
2. Ensure that the Arduino and motor driver share a common ground.
3. Verify that the battery voltage is within the permitted operating range of each connected component.
4. Check the L298N enable jumper configuration when using PWM speed control.
5. Ensure that the IR sensor outputs are compatible with the Arduino input voltage.

</br>

</br>
