#include "fan_control.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
static void test_rpm(void) {
    assert(fabsf(fan_rpm_from_frequency(100.0f, 2U) - 3000.0f) < 0.01f);
    assert(fan_rpm_from_frequency(-1.0f, 2U) == 0.0f);
    assert(fan_rpm_from_frequency(100.0f, 0U) == 0.0f);
}
static void test_pi(void) {
    fan_pi_t pi = {.kp=.1f,.ki=.5f,.min_output_pct=20,.max_output_pct=100,
                   .integral_min=-1000,.integral_max=1000,.integral=0};
    float value = fan_pi_update(&pi, 5000, 0, .1f);
    assert(value <= 100 && value >= 20);
    assert(pi.integral == 0); /* Upper saturation must not wind up. */
    value = fan_pi_update(&pi, 100, 200, .1f);
    assert(value >= 20 && value <= 100);
    assert(fan_pi_update(&pi, 100, 200, 0) == 0);
    fan_pi_reset(&pi);
    assert(pi.integral == 0 && !pi.initialized);
}
static void test_thermal(void) {
    const float temp[] = {20,30,50}, rpm[] = {800,1200,2800};
    float target = 0;
    assert(fan_thermal_target(40,temp,rpm,3,&target));
    assert(fabsf(target-2000)<.01f);
    assert(fan_thermal_target(10,temp,rpm,3,&target) && target==800);
    assert(!fan_thermal_target(40,temp,rpm,1,&target));
    { const float bad[] = {20,20}; const float speeds[] = {500,900};
      assert(!fan_thermal_target(25,bad,speeds,2,&target)); }
}
static void test_temperature(void) {
    assert(fan_temperature_is_valid(25,-20,100));
    assert(!fan_temperature_is_valid(150,-20,100));
    assert(!fan_temperature_is_valid(NAN,-20,100));
}
int main(void) {
    test_rpm(); test_pi(); test_thermal(); test_temperature();
    puts("All fan-control tests passed."); return 0;
}
