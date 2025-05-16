out vec4 frag_color;

uniform sampler2D buffer1;

void main() {
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  frag_color = texture(buffer1, uv) * pow(cos(iTime), 2.0);
}
