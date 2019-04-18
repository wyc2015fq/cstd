# struts2 上传文件并接受的2种方法 - z69183787的专栏 - CSDN博客
2012年11月13日 09:34:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1684
1：使用MultiPartRequestWrapper
```java
MultiPartRequestWrapper multi = (MultiPartRequestWrapper) servletRequest;
```
获取request对象并转换为MultiPartRequestWrapper
获取上传文件及文件名，filegroupName为jsp中file元素的name属性：
```java
multi.getFiles(fileGroupName)
```
返回值为File[]
```java
multi.getFileNames(fileGroupName)
```
返回值为String[]
2：action自动注入
action中添加属性以及get/set方法
```java
private List<File> fileupload;//这里的"fileName"一定要与表单中的文件域名相同  
    private List<String> fileuploadContentType;//格式同上"fileName"+ContentType  
    private List<String> fileuploadFileName;//格式同上"fileName"+FileName
```
使用get方法即可获取。
