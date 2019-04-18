# HtmlUnit采集页面信息加工并写入excel表 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月10日 18:23:27[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2044








1、功能：从网页上采集信息加工，如将商品名字重组（Name2FiveWords函数），并写入excel表，涉及对htmlunit元素定位以及点击重定向。

2、参考代码如下：



```java
package com;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;

import com.gargoylesoftware.htmlunit.BrowserVersion;
import com.gargoylesoftware.htmlunit.NicelyResynchronizingAjaxController;
import com.gargoylesoftware.htmlunit.ProxyConfig;
import com.gargoylesoftware.htmlunit.WebClient;
import com.gargoylesoftware.htmlunit.html.DomElement;
import com.gargoylesoftware.htmlunit.html.DomNodeList;
import com.gargoylesoftware.htmlunit.html.HtmlElement;
import com.gargoylesoftware.htmlunit.html.HtmlPage;

public class EBayHU {
	public static void main(String[] args) {
		String path="D:"+System.getProperty("file.separator")+"tmp"+System.getProperty("file.separator")+"test.xls";
		try { 	
			//webclient设置开始
			WebClient webClient = new WebClient(BrowserVersion.CHROME); //创建一个webclient           	        
	    	webClient.getOptions().setJavaScriptEnabled(true); // 启动JS	    	
	    	webClient.getOptions().setUseInsecureSSL(true);//忽略ssl认证	    	
	    	webClient.getOptions().setCssEnabled(false);//禁用Css，可避免自动二次请求CSS进行渲染	    	
			webClient.getOptions().setThrowExceptionOnScriptError(false);//运行错误时，不抛出异常	    	
		    webClient.setAjaxController(new NicelyResynchronizingAjaxController());// 设置Ajax异步    	
	    	//ProxyConfig proxyConfig = webClient.getOptions().getProxyConfig(); //设置代理
	    	//proxyConfig.setProxyHost("ip");  
	        //proxyConfig.setProxyPort(port);
	        //webclinet设置结束
			//打开excel表，准备采集入表
			POIFSFileSystem fs=new POIFSFileSystem(new FileInputStream(path));//得到Excel工作簿对象    	
			HSSFWorkbook wb = new HSSFWorkbook(fs);  //得到Excel工作表对象   	 
			HSSFSheet sheet = wb.getSheetAt(0);//获取第一张表
			int lastrow=sheet.getLastRowNum();	
			for(int i=1;i<=lastrow;i++){//第一行标题，从第二行开始
				//HSSFRow row = sheet.createRow(lastrow+1);
				HSSFRow row = sheet.getRow(i);
				HSSFCell cellURL=row.getCell(0);//获取第一列URL
				String url=cellURL.getStringCellValue();
		        HtmlPage page = webClient.getPage(url); 	//打开网页
		        webClient.waitForBackgroundJavaScript(10000);//等待1秒
		        //第一步：获取商品名称并写入excel第二列
		        HtmlElement itemTitle =page.getHtmlElementById("itemTitle");
		        HSSFCell cellItemTitle=row.createCell(1);
		        String title=itemTitle.asText();
		        title=title.replaceAll("Details about", "");
				cellItemTitle.setCellValue(title.trim());	
				//将商品名称随机分成5组		
		        HSSFCell cellItemreTitle=row.createCell(2);	
		        cellItemreTitle.setCellValue(Name2FiveWords(title));	
				//第二步：获取商品价格并写入第三列
				HtmlElement price =page.getHtmlElementById("prcIsum");
				HSSFCell cellPrice=row.createCell(3);
				cellPrice.setCellValue(price.asText());	
				//第三步，获取商品图片并写入excel第四列
				//if(!page.asText().contains("vi_main_img_fs")) continue;
		        HtmlElement propic =page.getHtmlElementById("vi_main_img_fs");
		        DomNodeList<HtmlElement> picnodes=propic.getElementsByTagName("img");
		        StringBuffer strbuf=new StringBuffer();
		        for(int m=0;m<picnodes.size();m++){
		    		HtmlElement pic=picnodes.get(m);
		    		page=(HtmlPage)pic.click();
		    		webClient.waitForBackgroundJavaScript(10000);
		    		HtmlElement bigpic =page.getHtmlElementById("icImg");
		    		String picsrc=bigpic.getAttribute("src");
		    		strbuf.append(picsrc+"\r\n");//图片的网址之间用逗号隔开
		        }
		        String strResult=strbuf.toString();
	        	strResult=strResult.substring(0,strResult.length()-1);//截取最后一个字符
	        	HSSFCell cellpic=row.createCell(4);//写入excel第列
	        	cellpic.setCellValue(strResult);
		        //第四步：卖家信息写入第5列
		        HtmlElement seller =page.getHtmlElementById("mbgLink");
		        String href=seller.getAttribute("href");
		        HSSFCell cellseller=row.createCell(5);
		        cellseller.setCellValue(seller.asText()+"\r\n"+href);
		        //第五步：获取商品详情并写入第6列
		        DomNodeList<DomElement> nodelist=page.getElementsByTagName("div");
		        for(int j=0;j<nodelist.getLength();j++){
		        	DomElement domElement = (DomElement)nodelist.get(j);
		        	if(domElement.getAttribute("class").equals("itemAttr")){
		        		//写入excel第四列
				        HSSFCell cellinfo=row.createCell(6);
				        cellinfo.setCellValue(domElement.asText());
				        break;
		        	}
		        } 
		        //第六步：获取商品更多介绍并写入第7列
		        HtmlElement descifr =page.getHtmlElementById("desc_ifr");//切换到iframe
		        String src=descifr.getAttribute("src");
		        HtmlPage ifrpage=webClient.getPage(src);//读取iframe网页
		        webClient.waitForBackgroundJavaScript(10000);
		        DomNodeList<DomElement> nodelist2=ifrpage.getElementsByTagName("div");
		        for(int j=0;j<nodelist2.getLength();j++){
		        	DomElement domElement = (DomElement)nodelist2.get(j);
		        	if(domElement.getAttribute("class").equals("prod_item description")){
		        		//写入excel第四列
		        		 HSSFCell cellmoreinfo=row.createCell(7);
		 		        cellmoreinfo.setCellValue(domElement.asText());
				        break;
		        	}
		        } 
		        //写入excel表
		        FileOutputStream out=new FileOutputStream(path);
				out.flush();
				wb.write(out);
				out.close();
			}
			wb.close();			
			fs.close();		    
		}catch (Exception e) {
			System.err.println( "Exception: " + e );
		}
		
	}
	
	//将字符串分为5个部分
	public static String Name2FiveWords(String title){
		String s[]=title.split(" ");
		String sr="";
		int len=s.length/5+1;//分成5组
        for(int m=0;m<5;m++){
        	String tmp="";
        	for(int n=0;n<len;n++){
        		if((m*len+n)<s.length) tmp=tmp+" "+s[m*len+n];
        	}    		       		
        	sr=sr+"|"+tmp.trim();
        }
        return sr;
	}
}
```





