#ifndef LIGHT_GLSL
#define LIGHT_HLSL

#pragma use "pbr.glsl"

vec3 light_pos[] = vec3[](
  vec3(3.0, 2.0, -2.0),
  vec3(-3.0, 2.0, -2.0)
);

vec3 light_color[] = vec3[](
  vec3(0.5, 1.0, 1.0) * 40.0,
  vec3(1.0, 0.5, 1.0) * 40.0
);

#define NUM_LIGHT 2

vec3 calc_point_scatter(vec3 frag_pos, vec3 view_pos)
{
  vec3 light = vec3(0.0);
  
  for (int i = 0; i < NUM_LIGHT; i++) {
    vec3 delta_pos = light_pos[i] - frag_pos;
    vec3 light_dir = normalize(delta_pos);
    
    vec3 view_dir = normalize(frag_pos - view_pos);
    
    vec3 dir = normalize(light_dir - view_dir * dot(light_dir, view_dir));
    float h = dot(light_pos[i], dir) - dot(view_pos, dir);
    float c = dot(light_pos[i], view_dir);
    float a = dot(view_pos, view_dir) - c;
    float b = dot(frag_pos, view_dir) - c;
    float fog = atan(b / h) / h - atan(a / h) / h;
    
    light += (0.0001 * fog * fog + 0.001 * fog) * light_color[i];
  }
  
  return light;
}

vec3 calc_point_lighting(vec3 p, vec3 V, vec3 N, vec3 albedo, float metallic, float roughness)
{
  vec3 light = vec3(0.0);
  
  for (int i = 0; i < NUM_LIGHT; i++) {
    vec3 L = normalize(light_pos[i] - p);
    float distance = length(light_pos[i] - p);
    
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light_color[i] * attenuation;
    
    light += BRDF(albedo, metallic, roughness, L, V, N) * radiance;
  }
  
  return light;
}

#endif
