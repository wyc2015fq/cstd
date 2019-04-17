# php在windows下发送邮件实现 - 在思索中前行！ - CSDN博客





2014年05月16日 10:28:04[_Tham](https://me.csdn.net/txl16211)阅读数：3599








## **一、使用PHP内置的mail()函数**


看了一下手册，就直接开始写代码了，如下：



```php
<?php
$to = "test@126.com"; //收件人
$subject = "Test"; //主题
$message = "This is a test mail!"; //正文
mail($to,$subject,$message);
?>
```





结果就直接报错，如下：

**Warning**: mail() [[function.mail](http://blog.csdn.net/rainday0310/article/details/function.mail)]: Failed to connect to mailserver at "localhost"
 port 25, verify your "SMTP" and "smtp_port" setting in php.ini or use ini_set() in**D:/www/email/email.php** on line **10**
通過internet搜索了相關的回答,包括相關的技術博客,大家都是很簡單的闡述了解決方案.大多是只是提到修改:php.ini 中的[mail function] 函數,將smtp的相關功能進行打開即可,以下是詳細的修改:
 [mail function]
 SMTP = localhost
 smtp_port = 25
 sendmail_from = youremailUsual@domin.com
 ;這裡有必要註釋下,這個郵件是From方面.
 sendmail_path = ""
 ;起初是空的,這裡在後面做說明.
 mail_add_x_header = on
 ;這裡是默認on的 無需更改
 到這一步,修改完畢,重啟apache,當然這個操作结果还是Error.

  仔细思考，为什么修改配置文件后还是error，突然意识到，我的本地没有stmp服务器，那怎么可能能发送邮件呢？因此，在自己的本地必须搭建一个stmp服务器，但是想到win7自带的IIS中有Smtp的功能，但是此功能是专对应于asp的，因此估计php也指望不上它。那就使用别人的试试吧，又改了下代码：






```php
<?php
```

```php
$to = "test@163.com";//收件人
   $subject = "Test";//邮件主题
   $message = "This is a test mail!";//邮件正文
   ini_set('SMTP','smtp.163.com');//发件SMTP服务器
   ini_set('smtp_port',25);//发件SMTP服务器端口
   ini_set('sendmail_from',"admin@163.com");//发件人邮箱
   mail($to,$subject,$message);
   ?>
```




结果还是错误：

**Warning**: mail() [[function.mail](http://blog.csdn.net/rainday0310/article/details/function.mail)]: SMTP server response: 553 authentication is required,smtp2,DNGowKD7v5BTDo9NnplVBA--.1171S2
 1301220947 in**D:/www/email/email.php** on line**9**

**之後改了其他的郵箱,如QQ,Gamil等,錯誤都如上面提示,看了一個熟悉的單詞authorized ,就明白啥意思了.    到這一步貌似離成功不遠了,想起以前使用客戶端配置郵箱用戶的時候,最後一步都會詢問你需要開啟"服務器身份驗證"**

**需要验证信息，怎么写验证信息呢？在哪里配置呢？上网找了半天也没找出个所以然，最后看了别人一些技术文章后得出结论（由于对SMTP邮件什么的不是非常了解）：使用mail()函数发送邮件就必须要有一台无需SMTP验证就可以发信的邮件服务器。但现在的SMTP邮件服务器基本上都是需要验证的，所以要想使用它发邮件就只能自己在本地搭一个需要验证的SMTP服务器。**

      sendmail包实现Smtp邮件发送服务验证。

php
 mail()函数在windows不能用，需要安装sendmail
    1.从http://glob.com.au/sendmail/下载sendmail.zip
    2.解压到D:\php下，例如D:\php\sendmail

修改一個叫做sendmail.ini的文件,修改的地方如下:




 ;take the fomus email provider  in China 126 for example

   [sendmail]

   smtp_server= smtp.126.com  

   smtp_port=25

   default_domin =126.com

   auth_username= strings in your email address before char '@'    

   auth_password= your password which allow you to login in your email-box

   hostname= 126.com

   之後,你就要修改 php.ini,還是老地方:

   SMTP = smtp.126.com

   ;這裡修改為localhost 也是一樣,其他選項保持不變.

   smtp_port = 25

   sendmail_from = my126EmailAddress@126.com

   sendmail_path = "your sendmail path/sendemail.exe -t "

   ;當然這個路徑的前半部分必須包括你的PHP安裝的根目錄,將"\"換成"/",我想是必須要做的.

   mail_add_x_header = on

4.根据你自己的配置环境修改sendmail.ini。
        第一次最好启用debug.log_file,error_logfile，以查看sendmail是否生效。
      5.重启apache


到這裡配置完成,重啟你的apache,刷新頁面,錯誤的消失,因為程序中設置的$to是自己的126和QQ郵箱,當然循環發的.

```php
<?php
$now = date('Y-m-d h:i:s');
$from_name = '测试寄件人';
$from_email = 'info@test.com';
$headers = "From: $from_name <$from_email>";
$to = 'to@test.com';  //收件人邮件地址
$body = "嗨, \n 这是一封测试邮件来自 $from_name <$from_email>.";
$subject = "[$now] 测试邮件发送";
if (mail($to, $subject, $body, $headers)) {
echo 'success!';
} else {
echo 'fail…';
}
?>
```

## **二、使用封装SMTP协议的邮件类**


这种方法就比较常见了，尤其对于广大自己没有服务器，从网上购买虚拟主机的同学，


第一种方法不现实，所以还是自己使用SMTP协议来发送邮件吧。


不过要完成这项工作的话，就需要你对SMTP协议有一定的了解，喜欢事必躬亲的同学可以自己动手写一个，喜欢拿来主义的同学就可以从网上下载了，有很多。


不过我比较推荐使用PEAR扩展中的Mail类，功能强大：可以支持纯文本、HTML格式的邮件；各字段都可设置编码，正确配置不会出现中文乱码情况；可以支持附件等等。


在服务器可以使用 pear install Mail 命令快速安装，没有足够服务器权限的同学也可以直接下载类的PHP源码包含进来就可以了。


注：Mail类依赖  Net/SMTP.php  和 Mail/mime.php ，要一块下载，使用时一块包含进来。


详细安装方法可以在官网查看，[http://pear.php.net/package/Mail](http://pear.php.net/package/Mail)。


下面我举例说明一下在Mail类发送邮件的方法吧，网上其他SMTP邮件类使用方法一块也类似，可以参考：

```php
<?php

// Pear Mail 扩展
require_once('Mail.php');
require_once('Mail/mime.php');
require_once('Net/SMTP.php');

$smtpinfo = array();
$smtpinfo["host"] = "smtp.163.com";//SMTP服务器
$smtpinfo["port"] = "25"; //SMTP服务器端口
$smtpinfo["username"] = "username@163.com"; //发件人邮箱
$smtpinfo["password"] = "password";//发件人邮箱密码
$smtpinfo["timeout"] = 10;//网络超时时间，秒
$smtpinfo["auth"] = true;//登录验证
//$smtpinfo["debug"] = true;//调试模式

// 收件人列表
$mailAddr = array('receiver@163.com');

// 发件人显示信息
$from = "Name <username@163.com>";

// 收件人显示信息
$to = implode(',',$mailAddr);

// 邮件标题
$subject = "这是一封测试邮件";

// 邮件正文
$content = "<h3>随便写点什么</h3>";

// 邮件正文类型，格式和编码
$contentType = "text/html; charset=utf-8";

//换行符号 Linux: \n  Windows: \r\n
$crlf = "\n";
$mime = new Mail_mime($crlf);
$mime->setHTMLBody($content);

$param['text_charset'] = 'utf-8';
$param['html_charset'] = 'utf-8';
$param['head_charset'] = 'utf-8';
$body = $mime->get($param);

$headers = array();
$headers["From"] = $from;
$headers["To"] = $to;
$headers["Subject"] = $subject;
$headers["Content-Type"] = $contentType;
$headers = $mime->headers($headers);

$smtp =& Mail::factory("smtp", $smtpinfo);


$mail = $smtp->send($mailAddr, $headers, $body);
$smtp->disconnect();

if (PEAR::isError($mail)) {
    //发送失败
    echo 'Email sending failed: ' . $mail->getMessage()."\n";
}
else{
    //发送成功
    echo "success!\n";
}
```


如果从网上找的SMTP类都是高度封装的，所以使用起来比上面会更简单，但使用方法都是比较相似的。

**结论：**这种方式发送邮件无需装任何软件，只需要包含进来一个PHP类，然后多写几行配置代码，就可以了。并且网上有很多示例的代码，很多时候只要复制过来然后修改个别的几个参数就可以用了，所以会很方便，推荐使用此方法。






## [PHP发送邮件（PHPMailer）](http://www.cnblogs.com/FTD2012/archive/2012/12/01/2795052.html)




      利用PHPMailer来发送邮件方法。

　　我们在做project的时候常常需要邮件的功能。其实PHP语言自己已经有一个发送邮件的方法（mail()方法），且不说这个方法实现的功能很少，想要利用mail()方法来发送邮件就必须要自己配置好smtp服务器，这里就不讲mail()的使用方法了（函数调用确实很简单）。所以，我们推荐使用第二种方法：PHPMailer。

　　首先，先到[http://phpmailer.sourceforge.net/](http://phpmailer.sourceforge.net/)这个地址去下载最新的PHPMailer的包（PHPMailer方法必须使用这个包）。

　　下载完成后解压到相应的目录。可以看到解压后的文件夹里面的class.phpmailer.php(调用PHPMailer需要包含这个文件)

　　示例代码：



```php
<?php    
require("phpmailer/class.phpmailer.php");    
function smtp_mail( $sendto_email, $subject, $body, $extra_hdrs, $user_name){    
    $mail = new PHPMailer();    
    $mail->IsSMTP();                  // send via SMTP    
    $mail->Host = "smtp.163.com";   // SMTP servers    
    $mail->SMTPAuth = true;           // turn on SMTP authentication    
    $mail->Username = "xuchao842363331";     // SMTP username  注意：普通邮件认证不需要加 @域名  这里是我的163邮箱
    $mail->Password = "password"; // SMTP password    在这里输入邮箱的密码
    $mail->From = "xuchao842363331@163.com";      // 发件人邮箱    
    $mail->FromName =  "管理员";  // 发件人    
  
    $mail->CharSet = "UTF-8";   // 这里指定字符集！    指定UTF-8后邮件的标题和发件人等等不会乱码，如果是GB2312标题会乱码
    $mail->Encoding = "base64";    
    $mail->AddAddress($sendto_email,"username");  // 收件人邮箱和姓名    
    $mail->AddReplyTo("yourmail@yourdomain.com","yourdomain.com");    
    //$mail->WordWrap = 50; // set word wrap 换行字数    
    //$mail->AddAttachment("/var/tmp/file.tar.gz"); // attachment 附件    
    //$mail->AddAttachment("/tmp/image.jpg", "new.jpg");    
    //$mail->IsHTML(true);  // send as HTML    
    // 邮件主题    
    $mail->Subject = $subject;    
    // 邮件内容    
    $mail->Body = "hello！PHPMailer";                                                                          
    //$mail->AltBody ="text/html";    
    if(!$mail->Send())    
    {    
        echo "error <p>";    
        echo "error: " . $mail->ErrorInfo;    
        exit;    
    }    
    else {    
        echo"success!"; 
    }    
}    
// 参数说明(发送到, 邮件主题, 邮件内容, 附加信息, 用户名)      
?>
```

　　这里要说明一下：当字符集指定为GB2312的时候标题会乱码，这里指定为UTF-8不会出现乱码。

　　其实，PHPMailer还有很多功能，比如可以添加附件等等，这里就不再演示了。　

　　这样在你需要用邮件功能的时候调用这个函数就可以了：



```
```php
<?php
```



```php
require
```

```php
(
```

```php
"mail.php"
```

```php
);
```



```php

```



```php
smtp_mail(
```

```php
"842363331@qq.com"
```

```php
,
```

```php
"催还"
```

```php
,
```

```php
""
```

```php
,
```

```php
""
```

```php
,
```

```php
"username"
```

```php
);
```





```php
?>
```
```



 总的来说，这个方法还是很简单的，读者感兴趣的可以学习一下












