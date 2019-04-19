# 浅谈MVC三层架构 - fanyun的博客 - CSDN博客
2017年07月30日 21:37:20[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：413
三层架构并不是MVC，MVC是一个很早就有的经典的程序[设计](http://www.warting.com/)模式，M-V-C分为三层，M(Model)-V(View)-C(Control)。而web开发中的三层架构是指：数据访问层(DAL-DatabaseAccessLayer)，业务逻辑层(BLL-BusinessLoginLayer)，以及用户界面层(UI-UserInterface，实际就是网页后台的具体调用BLL层)。这个是基本概念。曾经我以为三层架构就是在AppCode中，分为三个大类与若干小类，各司其职。在经过一番洗礼后，才发觉多么的无知。
首先AppCode中，放的是通用类，如数据库通用类，实现数据库连接，基本的SqlCommand创建，自定义CRUD的方法等，与三层架构毫无关系，就是常用的开发模式中存放类(Class)的文件夹。
其次，当使用三层架构时，一定是在大项目中，因为三层架构的目的是提高项目的松散性和降低项目的耦合度，使之更容易扩展或者维护。小项目使用了三层架构，由于过度的在意分层而导致了项目的复杂度增加。
创建三层架构的应用程序。我们必须对这三层分别创建不同的类库(ClassLibrary)，而不是普通的类(Class)。我们对于任何一个模块或者功能进行OOP，把它扩展为对象(面向对象的思想就是：将所操作的目标当成一个对象，对它进行的操作，将由对象自己的方法进行，而非外界传参。譬如注册用户，用面向过程的方法事先，就是:public static bool Register(string userName, string userPwd)。若用OO的思想，我们不可将账号密码作为参数传入，而是将用户作为一个对象，这个对象具有private
 _userName，和private _userPwd的属性。在注册时，用构造函数初始化一个新的对象，User one = new User(userName,userPwd)，使之在初始化后具有这两个字段的值。然后调用User类中的public static bool Register()方法(注意这个方法是不进行传参的)，而在这个Register方法中，使用对象的_userName和_userPwd属性进行注册。），那么，我们在这个对象中的任何操作都将以该对象的方法(函数)实现。
在进行三层分类时，这样新建类库。
先写模块的实体类，是数据库中表的抽象，假设数据库中注册信息只有账号，密码两个字段。那么抽象到实体类就是这样：
```cpp
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
namespace Entity
{
class UserInfo
{
public string UserName { get; set; }    //C#3.0中属性构造器的新写法；
public string UserPwd { get; set; }
}
}
```
再写DAL层：
```cpp
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using Entity;   //这里添加对Entity实体类的引用；
namespace DAL
{
public class UserDAL
{
//在该类中，为了方便，一般会构造一个DataBaseFactory，方便进行代码的操作。所以以下代码仅为逻辑实现，不代表代码正确。
public bool AddUser(UserInfo uInfo) //这里将实体类作为参数传入；
{
string sqlStr="INSERT INTO UserInfo(Name,Pwd) VALUES(@name,@pwd)";
SqlCommand cmd=new SqlCommand(sqlStr);
cmd.Parameters.Clear();
cmd.Parameters.Add("@name", SqlDbType.NVarChar, 50).Value = uInfo.UserName; //调用实体类的属性
cmd.Parameters.Add("@pwd", SqlDbType.NVarChar, 50).Value = uInfo.UserPwd;
return Convert.ToInt32(cmd.ExecuteNonQuery()) > 0 ？ true : false;
}
public DataTable GetUserInfo(string name)   //根据用户名获得用户的具体信息
{
string sqlStr="SELECT * FROM UserInfo WHERE Name=@name";
SqlCommand cmd=new SqlCommand(sqlStr);
cmd.Parameters.Clear();
cmd.Parameters.Add("@name", SqlDbType.NVarChar, 50).Value = name;
SqlDataAdapter sda = new SqlDataAdapter(cmd);
DataSet ds=new DataSet();
sda.Fill(ds,"UserInfo");
return ds.Tables["UserInfo"];
}
}
}
```
再写BLL层：
```cpp
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using Entity;   //添加对Entity类库的引用
using DAL;  //添加对DAL类库的引用
namespace BLL
{
public class UserBLL
{
public static bool AddUser(UserInfo uInfo)  //BLL层的方法多为静态方法，DAL层也可以为静态方法。
{
UserDAL uDal = new UserDAL();
DataTable dTable = uDal.GetUserInfo(uInfo.UserName);
if (dTable.Rows.Count > 0)  //这里对注册用户有一个判断，从DAL层中先通过注册名获得用户的具体信息，若可以获得则证明该用户名已被注册，返回false；
return false;
else
return uDal.AddUser(uInfo);
}
}
}
```
这样一个小的三层架构程序就出来了。
这个程序中，操作的实体为UserInfo表的抽象。在DAL层进行了AddUser()的方法，在BLL层也进行了AddUser()的方法，唯一的区别是BLL层做了逻辑判断，如果用户名存在，则注册失败。
三层架构的特点：
1.数据库访问层(DAL)仅提供对数据库的CRUD操作，而不管操作后的结果，也不管逻辑过程(譬如同名用户，不合法用户名)。
2.业务逻辑层(BLL)不会直接与数据库交互，他与数据库的交互是通过DAL提供的方法。在调用这些方法前，要加入自己的逻辑判断或者业务处理。另外业务逻辑层(BLL)还有可能不会去调用DAL层的方法，而是进行其他业务处理。
3.用户界面层(UI)层是不会调用DAL层的，他只调用BLL层提供的方法，再由BLL层自己决定是否继续调用DAL层。
这个例子可以看出三层架构的优点就是结构清晰，容易扩展与维护。缺点就是，复杂。仅仅一个注册用户，就这么麻烦，所以对于小项目来说，费这么大劲换取一个相对较清晰的分层结构是不划算的。 
