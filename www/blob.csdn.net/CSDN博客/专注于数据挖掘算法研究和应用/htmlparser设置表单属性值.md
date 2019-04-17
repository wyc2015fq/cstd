# htmlparser设置表单属性值 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月25日 09:54:02[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2455








想用htmlparser实现自动填写表单并提交，但暂时实现不了提交，计划通过httpclient实现。htmlparser可实现设置表单属性值，但貌似意义不大，其主要价值在解析html网页。设置表单属性值的代码如下：



```java
package an;

import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.tags.InputTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;


public class PostForm {
	
	/*
	 * 自动填单
	 */
	public String hcPost(String url) throws ParserException {
		
		Parser parser = new Parser(url);   
		parser.setEncoding(parser.getEncoding());
		//提取节点
		NodeFilter inputFilter = new NodeClassFilter(InputTag.class); 
		NodeFilter attrFilter = new HasAttributeFilter("name","wd");   
	    NodeFilter andFilter = new AndFilter(inputFilter,attrFilter);  
	    NodeList nodeList = parser.parse(andFilter);   
	    
	    //设置节点
	    for (int i = 0; i < nodeList.size(); i++){
	    	InputTag inputTag = (InputTag)nodeList.elementAt(i);
	    	if(inputTag.getAttribute("name").equals("wd")){
	    		inputTag.setAttribute("value","百度");
	    		System.out.println(inputTag.toHtml());
	    	} 
	    }
        return "yes";
	}
	
	public static void main(String[]args) throws Exception{
		PostForm pf=new PostForm();
		String cont=pf.hcPost("http://www.baidu.com");
		//打印输出结果内容  
        System.out.println(cont);  
  
	}
}
```
需提前知道百度网页的属性名（IE浏览器F12可查看html页面元素），执行结果如下：<input id="kw" name="wd" class="s_ipt" value="百度" maxlength="255" autocomplete="off">




