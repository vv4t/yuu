out vec4 frag_color;

uniform sampler2D first;
uniform sampler2D second;

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution;
  frag_color.rgb = mix(texture(first, uv).rgb, texture(second, uv).rgb, 0.5);
  frag_color.w = 1.0;
}
