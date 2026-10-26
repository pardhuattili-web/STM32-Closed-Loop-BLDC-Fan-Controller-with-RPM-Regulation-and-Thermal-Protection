#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H
#include <stdbool.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { float kp, ki, min_output_pct, max_output_pct; float integral_min, integral_max, integral, previous_error; bool initialized; } fan_pi_t;
float fan_rpm_from_frequency(float tach_hz, uint32_t pulses_per_revolution);
float fan_pi_update(fan_pi_t *controller, float setpoint, float measurement, float dt_seconds);
void fan_pi_reset(fan_pi_t *controller);
bool fan_thermal_target(float temperature_c, const float *temperatures_c, const float *rpm_points, uint32_t point_count, float *target_rpm);
bool fan_temperature_is_valid(float temperature_c, float minimum_c, float maximum_c);
#ifdef __cplusplus
}
#endif
#endif
