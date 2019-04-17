# MFC如何在有界面的应用程序中开启控制台窗口 - DoubleLi - 博客园






在有界面的应用程序中开启控制台窗口有时候非常有用,尤其是在调试多线程应用程序中,
由于通过断点的方式调试程序时会导致线程挂起从而导致各种难于预料的结果。这时候就可
以通过开启控制台窗口往窗口输出信息来查看线程的运行情况,这比通过日志文件输出的方式
更方便和实时。下面介绍两种开启控制台窗口的方法:
1.通过AllocConsole()函数打开控制台窗口
  AllocConsole();//打开控制台窗口
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//获取标准输出句柄
  char msg[] = "点击了确定按钮!\n";
  WriteConsole(hOutput,msg,strlen(msg),NULL,NULL);//向控制台窗口写入信息
以下是示例程序运行的的效果图:

每当点击按钮时就会在控制台输出"点击了确定按钮!\n"![](https://img-blog.csdn.net/20170719105401963?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmNfbmV3ZXJfYW5kX25ld2Vy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2.通过配置相关属性
  通过在 属性->配置属性->生成事件->后期生成事件->命令行 中添加 
  editbin /subsystem:console $(OutDir)\$(TargetName).exe
  使用cout或者printf向控制台输出 也可以通过获取标准输出句柄然后调用WriteConsole
  方式输出
本程序采用cout打印"Hello World"字符串

![](https://img-blog.csdn.net/20170719105455542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmNfbmV3ZXJfYW5kX25ld2Vy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)









