#include "shader.h"

void ShaderContext::Clear() {
  varying_float_.clear();
  varying_vec2f_.clear();
  varying_vec3f_.clear();
  varying_vec4f_.clear();
}

//for (int cx = min_x; cx <= max_x; cx++) {
//
//}