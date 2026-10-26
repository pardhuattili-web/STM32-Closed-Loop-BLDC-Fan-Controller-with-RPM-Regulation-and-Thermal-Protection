# Architecture

## Separation of concerns

- `firmware/core/fan_control.c`: portable algorithms that can be built and tested on a host.
- `firmware/platform/`: STM32 HAL adapters for ADC, timer input capture, PWM, and UART.
- `firmware/app/`: application orchestration and host demo.
- `sim/`: first-order fan model for repeatable algorithm experiments.
- `tests/`: host-side unit and model tests.

## Suggested periodic execution

Run a periodic control step at a fixed frequency selected for the plant and measurement rates (for example, 10–20 Hz for an initial fan experiment). Each step should:
1. consume the latest valid filtered temperature;
2. calculate the requested speed from the thermal schedule;
3. consume a fresh tachometer measurement;
4. update the PI output only when the measurement is valid;
5. apply duty limits and startup behavior;
6. update fault state and publish diagnostics.

Use timer hardware for PWM and tachometer capture. Avoid blocking delays in the control path.

## Concurrency

If ADC DMA and timer capture callbacks update values asynchronously, transfer snapshots to the control task/loop using atomic access or a short critical section appropriate to the MCU. Do not read partially updated multiword structures without a synchronization plan.

## Safe policy

The policy for an invalid temperature or tachometer signal must be application-specific. For a cooling fan, maximum requested cooling can be a reasonable fail-safe only if the fan interface and powered system support it. Keep a persistent fault indicator and document restart/recovery criteria.
