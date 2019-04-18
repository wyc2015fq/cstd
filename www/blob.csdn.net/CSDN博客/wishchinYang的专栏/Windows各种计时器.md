# Windows各种计时器 - wishchinYang的专栏 - CSDN博客
2013年10月24日 00:48:41[wishchin](https://me.csdn.net/wishchin)阅读数：1193
（一）：OnTimer类
1.打开对应对话框的类向导ClassWizard。 
2.在消息映射MessageMaps中添加消息Message：WM_TIMER。
3.程序代码中将自动添加函数OnTimer(UINT nIDEvent)，在该函数内编写代码实现计时器功能。
函数代码一般写在switch...case中，nIDEvent表示计时器ID号，从而区分不同计时器的实现内容。
若不使用switch...case结构，则表示实现ID为1的计时器功能。
整个函数代码如下：
```cpp
void CCountDown::OnTimer(UINT nIDEvent)
{
 switch (nIDEvent)
 {
 case 1:
  { 
   //代码实现
   break;
  }
 default:
  break;
 }
 CDialog::OnTimer(nIDEvent);
}
```
4.在要开始计时器的地方调用：
SetTimer(1,1000,NULL);//第一个参数表示计时器的ID；
                        第二个参数表示间隔时间（ms）；
                        第三个表示调用的函数，NULL表示默认调用OnTimer函数。
5.在要结束计时器的地方调用：
KillTimer(1);)        //参数表示计时器的ID
（2）：ontimer实例： 
(1)   创建一个基于对话框的应用程序。
(2)   在对话框界面添加一个文本框和两个按钮。
(3)   文本框添加一个变量m_nTime。
(4)   双击“设置定时器”按钮，添加如下代码：
```cpp
void CTimeDlg::OnButton1()
{
       // TODO: Add your control notification handler code here
       UpdateData(false);
       //取消定时器
       KillTimer(1);
       //设置定时器
       SetTimer(1,1000,NULL);
}
(5)   双击“取消定时器”按钮，添加如下代码：
void CTimeDlg::OnButton1()
{
       // TODO: Add your control notification handler code here
       UpdateData(false);
       //取消定时器
       KillTimer(1);
}
(6)   添加一个静态变量n，申明如下：
static n=0;
(7)   添加WM_TIMER事件，方法为：右键单击对话框空白部分—>Event，在弹出的对话框左边选择WM_TIMER双击，右边就出现WM_TIMER，再双击添加如下代码：
void CTimeDlg::OnTimer(UINT nIDEvent)
{
       // TODO: Add your message handler code here and/or call default
       if(nIDEvent==1){
              n++;
              m_nTime=n;
              UpdateData(false);
       }
       CDialog::OnTimer(nIDEvent);
}
```
运行结果:
点击“设置定时器”文本框数字每隔1秒自动增1，点击“取消定时器”，数字停止增加，再点击“设置定时器”，数字接着继续增加。如果每次设置都从0重新开始，只要在“设置定时器”按钮把n值重新赋值0即可。
