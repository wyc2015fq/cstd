# htmlparser解析网页内容代码 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月24日 15:22:53[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2628








```java
/*抽取html网页文本，循环到值节点并判断是否有src链接*/
	public void htmlText(String url) throws Exception
	{
		
		try {
			URL u = new URL(url);
			if("https".equalsIgnoreCase(u.getProtocol())){
				SslUtils.ignoreSsl();
			}
			//生成一个解析器对象，用网页的 url 作为参数
			Parser parser = new Parser(url);
			if(parser.getEncoding().equals("ISO-8859-1")) 
				parser.setEncoding("UTF-8");
			//迭代所有节点, null 表示不使用 NodeFilter
			NodeList list = parser.parse(null);
            //循环值节点并输出
			processNodeList(list);	
			//输出大节点
			/*SimpleNodeIterator iterator = list.elements();
			while (iterator.hasMoreNodes()) {
				Node node = iterator.nextNode();
				String result = node.toHtml();
				System.out.println(result);	
			}*/
        }catch (ParserException e) {e.printStackTrace();}
	}
	private void processNodeList(NodeList list) {
		//迭代开始
		SimpleNodeIterator iterator = list.elements();
		while (iterator.hasMoreNodes()) {
			Node node = iterator.nextNode();	
			//得到该节点的子节点列表
			NodeList childList = node.getChildren();
			//孩子节点为空，说明是值节点
			if (null == childList){
				//得到值节点的值
				String txt = node.toHtml();
				System.out.println(txt);
				if(txt.contains("src="))
				{
					int start = txt.indexOf("src=");
					txt=txt.substring(start);
					int end = txt.indexOf(" ");
					if (end == -1) end = txt.indexOf(">");
					String src = txt.substring(5, end - 1);
					System.out.println(src);
				}			
			}else processNodeList(childList);//孩子节点不为空，继续迭代该孩子节点
		}
	}
```


```java
/*
	 * 在htmlparser中，Node分成三类，都继承AbstractNode：
	 *  RemarkNode：代表Html中的注释
	 *  TagNode：标签节点。
	 *  TextNode：文本节点
	 */
	public void tagGet(String url){
		try {
		    Parser parser = new Parser(url);
		    if(parser.getEncoding().equals("ISO-8859-1")) 
				parser.setEncoding("UTF-8");
		    
		    NodeVisitor visitor = new NodeVisitor( true, true ) {
                public void visitTag(Tag tag) {
                	System.out.println("This is Tag:"+tag.getText());
                }
                public void visitStringNode (Text string)    {
                	System.out.println("This is Text:"+string);
                }
                public void visitRemarkNode (Remark remark) {
                	System.out.println("This is Remark:"+remark.getText());
                }
                public void beginParsing () {
                	System.out.println("beginParsing");
                }
                public void visitEndTag (Tag tag){
                	System.out.println("visitEndTag:"+tag.getText());
                }
                public void finishedParsing () {
                	System.out.println("finishedParsing");
                }
            };
            parser.visitAllNodesWith(visitor);
            
		    /*NodeVisitor visitor = new NodeVisitor() {  
                public void visitTag(Tag tag) { 
                    System.out.println("正在访问的tag：" + tag.getTagName() + "  ||  Class is :"+ tag.getClass());  
                }  
            };  
		    parser.visitAllNodesWith(visitor);
		    */
		    //NodeList parse = parser.parse(null);
		    //System.out.println(parse.toHtml());
		    
		} catch (ParserException e) {
		    e.printStackTrace();
		}
	}
```


```java
/*
	 * 单独获取inputtag和formtag
	 */
	public void formGet(String url) throws ParserException{
		Parser parser = new Parser(url);
		if(parser.getEncoding().equals("ISO-8859-1")) 
			parser.setEncoding("UTF-8");
		NodeFilter inputFilter = new NodeClassFilter(InputTag.class);
		NodeFilter formFilter = new NodeClassFilter(FormTag.class);
		NodeFilter frameFilter = new NodeClassFilter(FrameTag.class);
		OrFilter lastFilter = new OrFilter();
	    lastFilter.setPredicates(new NodeFilter[] { formFilter, inputFilter,frameFilter });
		NodeList  nodeList = parser.parse(lastFilter);
		for(int i = 0; i <= nodeList.size(); i++)
		{
			if(nodeList.elementAt(i) instanceof InputTag)
			{
				InputTag tag = (InputTag) nodeList.elementAt(i);
				System.out.println("Input Info:" + tag.getTagName()+":"
				                   +tag.getText());
			}
			if(nodeList.elementAt(i) instanceof FormTag)
			{
				FormTag tag = (FormTag) nodeList.elementAt(i);
				System.out.println("Form Info: " + tag.getFormName()+":"
		                   +tag.getText());
				NodeList inputnodes=tag.getFormInputs();
				for(int j=0;j<=inputnodes.size();j++){
					InputTag tag1 = (InputTag) inputnodes.elementAt(j);
					System.out.println("Input Info:" + tag1.getTagName()+":"
					                   +tag1.getText());
				}			
			}
			if(nodeList.elementAt(i) instanceof FrameTag)
			{
				FrameTag tag = (FrameTag) nodeList.elementAt(i);
				System.out.println("Frame Info: " + tag.getFrameName()+":"
				                   +tag.getText());
			}
		}
	}
	/*
	 * org.htmlparser Class PrototypicalNodeFactory
	 * A node factory based on the prototype pattern. 
	 * This factory uses the prototype pattern to generate new nodes. 
	 * These are cloned as needed to form new Text, Remark and Tag nodes.
	 */
	public void hyperGet(String url) throws ParserException{
		PrototypicalNodeFactory factory = new PrototypicalNodeFactory ();
		factory.registerTag(new FormTag());
		Parser parser = new Parser (url);
		parser.setNodeFactory (factory);
		NodeFilter filter = new NodeClassFilter(FormTag.class);
		NodeList  nodeList = parser.parse(filter);
		for (Node node : nodeList.toNodeArray()) {  
			if (node instanceof FormTag) { 
				FormTag tag = (FormTag) node;
				System.out.println("Form Info: " + tag.getFormName()+":"
		                   +tag.getText());
			}
		}
	}
```




