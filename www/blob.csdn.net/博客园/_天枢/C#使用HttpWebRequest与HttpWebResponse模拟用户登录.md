# C#使用HttpWebRequest与HttpWebResponse模拟用户登录 - _天枢 - 博客园
## [C#使用HttpWebRequest与HttpWebResponse模拟用户登录](https://www.cnblogs.com/yhleng/p/6728864.html)
2017-04-18 17:19 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=6728864)
模拟艺龙旅游网登录
想模拟登录，首先整理一下流程
1.通过360浏览器(IE,火狐等等)F12开发人员工具抓到相关数据
2.获取验证码（拿到cookie），登录时也需要使用
3.登录
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
F12调出开发人员工具，输入用户名，密码登录，看我们抓到了什么信息。
![](https://images2015.cnblogs.com/blog/1149221/201704/1149221-20170418165306899-767328250.png)
Request URL:这个就是登录请求的url  
https://secure.elong.com/passport/ajax/elongLogin
方式POST
Form Data:这个是我们要POST传输的数据：
userName=xzdylyh&passwd=12313&validateCode=验证码&rememberMe=false
其它一些重要信息在Request Headers中
*****************************************************************
我使用C# 设计的winform界面
![](https://images2015.cnblogs.com/blog/1149221/201704/1149221-20170418170109243-1029804979.png)
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web;
using System.Net;
using System.IO;
using System.Data;
namespace HTTPHELPER
{
    public  class ELOGN_LOGIN
    {
       public static CookieContainer container = null; //存储验证码cookie
        #region 登录
        public string  requestM(string uName,string passwd,string vaildate)
        {
            HttpWebRequest request = null;
            HttpWebResponse response = null;
            try
            {
                request = (HttpWebRequest)HttpWebRequest.Create("https://secure.elong.com/passport/ajax/elongLogin");
                request.Method = "Post";
                request.ContentType = "application/x-www-form-urlencoded; charset=UTF-8";
                request.UserAgent = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36";
                request.AllowAutoRedirect = true;
                request.CookieContainer = container;//获取验证码时候获取到的cookie会附加在这个容器里面
                request.KeepAlive = true;//建立持久性连接
                //整数据
                string postData = string.Format("userName={0}&passwd={1}&validateCode={2}&rememberMe=true", uName, passwd, vaildate);
                ASCIIEncoding encoding = new ASCIIEncoding();
                byte[] bytepostData = encoding.GetBytes(postData);
                request.ContentLength = bytepostData.Length;
                //发送数据  using结束代码段释放
                using (Stream requestStm = request.GetRequestStream())
                {
                    requestStm.Write(bytepostData, 0, bytepostData.Length);
                }
                //响应
                response = (HttpWebResponse)request.GetResponse();
                string text = string.Empty;
                using (Stream responseStm = response.GetResponseStream())
                {
                    StreamReader redStm = new StreamReader(responseStm, Encoding.UTF8);
                    text = redStm.ReadToEnd();
                }
               return text;
            }
            catch (Exception ex)
            {
                var msg = ex.Message;
                return msg;
            }
        }
        #endregion 
        #region 获取验证码
        public Stream getCodeStream(string codeUrl)
        {
            //验证码请求
           HttpWebRequest request = (HttpWebRequest)WebRequest.Create(codeUrl);
            request.Method = "GET";
            request.ContentType = "application/x-www-form-urlencoded";
            request.UserAgent = "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:5.0.1) Gecko/20100101 Firefox/5.0.1";
            request.Accept = "image/webp,*/*;q=0.8";
            request.CookieContainer = new CookieContainer();//!Very Important.!!!
            container = request.CookieContainer;
            var c = request.CookieContainer.GetCookies(request.RequestUri);
            HttpWebResponse  response = (HttpWebResponse)request.GetResponse();
            response.Cookies = container.GetCookies(request.RequestUri);
          
          Stream stream = response.GetResponseStream();
          return stream;
        }
    }
        #endregion
}
```
```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using HTTPHELPER;
namespace WindowsFormsApplication8
{
    public partial class ELONG_LOGIN_FORM : Form
    {
        public ELONG_LOGIN_FORM()
        {
            InitializeComponent();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            
            ELOGN_LOGIN elongLogin = new ELOGN_LOGIN();
            
            var rmsg = elongLogin.requestM(txtuserName.Text,txtPassword.Text,txtVaildata.Text);
            MessageBox.Show(rmsg);
        }
        private void ELONG_LOGIN_FORM_Load(object sender, EventArgs e)
        {
            ReflshPicImage();//更新验证码
        }
        //更新验证码
        public void ReflshPicImage()
        {
            string codeUrl = "https://secure.elong.com/passport/getValidateCode";
            ELOGN_LOGIN agent = new ELOGN_LOGIN();
            Stream stmImage = agent.getCodeStream(codeUrl);
            picValidate.Image = Image.FromStream(stmImage);
        }
        private void btnReValidate_Click(object sender, EventArgs e)
        {
            ReflshPicImage();//更新验证码
        }
        private void picValidate_Click(object sender, EventArgs e)
        {
            ReflshPicImage();//更新验证码
        }
    }
}
```
最后执行效果，登录的session已经成功返回。
![](https://images2015.cnblogs.com/blog/1149221/201704/1149221-20170418171553602-400149654.png)
