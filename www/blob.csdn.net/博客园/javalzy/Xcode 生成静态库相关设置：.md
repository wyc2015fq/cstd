# Xcode 生成静态库相关设置： - javalzy - 博客园
# [Xcode 生成静态库相关设置：](https://www.cnblogs.com/javalzy/p/6151921.html)
Xcode 生成静态库相关设置：
#Build Setting
1. Architectures  
   ------- Architectures -----> $(ARCHS_STANDARD) --- Armv7, Arm64
   ------- Build Active Architecture Only --> NO
           若选项设为YES，在其编译时只生成当前机器的框架。 将其设置为NO后，发现用模拟器编译后生成的framework同时包含x86_64和i386架构。
2. Build Option:
   ----- Enable Bitcode   ----> YES
   Bitcode是苹果在Xcode7及以后推出的新功能。用于代码的二次编译，针对CPU进行优化，编译工作由苹果AppStore后台来完成。针对iOS是可选项，默认打开。watchOS 和 tvOS 是必选项。所以需要库工程的 Apply LLVM 8.0 - Custom Compiler Flags 此选项并加上-fembed-bitcode参数，重新编译 
   ----- Other C Flags    ----> -fembed-bitcode
   ----- Other C++ Flags  ----> -fembed-bitcode
3. Deployment:
   ----- Strip Debug Symbols During Copy  -------> No
   ----- Strip Style ------> No-Global Symbols
4. Linking
   ----- Dead Code Stripping ----> NO
   ----- Link With Standard Libraries  -----> NO
   ----- Mach-O Type  -----> Static Library
   ----- Other Linker Flags   --------  -Objc
# Scheme  
  ------ Generic ios Device
  ------ Editor Scheme ---> Release
# Build Phases
  ------------ Headers ---- Public Project 设置
# 使用 
1. Build Setting: Search Path
   ---- Header Search Paths: ------- 加上.a 或.framework库的Headers的路径.
#命令
1. 类库合并命令：lipo -create +上面两个文件的路径 + -output + 合成后文件的输出路径 
2. 用lipo命令查看： lipo -info 类库

