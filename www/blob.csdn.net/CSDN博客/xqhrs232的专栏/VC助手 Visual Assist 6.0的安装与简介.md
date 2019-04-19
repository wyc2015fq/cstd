# VC助手 Visual Assist 6.0的安装与简介 - xqhrs232的专栏 - CSDN博客
2010年05月12日 12:04:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2257
原文地址::
[http://blog.pconline.com.cn/article/240357.html](http://blog.pconline.com.cn/article/240357.html)
Visual Assist X是开发环境的辅助工具，使用该工具可以让用户更加轻松地编写代码。下面以Visual Assist 6.0为例，详细介绍它的安装与使用。
　　1.功能
　　Visual Assist X主要有3个功能，具体如下。
　　（1）成员列表框的出现更加频繁、迅速，并且结果更加准确。参数信息更加完善，并带有注释。含有所有符号的停驻工具提示。使用Visual Assist X前的提示如图1-23所示，使用Visual Assist X后的提示如图1-24所示。
![](http://img1.pconline.com.cn/pconline/0903/26/1602032_image024.jpg)
图1-23 使用Visual Assist X前
![](http://img1.pconline.com.cn/pconline/0903/26/1602032_image025.jpg)
图1-24 使用Visual Assist X后
　　（2）智能提示。输入“da”，Visual Assist X会自动给出一个提示“data”（如图1-25所示），如果提示正确，可直接按回车键使用提示的内容。
![](http://img.pconline.com.cn/images/itblog/7502263/20093/26/1238031937656.jpg)
图1-25 智能提示
　　（3）错误自动校正：监控您的IDE，对那些简单但耗时的错误进行即时校正。在以下代码中，输入“Cdata”，再输入空格，“Cdata”会自动变成“CData”。
　　void CEx010106Dlg::OnButton1（）
　　{
　　Cdata
　　}
　　2.安装
　　安装Visual Assist X的具体步骤如下。
　　（1）运行安装文件（如VA6Setup1079）。在运行安装文件之前，必须关闭Visual C++及其同系列产品，否则会出现如图1-26所示的提示。
![](http://img.pconline.com.cn/images/itblog/7502263/20093/26/1238031982434.jpg)
图1-26 错误信息
　　（2）选中“I agree with the above terms and conditions”复选框，如图1-27所示，单击“Next”按钮。
![](http://img1.pconline.com.cn/pconline/0903/26/1602032_image028.jpg)
图1-27 同意协议
　　（3）选择安装路径，不必和Visual C++安装在一起，如图1-28所示。
![](http://img1.pconline.com.cn/pconline/0903/26/1602032_image029.jpg)
图1-28 选择安装路径
　　（4）单击“OK”按钮完成Visual Assist 6.0的安装，如图1-29所示。
![](http://img.pconline.com.cn/images/itblog/7502263/20093/26/1238032100933.jpg)
图1-29 安装完成
　　（5）打开Visual C++，由于Visual Assist 6.0还是试用版，所以启动时都会弹出如图1-30所示的对话框，单击“Quit”按钮关闭此对话框，Visual C++就自动具有Visual Assist 6.0的相关功能了。
![](http://img.pconline.com.cn/images/itblog/7502263/20093/26/1238032143923.jpg)
图1-30 Visual Assist 6.0 Trial
　　注意：试用版某些功能（如工具栏）无法使用，最好购买正式版。
　　3.卸载
　　如果不再使用Visual Assist 6.0，可以在控制面板将其删除。
　　4.禁用、启用
　　如果只是暂时不想使用此工具栏，可以将其禁用。选择菜单“Tools→Customize”命令，在弹出的“Customize”对话框中，取消“Visual Assist 6.0 Add-in”复选框就可以禁用Visual Assist，如图1-31所示。选中“Visual Assist 6.0 Add-in”复选框就可以重新启用Visual Assist。
![](http://img1.pconline.com.cn/pconline/0903/26/1602032_image032.jpg)
图1-31 禁用辅助工具
　　5.工具栏简介
　　Visual Assist 6.0的工具栏有如图1-32所示的16个按钮，下面依次介绍。
![](http://img.pconline.com.cn/images/itblog/7502263/20093/26/1238032208613.jpg)
图1-32 Visual Assist 6.0工具栏
　　n Visual Assist Options：单击该按钮，可以设置Visual Assist。
　　n Goto Method in Current File：单击该按钮，会弹出如图1-33所示的下拉列表，在此可以查看当前文件中的方法。
　　n Back和Forward：该两个按钮用来控制光标的位置。例如，当光标在第1行时，在第3行单击鼠标，光标就移到第3行，再在第10行单击鼠标，光标就位于第10行了，这时单击“后退”按钮，光标退到第3行，再单击“后退”按钮，光标就回到第1行，如果此时单击“前进”按钮，光标将返回第3行，再次单击“前进”按钮，光标就出现在第10行了。
　　n Open Project File：单击该按钮，打开如图1-34所示的“Open Project File”对话框，列出本工程的部分文件，选择一个文件，直接单击“OK”按钮，即可打开此文件。
![](http://img1.pconline.com.cn/pconline/0903/26/1602032_image039.jpg)
图1-33 单击“Goto Method in Current File”按钮弹出下拉列表
![](http://img1.pconline.com.cn/pconline/0903/26/1602032_image040.jpg)
图1-34 Open Project File
　　n Open Corresponding.h or .cpp：打开对应的头文件或源文件。
　　n Paste from Multiple Buffers：单击该按钮，从多缓冲区中进行粘贴操作。可以复制或剪切多次，然后从中选择要粘贴的内容，效果如图1-35所示。
![](http://img1.pconline.com.cn/pconline/0903/26/1602032_image043.jpg)
图1-35 Paste from Multiple Buffer
　　n Insert Code Template：插入代码模板。单击此按钮，在弹出的菜单中选择“Class definition using selection”，如图1-36所示。
![](http://img1.pconline.com.cn/pconline/0903/26/1602032_image045.jpg)
图1-36 Insert Code Template
　　辅助工具会自动添加如下代码：
　　class
　　{
　　public:
　　（）;
　　~（）;
　　protected:
　　private:
　　};
　　n Display Context Menu：显示上下文菜单，效果如图1-37所示。
![](http://img.pconline.com.cn/images/itblog/7502263/20093/26/1238032507772.jpg)
图1-37 上下文菜单
　　n Find Provious by Context：联系上下文查找上一个。
　　n Find Next by Context：联系上下文查找下一个。
　　n Comment Selection：用/*和*/注释选择的内容。
　　n Comment Selection：用//注释选择的内容。
　　n Spell Check Selection：对选中内容进行拼写检查。
　　n Reparse Current File：重新分析当前文件。
　　n Enable/Disable Visual Assist：启用、禁用Visual Assist。
