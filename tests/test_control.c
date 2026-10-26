#include "fan_control.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void){fan_pi_t c={.kp=.1f,.ki=.5f,.min_output_pct=20,.max_output_pct=100,.integral_min=-1000,.integral_max=1000};float target;const float t[]={20,30,50},r[]={800,1200,2800};assert(fabsf(fan_rpm_from_frequency(100,2)-3000)<.01f);assert(fan_rpm_from_frequency(-1,2)==0);assert(fan_rpm_from_frequency(100,0)==0);assert(fan_pi_update(&c,5000,0,.1f)==100);assert(c.integral==0);assert(fan_pi_update(&c,100,200,.1f)>=20);assert(fan_pi_update(&c,100,200,0)==0);fan_pi_reset(&c);assert(c.integral==0&&!c.initialized);assert(fan_thermal_target(40,t,r,3,&target)&&fabsf(target-2000)<.01f);assert(fan_thermal_target(10,t,r,3,&target)&&target==800);assert(!fan_thermal_target(40,t,r,1,&target));assert(fan_temperature_is_valid(25,-20,100));assert(!fan_temperature_is_valid(NAN,-20,100));puts("All fan-control tests passed.");return 0;}
