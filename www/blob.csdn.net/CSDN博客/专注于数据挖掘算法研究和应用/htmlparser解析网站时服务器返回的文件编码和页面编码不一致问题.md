# htmlparser解析网站时服务器返回的文件编码和页面编码不一致问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月29日 10:34:54[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3558








1、用htmlparser解析http://gz.fang.com/抛出如下异常：

Exception in thread "main" org.htmlparser.util.EncodingChangeException: character mismatch (new: 【 [0x3010] != old:  [0xa1?]) for encoding change from ISO-8859-1 to GB2312 at character offset 149
at org.htmlparser.lexer.InputStreamSource.setEncoding(InputStreamSource.java:279)
at org.htmlparser.lexer.Page.setEncoding(Page.java:864)
at org.htmlparser.tags.MetaTag.doSemanticAction(MetaTag.java:149)
at org.htmlparser.scanners.TagScanner.scan(TagScanner.java:68)
at org.htmlparser.scanners.CompositeTagScanner.scan(CompositeTagScanner.java:159)
at org.htmlparser.util.IteratorImpl.nextNode(IteratorImpl.java:91)
at org.htmlparser.Parser.parse(Parser.java:700)





发现：在Parser parser = new Parser(src);parser.getEncoding();返回网站的编码是 ISO-8859-1 ，但NodeList nodeList = parser.parse(null)解析http://gz.fang.com/页面时，页面设置的字符串<metahttp-equiv="Content-Type"content="text/html;
 charset=gb2312" />是gb2312，二者冲突异常；




2、经查阅，可能是htmlparser的一个bug，通过修改Page类和InputStreamSource类源码可解决，参考http://www.codeweblog.com/htmlparser-coding-problem/，该方法未试验；




3、有人提出通过显示设置字符集来解决，parser.setEncoding("GB2312");但仍无法解决；




该问题后续继续研究如何解决，本文这里暂时先用httpurlclient输出网页文件，再用htmlparser解析网页内容，具体代码如下：



```java
public static void main(String[]args) throws Exception{
		UrlFilter uf=new UrlFilter();
		uf.httpForCoding("http://gz.fang.com/");
	}
	
	//先用httpurlclient输出网页文件，再用htmlparser解析，解决htmlparser编码不一致问题
	public void httpForCoding(String src) throws ParserException {    
        URL u = null;    
        HttpURLConnection con = null;        
        // 尝试发送请求    
        try {    
            u = new URL(src);    
            con = (HttpURLConnection) u.openConnection();    
            con.setConnectTimeout(5*1000);
            con.setRequestMethod("GET");  
            StringBuffer buffer = new StringBuffer();    
            try {    
                //一定要有返回值，否则无法把请求发送给server端。    
                BufferedReader br = new BufferedReader(new InputStreamReader(con.getInputStream(), "UTF-8"));    
                String temp;    
                while ((temp = br.readLine()) != null) {    
                    buffer.append(temp);    
                    buffer.append("\n");    
                }   
            } catch (Exception e) {  e.printStackTrace(); }
          //用parser解析输出内容           
            Parser parser = new Parser(); 
            parser.setInputHTML(buffer.toString());    
            NodeFilter linkFilter = new NodeClassFilter(LinkTag.class);  
            NodeList nodeList = parser.parse(linkFilter);
            for(int i = 0; i <= nodeList.size(); i++){  
            	if(nodeList.elementAt(i) instanceof LinkTag){  
            		LinkTag tag = (LinkTag) nodeList.elementAt(i);  
            		String linkUrl = tag.getLink();// url				
    				System.out.println(linkUrl); 
                }
            }
        } catch (Exception e) {   e.printStackTrace();} 
        finally {    
            if (con != null) {    
                con.disconnect();    
            }    
        }        
    }
```





