#pragma use "../lib/math.glsl"
#pragma use "ray_march.glsl"
#pragma use "sdf.glsl"

out vec4 frag_color;

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution * 2.0 - 1.0;
  uv.x *= u_resolution.x / u_resolution.y;
  
  float t = u_time;
  float u = 6.0;
  vec3 view_pos = vec3(cos(t) * u, 3.0, sin(t) * u);
  mat4 view_mat = mat4(1.0) * rotate_y(-t - M_PI / 2.0);
  
  vec3 rd = normalize(vec3(view_mat * vec4(uv, 1.0, 1.0)));
  trace_t tr = ray_march(view_pos, rd);
  
  vec3 color;
  if (tr.d < MAX_DISTANCE) {
    color = vec3(1.0);
  } else {
    color = vec3(0.5, 0.5, 1.0);
  }
  
  frag_color.xyz = color;
  frag_color.w = 1.0;
}

trace_t sdf(vec3 p) {
  trace_t tr = new_trace(0, MAX_DISTANCE);
  tr = trace_cmp(tr, new_trace(0, sdf_cuboid(p, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0))));
  tr = trace_cmp(tr, new_trace(0, sdf_cuboid(p, vec3(3.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0))));
  tr = trace_cmp(tr, new_trace(0, sdf_cuboid(p, vec3(-3.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0))));
  tr = trace_cmp(tr, new_trace(0, sdf_cuboid(p, vec3(0.0, 0.0, 3.0), vec3(1.0, 1.0, 1.0))));
  tr = trace_cmp(tr, new_trace(0, sdf_cuboid(p, vec3(0.0, 0.0, -3.0), vec3(1.0, 1.0, 1.0))));
  return tr;
}
