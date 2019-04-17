# htmlparser新建tag类(以iframe标签为例） - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月24日 17:32:33[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2437








在用htmlparser解析网页内容时，发现iframe没有tag类，于是参照源码自己增加并注册使用。

1、创建iFrameTag类：



```java
// HTMLParser Library - A java-based parser for HTML
// http://htmlparser.org
// Copyright (C) 2006 Somik Raha
//
// Revision Control Information
//
// $URL: https://svn.sourceforge.net/svnroot/htmlparser/trunk/parser/src/main/java/org/htmlparser/tags/FrameTag.java $
// $Author: derrickoswald $
// $Date: 2006-09-16 10:44:17 -0400 (Sat, 16 Sep 2006) $
// $Revision: 4 $
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Common Public License; either
// version 1.0 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// Common Public License for more details.
//
// You should have received a copy of the Common Public License
// along with this library; if not, the license is available from
// the Open Source Initiative (OSI) website:
//   http://opensource.org/licenses/cpl1.0.php

package an;

import org.htmlparser.nodes.TagNode;

/**
 * Identifies a frame tag
 */
public class iFrameTag
    extends
        TagNode
{
    /**
     * The set of names handled by this tag.
     */
    private static final String[] mIds = new String[] {"iframe"};
    
    private static final String mEndTagEnders[] = {     
            "iframe"     
        }; 

    /**
     * Create a new frame tag.
     */
    public iFrameTag ()
    {
    }

    /**
     * Return the set of names handled by this tag.
     * @return The names to be matched that create tags of this type.
     */
    public String[] getIds ()
    {
        return (mIds);
    }

    public String[] getEndTagEnders()     
    {     
        return mEndTagEnders;     
    } 
    /**
     * Returns the location of the frame.
     * @return The contents of the SRC attribute converted to an absolute URL.
     */
    public String getFrameLocation ()
    {
        String ret;
        
        ret = getAttribute ("SRC");
        if (null == ret)
            ret = "";
        else if (null != getPage ())
            ret = getPage ().getAbsoluteURL (ret);
        
        return (ret);
    }

    /**
     * Sets the location of the frame.
     * @param url The new frame location.
     */
    public void setFrameLocation (String url)
    {
        setAttribute ("SRC", url);
    }

    /**
     * Get the <code>NAME</code> attribute, if any.
     * @return The value of the <code>NAME</code> attribute,
     * or <code>null</code> if the attribute doesn't exist.
     */
    public String getFrameName()
    {
        return (getAttribute ("NAME"));
    }

    /**
     * Return a string representation of the contents of this <code>FRAME</code> tag suitable for debugging.
     * @return A string with this tag's contents.
     */
    public String toString()
    {
        return "FRAME TAG : Frame " +getFrameName() + " at "+getFrameLocation()+"; begins at : "+getStartPosition ()+"; ends at : "+getEndPosition ();
    }
}
```
2、注册并使用



```java
package an;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.PrototypicalNodeFactory;
import org.htmlparser.beans.StringBean;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.filters.OrFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.FrameSetTag;
import org.htmlparser.tags.FrameTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.util.NodeList;
```




```java
/*提取网页html源码中form\frame\iframe\input\frameset，查看表单参数
     * iframe要自定义tag类,iFrameTag.java
     * */
    public List<String> getHtml(String url) throws Exception{
    	List<String> htmlList=new ArrayList<String>();//初始化
    	//生成一个解析器对象，用网页的 url 作为参数  
    	Parser parser = new Parser(url);
    	if(parser.getEncoding().equals("ISO-8859-1"))
    		parser.setEncoding("UTF-8"); 
    	//设置节点过滤
    	NodeFilter formFilter = new NodeClassFilter(FormTag.class);  
        NodeFilter frameFilter = new NodeClassFilter(FrameTag.class);  
        NodeFilter inputFilter = new NodeClassFilter(InputTag.class);
        NodeFilter framesetFilter = new NodeClassFilter(FrameSetTag.class);
        //注册iframetag
        PrototypicalNodeFactory p=new PrototypicalNodeFactory();
        p.registerTag(new iFrameTag()); 
        parser.setNodeFactory(p);
        NodeFilter iframeFilter = new NodeClassFilter(iFrameTag.class);
        //添加过滤条件
        OrFilter lastFilter = new OrFilter();  
        lastFilter.setPredicates(new NodeFilter[] { formFilter,frameFilter,inputFilter,framesetFilter ,iframeFilter});  
        NodeList  nodeList = parser.parse(lastFilter);  
        for(int i = 0; i <= nodeList.size(); i++)  {  
        	if(nodeList.elementAt(i) instanceof FormTag)  {  
                FormTag tag = (FormTag) nodeList.elementAt(i);  
                htmlList.add(tag.getText());  
            }  
        	if(nodeList.elementAt(i) instanceof InputTag) {  
                InputTag tag = (InputTag) nodeList.elementAt(i);  
                htmlList.add(tag.getText()); 
            }
            if(nodeList.elementAt(i) instanceof FrameTag){  
                FrameTag tag = (FrameTag) nodeList.elementAt(i);  
                htmlList.add(tag.getText()); 
            }  
            if(nodeList.elementAt(i) instanceof FrameSetTag){  
            	FrameSetTag tag = (FrameSetTag) nodeList.elementAt(i);  
                htmlList.add(tag.getText());
            }
            if(nodeList.elementAt(i) instanceof iFrameTag){  
            	iFrameTag tag = (iFrameTag) nodeList.elementAt(i);  
                htmlList.add(tag.getText());
            }
        }  
        return htmlList;
    }
```

```java
public static void main(String[]args) throws Exception{
		UrlParser fd=new UrlParser();	
		List<String> list=fd.getHtml("http://mail.189.cn");
		for(int i=0;i<list.size();i++){
			System.out.println(list.get(i));
		}
	}
```









