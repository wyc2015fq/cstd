# htmlparser抽取网页内相关节点链接的代码 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月18日 09:21:54[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2203
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









```java
import java.util.HashSet;
import java.util.Set;

import org.htmlparser.Node;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.filters.OrFilter;
import org.htmlparser.tags.ImageTag;
import org.htmlparser.tags.LinkTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

public class UrlFilter {
	
	public static String strUrlFilters;
	// 获取一个网站上的链接,filter 用来过滤链接
	public static Set<String> extracLinks(String url) {
		Set<String> links = new HashSet<String>();
		try {
			Parser parser = new Parser(url);
			
			// 过滤 <frame >标签的 filter，用来提取 frame 标签里的 src 属性所表示的链接
			NodeFilter frameFilter = new NodeFilter() {
				public boolean accept(Node node) {
					if (node.getText().startsWith("frame src=")) 
						return true;
					else return false;
				}
			};			
			//OrFilter 来设置过滤 <a> 标签，<img> 标签和 <frame> 标签，三个标签是 or 的关系
			//OrFilter rorFilter = new OrFilter(new NodeClassFilter(LinkTag.class), new NodeClassFilter(ImageTag.class));
			//OrFilter linkFilter = new OrFilter(rorFilter, frameFilter);
			// OrFilter 来设置过滤 <a> 标签，和 <frame> 标签
			OrFilter linkFilter = new OrFilter(new NodeClassFilter(LinkTag.class), frameFilter);
			
			// 得到所有经过过滤的标签
			NodeList list = parser.extractAllNodesThatMatch(linkFilter);
			String[] UrlFilters=strUrlFilters.trim().split(",");
			for (int i = 0; i < list.size(); i++) {
				Node tag = list.elementAt(i);
				if (tag instanceof LinkTag){// <a> 标签
					LinkTag link = (LinkTag) tag;
					String linkUrl = link.getLink();// url				
					for (String urlFilter : UrlFilters)
						if(linkUrl.startsWith(urlFilter)) links.add(linkUrl);
				}
				/*else if (tag instanceof ImageTag){//<img> 标签
					ImageTag image = (ImageTag) list.elementAt(i);
					String imageUrl=image.getImageURL();
					for (String urlFilter : UrlFilters)
						if(imageUrl.startsWith(urlFilter)) links.add(imageUrl);
				} */
				else { //<frame> 标签
					// 提取 frame 里 src 属性的链接如 <frame src="test.html"/>
					String frame = tag.getText();
					int start = frame.indexOf("src=");
					frame = frame.substring(start);
					int end = frame.indexOf(" ");
					if (end == -1) end = frame.indexOf(">");
					String frameUrl = frame.substring(5, end - 1);
					for (String urlFilter : UrlFilters) 
							if(frameUrl.startsWith(urlFilter)) links.add(frameUrl);
				}
			}
		} catch (ParserException e) {e.printStackTrace();}
		return links;
	}
	
	public static void main(String[]args){
		strUrlFilters="http,https";
		Set<String> links=extracLinks("https://e.alipay.com/index.htm");
		for(String link:links)
			System.out.println(link);
	}
}
```




