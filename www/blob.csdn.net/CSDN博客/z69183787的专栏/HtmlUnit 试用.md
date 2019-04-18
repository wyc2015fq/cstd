# HtmlUnit 试用 - z69183787的专栏 - CSDN博客
2016年11月17日 17:18:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1629
pom：
```
<dependency>
            <groupId>net.sourceforge.htmlunit</groupId>
            <artifactId>htmlunit</artifactId>
            <version>2.18</version>
        </dependency>
        <dependency>
            <groupId>net.sourceforge.htmlunit</groupId>
            <artifactId>htmlunit-core-js</artifactId>
            <version>2.9</version>
            <scope>compile</scope>
        </dependency>
        <dependency>
            <groupId>xml-apis</groupId>
            <artifactId>xml-apis</artifactId>
            <version>1.4.01</version>
        </dependency>
        <!-- http client -->
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpclient</artifactId>
            <version>4.5</version>
        </dependency>
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpmime</artifactId>
            <version>4.5</version>
        </dependency>
```
```java
package htmlunit;
import com.gargoylesoftware.htmlunit.BrowserVersion;
import com.gargoylesoftware.htmlunit.WebClient;
import com.gargoylesoftware.htmlunit.html.HtmlAnchor;
import com.gargoylesoftware.htmlunit.html.HtmlListItem;
import com.gargoylesoftware.htmlunit.html.HtmlPage;
import java.io.IOException;
import java.util.List;
/**
 * Created by Administrator on 2016/11/17.
 */
public class Main {
    public static void main(String[] args) throws IOException {
        final WebClient webClient=new WebClient(BrowserVersion.CHROME);
        // 启动JS
        webClient.getOptions().setJavaScriptEnabled(true);
        //忽略ssl认证
        //webClient.getOptions().setUseInsecureSSL(true);
        //禁用Css，可避免自动二次请求CSS进行渲染
        //webClient.getOptions().setCssEnabled(false);
        int count = Integer.MAX_VALUE;
        final HtmlPage page=webClient.getPage("http://event.dianping.com/midas/1activities/a4635/index.html");
        count = webClient.waitForBackgroundJavaScript(10000);
//        while(count != 0 ){
//            count = webClient.waitForBackgroundJavaScript(10000);
//            System.out.println(count);
//        }
        //获取所有输出链接
//        for(HtmlAnchor anchor : page.getAnchors()){
//            System.out.println(anchor.getTextContent()+" : "+anchor.getAttribute("href"));
//        }
        final HtmlListItem titles = (HtmlListItem)page.getByXPath("/html/body/div[5]/div[1]/ul/li[2]").get(0);
        System.out.println(titles.getTextContent());
        System.out.println(page.asXml());
        webClient.close();
    }
}
```
