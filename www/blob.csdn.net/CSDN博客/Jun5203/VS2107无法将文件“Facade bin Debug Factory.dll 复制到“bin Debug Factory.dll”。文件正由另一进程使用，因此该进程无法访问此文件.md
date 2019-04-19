# VS2107无法将文件“Facade\bin\Debug\Factory.dll"复制到“bin\Debug\Factory.dll”。文件正由另一进程使用，因此该进程无法访问此文件 - Jun5203 - CSDN博客
2019年02月18日 16:01:55[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：153
所属专栏：[机房收费系统](https://blog.csdn.net/column/details/26589.html)
在使用VS2017的过程中，经常会遇到无法【将文件“Facade\bin\Debug\Factory.dll"复制到“bin\Debug\Factory.dll”。文件“bin\Debug\Factory.dll”正由另一进程使用，因此该进程无法访问此文件】类似这种问题，本想不了了之，可谁想越是放纵它，它越得寸进尺，时不时的冒出来给我一个“惊喜”。作为一个优秀的“程序猿”当然不能被它吓到，还得想办法治治它，说干就干。
**<错误显示>**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190218144723137.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
**<原因>**
1.使用this.Hide();隐藏窗体，而不是真正关闭窗体。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190218152212810.png)
2.有正在运行的程序，也就是说没有关闭窗体。那么是怎么造成这种情况的呢？请听我细细道来~
我是因为在设计登陆的时候遇到的这个问题，所以我就拿登录来举例：
（1）登录窗体在运行，没有关闭又将其重新启动了一遍
（2）登录成功后进入主窗体，但是主窗体中没有具体的代码，也就是说没有设计[当点击“X”的时候完全退出该程序]，导致只是关闭了当前窗体，而登录窗体还处于隐藏正在运行状态。也就是说关闭VS后UI.exe还处于运行状态，且你看不到它还在运行。那么怎么来解决呢？这时候任务管理器就起了很大的作用哦！
**<解决方法>**
- 如果你是在做登录的时候出现了这个问题，不要着急也不要慌，比较淡定的就把它放过去就可以了，实在忍受不了的就提前到主窗体的事件中设置一下
```
private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            Application.Exit();
        }
```
方法：主窗体-属性-事件-双击Formclosing-在Formclosing的方法中填上“Application.Exit();”
- 如果不想提前进入到下一个阶段-主窗体，那么就去任务管理器中看一下，将其结束任务即可。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019021815544457.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 如果你是在做其他的功能出现了这个问题，那么就关闭VS，再重新打开，最最最难受的方法就是重启电脑，当然这也是耗费不少时间的，如果你的电脑较慢的话。
**小编有话说**
决定工作效率和工作质量的，首先是工作态度，其次才是能力。你用了多少心，你的成就就会有多少。凡事全力以赴，尽心尽力，才能有所建树。即使效率不高，也一定要把态度端正，世界总会向有目标和远见的人让路。
