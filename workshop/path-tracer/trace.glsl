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

#define TILE 4.0

vec3 get_albedo(vec2 uv)
{
  float alpha = 0.0;
  alpha += min(cos((uv.x * TILE * 4.0 + 1.0) * M_PI) + 0.95, 0.0);
  alpha += min(cos((uv.y * TILE * 4.0 + 1.0) * M_PI) + 0.95, 0.0);
  return vec3(1.0 + alpha * 10.0);
}

float get_roughness(vec2 uv)
{
  float alpha = 0.0;
  
  if (fract(uv.y * TILE) > 0.5) {
    alpha = (fract(uv.x * TILE) > 0.5 ? 0.18 : 0.20);
  } else {
    alpha = (fract(uv.x * TILE) < 0.5 ? 0.18 : 0.20);
  }
  
  return alpha;
}

float get_height(vec2 uv)
{
  float alpha = 0.0;
  alpha += min(cos((uv.x * TILE * 4.0 + 1.0) * M_PI) + 0.95, 0.0);
  alpha += min(cos((uv.y * TILE * 4.0 + 1.0) * M_PI) + 0.95, 0.0);
  return alpha;
}

vec3 get_normal(vec2 uv)
{
  float u = get_height(uv);
  float du_x = get_height(uv + vec2(0.001, 0.0)) - u;
  float du_y = get_height(uv + vec2(0.0, 0.001)) - u;
  float s = 0.05;
  return normalize(cross(vec3(s, 0.0, du_x), vec3(0.0, s, du_y)));
}

surface_t surface_tile(vec3 p) {
  mat3 p_TBN = map_TBN(p);
  vec2 uv = (inverse(p_TBN) * p).xy * 0.2;
  
  surface_t s;
  s.albedo = get_albedo(uv);
  s.normal = get_normal(uv);
  s.metalness = 0.1;
  s.roughness = get_roughness(uv);
  s.radiance = 0.0;
  return s;
}

surface_t get_surface(int id, vec3 p) {
  if (id == 0) {
    return surface_tile(p);
  } else if (id == 1) {
    return surface_light(vec3(0.5, 1.0, 1.0), 5.0);
  } else if (id == 3) {
    return surface_light(vec3(1.0, 0.5, 1.0), 2.0);
  } else if (id == 2) {
    return surface_solid(vec3(0.4, 0.3, 1.0), 0.4, 0.1);
  }
}

/*
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
  
  s = map_cmp(s, 0, cube(p, vec3(-12.0, 0.0, 4.0), vec3(1.0, 12.0, 2.0)));
  // s = map_cmp(s, 0, cube(p, vec3(-12.0, 0.0, 0.0), vec3(1.0, 12.0, 1.0)));
  s = map_cmp(s, 0, cube(p, vec3(-12.0, 0.0, -4.0), vec3(1.0, 12.0, 2.0)));
  
  s = map_cmp(s, 3, cube(p, vec3( 3.0, -1.0, 12.8), vec3(4.0, 6.0, 1.0)));
  // s = map_cmp(s, 0, cube(p, vec3( 0.0, 12.0, 1.0), vec3(4.0, 0.5, 4.0)));
  
  s = map_cmp(s, 2, sphere(p, vec3(-2.0, -4.0, 4.0), 2.0));
  
  return s;
}
*/

hit_t map(vec3 p) {
  hit_t s;
  s.d = MAX_DISTANCE;
  
  s = map_cmp(s, 0, max(plane(p, vec3(0, 0, -1), -12.0), -cube(p, vec3(0.0, -2.0, 64.0), vec3(4.0, 6.0, 128.0))));
  s = map_cmp(s, 0, plane(p, vec3(0, 0, +1), -6.0));
  s = map_cmp(s, 0, plane(p, vec3(+1, 0, 0), -6.0));
  s = map_cmp(s, 0, plane(p, vec3(-1, 0, 0), -6.0));
  s = map_cmp(s, 0, plane(p, vec3(0, +1, 0), -6.0));
  s = map_cmp(s, 0, max(plane(p, vec3(0, -1, 0), -6.0), -cube(p, vec3(0.0, 6.0, 7.0), vec3(3.0, 12.0, 3.0))));
  s = map_cmp(s, 1, cube(p, vec3(0.0, 9.0, 7.0), vec3(3.0, 1.0, 3.0)));
  
  // s = map_cmp(s, 3, cube(p, vec3(0.0, -6.0, 7.0), vec3(1.0, 0.125, 4.0)));
  
  return s;
}

uniform sampler2D luminance;

void main() {
  vec2 uv = frag_coord * 2.0 - 1.0;
  uv.x *= resolution.x / resolution.y;
  vec2 mp = mouse * 2.0 - 1.0;
  
  vec3 view_pos = vec3(0.0, -2.0, -5.0);
  mat4 view_mat = mat4(1.0);
  
  struct {
    vec3 L;
    vec3 V;
    vec3 p;
    int id;
  } stack[MAX_BOUNCE];
  
  vec3 rand_p = vec3(vec2(rand(time), rand(time * time)) * 2.0 - 1.0, 0.0);
  vec3 rd = normalize(vec3(view_mat * vec4(uv, 1.0, 1.0)));
  hit_t td = ray_march(view_pos, rd);
  
  vec3 p = view_pos + rd * td.d;
  surface_t sf = get_surface(td.id, p);
  vec3 N = map_normal(p);
  vec3 radiance = sf.albedo * sf.radiance;
  
  if (sf.radiance <= 0.0) {
    stack[0].id = td.id;
    stack[0].V = -rd;
    stack[0].p = p;
    
    int i = 1;
    for (; i < MAX_BOUNCE; i++) {
      vec3 old_rd = rd;
      rd = randomHemispherePoint(
        vec3(
          rand((p.x * (seed.x * 2.0 - 1.0) * float(i)) * 123.0) * 2.0 - 1.0,
          rand((p.y * (seed.y * 2.0 - 1.0) * float(i)) * 456.0) * 2.0 - 1.0,
          rand((p.z * (seed.z * 2.0 - 1.0) * float(i)) * 789.0) * 2.0 - 1.0
        ), N
      );
      
      // rd = normalize(rd + reflect(old_rd, N) * rand((p.x * (seed.z * 2.0 - 1.0) * float(i)) * 789.0) * 4.0);
      
      p += rd * 0.0625;
      td = ray_march(p, rd);
      p += rd * td.d;
      sf = get_surface(td.id, p);
      N = map_normal(p);
      radiance = sf.albedo * sf.radiance;
      
      stack[i - 1].L = rd;
      stack[i].V = -rd;
      stack[i].id = td.id;
      stack[i].p = p;
      
      if (sf.radiance > 0.0) {
        break;
      }
    }
    
    float energy = 1.0;
    for (int j = i - 1; j >= 0; j--) {
      vec3 p = stack[j].p;
      mat3 p_TBN = map_TBN(p);
      vec3 L = stack[j].L;
      vec3 V = stack[j].V;
      int id = stack[j].id;
      sf = get_surface(id, p);
      vec3 N = p_TBN * sf.normal;
      
      float pdf_BRDF = (1.0 / (2.0 * PI));
      vec3 p_BRDF = BRDF(sf.albedo, sf.metalness, sf.roughness, L, V, N);
      
      radiance = radiance * p_BRDF / pdf_BRDF * dot(N, L) * energy;
      energy *= 0.8;
    }
  } else {
    radiance = radiance / (1.0 / (2.0 * PI));
  }
  
  vec3 total = texture(luminance, frag_coord).rgb + radiance;
  
  frag_color = vec4(total, 1.0);
}
