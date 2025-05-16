out vec4 frag_color;

uniform samplerCube sky;

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution.xy;
  frag_color = texture(sky, vec3(uv, 1));
}
