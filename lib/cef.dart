import 'dart:async';

import 'package:flutter/services.dart';

class Cef {
  static const MethodChannel _channel = const MethodChannel('cef');

  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  //create the new CefWidows Object with load url , return its unique ID
  static Future<int> newCefWindow(String url) async {
    final int uniqueID = await _channel.invokeMethod('newCefWindow', url);
    return uniqueID;
  }

  static Future<void> loadUrl(int uniqueID, String url) async {
    await _channel.invokeMethod(
        'loadUrl', <String, dynamic>{'uniqueID': uniqueID, 'url': url});
  }

  static Future<void> closeWindowByID(int uniqueID) async {
    await _channel.invokeMethod('closeWindowByID', uniqueID);
  }

  static Future<void> dispose() async {
    await _channel.invokeMethod('dispose');
  }
}
