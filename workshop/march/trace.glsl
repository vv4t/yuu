#ifndef TRACE_GLSL
#define TRACE_GLSL

struct trace_t {
  float d;
  int id;
};

trace_t new_trace(int id, float d) {
  trace_t tr;
  tr.id = id;
  tr.d = d;
  return tr;
}

trace_t trace_cmp(trace_t a, trace_t b) {
  return a.d < b.d ? a : b;
}

#endif
