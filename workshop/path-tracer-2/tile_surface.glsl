#ifndef TILE_MATERIAL
#define TILE_MATERIAL

#pragma use "surface.glsl"

#define TILE 10.0

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
    alpha = (fract(uv.x * TILE) > 0.5 ? 0.45 : 0.5);
  } else {
    alpha = (fract(uv.x * TILE) < 0.5 ? 0.45 : 0.5);
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

#endif
