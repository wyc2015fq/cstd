# Android测试写入文本Log - wishchinYang的专栏 - CSDN博客
2015年08月03日 20:51:27[wishchin](https://me.csdn.net/wishchin)阅读数：424

写入本地SD卡：
```java
@SuppressLint("SdCardPath")
    public void writeFileSdcard(String fileName, String message) {  
        try {  
            FileOutputStream fout = new FileOutputStream(fileName);  
            byte[] bytes = message.getBytes();  
            fout.write(bytes);  
            fout.close();  
        }  
        catch (Exception e) {  
            e.printStackTrace();  
        }  
    }
```
测试函数：
```java
String message="";
    	String filename ="/sdcard/DevCv/Log/";
	filename += "Log_projectPoints" + ".txt";
	writeFileSdcard(filename,  message);
```

