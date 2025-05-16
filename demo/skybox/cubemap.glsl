#pragma use "../util/math.glsl"

out vec4 fragColor;

uniform samplerCube sky;

void main() {
  vec2 uv = (gl_FragCoord.xy / iResolution.xy * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);
  
  vec2 viewRotation = (iMouse.xy * 2.0 - 1.0) * 4.0;
  mat4 viewMatrix = mat4(1.0) * rotateY(viewRotation.x) * rotateX(viewRotation.y);
  
  fragColor = texture(sky, (viewMatrix * vec4(uv, 1.0, 1.0)).xyz);
}
