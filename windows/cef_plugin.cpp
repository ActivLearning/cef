#include "cef_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <thread>
#include "simple_app.h"

using std::cout;
using std::endl;
using std::string;
using std::thread;
namespace {

    inline void  PluginDebug(const string& callbackFun,const string& info)
    {
        cout <<" From CefPlugin "<< callbackFun << ":" << info << endl;
    }
    // Returns the top-level window that owns |view|.
    HWND GetRootWindow(flutter::FlutterView* view) {
        return GetAncestor(view->GetNativeWindow(), GA_ROOT);
    }

    void InitCef()
    {
        // Enable High-DPI support on Windows 7 or newer.
        CefEnableHighDPISupport();

        void* sandbox_info = nullptr;

#if defined(CEF_USE_SANDBOX)
        // Manage the life span of the sandbox information object. This is necessary
        // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
        CefScopedSandboxInfo scoped_sandbox;
        sandbox_info = scoped_sandbox.sandbox_info();
#endif

        // Provide CEF with command-line arguments.
        CefMainArgs main_args;

        // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
        // that share the same executable. This function checks the command-line and,
        // if this is a sub-process, executes the appropriate logic.
        CefExecuteProcess(main_args, nullptr, sandbox_info);

        // Specify CEF global settings here.
        CefSettings settings;
#if !defined(CEF_USE_SANDBOX)
        settings.no_sandbox = true;
#endif

        // SimpleApp implements application-level callbacks for the browser process.
        // It will create the first browser instance in OnContextInitialized() after
        // CEF has initialized.
        CefRefPtr<SimpleApp> app(new SimpleApp);

        // Initialize CEF.
        CefInitialize(main_args, settings, app.get(), sandbox_info);

        // Run the CEF message loop. This will block until CefQuitMessageLoop() is
        // called.
        CefRunMessageLoop();

        // Shut down CEF.
        CefShutdown();
    }
class CefPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  CefPlugin(flutter::PluginRegistrarWindows* registrar);

  virtual ~CefPlugin();

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  flutter::PluginRegistrarWindows* m_pRegistrar;
};

// static
void CefPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "cef",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<CefPlugin>(registrar);

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });
  registrar->AddPlugin(std::move(plugin));
}

CefPlugin::CefPlugin(flutter::PluginRegistrarWindows* registrar):m_pRegistrar(registrar) {}

CefPlugin::~CefPlugin() {}

void CefPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  // Replace "getPlatformVersion" check with your plugin's method.
  // See:
  // https://github.com/flutter/engine/tree/master/shell/platform/common/cpp/client_wrapper/include/flutter
  // and
  // https://github.com/flutter/engine/tree/master/shell/platform/glfw/client_wrapper/include/flutter
  // for the relevant Flutter APIs.
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    flutter::EncodableValue response(version_stream.str());
    result->Success(&response);
  } 
  else if (method_call.method_name().compare("newCefWindow") == 0)
  {
      string url = method_call.arguments()->StringValue();
      PluginDebug("newCefWindow", "InitUrl: "+ url);
      //thread *thread1 = new thread (InitCef);
     // thread1->detach();
      flutter::EncodableValue response(10);
      result->Success(&response);
  }
  else if (method_call.method_name().compare("loadUrl") == 0)
  {
      
      int winID = method_call.arguments()->MapValue().at( flutter::EncodableValue( "uniqueID") ).IntValue();
      string url = method_call.arguments()->MapValue().at( flutter::EncodableValue( "url" ) ).StringValue();
      PluginDebug("loadUrl", "windowsID: "+ std::to_string( winID )+" url: "+ url);
      result->Success();
  }
  else if (method_call.method_name().compare("closeWindowByID") == 0)
  {
      PluginDebug("closeWindowByID", "windowID:"+ std::to_string( method_call.arguments()->IntValue() ) );
      result->Success();
  }
  else if (method_call.method_name().compare("dispose") == 0)
  {
      PluginDebug("dispose", "");
      result->Success();
  }
  else 
  {
    result->NotImplemented();
  }
}

}  // namespace

void CefPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  CefPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
