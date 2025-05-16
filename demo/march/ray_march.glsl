#ifndef RAY_MARCH_GLSL
#define RAY_MARCH_GLSL

#define MIN_DISTANCE 0.01
#define MAX_DISTANCE 1000.0
#define MAX_STEPS 64

struct trace_t {
  float d;
  int id;
};

trace_t trace(int id, float d) {
  trace_t tr;
  tr.id = id;
  tr.d = d;
  return tr;
}

trace_t tr_add(trace_t a, trace_t b) {
  return a.d < b.d ? a : b;
}

trace_t sdf(vec3 p);

vec3 sdf_normal(vec3 p) {
  float dp = 0.001;
  
  trace_t f = sdf(p);
  trace_t df_dx = sdf(p + vec3(dp, 0.0, 0.0));
  trace_t df_dy = sdf(p + vec3(0.0, dp, 0.0));
  trace_t df_dz = sdf(p + vec3(0.0, 0.0, dp));
  
  return normalize(vec3(df_dx.d, df_dy.d, df_dz.d) - f.d);
}

trace_t ray_march(vec3 ro, vec3 rd) {
  float td = 0.0;
  
  for (int i = 0; i < MAX_STEPS; i++) {
    trace_t tr = sdf(ro + rd * td);
    if (tr.d < MIN_DISTANCE) return trace(tr.id, td);
    if (td > MAX_DISTANCE) break;
    td += tr.d;
  }
  
  return trace(0, MAX_DISTANCE);
}

float sdf_union(float a, float b) {
  return min(a, b);
}

float sdf_sub(float a, float b) {
  return max(a, -(b - 0.05));
}

float sdf_and(float a, float b) {
  return max(a, b);
}

float sdf_smooth_union(float a, float b, float k) {
  float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
  return mix(b, a, h) - k*h*(1.0-h);
}

float sdf_cuboid(vec3 p, vec3 o, vec3 s) {
  s *= 0.5;
  o += s;
  vec3 d = abs(p - o) - s;
  return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

float sdf_sphere(vec3 p, vec3 o, float r) {
  return length(p - o) - r;
}

float sdf_plane(vec3 p, vec3 n, float d) {
  return dot(p, n) - d;
}

#endif
