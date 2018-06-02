#ifdef _WIN32
#define _GLFW_WIN32
#undef GL_VERSION
#undef GL_EXTENSIONS
#undef GL_NUM_EXTENSIONS
#undef GL_CONTEXT_FLAGS
#undef GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT
#define vsnprintf _vsnprintf
#define APIENTRY    WINAPI
#include "context.inl"
#include "init.inl"
#include "input.inl"
#include "monitor.inl"
#include "vulkan.inl"
#include "wgl_context.inl"
#include "win32_init.inl"
#include "win32_joystick.inl"
#include "win32_monitor.inl"
#include "win32_time.inl"
#include "win32_tls.inl"
#include "win32_window.inl"
#include "window.inl"
#include "xkb_unicode.inl"
#include "egl_context.inl"
#endif