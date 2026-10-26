# STM32 Closed-Loop BLDC Fan Controller

A portfolio project for temperature-aware speed control of a **4-wire PWM-controlled BLDC fan** using an STM32, tachometer feedback, and a PI controller.

> **Project status:** Portable control logic and host-side simulation are the first deliverables. Hardware-specific STM32 initialization and physical fan validation must be completed on the target board before claiming a fully validated hardware implementation.

## Goals

- Measure fan speed from tachometer pulses.
- Regulate RPM using a bounded PI controller.
- Schedule a target speed from temperature.
- Detect missing temperature data and a stalled fan.
- Keep control logic independent of STM32 HAL so it can be tested on a host.
- Document hardware assumptions, test evidence, and remaining work honestly.

## Architecture

```text
NTC thermistor -> ADC -> temperature conversion/filter -----+
                                                            |
                                                            v
                                                    Thermal policy
                                                            |
                                                            v
Tach output -> timer input capture -> RPM estimator -> PI controller
                                                            |
                                                            v
                                               PWM command / fan interface
                                                            |
                                                            v
                                                    4-wire BLDC fan

UART CLI <-> status/configuration <-> controller + fault manager
```

The portable modules in `firmware/core/` have no STM32 HAL dependency. `firmware/platform/` is the board-specific integration boundary. The host simulator exercises control logic without claiming to reproduce a real fan exactly.

## Feature set

- Temperature-to-speed scheduling with configurable thresholds.
- PI control with output clamping and conditional integration anti-windup.
- RPM calculation from tachometer frequency and configurable pulses per revolution.
- Startup/stall supervision and temperature-sensor plausibility checks.
- Host-side simulation and unit tests.
- Documented command protocol and hardware bring-up checklist.

## Repository layout

```text
firmware/
  core/       Portable control, thermal policy, RPM and fault logic
  platform/   STM32 HAL integration notes and adapter template
  app/        Minimal host demo
sim/
  fan_model.py
tests/
  test_control.c
  test_simulation.py
docs/
  architecture.md
  hardware.md
  test-plan.md
.github/workflows/ci.yml
Makefile
```

## Build and test on a computer

Requirements: GCC or Clang, Make, Python 3.10+, and pytest.

```bash
make test
make demo
python -m pip install -r requirements-dev.txt
pytest -q tests/test_simulation.py
```

The C tests and demo are hardware-independent. They are useful for continuous integration even when no STM32 board is attached.

## STM32 target and hardware

The intended target is an STM32 NUCLEO-F446RE using STM32CubeIDE / STM32 HAL. The exact timer channels, pin mapping, clock setup, and generated initialization depend on the board revision and the selected fan interface; document the actual pinout after configuring CubeMX.

See [hardware setup](docs/hardware.md) before wiring anything.

### Important electrical notes

- Do **not** power the fan from an MCU GPIO. Use a supply rated for the fan's voltage and startup current.
- Follow the exact 4-wire fan datasheet for PWM frequency, logic polarity, tachometer output type, and pulses per revolution. Many PC fans expect an open-collector/open-drain control signal; use a suitable transistor/interface where required.
- Verify the tachometer voltage and pull-up requirements before connecting it to an MCU pin.
- Connect signal grounds as required by the chosen interface; do not assume the tach output is 3.3 V compatible.
- The NTC divider, ADC range, resistor value, and conversion curve must be calibrated for the actual thermistor.
- This is a learning prototype, not a safety-certified thermal protection product. Never rely on it as the only cooling protection for critical hardware.

## Control design

### RPM

For tachometer frequency `f_t` in hertz and `P` pulses per revolution:

`RPM = 60 × f_t / P`

If measuring the interval between edges, ensure the period math uses the timer clock and handles counter wraparound. Filter noisy readings and mark the value invalid after a configurable timeout.

### PI controller

The portable controller uses the set-point error (target RPM minus measured RPM), bounded output, and anti-windup. Choose gains on a simulated plant first, then tune conservatively with the real fan. A fan's response is nonlinear and changes with supply voltage, airflow restriction, and load; simulator results are not hardware measurements.

### Thermal policy

The reference policy linearly interpolates between configured temperature/RPM points and clamps outside the range. Thresholds in examples are illustrative, not universal operating limits. Set them according to the equipment being cooled.

### Fault response

A missing or implausible temperature value should trigger a configured safe policy (typically request maximum cooling where safe and supported). A tachometer timeout while a nonzero speed is requested should raise a stall fault. Fault clearing must require valid recovery conditions; do not silently clear persistent faults.

## UART CLI proposal

The final board adapter can expose a line-oriented CLI such as:

```text
help
status
mode auto
mode manual
target 2400
duty 55
thermal show
faults
faults clear
```

Commands must validate input and return an explicit error for invalid values. The UART shell is a platform integration task; the portable core and host demo are built separately.

## Verification

Run `make test` and the Python simulation tests. The suite checks RPM conversion, output clamping, anti-windup behavior, thermal interpolation, invalid sensor handling, and deterministic simulator behavior.

Hardware evidence to add before calling the project fully complete:

- [ ] CubeMX configuration and exact pin map.
- [ ] Build log from the selected STM32CubeIDE/toolchain version.
- [ ] UART log showing measured RPM and duty response.
- [ ] Tachometer measurement compared with an independent reference.
- [ ] Temperature reading checked against a reference thermometer.
- [ ] Fan startup, disconnected tachometer, and sensor-fault tests.
- [ ] Measured step response (settling time, overshoot, and steady-state error).

## Limitations

- Host simulations do not prove timing, analog accuracy, EMI robustness, or electrical compatibility.
- The STM32 HAL adapter requires configuration for the specific CubeMX-generated project.
- Temperature conversion must be calibrated for the actual NTC and divider.
- PI gains and thermal limits must be tuned for the chosen fan and application.

## License

MIT — see [LICENSE](LICENSE).
