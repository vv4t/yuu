#pragma use "../lib/pbr.glsl"
#pragma use "../lib/random.glsl"
#pragma use "../lib/ray_march_with_id.glsl"
#pragma use "../lib/math.glsl"

#define MAX_BOUNCE 5

out vec4 frag_color;
in vec2 frag_coord;

struct surface_t {
  vec3 albedo;
  vec3 normal;
  float radiance;
  float metalness;
  float roughness;
};

uniform sampler2D luminance;

hit_t map(vec3 p);
surface_t get_surface(int id);
surface_t surface_light(vec3 color, float radiance);
surface_t surface_solid(vec3 albedo, float metalness, float roughness);
vec3 get_next_ray(vec3 p, int i, vec3 N);

void main() {
  vec2 uv = frag_coord * 2.0 - 1.0;
  uv.x *= resolution.x / resolution.y;
  
  vec3 view_pos = vec3(0.0, -2.0, -5.0);
  mat4 view_mat = mat4(1.0) * rotate_y(-0.5);
  
  struct {
    vec3 L;
    vec3 V;
    vec3 N;
    int id;
  } stack[MAX_BOUNCE];
  
  vec3 radiance = vec3(0.0);
  vec3 p = view_pos;
  vec3 rd = normalize(vec3(view_mat * vec4(uv, 1.0, 1.0)));
  
  int i = -1;
  do {
    p += rd * 0.0625;
    hit_t td = ray_march(p, rd);
    p += rd * td.d;
    vec3 N = map_normal(p);
    surface_t sf = get_surface(td.id);
    
    if (sf.radiance > 0.0) {
      radiance = sf.albedo * sf.radiance;
      break;
    } else {
      i++;
      stack[i].V = -rd;
      stack[i].N = N;
      stack[i].id = td.id;
      rd = get_next_ray(p, i, N);
      stack[i].L = rd;
    }
  } while (i < MAX_BOUNCE);
  
  for (; i >= 0; i--) {
    vec3 L = stack[i].L;
    vec3 V = stack[i].V;
    vec3 N = stack[i].N;
    surface_t sf = get_surface(stack[i].id);
    
    float NdotL = clamp(dot(N, L), 0.0, 1.0);
    float pdf_BRDF = (1.0 / (2.0 * PI));
    vec3 p_BRDF = BRDF(sf.albedo, sf.metalness, sf.roughness, L, V, N);
    
    radiance = radiance * p_BRDF / pdf_BRDF * NdotL;
  }
  
  vec3 total = texture(luminance, frag_coord).rgb + radiance;
  
  frag_color = vec4(total, 1.0);
}

surface_t get_surface(int id) {
  if (id == 0) {
    return surface_solid(vec3(1.0, 1.0, 1.0), 0.1, 0.1);
  } else if (id == 1) {
    return surface_light(vec3(1.0, 1.0, 1.0), 3.0);
  } else if (id == 3) {
    return surface_light(vec3(1.0, 0.5, 1.0), 10.0);
  } else if (id == 2) {
    return surface_solid(vec3(0.4, 0.3, 1.0), 0.4, 0.1);
  }
}

hit_t map(vec3 p) {
  hit_t s;
  s.d = MAX_DISTANCE;
  s.id = 0;
  
  s = map_cmp(s, 0, plane(p, vec3(0, 0, -1), -12.0));
  s = map_cmp(s, 0, plane(p, vec3(0, 0, +1), -12.0));
  s = map_cmp(s, 1, plane(p, vec3(+1, 0, 0), -18.0));
  s = map_cmp(s, 0, plane(p, vec3(-1, 0, 0), -12.0));
  s = map_cmp(s, 0, plane(p, vec3(0, +1, 0), -6.0));
  s = map_cmp(s, 0, plane(p, vec3(0, -1, 0), -12.0));
  
  s = map_cmp(s, 0, cube(p, vec3(-1.0, -3.0, 12.0), vec3(1.0, 6.0, 1.0)));
  s = map_cmp(s, 0, cube(p, vec3(+7.0, -3.0, 12.0), vec3(1.0, 6.0, 1.0)));
  s = map_cmp(s, 0, cube(p, vec3( 3.0, 4.0, 12.0), vec3(5.0, 1.0, 1.0)));
  // s = map_cmp(s, 3, cube(p, vec3( 3.0, -1.0, 12.0), vec3(4.0, 6.0, 1.0)));
  
  s = map_cmp(s, 2, sphere(p, vec3(-2.0, -4.0, 4.0), 2.0));
  
  return s;
}

vec3 get_next_ray(vec3 p, int i, vec3 N) {
  vec3 v = p * 2404.4324 * seed + seed * 15043.4234 + rand(float(i + 1)) * 13242.4352;
  v = fract(sin(v) * 43758.5453123) * 2.0 - 1.0;
  return randomHemispherePoint(v, N);
}

surface_t surface_solid(vec3 albedo, float metalness, float roughness) {
  surface_t s;
  s.albedo = albedo;
  s.normal = vec3(0.0, 0.0, 1.0);
  s.metalness = metalness;
  s.roughness = roughness;
  s.radiance = 0.0;
  return s;
}

surface_t surface_light(vec3 color, float radiance) {
  surface_t s;
  s.albedo = color;
  s.radiance = radiance;
  return s;
}
