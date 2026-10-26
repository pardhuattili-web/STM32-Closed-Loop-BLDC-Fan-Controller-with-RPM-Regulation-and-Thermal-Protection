# Architecture

- `firmware/core/fan_control.c`: portable algorithms; no HAL dependencies.
- `firmware/platform/`: target-specific ADC, timer capture, PWM and UART adapters.
- `firmware/app/`: demo and application orchestration.
- `sim/`: deterministic first-order fan model for repeatable experiments.
- `tests/`: portable C and Python tests.

## Suggested periodic control sequence

1. Consume the latest valid, filtered temperature.
2. Interpolate requested RPM from the thermal schedule.
3. Consume a fresh tachometer measurement.
4. Update PI only when the measurement is valid.
5. Apply duty limits and any fan-specific startup policy.
6. Update fault state and publish diagnostics.

Use timer hardware for PWM and tach capture; avoid blocking delays in the control path. Protect snapshots shared with ADC-DMA or timer callbacks with atomic access or brief critical sections. The response to invalid temperature/tach data is application-specific; maximum cooling is appropriate only if the fan and powered system support it.
