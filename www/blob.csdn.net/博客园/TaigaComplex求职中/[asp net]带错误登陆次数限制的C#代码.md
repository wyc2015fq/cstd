# [asp.net]带错误登陆次数限制的C#代码 - TaigaComplex求职中 - 博客园







# [[asp.net]带错误登陆次数限制的C#代码](https://www.cnblogs.com/TaigaCon/archive/2012/08/26/2657684.html)





一旦登陆次数超过3次则账号被锁定，锁定后无论密码是否正确都不能登陆，

锁定后60分钟解除锁定，

采用强类型dataset，

数据库设计：
|用户ID|用户名|用户密码|用户等级|错误次数|错误时间|
|----|----|----|----|----|----|
|UID|UName|PW|lev|ErrorTimes|ErrorDateTime|



dataset新增方法：

　　ResetErrorTimes()重置错误次数

　　IncError() 增加错误次数，设置当前时间为错误时间



C#代码：

```
1 protected void Button1_Click(object sender, EventArgs e)
 2         {
 3             T_LoginTableAdapter adapter = new T_LoginTableAdapter();
 4             var info = adapter.GetDataByUName(TextBox1.Text);
 5             var row = info.Single();                 //等到数据是否唯一？，不唯一则出错
 6 
 7 /////////////////////////////  上次错误时间 〉60?  //////////////////////////////////////////
 8             if (!row.IsNull("ErrorDateTime"))
 9             {
10                 if ((new DateTime() - row.ErrorDateTime).TotalMinutes > 60)
11                 {
12                     adapter.ResetErrorTimes(TextBox1.Text);
13                 }
14             }
15 ////////////////////////////////////////////////////////////////////////////////////////////
16 
17 ////////////////////////////////错误次数 > 3？/////////////////////////////////////////////
18             if (!row.IsNull("ErrorTimes"))
19             {
20                 if (row.ErrorTimes >= 3)
21                 {
22                     Label4.Visible = true;
23                     return;
24                 }
25             }
26 /////////////////////////////////////////////////////////////////////////////////////////////
27 
28 ////////////////////////////////密码是否正确？////////////////////////////////////////////////
29             if (row.PW!= TextBox2.Text)
30             {
31                 adapter.IncError(TextBox1.Text);
32                 return;
33             }
34 /////////////////////////////////////////////////////////////////////////////////////////////
35 
36 ///////////////////////////////登陆成功 ////////////////////////////////////////////////
37             adapter.ResetErrorTimes(TextBox1.Text);
38             Session["level"] = row.Lev;
39             Session["Login"] = "true";
40             Session["ID"] = TextBox1.Text;
41             Response.Redirect("Download.htm");
42 //////////////////////////////////////////////////////////////////////////////////////////////
43         }
```












