#ifndef RAY_MARCH_GLSL
#define RAY_MARCH_GLSL

#define MIN_DISTANCE 0.01
#define MAX_DISTANCE 1000.0
#define MAX_STEPS 64

float sdf(vec3 p);

vec3 sdf_normal(vec3 p) {
  float dp = 0.001;
  
  float f = sdf(p);
  float df_dx = sdf(p + vec3(dp, 0.0, 0.0));
  float df_dy = sdf(p + vec3(0.0, dp, 0.0));
  float df_dz = sdf(p + vec3(0.0, 0.0, dp));
  
  return normalize(vec3(df_dx, df_dy, df_dz) - f);
}

float ray_march(vec3 ro, vec3 rd) {
  float td = 0.0;
  
  for (int i = 0; i < MAX_STEPS; i++) {
    float d = sdf(ro + rd * td);
    if (d < MIN_DISTANCE) return d;
    if (td > MAX_DISTANCE) break;
    td += d;
  }
  
  return MAX_DISTANCE;
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
