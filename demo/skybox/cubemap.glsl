#pragma use "../util/math.glsl"

out vec4 frag_color;

uniform samplerCube sky;

void main() {
  vec2 uv = (gl_FragCoord.xy / iResolution.xy * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);
  
  vec2 view_rot = (iMouse.xy * 2.0 - 1.0) * 4.0;
  mat4 view_mat = mat4(1.0) * rotate_y(view_rot.x) * rotate_x(view_rot.y);
  
  frag_color = texture(sky, (view_mat * vec4(uv, 1.0, 1.0)).xyz);
}
