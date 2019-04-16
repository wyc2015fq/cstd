# Java学习笔记28：keyStroe https证书 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年02月16日 13:17:37[initphp](https://me.csdn.net/initphp)阅读数：1195
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)











JDK中keytool常用命令

-genkey     在用户主目录中创建一个默认文件".keystore",还会产生一个mykey的别名，mykey中包含用户的公钥、私钥和证书

-alias      产生别名

-keystore   指定密钥库的名称(产生的各类信息将不在.keystore文件中

-keyalg     指定密钥的算法  

-validity   指定创建的证书有效期多少天

-keysize    指定密钥长度

-storepass   指定密钥库的密码

-keypass    指定别名条目的密码

-dname      指定证书拥有者信息 例如： "CN=sagely,OU=atr,O=szu,L=sz,ST=gd,C=cn"

-list       显示密钥库中的证书信息     keytool -list -v -keystore sage -storepass ....

-v          显示密钥库中的证书详细信息

-export     将别名指定的证书导出到文件  keytool -export -alias caroot -filecaroot.crt

-file       参数指定导出到文件的文件名

-delete     删除密钥库中某条目         keytool -delete -alias sage -keystore sage

-keypasswd  修改密钥库中指定条目口令   keytool -keypasswd -alias sage -keypass .... -new .... -storepass... -keystore sage

-import     将已签名数字证书导入密钥库  keytool -import -alias sage-keystore sagely -file sagely.crt

            导入已签名数字证书用keytool -list -v 以后可以明显发现多了认证链长度，并且把整个CA链全部打印出来。


1.证书的显示

-list

[-v | -rfc] [-alias <alias>]

[-keystore <keystore>] [-storepass<storepass>]

[-storetype <storetype>] [-provider<provider_class_name>]

例如：keytool -list -v -alias RapaServer -keystore cacerts -storepass12345678

keytool -list -v -keystore d2aapplet.keystore -storepass 12345678-storetype IAIKKeystore

2.将证书导出到证书文件

例如：keytool -export -keystore monitor.keystore -alias monitor -filemonitor.cer

将把证书库 monitor.keystore 中的别名为 monitor 的证书导出到 monitor.cer证书文件中，它包含证书主体的信息及证书的公钥，不包括私钥，可以公开。

keytool -export -keystore d2aApplet.keystore -alias RapaServer-file Rapa.cert -storetype IAIKKeystore

3.将keystore导入证书中

这里向Java默认的证书 cacerts导入Rapa.cert

keytool -import -alias RapaServer -keystore cacerts -file Rapa.cert-keystore cacerts

4.证书条目的删除

keytool的命令行参数 -delete 可以删除密钥库中的条目，如： keytool -delete -aliasRapaServer -keystore d2aApplet.keystore ，这条命令将 d2aApplet.keystore中的 RapaServer 这一条证书删除了。

5.证书条目口令的修改

使用 -keypasswd 参数，如：keytool -keypasswd -alias RapaServer -keystored2aApplet.keystore，可以以交互的方式修改 d2aApplet.keystore证书库中的条目为 RapaServer的证书。

Keytool -keypasswd -alias RapaServer -keypass 654321 -new 123456-storepass 888888 -keystore d2aApplet.keystore这一行命令以非交互式的方式修改库中别名为RapaServer 的证书的密码为新密码 654321，行中的 123456 是指该条证书的原密码， 888888是指证书库的密码。



做的事情是：
- 创建一个keystore my.keystore，其中包含一个别名为my1stcert的证明书；
- 然后导出该证明书到my1stcert.cer文件;
- 查看证明书文件的内容；
- 再将文件my1stcert.cer导入到my.keystore中,其别名为my2ndcert；
- 查看keystore的内容。

最终，my.keystore中存放了两个证明书，他们是一样的，但是别名不一样。



D:j2sdk1.4.2_11bin>keytool.exe -genkey-alias my1stcert -keystore d:/mshuai/my.keystore -storepass"yechao" -keypass "yechao" -keyalg "RSA"
What is yourfirst and last name?

[Unknown]: Ye Chao

What is the name of your organizational unit?

[Unknown]: 3k

What is the name of your organization?

[Unknown]: rits

What is the name of your City or Locality?

[Unknown]: shanghai

What is the name of your State or Province?

[Unknown]: shanghai

What is the two-letter country code for this unit?

[Unknown]: zh

Is CN=Ye Chao, OU=3k, O=rits, L=shanghai, ST=shanghai, C=zhcorrect?

[no]: yes



D:j2sdk1.4.2_11bin>keytool.exe -export-alias my1stcert -keystore my.keystore -filemy1cert.cer
Enter keystore password:yechao

Certificate stored in file<my1cert.cer>

D:j2sdk1.4.2_11bin>keytool.exe -printcert-file my1cert.cer
Owner: CN=Ye Chao, OU=3k, O=rits,L=shanghai, ST=shanghai, C=zh

Issuer: CN=Ye Chao, OU=3k, O=rits, L=shanghai, ST=shanghai,C=zh

Serial number: 4695a81f

Valid from: Thu Jul 12 12:03:43 GMT+08:00 2007 until: Wed Oct 1012:03:43 GMT+08:00 2007

Certificate fingerprints:

        MD5: F8:8E:3E:41:59:8A:34:F5:DF:0F:4B:DE:37:EB:87:1F

        SHA1:8C:D1:B0:50:9B:2C:1D:E9:D5:1C:CF:51:F0:8B:CA:DF:1D:8D:AD:DF

D:j2sdk1.4.2_11bin>keytool.exe -import -alias my2ndcert -file my1cert.cer-keystore my.keystore
Enter keystore password:yechao

Owner: CN=Ye Chao, OU=3k, O=rits, L=shanghai, ST=china,C=     ZH

Issuer: CN=Ye Chao, OU=3k, O=rits, L=shanghai, ST=china,C=    ZH

Serial number: 4695a009

Valid from: Thu Jul 12 11:29:13 GMT+08:00 2007 until: Wed Oct 1011:29:13 GMT+08:00 2007

Certificate fingerprints:

        MD5: 88:AF:00:DA:4E:45:5E:60:F8:DB:5E:62:4E:E3:65:57

        SHA1:0F:FD:D8:0D:AD:E2:5B:DA:4E:A2:89:F0:4C:1B:B1:63:9D:1F:7F:89

Trust this certificate? [no]: yes

Certificate was added to keystore

D:j2sdk1.4.2_11bin>keytool.exe -list-keystore my.keystore
Enter keystore password:yechao

Keystore type: jks

Keystore provider: SUN

Your keystore contains 2 entries

my2ndcert, Jul 12, 2007, trustedCertEntry,

Certificate fingerprint (MD5):88:AF:00:DA:4E:45:5E:60:F8:DB:5E:62:4E:E3:65:57

my1stcert, Jul 12, 2007, keyEntry,

Certificate fingerprint (MD5):F8:8E:3E:41:59:8A:34:F5:DF:0F:4B:DE:37:EB:87:1F





