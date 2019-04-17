# Java解码网页表单post内容小记 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月27日 14:59:41[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2653
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









对网页表单post提交的数据进行解码。一般编码是base64或application/x-www-form-urlencoded类型，本文主要是针对这个实现。当然有很多是js加密的，就只能看怎么破解js中的加密算法了。还有如ContentType文件类型是text/octet纯二进制文件的，不知如何解码。

一般解码代码如下，仅供参考。



```java
public static void main(String[] args) throws Exception {
	String line="ifacelog.iqiyi.com|dnY9MCUwOTIwMTYtMDctMjYrMDAlM0EwNCUzQTAwJTA5MzUyNzQ2MDY2Njc0MjUyJTA5R1Bob25lJTA5NC40LjIlMDkxOTIwKjEwODAlMDlTTS1OOTAwJTA5MSUwOTExODk4ODAwMzElMDk2OTg0MjY0MjQ4M2FkZDBhNjM1MDMzMDZkNjNmMDQ0MyUwOTcuMyUwOTUxMzE0MDkwMCUwOTEzJTA5MSUwOTEwJTA5JTA5MCUwOTAlMDkxJTA5MSUwOTIxNTMlMDkxJTA5NzI3NDglMDkxNiUwOTAlMDkxJTA5MjAwJTA5MCUwOTExMy4wNzQ0MyUyQzIyLjg2ODM4NCUwOTIwMCUwOTUxMzE0MDkwMCUwOTAlMDkxMSUwOSUwOWE0MzQ0YWJlNmU1NmE5MjgwYmViMGVkNWFiNTc0ZTc2JTA5ZTc3Yjg3OTkyNzQzNTJjZiUwOSUwOTElMDkwJTA5MSUwOTc0MjE3JTA5MCUwOTElMDk2MTNBMDFFNjYzNkY1MTc2NUNDN0M4QTYzOTM3RjVGQyUwOSUyNnVwZGVyaWQlM0QlMDklMDkwJTA5JTA5JTA5JTA5cXlfaG9tZSUyQ1IlM0EyMDQ4MzUxMTIlM0E2JTJDMTAwJTJDKyUyQzElMDklMDklMDkwJTA5MCUwOTAlMDkwJTA5LTElMDklN0IlMjJydHlwZSUyMiUzQSUyMjElMjIlMkMlMjJwb3AlMjIlM0ElMjIxJTIyJTJDJTIycG9zJTIyJTNBJTIyMCUyMiU3RCUwOSUwOXZiN3JuaGExY2s1a3pvdGQlMDkwJTA5Y25fcw==";
	String[] strPosts=line.split("\\|");//获取行，并按照|分隔符提取
	String domain=strPosts[0];//domian 
	String postdata=strPosts[1];//content
	System.out.println(postdata);
	String lastStr=postdata.substring(postdata.length()-1);//获取最后一位
	if(lastStr.equals("=")){ //符合base64编码特征，用base64解码
		byte[] bEncode=postdata.getBytes();//获取post串的字节
		byte[] bDecode= it.sauronsoftware.base64.Base64.decode(bEncode);//用base64解码
		postdata=new String(bDecode);
		System.out.println(postdata);
	}
	if(postdata.contains("%") || postdata.contains("+")){
	        //原串和经过base64解码后的串中合application/x-www-form-urlencoded类型             	
	        //支持text/plain类型
	        String charSet="UTF-8";//默认
	        try{//获取域名，返回服务器编码方式ContentType 
	        	URL u = new URL("http://"+domain);  
		        URLConnection uc = u.openConnection();  
		        String type = uc.getContentType();
		        if(type!=null) {
		            	int i=type.indexOf("=");
		            	if(i!=-1) charSet=type.substring(i+1, type.length());
		        }           
		        System.out.println(charSet);
		        String content=URLDecoder.decode(postdata,charSet);
		        System.out.println(content);
	        }catch(IOException e){e.printStackTrace();}
	}
}
```


执行后结果：



```
dnY9MCUwOTIwMTYtMDctMjYrMDAlM0EwNCUzQTAwJTA5MzUyNzQ2MDY2Njc0MjUyJTA5R1Bob25lJTA5NC40LjIlMDkxOTIwKjEwODAlMDlTTS1OOTAwJTA5MSUwOTExODk4ODAwMzElMDk2OTg0MjY0MjQ4M2FkZDBhNjM1MDMzMDZkNjNmMDQ0MyUwOTcuMyUwOTUxMzE0MDkwMCUwOTEzJTA5MSUwOTEwJTA5JTA5MCUwOTAlMDkxJTA5MSUwOTIxNTMlMDkxJTA5NzI3NDglMDkxNiUwOTAlMDkxJTA5MjAwJTA5MCUwOTExMy4wNzQ0MyUyQzIyLjg2ODM4NCUwOTIwMCUwOTUxMzE0MDkwMCUwOTAlMDkxMSUwOSUwOWE0MzQ0YWJlNmU1NmE5MjgwYmViMGVkNWFiNTc0ZTc2JTA5ZTc3Yjg3OTkyNzQzNTJjZiUwOSUwOTElMDkwJTA5MSUwOTc0MjE3JTA5MCUwOTElMDk2MTNBMDFFNjYzNkY1MTc2NUNDN0M4QTYzOTM3RjVGQyUwOSUyNnVwZGVyaWQlM0QlMDklMDkwJTA5JTA5JTA5JTA5cXlfaG9tZSUyQ1IlM0EyMDQ4MzUxMTIlM0E2JTJDMTAwJTJDKyUyQzElMDklMDklMDkwJTA5MCUwOTAlMDkwJTA5LTElMDklN0IlMjJydHlwZSUyMiUzQSUyMjElMjIlMkMlMjJwb3AlMjIlM0ElMjIxJTIyJTJDJTIycG9zJTIyJTNBJTIyMCUyMiU3RCUwOSUwOXZiN3JuaGExY2s1a3pvdGQlMDkwJTA5Y25fcw==
vv=0%092016-07-26+00%3A04%3A00%09352746066674252%09GPhone%094.4.2%091920*1080%09SM-N900%091%091189880031%0969842642483add0a63503306d63f0443%097.3%09513140900%0913%091%0910%09%090%090%091%091%092153%091%0972748%0916%090%091%09200%090%09113.07443%2C22.868384%09200%09513140900%090%0911%09%09a4344abe6e56a9280beb0ed5ab574e76%09e77b8799274352cf%09%091%090%091%0974217%090%091%09613A01E6636F51765CC7C8A63937F5FC%09%26upderid%3D%09%090%09%09%09%09qy_home%2CR%3A204835112%3A6%2C100%2C+%2C1%09%09%090%090%090%090%09-1%09%7B%22rtype%22%3A%221%22%2C%22pop%22%3A%221%22%2C%22pos%22%3A%220%22%7D%09%09vb7rnha1ck5kzotd%090%09cn_s
utf-8
vv=0	2016-07-26 00:04:00	352746066674252	GPhone	4.4.2	1920*1080	SM-N900	1	1189880031	69842642483add0a63503306d63f0443	7.3	513140900	13	1	10		0	0	1	1	2153	1	72748	16	0	1	200	0	113.07443,22.868384	200	513140900	0	11		a4344abe6e56a9280beb0ed5ab574e76	e77b8799274352cf		1	0	1	74217	0	1	613A01E6636F51765CC7C8A63937F5FC	&upderid=		0				qy_home,R:204835112:6,100, ,1			0	0	0	0	-1	{"rtype":"1","pop":"1","pos":"0"}		vb7rnha1ck5kzotd	0	cn_s
```








