# andorid 查看OpenCv Mat的Debug信息 - wishchinYang的专栏 - CSDN博客
2015年08月03日 20:54:07[wishchin](https://me.csdn.net/wishchin)阅读数：858
个人分类：[人形机器人](https://blog.csdn.net/wishchin/article/category/5597059)
          在进行Android调试时，不能再Console显示Debug信息，只能在LogCat上显示，显示信息如下图：
         代码段：
```java
public void printMat2Txt(Mat ElemM, String message) {  
    	System.out.println( "mat = " + ElemM.dump() );
    }
```
![](https://img-blog.csdn.net/20150803205339285?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
