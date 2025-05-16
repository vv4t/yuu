out vec4 frag_color;

void main() {
  frag_color = vec4(gl_FragCoord.xy / u_resolution.xy, 1.0, 1.0);
}
