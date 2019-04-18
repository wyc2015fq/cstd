# dom4j格式化xml文件 - z69183787的专栏 - CSDN博客
2014年01月20日 11:57:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5072
**dom4j格式xml并输出为*.xml文件**
Java代码  ![收藏代码](http://ymgjava.iteye.com/images/icon_star.png)
- publicstaticvoid main(String[] args) {     
-     Document document = DocumentHelper.createDocument();        
-     Element employees=document.addElement("employees");        
-     Element employee=employees.addElement("employee");        
-     Element name= employee.addElement("name");        
-     name.setText("jim");        
-     Element sex=employee.addElement("sex");        
-     sex.setText("male");        
-     Element age=employee.addElement("age");        
-     age.setText("29");        
- try {        
-         OutputFormat format = new OutputFormat();     
-         format.setIndent(true);     
-         format.setNewlines(true);     
-         Writer fileWriter=new FileWriter("F:/test.xml");        
-         XMLWriter xmlWriter=new XMLWriter(fileWriter,format);        
-         xmlWriter.write(document);        
-         xmlWriter.close();                           
-     } catch (IOException e) {        
-         System.out.println(e.getMessage());        
-     }               
- }  
**dom4j格式化xml并格式化输出**
Java代码  ![收藏代码](http://ymgjava.iteye.com/images/icon_star.png)
- publicstatic String formatXml(String str) throws DocumentException, IOException {  
-     SAXReader reader=new SAXReader();  
-     StringReader in=new StringReader(str);  
-     Document doc=reader.read(in);  
-     OutputFormat formater=OutputFormat.createPrettyPrint();  
-     formater.setEncoding("UTF-8");  
-     StringWriter out=new StringWriter();  
-     XMLWriter writer=new XMLWriter(out,formater);  
-     writer.write(doc);  
-     writer.close();  
- return out.toString();  
- }  
- 
