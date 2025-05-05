#pragma use "../lib/ray_march.glsl"
#pragma use "../lib/math.glsl"

out vec4 frag_color;

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution * 2.0 - 1.0;
  uv.x *= u_resolution.x / u_resolution.y;
  
  vec3 view_pos = vec3(0.0, -3.0, -5.0);
  mat4 view_mat = mat4(1.0);
  
  vec3 rd = normalize(vec3(view_mat * vec4(uv, 1.0, 1.0)));
  float td = ray_march(view_pos, rd);
  
  if (td < MAX_DISTANCE) {
    frag_color.xyz = vec3(1.0, 3.0, 3.0) * (1.0 + 0.05 * cos(u_time * 2.0));
  } else {
    frag_color.xyz = vec3(0.0);
  }
  
  frag_color.w = 1.0;
}

float map(vec3 p) {
  return cube(p, vec3(0.0, -2.0, 6.0), vec3(2.0, 4.0, 0.01));
}
