# JbuilderX的修改方法 - 逍遥剑客 - CSDN博客
2006年11月30日 21:58:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1212
打开JBuilderX/bin/jbuilder.config文件，在
#TunethisVMtoprovideenoughheadroomtoworkonlarge
#applications
vmparam-Xms32m
vmparam-Xmx256m
后面追加一行
vmparam-Dprimetime.editor.useVariableWidthFont=true
此方法方法，在不同的系统和机器上表现的结果可能会不尽相同，大家自己研究吧。
可以把%system%/document．andSettings/you_user
目录下的/.primetimeX和/.jbuilderX目录删掉，然后再启动JBuilderX试试，一般就正常了。这时，可以看到“Tools”—“Perferences”中“Edit”—“Display”的“Fontfamily”选项为“DialogInput”。无论中文显示还光标位置应该都没有问题了，而且即使系统格掉重装，也不会有影响，不需要再重新设置。
