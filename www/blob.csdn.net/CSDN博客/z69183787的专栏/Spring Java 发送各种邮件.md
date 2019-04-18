# Spring Java 发送各种邮件 - z69183787的专栏 - CSDN博客
2014年04月12日 00:09:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：685
                Spring邮件抽象层的主要包为org.springframework.mail。它包括了发送电子邮件的主要接口MailSender，和值对象SimpleMailMessage，它封装了简单邮件的属性如from, to,cc, subject,text。包里还包含一棵以MailException为根的checkedException继承树，它们提供了对底层邮件系统异常的高级别抽象。要获得            
