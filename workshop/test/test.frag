out vec4 frag_color;

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution;
  frag_color.xyz = vec3(uv, 1.0);
  frag_color.w = 1.0;
}
