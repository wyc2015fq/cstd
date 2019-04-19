# 三层C#版实现 - Big Smile - CSDN博客
2015年12月17日 20:37:00[王啸tr1912](https://me.csdn.net/tr1912)阅读数：692
        三层，顾名思义就是把整个的逻辑结构分为了三个层次，整体上看这个功能有三个项目组成，分别是UI，BLL,DAL。另外还有一个之前一直没有想到的就是实体层（Model，视频中用的是这个，书上用的是s打头的一个），里面主要就是一些实体的属性和一些值，数据在UI ,BLL, DAL中传递靠的就是实体的各种属性，下面就来看一下具体是怎么实现的：
程序的主体是这样的：主要实现的是登陆的作用，主要是登陆验证，登陆成功后在Score里面写入成功数值10。
#         实体层（Model）：
```
namespace Login.Model
{
    public  class UserInfo
    {
        public int ID { get; set; }           //实体属性（外部获取属性，设定属性）
        public string UserName { get; set; }
        public string Password { get; set; }
        public string Email { get; set; }       
    }
}
```
#         首先是UI层的建立：
```
namespace LoginUI
{
    public partial class frmLogin : Form
    {
        public frmLogin()
        {
            InitializeComponent();
        }
        //Login.BLL.LoginService  a = new Login.BLL.LoginService();
        private void btnLogin_Click(object sender, EventArgs e)
        {                                                  //实体获值
            string userName = txtUsername.Text.Trim();     //获取用户名
            string password = txtPassword.Text;            //获取密码
            Login.BLL.LoginService svr = new Login.BLL.LoginService();     //实例化一个B层
            Login.Model.UserInfo user = svr.UserLogin(userName, password);   //调用B层，传入实体值
            MessageBox.Show("登录用户：" + user.UserName );     //登陆成功提示
        }
    }
}
```
主要行为就是把值传给实例化的实体，然后把实体传给B层，调用B层的方法。
# 然后是Dal层（分为UserDAO和ScoreDAO两个）：
链接字段：
```
namespace Login.DAL
{
    class DbUtil
    {
        public static string ConnString = @"Server=sampc;Database=Login;User ID=sa;Password=1"; //链接
    }
}
```
UserDAO：
```
namespace Login.DAL
{
    public  class UserDAO
    {
        public Login .Model .UserInfo  SelectUser(string userName, string password)
        {
            using(SqlConnection conn = new SqlConnection(DbUtil.ConnString))      //引用的数据对象链接
            {
                SqlCommand cmd = conn.CreateCommand();         //数据库操作对象
                cmd.CommandText = "SELECT ID,UserName,Password,Email FROM USERS WHERE UserName=@UserNa                                  me and Password=@Password";      //带@的变量为参数，取数据库中的字段
                cmd.CommandType = CommandType.Text;
                cmd.Parameters.Add(new SqlParameter("@UserName", userName));
                cmd.Parameters.Add(new SqlParameter("@Password", password ));
                conn.Open();             //链接打开
                SqlDataReader reader = cmd.ExecuteReader ();     //查询数据库
                Login.Model .UserInfo user=null;       //实例化Model，准备获取数据
                while (reader.Read())         //数据库读取对象读数据
                {
                    if (user == null)
                    {
                        user = new Login.Model.UserInfo();
                    }
                    user.ID = reader.GetInt32(0);
                    user.UserName = reader.GetString(1);
                    user.Password = reader.GetString(2);    //not suggestion
                    if(!reader.IsDBNull(3))
                    {
                        user.Email = reader.GetString(3);
                    }
                }
                return user;          //返回取值是否验证成功
                conn.Close();
            }
        }
    }
}
```
ScoreDAO：
```
namespace Login.DAL
{
    public  class ScoreDAO
    {
        public void UpdateScore(string userName, int value)      //与上面类似，value是要赋值的值
        {
            using (SqlConnection conn = new SqlConnection(DbUtil.ConnString))
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = "INSERT INTO SCORSE(UserName,Score) Values(@UserName,@Score)";//添加记录语句
                cmd.Parameters.Add(new SqlParameter("@UserName",userName));
                cmd.Parameters.Add(new SqlParameter ("@Score",value ));
                conn.Open();
                cmd.ExecuteNonQuery();    //执行查询
                conn.Close();
            }
        }
    }
}
```
# B层：
主要是调用D层进行一系列操作，并把值返回U层
```
namespace Login.BLL
{
    public class LoginService
    {
        public Login.Model.UserInfo  UserLogin(string userName, string password)  //返回类型为实体
        {
            Login.DAL.UserDAO  uDao = new Login.DAL.UserDAO();    //实例化D层对象
            Login.Model.UserInfo user = uDao.SelectUser(userName,password);    //实例化实体并传值调用D层查询
            if (user != null)     //判断D层返回值
            {
                Login.DAL.ScoreDAO sDao = new Login.DAL.ScoreDAO();      //实例化D层Score类
                sDao.UpdateScore(userName, 10);         //添加数据
                return user;
            }
            else 
            {
                throw new Exception("登录失败。");  
            }
        }
    }
}
```
总体上的数据流就是从U->B->D->B(判断)->U的过程，其中传值是以传实体的方式实现的。
# 传实体的好处：
        我认为传实体能够满足各层调用数据的完整性和复用性，可以减少一些不必要的传多值的耦合，也保证了数据的私密性。
