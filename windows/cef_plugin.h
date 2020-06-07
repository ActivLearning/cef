#ifndef FLUTTER_PLUGIN_CEF_PLUGIN_H_
#define FLUTTER_PLUGIN_CEF_PLUGIN_H_

#include <flutter_plugin_registrar.h>



#pragma comment(lib,"libcef_dll_wrapper.lib")
#pragma comment(lib,"libcef.lib")


#ifdef FLUTTER_PLUGIN_IMPL
#define FLUTTER_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FLUTTER_PLUGIN_EXPORT __declspec(dllimport)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

FLUTTER_PLUGIN_EXPORT void CefPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar);

#if defined(__cplusplus)
}  // extern "C"
#endif

#endif  // FLUTTER_PLUGIN_CEF_PLUGIN_H_
