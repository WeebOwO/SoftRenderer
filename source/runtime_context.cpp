#include "runtime_context.h"

RuntimeContext& RuntimeContext::GetInstance() {
  auto static instance = RuntimeContext();
  return instance;
}