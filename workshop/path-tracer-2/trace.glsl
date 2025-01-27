#pragma use "../lib/pbr.glsl"
#pragma use "../lib/random.glsl"
#pragma use "../lib/ray_march_with_id.glsl"
#pragma use "../lib/math.glsl"
#pragma use "surface.glsl"
#pragma use "tile_surface.glsl"

#define MAX_BOUNCE 5

out vec4 frag_color;
in vec2 frag_coord;

uniform sampler2D luminance;

hit_t map(vec3 p);
surface_t get_surface(vec3 p, int id);
surface_t surface_light(vec3 color, float radiance);
surface_t surface_solid(vec3 albedo, float metalness, float roughness);
vec3 get_sample_seed(vec3 p, int i);
vec3 randomSpecularRay(vec3 seed, vec3 V, vec3 N, float roughness);

void main() {
  vec2 uv = frag_coord * 2.0 - 1.0;
  uv.x *= resolution.x / resolution.y;
  
  vec3 view_pos = vec3(3.0, 0.0, -9.0);
  mat4 view_mat = mat4(1.0) * rotate_y(-0.0) * rotate_x(-0.0);
  
  struct {
    vec3 luminance;
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
    surface_t sf = get_surface(p, td.id);
    
    if (i >= 0) stack[i].luminance *= exp(-td.d * 0.03);
    
    if (sf.radiance > 0.0) {
      radiance = sf.albedo * sf.radiance;
      break;
    } else {
      i++;
      
      vec3 sample_seed = get_sample_seed(p, i);
      bool do_reflect = sample_seed.x > 0.0;
      
      vec3 V = -rd;
      vec3 L = do_reflect ? randomSpecularRay(sample_seed, V, N, sf.roughness) : randomHemispherePoint(sample_seed, N);
      vec3 H = normalize(L + V);
      float pdf = do_reflect ? 1.0 : (1.0 / (2.0 * PI));
      
      float NdotL = clamp(dot(N, L), 0.0, 1.0);
      vec3 p_BRDF = BRDF(sf.albedo, sf.metalness, sf.roughness, L, V, N);
      
      stack[i].luminance = p_BRDF * NdotL / pdf;
      rd = L;
    }
  } while (i < MAX_BOUNCE);
  
  float energy = 1.0;
  
  for (; i >= 0; i--) {
    radiance = radiance * stack[i].luminance * energy;
    energy *= 0.8;
  }
  
  vec3 total = texture(luminance, frag_coord).rgb + radiance;
  
  frag_color = vec4(total, 1.0);
}

vec3 randomSpecularRay(vec3 seed, vec3 V, vec3 N, float roughness) {
  return normalize(randomSpherePoint(seed) + reflect(-V, N) * exp((1.0 - roughness) * 3.0));
}

surface_t get_surface(vec3 p, int id) {
  if (id == 0) {
    return surface_solid(vec3(1.0, 0.5, 1.0), 0.1, 0.4);
  } else if (id == 1) {
    return surface_light(vec3(0.7, 1.0, 1.0), 4.0);
  } else if (id == 2) {
    return surface_solid(vec3(0.7, 0.4, 0.6), 0.2, 0.5);
  } else if (id == 3) {
    return surface_light(vec3(1.0, 0.5, 0.2), 2.0);
  } else if (id == 4) {
    return surface_solid(vec3(1.0, 0.2, 0.2), 0.1, 0.5);
  } else if (id == 5) {
    return surface_tile(p);
  }
}

hit_t map(vec3 p) {
  hit_t s;
  s.d = MAX_DISTANCE;
  s.id = 0;
  
  s = map_cmp(s, 0, plane(p, vec3(0, 0, -1), -22.0));
  s = map_cmp(s, 5, plane(p, vec3(0, 0, +1), -12.0));
  s = map_cmp(s, 5, plane(p, vec3(+1, 0, 0), -12.0));
  s = map_cmp(s, 5, plane(p, vec3(-1, 0, 0), -12.0));
  s = map_cmp(s, 5, plane(p, vec3(0, +1, 0), -6.0));
  s = map_cmp(s, 1, plane(p, vec3(0, -1, 0), -12.0));
  
  s = map_cmp(s, 4, cube(p, vec3(-1.0, -3.0, 12.0), vec3(0.5, 2.0, 12.0)));
  s = map_cmp(s, 4, cube(p, vec3(+7.0, -3.0, 12.0), vec3(0.5, 2.0, 12.0)));
  s = map_cmp(s, 4, cube(p, vec3( 3.0, 4.0, 12.0), vec3(2.0, 0.5, 12.0)));
  // s = map_cmp(s, 3, cube(p, vec3( 3.0, -1.0, 12.0), vec3(4.0, 6.0, 1.0)));
  
  s = map_cmp(s, 2, sphere(p, vec3(1.0, -4.0, 2.0), 2.0));
  s = map_cmp(s, 4, sphere(p, vec3(3.0, -4.0, 7.0), 2.0));
  
  return s;
}

vec3 get_sample_seed(vec3 p, int i) {
  vec3 v = p * 2404.4324 * seed + seed * 15043.4234 + rand(float(i + 1)) * 13242.4352;
  return fract(sin(v) * 43758.5453123) * 2.0 - 1.0;
}
