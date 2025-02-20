out vec4 frag_color;

uniform sampler2D thing;

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution.xy;
  frag_color = texture(thing, uv);
}
