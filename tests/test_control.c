#include "fan_control.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

static void test_rpm(void)
{
    assert(fabsf(fan_rpm_from_frequency(100.0f, 2U) - 3000.0f) < 0.01f);
    assert(fan_rpm_from_frequency(-1.0f, 2U) == 0.0f);
    assert(fan_rpm_from_frequency(100.0f, 0U) == 0.0f);
}

static void test_pi_clamping_and_anti_windup(void)
{
    fan_pi_t pi = {
        .kp = 0.1f, .ki = 0.5f,
        .min_output_pct = 20.0f, .max_output_pct = 100.0f,
        .integral_min = -1000.0f, .integral_max = 1000.0f,
        .integral = 0.0f, .previous_error = 0.0f, .initialized = false
    };
    float result = fan_pi_update(&pi, 5000.0f, 0.0f, 0.1f);
    assert(result <= 100.0f && result >= 20.0f);
    assert(pi.integral == 0.0f);
    result = fan_pi_update(&pi, 100.0f, 200.0f, 0.1f);
    assert(result >= 20.0f && result <= 100.0f);
    assert(fan_pi_update(&pi, 100.0f, 200.0f, 0.0f) == 0.0f);
}

static void test_thermal_interpolation(void)
{
    const float temps[] = {20.0f, 30.0f, 50.0f};
    const float rpms[] = {800.0f, 1200.0f, 2800.0f};
    float target = 0.0f;

    assert(fan_thermal_target(40.0f, temps, rpms, 3U, &target));
    assert(fabsf(target - 2000.0f) < 0.01f);
    assert(fan_thermal_target(10.0f, temps, rpms, 3U, &target));
    assert(fabsf(target - 800.0f) < 0.01f);
    assert(!fan_thermal_target(40.0f, temps, rpms, 1U, &target));
}

static void test_temperature_validation(void)
{
    assert(fan_temperature_is_valid(25.0f, -20.0f, 100.0f));
    assert(!fan_temperature_is_valid(150.0f, -20.0f, 100.0f));
    assert(!fan_temperature_is_valid(NAN, -20.0f, 100.0f));
}

int main(void)
{
    test_rpm();
    test_pi_clamping_and_anti_windup();
    test_thermal_interpolation();
    test_temperature_validation();
    puts("All fan-control tests passed.");
    return 0;
}
