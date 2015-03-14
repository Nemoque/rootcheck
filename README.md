# rootcheck
These codes are just simple tool which used for Android Root tampering check and restore infomation into /sdcard or other storage you attempt to.

The main logic is checking md5 value for system apps which pre-installed in the device side.

basically ,just stick this library into an application(Android APK) for JNI calling.

Usage:
   1. "tools" dir: a simple script could be expanded for md5 list generation;
   2. "utils" dir: main checking logic and md5 value matching for the devices,and compile them into Android source code or NDK tools;
