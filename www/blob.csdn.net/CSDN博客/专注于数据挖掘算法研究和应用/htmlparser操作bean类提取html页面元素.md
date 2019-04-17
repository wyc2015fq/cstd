# htmlparser操作bean类提取html页面元素 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月22日 15:04:47[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2827








HtmlParser对Visitor和Filter的方法进行了封装，定义了针对一些常用html元素操作的bean，简化对常用元素的提取操作。包括：FilterBean、HTMLLinkBean、HTMLTextBean、LinkBean、StringBean、BeanyBaby等。这里给出LinKBean和StringBean的源码：

1、LinkBean代码

```java
import java.net.URL;
import java.util.HashSet;
import java.util.Set;

import org.htmlparser.Node;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.beans.LinkBean;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.filters.OrFilter;
import org.htmlparser.tags.ImageTag;
import org.htmlparser.tags.LinkTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

public class UrlFilter {
	//LinkBean类使用  
    public  Set<String> LinkBean(String src,String urlFilters) throws Exception {  
    	Set<String> links = new HashSet<String>();
    	LinkBean linkBean = new LinkBean();  
        linkBean.setURL(src);  
        URL[] urls = linkBean.getLinks();  
        for (int i = 0; i < urls.length; i++) { 
           String url=urls[i].toString();
           String[] UrlFilters=urlFilters.trim().split(",");
           for (String urlFilter : UrlFilters)
		if(url.startsWith(urlFilter)) links.add(url);
        }  
        return links;
    }  
	
	public static void main(String[]args) throws Exception{
		UrlFilter uf=new UrlFilter();
		String strUrlFilters="https://www.rizhiyi.com";
		Set<String> links=uf.LinkBean("https://www.rizhiyi.com/",strUrlFilters);
		for(String link:links)
			System.out.println(link);
	}
}
```



2、StringBean代码



```java
import java.net.URL;
import org.htmlparser.Node;
import org.htmlparser.Parser;
import org.htmlparser.beans.StringBean;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;
import org.htmlparser.util.SimpleNodeIterator;

import sp.SslUtils;

public class UrlParser {
     * 根据提供的URL，获取此URL对应网页的纯文本信息 
     * @param url 提供的URL链接 
     * @return RL对应网页的纯文本信息 
	 * @throws Exception 
     */  
    public String getText(String url)throws Exception{  
		StringBean sb = new StringBean ();
		sb.setLinks (false);
		sb.setReplaceNonBreakingSpaces(true);
		sb.setCollapse(true);
		Parser parser = new Parser (url);
		parser.setEncoding("UTF-8");
		parser.reset ();
		parser.visitAllNodesWith (sb);
		String text = sb.getStrings ();
        return text;  
    }  
    
	public static void main(String[]args) throws Exception{
		UrlParser fd=new UrlParser();
		try {
			String str=fd.getText("https://www.rizhiyi.com/");
			System.out.println(str);
		} catch (ParserException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
```







