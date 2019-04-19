# 使用HttpURLConnection实现页面静态化工具类 - 零度的博客专栏 - CSDN博客
2016年04月20日 14:23:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：902
package zmx.util;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
public class GenerateStaticPageUtil {
 protected static String defaultToFile = "D:\\projects\\testWeb\\src\\zmx\\util\\main_Null.html";
 protected static String defaultFromFile = "[http://www.hao123.com/](http://www.hao123.com/)";
 public static String genHtml(String fromFile,String toFile) {
  String result = "";
  if("".equals(fromFile)||fromFile==null){
   fromFile = defaultFromFile;
  }
  if("".equals(toFile)||toFile==null){
   toFile = defaultToFile;
  }
  try{
   URL url = new URL(fromFile);
   HttpURLConnection conn =(HttpURLConnection) url.openConnection();
   conn.connect();
   if (conn.getResponseCode() == 200) {
    InputStream is = (InputStream) conn.getInputStream();
    try{
     FileOutputStream baos = new FileOutputStream(new File(toFile));
     int buffer = 1024;
     byte[] b = new byte[buffer];
     int n = 0;
     while ((n = is.read(b, 0, buffer)) > 0) {
      baos.write(b, 0, n);
     }
     //String s = new String(baos.toByteArray(), WEATHER_HTML_CHARSET);
     is.close();
     baos.close();
     result = "生成成功";
    }catch(Exception e){
     result="写文件过程出错，取消生成。";
    }
   }else{
    result="获得链接过程出错，取消生成。";   
   }
  }catch(Exception e){
    e.printStackTrace();
    result="获得内容过程出错，取消生成。";
  }
  return result;
 }
 //返回字节流
 public static String getHtml(String fromUrl){
  try {
   URL url = new URL(fromUrl);
   HttpURLConnection conn = (HttpURLConnection) url.openConnection();
   if (conn.getResponseCode() == 200) {
    InputStream is = (InputStream) conn.getContent();
    ByteArrayOutputStream baos =new ByteArrayOutputStream();   
    int buffer = 1024;
    byte[] b = new byte[buffer];
    int n = 0;
    while ((n = is.read(b, 0, buffer)) > 0) {
     baos.write(b, 0, n);
    }
    String s = new String(baos.toByteArray(), "UTF-8");
    is.close();
    baos.close();
    return s;
   }
  } catch (Exception e) {
   e.printStackTrace();
   return "";
  } 
  return "";
 }
 public static void main(String[] args) {
  GenerateStaticPageUtil.genHtml("[http://www.hao123.com/](http://www.hao123.com/)", "D:\\projects\\testWeb\\src\\zmx\\util\\main_Null.html");
  System.out.println(GenerateStaticPageUtil.getHtml("[http://www.baidu.com/](http://www.baidu.com/)"));
 }
}
