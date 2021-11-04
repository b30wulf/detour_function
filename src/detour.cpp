#include "detour.h"

void* detour_fn(_In_ void* loc, _In_ void* fn, size_t len)
{
  void* trampoline = VirtualAlloc(nullptr, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  if ( trampoline )
  {
	memcpy(trampoline, loc, len);

	*RC<BYTE*>(RC<DWORD>(trampoline) + len) = 0xE9;
	*RC<DWORD*>(RC<DWORD>(trampoline) + len + 1) = RC<DWORD>(loc) - (RC<DWORD>(trampoline) + len);

	DWORD old_protection{};
	VirtualProtect(loc, len, PAGE_EXECUTE_READWRITE, &old_protection);

	*RC<BYTE*>(RC<DWORD>(loc)) = 0xE9;
	*RC<DWORD*>(RC<DWORD>(loc) + (size_t)1) = RC<DWORD>(fn) - (RC<DWORD>(loc) + 5);

	for ( size_t i = 5; i < len; ++i )
	  *RC<BYTE*>(RC<DWORD>(loc) + i) = 0x90;

	DWORD new_protection{};
	VirtualProtect(loc, len, new_protection, &old_protection);
  }
  return trampoline;
}
