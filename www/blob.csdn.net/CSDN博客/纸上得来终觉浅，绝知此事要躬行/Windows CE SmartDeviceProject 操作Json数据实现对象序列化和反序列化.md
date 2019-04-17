# Windows CE /SmartDeviceProject 操作Json数据实现对象序列化和反序列化 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年01月21日 14:15:31[boonya](https://me.csdn.net/boonya)阅读数：3026








      使用场景：实现Java对象的Json数据在C#中读取，重新封装为C#的对象，数据传递采用webservice方式，所有返回类型为int 、boolean 、String、long 等基础数据类型，避免传输复杂的java List和自定义对象；而String的格式就是熟悉的Json。 访问资源下载路径：[http://jsoncf.codeplex.com/](http://jsoncf.codeplex.com/)

源代码和编译后文件下载地址：[http://jsoncf.codeplex.com/releases/view/18199?RateReview=true](http://jsoncf.codeplex.com/releases/view/18199?RateReview=true)

在线代码阅读地址：[http://jsoncf.codeplex.com/SourceControl/latest#ReadMe.txt](http://jsoncf.codeplex.com/SourceControl/latest#ReadMe.txt)

### 1、引用程序包

![](https://img-blog.csdn.net/20140121140311296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 2、编写实体对象



```
using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using CodeBetter.Json;

namespace SmartDeviceProjectWtms.Domain
{
    [SerializeIncludingBase]
    class Demo
    {
        private int _id;

        private String _name;

        public int id {
            get {  return _id; }
            set {  _id = value;}
        }

        public String name {
            get {  return _name;}
            set { _name = value;}
        }

        public Demo() { 
        
        }

        public Demo(int _id,String _name)
        {
            id = _id;
            name = _name;
        }
    }
}
```
注：必须引用可序列化注解


### 3、反序列化封装工具



```
using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using SmartDeviceProjectWtms.Domain;
using CodeBetter.Json;

namespace SmartDeviceProjectWtms
{
    class JsonUtil
    {
        public static string ReplaceSpecialChar(String jsonStr) 
        {
            jsonStr = jsonStr.Replace("\\t", " ");
            jsonStr = jsonStr.Replace("null", "\"" + "null" + "\"");
            return jsonStr;
        }

        /************************************************测试例子**********************************/ 
          
        /// <summary>
        /// 获取Demo属性的封装对象【例子】
        /// </summary>
        /// <param name="jsonStr">Json对象字符串集合</param>
        /// <returns></returns>
        public static Demo GetDemoByJsonStr(String jsonStr)
        {
            Demo demo = Converter.Deserialize<Demo>(jsonStr,"_");
            return demo;
        }

        /// <summary>
        /// 获取Demo属性的封装对象【例子】
        /// </summary>
        /// <param name="jsonStr">Json对象字符串集合</param>
        /// <returns></returns>
        public static List<Demo> GetDemoListByJsonStr(String jsonStr)
        {
            List<Demo> list = Converter.Deserialize<List<Demo>>(jsonStr, "_");
            return list;
        }

    }
}
```
注：ReplaceSpecialChar方法是处理Java　javax.util.Date对象和值为空null的Json字符问题。


### 4、编写反序列测试类


```
using System;
using System.Linq;
using System.Collections.Generic;
using System.Windows.Forms;
using SmartDeviceProjectWtms.Domain;
using CodeBetter.Json;

namespace SmartDeviceProjectWtms
{
    class JsonTest
    {
        /// <summary>
        /// 测试【JSON单个对象】反序列化
        /// </summary>
        public static void TestDemo2()
        {
            string jsonStr =@"{""id"":1,""name"":""boonya""}";

            MessageBox.Show("{}JSon:" + jsonStr);
            Demo demo = JsonUtil.GetDemoByJsonStr(jsonStr);
            if (demo != null)
            {
                MessageBox.Show("{}获取Object：" + demo.id + " " + demo.name);
            }
        }

        /// <summary>
        /// 测试【JSON对象集合】反序列化
        /// </summary>
        public static String TestDemo3() {
            string jsonStr = @"[{""id"":1,""name"":""boonya""}]";
            List<Demo> dList = JsonUtil.GetDemoListByJsonStr(jsonStr);
            if (dList != null) {
                MessageBox.Show("[]JSon:" + jsonStr);
                foreach (Demo demo in dList)
                {
                    MessageBox.Show("[]获取Object：" + demo.id + " " + demo.name);
                }
            }
            return jsonStr;
        }
    }
}
```
注：本例只是一个简单的例子，具体Java对象转过来的Json处理可能更为复杂，但处理基本一样。

### 5、Json包程序序列化和反序列化操作例子


```
namespace CodeBetter.Json.Test.Console
{
    using System;

    internal class Program
    {
        private static void Main(string[] args)
        {            
            string json = Converter.Serialize(new User("name", "password", AccountStatus.Enabled));
            Converter.Serialize("out.txt", new int[] { 1, 2, 3, -4 }, "_");
            Console.WriteLine(json);


            User user = Converter.Deserialize<User>(json, "_");
            int[] values = Converter.DeserializeFromFile<int[]>("out.txt", "_");
            Console.WriteLine(user.UserName);
            
            Console.WriteLine("Done. Press enter to exit");
            Console.ReadLine();
        }
    }

    public class BaseUser
    {
        private int _id = 1;
    }

    [SerializeIncludingBase]
    public class User : BaseUser
    {
        private string _userName;
        private string _password;
        [NonSerialized]
        private readonly Role _role;
        private AccountStatus _status;    
        private Thing _think = new Thing();

        public string UserName
        {
            get { return _userName; }
            set { _userName = value; }
        }
        public string Password
        {
            get { return _password; }
            set { _password = value; }
        }
        public AccountStatus Status
        {
            get { return _status; }
            set { _status = value; }
        }
        public Role Role
        {
            get { return _role; }
        }
        public Thing Thing
        {
            get { return new Thing(); }
        }

        public User(string userName, string password, AccountStatus status)
        {
            UserName = userName;
            Password = password;
            Status = status;
            _role = new Role(DateTime.Now, "Admin", this);
        }

        private User()
        {
        }
    }

    public class Role
    {
        public Role(DateTime expires, string name, User user)
        {
            Expires = expires;
            Name = name;
            User = user;
        }

        public DateTime Expires { get; set; }

        public string Name { get; set; }

        public User User { get; set; }

        public Thing Thing
        {
            get { return new Thing(); }
        }
    }

    public class Thing
    {
        private string _name = "ABC";

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }
    }

    public enum AccountStatus
    {
        Enabled = 1,
        Disabled = 2,
    }
}
```










