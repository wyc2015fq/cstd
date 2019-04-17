# Https Tomcat CSR生成 以及相关操作 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月30日 13:25:57[boonya](https://me.csdn.net/boonya)阅读数：1626








### **生成**

##### keytool -genkey -alias server -keyalg RSA -keysize 2048 -keystore C:\keystore.jks -storepass password -keypass password![](http://www.pianyissl.com/resource/img/tomcat-1.png)注意：除了“该单位的双字母国家/地区代码是什么?”填写大写的CN，其他都填域名(图片上的www.pianyissl.com 要改成你自己的域名噢）

### **生成CSR**

##### keytool -certreq -alias server -sigalg SHA1withRSA -file C:\certreq.csr -keystore C:\keystore.jks -keypass password -storepass password生成后在本站订单页面提交CSR文件（也就是certreq.csr的内容）。完成证书申请后再进入下面步骤。**导入中级证书(3个文件，取自ca.crt，用记事本打开-----begin----- .....-----end----- 保存为一个文件)**keytool -import -alias intermediate1 -keystore C:\keystore.jks -trustcacerts -storepass password -file C:\cert2\1.crtkeytool -import -alias intermediate2 -keystore C:\keystore.jks -trustcacerts -storepass password -file C:\cert2\2.crtkeytool -import -alias intermediate3 -keystore C:\keystore.jks -trustcacerts -storepass password -file C:\cert2\3.crt**导入服务器证书（域名.crt文件）**keytool -import -alias server -keystore C:\keystore.jks -trustcacerts -storepass password -file C:\cert2\4.crt

### **查看**

**keytool -list -keystore C:\keystore.jks -storepass password**

原文地址：http://www.pianyissl.com/support/page/25




