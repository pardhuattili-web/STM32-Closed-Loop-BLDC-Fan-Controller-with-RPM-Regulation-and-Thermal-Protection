# Test plan

## Host tests

| Test | Expected result |
|---|---|
| Tach frequency / pulses per revolution | RPM matches formula |
| Negative frequency or zero pulses/rev | Safe zero result |
| PI saturation | Output remains within limits |
| Upper saturation with positive error | Integral does not wind up |
| Thermal interpolation | Correct linear target |
| Values beyond schedule | Endpoint clamping |
| Invalid point count / non-increasing temperatures | Rejected configuration |
| NaN or out-of-range temperature | Invalid measurement |
| Fan model step response | Converges toward commanded RPM |
| Invalid simulation input | Python raises ValueError |

## Hardware tests (not yet run)
Record MCU board/revision, IDE/compiler version, clock/timer settings, fan model, pulses per revolution, PWM frequency/duty range, RPM error at several targets, temperature error, startup time, stall-detection time and test conditions. Do not enter invented measurements; mark each test PASS/FAIL/NOT RUN and attach evidence.
