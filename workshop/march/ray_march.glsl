#ifndef RAY_MARCH_GLSL
#define RAY_MARCH_GLSL

#define MIN_DISTANCE 0.01
#define MAX_DISTANCE 1000.0
#define MAX_STEPS 64

struct hit_t {
  float d;
  int id;
};

hit_t map(vec3 p);

float cube(vec3 p, vec3 o, vec3 s) {
  vec3 d = abs(p - o) - s;
  return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

float sphere(vec3 p, vec3 o, float r) {
  return length(p - o) - r;
}

float plane(vec3 p, vec3 n, float d) {
  return dot(p, n) - d;
}

hit_t map_cmp(hit_t s, int id, float d) {
  if (d < s.d) {
    s.d = d;
    s.id = id;
  }
  
  return s;
}

vec3 map_normal(vec3 p) {
  float dp = 0.001;
  hit_t d = map(p);
  hit_t dx = map(p + vec3(dp, 0.0, 0.0));
  hit_t dy = map(p + vec3(0.0, dp, 0.0));
  hit_t dz = map(p + vec3(0.0, 0.0, dp));
  
  vec3 N = normalize(vec3(dx.d, dy.d, dz.d) - d.d);
  
  return N;
}

hit_t ray_march(vec3 ro, vec3 rd)
{
  hit_t td;
  td.d = 0.0;
  td.id = 0;
  
  for (int i = 0; i < MAX_STEPS; i++) {
    hit_t d = map(ro + rd * td.d);
    td.id = d.id;
    if (d.d < MIN_DISTANCE) return td;
    if (td.d > MAX_DISTANCE) break;
    td.d += d.d;
  }
  
  td.d = MAX_DISTANCE;
  td.id = 0;
  return td;
}

#endif
