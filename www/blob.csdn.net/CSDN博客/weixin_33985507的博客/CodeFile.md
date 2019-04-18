# CodeFile - weixin_33985507的博客 - CSDN博客
2006年01月14日 11:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
    在ASP.NET2.0里，Page指令的一个显著的变化就是从CodeBehind改变成了CodeFile，下面是一个较为典型的例子
<%@ Page Language="C#" CodeFile="MyPage.aspx.cs" 
               Inherits="PS.AspDotNet20.MyPage" %>
<html xmlns="[http://www.w3.org/1999/xhtml](http://www.w3.org/1999/xhtml)" >
  <body>
    <form id="form1" runat="server">
      Enter your name: <asp:TextBox ID="nameTextBox" runat="server" /><br />
      <asp:Button ID="enterButton" runat="server" 
                       Text="Enter" OnClick="enterButton_Click" />
      <br />
      <asp:Label ID="messageLabel" runat="server" />
    </form>
  </body>
</html>
我们很容易写出他的后台代码MyPage.aspx.cs：
using System;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
 namespace PS.AspDotNet20
{
  public partial class MyPage : System.Web.UI.Page
  {
    protected void Page_Load(object sender, EventArgs e)
    {    }
    protected void enterButton_Click(object sender, EventArgs e)
    {
      messageLabel.Text = "Hello " + nameTextBox.Text + "!";
    }
  }
}
    现在在ASP.NET2.0里，当用户请求MyPage.aspx时，它将生成两个类文件：
**一个是由后台文件MyPage.aspx.cs里定义的类生成**
**一个是由MyPage.aspx生成的类。**
这两个类文件都是不完全类型（partial），
毫无疑问，在ASP.NET框架处理页面前，最终必须建立一个完整的类，类似如下
namespace PS.AspDotNet20
{
  public partial class MyPage : IRequiresSessionState
  {
    protected global::System.Web.UI.WebControls.TextBox nameTextBox;
    protected global::System.Web.UI.WebControls.Button enterButton;
    protected global::System.Web.UI.WebControls.Label messageLabel;
    protected global::System.Web.UI.HtmlControls.HtmlForm form1;
    protected System.Web.Profile.DefaultProfile Profile
    {
      get
      {
        return ((System.Web.Profile.DefaultProfile)(this.Context.Profile));
      }
    }
    protected System.Web.HttpApplication ApplicationInstance
    {
      get
      {
        return ((System.Web.HttpApplication)(this.Context.ApplicationInstance));
      }
    }
  }
}
    这样您就可以看到，这里的原理和ASP.NET1.X类似了，所有的控件都以类成员的省份显示。当然上面这种“整合”是系统自动运行的，所以从本质上看，ASP.NET2.0解析*.aspx里面的控件并生成类成员和ASP.NET1.X类似的。
    另外，这种类的放置位置和ASP.NET1.X非常类型，默认放置在临时文件，当然如果您不知道临时文件夹
的具体内容，可以参考如下一篇文章。
[http://www.microsoft.com/china/MSDN/library/archives/library/DNAspp/html/dngrftheaspnethttpruntime.asp](http://www.microsoft.com/china/MSDN/library/archives/library/DNAspp/html/dngrftheaspnethttpruntime.asp)
 这样.ASPX生成的编译代码类似如下：
namespace ASP
{
  public class MyPage_aspx : global::PS.AspDotNet20.MyPage
  {
    private static bool @__initialized = false;
    private static object @__fileDependencies;
    public MyPage_aspx()
    {
      string[] dependencies;
      AppRelativeVirtualPath = "~/MyPage.aspx";
      if ((global::ASP.MyPage_aspx.@__initialized == false))
      {
        dependencies = new string[2];
        dependencies[0] = "~/MyPage.aspx";
        dependencies[1] = "~/MyPage.aspx.cs";
        global::ASP.MyPage_aspx.@__fileDependencies = this.GetWrappedFileDependencies(dependencies);
        global::ASP.MyPage_aspx.@__initialized = true;
      }
      this.Server.ScriptTimeout = 30000000;
    }
    private global::System.Web.UI.WebControls.TextBox @__BuildControlnameTextBox()
    {
      global::System.Web.UI.WebControls.TextBox @__ctrl;
      @__ctrl = new global::System.Web.UI.WebControls.TextBox();
      this.nameTextBox = @__ctrl;
      @__ctrl.ApplyStyleSheetSkin(this);
      @__ctrl.ID = "nameTextBox";
      return @__ctrl;
    }
    private global::System.Web.UI.WebControls.Button @__BuildControlenterButton()
    {
      global::System.Web.UI.WebControls.Button @__ctrl;
      @__ctrl = new global::System.Web.UI.WebControls.Button();
      this.enterButton = @__ctrl;
      @__ctrl.ApplyStyleSheetSkin(this);
      @__ctrl.ID = "enterButton";
      @__ctrl.Text = "Enter";
      @__ctrl.Click += new System.EventHandler(this.enterButton_Click);
      return @__ctrl;
    }
    private global::System.Web.UI.WebControls.Label @__BuildControlmessageLabel()
    {
      global::System.Web.UI.WebControls.Label @__ctrl;
      @__ctrl = new global::System.Web.UI.WebControls.Label();
      this.messageLabel = @__ctrl;
      @__ctrl.ApplyStyleSheetSkin(this);
      @__ctrl.ID = "messageLabel";
      return @__ctrl;
    }
    private global::System.Web.UI.HtmlControls.HtmlForm @__BuildControlform1()
    {
      global::System.Web.UI.HtmlControls.HtmlForm @__ctrl;
      @__ctrl = new global::System.Web.UI.HtmlControls.HtmlForm();
      this.form1 = @__ctrl;
      @__ctrl.ID = "form1";
      System.Web.UI.IParserAccessor @__parser = ((System.Web.UI.IParserAccessor)(@__ctrl));
      @__parser.AddParsedSubObject(new System.Web.UI.LiteralControl("\r\n Enter your name: "));
      global::System.Web.UI.WebControls.TextBox @__ctrl1;
      @__ctrl1 = [this.@__BuildControlnameTextBox](mailto:this.@__BuildControlnameTextBox)();
      @__parser.AddParsedSubObject(@__ctrl1);
      @__parser.AddParsedSubObject(new System.Web.UI.LiteralControl("<br />\r\n "));
      global::System.Web.UI.WebControls.Button @__ctrl2;
      @__ctrl2 = [this.@__BuildControlenterButton](mailto:this.@__BuildControlenterButton)();
      @__parser.AddParsedSubObject(@__ctrl2);
      @__parser.AddParsedSubObject(new System.Web.UI.LiteralControl("\r\n <br />\r\n "));
      global::System.Web.UI.WebControls.Label @__ctrl3;
      @__ctrl3 = [this.@__BuildControlmessageLabel](mailto:this.@__BuildControlmessageLabel)();
      @__parser.AddParsedSubObject(@__ctrl3);
      @__parser.AddParsedSubObject(new System.Web.UI.LiteralControl("\r\n "));
      return @__ctrl;
    }
    private void @__BuildControlTree(MyPage_aspx @__ctrl)
    {
      System.Web.UI.IParserAccessor @__parser = ((System.Web.UI.IParserAccessor)(@__ctrl));
      @__parser.AddParsedSubObject(new System.Web.UI.LiteralControl(
                    "\r\n\r\n<html xmlns=\"[http://www.w3.org/1999/xhtml\](http://www.w3.org/1999/xhtml/)" >\r\n<body>\r\n "));
      global::System.Web.UI.HtmlControls.HtmlForm @__ctrl1;
      @__ctrl1 = [this.@__BuildControlform1](mailto:this.@__BuildControlform1)();
      @__parser.AddParsedSubObject(@__ctrl1);
      @__parser.AddParsedSubObject(new System.Web.UI.LiteralControl("\r\n</body>\r\n</html>\r\n"));
    }
    protected override void FrameworkInitialize()
    {
      base.FrameworkInitialize();
[this.@__BuildControlTree(this](mailto:this.@__BuildControlTree(this));
      this.AddWrappedFileDependencies(global::ASP.MyPage_aspx.@__fileDependencies);
      this.Request.ValidateInput();
    }
    public override int GetTypeHashCode()
    {
      return -731145279;
    }
  }
}
     上面说过，在ASP.NET框架处理页面前，最终必须建立一个完整的类，这种后果是页面在运行时编译，结果影响了性能，可以使用aspnet_compiler.exe 编译，（aspnet_compiler.exe 是.NET2.0新增的），以后回介绍这个命令。
