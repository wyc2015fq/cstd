# WinForm窗体实现托盘 - Jun5203 - CSDN博客
2019年04月07日 16:58:18[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：22
个人分类：[【机房收费系统】](https://blog.csdn.net/Ellen5203/article/category/7907465)
所属专栏：[C#](https://blog.csdn.net/column/details/29824.html)
为了让自己设计的软件更高逼格一点，那就加入托盘吧！
详细步骤请看下文~
**步骤一：从工具箱中添加控件——NotifyIcon，到窗体上；**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190407162510744.png)
**步骤二：设置NotifyIcon属性**
（1）Visible设置成false
（2）Icon添加托盘图像
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190407162713462.png)
**步骤三：代码**
```
//窗体最小化时托盘出现；窗体其它状态时，托盘不出现
     private void FormModifyPwd_Resize(object sender, EventArgs e)
     {
         if (this.WindowState == FormWindowState.Minimized)
         {
             notifyIcon1.Visible = true;
         }
         else
         {
             notifyIcon1.Visible = false;
         }
     }
```
```
// 双击托盘，窗体还原
    private void notifyIcon1_MouseDoubleClick(object sender, MouseEventArgs e)
     {
         this.Visible = true;
         this.WindowState = FormWindowState.Normal;
     }
```
自窗体的设置完成之后不要忘记去调用它哦！
```
FormModifyPwd formModifyPwd = new FormModifyPwd();
    formModifyPwd.Show();//修改密码窗体显示
    formModifyPwd.WindowState = FormWindowState.Minimized; //最小化
```
