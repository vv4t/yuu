#ifndef RAY_MARCH_GLSL
#define RAY_MARCH_GLSL

#define MIN_DISTANCE 0.01
#define MAX_DISTANCE 1000.0
#define MAX_STEPS 64

#pragma use "trace.glsl"

trace_t sdf(vec3 p);

vec3 sdf_get_normal(vec3 p) {
  float dp = 0.001;
  trace_t d = sdf(p);
  trace_t dx = sdf(p + vec3(dp, 0.0, 0.0));
  trace_t dy = sdf(p + vec3(0.0, dp, 0.0));
  trace_t dz = sdf(p + vec3(0.0, 0.0, dp));
  
  vec3 N = normalize(vec3(dx.d, dy.d, dz.d) - d.d);
  
  return N;
}

trace_t ray_march(vec3 ro, vec3 rd)
{
  float td = 0.0;
  
  for (int i = 0; i < MAX_STEPS; i++) {
    trace_t tr = sdf(ro + rd * td);
    if (tr.d < MIN_DISTANCE) return tr;
    if (td > MAX_DISTANCE) break;
    td += tr.d;
  }
  
  return new_trace(0, MAX_DISTANCE);
}

#endif
