# Environment Configuration
## Prerequisites
In this wiki:<br>
We choose Windows x86_64 as this wiki platform<br>
**Note:<br>**
- hmi_sdk must be used combine with sdl_core generated dynamic library libsmartDeviceLinkCore.so. Reference this [wiki](https://github.com/APCVSRepo/hmi_sdl_android/wiki/SDL-Android-Environment-Configuration-and-Build) for how to.
- if your are working on different OS, such as, Ubuntu system. Please download and install relative tools according to your system.

## Java SE Development Kit
- Download and install [JDK](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)
- Add the jdk install path(such as, such as C:\Program Files (x86)\Java\jdk1.8.0_91\bin) to system environment variable ```PATH```<br>
**Note:<br>**
- Making sure to download the correct version for your development platform<br>
- You must Accept the License Agreement before downloading

## Android NDK
- Download and extract [Android NDK](http://dl.google.com/android/repository/android-ndk-r10e-windows-x86_64.zip)<br>
- Add the ndk install path(such as C:\toolchain\android-ndk-r10e) to system environment variable ```PATH```<br>
**Note:**<br>
Install a version older than r11c NDK, currently the r11c version NDK does not provide arm-linux-androideabi-gdb.exe file on Windows 64 platform, not sure other platforms have the same question.

## Android SDK
- Download and extract [Android SDK](https://dl.google.com/android/android-sdk_r24.4.1-windows.zip)<br>
- Add the sdk install path(such as C:\toolchain\sdk) to the system environment variable ```PATH```

**Note:**<br>
After install SDK, open the folder platforms to check if the android platform you want is here, otherwise, run SDK Manager application to install other Android API.<br> 
If you arise error fetch dl.google.com failed, follow below steps to resolve:
 - select Android SDK Manager menu Tools->options
 - Enter the HTTP Proxy Server and HTTP Proxy Port if you access internet through proxy setting.
 - Check "Force https://... sources to be fetched using http://...", close settings window.
 - select Android SDK Manager menu Packages->Reload
 - Select Android version you need
 - Click button ```Install x packages...``` to install, x is the packages numbers you have selected in above step.

## Apache Ant 
- Download and install [Apache Ant](http://ant.apache.org/bindownload.cgi)<br>
- Add the ant install path(such as C:\toolchain\apache-ant-1.9.7\bin) to the system environment variable ```PATH```

## Qt Creator
### Download and install
- Download [Qt Creator](http://download.qt.io/official_releases/qt/5.5/5.5.1/qt-opensource-windows-x86-android-5.5.1.exe), here we tested on version 5.5.1
- After installation, add install path \Qt\Qt5.5.1\5.5\android_armv7\bin to system environment variable ```PATH```

### Configure Qt Creator
- Compiler setting up
 Open Qt Creator, Choose Tools->Options->Build&Run:
 - Select tab `Qt Versions`, check if `Qt 5.5.1 for Android armv7` is listed under Auto-detected<br>
   if not auto detected, manually add:<br>
   Click ```Add``` button on the right, choose qmake location: \Qt\Qt5.5.1\5.5\android_armv7\bin\qmake
 - Select tab `Kits`, check if `Android for armeabi-v7a` is listed under Auto-detected<br>
   if not auto detected, manually add:<br>
   Click ```Add``` button on the right, enter or choose the right value from dropdown list. For example, below is added by manually<br>
   ```Name:``` Manual-Andorid for armeabi-armv7<br>
   ```Device type:``` Android Device<br>
   ```Device:``` This will be auto changed according to Device type choice, here is Run on Android(default for Android)<br>
   ```Compiler:``` Android GCC (arm-4.9)<br>
   ```Debugger:``` Android Debugger for Android GCC (arm-4.9)<br>
   ```Android GDB server:``` C:\toolchain\android-ndk-r10e\prebuilt\android-arm\gdbserver\gdbserver, this location can also be auto-detected by click Manager button on the right, and choose auto-detect<br>
   ```Qt version:``` Qt 5.5.1 for Android armv7<br>
   Click bottom right ```OK``` button to save, if you manually added kits successful, it's no error remind
- Android compiler environment setting up
  Select Tools->Options->Android:<br>
  If you have installed jdk, sdk, ndk and ant, please choose the right location. If not, there have a download button on the right of each label, download and install them, respectively add them to the system environment variable ```PATH```.
- Restart Qt Creator to make the settings effect

# Build 
  Download the [hmi_sdl_core_androidport](https://github.com/APCVSRepo/hmi_sdl_android.git) code

## Build Settings
- open the project file hmi_sdl_android/hmi_sdk/AppLinkHMI.pro with Qt Creator.
- Switch to Projects mode, it's like a folder icon on the right pane.
- Choose the Build directory, it will make a new android-build directory to store all the build generated files.
- Select Build Android APK details
  - Choose Android build SDK value, here we use ```android-18```<br>
    If not find the sdk platform that you want, back to step android sdk to install platform.
  - Choose Keystore file, it's in directory hmi_sdk\hmi_sdk\Library\android\apk with extension .keystore
  - Check Sign package, it will require to enter the password, here is ```8888888```<br>
    You can also click Create button to create your own keystore and certificate
  - In Qt Deployment area, check Bundle Qt libraries in APK, it will help to package a apk file.

## Build
  Firstly, you should follow [SDL Android Environment Configuration and Build](https://github.com/APCVSRepo/hmi_sdl_android/wiki/SDL-Android-Environment-Configuration-and-Build) wiki to generate the libsmartDeviceLinkCore.so file and other library files.
- Copy the sdl core dynamic library file libsmartDeviceLinkCore.so to the directory \hmi_sdk\hmi_sdk\Library\android\sdl.
- Copy other files to the directory \hmi_sdk\hmi_sdk\Library\android\sdl\config
- Select Build->Build Project "AppLinkHMI.pro" to build. After build finished, it will generate several folders under the ```Build directory``` which we just set in above step.
- Open the android-build\bin directory, will find the build generate apk file.

# Run
There have two ways to run hmi sdl core android port apk file:
- Manually install generated apk file to android system smart phone or android system head unit, open file in smart phone or head unit side.
- Connect android system smart phone or android system head unit to computer via USB, Qt will automatically find the device, click ```Run``` button to directly install sdl core to device.<br>
It maybe need to enter the certificate password while install process, just enter the keystore password, here is 8888888.

