# Jquery Jcrop 插件java的使用方法 包括图片保存类 - z69183787的专栏 - CSDN博客
2012年10月27日 11:50:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1594
个人分类：[Javascript-Jquery																[Javascript-Jquery插件](https://blog.csdn.net/z69183787/article/category/2175469)](https://blog.csdn.net/z69183787/article/category/2175111)
1。下载最新的Jcrop文件。
[http://deepliquid.com/content/Jcrop.html](http://deepliquid.com/content/Jcrop.html)
里面有demo文件，可以直接看.但要实现剪切功能还需要类来支持。默认的是PHP的。
2。Utils类
Java代码  ![收藏代码](http://mofan.iteye.com/images/icon_star.png)
- publicclass Utils {  
- 
- publicstatic String getExtension(File f) {   
- return (f != null) ? getExtension(f.getName()) : "";   
-     }   
- 
- publicstatic String getExtension(String filename) {   
- return getExtension(filename, "");   
-     }   
- 
- publicstatic String getExtension(String filename, String defExt) {   
- if ((filename != null) && (filename.length() > 0)) {   
- int i = filename.lastIndexOf('.');   
- 
- if ((i >-1) && (i < (filename.length() - 1))) {   
- return filename.substring(i + 1);   
-             }   
-         }   
- return defExt;   
-     }   
- 
- publicstatic String trimExtension(String filename) {   
- if ((filename != null) && (filename.length() > 0)) {   
- int i = filename.lastIndexOf('.');   
- if ((i >-1) && (i < (filename.length()))) {   
- return filename.substring(0, i);   
-             }   
-         }   
- return filename;   
-     }   
- }  
 这个类主要是用来获取文件的扩展名。
3。SaveImges类
Java代码  ![收藏代码](http://mofan.iteye.com/images/icon_star.png)
- publicclass SaveImage{  
Java代码  ![收藏代码](http://mofan.iteye.com/images/icon_star.png)
- /**  
-   * 保存图片  
-   * @param img       原图路径  
-   * @param dest      目标图路径  
-   * @param top       选择框的左边y坐标  
-   * @param left      选择框的左边x坐标  
-   * @param width     选择框宽度  
-   * @param height    选择框高度  
-   * @return  
-   * @throws IOException  
-   */
- publicstaticboolean saveImage(File img,      
-             String dest,      
- int top,      
- int left,      
- int width,      
- int height) throws IOException {     
-   File fileDest = new File(dest);     
- if(!fileDest.getParentFile().exists())     
-       fileDest.getParentFile().mkdirs();     
-   String ext = Utils.getExtension(dest).toLowerCase();     
-   BufferedImage bi = (BufferedImage)ImageIO.read(img);     
-   height = Math.min(height, bi.getHeight());     
-   width = Math.min(width, bi.getWidth());     
- if(height <= 0) height = bi.getHeight();     
- if(width <= 0) width = bi.getWidth();     
-   top = Math.min(Math.max(0, top), bi.getHeight()-height);     
-   left = Math.min(Math.max(0, left), bi.getWidth()-width);     
- 
-   BufferedImage bi_cropper = bi.getSubimage(left, top, width, height);     
- return ImageIO.write(bi_cropper, ext.equals("png")?"png":"jpeg", fileDest);     
-  }    
- 
- publicstaticvoid main(String[] args) {  
- try {  
-    System.out.println(saveImage(new File("E:\\JavaWork\\pic\\WebRoot\\css\\flowers.jpg"),"E:\\JavaWork\\pic\\WebRoot\\css\\flowers1.jpg",106,87,289,217));  
-   } catch (IOException e) {  
- // TODO Auto-generated catch block
-    e.printStackTrace();  
-   }  
-  }  
- }  
这里的top、left、width、height都可以直接用Jcrop里获取到。 在Jcrop的“Basic Handler”这个demo里面，相应的X1、Y1、W、H这四个参数，用request可以得到这些值。
