# 在msys里进行复制和粘贴操作 - DoubleLi - 博客园






You can copy text from an MSYS window to the clipboard simply by selecting the text you want to copy. As the text selection changes the clipboard is updated to contain the selected text. To paste text from the clipboard into an MSYS window either click the left button while holding down the Shift key, or else click the mouse wheel (clicking anywhere in the MSYS window will do). Pasting text you've copied from MSYS into another program (e.g. Microsoft Word) is accomplished using the normal mechanism(s) supported by the other program (e.g. Ctrl-V).


An alternative way to copy output from MSYS is to redirect it to /dev/clipboard. E.g. you can copy the contents of file foo to the clipboard using |cat foo > /dev/clipboard| and then paste it elsewhere.




粘贴：在任何地方复制需要粘贴的文本，在msys里面按住shift+左键，就可以完成粘贴操作。

复制：在msys里面用左键选中文本，松开左键，用右键点击，即可把选中内容拷贝到剪切板里。



在windows7下：

粘贴：快捷键ctrl+insert，或者右键点击控制台标题栏上的应用程序图标，选择编辑---粘贴

复制：右键点击控制台标题栏上的应用程序图标，选择编辑---标记，选中文本后按回车键(Enter)









