#define MIN_DISTANCE 0.01
#define MAX_DISTANCE 1000.0
#define MAX_STEPS 64

float map(vec3 p);

float cube(vec3 p, vec3 o, vec3 s)
{
  vec3 d = abs(p - o) - s;
  return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

float plane(vec3 p, vec3 n, float d)
{
  return dot(p, n) - d;
}

float sphere(vec3 p, vec3 o, float r)
{
  return length(p - o) - r;
}

float ray_march(vec3 ro, vec3 rd)
{
  float td = 0.0;
  for (int i = 0; i < MAX_STEPS; i++) {
    float d = map(ro + rd * td);
    if (d < MIN_DISTANCE) return td;
    if (td > MAX_DISTANCE) break;
    td += d;
  }
  return MAX_DISTANCE;
}

vec3 map_normal(vec3 p)
{
  float dp = 0.001;
  float d = map(p);
  float dx = map(p + vec3(dp, 0.0, 0.0));
  float dy = map(p + vec3(0.0, dp, 0.0));
  float dz = map(p + vec3(0.0, 0.0, dp));
  
  vec3 N = normalize(vec3(dx, dy, dz) - d);
  
  return N;
}

float shadow(vec3 pt, vec3 rd, float ld)
{
  vec3 p = pt;
  float td = MIN_DISTANCE;
  float kd = 1.0;
  
  for (int i = 0; i < MAX_STEPS && kd > 0.01; i++) {
    p = pt + rd * td;
    
    float d = map(p);
    
    if (td > MAX_DISTANCE || td + d > ld) break;
    if (d < 0.001) kd = 0.0;
    // else kd = min(kd, 16.0 * d / td);
    
    td += d;
  }
  
  return kd;
}
