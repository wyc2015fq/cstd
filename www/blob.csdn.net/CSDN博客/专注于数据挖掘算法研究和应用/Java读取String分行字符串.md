# Java读取String分行字符串 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月29日 16:33:35[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：9560
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









1、场景：String字符串中带有分行，需要提取每一行出来处理。

2、参考代码如下：



```java
public static void main(String[] args) throws IOException{
		 String s="1\r\n2\r\n3\r\n \r\nabd\r\n";
		 BufferedReader br = new BufferedReader(new InputStreamReader(new ByteArrayInputStream(s.getBytes(Charset.forName("utf8"))), Charset.forName("utf8")));  
		 String line;  
		 StringBuffer strbuf=new StringBuffer();
		 while ( (line = br.readLine()) != null ) {  
		     if(!line.trim().equals("")){
		    	 line="<br>"+line;//每行可以做加工
		    	 strbuf.append(line+"\r\n");
		     }       		    	 
		 } 
	     System.out.println(strbuf.toString());
	}
```


执行结果：





```
<br>1
<br>2
<br>3
<br>abd
```





