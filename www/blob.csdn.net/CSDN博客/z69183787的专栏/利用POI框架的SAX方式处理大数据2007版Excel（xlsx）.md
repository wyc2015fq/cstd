# 利用POI框架的SAX方式处理大数据2007版Excel（xlsx） - z69183787的专栏 - CSDN博客
2017年05月23日 16:47:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1038
[http://m.blog.csdn.net/article/details?id=50747122](http://m.blog.csdn.net/article/details?id=50747122)
1.开发环境：jdk7,poi3.13
如图：
![](https://img-blog.csdn.net/20160226084245386)
红色标注的jar包需要自己去下，这个是利用SAX机理处理xml的接口，而POI实现了它（我记得是这样，仅供参考）
2. 背景需求：
              前台上传一个xlsx格式的Excel（超过10w条，每条20列），保存到服务器，然后在页面上分页显示出来，将不符合规则的行表红色或黄色，点击处理按钮，将所有   数据插入数据库。
3. 综合考虑：
              使用dom方式的XSSFWorkbook是不行的，我实际测试过，每20列，不到1万条就已经内存溢出了，果断采用POI的SAX方式，上面的模块我是利用
springmvc4 + jQuery EasyUI 1.4.1 +hibernate4+mysql5+jdk7,我不可能都贴出来，为便于以后使用，将POI的SAX方式解析这个关键点记录下来。
需求简化：将保存在服务器的xlsx格式的Excel（包含大量数据）解析出来，便于其他使用，（这里我要强调一点，既然是大量数据，我们并不是将所有的数据
一次性都读取出来，放在内存中，而是分页读取，这里分页的概念和客户端分页是一个意思，只不过不是现实在客户端，而是放在内存，做其他用途。。。比如分页插入DB，分页校验）
4.代码：
4.1. 先贴上POI官网的案例，我也是从这里开始的。。。
http://poi.apache.org/spreadsheet/how-to.html#xssf_sax_api
我贴一份代码，备份下：
```java
/* ====================================================================
   Licensed to the Apache Software Foundation (ASF) under one or more
   contributor license agreements.  See the NOTICE file distributed with
   this work for additional information regarding copyright ownership.
   The ASF licenses this file to You under the Apache License, Version 2.0
   (the "License"); you may not use this file except in compliance with
   the License.  You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
==================================================================== */
package org.apache.poi.xssf.eventusermodel.examples;
import java.io.InputStream;
import java.util.Iterator;
import org.apache.poi.xssf.eventusermodel.XLSX2CSV;
import org.apache.poi.xssf.eventusermodel.XSSFReader;
import org.apache.poi.xssf.model.SharedStringsTable;
import org.apache.poi.xssf.usermodel.XSSFRichTextString;
import org.apache.poi.openxml4j.opc.OPCPackage;
import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.helpers.XMLReaderFactory;
/**
 * XSSF and SAX (Event API) basic example.
 * See {@link XLSX2CSV} for a fuller example of doing
 *  XSLX processing with the XSSF Event code.
 */
public class FromHowTo {
	public void processFirstSheet(String filename) throws Exception {
		OPCPackage pkg = OPCPackage.open(filename);
		XSSFReader r = new XSSFReader( pkg );
		SharedStringsTable sst = r.getSharedStringsTable();
		XMLReader parser = fetchSheetParser(sst);
		// To look up the Sheet Name / Sheet Order / rID,
		//  you need to process the core Workbook stream.
		// Normally it's of the form rId# or rSheet#
		InputStream sheet2 = r.getSheet("rId2");
		InputSource sheetSource = new InputSource(sheet2);
		parser.parse(sheetSource);
		sheet2.close();
	}
	public void processAllSheets(String filename) throws Exception {
		OPCPackage pkg = OPCPackage.open(filename);
		XSSFReader r = new XSSFReader( pkg );
		SharedStringsTable sst = r.getSharedStringsTable();
		
		XMLReader parser = fetchSheetParser(sst);
		Iterator<InputStream> sheets = r.getSheetsData();
		while(sheets.hasNext()) {
			System.out.println("Processing new sheet:\n");
			InputStream sheet = sheets.next();
			InputSource sheetSource = new InputSource(sheet);
			parser.parse(sheetSource);
			sheet.close();
			System.out.println("");
		}
	}
	public XMLReader fetchSheetParser(SharedStringsTable sst) throws SAXException {
		XMLReader parser =
			XMLReaderFactory.createXMLReader(
					"org.apache.xerces.parsers.SAXParser"
			);
		ContentHandler handler = new SheetHandler(sst);
		parser.setContentHandler(handler);
		return parser;
	}
	/** 
	 * See org.xml.sax.helpers.DefaultHandler javadocs 
	 */
	private static class SheetHandler extends DefaultHandler {
		private SharedStringsTable sst;
		private String lastContents;
		private boolean nextIsString;
		
		private SheetHandler(SharedStringsTable sst) {
			this.sst = sst;
		}
		
		public void startElement(String uri, String localName, String name,
				Attributes attributes) throws SAXException {
			// c => cell
			if(name.equals("c")) {
				// Print the cell reference
				System.out.print(attributes.getValue("r") + " - ");
				// Figure out if the value is an index in the SST
				String cellType = attributes.getValue("t");
				if(cellType != null && cellType.equals("s")) {
					nextIsString = true;
				} else {
					nextIsString = false;
				}
			}
			// Clear contents cache
			lastContents = "";
		}
		
		public void endElement(String uri, String localName, String name)
				throws SAXException {
			// Process the last contents as required.
			// Do now, as characters() may be called more than once
			if(nextIsString) {
				int idx = Integer.parseInt(lastContents);
				lastContents = new XSSFRichTextString(sst.getEntryAt(idx)).toString();
            nextIsString = false;
			}
			// v => contents of a cell
			// Output after we've seen the string contents
			if(name.equals("v")) {
				System.out.println(lastContents);
			}
		}
		public void characters(char[] ch, int start, int length)
				throws SAXException {
			lastContents += new String(ch, start, length);
		}
	}
	
	public static void main(String[] args) throws Exception {
		FromHowTo howto = new FromHowTo();
		howto.processFirstSheet(args[0]);
		howto.processAllSheets(args[0]);
	}
}
```
-----------------------------------------------------------------------------------------懒惰的分割线---------------------------------------------------------------------------------------------------------------------
我写了2个模式，
        第一种模式会将空白单元格过滤掉，这也是在官网案例稍加改动就ok的，但是发现个缺点，就是利用SAX方式解析会自动忽略空白单元格，而跳过它去存储下一个真正有值的单元格，比如三个同一行的单元格A1 B1 C1 D1的值分别为【123】【空白单元格】【空白单元格】【qwe】，存储为{123,qwe}，反正我的需求是不允许这样的，于是我有些了第二个模式
        第二种模式是会将“空白单元格”的数据存为Null。比如三个同一行的单元格A1 B1 C1 D1的值分别为【123】【空白单元格】【空白单元格】【qwe】，
存储为{123,null,null,qwe}，
        注意：这里说的空白单元格仅仅指的是没有进行任何编辑的单元格，值为一个或多个空格不是空白单元格
4.2 模式一代码：
```java
package office;
/* ====================================================================
   Licensed to the Apache Software Foundation (ASF) under one or more
   contributor license agreements.  See the NOTICE file distributed with
   this work for additional information regarding copyright ownership.
   The ASF licenses this file to You under the Apache License, Version 2.0
   (the "License"); you may not use this file except in compliance with
   the License.  You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
==================================================================== */
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;
import org.apache.commons.lang.StringUtils;
import org.apache.poi.openxml4j.opc.OPCPackage;
import org.apache.poi.xssf.eventusermodel.XSSFReader;
import org.apache.poi.xssf.model.SharedStringsTable;
import org.apache.poi.xssf.usermodel.XSSFRichTextString;
import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.helpers.XMLReaderFactory;
/**
 * XSSF and SAX (Event API) basic example.
 * See {@link XLSX2CSV} for a fuller example of doing
 *  XSLX processing with the XSSF Event code.
 */
public class MyExcel2007ForPaging {
	
		public List<List<String>> dataList = new ArrayList<List<String>>();
		public final int startRow;
		public final int endRow;
		private int currentRow = 0;
		private final String filename;
		private List<String> rowData;
		
		public MyExcel2007ForPaging(String filename,int startRow,int endRow) throws Exception{
			if(StringUtils.isBlank(filename)) throw new Exception("文件名不能空");
			this.filename = filename;
			this.startRow = startRow;
			this.endRow = endRow;
			processFirstSheet();
		}
		/**
		 * 指定获取第一个sheet
		 * @param filename
		 * @throws Exception
		 */
		private void processFirstSheet() throws Exception {
			OPCPackage pkg = OPCPackage.open(filename);
			XSSFReader r = new XSSFReader( pkg );
			SharedStringsTable sst = r.getSharedStringsTable();
			XMLReader parser = fetchSheetParser(sst);
			// To look up the Sheet Name / Sheet Order / rID,
			//  you need to process the core Workbook stream.
			// Normally it's of the form rId# or rSheet#
			InputStream sheet1 = r.getSheet("rId1");
			InputSource sheetSource = new InputSource(sheet1);
			parser.parse(sheetSource);
			sheet1.close();
		}
		
		private XMLReader fetchSheetParser(SharedStringsTable sst) throws SAXException {
			XMLReader parser =
				XMLReaderFactory.createXMLReader(
						"org.apache.xerces.parsers.SAXParser"
				);
			ContentHandler handler = new PagingHandler(sst);
			parser.setContentHandler(handler);
			return parser;
		}
		/** 
		 * See org.xml.sax.helpers.DefaultHandler javadocs 
		 */
		private  class PagingHandler extends DefaultHandler {
			private SharedStringsTable sst;
			private String lastContents;
			private boolean nextIsString;
			
			private PagingHandler(SharedStringsTable sst) {
				this.sst = sst;
			}
			/**
			 * 每个单元格开始时的处理
			 */
			@Override
			public void startElement(String uri, String localName, String name,
					Attributes attributes) throws SAXException {
				// c => cell
				if(name.equals("c")) {
					// Print the cell reference
//					System.out.print(attributes.getValue("r") + " - ");
					
					String index = attributes.getValue("r");
					//这是一个新行
					if(Pattern.compile("^A[0-9]+$").matcher(index).find()){
						
						//存储上一行数据
						if(rowData!=null&&isAccess()&&!rowData.isEmpty()){
							dataList.add(rowData);
						}
						rowData = new ArrayList<String>();;//新行要先清除上一行的数据
						currentRow++;//当前行+1
						System.out.println(currentRow);
					}
					if(isAccess()){
						// Figure out if the value is an index in the SST
						String cellType = attributes.getValue("t");
						if(cellType != null && cellType.equals("s")) {
							nextIsString = true;
						} else {
							nextIsString = false;
						}
					}
				
				}
				// Clear contents cache
				lastContents = "";
			}
			/**
			 * 每个单元格结束时的处理
			 */
			@Override
			public void endElement(String uri, String localName, String name)
					throws SAXException {
				if(isAccess()){
					// Process the last contents as required.
					// Do now, as characters() may be called more than once
					if(nextIsString) {
						int idx = Integer.parseInt(lastContents);
						lastContents = new XSSFRichTextString(sst.getEntryAt(idx)).toString();
						nextIsString = false;
					}
					// v => contents of a cell
					// Output after we've seen the string contents
					if(name.equals("v")) {
//						System.out.println(lastContents);
						rowData.add(lastContents);
						
					}
				}
				
			}
			@Override
			public void characters(char[] ch, int start, int length)
					throws SAXException {
				if(isAccess()){
					lastContents += new String(ch, start, length);
				}
				
			}
			/**
			 * 如果文档结束后，发现读取的末尾行正处在当前行中，存储下这行
			 * （存在这样一种情况，当待读取的末尾行正好是文档最后一行时，最后一行无法存到集合中，
			 * 因为最后一行没有下一行了，所以不为启动starElement()方法，
			 * 当然我们可以通过指定最大列来处理，但不想那么做，扩展性不好）
			 */
			@Override
			public void endDocument ()throws SAXException{
				if(rowData!=null&&isAccess()&&!rowData.isEmpty()){
					dataList.add(rowData);
					System.out.println("--end");
				}
				  
			}
		
		}
		private boolean isAccess(){
			if(currentRow>=startRow&¤tRow<=endRow){
				return true;
			}
			return false;
		}
		public static void main(String[] args) throws Exception {
			MyExcel2007ForPaging reader = new MyExcel2007ForPaging("E:/weld_small.xlsx",15,100);
			System.out.println("\n---"+reader.dataList);
			
		}
}
```
4.3 模式二代码：
```java
package office;
/* ====================================================================
   Licensed to the Apache Software Foundation (ASF) under one or more
   contributor license agreements.  See the NOTICE file distributed with
   this work for additional information regarding copyright ownership.
   The ASF licenses this file to You under the Apache License, Version 2.0
   (the "License"); you may not use this file except in compliance with
   the License.  You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
==================================================================== */
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;
import org.apache.commons.lang.StringUtils;
import org.apache.poi.openxml4j.opc.OPCPackage;
import org.apache.poi.xssf.eventusermodel.XSSFReader;
import org.apache.poi.xssf.model.SharedStringsTable;
import org.apache.poi.xssf.usermodel.XSSFRichTextString;
import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.helpers.XMLReaderFactory;
/**
 * XSSF and SAX (Event API) basic example.
 * See {@link XLSX2CSV} for a fuller example of doing
 *  XSLX processing with the XSSF Event code.
 */
public class MyExcel2007ForPaging_high {
	
		public List<List<IndexValue>> dataList = new ArrayList<List<IndexValue>>();
		private final int startRow;
		private final int endRow;
		private int currentRow = 0;
		private final String filename;
		private List<IndexValue> rowData;
		
		public MyExcel2007ForPaging_high(String filename,int startRow,int endRow) throws Exception{
			if(StringUtils.isBlank(filename)) throw new Exception("文件名不能空");
			this.filename = filename;
			this.startRow = startRow;
			this.endRow = endRow;
			processFirstSheet();
		}
		/**
		 * 指定获取第一个sheet
		 * @param filename
		 * @throws Exception
		 */
		private void processFirstSheet() throws Exception {
			OPCPackage pkg = OPCPackage.open(filename);
			XSSFReader r = new XSSFReader( pkg );
			SharedStringsTable sst = r.getSharedStringsTable();
			XMLReader parser = fetchSheetParser(sst);
			// To look up the Sheet Name / Sheet Order / rID,
			//  you need to process the core Workbook stream.
			// Normally it's of the form rId# or rSheet#
			InputStream sheet1 = r.getSheet("rId1");
			InputSource sheetSource = new InputSource(sheet1);
			parser.parse(sheetSource);
			sheet1.close();
		}
		
		private XMLReader fetchSheetParser(SharedStringsTable sst) throws SAXException {
			XMLReader parser =
				XMLReaderFactory.createXMLReader(
						"org.apache.xerces.parsers.SAXParser"
				);
			ContentHandler handler = new PagingHandler(sst);
			parser.setContentHandler(handler);
			return parser;
		}
		/** 
		 * See org.xml.sax.helpers.DefaultHandler javadocs 
		 */
		private  class PagingHandler extends DefaultHandler {
			private SharedStringsTable sst;
			private String lastContents;
			private boolean nextIsString;
			private String index = null;
			
			private PagingHandler(SharedStringsTable sst) {
				this.sst = sst;
			}
			/**
			 * 每个单元格开始时的处理
			 */
			@Override
			public void startElement(String uri, String localName, String name,
					Attributes attributes) throws SAXException {
				// c => cell
				if(name.equals("c")) {
					// Print the cell reference
//					System.out.print(attributes.getValue("r") + " - ");
					
					index = attributes.getValue("r");
					System.out.println(index);
					if(index.contains("N")){
						System.out.println("##"+attributes+"##");
					}
					
					//这是一个新行
					if(Pattern.compile("^A[0-9]+$").matcher(index).find()){
						
						//存储上一行数据
						if(rowData!=null&&isAccess()&&!rowData.isEmpty()){
							dataList.add(rowData);
						}
						rowData = new ArrayList<IndexValue>();;//新行要先清除上一行的数据
						currentRow++;//当前行+1
//						System.out.println(currentRow);
					}
					if(isAccess()){
						// Figure out if the value is an index in the SST
						String cellType = attributes.getValue("t");
						if(cellType != null && cellType.equals("s")) {
							nextIsString = true;
						} else {
							nextIsString = false;
						}
					}
				
				}
				// Clear contents cache
				lastContents = "";
			}
			/**
			 * 每个单元格结束时的处理
			 */
			@Override
			public void endElement(String uri, String localName, String name)
					throws SAXException {
				if(isAccess()){
					// Process the last contents as required.
					// Do now, as characters() may be called more than once
					if(nextIsString) {
						int idx = Integer.parseInt(lastContents);
						lastContents = new XSSFRichTextString(sst.getEntryAt(idx)).toString();
						nextIsString = false;
					}
					// v => contents of a cell
					// Output after we've seen the string contents
					if(name.equals("v")) {
//						System.out.println(lastContents);
						
						rowData.add(new IndexValue(index,lastContents));
						
					}
				}
				
			}
			@Override
			public void characters(char[] ch, int start, int length)
					throws SAXException {
				if(isAccess()){
					lastContents += new String(ch, start, length);
				}
				
			}
			/**
			 * 如果文档结束后，发现读取的末尾行正处在当前行中，存储下这行
			 * （存在这样一种情况，当待读取的末尾行正好是文档最后一行时，最后一行无法存到集合中，
			 * 因为最后一行没有下一行了，所以不为启动starElement()方法，
			 * 当然我们可以通过指定最大列来处理，但不想那么做，扩展性不好）
			 */
			@Override
			public void endDocument ()throws SAXException{
				if(rowData!=null&&isAccess()&&!rowData.isEmpty()){
					dataList.add(rowData);
					System.out.println("--end");
				}
				  
			}
		
		}
		private boolean isAccess(){
			if(currentRow>=startRow&¤tRow<=endRow){
				return true;
			}
			return false;
		}
		private class IndexValue{
			String v_index;
			String v_value;
			public IndexValue(String v_index, String v_value) {
				super();
				this.v_index = v_index;
				this.v_value = v_value;
			}
			@Override
			public String toString() {
				return "IndexValue [v_index=" + v_index + ", v_value="
						+ v_value + "]";
			}
			public int getLevel(IndexValue p){
			
				char[] other = p.v_index.replaceAll("[0-9]", "").toCharArray();
				char[] self = this.v_index.replaceAll("[0-9]", "").toCharArray();
				if(other.length!=self.length) return -1;
				for(int i=0;i<other.length;i++){
					if(i==other.length-1){
						return self[i]-other[i];
					}else{
						if(self[i]!=other[i]){
							return -1;
						}
					}
					
				}
				return -1;
				
			}
		}
		/**
		 * 获取真实的数据（处理空格）
		 * @return
		 * @throws Exception 
		 */
		public List<List<String>> getMyDataList() throws Exception{
			
			List<List<String>> myDataList = new ArrayList<List<String>>();
			if(dataList==null||dataList.size()<=0) return myDataList;
			
			for(int i=0;i<dataList.size();i++){
				List<IndexValue> i_list = dataList.get(i);
				List<String> tem = new ArrayList<String>();
				int j=0;
                    for(;j< i_list.size()-1;j++){
                    //获取当前值,并存储
                    IndexValue current = i_list.get(j);
                    tem.add(current.v_value);
                    //预存下一个
                    IndexValue next = i_list.get(j+1);
                    //获取差值
                    int level = next.getLevel(current);
                    if(level<=0) throw new Exception("超出处理范围");
                    for(int k = 0;k<level-1;k++){
                        tem.add(null);
                    }
                    }
                    tem.add(i_list.get(j).v_value);
				myDataList.add(tem);
				
			}
			return myDataList;
		}
		public static void main(String[] args) throws Exception {
			
			/*System.out.println('O'-'M');
			System.out.println("O12".hashCode()-"M12".hashCode());
			
			String str = "ggg";
			char[] bm;
			bm = str.toCharArray();
			str = String.valueOf(bm); 
			
			String p1 = "OOM123".replaceAll("[0-9]", "");
			String p2 = "OOO123".replaceAll("[0-9]", "");
			System.out.println(p1.hashCode()-p2.hashCode());*/
		
			
		/*	List<String> list = new ArrayList<String>();
			list.add("a");
			list.add(null);
			list.add("b");
			list.add("");
			list.add("c");
			list.add(" ");
			System.out.println(list);
			System.out.println(list.get(1));
			System.out.println(null=="null");
			System.out.println("null".equals(null));*/
			
			MyExcel2007ForPaging_high reader = new MyExcel2007ForPaging_high("E:/Y02U_CWS-920-2006_01-R01.xlsx",1,100);
			System.out.println("\n---"+reader.getMyDataList());
			
		}
		
}
```
4.3 我还写了个快速算出2007版Excel总行数的帮助类，备份下：
```java
package office;
/* ====================================================================
   Licensed to the Apache Software Foundation (ASF) under one or more
   contributor license agreements.  See the NOTICE file distributed with
   this work for additional information regarding copyright ownership.
   The ASF licenses this file to You under the Apache License, Version 2.0
   (the "License"); you may not use this file except in compliance with
   the License.  You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
==================================================================== */
import java.io.InputStream;
import java.util.regex.Pattern;
import org.apache.commons.lang.StringUtils;
import org.apache.poi.openxml4j.opc.OPCPackage;
import org.apache.poi.xssf.eventusermodel.XSSFReader;
import org.apache.poi.xssf.model.SharedStringsTable;
import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.helpers.XMLReaderFactory;
/**
 * XSSF and SAX (Event API) basic example.
 * See {@link XLSX2CSV} for a fuller example of doing
 *  XSLX processing with the XSSF Event code.
 */
public class MyExcel2007ForMaxRow {
	//new add
	public long maxRow = 0;//记录总行数
	
	private String filename = null;
	public MyExcel2007ForMaxRow(String filename) throws Exception{
		if(StringUtils.isBlank(filename)) throw new Exception("文件名不能空");
		this.filename = filename;
		processFirstSheet();
	}
	/**
	 * 指定获取第一个sheet
	 * @param filename
	 * @throws Exception
	 */
	private void processFirstSheet() throws Exception {
		OPCPackage pkg = OPCPackage.open(filename);
		XSSFReader r = new XSSFReader( pkg );
		SharedStringsTable sst = r.getSharedStringsTable();
		XMLReader parser = fetchSheetParser(sst);
		// To look up the Sheet Name / Sheet Order / rID,
		//  you need to process the core Workbook stream.
		// Normally it's of the form rId# or rSheet#
		InputStream sheet2 = r.getSheet("rId1");
		InputSource sheetSource = new InputSource(sheet2);
		parser.parse(sheetSource);
		sheet2.close();
	}
	
	private XMLReader fetchSheetParser(SharedStringsTable sst) throws SAXException {
		XMLReader parser =
			XMLReaderFactory.createXMLReader(
					"org.apache.xerces.parsers.SAXParser"
			);
		ContentHandler handler = new MaxRowHandler();
		parser.setContentHandler(handler);
		return parser;
	}
	/** 
	 * See org.xml.sax.helpers.DefaultHandler javadocs 
	 */
	private  class MaxRowHandler extends DefaultHandler {
		@Override
		public void startElement(String uri, String localName, String name,
				Attributes attributes) throws SAXException {
			// c => cell
			if(name.equals("c")) {
				String index = attributes.getValue("r");
				if(Pattern.compile("A[0-9]+$").matcher(index).find()){
					maxRow++;
				}
		
			}
		}
	
	}
	
	public static void main(String[] args) throws Exception {
		MyExcel2007ForMaxRow reader = new MyExcel2007ForMaxRow("E:/welding_small.xlsx");
		System.out.println("\n---"+reader.maxRow);
		
	}
}
```
