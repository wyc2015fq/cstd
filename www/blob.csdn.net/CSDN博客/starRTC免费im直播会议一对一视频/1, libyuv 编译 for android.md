# 1, libyuv 编译 for android - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月17日 12:25:47[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：2955
所属专栏：[编解码](https://blog.csdn.net/column/details/12721.html)
libyuv is an open source project that includes
YUV scaling and conversion
functionality.
Scale YUV to prepare content for compression, with point, bilinear or box filter.
Convert to YUV from webcam formats.
Convert from YUV to formats for rendering/effects.
Rotate by 
90/180/270 degrees to adjust for mobile devices in portrait mode.
Optimized for SSE2/SSSE3/AVX2 on x86/x64.
Optimized for
Neon on Arm.
Optimized for DSP R2 on Mips.
[https://github.com/illuspas/libyuv-android](https://github.com/illuspas/libyuv-android)
直接ndk-build即可

参考资料：
**一，Getting Started**
How to get and build the libyuv code.
**Pre-requisites**
You'll need to have depot tools installed: [https://www.chromium.org/developers/how-tos/install-depot-tools](https://www.chromium.org/developers/how-tos/install-depot-tools) Refer
 to chromium instructions for each platform for other prerequisites.
**Getting the Code**
Create a working directory, enter it, and run:
gclient config https://chromium.googlesource.com/libyuv/libyuvgclient sync
Then you'll get a .gclient file like:
solutions = [ { "name" : "libyuv", "url" : "https://chromium.googlesource.com/libyuv/libyuv", "deps_file" : "DEPS", "managed" : True, "custom_deps" : { }, "safesync_url": "", },];
For iOS add ;target_os=['ios']; to
 your OSX .gclient and run GYP_DEFINES="OS=ios" gclient sync.
Browse the Git reprository: [https://chromium.googlesource.com/libyuv/libyuv/+/master](https://chromium.googlesource.com/libyuv/libyuv/+/master)
**Android**
For Android add ;target_os=['android']; to
 your Linux .gclient
solutions = [ { "name" : "libyuv", "url" : "https://chromium.googlesource.com/libyuv/libyuv", "deps_file" : "DEPS", "managed" : True, "custom_deps" : { }, "safesync_url": "", },];target_os =
 ["android", "unix"];
Then run:
export GYP_DEFINES="OS=android"gclient sync
Caveat: Theres an error with Google Play services updates. If you get the error “Your version of the Google Play services library is not
 up to date”, run the following:
cd chromium/src./build/android/play_services/update.py downloadcd ../..
For Windows the gclient sync must be done from an Administrator command prompt.
The sync will generate native build files for your environment using gyp (Windows: Visual Studio, OSX: XCode, Linux: make). This generation
 can also be forced manually: gclient runhooks
To get just the source (not buildable):
git clone https://chromium.googlesource.com/libyuv/libyuv
**Building the Library and Unittests**
**Windows**
set GYP_DEFINES=target_arch=ia32call python gyp_libyuv -fninja -G msvs_version=2013ninja -j7 -C out\Releaseninja -j7 -C out\Debugset GYP_DEFINES=target_arch=x64call python gyp_libyuv -fninja
 -G msvs_version=2013ninja -C out\Debug_x64ninja -C out\Release_x64
**Building with clangcl**
set GYP_DEFINES=clang=1 target_arch=ia32 libyuv_enable_svn=1set LLVM_REPO_URL=svn://svn.chromium.org/llvm-projectcall python tools\clang\scripts\update.pycall python gyp_libyuv -fninja libyuv_test.gypninja
 -C out\Debugninja -C out\Release
**OSX**
Clang 64 bit shown. Remove clang=1 for
 GCC and change x64 to ia32 for 32 bit.
GYP_DEFINES="clang=1 target_arch=x64" ./gyp_libyuvninja -j7 -C out/Debugninja -j7 -C out/ReleaseGYP_DEFINES="clang=1 target_arch=ia32" ./gyp_libyuvninja -j7 -C out/Debugninja -j7 -C out/Release
**iOS**
[http://www.chromium.org/developers/how-tos/build-instructions-ios](http://www.chromium.org/developers/how-tos/build-instructions-ios)
Add to .gclient last line: target_os=['ios'];
armv7
GYP_DEFINES="OS=ios target_arch=armv7 target_subarch=arm32" GYP_CROSSCOMPILE=1 GYP_GENERATOR_FLAGS="output_dir=out_ios" ./gyp_libyuvninja -j7 -C out_ios/Debug-iphoneos libyuv_unittestninja -j7
 -C out_ios/Release-iphoneos libyuv_unittest
arm64
GYP_DEFINES="OS=ios target_arch=arm64 target_subarch=arm64" GYP_CROSSCOMPILE=1 GYP_GENERATOR_FLAGS="output_dir=out_ios" ./gyp_libyuvninja -j7 -C out_ios/Debug-iphoneos libyuv_unittestninja -j7
 -C out_ios/Release-iphoneos libyuv_unittest
both armv7 and arm64 (fat)
GYP_DEFINES="OS=ios target_arch=armv7 target_subarch=both" GYP_CROSSCOMPILE=1 GYP_GENERATOR_FLAGS="output_dir=out_ios" ./gyp_libyuvninja -j7 -C out_ios/Debug-iphoneos libyuv_unittestninja -j7
 -C out_ios/Release-iphoneos libyuv_unittest
simulator
GYP_DEFINES="OS=ios target_arch=ia32 target_subarch=arm32" GYP_CROSSCOMPILE=1 GYP_GENERATOR_FLAGS="output_dir=out_sim" ./gyp_libyuvninja -j7 -C out_sim/Debug-iphonesimulator libyuv_unittestninja
 -j7 -C out_sim/Release-iphonesimulator libyuv_unittest
**Android**
[https://code.google.com/p/chromium/wiki/AndroidBuildInstructions](https://code.google.com/p/chromium/wiki/AndroidBuildInstructions)
Add to .gclient last line: target_os=['android'];
armv7
GYP_DEFINES="OS=android" GYP_CROSSCOMPILE=1 ./gyp_libyuvninja -j7 -C out/Debug yuv_unittest_apkninja -j7 -C out/Release yuv_unittest_apk
arm64
GYP_DEFINES="OS=android target_arch=arm64 target_subarch=arm64" GYP_CROSSCOMPILE=1 ./gyp_libyuvninja -j7 -C out/Debug yuv_unittest_apkninja -j7 -C out/Release yuv_unittest_apk
ia32
GYP_DEFINES="OS=android target_arch=ia32" GYP_CROSSCOMPILE=1 ./gyp_libyuvninja -j7 -C out/Debug yuv_unittest_apkninja -j7 -C out/Release yuv_unittest_apkGYP_DEFINES="OS=android target_arch=ia32
 android_full_debug=1" GYP_CROSSCOMPILE=1 ./gyp_libyuvninja -j7 -C out/Debug yuv_unittest_apk
mipsel
GYP_DEFINES="OS=android target_arch=mipsel" GYP_CROSSCOMPILE=1 ./gyp_libyuvninja -j7 -C out/Debug yuv_unittest_apkninja -j7 -C out/Release yuv_unittest_apk
arm32 disassembly:
third_party/android_tools/ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump -d out/Release/obj/source/libyuv.row_neon.o
arm64 disassembly:
third_party/android_tools/ndk/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-objdump -d out/Release/obj/source/libyuv.row_neon64.o
Running tests:
util/android/test_runner.py gtest -s libyuv_unittest -t 7200 --verbose --release --gtest_filter=*
Running test as benchmark:
util/android/test_runner.py gtest -s libyuv_unittest -t 7200 --verbose --release --gtest_filter=* -a "--libyuv_width=1280 --libyuv_height=720 --libyuv_repeat=999 --libyuv_flags=-1"
Running test with C code:
util/android/test_runner.py gtest -s libyuv_unittest -t 7200 --verbose --release --gtest_filter=* -a "--libyuv_width=1280 --libyuv_height=720 --libyuv_repeat=999 --libyuv_flags=1 --libyuv_cpu_info=1"
**Building with GN**
gn gen out/Release "--args=is_debug=false target_cpu=\"x86\""gn gen out/Debug "--args=is_debug=true target_cpu=\"x86\""ninja -C out/Releaseninja -C out/Debug
**Building Offical with GN**
gn gen out/Official "--args=is_debug=false is_official_build=true is_chrome_branded=true"ninja -C out/Official
**Linux**
GYP_DEFINES="target_arch=x64" ./gyp_libyuvninja -j7 -C out/Debugninja -j7 -C out/ReleaseGYP_DEFINES="target_arch=ia32" ./gyp_libyuvninja -j7 -C out/Debugninja -j7 -C out/Release
**CentOS**
On CentOS 32 bit the following work around allows a sync:
export GYP_DEFINES="host_arch=ia32"gclient sync
**Windows Shared Library**
Modify libyuv.gyp from ‘static_library’ to ‘shared_library’, and add ‘LIBYUV_BUILDING_SHARED_LIBRARY’ to ‘defines’.
gclient runhooks
After this command follow the building the library instructions above.
If you get a compile error for atlthunk.lib on Windows, read [http://www.chromium.org/developers/how-tos/build-instructions-windows](http://www.chromium.org/developers/how-tos/build-instructions-windows)
**Build targets**
ninja -C out/Debug libyuvninja -C out/Debug libyuv_unittestninja -C out/Debug compareninja -C out/Debug convertninja -C out/Debug psnrninja -C out/Debug cpuid
**Building the Library with make**
**Linux**
make -j7 V=1 -f linux.mkmake -j7 V=1 -f linux.mk cleanmake -j7 V=1 -f linux.mk CXX=clang++
**Building the Library with cmake**
Install cmake: [http://www.cmake.org/](http://www.cmake.org/)
Default debug build:
mkdir outcd outcmake ..cmake --build .
Release build/install
mkdir outcd outcmake -DCMAKE_INSTALL_PREFIX="/usr/lib" -DCMAKE_BUILD_TYPE="Release" ..cmake --build . --config Releasesudo cmake --build . --target install --config Release
**Windows 8 Phone**
Pre-requisite:
- Install Visual Studio 2012 and Arm to your environment.
Then:
call "c:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\x86_arm\vcvarsx86_arm.bat"
or with Visual Studio 2013:
call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\x86_arm\vcvarsx86_arm.bat"nmake /f winarm.mk cleannmake /f winarm.mk
**Windows Shared Library**
Modify libyuv.gyp from ‘static_library’ to ‘shared_library’, and add ‘LIBYUV_BUILDING_SHARED_LIBRARY’ to ‘defines’. Then run this.
gclient runhooks
After this command follow the building the library instructions above.
If you get a compile error for atlthunk.lib on Windows, read [http://www.chromium.org/developers/how-tos/build-instructions-windows](http://www.chromium.org/developers/how-tos/build-instructions-windows)
**64 bit Windows**
set GYP_DEFINES=target_arch=x64gclient runhooks V=1
**ARM Linux**
export GYP_DEFINES="target_arch=arm"export CROSSTOOL=`<path>`/arm-none-linux-gnueabiexport CXX=$CROSSTOOL-g++export CC=$CROSSTOOL-gccexport AR=$CROSSTOOL-arexport AS=$CROSSTOOL-asexport RANLIB=$CROSSTOOL-ranlibgclient
 runhooks
**Running Unittests**
**Windows**
out\Release\libyuv_unittest.exe --gtest_catch_exceptions=0 --gtest_filter="*"
**OSX**
out/Release/libyuv_unittest --gtest_filter="*"
**Linux**
out/Release/libyuv_unittest --gtest_filter="*"
Replace --gtest_filter=“*” with specific unittest to run. May include wildcards. e.g.
out/Release/libyuv_unittest --gtest_filter=libyuvTest.I420ToARGB_Opt
**CPU Emulator tools**
**Intel SDE (Software Development Emulator)**
Pre-requisite: Install IntelSDE for Windows: [http://software.intel.com/en-us/articles/intel-software-development-emulator](http://software.intel.com/en-us/articles/intel-software-development-emulator)
Then run:
c:\intelsde\sde -hsw -- out\release\libyuv_unittest.exe --gtest_filter=*
**Memory tools**
**Running Dr Memory memcheck for Windows**
Pre-requisite: Install Dr Memory for Windows and add it to your path: [http://www.drmemory.org/docs/page_install_windows.html](http://www.drmemory.org/docs/page_install_windows.html)
set GYP_DEFINES=build_for_tool=drmemory target_arch=ia32call python gyp_libyuv -fninja -G msvs_version=2013ninja -C out\Debugdrmemory out\Debug\libyuv_unittest.exe --gtest_catch_exceptions=0
 --gtest_filter=*
**Running UBSan**
See Chromium instructions for sanitizers: [https://www.chromium.org/developers/testing/undefinedbehaviorsanitizer](https://www.chromium.org/developers/testing/undefinedbehaviorsanitizer)
Sanitizers available: TSan, MSan, ASan, UBSan, LSan
GYP_DEFINES='ubsan=1' gclient runhooksninja -C out/Release
**Running Valgrind memcheck**
Memory errors and race conditions can be found by running tests under special memory tools. [Valgrind] [1](http://valgrind.org/) is
 an instrumentation framework for building dynamic analysis tools. Various tests and profilers are built upon it to find memory handling errors and memory leaks, for instance.
solutions = [ { "name" : "libyuv", "url" : "https://chromium.googlesource.com/libyuv/libyuv", "deps_file" : "DEPS", "managed" : True, "custom_deps" : { "libyuv/chromium/src/third_party/valgrind":
 "https://chromium.googlesource.com/chromium/deps/valgrind/binaries", }, "safesync_url": "", },]
Then run:
GYP_DEFINES="clang=0 target_arch=x64 build_for_tool=memcheck" python gyp_libyuvninja -C out/Debugvalgrind out/Debug/libyuv_unittest
For more information, see [http://www.chromium.org/developers/how-tos/using-valgrind](http://www.chromium.org/developers/how-tos/using-valgrind)
**Running Thread Sanitizer (TSan)**
GYP_DEFINES="clang=0 target_arch=x64 build_for_tool=tsan" python gyp_libyuvninja -C out/Debugvalgrind out/Debug/libyuv_unittest
For more info, see [http://www.chromium.org/developers/how-tos/using-valgrind/threadsanitizer](http://www.chromium.org/developers/how-tos/using-valgrind/threadsanitizer)
**Running Address Sanitizer (ASan)**
GYP_DEFINES="clang=0 target_arch=x64 build_for_tool=asan" python gyp_libyuvninja -C out/Debugvalgrind out/Debug/libyuv_unittest
For more info, see [http://dev.chromium.org/developers/testing/addresssanitizer](http://dev.chromium.org/developers/testing/addresssanitizer)
**Benchmarking**
The unittests can be used to benchmark.
**Windows**
set LIBYUV_WIDTH=1280set LIBYUV_HEIGHT=720set LIBYUV_REPEAT=999set LIBYUV_FLAGS=-1out\Release\libyuv_unittest.exe --gtest_filter=*I420ToARGB_Opt
**Linux and Mac**
LIBYUV_WIDTH=1280 LIBYUV_HEIGHT=720 LIBYUV_REPEAT=1000 out/Release/libyuv_unittest --gtest_filter=*I420ToARGB_OptlibyuvTest.I420ToARGB_Opt (547 ms)
Indicates 0.547 ms/frame for 1280 x 720.
**Making a change**
gclient syncgit checkout -b mycl -t origin/mastergit pull<edit files>git add -ugit commit -m "my change"git cl lintgit cl trygit cl upload -r a-reviewer@chomium.org -s<once approved..>git cl
 land
二，filtering
Introduction
This document discusses the current state of filtering in libyuv. An emphasis on maximum performance while avoiding memory exceptions, and minimal amount of code/complexity. See future work
 at end.
LibYuv Filter Subsampling
There are 2 challenges with subsampling
centering of samples, which involves clamping on edges
clipping a source region
Centering depends on scale factor and filter mode.
Down Sampling
If scaling down, the stepping rate is always src_width / dst_width.
dx = src_width / dst_width;
e.g. If scaling from 1280x720 to 640x360, the step thru the source will be 2.0, stepping over 2 pixels of source for each pixel of destination.
Centering, depends on filter mode.
Point downsampling takes the middle pixel.
x = dx >> 1;
For odd scale factors (e.g. 3x down) this is exactly the middle. For even scale factors, this rounds up and takes the pixel to the right of center. e.g. scale of 4x down will take pixel 2.
Bilinear filter, uses the 2x2 pixels in the middle.
x = dx / 2 - 0.5;
For odd scale factors (e.g. 3x down) this is exactly the middle, and point sampling is used. For even scale factors, this evenly filters the middle 2x2 pixels. e.g. 4x down will filter pixels
 1,2 at 50% in both directions.
Box filter averages the entire box so sampling starts at 0.
x = 0;
For a scale factor of 2x down, this is equivalent to bilinear.
Up Sampling
Point upsampling use stepping rate of src_width / dst_width and a starting coordinate of 0.
x = 0;
dx = src_width / dst_width;
e.g. If scaling from 640x360 to 1280x720 the step thru the source will be 0.0, stepping half a pixel of source for each pixel of destination. Each pixel is replicated by the scale factor.
Bilinear filter stretches such that the first pixel of source maps to the first pixel of destination, and the last pixel of source maps to the last pixel of destination.
x = 0;
dx = (src_width - 1) / (dst_width - 1);
This method is not technically correct, and will likely change in the future.
It is inconsistent with the bilinear down sampler. The same method could be used for down sampling, and then it would be more reversible, but that would prevent specialized 2x down sampling.
Although centered, the image is slightly magnified.
The filtering was changed in early 2013 - previously it used:
x = 0;
dx = (src_width - 1) / (dst_width - 1);
Which is the correct scale factor, but shifted the image left, and extruded the last pixel. The reason for the change was to remove the extruding code from the low level row functions, allowing
 3 functions to sshare the same row functions - ARGBScale, I420Scale, and ARGBInterpolate. Then the one function was ported to many cpu variations: SSE2, SSSE3, AVX2, Neon and ‘Any’ version for any number of pixels and alignment. The function is also specialized
 for 0,25,50,75%.
The above goes still has the potential to read the last pixel 100% and last pixel + 1 0%, which may cause a memory exception. So the left pixel goes to a fraction less than the last pixel, but
 filters in the minimum amount of it, and the maximum of the last pixel.
dx = FixedDiv((src_width << 16) - 0x00010001, (dst << 16) - 0x00010000);
Box filter for upsampling switches over to Bilinear.
Scale snippet:
#define CENTERSTART(dx, s) (dx < 0) ? -((-dx >> 1) + s) : ((dx >> 1) + s)
#define FIXEDDIV1(src, dst) FixedDiv((src << 16) - 0x00010001, \
(dst << 16) - 0x00010000);
// Compute slope values for stepping.
void ScaleSlope(int src_width, int src_height,
int dst_width, int dst_height,
FilterMode filtering,
int* x, int* y, int* dx, int* dy) {
assert(x != NULL);
assert(y != NULL);
assert(dx != NULL);
assert(dy != NULL);
assert(src_width != 0);
assert(src_height != 0);
assert(dst_width > 0);
assert(dst_height > 0);
if (filtering == kFilterBox) {
// Scale step for point sampling duplicates all pixels equally.
*dx = FixedDiv(Abs(src_width), dst_width);
*dy = FixedDiv(src_height, dst_height);
*x = 0;
*y = 0;
} else if (filtering == kFilterBilinear) {
// Scale step for bilinear sampling renders last pixel once for upsample.
if (dst_width <= Abs(src_width)) {
*dx = FixedDiv(Abs(src_width), dst_width);
*x = CENTERSTART(*dx, -32768);
} else if (dst_width > 1) {
*dx = FIXEDDIV1(Abs(src_width), dst_width);
*x = 0;
}
if (dst_height <= src_height) {
*dy = FixedDiv(src_height, dst_height);
*y = CENTERSTART(*dy, -32768); // 32768 = -0.5 to center bilinear.
} else if (dst_height > 1) {
*dy = FIXEDDIV1(src_height, dst_height);
*y = 0;
}
} else if (filtering == kFilterLinear) {
// Scale step for bilinear sampling renders last pixel once for upsample.
if (dst_width <= Abs(src_width)) {
*dx = FixedDiv(Abs(src_width), dst_width);
*x = CENTERSTART(*dx, -32768);
} else if (dst_width > 1) {
*dx = FIXEDDIV1(Abs(src_width), dst_width);
*x = 0;
}
*dy = FixedDiv(src_height, dst_height);
*y = *dy >> 1;
} else {
// Scale step for point sampling duplicates all pixels equally.
*dx = FixedDiv(Abs(src_width), dst_width);
*dy = FixedDiv(src_height, dst_height);
*x = CENTERSTART(*dx, 0);
*y = CENTERSTART(*dy, 0);
}
// Negative src_width means horizontally mirror.
if (src_width < 0) {
*x += (dst_width - 1) * *dx;
*dx = -*dx;
src_width = -src_width;
}
}
Future Work
Point sampling should ideally be the same as bilinear, but pixel by pixel, round to nearest neighbor. But as is, it is reversible and exactly matches ffmpeg at all scale factors, both up and
 down. The scale factor is
dx = src_width / dst_width;
The step value is centered for down sample:
x = dx / 2;
Or starts at 0 for upsample.
x = 0;
Bilinear filtering is currently correct for down sampling, but not for upsampling. Upsampling is stretching the first and last pixel of source, to the first and last pixel of destination.
dx = (src_width - 1) / (dst_width - 1);<br>
x = 0;
It should be stretching such that the first pixel is centered in the middle of the scale factor, to match the pixel that would be sampled for down sampling by the same amount. And same on last
 pixel.
dx = src_width / dst_width;<br>
x = dx / 2 - 0.5;
This would start at -0.5 and go to last pixel + 0.5, sampling 50% from last pixel + 1. Then clamping would be needed. On GPUs there are numerous ways to clamp.
Clamp the coordinate to the edge of the texture, duplicating the first and last pixel.
Blend with a constant color, such as transparent black. Typically best for fonts.
Mirror the UV coordinate, which is similar to clamping. Good for continuous tone images.
Wrap the coordinate, for texture tiling.
Allow the coordinate to index beyond the image, which may be the correct data if sampling a subimage.
Extrapolate the edge based on the previous pixel. pixel -0.5 is computed from slope of pixel 0 and 1.
Some of these are computational, even for a GPU, which is one reason textures are sometimes limited to power of 2 sizes. We do care about the clipping case, where allowing coordinates to become
 negative and index pixels before the image is the correct data. But normally for simple scaling, we want to clamp to the edge pixel. For example, if bilinear scaling from 3x3 to 30x30, we’d essentially want 10 pixels of each of the original 3 pixels. But we
 want the original pixels to land in the middle of each 10 pixels, at offsets 5, 15 and 25. There would be filtering between 5 and 15 between the original pixels 0 and 1. And filtering between 15 and 25 from original pixels 1 and 2. The first 5 pixels are clamped
 to pixel 0 and the last 5 pixels are clamped to pixel 2. The easiest way to implement this is copy the original 3 pixels to a buffer, and duplicate the first and last pixels. 0,1,2 becomes 0, 0,1,2, 2. Then implement a filtering without clamping. We call this
 source extruding. Its only necessary on up sampling, since down sampler will always have valid surrounding pixels. Extruding is practical when the image is already copied to a temporary buffer. It could be done to the original image, as long as the original
 memory is restored, but valgrind and/or memory protection would disallow this, so it requires a memcpy to a temporary buffer, which may hurt performance. The memcpy has a performance advantage, from a cache point of view, that can actually make this technique
 faster, depending on hardware characteristics. Vertical extrusion can be done with a memcpy of the first/last row, or clamping a pointer.
The other way to implement clamping is handle the edges with a memset. e.g. Read first source pixel and memset the first 5 pixels. Filter pixels 0,1,2 to 5 to 25. Read last pixel and memset
 the last 5 pixels. Blur is implemented with this method like this, which has 3 loops per row - left, middle and right.
Box filter is only used for 2x down sample or more. Its based on integer sized boxes. Technically it should be filtered edges, but thats substantially slower (roughly 100x), and at that point
 you may as well do a cubic filter which is more correct.
Box filter currently sums rows into a row buffer. It does this with
Mirroring will use the same slope as normal, but with a negative. The starting coordinate needs to consider the scale factor and filter. e.g. box filter of 30x30 to 3x3 with mirroring would
 use -10 for step, but x = 20. width (30) - dx.
Step needs to be accurate, so it uses an integer divide. This is as much as 5% of the profile. An approximated divide is substantially faster, but the inaccuracy causes stepping beyond the original
 image boundaries. 3 general solutions:
copy image to buffer with padding. allows for small errors in stepping.
hash the divide, so common values are quickly found.
change api so caller provides the slope.
三， formats
Introduction
Formats (FOURCC) supported by libyuv are detailed here.
Core Formats
There are 2 core formats supported by libyuv - I420 and ARGB. All YUV formats can be converted to/from I420. All RGB formats can be converted to/from ARGB.
Filtering functions such as scaling and planar functions work on I420 and/or ARGB.
OSX Core Media Pixel Formats
This is how 
OSX formats map to libyuv
enum {
kCMPixelFormat_32ARGB = 32, FOURCC_BGRA
kCMPixelFormat_32BGRA = 'BGRA', FOURCC_ARGB
kCMPixelFormat_24RGB = 24, FOURCC_RAW
kCMPixelFormat_16BE555 = 16, Not supported.
kCMPixelFormat_16BE565 = 'B565', Not supported.
kCMPixelFormat_16LE555 = 'L555', FOURCC_RGBO
kCMPixelFormat_16LE565 = 'L565', FOURCC_RGBP
kCMPixelFormat_16LE5551 = '5551', FOURCC_RGBO
kCMPixelFormat_422YpCbCr8 = '2vuy', FOURCC_UYVY
kCMPixelFormat_422YpCbCr8_yuvs = 'yuvs', FOURCC_YUY2
kCMPixelFormat_444YpCbCr8 = 'v308', FOURCC_I444 ?
kCMPixelFormat_4444YpCbCrA8 = 'v408', Not supported.
kCMPixelFormat_422YpCbCr16 = 'v216', Not supported.
kCMPixelFormat_422YpCbCr10 = 'v210', FOURCC_V210 previously. Removed now.
kCMPixelFormat_444YpCbCr10 = 'v410', Not supported.
kCMPixelFormat_8IndexedGray_WhiteIsZero = 0x00000028, Not supported.
};
FOURCC (Four Charactacter Code) List
The following is extracted from video_common.h as a complete list of formats supported by libyuv.
enum FourCC {
// 9 Primary YUV formats: 5 planar, 2 biplanar, 2 packed.
FOURCC_I420 = FOURCC('I', '4', '2', '0'),
FOURCC_I422 = FOURCC('I', '4', '2', '2'),
FOURCC_I444 = FOURCC('I', '4', '4', '4'),
FOURCC_I411 = FOURCC('I', '4', '1', '1'),
FOURCC_I400 = FOURCC('I', '4', '0', '0'),
FOURCC_NV21 = FOURCC('N', 'V', '2', '1'),
FOURCC_NV12 = FOURCC('N', 'V', '1', '2'),
FOURCC_YUY2 = FOURCC('Y', 'U', 'Y', '2'),
FOURCC_UYVY = FOURCC('U', 'Y', 'V', 'Y'),
// 2 Secondary YUV formats: row biplanar.
FOURCC_M420 = FOURCC('M', '4', '2', '0'),
FOURCC_Q420 = FOURCC('Q', '4', '2', '0'),
// 9 Primary RGB formats: 4 32 bpp, 2 24 bpp, 3 16 bpp.
FOURCC_ARGB = FOURCC('A', 'R', 'G', 'B'),
FOURCC_BGRA = FOURCC('B', 'G', 'R', 'A'),
FOURCC_ABGR = FOURCC('A', 'B', 'G', 'R'),
FOURCC_24BG = FOURCC('2', '4', 'B', 'G'),
FOURCC_RAW = FOURCC('r', 'a', 'w', ' '),
FOURCC_RGBA = FOURCC('R', 'G', 'B', 'A'),
FOURCC_RGBP = FOURCC('R', 'G', 'B', 'P'), // rgb565 LE.
FOURCC_RGBO = FOURCC('R', 'G', 'B', 'O'), // argb1555 LE.
FOURCC_R444 = FOURCC('R', '4', '4', '4'), // argb4444 LE.
// 4 Secondary RGB formats: 4 Bayer Patterns.
FOURCC_RGGB = FOURCC('R', 'G', 'G', 'B'),
FOURCC_BGGR = FOURCC('B', 'G', 'G', 'R'),
FOURCC_GRBG = FOURCC('G', 'R', 'B', 'G'),
FOURCC_GBRG = FOURCC('G', 'B', 'R', 'G'),
// 1 Primary Compressed YUV format.
FOURCC_MJPG = FOURCC('M', 'J', 'P', 'G'),
// 5 Auxiliary YUV variations: 3 with U and V planes are swapped, 1 Alias.
FOURCC_YV12 = FOURCC('Y', 'V', '1', '2'),
FOURCC_YV16 = FOURCC('Y', 'V', '1', '6'),
FOURCC_YV24 = FOURCC('Y', 'V', '2', '4'),
FOURCC_YU12 = FOURCC('Y', 'U', '1', '2'), // Linux version of I420.
FOURCC_J420 = FOURCC('J', '4', '2', '0'),
FOURCC_J400 = FOURCC('J', '4', '0', '0'),
// 14 Auxiliary aliases. CanonicalFourCC() maps these to canonical fourcc.
FOURCC_IYUV = FOURCC('I', 'Y', 'U', 'V'), // Alias for I420.
FOURCC_YU16 = FOURCC('Y', 'U', '1', '6'), // Alias for I422.
FOURCC_YU24 = FOURCC('Y', 'U', '2', '4'), // Alias for I444.
FOURCC_YUYV = FOURCC('Y', 'U', 'Y', 'V'), // Alias for YUY2.
FOURCC_YUVS = FOURCC('y', 'u', 'v', 's'), // Alias for YUY2 on Mac.
FOURCC_HDYC = FOURCC('H', 'D', 'Y', 'C'), // Alias for UYVY.
FOURCC_2VUY = FOURCC('2', 'v', 'u', 'y'), // Alias for UYVY on Mac.
FOURCC_JPEG = FOURCC('J', 'P', 'E', 'G'), // Alias for MJPG.
FOURCC_DMB1 = FOURCC('d', 'm', 'b', '1'), // Alias for MJPG on Mac.
FOURCC_BA81 = FOURCC('B', 'A', '8', '1'), // Alias for BGGR.
FOURCC_RGB3 = FOURCC('R', 'G', 'B', '3'), // Alias for RAW.
FOURCC_BGR3 = FOURCC('B', 'G', 'R', '3'), // Alias for 24BG.
FOURCC_CM32 = FOURCC(0, 0, 0, 32), // Alias for BGRA kCMPixelFormat_32ARGB
FOURCC_CM24 = FOURCC(0, 0, 0, 24), // Alias for RAW kCMPixelFormat_24RGB
FOURCC_L555 = FOURCC('L', '5', '5', '5'), // Alias for RGBO.
FOURCC_L565 = FOURCC('L', '5', '6', '5'), // Alias for RGBP.
FOURCC_5551 = FOURCC('5', '5', '5', '1'), // Alias for RGBO.
// 1 Auxiliary compressed YUV format set aside for capturer.
FOURCC_H264 = FOURCC('H', '2', '6', '4'),
The ARGB FOURCC
There are 4 ARGB layouts - ARGB, BGRA, ABGR and RGBA. ARGB is most common by far, used for screen formats, and windows webcam drivers.
The fourcc describes the order of channels in a register.
A fourcc provided by capturer, can be thought of string, e.g. “ARGB”.
On little endian machines, as an int, this would have ‘A’ in the lowest byte. The FOURCC macro reverses the order:
#define FOURCC(a, b, c, d) (((uint32)(a)) | ((uint32)(b) << 8) | ((uint32)(c) << 16) | ((uint32)(d) << 24))
So the “ARGB” string, read as an uint32, is
FOURCC_ARGB = FOURCC('A', 'R', 'G', 'B')
If you were to read ARGB pixels as uint32's, the alpha would be in the high byte, and the blue in the lowest byte. In memory, these are stored little endian, so ‘B’ is first, then ‘G’, ‘R’ and
 ‘A’ last.
When calling conversion functions, the names match the FOURCC, so in this case it would be I420ToARGB().
All formats can be converted to/from ARGB.
Most ‘planar_functions’ work on ARGB (e.g. ARGBBlend).
Some are channel order agnostic (e.g. ARGBScale).
Some functions are symmetric (e.g. ARGBToBGRA is the same as BGRAToARGB, so its a macro).
ARGBBlend expects preattenuated ARGB. The R,G,B are premultiplied by alpha. Other functions don't care.
四，rotation
Introduction
Rotation by multiplies of 90 degrees allows mobile devices to rotate webcams from landscape to portrait. The higher level functions ConvertToI420 and ConvertToARGB allow rotation of any format.
 Optimized functionality is supported for I420, ARGB, NV12 and NV21.
ConvertToI420
int ConvertToI420(const uint8* src_frame, size_t src_size,
uint8* dst_y, int dst_stride_y,
uint8* dst_u, int dst_stride_u,
uint8* dst_v, int dst_stride_v,
int crop_x, int crop_y,
int src_width, int src_height,
int crop_width, int crop_height,
enum RotationMode rotation,
uint32 format);
This function 
crops, converts, and rotates. You should think of it in that order.
Crops the original image, which is src_width x src_height, to crop_width x crop_height. At this point the image is still not rotated.
Converts the cropped region to I420. Supports inverted source for src_height negative.
Rotates by 90, 180 or 270 degrees. The buffer the caller provides should account for rotation. Be especially important to get stride of the destination correct.
e.g. 640 x 480 NV12 captured
Crop to 640 x 360
Rotate by 90 degrees to 360 x 640.
Caller passes stride of 360 for Y and 360 / 2 for U and V.
Caller passes crop_width of 640, crop_height of 360.
ConvertToARGB
int ConvertToARGB(const uint8* src_frame, size_t src_size,
uint8* dst_argb, int dst_stride_argb,
int crop_x, int crop_y,
int src_width, int src_height,
int crop_width, int crop_height,
enum RotationMode rotation,
uint32 format);
Same as I420, but implementation is less optimized - reads columns and writes rows, 16 bytes at a time.
I420Rotate
int I420Rotate(const uint8* src_y, int src_stride_y,
const uint8* src_u, int src_stride_u,
const uint8* src_v, int src_stride_v,
uint8* dst_y, int dst_stride_y,
uint8* dst_u, int dst_stride_u,
uint8* dst_v, int dst_stride_v,
int src_width, int src_height, enum RotationMode mode);
Destination is rotated, so pass dst_stride_y etc that consider rotation.
Rotate by 180 can be done in place, but 90 and 270 can not.
Implementation (Neon/SSE2) uses 8 x 8 block transpose, so best efficiency is with sizes and pointers that are aligned to 8.
Cropping can be achieved by adjusting the src_y/u/v pointers and src_width, src_height.
Lower level plane functions are provided, allowing other planar formats to be rotated. (e.g. I444)
For other planar YUV formats (I444, I422, I411, I400, NV16, NV24), the planar functions are exposed and can be called directly
// Rotate a plane by 0, 90, 180, or 270.
int RotatePlane(const uint8* src, int src_stride,
uint8* dst, int dst_stride,
int src_width, int src_height, enum RotationMode mode);
ARGBRotate
LIBYUV_API
int ARGBRotate(const uint8* src_argb, int src_stride_argb,
uint8* dst_argb, int dst_stride_argb,
int src_width, int src_height, enum RotationMode mode);
Same as I420, but implementation is less optimized - reads columns and writes rows.
Rotate by 90, or any angle, can be achieved using ARGBAffine.
Mirror - Horizontal Flip
Mirror functions for horizontally flipping an image, which can be useful for ‘self view’ of a webcam.
int I420Mirror(const uint8* src_y, int src_stride_y,
const uint8* src_u, int src_stride_u,
const uint8* src_v, int src_stride_v,
uint8* dst_y, int dst_stride_y,
uint8* dst_u, int dst_stride_u,
uint8* dst_v, int dst_stride_v,
int width, int height);
int ARGBMirror(const uint8* src_argb, int src_stride_argb,
uint8* dst_argb, int dst_stride_argb,
int width, int height);
Mirror functionality can also be achieved with the I420Scale and ARGBScale functions by passing negative width and/or height.
Invert - Vertical Flip
Inverting can be achieved with almost any libyuv function by passing a negative source height.
I420Mirror and ARGBMirror can also be used to rotate by 180 degrees by passing a negative height.
五，environment_variables
# Introduction
For test purposes, environment variables can be set to control libyuv behavior. These should only be used for testing, to narrow down bugs or to test performance.
# CPU
By default the cpu is detected and the most advanced form of SIMD is used. But you can disable instruction sets selectively, or completely, falling back on C code. Set the variable to 1 to disable
 the specified instruction set.
LIBYUV_DISABLE_ASM
LIBYUV_DISABLE_X86
LIBYUV_DISABLE_SSE2
LIBYUV_DISABLE_SSSE3
LIBYUV_DISABLE_SSE41
LIBYUV_DISABLE_SSE42
LIBYUV_DISABLE_AVX
LIBYUV_DISABLE_AVX2
LIBYUV_DISABLE_AVX3
LIBYUV_DISABLE_ERMS
LIBYUV_DISABLE_FMA3
LIBYUV_DISABLE_DSPR2
LIBYUV_DISABLE_NEON
# Test Width/Height/Repeat
The unittests default to a small image (128x72) to run fast. This can be set by environment variable to test a specific resolutions.
You can also repeat the test a specified number of iterations, allowing benchmarking and profiling.
set LIBYUV_WIDTH=1280
set LIBYUV_HEIGHT=720
set LIBYUV_REPEAT=999
set LIBYUV_FLAGS=-1
set LIBYUV_CPU_INFO=-1
