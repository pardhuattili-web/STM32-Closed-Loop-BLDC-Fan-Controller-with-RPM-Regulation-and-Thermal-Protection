#include "fan_control.h"
#include <stdio.h>
int main(void){const float t[]={20,30,40,55},r[]={800,1200,2000,3200};float target=0;puts("Portable fan-control demo: illustrative policy output, not hardware measurements");for(float c=20;c<=60;c+=5){if(!fan_thermal_target(c,t,r,4,&target))return 1;printf("temperature=%5.1f C -> target=%6.1f RPM\n",c,target);}return 0;}
