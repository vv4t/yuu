out vec4 frag_color;

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution.xy;
  frag_color = vec4(uv, 1.0, 1.0);
}
