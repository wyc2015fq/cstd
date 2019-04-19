# Java使用Jacob转换Word为HTML - 零度的博客专栏 - CSDN博客
2016年04月14日 13:41:16[零度anngle](https://me.csdn.net/zmx729618)阅读数：1323
        JACOB一个Java-COM中间件.通过这个组件你可以在Java应用程序中调用COM组件和Win32 libraries。”Jacob只能用于windows系统，如果你的系统不是windows，建议使用Openoffice.org，这个是跨平台的，虽然我没用，但是应该不麻烦，就是需要先安装Openoffice这个软件，然后使用8100服务。至于Poi，说实话，我真不爱用，那个需要先解析word，然后自己覆写成html，工作量大不说，还得不偿失，因为很难保证转换的html内容的格式与原来word文档格式一致，并且.docx转换也很费劲。利用Jacob来转换Word为html，除了占用CPU性能多一些，好像还不错。
        1、到官网下载Jacob，目前最新版是1.17，地址链接：[http://sourceforge.net/projects/jacob-project/](http://sourceforge.net/projects/jacob-project/)
        2、将压缩包解压后，Jacob.jar添加到Libraries中(先复制到项目目录中，右键单击jar包选择Build Path—>Add to Build Path)；
        3、将Jacob.dll放至当前项目所用到的“jre\bin”下面(比如我的Eclipse正在用的Jre路径是D:\Java\jdk1.7.0_17\jre\bin)。
Ps：我就是按照上面的步骤配置的，一点问题没有，但是有些人可能还会报错，比如：java.lang.UnsatisfiedLinkError: no jacob in java.library.path，这是系统没有加载到jacob.dll，网上解决方法是将Jacob.dll放至“WINDOWS\SYSTEM32”下面
       java代码：JacobUtil.java
```
package zmx.util;
import com.jacob.activeX.ActiveXComponent;
import com.jacob.com.Dispatch;
import com.jacob.com.Variant;
/**
 * JACOB一个Java-COM中间件.通过这个组件你可以在Java应用程序中调用COM组件和Win32 libraries
 * 实现Word转换为Html
 * @author zhangwenchao
 *
 */
public class JacobUtil {
	
	// 8 代表word保存成html
	public static final int WORD_HTML = 8; 
	public static void main(String[] args) {
		String docfile = "C:\\Users\\zhangwenchao\\Desktop\\xxx.docx";
		String htmlfile = "C:\\Users\\zhangwenchao\\Desktop\\xxx.html";
		JacobUtil.wordToHtml(docfile, htmlfile);
	}
	
    /**  
     * WORD转HTML  
     * @param docfile WORD文件全路径  
     * @param htmlfile 转换后HTML存放路径  
     */  
    public static void wordToHtml(String docfile, String htmlfile)   
    {   
    	// 启动word应用程序(Microsoft Office Word 2003)
        ActiveXComponent app = new ActiveXComponent("Word.Application");
        System.out.println("*****正在转换...*****");
        try  
        {	
        	// 设置word应用程序不可见  
            app.setProperty("Visible", new Variant(false));  
            // documents表示word程序的所有文档窗口，（word是多文档应用程序）
            Dispatch docs = app.getProperty("Documents").toDispatch();  
            // 打开要转换的word文件
            Dispatch doc = Dispatch.invoke(   
                    docs,   
                    "Open",   
                    Dispatch.Method,   
                    new Object[] { docfile, new Variant(false), 
                    		new Variant(true) }, new int[1]).toDispatch();   
            // 作为html格式保存到临时文件
            Dispatch.invoke(doc, "SaveAs", Dispatch.Method, new Object[] {   
                    htmlfile, new Variant(WORD_HTML) }, new int[1]);   
            // 关闭word文件
            Dispatch.call(doc, "Close", new Variant(false));   
        }   
        catch (Exception e)   
        {   
            e.printStackTrace();   
        }   
        finally  
        {   
        	//关闭word应用程序
            app.invoke("Quit", new Variant[] {});   
        } 
        System.out.println("*****转换完毕********");
    }
}
```

