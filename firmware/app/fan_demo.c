#include "fan_control.h"

#include <stdio.h>

int main(void)
{
    const float temperatures[] = {20.0f, 30.0f, 40.0f, 55.0f};
    const float rpm_points[] = {800.0f, 1200.0f, 2000.0f, 3200.0f};
    float target = 0.0f;

    puts("Fan controller portable-core demo (host simulation, not hardware data)");
    for (float temperature = 20.0f; temperature <= 60.0f; temperature += 5.0f) {
        if (!fan_thermal_target(temperature, temperatures, rpm_points, 4U, &target)) {
            fputs("thermal policy configuration invalid\n", stderr);
            return 1;
        }
        printf("temperature=%5.1f C -> target=%6.1f RPM\n", temperature, target);
    }
    return 0;
}
