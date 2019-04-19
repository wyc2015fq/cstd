# PNaCl：Google通过LLVM增强对Chrome中原生应用的支持 - SHINING的博客 - CSDN博客
2013年12月08日 15:56:48[snsn1984](https://me.csdn.net/snsn1984)阅读数：1887
作者 [Abel
 Avram](http://www.infoq.com/cn/author/Abel-Avram) ，译者 [臧秀涛](http://www.infoq.com/cn/author/%E8%87%A7%E7%A7%80%E6%B6%9B)发布于
 五月 20, 2013
通过支持运行LLVM位码（bitcode），Google增强了对Chrome中原生应用的支持。
在[Google I/O 2013](https://developers.google.com/events/io/)大会上，Google宣布了[预期](http://blog.chromium.org/2013/01/native-client-support-on-arm.html)加入的[PNaCl](https://developers.google.com/native-client/announcements)（Portable
 NaCl，读作“pinnacle”），继续推进对Chrome中原生应用的支持。PNaCl修改了原来的工具链，与之前为每种目标平台编译C/C++应用不同，现在开发者只需生成一份[LLVM](http://llvm.org/)位码，之后位码可以由任一Chrome客户端加载，在本地转换为原生代码、验证和执行。
目前，x86-32、x86-64和ARM架构上的Chrome 29已支持PNaCl。在PNaCl完整发布之后，Chrome客户端能够在任意页面上运行原生模块，而无需将其打包为面向Chrome Web Store的应用。此外，PNaCl模块无需重新编译即可在未来的架构上运行。更为有趣的是，尽管目前只支持C/C++，理论上PNaCl模块可以[以任何能够编译为LLVM位码的语言](http://en.wikipedia.org/wiki/LLVM)编写，包括ActionScript、Ada、D、Fortran、Haskell、Java字节码、Objective-C、Python、Ruby、Rust、Scala和C#等，拓宽了可以在Chrome中运行的遗留的和较新的原生应用的使用范围，这些应用只需要很小的调整，甚至无需调整。
2010年，4位Google工程师组成的一个团队在内部提出了PnaCl（[PDF](http://css.csail.mit.edu/6.858/2012/readings/pnacl.pdf)），几位作者想象的是这样一种解决方案：
- 
为编译的NaCl模块提供一种独立于指令集体系结构（ISA）的格式，无需重新编译即可支持多种目标平台。
- 
便于NaCl开发者构建、测试和部署可移植的执行模块。
- 
首先要支持x86-32、x86-64和ARM指令集，但要很容易支持未来其他流行的通用处理器。
- 
能够保持原生客户端所拥有的安全和性能属性。
PNaCl仍然使用原来的NaCl沙盒机制，以保证运行翻译位码时的安全性。具体逻辑如下图所示，客户端从服务器请求位码，然后针对自己的架构将其转换为原生的可执行代码。翻译可能在本地进行（在另一个NaCI沙盒内），也可能通过另一个实体远程执行。
![](http://infoqstatic.com/resource/news/2013/05/pnacl-google-chrome-llvm/zh/resources/1.png)
PnaCl开发工具链支持Windows、Mac和Linux，[SDK](https://developers.google.com/native-client/sdk/)可以从[项目网站下载](https://developers.google.com/native-client/)。
查看英文原文：[PNaCl:
 Google Adds More Native Support to Chrome via LLVM](http://www.infoq.com/news/2013/05/pnacl-google-chrome-llvm)
## Portable Native Client (PNaCl)
May 15, 2013
Portable Native Client (PNaCl, pronounced "pinnacle"), is an architecture-independent version of Native Client. Traditional C and C++ development uses a "compile → link" workflow that produces a platform- and architecture-dependent executable. In contrast,
 PNaCl development uses an [LLVM](http://en.wikipedia.org/wiki/LLVM) compiler infrastructure
 with a "compile → link → translate" workflow. This workflow produces a "linked" binary that is provided as intermediate representation (IR) bitcode; the bitcode is then translated locally for a specific end-user system architecture. In conjunction with Native
 Client's [POSIX](http://en.wikipedia.org/wiki/POSIX)-like environment and the [Pepper
 API](https://developers.google.com/native-client/overview#pepper) for media interfaces, PNaCl provides complete platform independence. With PNaCl, developers can create a single executable for all users. Because translation happens at the client, developers get the benefit of translations for new architectures, as
 well as new translation optimizations, for free, as such features become available and without the need to rebuild their applications. Once PNaCl is fully released, users will be able to run PNaCl modules on any web page – applications will not need to be
 deployed through the Chrome Web Store in order to run PNaCl modules.
For background information about PNacl, see [PNaCl:
 Portable Native Client Executables](http://nativeclient.googlecode.com/svn/data/site/pnacl.pdf) (PDF). For current release information, see the [release
 notes](https://developers.google.com/native-client/sdk/release-notes#pnacl_0).
