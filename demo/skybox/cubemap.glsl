out vec4 fragColor;

uniform samplerCube sky;

void main() {
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  fragColor = texture(sky, vec3(uv, 1));
}
