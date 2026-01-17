#include "vm.h"

#include <cstddef>
#include <cstdlib>

#if __APPLE__ || __linux__

#include <sys/mman.h>

namespace proj::heap::vm
{
  void *resv (size_t sz)
  {
    void *ptr = mmap (NULL, sz, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) abort ();
    return ptr;
  }
  void unresv (void *ptr, size_t sz)
  {
    if (sz == 0 || munmap (ptr, sz)) abort ();
  }
  void map (void *ptr, size_t sz)
  {
    if (sz == 0 || mprotect (ptr, sz, (PROT_READ | PROT_WRITE))) abort ();
  }
  void unmap (void *ptr, size_t sz)
  {
    if (sz == 0 || madvise (ptr, sz, MADV_DONTNEED)) abort ();
  }
} // namespace proj::heap::vm

#elif _WIN32

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace proj::heap::vm
{
  void *resv (size_t sz)
  {
    void *ptr = VirtualAlloc (NULL, sz, MEM_RESERVE, PAGE_NOACCESS);
    if (!ptr) abort ();
    return ptr;
  }
  void unresv (void *ptr, size_t sz)
  {
    (void) sz;
    if (!VirtualFree (ptr, 0, MEM_RELEASE)) abort ();
  }
  void map (void *ptr, size_t sz)
  {
    if (sz == 0 || !VirtualAlloc (ptr, sz, MEM_COMMIT, PAGE_READWRITE)) abort ();
  }
  void unmap (void *ptr, size_t sz)
  {
    if (sz == 0 || !VirtualFree (ptr, sz, MEM_DECOMMIT)) abort ();
  }
} // namespace proj::heap::vm

#endif
