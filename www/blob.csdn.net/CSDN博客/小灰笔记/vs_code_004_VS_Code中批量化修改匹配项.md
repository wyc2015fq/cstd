# vs_code_004_VS_Code中批量化修改匹配项 - 小灰笔记 - CSDN博客





2018年10月08日 22:12:59[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1783








         批量化修改匹配项，这是我在VS Code中发现的又一个默认自带的高效工具。如果是在emacs中，由于我一直使用vim模式，在evil中使用正则表达式替换也是一种高效的方式。不过，相比之下，VS Code的简单容易上手以及便捷的操作在这方面已经做得非常好了。

         拿之前的一段简单的代码来演示一下：

![](https://img-blog.csdn.net/20181008221244611?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

         假如，我想把上面代码中的int全都替换成int32_t。除了各种替换之外，现在已经有了一个更见便捷直观的方式。选中其中的一个int，然后鼠标右键选择更改所有匹配项（功能也提供了一个组合键，不过这个组合键的键位比起emacs的那种方式来说还是有些不舒服）：

![](https://img-blog.csdn.net/20181008221244808?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

         修改的过程中，光标会在匹配项上进行闪烁，同时能够看到所有的匹配项同时方式变化。

![](https://img-blog.csdn.net/20181008221244810?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

         这种简单易操作的方式确实是非常棒，作为一个编辑器来说，简单易用且功能强大，VS Code做的确实是很好了。





