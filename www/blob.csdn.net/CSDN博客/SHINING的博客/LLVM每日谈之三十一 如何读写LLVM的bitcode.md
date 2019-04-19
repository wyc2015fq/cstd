# LLVM每日谈之三十一 如何读写LLVM的bitcode - SHINING的博客 - CSDN博客
2018年05月14日 20:56:52[snsn1984](https://me.csdn.net/snsn1984)阅读数：1056
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
      LLVM的IR在整个LLVM工程中占据着核心地位，它是整个LLVM三个阶段的中间环节，起着承上启下的作用。如何读写LLVM的bitcode在LLVM的前端和后端都会涉及这个问题，在基于LLVM做一些定制化开发的时候，对LLVM的bitcode也是重中之重。
       LLVM的官方文档在这个方面介绍很详细，但是随着LLVM的扩充，官方文档也越来越厚重，有的时候只是简单的想做某个事，可能需要查找好几个文档，并且要结合代码。Neil Henning写了一篇介绍如何读写LLVM的bitcode的文章，简单易读，内容非常详细，从安装LLVM到CMAKE文件，到最后每步代码做什么用，都介绍的很清楚。非常适合就想解决这个问题的朋友读，并且附上了示例代码。博客地址：[How to read & write LLVM bitcode](https://link.zhihu.com/?target=http%3A//www.duskborn.com/how-to-read-write-llvm-bitcode/)
Github代码地址：[sheredom/llvm_bc_parsing_example](https://link.zhihu.com/?target=https%3A//github.com/sheredom/llvm_bc_parsing_example)
----------------------------------------------------------------------------------------------
为了防止抽风这个博客打不开，将博客全文附录如下：
## How to read & write LLVM bitcode
I’ve read multiple posts on social media now complaining about how scary LLVM is to get to grips with. It doesn’t help that the repository is ginormous, there are frequently hundreds of commits a day, the mailing list is nearly impossible to keep track of, and the resulting executables are topping 40Mb now…
Those tidbits aside – LLVM is *super* easy to work with once you get to grips with the beast that it is. To help aid people in using LLVM, I thought I’d put together the most trivial no-op example you can do with LLVM – parsing one of LLVM’s intermediate representation files (known as bitcode, file extension .bc) and then writing it back out.
Firstly, lets go through some high level LLVM terms:
- LLVM’s main abstraction for user code is the *Module*. It’s a class that contains all the functions, global variables, and instructions for the code you or other users write.
- Bitcode files are effectively a serialization of an LLVM Module such that it can be reconstructed in a different program later.
- LLVM uses *MemoryBuffer* objects to handle data that comes from files, stdin, or arrays.
For my example, we’ll use the LLVM C API – a more stable abstraction ontop of LLVM’s core C++ headers. The C API is really useful if you’ve got code that you want to work with multiple versions of LLVM, it’s significantly more stable than the LLVM C++ headers. (An aside, I use LLVM extensively for my job and nearly every week some LLVM C++ header change will break our code. I’ve never had the C API break my code.)
First off, I’m going to assume you’ve pulled LLVM, built and installed it. Some simple steps to do this:
git clone [https://git.llvm.org/git/llvm.git](https://link.zhihu.com/?target=https%3A//git.llvm.org/git/llvm.git) <llvm dir>
cd <llvm dir>
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=install ..
cmake --build . --target install
After doing the above, you’ll have an LLVM install in <llvm dir>/build/install!
So for our little executable I’ve used CMake. CMake is by far the easiest way to integrate with LLVM as it is the build system LLVM also uses.
project(llvm_bc_parsing_example)
cmake_minimum_required(VERSION 3.4.3)
# option to allow a user to specify where an LLVM install is on the system
set(LLVM_INSTALL_DIR "" CACHE STRING "An LLVM install directory.")
if("${LLVM_INSTALL_DIR}" STREQUAL "")
  message(FATAL_ERROR "LLVM_INSTALL_DIR not set! Set it to the location of an LLVM install.")
endif()
# fixup paths to only use the Linux convention
string(REPLACE "\\" "/" LLVM_INSTALL_DIR ${LLVM_INSTALL_DIR})
# tell CMake where LLVM's module is
list(APPEND CMAKE_MODULE_PATH ${LLVM_INSTALL_DIR}/lib/cmake/llvm)
# include LLVM
include(LLVMConfig)
add_executable(llvm_bc_parsing_example main.c)
target_include_directories(llvm_bc_parsing_example PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(llvm_bc_parsing_example PUBLIC LLVMBitReader LLVMBitWriter)
So now we’ve got our CMake setup, and we can use our existing LLVM install, we can now get working on our actual C code!
So to use the LLVM C API there is one header you basically always need:
#include <llvm-c/Core.h>
And two extra headers we need for our executable are the bitcode reader and writer:
#include <llvm-c/BitReader.h>
#include <llvm-c/BitWriter.h>
Now we create our main function. I’m assuming here that we always take exactly 2 command line arguments, the first being the input file, the second being the output file. LLVM has a system whereby if a file named ‘-‘ is provided, that means read from stdin or write to stdout, so I decided to support that too:
if (3 != argc) {
  fprintf(stderr, "Invalid command line!\n");
  return 1;
}
const char *const inputFilename = argv[1];
const char *const outputFilename = argv[2];
So first we parse the input file. We’ll create an LLVM memory buffer object from either stdin, or a filename:
LLVMMemoryBufferRef memoryBuffer;
// check if we are to read our input file from stdin
if (('-' == inputFilename[0]) && ('\0' == inputFilename[1])) {
  char *message;
  if (0 != LLVMCreateMemoryBufferWithSTDIN(&memoryBuffer, &message)) {
    fprintf(stderr, "%s\n", message);
    free(message);
    return 1;
  }
} else {
  char *message;
  if (0 != LLVMCreateMemoryBufferWithContentsOfFile(
               inputFilename, &memoryBuffer, &message)) {
    fprintf(stderr, "%s\n", message);
    free(message);
    return 1;
  }
}
So after this code, memoryBuffer will be usable to read our bitcode file into an LLVM module. So lets create the module!
// now create our module using the memory buffer
LLVMModuleRef module;
if (0 != LLVMParseBitcode2(memoryBuffer, &module)) {
  fprintf(stderr, "Invalid bitcode detected!\n");
  LLVMDisposeMemoryBuffer(memoryBuffer);
  return 1;
}
// done with the memory buffer now, so dispose of it
LLVMDisposeMemoryBuffer(memoryBuffer);
Once we’ve got our module, we no longer need the memory buffer, so we can free up the memory straight away. And that’s it! We’ve managed to take an LLVM bitcode file, deserialize it into an LLVM module, which we *could* (I’m not going to in this blog post at least!) fiddle with. So lets assume you’ve done all you wanted with the LLVM module, and want to write the sucker back out to a bitcode file again.
The approach is orthogonal to the reading approach, we look for the special filename ‘-‘ and handle accordingly:
// check if we are to write our output file to stdout
if (('-' == outputFilename[0]) && ('\0' == outputFilename[1])) {
  if (0 != LLVMWriteBitcodeToFD(module, STDOUT_FILENO, 0, 0)) {
    fprintf(stderr, "Failed to write bitcode to stdout!\n");
    LLVMDisposeModule(module);
    return 1;
  }
} else {
  if (0 != LLVMWriteBitcodeToFile(module, outputFilename)) {
    fprintf(stderr, "Failed to write bitcode to file!\n");
    LLVMDisposeModule(module);
    return 1;
  }
}
Lastly, we should be good citizens and clean up our garbage, so also delete the module to:
LLVMDisposeModule(module);
And that’s it! You are now able to parse and then write an LLVM bitcode file. I’ve put the full example up here on GitHub – [https://github.com/sheredom/llvm_bc_parsing_example](https://link.zhihu.com/?target=https%3A//github.com/sheredom/llvm_bc_parsing_example).
Maybe I’ll do a follow-up post at some point taking you through the basics of how to do things to an LLVM module, but for now, adieu!
