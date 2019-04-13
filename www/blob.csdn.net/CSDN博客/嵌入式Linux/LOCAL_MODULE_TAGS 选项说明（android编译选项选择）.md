
# LOCAL_MODULE_TAGS 选项说明（android编译选项选择） - 嵌入式Linux - CSDN博客

2014年03月12日 15:03:35[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1943


LOCAL_MODULE_TAGS ：=user eng tests optional
user: 指该模块只在user版本下才编译
eng: 指该模块只在eng版本下才编译
tests: 指该模块只在tests版本下才编译
optional:指该模块在所有版本下都编译
eng This is the default flavor. A plain "make" is the same as "make eng". droid is an alias for eng.
* Installs modules tagged with: eng, debug, user, and/or development.
* Installs non-APK modules that have no tags specified.
* Installs APKs according to the product definition files, in addition to tagged APKs.
* ro.secure=0
* ro.debuggable=1
* ro.kernel.android.checkjni=1
* adb is enabled by default.
user "make user"This is the flavor intended to be the final release
 bits.
* Installs modules tagged with user.
* Installs non-APK modules that have no tags specified.
* Installs APKs according to the product definition files; tags are ignored for APK modules.
* ro.secure=1
* ro.debuggable=0
* adb is disabled by default.
userdebug "make userdebug"The same as user, except:
* Also installs modules tagged with debug.
* ro.debuggable=1
* adb is enabled by default.

### Build flavors/types
When building for a particular product, it's often useful to have minor variations on what is ultimately the final release build. These are the currently-defined "flavors" or "types" (we need to settle on a real name for these).
|eng|This is the default flavor. A plain "|make|" is the same as "|make eng|".|droid|is an alias for|eng|.|Installs modules tagged with:|eng|,|debug|,|user|, and/or|development|.
|Installs non-APK modules that have no tags specified.
|Installs APKs according to the product definition files, in addition to tagged APKs.
|ro.secure=0
|ro.debuggable=1
|ro.kernel.android.checkjni=1
|adb|is enabled by default.
|
|user|"|make user|"|This is the flavor intended to be the final release bits.
|Installs modules tagged with|user|.
|Installs non-APK modules that have no tags specified.
|Installs APKs according to the product definition files; tags are ignored for APK modules.
|ro.secure=1
|ro.debuggable=0
|adb|is disabled by default.
|
|userdebug|"|make userdebug|"|The same as|user|, except:
|Also installs modules tagged with|debug|.
|ro.debuggable=1
|adb|is enabled by default.
|
If you build one flavor and then want to build another, you should run "`make installclean`" between the two makes to guarantee that you don't pick up files installed by the previous flavor. "`make clean`" will also suffice, but it takes
 a lot longer.

