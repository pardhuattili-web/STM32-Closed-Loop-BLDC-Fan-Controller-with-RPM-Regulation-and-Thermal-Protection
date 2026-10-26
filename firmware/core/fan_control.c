#include "fan_control.h"
#include <math.h>
#include <stddef.h>

static float clampf_local(float value, float low, float high)
{
    if (value < low) return low;
    if (value > high) return high;
    return value;
}

float fan_rpm_from_frequency(float tach_hz, uint32_t pulses_per_revolution)
{
    if (!isfinite(tach_hz) || tach_hz < 0.0f || pulses_per_revolution == 0U) {
        return 0.0f;
    }
    return (60.0f * tach_hz) / (float)pulses_per_revolution;
}

void fan_pi_reset(fan_pi_t *controller)
{
    if (controller == NULL) return;
    controller->integral = 0.0f;
    controller->previous_error = 0.0f;
    controller->initialized = false;
}

float fan_pi_update(fan_pi_t *controller,
                    float setpoint,
                    float measurement,
                    float dt_seconds)
{
    float error;
    float candidate_integral;
    float output;
    float unsaturated;

    if (controller == NULL ||
        !isfinite(setpoint) ||
        !isfinite(measurement) ||
        !isfinite(dt_seconds) ||
        dt_seconds <= 0.0f ||
        !isfinite(controller->kp) ||
        !isfinite(controller->ki) ||
        !isfinite(controller->min_output_pct) ||
        !isfinite(controller->max_output_pct) ||
        !isfinite(controller->integral_min) ||
        !isfinite(controller->integral_max) ||
        controller->integral_min > controller->integral_max ||
        controller->min_output_pct > controller->max_output_pct) {
        return 0.0f;
    }

    error = setpoint - measurement;
    candidate_integral = clampf_local(controller->integral + error * dt_seconds,
                                      controller->integral_min,
                                      controller->integral_max);
    unsaturated = controller->kp * error + controller->ki * candidate_integral;
    output = clampf_local(unsaturated,
                          controller->min_output_pct,
                          controller->max_output_pct);

    /* Conditional integration prevents wind-up while output is saturated. */
    if (unsaturated == output ||
        (unsaturated > controller->max_output_pct && error < 0.0f) ||
        (unsaturated < controller->min_output_pct && error > 0.0f)) {
        controller->integral = candidate_integral;
    }

    controller->previous_error = error;
    controller->initialized = true;
    return output;
}

bool fan_thermal_target(float temperature_c,
                        const float *temperatures_c,
                        const float *rpm_points,
                        uint32_t point_count,
                        float *target_rpm)
{
    uint32_t i;

    if (target_rpm == NULL || temperatures_c == NULL || rpm_points == NULL ||
        point_count < 2U || !isfinite(temperature_c)) {
        return false;
    }

    for (i = 0U; i < point_count; ++i) {
        if (!isfinite(temperatures_c[i]) || !isfinite(rpm_points[i]) ||
            rpm_points[i] < 0.0f ||
            (i > 0U && temperatures_c[i] <= temperatures_c[i - 1U])) {
            return false;
        }
    }

    if (temperature_c <= temperatures_c[0]) {
        *target_rpm = rpm_points[0];
        return true;
    }
    if (temperature_c >= temperatures_c[point_count - 1U]) {
        *target_rpm = rpm_points[point_count - 1U];
        return true;
    }

    for (i = 1U; i < point_count; ++i) {
        if (temperature_c <= temperatures_c[i]) {
            const float fraction = (temperature_c - temperatures_c[i - 1U]) /
                                   (temperatures_c[i] - temperatures_c[i - 1U]);
            *target_rpm = rpm_points[i - 1U] +
                          fraction * (rpm_points[i] - rpm_points[i - 1U]);
            return true;
        }
    }
    return false;
}

bool fan_temperature_is_valid(float temperature_c,
                              float minimum_c,
                              float maximum_c)
{
    return isfinite(temperature_c) &&
           isfinite(minimum_c) &&
           isfinite(maximum_c) &&
           minimum_c < maximum_c &&
           temperature_c >= minimum_c &&
           temperature_c <= maximum_c;
}
