#ifndef HDR_GLSL
#define HDR_GLSL

vec3 hdr_light(vec3 light, float gamma, float exposure) {
  vec3 color = light;
  color = vec3(1.0) - exp(-color * exposure);
  color = pow(color, vec3(1.0 / gamma));
  return color;
}

#endif
