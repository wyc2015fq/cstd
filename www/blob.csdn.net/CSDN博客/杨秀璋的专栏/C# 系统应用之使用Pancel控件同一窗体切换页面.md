
# C\# 系统应用之使用Pancel控件同一窗体切换页面 - 杨秀璋的专栏 - CSDN博客

2014年03月18日 15:57:49[Eastmount](https://me.csdn.net/Eastmount)阅读数：13099所属专栏：[C\# 系统应用知识](https://blog.csdn.net/column/details/eastmount-xtyy.html)



该文章是项目"个人电脑历史记录清除软件"的系列文章.主要讲述如何使用Pancel控件实现类似于360安全卫士的点击图标窗体不变,但页面内容变换的功能及遇到的重叠Panel不能设置Visible(可见)问题.
## 一.通过文档大纲查看层次关系
在使用panel控件时我遇到的一个问题是:当有多个panel(对控件集合分组)控件重叠显示时,在页面切换如下代码:
```python
private void pictureBox1_Click(object sender, EventArgs e)
{
    panelIE.Visible = true;
    panel2.Visible = false;
}
```
但运行的结果总是Panel1和Panel2同时隐藏.(感谢:bdmh)
解决方案:其中一个panel的parent可能被设置为另一个panel了,在拖动窗体控件时很容易就拖动至其他空间上.通过打开"视图->其他窗口->文档大纲"查看控件的层次图,就可以发现panelIE确实被自动设置为panel2的parent.
所以可以在"文档大纲"中手动拖动设置其panel关系,但好像只能拖动至顶层.(PS:没有找到好的方法,而且选中某个panel添加控件如button没有其视图可编辑)
![](https://img-blog.csdn.net/20140318152935546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 二.设置多个panel切换界面
最初的打算是使用6个panel控件实现,点击不同的图标设置其Visible(是否课件)实现同一个窗体切换界面.代码如下:
```python
/// <summary>
/// 点击标题菜单,对panel的显示
/// </summary>
/// <param name="p"></param>
private void PanelIsDisplay(int p)
{
    //设置panel显示界面 (IE\Word\回收站\U盘\电脑\文件粉碎)
    panelIE.Visible = false;
    panelWord.Visible = false;
    panelRecycle.Visible = false;
    panelU.Visible = false;
    panelRecent.Visible = false;
    panelFile.Visible = false;        
    switch (p)
    {
        case 1:  //显示"清除IE"
            {
                panelIE.Visible = true;
            }
            break;
        case 2:  //显示"清除Word"
            {
                panelWord.Visible = true;
            }
            break;
        case 3:  //显示"清空回收站"
            {
                panelRecycle.Visible = true;
                this.panelRecycle.BackColor = Color.Red;
            }
            break;
        case 4:  //显示"清除U盘"
            {
                panelU.Visible = true;
            }
            break;
        case 5:  //显示"电脑清理"
            {
                panelRecent.Visible = true;
            }
            break;
        case 6:  //显示"电脑清理"
            {
                panelFile.Visible = true;
            }
            break;
        default:
            {
            }
            break;
    }
}
//鼠标点击"清除IE"实现切换界面
private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
{
    //透明罩设置
    cmd.ShowOpaqueLayer(panel_mol3, 125, true);
    //自定义设置Panel切换函数
    PanelIsDisplay(1);
}
```
但是后来发现设置多个panel的层次关系和向某个panel中添加控件时非常繁琐,同时有的控件不能很好的设置.所以建议如果是2个panel的切换或者没有重叠的panel设置,可以通过该方法设置其Visible(是否可见)来隐藏与显示.如果重叠的可以参考下面的方法.
## 三.Panel控件加载Form实现页面切换
可以通过panel控件加载Form来实现该页面切换功能.向解决方案中添加不同页面的Form,设置窗体的控件是非常方便的,再加载到Panel2中显示即可.代码如下:
```python
/// <summary>
/// 向panel2中加载窗体Form,MouseEnter事件调用
/// </summary>
/// <param name="p"></param>
private void Control_Add(Form form)
{
    panel2.Controls.Clear();    //移除所有控件
    form.TopLevel = false;      //设置为非顶级窗体
    form.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None; //设置窗体为非边框样式
    form.Dock = System.Windows.Forms.DockStyle.Fill;                  //设置样式是否填充整个panel
    panel2.Controls.Add(form);        //添加窗体
    form.Show();                      //窗体运行
} 
//鼠标进入"清空回收站"
private void panel_mol3_MouseEnter(object sender, EventArgs e)
{
    //透明罩设置
    cmd3.ShowOpaqueLayer(panel_mol3, 125, true);
    if (isClick1 == false) cmd1.HideOpaqueLayer();
    if (isClick2 == false) cmd2.HideOpaqueLayer();
    if (isClick4 == false) cmd4.HideOpaqueLayer();
    if (isClick5 == false) cmd5.HideOpaqueLayer();
    if (isClick6 == false) cmd6.HideOpaqueLayer();
    //自定义函数加载窗体 CleanRecycle
    CleanRecycle recycle = new CleanRecycle();
    Control_Add(recycle);
}
```
运行结果如下图所示:
![](https://img-blog.csdn.net/20140318154544078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最后该文章主要是结合自己的实际东西讲解,有些浅显和简单,如果你刚好遇到类似的问题就可能对你使用Panel控件有所帮助.同时如果在文章中遇到错误或不足的地方,请海涵!我也在寻找更好的方法,如果你在同一个窗体下切换界面有什么更好的方法,希望告知.
(By:Eastmount 2014-3-18 下午4点 原创CSDNhttp://blog.csdn.net/eastmount/)


