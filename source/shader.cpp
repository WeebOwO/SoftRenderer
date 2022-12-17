#include "shader.h"

void ShaderContext::Clear() {
  varying_float.clear();
  varying_vec2f.clear();
  varying_vec3f.clear();
  varying_vec4f.clear();
}
