# Release模式下加载FFmpeg提示“无法定位程序输入点XXX与动态链接库XXX.dll ”的问题解决 - 爱写代码的自由人 - CSDN博客





2018年02月23日 20:04:43[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：1030








   今天，遇到很奇怪的一个问题：我的一个程序在Debug模块下运行没问题，而在Release模式下运行报错了，启动程序的时候提示“无法定位程序输入点CoTaskMemFree 于动态链接库avcodec-56.dll”，这个程序引用了FFmpeg的DLL，我开始以为是DLL的问题，但是之前用这些DLL都没有出现错误啊！我检查了编译选项多次也没发现什么问题。后来通过上网搜，终于在一篇博文中找到了答案，见：http://blog.csdn.net/Solomon1558/article/details/45582249。谢谢这位仁兄，不然我都不知道对这个问题会头疼多久。
   这个问题通过修改工程属性里的一个选项就可以解决，打开工程的属性对话框，找到链接项目，在优化中有个引用项，选择保留未引用数据(/OPT:NOREF)即可，截图如下：


![](https://img-blog.csdn.net/20180223195851581)




下面的内容引用自原博文：

在MSDN上查找/OPT（优化）相关信息

REF | NOREF

/OPT:REF清除从未引用的函数和数据；

/OPT:NOREF保留从未引用的函数和数据。

当启用 /OFT:REF时，LINK会移除未引用的已打包函数和数据。 如果对象已经用 [/Gy](https://msdn.microsoft.com/zh-cn/library/xsa71f43.aspx) 选项编译过，它将包含打包的函数和数据 (COMDAT)。 此优化称为可传递的 COMDAT 消除。 默认情况下，在非调试生成中启用 /OPT:REF。 若要重写此默认值并在程序中保留未引用的 COMDAT，请指定 /OPT:NOREF。 可以使用 [/INCLUDE](https://msdn.microsoft.com/zh-cn/library/2s3hwbhs.aspx) 选项重写特定符号的移除。

在显式或默认启用 /OPT:REF后，将启用受限形式的 /OPT:ICF（仅会折叠相同的函数）。如果需要 /OPT:REF而不是 /OPT:ICF，则必须指定 /OPT:REF,NOICF 或 /OPT:NOICF。

如果指定了 [/DEBUG](https://msdn.microsoft.com/zh-cn/library/xe4t6fc1.aspx)，则 /OPT 的默认项是 NOREF，而且所有函数都保留在映像中。 若要重写此默认项并优化调试生成，请指定 /OPT:REF。 由于 /OPT:REF 隐式使用 /OPT:ICF，建议你同时指定 /OPT:NOICF 以在调试生成中保留相同的函数。 这样更容易读取堆栈跟踪以及在本应折叠在一起的函数中设置断点。 /OPT:REF 选项禁用增量链接。

你必须将 const数据显式标记为 COMDAT；使用 [__declspec(selectany)](https://msdn.microsoft.com/zh-cn/library/5tkz6s71.aspx)。

指定 /OPT:ICF不启用 /OPT:REF 选项。

ICF[= *iterations*]| NOICF

使用 /OPT:ICF[=*iterations*]执行相同的 COMDAT 折叠。 可以从链接器输出中删除冗余 COMDAT。 可选 *iterations* 参数指定遍历符号以查找重复项的次数。 默认迭代次数是两次。 附加的迭代可以找到更多前一次迭代中未通过折叠发现的重复项。

指定 /OPT:REF并且 ICF 默认为有效时的链接器行为方式与显式指定 /OPT:REF,ICF 时的行为方式不同。 单独使用 /OPT:REF 启用的 ICF 的窗体不折叠只读数据（包括 .rdata、.pdata 和 .xdata）。 因此，为 x64 生成映像时将折叠较少的函数，因为这些模块中的函数更依赖于只读数据（例如.pdata 和 .xdata）。 若要获取完整的 ICF 折叠行为，请显式指定 /OPT:ICF。

若要在 COMDAT中放置函数，请使用 /Gy 编译器选项；若要放置 const 数据，请将其声明为 __declspec(selectany)。 有关如何指定用于折叠的数据的详细信息，请参阅 [selectany](https://msdn.microsoft.com/zh-cn/library/5tkz6s71.aspx)。

默认情况下，如果 REF处于打开状态，则 ICF 处于打开状态。 若要重写此默认值，当指定 REF 时，请使用 NOICF。 当未在调试生成中指定 /OPT:ICF 时，你必须显式指定 /OPT:REF 以启用 COMDAT 折叠。 但是，由于 /OPT:ICF 能合并相同的数据或函数，因此它也能更改显示在堆栈跟踪中的函数名。 它还能使你无法在某些函数中设置断点或在调试器中检查某些数据，并让你在单步执行代码时进入意外的函数。 因此，建议不在调试生成中使用 /OPT:ICF，除非较小的代码的好处能弥补这些不足。







