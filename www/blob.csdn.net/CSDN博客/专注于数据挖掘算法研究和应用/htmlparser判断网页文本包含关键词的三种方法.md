# htmlparser判断网页文本包含关键词的三种方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月18日 09:19:15[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2715








```java
import org.htmlparser.Node;
import org.htmlparser.Parser;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;
import org.htmlparser.util.SimpleNodeIterator;
import org.htmlparser.visitors.TextExtractingVisitor;

public class KeyFilter {
	
	public static String strKeyWords;
	/**判断是否包含关键词
	 */
	public static boolean bKey=false;
	private static void processNodeList(NodeList list) {
		//迭代开始
		SimpleNodeIterator iterator = list.elements();
		String[] KeyWords=strKeyWords.trim().split(",");
		while (iterator.hasMoreNodes()) {
			Node node = iterator.nextNode();
			//得到该节点的子节点列表
			NodeList childList = node.getChildren();
			//孩子节点为空，说明是值节点
			if (null == childList){
				//得到值节点的值
				String result = node.toPlainTextString();
				//若包含关键字，则为搜索的网页
				for (String keyword : KeyWords) 
					if (result.indexOf(keyword) != -1) {
						bKey= true;
						return;
					}
			}else processNodeList(childList);//孩子节点不为空，继续迭代该孩子节点
		}
	}

	public static boolean extractKeyWord(String url) {
		try {
            //生成一个解析器对象，用网页的 url 作为参数
			Parser parser = new Parser(url);
			
            //判断网页文本是否包含关键词方法一，节点循环，性能低
			//迭代所有节点, null 表示不使用 NodeFilter
			//NodeList list = parser.parse(null);
			//processNodeList(list);
			
			//判断网页文本是否包含关键词方法二，运行快速
			TextExtractingVisitor tev=new TextExtractingVisitor();
			parser.visitAllNodesWith(tev);
			String body=tev.getExtractedText();
            String[] KeyWords=strKeyWords.trim().split(",");
            for (String keyword : KeyWords) 
				if (body.indexOf(keyword) != -1) return true;
            
			//判断网页文本是否包含关键词方法三
			/*StringBuffer buftext = new StringBuffer();
			Node[] nodes = list.toNodeArray();//转化为数组
            for(int i=0;i<nodes.length;i++){
            	 String line = nodes[i].toPlainTextString();  
                 if(line != null) 
                     buftext.append(line);  
            }
            String body=buftext.toString();
            String[] KeyWords=strKeyWords.trim().split(",");
            for (String keyword : KeyWords) 
				if (body.indexOf(keyword) != -1) return true;*/
            
		} catch (ParserException e) {
			e.printStackTrace();
		}
		return bKey;
	}
	
	public static void main(String[]args){
		strKeyWords="我们,你们";
	    if(extractKeyWord("https://www.icbc-axa.com"))
	    	System.out.println("网页包含关键词");
	}
}
```




