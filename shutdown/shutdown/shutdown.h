#pragma once

extern "C" {
	__declspec(dllexport) int __cdecl nativeLoop(void (*shutdown_ready)(int ignored), BOOL (*shutdown_query_endsession)(), BOOL(*shutdown_endsession)());
	__declspec(dllexport) void __cdecl quit();
}