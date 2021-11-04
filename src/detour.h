#pragma once

#include <sal.h>
#include <windows.h>
#define RC reinterpret_cast

void* detour_fn(_In_ void* loc, _In_ void* fn, size_t len);
