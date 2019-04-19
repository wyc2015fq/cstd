# EasyDarwin开源流媒体云平台VS调试断点提示“还没有为该文档加载任何符号”的解决办法 - xcyl的口袋 - CSDN博客
2016年07月29日 12:33:56[xcyl](https://me.csdn.net/cai6811376)阅读数：959
近日，我们EasyDarwin团队在调试EasyDarwin的时候遇到有些C++源文件无法命中断点，提示“当前不会命中断点。还没有为该文档加载任何符号”。
这里是因为，有些源文件的调试信息被禁用了，生成不了调试符号。只需要将源文件的“调试信息格式”设置一下就可以了。
> 
**调试信息格式说明**
[/Z7、/Zi、/ZI（调试信息格式）](https://msdn.microsoft.com/zh-cn/library/958x11bc%28VS.80%29.aspx)
*无*
  不产生任何调试信息，因此编译较快。
*/Z7*
  生成包含用于调试器的完整符号调试信息的 .obj 文件。符号调试信息包括变量的名称和类型以及函数和行号。不会生成任何 .pdb 文件。 
  对于第三方库的发布者，不生成 .pdb 文件是一个优点。但是，在链接和调试期间，用于预编译头的 .obj 文件是必需的。如果 .pch 对象文件中只有类型信息（没有代码），则还必须使用 /Yl（为调试库插入 PCH 引用） 进行编译。
*/Zi*
  产生包含用于调试器的类型信息和符号调试信息的程序数据库 (PDB)。符号调试信息包括变量的名称和类型以及函数和行号。 
  /Zi 不影响优化。但是，/Zi 的确暗示了 /debug；有关更多信息，请参见 /DEBUG（生成调试信息）。 
  类型信息放置在 .pdb 文件而不是 .obj 文件中。 
  可以将 /Gm（启用最小重新生成） 和 /Zi 结合使用，但使用 /Z7 编译时不能使用 /Gm。 
  使用 /Zi 和 /clr 编译时，DebuggableAttribute 属性将不会被放入程序集元数据中；如果要使用该属性，则必须在源代码中指定它。该属性可影响应用程序的运行时性能。有关 Debuggable 属性如何影响性能以及如何减轻性能影响的更多信息，请参见令映像更易于调试。
*/ZI*
  以支持“编辑并继续”功能的格式产生如上所述的程序数据库。如果想使用“编辑并继续”调试，则必须使用此选项。因为大多数优化与“编辑并继续”不兼容，所以使用 /ZI 会禁用代码中的所有 #pragma optimize 语句。 
  /ZI 会导致在编译中使用 /Gy（启用函数级链接）。 
  /ZI 与 /clr（公共语言运行库编译） 不兼容。
*注意*
  /ZI 只可用于面向 x86 的编译器中；此编译器选项不可用于面向 x64 或 Itanium 处理器系列 (IPF) 的编译器中。
将源文件的“调试信息格式”改为“用于编辑并继续的程序数据库(ZI)”
![PIC](https://img-blog.csdn.net/20160729123206784)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：288214068
Copyright © EasyDarwin.org 2012-2015
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
