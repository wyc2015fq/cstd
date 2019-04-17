# 用咒语操作PC——Voice Activated Commands语音识别 - 迭代的是人，递归的是神 - CSDN博客





2011年12月15日 07:50:11[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：3690








年底了，事情比较多，大家也都是如此吧~~

今天咱不谈OpenCV了，也给大家轻松轻松。说一个我网上无意中遇到的软件Voice Activated Commands（VAC）。这是一个语音识别的系统，并且可以利用识别结果控制电脑，它允许用户自己定义语音命令，并且把语音命令转化为键盘命令。

VAC的官方网站为[http://www.dwvac.com/](http://www.dwvac.com/)，里面提供了软件的下载。我这里下载的是2.6.0的完整版。安装好后分为两个软件，一个VACBuilder负责制作和管理profile，另一个VACSystem是识别运行的软件。这软件还是基本可以无师自通的，有比较详细的help文档说明。

用VACBuilder产生一个命令主要需要3个步骤：




1.   An Action.就是对命令的一个描述


2.   A Phrase.是要说的单词（中文也可以么？你试试就知道了~~）


3.   A Key Command.对应的键盘命令

具体的乐趣还是大家自己发掘吧，这个软件真的做的还行，还可以在VACSystem里对声音进行训练，有意思

有时候有误识，总体来说识别率还是很可以的，不错不错



