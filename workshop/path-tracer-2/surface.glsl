#ifndef SURFACE_GLSL
#define SURFACE_GLSL

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

#endif
