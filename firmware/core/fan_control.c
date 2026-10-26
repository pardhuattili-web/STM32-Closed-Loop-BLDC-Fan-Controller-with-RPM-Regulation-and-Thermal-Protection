#include "fan_control.h"
#include <math.h>
#include <stddef.h>
static float clampf_local(float x,float lo,float hi){return x<lo?lo:(x>hi?hi:x);}
float fan_rpm_from_frequency(float hz,uint32_t ppr){if(!isfinite(hz)||hz<0||ppr==0U)return 0.0f;return 60.0f*hz/(float)ppr;}
void fan_pi_reset(fan_pi_t *c){if(c){c->integral=0;c->previous_error=0;c->initialized=false;}}
float fan_pi_update(fan_pi_t *c,float sp,float pv,float dt){float e,ci,u,y;if(!c||!isfinite(sp)||!isfinite(pv)||!isfinite(dt)||dt<=0||!isfinite(c->kp)||!isfinite(c->ki)||!isfinite(c->min_output_pct)||!isfinite(c->max_output_pct)||!isfinite(c->integral_min)||!isfinite(c->integral_max)||c->integral_min>c->integral_max||c->min_output_pct>c->max_output_pct)return 0;
e=sp-pv;ci=clampf_local(c->integral+e*dt,c->integral_min,c->integral_max);u=c->kp*e+c->ki*ci;y=clampf_local(u,c->min_output_pct,c->max_output_pct);
/* Conditional integration prevents wind-up at saturated output. */
if(u==y||(u>c->max_output_pct&&e<0)||(u<c->min_output_pct&&e>0))c->integral=ci;c->previous_error=e;c->initialized=true;return y;}
bool fan_thermal_target(float t,const float *ts,const float *rs,uint32_t n,float *out){uint32_t i;if(!out||!ts||!rs||n<2U||!isfinite(t))return false;for(i=0;i<n;i++){if(!isfinite(ts[i])||!isfinite(rs[i])||rs[i]<0||(i>0&&ts[i]<=ts[i-1]))return false;}if(t<=ts[0]){*out=rs[0];return true;}if(t>=ts[n-1]){*out=rs[n-1];return true;}for(i=1;i<n;i++)if(t<=ts[i]){float f=(t-ts[i-1])/(ts[i]-ts[i-1]);*out=rs[i-1]+f*(rs[i]-rs[i-1]);return true;}return false;}
bool fan_temperature_is_valid(float t,float lo,float hi){return isfinite(t)&&isfinite(lo)&&isfinite(hi)&&lo<hi&&t>=lo&&t<=hi;}
