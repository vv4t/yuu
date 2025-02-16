#ifndef SDF_GLSL
#define SDF_GLSL

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
