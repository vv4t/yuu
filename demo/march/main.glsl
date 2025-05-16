#pragma use "../util/math.glsl"
#pragma use "ray_march.glsl"

out vec4 frag_color;
uniform samplerCube sky;

void main() {
  vec2 uv = (gl_FragCoord.xy / iResolution.xy * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);
  
  vec2 view_rot = (iMouse.xy * 2.0 - 1.0) * 4.0;
  vec3 view_pos = vec3(0.0);
  mat4 view_mat = mat4(1.0) * rotate_y(view_rot.x) * rotate_x(view_rot.y);
  vec3 rd = (view_mat * vec4(uv, 1.0, 1.0)).xyz;
  
  float td = ray_march(view_pos, rd);
  
  vec3 color = texture(sky, rd).xyz;
  if (td < MAX_DISTANCE) {
    color = vec3(1.0);
  }
  
  frag_color = vec4(color, 1.0);
}

float sdf(vec3 p) {
  return sdf_cuboid(p, vec3(0.0, 0.0, 3.0), vec3(2.0, 1.0, 1.0));
}
