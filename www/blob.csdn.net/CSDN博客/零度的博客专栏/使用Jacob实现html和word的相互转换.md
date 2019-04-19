# 使用Jacob实现html和word的相互转换 - 零度的博客专栏 - CSDN博客
2016年11月21日 16:19:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：5426
```
package zmx.jacob.test;
import com.jacob.activeX.ActiveXComponent;
import com.jacob.com.Dispatch;
import com.jacob.com.Variant;
public class JacobUtil {
	
	// 8 代表word保存成html
	public static final int WORD_HTML = 8; 
	
	// 1 代表html保存成word
	public static final int HTML_WORD = 1; 
	public static void main(String[] args) {
		String wordFile = "D:\\projects\\testWeb\\src\\zmx\\jacob\\test\\wordFile.doc";
		String htmlFile = "D:\\projects\\testWeb\\src\\zmx\\jacob\\test\\htmlFile.html";
	//	JacobUtil.wordToHtml(wordFile, htmlFile);
		
		
		JacobUtil.htmlToWord(htmlFile, wordFile);
	}
	
    /**  
     * WORD转HTML  
     * @param docfile WORD文件全路径  
     * @param htmlfile 转换后HTML存放路径 
     * notes:需要将jacob.dll拷贝到windows/system32或者项目所在jre\bin目录下面(比如我的Eclipse正在用的Jre路径是D:\Java\jdk1.7.0_17\jre\bin)。
     * @param html html静态页面路径
     * @param wordFile 要生成的word文档路径 
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
            Dispatch doc = Dispatch.invoke(docs, "Open", Dispatch.Method, new Object[] { docfile, new Variant(false), new Variant(true) }, new int[1]).toDispatch();   
            // 作为html格式保存到临时文件
            Dispatch.invoke(doc, "SaveAs", Dispatch.Method, new Object[] { htmlfile, new Variant(WORD_HTML) }, new int[1]);   
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
    
    
    /**
     * JACOB方式
     * notes:需要将jacob.dll拷贝到windows/system32或者项目所在jre\bin目录下面(比如我的Eclipse正在用的Jre路径是D:\Java\jdk1.7.0_17\jre\bin)。
     * @param html html静态页面路径
     * @param wordFile 要生成的word文档路径
     */
    public static void htmlToWord(String htmlFile, String wordFile) {    
    	
            ActiveXComponent app = new ActiveXComponent("Word.Application"); // 启动word
            System.out.println("*****正在转换...*****");
            try {
                app.setProperty("Visible", new Variant(false));
                Dispatch wordDoc = app.getProperty("Documents").toDispatch();
                wordDoc = Dispatch.invoke(wordDoc, "Add", Dispatch.Method, new Object[0], new int[1]).toDispatch();
                Dispatch.invoke(app.getProperty("Selection").toDispatch(), "InsertFile", Dispatch.Method, new Object[] { htmlFile, "", new Variant(false), new Variant(false), new Variant(false) }, new int[3]);
                Dispatch.invoke(wordDoc, "SaveAs", Dispatch.Method, new Object[] {wordFile, new Variant(HTML_WORD)}, new int[1]);
                Dispatch.call(wordDoc, "Close", new Variant(false));
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                app.invoke("Quit", new Variant[] {});
            }
            System.out.println("*****转换完毕********");
    }
}
```
