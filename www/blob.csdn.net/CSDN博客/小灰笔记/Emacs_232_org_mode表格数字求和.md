# Emacs_232_org_mode表格数字求和 - 小灰笔记 - CSDN博客





2018年07月24日 21:02:25[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：175








![](https://img-blog.csdn.net/20180724210201416?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       如上图所示，org-mode可以方便的进行表格的求和，上面截图中的11行是使用这种方式求出来的结果。

       求取方式：光标移动到相应列的下方，调用org-table-sum命令。计算出来的结果会在minibuffer中显示。同时，也被复制到了剪切板（ring）中，vim编辑模式下直接按p就可以粘贴结果到表格中。

       功能说不上很强，但是也已经足够强了。我对于表格的功能需求大概有几个：排序、插图、公式、着色标记、字体。

       目前的功能还是不够强大，后面慢慢学习掌握吧！



