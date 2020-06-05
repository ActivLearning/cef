#import "CefPlugin.h"
#if __has_include(<cef/cef-Swift.h>)
#import <cef/cef-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "cef-Swift.h"
#endif

@implementation CefPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftCefPlugin registerWithRegistrar:registrar];
}
@end
