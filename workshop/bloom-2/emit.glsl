#pragma use "../lib/ray_march.glsl"
#pragma use "../lib/math.glsl"

out vec4 frag_color;
in vec2 frag_coord;

void main() {
  vec2 uv = frag_coord * 2.0 - 1.0;
  uv.x *= resolution.x / resolution.y;
  vec2 mp = mouse * 2.0 - 1.0;
  
  float t = cos(time * 0.1 + M_PI / 2.0) * M_PI / 4.0;
  vec3 view_pos = vec3(cos(t) * 2.0, -2.0 + cos(time * 0.05) * 2.0, -3.0 + sin(t) * 2.0);
  mat4 view_mat = mat4(1.0) * rotate_y(mp.x * 1.5) * rotate_x(mp.y * 1.5);
  
  vec3 rd = normalize(vec3(view_mat * vec4(uv, 1.0, 1.0)));
  float td = ray_march(view_pos, rd);
  
  if (td < MAX_DISTANCE) {
    frag_color.xyz = vec3(1.0, 3.0, 3.0) * (1.0 + 0.05 * cos(time * 2.0));
  } else {
    frag_color.xyz = vec3(0.0);
  }
  
  frag_color.w = 1.0;
}

float map(vec3 p) {
  return cube(p, vec3(0.0, -2.0, 6.0), vec3(2.0, 4.0, 0.01));
}
