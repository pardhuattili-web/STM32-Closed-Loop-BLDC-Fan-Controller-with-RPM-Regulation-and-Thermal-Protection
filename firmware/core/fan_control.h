#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float kp;
    float ki;
    float min_output_pct;
    float max_output_pct;
    float integral_min;
    float integral_max;
    float integral;
    float previous_error;
    bool initialized;
} fan_pi_t;

/* Return 60 * tach_hz / pulses_per_revolution. Invalid inputs return 0. */
float fan_rpm_from_frequency(float tach_hz, uint32_t pulses_per_revolution);

/* Update bounded PI controller. dt_seconds must be positive and finite. */
float fan_pi_update(fan_pi_t *controller,
                    float setpoint,
                    float measurement,
                    float dt_seconds);

void fan_pi_reset(fan_pi_t *controller);

/* Linear interpolation between strictly increasing temperature points. */
bool fan_thermal_target(float temperature_c,
                       const float *temperatures_c,
                       const float *rpm_points,
                       uint32_t point_count,
                       float *target_rpm);

/* Validate whether a temperature is physically plausible for a configured range. */
bool fan_temperature_is_valid(float temperature_c,
                              float minimum_c,
                              float maximum_c);

#ifdef __cplusplus
}
#endif

#endif
