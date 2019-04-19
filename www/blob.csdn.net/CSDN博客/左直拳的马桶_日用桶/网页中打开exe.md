# 网页中打开exe - 左直拳的马桶_日用桶 - CSDN博客
2016年06月29日 20:03:24[左直拳](https://me.csdn.net/leftfist)阅读数：9226
网页上打开本地的exe文件，可以吗？
西蒙说：可以的。
方法如下：
> 
1、定义一个私有协议，指向本地的那个exe  
  2、在网页上将此私有协议作为URL，点击之即可打开那个exe 
  3、URL中还可以包含参数，传递给那个exe
**1、定义一个私有协议**
协议，http就是一个众所周知的例子。在浏览器中输入这个东东，浏览器就能明白是打开一个网页。那么，这个私有协议，就是用来打开我们那个该死的exe的。
私有协议怎么定义？很简单，修改注册表。写一段脚本，正确命名文件后缀，双击，windows能识别并运行，修改注册表。
示例，记事本打开，输入脚本如下：
```
Windows Registry Editor Version 5.00
[HKEY_CLASSES_ROOT\HYZT]
@="URL:HYZT Protocol Handler"
"URL Protocol"=""
[HKEY_CLASSES_ROOT\HYZT\shell]
[HKEY_CLASSES_ROOT\HYZT\shell\open]
[HKEY_CLASSES_ROOT\HYZT\shell\open\command]
@="E:\\hyzt\\bin\\Debug\\hyzt.exe %1"
```
文件另存为：hyzt.reg。然后双击此文件，注册表即修改矣。文件可以多次修改，多次运行。新修改覆盖原来。
如此，我们就定义了一个私有协议：hyzt。
**2、URL中包含此协议**
现在，在本机浏览器地址栏中输入`hyzt://`，回车，即可打开那个该死的exe了。IE是可以的，chrome的话，浏览器中直接输入有时不行，可以定义一个书签（类似IE收藏夹中的收藏），点击就可以了。
学习http协议的时候，我们会接触两个名词：URI 和 URL。二者有啥区别？
> 
URI，统一资源定位符；URL，统一资源位置符。简单来讲, URL = 协议 + URI。
比方说，www.baidu.com是一个URI，而[http://www.baidu.com](http://www.baidu.com)是一个URL。
也就是说，URL不仅仅标明资源的位置，而且标明访问资源的方法（协议）。http是一种协议，我们那个hyzt，也是一种协议。
**3、URL中传递参数给exe**
我们用 `hyzt://` 就可以打开exe。如果想传参数该咋办？可以类似酱紫：
`hyzt://1000,chenqu`
注意，exe接收到的参数，并不是 `1000,chenqu`，而是完整的 `hyzt://1000,chenqu` ！
附exe接收参数并处理：
```
namespace TackColor
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main(string[] paras)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1(paras));
        }
    }
}
```
```
namespace TackColor
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        string _UserId = "0";
        string _UserName = "";
        public Form1(string[] paras) : this()
        {
            pickupParam(paras);
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            MessageBox.Show(String.Format("用户ID：{0},用户名：{1}",this._UserId,this._UserName));
        }
        void pickupParam(string[] paras)
        {
            if(paras == null && paras.Length == 0)
            {
                return;
            }
            string p = paras[0];
            Regex rx = new Regex(@"hyzt\://(?<p>[^/]+)/?", RegexOptions.Compiled | RegexOptions.IgnoreCase);
            Match m = rx.Match(p);
            if(m.Success)
            {
                string[] ps = m.Result("${p}").Split(',');
                if(ps.Length > 0)
                {
                    this._UserId = ps[0];
                }
                if (ps.Length > 1)
                {
                    this._UserName = ps[1];
                }
            }
        }
    }
}
```
![这里写图片描述](https://img-blog.csdn.net/20160629200203876)
**4、私有协议与程序安装包结合**
私有协议里面，明确指明了相应的可执行文件的路径：
```
[HKEY_CLASSES_ROOT\HYZT\shell\open\command]
@="C:\\hyzt\\bin\\Debug\\hyzt.exe %1"
```
问题是，每台客户机实际情况千差万别，比如本例子中，有些机器根本没有C盘，可执行文件可能装在D盘。那么这个注册表文件在运行前，都要手动改一下吗？ 
一个思路是，这个注册表修改工作放在程序的安装包里。用户安装的时候，就将私有协议写进去。
