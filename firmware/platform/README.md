# STM32 HAL adapter boundary

Implement target-specific adapters for:
- ADC temperature acquisition and conversion;
- filtered RPM snapshots from timer input capture;
- bounded PWM output for the selected fan's electrical interface;
- UART CLI transport;
- periodic control scheduling.

Keep hardware handles and ISR callbacks outside `firmware/core/fan_control.c`. Generate target initialization in CubeMX/CubeIDE and record the real pin map and timer clocks in `docs/hardware.md`.

**Not included yet:** generated CubeMX `.ioc` and compile-verified STM32 HAL application. Host-core compilation does not prove target firmware builds.
