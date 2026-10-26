# Hardware bring-up checklist

## Intended components
- STM32 NUCLEO-F446RE
- 4-wire PWM BLDC fan and its datasheet
- External supply rated for steady and startup current
- NTC thermistor and correctly selected divider, or a documented analog temperature sensor
- Suitable open-drain/transistor interface if required by the fan
- Optional oscilloscope/logic analyzer and an independent RPM reference

## Before wiring
1. Verify fan supply range, PWM frequency/polarity, tach output type, pull-up voltage and pulses per revolution from its datasheet.
2. Check MCU pin voltage limits; do not connect an unknown tach pull-up directly.
3. Do not power a fan from an MCU GPIO.
4. Ensure the ADC divider stays within the input range during normal operation and sensor faults.
5. Establish signal-ground requirements for the chosen fan and supply.

## CubeMX configuration to record
- PWM timer/channel/pin and calculated output frequency.
- Tach input-capture timer/channel/pin, timer clock and prescaler.
- ADC channel, reference, sample time, optional DMA and conversion curve.
- UART instance, pins and baud rate.
- Interrupt priorities if callbacks are used.

This repository currently has no generated `.ioc` file or compile-verified STM32 HAL application. The host build must not be represented as a target firmware build.

## Physical validation
1. Verify PWM frequency/polarity with a scope before connecting the fan.
2. Compare RPM conversion against an independent reference.
3. Compare temperature against a reference thermometer at multiple points.
4. Test several setpoints, then check startup, missing tach, fan stall and sensor faults.
5. Store logs/screenshots under `docs/evidence/` with the board, toolchain and test setup.

This prototype is not safety-certified and must not be the only cooling mechanism for critical hardware.
