out vec4 frag_color;

void main() {
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  vec3 color = vec3(0.0);
  
  if (length(uv - pos) < size) {
    color = vec3(1.0);
  }
  
  frag_color = vec4(color, 1.0);
}
