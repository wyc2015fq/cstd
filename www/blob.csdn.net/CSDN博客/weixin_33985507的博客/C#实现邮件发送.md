# C#实现邮件发送 - weixin_33985507的博客 - CSDN博客
2019年01月07日 22:57:11[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
C#实现邮件发送
[原文链接](http://www.xxlinux.com/linux/dev/c/2007-11-26/12752.html)
通过.Net FrameWork 2.0下提供的“System.Net.Mail”可以轻松的实现，本文列举了3种途径来发送： 
  1.通过Localhost； 
  2.通过普通SMTP; 
  3.通过SSL的SMTP; 
  下面一个一个来说： 
  1.通过LocalHost 
  1public void SendMailLocalhost() 
  2 ...{ 
  3 System.Net.Mail.MailMessage msg = new System.Net.Mail.MailMessage(); 
  4 msg.To.Add("a@a.com"); 
  5 msg.To.Add("b@b.com"); 
  6 /**//* 
  7 * msg.To.Add("b@b.com"); 
  8 * msg.To.Add("b@b.com"); 
  9 * msg.To.Add("b@b.com");可以发送给多人 
  10 */ 
  11 msg.CC.Add("c@c.com"); 
  12 /**//* 
  13 * msg.CC.Add("c@c.com"); 
  14 * msg.CC.Add("c@c.com");可以抄送给多人 
  15 */ 
  16 msg.From = new MailAddress("a@a.com", "AlphaWu", System.Text.Encoding.UTF8); 
  17 /**//* 上面3个参数分别是发件人地址（可以随便写），发件人姓名，编码*/ 
  18 msg.Subject = "这是测试邮件";//邮件标题 
  19 msg.SubjectEncoding = System.Text.Encoding.UTF8;//邮件标题编码 
  20 msg.Body = "邮件内容";//邮件内容 
  21 msg.BodyEncoding = System.Text.Encoding.UTF8;//邮件内容编码 
  22 msg.IsBodyHtml = false;//是否是HTML邮件 
  23 msg.Priority = MailPriority.High;//邮件优先级 
  24 
  25 SmtpClient client = new SmtpClient(); 
  26 client.Host = "localhost"; 
  27 object userState = msg; 
  28 try 
  29 ...{ 
  30 client.SendAsync(msg, userState); 
  31 //简单一点儿可以client.Send(msg); 
  32 MessageBox.Show("发送成功"); 
  33 } 
  34 catch (System.Net.Mail.SmtpException ex) 
  35 ...{ 
  36 MessageBox.Show(ex.Message, "发送邮件出错"); 
  37 } 
  38 } 
  39 
  2.通过普通SMTP 
  1public void SendMailUseZj() 
  2 ...{ 
  3 System.Net.Mail.MailMessage msg = new System.Net.Mail.MailMessage(); 
  4 msg.To.Add("a@a.com"); 
  5 msg.To.Add("b@b.com"); 
  6 /**//* 
  7 * msg.To.Add("b@b.com"); 
  8 * msg.To.Add("b@b.com"); 
  9 * msg.To.Add("b@b.com");可以发送给多人 
  10 */ 
  11 msg.CC.Add("c@c.com"); 
  12 /**//* 
  13 * msg.CC.Add("c@c.com"); 
  14 * msg.CC.Add("c@c.com");可以抄送给多人 
  15 */ 
  16 msg.From = new MailAddress("a@a.com", "AlphaWu", System.Text.Encoding.UTF8); 
  17 /**//* 上面3个参数分别是发件人地址（可以随便写），发件人姓名，编码*/ 
  18 msg.Subject = "这是测试邮件";//邮件标题 
  19 msg.SubjectEncoding = System.Text.Encoding.UTF8;//邮件标题编码 
  20 msg.Body = "邮件内容";//邮件内容 
  21 msg.BodyEncoding = System.Text.Encoding.UTF8;//邮件内容编码 
  22 msg.IsBodyHtml = false;//是否是HTML邮件 
  23 msg.Priority = MailPriority.High;//邮件优先级 
  24 
  25 SmtpClient client = new SmtpClient(); 
  26 client.Credentials = new System.Net.NetworkCredential("username@zj.com", "userpass"); 
  27 //在zj.com注册的邮箱和密码 
  28 client.Host = "smtp.zj.com"; 
  29 object userState = msg; 
  30 try 
  31 ...{ 
  32 client.SendAsync(msg, userState); 
  33 //简单一点儿可以client.Send(msg); 
  34 MessageBox.Show("发送成功"); 
  35 } 
  36 catch (System.Net.Mail.SmtpException ex) 
  37 ...{ 
  38 MessageBox.Show(ex.Message, "发送邮件出错"); 
  39 } 
  40 } 
  41 
  上述方法不适用于所有SMTP，经测试zj.com可以，而smtp.163.com不行 
  3.通过SSL的SMTP 
  1public void SendMailUseGmail() 
  2 ...{ 
  3 System.Net.Mail.MailMessage msg = new System.Net.Mail.MailMessage(); 
  4 msg.To.Add("a@a.com"); 
  5 msg.To.Add("b@b.com"); 
  6 /**//* 
  7 * msg.To.Add("b@b.com"); 
  8 * msg.To.Add("b@b.com"); 
  9 * msg.To.Add("b@b.com");可以发送给多人 
  10 */ 
  11 msg.CC.Add("c@c.com"); 
  12 /**//* 
  13 * msg.CC.Add("c@c.com"); 
  14 * msg.CC.Add("c@c.com");可以抄送给多人 
  15 */ 
  16 msg.From = new MailAddress("a@a.com", "AlphaWu", System.Text.Encoding.UTF8); 
  17 /**//* 上面3个参数分别是发件人地址（可以随便写），发件人姓名，编码*/ 
  18 msg.Subject = "这是测试邮件";//邮件标题 
  19 msg.SubjectEncoding = System.Text.Encoding.UTF8;//邮件标题编码 
  20 msg.Body = "邮件内容";//邮件内容 
  21 msg.BodyEncoding = System.Text.Encoding.UTF8;//邮件内容编码 
  22 msg.IsBodyHtml = false;//是否是HTML邮件 
  23 msg.Priority = MailPriority.High;//邮件优先级 
  24 
  25 SmtpClient client = new SmtpClient(); 
  26 client.Credentials = new System.Net.NetworkCredential("username@gmail.com", "password"); 
  27 //上述写你的GMail邮箱和密码 
  28 client.Port = 587;//Gmail使用的端口 
  29 client.Host = "smtp.gmail.com"; 
  30 client.EnableSsl = true;//经过ssl加密 
  31 object userState = msg; 
  32 try 
  33 ...{ 
  34 client.SendAsync(msg, userState); 
  35 //简单一点儿可以client.Send(msg); 
  36 MessageBox.Show("发送成功"); 
  37 } 
  38 catch (System.Net.Mail.SmtpException ex) 
  39 ...{ 
  40 MessageBox.Show(ex.Message, "发送邮件出错"); 
  41 } 
  42 } 
  43 
  通过Gmail来发送邮件，成功率极高，几乎都可以发到，推荐使用。 
