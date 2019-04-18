# 验证码图片Struts2 验证码图片实例 - weixin_33985507的博客 - CSDN博客
2013年05月05日 21:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
这几周一直在学习验证码图片之类的问题,上午正好有机会和大家共享一下.
                这周实现一个小功能，那就是登录的时候用使的验证码，上面讲一讲骤步。
                第一步：机随生成验证码
```java
package com.dong.framework.tool;
 
 import java.util.Arrays;
 
 /**
  * 工具类，生成机随验证码字符串
  * @version 1.0 2012/08/21
  * @author dongliyang
  *
  */
 public class SecurityCode {
     
     /**
      * 验证码难度级别，Simple只含包数字，Medium含包数字和小写英文，Hard含包数字和巨细写英文
      */
     public enum SecurityCodeLevel {Simple,Medium,Hard};
     
     /**
      * 发生默许验证码，4位等中难度
      * @return  String 验证码
      */
     public static String getSecurityCode(){
         return getSecurityCode(4,SecurityCodeLevel.Medium,false);
     }
     
     /**
      * 发生长度和难度意任的验证码
      * @param length  长度
      * @param level   难度级别
      * @param isCanRepeat  是不是够能涌现复重的字符，如果为true，则可能涌现 5578这样含包两个5,如果为false，则不可能涌现种这情况
      * @return  String 验证码
      */
     public static String getSecurityCode(int length,SecurityCodeLevel level,boolean isCanRepeat){
         
         //机随取抽len个字符
         int len=length;
         
         //字符合集(除去易淆混的数字0、数字1、字母l、字母o、字母O)
         char[] codes={'1','2','3','4','5','6','7','8','9',
                       'a','b','c','d','e','f','g','h','i',
                       'j','k','m','n','p','q','r','s','t',
                       'u','v','w','x','y','z','A','B','C',
                       'D','E','F','G','H','I','J','K','L',
                       'M','N','P','Q','R','S','T','U','V',
                       'W','X','Y','Z'};
         
         //根据不同的难度截取字符数组
         if(level==SecurityCodeLevel.Simple){
             codes=Arrays.copyOfRange(codes, 0,9);
         }else if(level==SecurityCodeLevel.Medium){
             codes=Arrays.copyOfRange(codes, 0,33);
         }
         
         //字符合集长度
         int n=codes.length;
         
         //抛出运行时异常
         if(len>n&&isCanRepeat==false){
             throw new RuntimeException(
                     String.format("调用SecurityCode.getSecurityCode(%1$s,%2$s,%3$s)涌现异常，" +
                                    "当isCanRepeat为%3$s时，传入参数%1$s不能大于%4$s",
                                    len,level,isCanRepeat,n));
         }
         
         //寄存取抽出来的字符
         char[] result=new char[len];
         
         //断判是不是涌现复重的字符
         if(isCanRepeat){
             for(int i=0;i<result.length;i++){
                 //索引 0 and n-1
                 int r=(int)(Math.random()*n);
             
                 //将result中的第i个元素置设为codes[r]寄存的值数
                 result[i]=codes[r];
             }
         }else{
             for(int i=0;i<result.length;i++){
                 //索引 0 and n-1
                 int r=(int)(Math.random()*n);
                 
                 //将result中的第i个元素置设为codes[r]寄存的值数
                 result[i]=codes[r];
                 
                 //必须确保不会再次取抽到那个字符，因为全部取抽的字符必须不相同。
                 //因此，这里用数组中的最后一个字符改写codes[r]，并将n减1
                 codes[r]=codes[n-1];
                 n--;
             }
         }
         
         return String.valueOf(result);
     }
 }
```
    第二步：生成图片
    每日一道理 
够能破碎的人，必定真正活过。林黛玉的破碎，在于她有刻骨铭心的爱情；三毛的破碎，源于她历经沧桑后一刹那的明彻与超脱；凡高的破碎，是太阳用黄金的刀子让他在光明中不断剧痛，贝多芬的破碎，则是灵性至极的黑白键撞击生命的悲壮乐章。如果说那些平凡者的破碎泄漏的是人性最纯最美的光点，那么这些优秀的灵魂的破碎则如银色的梨花开满了我们头顶的天空。
```java
package com.dong.framework.tool;
 
 import java.awt.Color;
 import java.awt.Font;
 import java.awt.Graphics;
 import java.awt.image.BufferedImage;
 import java.io.ByteArrayInputStream;
 import java.io.ByteArrayOutputStream;
 import java.io.IOException;
 import java.util.Random;
 import com.sun.image.codec.jpeg.ImageFormatException;
 import com.sun.image.codec.jpeg.JPEGCodec;
 import com.sun.image.codec.jpeg.JPEGImageEncoder;
 
 /**
  * 工具类，生成验证码图片
  * @version 1.0 2012/08/21
  * @author dongliyang
  *
  */
 public class SecurityImage {
     
     /**
      * 生成验证码图片
      * @param securityCode   验证码字符
      * @return  BufferedImage  图片
      */
     public static BufferedImage createImage(String securityCode){
         
         //验证码长度
         int codeLength=securityCode.length();
         //字体巨细
         int fSize = 15;
         int fWidth = fSize + 1;
         //图片宽度
         int width = codeLength * fWidth + 6 ;
         //图片度高
         int height = fSize * 2 + 1;
         
         //图片
         BufferedImage image=new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
         Graphics g=image.createGraphics();
         
         //置设景背色
         g.setColor(Color.WHITE);
         //填充景背
         g.fillRect(0, 0, width, height);
         
         //置设边框颜色
         g.setColor(Color.LIGHT_GRAY);
         //边框字体式样
         g.setFont(new Font("Arial", Font.BOLD, height - 2));
         //制绘边框
         g.drawRect(0, 0, width - 1, height -1);
         
         
         //制绘噪点
         Random rand = new Random();
         //置设噪点颜色
         g.setColor(Color.LIGHT_GRAY);
         for(int i = 0;i < codeLength * 6;i++){
             int x = rand.nextInt(width);
             int y = rand.nextInt(height);
             //制绘1*1巨细的矩形
             g.drawRect(x, y, 1, 1);
         }
         
         //制绘验证码
         int codeY = height - 10;  
         //置设字体颜色和式样
         g.setColor(new Color(19,148,246));
         g.setFont(new Font("Georgia", Font.BOLD, fSize));
         for(int i = 0; i < codeLength;i++){
             g.drawString(String.valueOf(securityCode.charAt(i)), i * 16 + 5, codeY);
         }
         //闭关源资
         g.dispose();
         
         return image;
     }
     
     /**
      * 回返验证码图片的流格式
      * @param securityCode  验证码
      * @return ByteArrayInputStream 图片流
      */
     public static ByteArrayInputStream getImageAsInputStream(String securityCode){
         
         BufferedImage image = createImage(securityCode);
         return convertImageToStream(image);
     }
     
     /**
      * 将BufferedImage转换成ByteArrayInputStream
      * @param image  图片
      * @return ByteArrayInputStream 流
      */
     private static ByteArrayInputStream convertImageToStream(BufferedImage image){
         
         ByteArrayInputStream inputStream = null;
         ByteArrayOutputStream bos = new ByteArrayOutputStream();
         JPEGImageEncoder jpeg = JPEGCodec.createJPEGEncoder(bos);
         try {
             jpeg.encode(image);
             byte[] bts = bos.toByteArray();
             inputStream = new ByteArrayInputStream(bts);
         } catch (ImageFormatException e) {
             e.printStackTrace();
         } catch (IOException e) {
             e.printStackTrace();
         }
         return inputStream;
     }
 }
```
    第三步：验证码与Struts 2结合
```java
package com.dong.security.action;
 
 import com.opensymphony.xwork2.ActionSupport;
 import java.io.ByteArrayInputStream;
 import java.util.Map;
 import org.apache.struts2.interceptor.SessionAware;
 import com.dong.framework.tool.SecurityCode;
 import com.dong.framework.tool.SecurityImage;
 
 /**
  * 供给图片验证码
  * @version 1.0 2012/08/22
  * @author dongliyang
  */
 @SuppressWarnings("serial")
 public class SecurityCodeImageAction extends ActionSupport implements SessionAware{
     
     //Struts2中Map类型的session
     private Map<String, Object> session;
     
     //图片流
     private ByteArrayInputStream imageStream;
 
     public ByteArrayInputStream getImageStream() {
         return imageStream;
     }
 
     public void setImageStream(ByteArrayInputStream imageStream) {
         this.imageStream = imageStream;
     }
 
     
     public String execute() throws Exception {
         //如果开启Hard模式，可以不区分巨细写
 //        String securityCode = SecurityCode.getSecurityCode(4,SecurityCodeLevel.Hard, false).toLowerCase();
         
         //取获默许难度和长度的验证码
         String securityCode = SecurityCode.getSecurityCode();
         imageStream = SecurityImage.getImageAsInputStream(securityCode);
         //放入session中
         session.put("SESSION_SECURITY_CODE", securityCode);
         return SUCCESS;
     }
 
     public void setSession(Map<String, Object> session) {
         this.session = session;
     }
 
 }
```
    第四步：配置Struts.xml
```java
<package name="Security" namespace="/Security" extends="struts2">
     <action name="SecurityCodeImageAction" 
     class="com.dong.security.action.SecurityCodeImageAction">
         <result name="success" type="stream">
             <param name="contentType">image/jpeg</param>
             <param name="inputName">imageStream</param>
             <param name="bufferSize">2048</param>
         </result>
     </action>
 </package>
```
    第五步：前台jsp和js
```java
<script type="text/javascript">
			function changeValidateCode(obj) {
				//取获以后的间时作为参数，无体具义意 
				var timenow = new Date().getTime();
				//每次请求须要一个不同的参数，否则可能会回返一样的验证码 
				//这和浏览器的缓存制机有关系，也可以把页面置设为不缓存，这样就用不这个参数了。 
				obj.src = "SecurityCodeImageAction?d=" + timenow;
			}
```
    </script>
```java
<img src="Security/SecurityCodeImageAction" id="Verify"  style="cursor:hand;" alt="看不清，换一张"/>
 <input type="text" name="securityCode"/>
```
    这就是验证码生成的全过程，分享一下！
文章结束给大家分享下程序员的一些笑话语录： 
爱情观
    　　爱情就是死循环，一旦执行就陷进去了。
    　　爱上一个人，就是内存泄露--你永远释放不了。
    　　真正爱上一个人的时候，那就是常量限定，永远不会改变。
    　　女朋友就是私有变量，只有我这个类才能调用。
    　　情人就是指针用的时候一定要注意，要不然就带来巨大的灾难。
