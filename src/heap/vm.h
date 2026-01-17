#pragma once

#include <cstddef>

namespace proj::heap::vm
{
  void *resv (size_t sz);
  void unresv (void *ptr, size_t sz);
  void map (void *ptr, size_t sz);
  void unmap (void *ptr, size_t sz);
}
