# ChakraCore现在可以在Linux和Mac OS上运行了 - weixin_33985507的博客 - CSDN博客
2016年08月01日 19:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
微软演示了在Linux和Mac OS X上运行ChakraCore以及在Linux上运行Node.js/ChakraCore。
\\
六个月之前，我们[曾对Chakra引擎开源进行过报道](%5C)。微软的目的是将其移植到其他平台，并让Node.js可以在上面运行。7月27日，在这些天于旧金山举行的[Node峰会2016](%5C)上，微软演示了ChakraCore在Linux x64和Mac OS X 10.9+上运行，其中包括在运行于Linux x64之上的ChakraCore上运行Node.js（尚处于实验阶段）。
\\
微软之所以开展这项跨平台工作，是为了帮助开发人员编写可以运行在Windows UWP和其他平台上的应用程序。UWP是微软为了让应用程序可以跨各种不同类型的Windows设备运行而提出的解决方案，包括桌面、移动、IoT、Xbox、Surface Hubs和HoloLens。除C#、C++和Visual Basic之外，UWP应用程序还可以使用HTML/JavaScript编写，并运行在所有这些设备的Chakra上。由于ChakraCore是Chakra去掉了一些Windows相关的API，所以，开发人员现在可以将其运行范围扩展到Linux和Mac OS X上。借助[Windows Bridge for iOS](%5C)，许多iOS应用程序都可以移植到Windows上，尤其是手机和平板。由于缺少一个好的应用程序生态系统，微软在这两个领域似乎没有什么发展。
\\
微软原本计划通过[Astoria](%5C)项目打通Android和Windows，但他们[放弃了这个想法](%5C)。开发人员仍然可以通过Xamarin将应用移植到UWP，尽管那不是一种直观的方法。不过，考虑到大部分移动开发人员都面向iOS开发，他们可能不需要一个Android-UWP桥，如果他们有兴趣，Xamarin足以让他们将Objective-C代码移植到Windows平台了。
\\
按照[微软的说法](%5C)，除了[Intl API](%5C)正在开发中之外，ChakraCore在JavaScript特性支持方面已经和Chakra/Windows不相上下。JIT和部分GC特性尚不完善，但他们计划充分完善它们的功能，让它们具有和在Windows上同样的性能。
\\
微软不只是将桌面应用程序作为目标，他们还希望使用一个运行在ChakraCore上的Node.js分支提供服务器应用的跨平台支持。感兴趣的读者可以从[这里](%5C)下载用于Windows和Linux的预构建二进制文件。
\\
如果希望了解有关Node.js如何在ChakraCore上运行的技术细节，我们建议您阅读文章《[微软开源Chakra，并计划在上面运行Node.js](%5C)》。
\\
**查看英文原文**：[ChakraCore Works Now on Linux and Mac OS](%5C)
