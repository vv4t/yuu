out vec4 frag_color;

uniform sampler2D image;

void main() {
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  frag_color = texture(image, uv);
}
