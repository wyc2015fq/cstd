# WebRTC编译系统之GYP，gn和ninja - starRTC免费im直播会议一对一视频 - CSDN博客
2017年12月11日 15:32:12[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：300
GN(Generate Ninja)来生成构建脚本，使用 ninja 来构建。
gn 的介绍在这里：[https://www.chromium.org/developers/gn-build-configuration](https://www.chromium.org/developers/gn-build-configuration)
使用 gn 生成 ninja 构建文件的常用命令：
// 生成 debug 版本的构建文件，默认配置gn gen out/Debug // 生成 release 版本的构建文件gn gen out/Release --args="is_debug=false"
注意，通过 --args 可以传递参数给
 gn ，具体参数的含义，由 WebRTC 的构建系统来解释。比如 is_debug 选项，决定构建 debug 还是 release 版本。
如果你已经使用 gn gen 生成过构建文件，想看看这个版本的构建文件都指定了什么参数，可以使用下面命令：
gn args out/Release --list
它会列出所有的 build arguments 和对应的文档，以及当前值。


ninja 的官网在这里：[https://ninja-build.org/](https://ninja-build.org/)。
后缀为 ninja（*.ninja） 的文件是 ninja 的 构建文件。对 WebRTC 来讲，执行完 gn gen 之后，会在 out/Release 下生成 build.ninja 文件，可以把这个文件看做是整个 WebRTC 的“ Makefile ”。它里面调用了各个模块的 ninja 文件。
要完整编译 WebRTC ，只要在 src 目录执行下列命令：
ninja -C out/Release
-C 选项告诉 ninja ，进入 out/Release 目录来编译。所以，它等同于：
cd out/Releaseninja
要编译某个模块，可以在 ninja 命令后跟模块名字（build.ninja文件中定义的构建目标，就像 Makefile 中的构建目标一样）。比如：
// 构建 webrtc/pcninja pc // 构建 webrtc/medianinja media

看看 gn 用到的项目文件 .gn 、 .gni 和 DEPS ，它们指导了如何生成 ninja 构建文件。
如果把 gn 看成一个编译系统， .gn 就是源文件， .gni 就是头文件。我们姑且这么理解就好了（其实 gni 里做的事情， gn 都可以做）。DEPS 主要用来设定包含路径。
gn 和 gni 文件都在源码树中，比如 src 目录。当执行 gn gen 时，gn 工具根据 gn 和 gni 生成 ninja 文件并将这些 ninja 文件放到指定的构建目录中。
.gn 文件是 GN build 的 “源文件”，在这里可以做各种条件判断和配置，gn 会根据这些配置生成特定的 ninja 文件。
.gn 文件中可以使用预定义的参数，比如 is_debug ， target_os ， rtc_use_h264 等。
.gn 中可以 import .gni 文件。
.gn 和 .gni 文件中用到各种指令，都在这里有说明：[GN
 Reference](https://chromium.googlesource.com/chromium/src/+/master/tools/gn/docs/reference.md)。
import("webrtc/webrtc.gni")group("default") { testonly = true deps = [ "//webrtc", "//webrtc/examples", "//webrtc/tools", ] if (rtc_include_tests)
 { deps += [ "//webrtc:webrtc_tests" ] }}
group 指令声明了一个 default 目标，这个目标依赖 webrtc 、 webrtc/examples 和 webrtc/tools ，你可以在 webrtc 、 webrtc/examples 、 webrtc/tools 目录下找到对应的 BUILD.gn 。你可以把 group 当做 VS 的
 solution
gn 文件中也可以通过 defines 来定义宏，通过 cflags 来指定传递给编译器的标记，通过 ldflags 指定传递给链接器的标记，还可以使用 sources 指定源文件。下面是 webrtc/BUILD.gn 文件的部分内容：
if (is_win) {
defines += [ "WEBRTC_WIN", "_CRT_SECURE_NO_WARNINGS", # Suppress warnings about _vsnprinf ] } if (is_android) { defines += [ "WEBRTC_LINUX",
 "WEBRTC_ANDROID", ] } if (is_chromeos) { defines += [ "CHROMEOS" ] } if (rtc_sanitize_coverage != "") { assert(is_clang, "sanitizer coverage requires clang")
cflags += [ "-fsanitize-coverage=${rtc_sanitize_coverage}" ]
ldflags += [ "-fsanitize-coverage=${rtc_sanitize_coverage}" ] }
gni 文件是 GN build 使用的头文件，它里面可以做各种事情，比如定义变量、宏、定义配置、定义模板等。
webrtc.gni 是一个比较特殊的 gni 文件，你可以把它看做全局配置文件。
webrtc.gni 定义了 WebRTC 项目用到的一些标记，比如 rtc_build_libvpx、rtc_build_ssl、rtc_use_h264 等。
还使用
template 语句定义了几个模板，比如 rtc_executable 、 rtc_static_library 、 rtc_shared_library ，这几个模板定义了生成可执行文件、静态库、动态库的规则。在
webrtc/examples/BUILD.gn 中就有用到这些模板，用它们来指导如何生成可执行文件、静态库等。
你也可以直接使用 gn 内置的 shared_library 和 static_library 来声明目标，比如 third_party/ffmpeg/BUILD.gn 就使用 shared_library 来生成动态库。
DEPS 文件
webrtc/examples/DEPS ：
include_rules = [ "+WebRTC", "+webrtc/api", "+webrtc/base", "+webrtc/media", "+webrtc/modules/audio_device", "+webrtc/modules/video_capture",
 "+webrtc/p2p", "+webrtc/pc",]
include_rules 定义了包含路径。
了解 .gn 和 .gni 文件的目的是修改它们。比如你想打开 WebRTC 对 H264 的支持，就可以修改 webrtc/webrtc.gni ，直接把 rtc_use_h264 设置为 true 。
比如你想为某个模块加一些文件，就可以修改 .gn 文件，修改 sources 变量，直接把你的源文件加进去。
GYP是一个在不同平台构建项目的工具，GN是GYP的升级版
GYP是Generate
 Your Projects的缩写，GYP的目的是为了支持更大的项目编译在不同的平台，比如Mac，Windows，Linux，它可以生成Xcode工程，Visual
 Studio工程，Ninja编译文件和Mackefiles。
GYP的输入是.gyp和.gypi文件，.gypi文件是用于.gyp文件include使用的。.gyp文件就是符合特定格式的json文件。
用gn gen指定在out/目录里面生成ninja。
|1|gn gen out|
再执行ninja来build code
|1|ninja -C out|
在src目录有一个.gn的隐藏文件
import("//build/dotfile_settings.gni")
# The location of the build configuration file.
buildconfig
= "//build/config/BUILDCONFIG.gn"
# The secondary source root is a parallel directory tree where
# GN build files are placed when they can not be placed directly
# in the source tree, e.g. for third party source trees.
secondary_source = "//build/secondary/"
# These are the targets to check headers for by default. The files in targets
# matching these patterns (see "gn help label_pattern" for format) will have
# their includes checked for proper dependencies when you run either
# "gn check" or "gn gen --check".
check_targets = [ "//webrtc/*" ]
# These are the list of GN files that run exec_script. This whitelist exists
# to force additional review for new uses of exec_script, which is strongly
# discouraged except for gypi_to_gn calls.
exec_script_whitelist = build_dotfile_settings.exec_script_whitelist
default_args
= {
# Webrtc does not support component builds because we are not using the
# template "component" but we rely directly on "rtc_static_library" and
# "rtc_shared_library". This means that we cannot use the chromium default
# value for this argument.
# This also means that the user can override this value using --args or
# the args.gn file but this setting will be ignored because we don't support
# component builds.
is_component_build = false
}
.gn 檔所在的目錄會被 GN 工具認定是
 project 的 **source root**，.gn 的內容最基本就是用 buildconfig 來指定
 build config 檔的位置，其中 // 開頭的字串就是用來指定相對於
 source root 的路徑。
- [args: Display or configure arguments declared by the build.](https://chromium.googlesource.com/chromium/src/+/master/tools/gn/docs/reference.md#args)
- [gen: Generate ninja files.](https://chromium.googlesource.com/chromium/src/+/master/tools/gn/docs/reference.md#gen)
args命令应该是产生args.gn文件的。
gen命令应该是根据args.gn生成ninja文件的。

If specified, arguments from the --args command line flag are used. If that flag is not specified, args from previous builds
 in the build directory will be used (this is in the file args.gn in the build directory).
gn gen out/FooBar --args="enable_doom_melon=true os=\"android\"" This will overwrite the build directory with the given arguments.
build flow流程
1. Look for ".gn" file (see "gn help dotfile") in the current directory and walk up the directory tree until one is found. Set
 this directory to be the "source root" and interpret this file to find the name of the build config file. 2. Execute the build config file
identified by .gn
to set up the global variables and default toolchain name. Any arguments, variables, defaults, etc. set up in this file will be visible to all files in the build. 3. Load the
 //BUILD.gn (in the source root directory). 4.
Recursively
evaluate rules and
load BUILD.gn in other directories
as necessary to resolve dependencies. If a BUILD file isn't found in the specified location, GN will look in the corresponding location inside the secondary_source defined in the dotfile
 (see "gn help dotfile"). 5. When a target's dependencies are resolved, write out the `.ninja` file to disk.
 6. When all targets are resolved, write out the root build.ninja file.

ninja -t
-t flag on the Ninja command line runs some tools that we have found useful during Ninja’s development. The current tools are:
ninja -t clean
remove built files. By default it removes all built files except for those created by the generator.
Adding the -g flag also removes built files created by the generator
更多ninja -h
参考：
[http://blog.csdn.net/foruok/article/details/70050342](http://blog.csdn.net/foruok/article/details/70050342)
[http://www.alonemonkey.com/2016/06/15/gn-gyp-ninja/](http://www.alonemonkey.com/2016/06/15/gn-gyp-ninja/)
[https://chromium.googlesource.com/chromium/src/+/master/tools/gn/docs/reference.md](https://chromium.googlesource.com/chromium/src/+/master/tools/gn/docs/reference.md)
[https://chromium.googlesource.com/chromium/src/+/master/tools/gn/docs/quick_start.md](https://chromium.googlesource.com/chromium/src/+/master/tools/gn/docs/quick_start.md)
