out vec4 fragColor;

void main() {
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  float a = 0.01 / length(uv - iMouse.xy);
  float b = 0.01 / length(uv - abs(iMouse.zw));
  fragColor = vec4(a + b + (iMouse.z < 0.0 ? 0.3 : 0.0));
}
