# Json 与 XML 的互相转换 - z69183787的专栏 - CSDN博客
2013年10月25日 16:23:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3193
```java
net.sf.json.xml.XMLSerializer
private static String json2Xml(JSONObject json,String rootName){
		String sXml="";
		XMLSerializer xmlSerializer = new XMLSerializer();				
		xmlSerializer.setTypeHintsEnabled( false );       
		xmlSerializer.setRootName(rootName); 
		String sContent=xmlSerializer.write(json);
		try {
			Document docCon = DocumentHelper.parseText(sContent);					
			sXml=docCon.getRootElement().asXML();
			//System.out.println(sXml);
		} catch (DocumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return sXml;
	}
	
	public String xml2Json(String xmlString){
		  XMLSerializer xmlSerializer = new XMLSerializer();
		  //xmlSerializer.setRootName("result" );
		  JSON json = xmlSerializer.read(xmlString);		 
		  return json.toString(1);
	}
```
