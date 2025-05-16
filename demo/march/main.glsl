#pragma use "../util/math.glsl"
#pragma use "ray_march.glsl"

out vec4 frag_color;
uniform samplerCube sky;

void main() {
  vec2 uv = (gl_FragCoord.xy / iResolution.xy * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);
  
  vec2 view_rot = (iMouse.xy * 2.0 - 1.0) * 4.0;
  vec3 view_pos = vec3(0.0);
  mat4 view_mat = mat4(1.0) * rotate_y(view_rot.x) * rotate_x(view_rot.y);
  vec3 rd = normalize((view_mat * vec4(uv, 1.0, 1.0)).xyz);
  
  trace_t tr = ray_march(view_pos, rd);
  
  vec3 color = texture(sky, rd).xyz;
  if (tr.id == 1) {
    color = vec3(1.0 / tr.d);
  } else if (tr.id == 2) {
    color = vec3(1.0 / tr.d) * vec3(1.0, 0.5, 1.0);
  }
  
  frag_color = vec4(color, 1.0);
}

trace_t sdf(vec3 p) {
  trace_t tr = trace(0, MAX_DISTANCE);
  
  float s1 = sdf_sub(
    sdf_cuboid(p, vec3(0.0, -2.0, 3.0), vec3(4.0, 1.0, 1.0)),
    sdf_cuboid(p, vec3(0.0, -1.5, 3.0), vec3(1.0))
  );
  
  float s2 = sdf_smooth_union(
    sdf_cuboid(p, vec3(2.75, -2.0, -0.25), vec3(0.5, 3.0, 0.5)),
    sdf_sphere(p, vec3(3.0, -2.0, 0.0), 1.0),
    pow(cos(iTime), 2.0)
  );
  
  tr = tr_add(tr, trace(1, s1));
  tr = tr_add(tr, trace(2, s2));
  
  return tr;
}
