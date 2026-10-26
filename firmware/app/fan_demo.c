#include "fan_control.h"
#include <stdio.h>
int main(void) {
    const float temperatures[] = {20,30,40,55};
    const float rpm_points[] = {800,1200,2000,3200};
    float target = 0;
    puts("Portable fan-control demo: simulated policy output, not hardware data");
    for (float temperature=20; temperature<=60; temperature+=5) {
        if (!fan_thermal_target(temperature,temperatures,rpm_points,4,&target)) return 1;
        printf("temperature=%5.1f C -> target=%6.1f RPM\n",temperature,target);
    }
    return 0;
}
