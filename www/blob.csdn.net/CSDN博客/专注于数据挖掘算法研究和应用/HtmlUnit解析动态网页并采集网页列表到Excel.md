# HtmlUnit解析动态网页并采集网页列表到Excel - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年06月01日 14:55:38[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4384








HtmlUnit可以解析动态网页，本文这里选取了一个网址（页面需js/ajax动态加载），模拟浏览器操作页面各元素，包括点击获取列表值，主要是掌握了HtmlUnit解析页面元素的主要用法，同时使用POI HSSF将页面解析出来的ul/li标签内容导入到excel表。

本文代码只用来作为HtmlUnit页面解析的参考，因为具体页面有具体信息需要处理。其中对页面如果有级联元素，可以参考，如先选地市再选区域。



```java
package com;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRichTextString;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;

import com.gargoylesoftware.htmlunit.BrowserVersion;
import com.gargoylesoftware.htmlunit.WebClient;
import com.gargoylesoftware.htmlunit.html.DomElement;
import com.gargoylesoftware.htmlunit.html.DomNodeList;
import com.gargoylesoftware.htmlunit.html.HtmlDivision;
import com.gargoylesoftware.htmlunit.html.HtmlElement;
import com.gargoylesoftware.htmlunit.html.HtmlPage;
import com.gargoylesoftware.htmlunit.html.HtmlTextInput;

public class CMHtml {
	
	public static ArrayList<String> gAddList=new ArrayList<String>();
	public static String gQueryURL = "http://xxx";
	public void spider(String strCity,String strArea,String strAddr){  
		try { 	
			//创建一个webclient
	        WebClient webClient = new WebClient(BrowserVersion.CHROME);               
	        // 启动JS
	    	webClient.getOptions().setJavaScriptEnabled(true); 
	    	//忽略ssl认证
	    	webClient.getOptions().setUseInsecureSSL(true);
	    	//禁用Css，可避免自动二次请求CSS进行渲染
	    	webClient.getOptions().setCssEnabled(false);
	    	//运行错误时，不抛出异常
			//webClient.getOptions().setThrowExceptionOnScriptError(false);
	    	// 设置Ajax异步
		    //webClient.setAjaxController(new NicelyResynchronizingAjaxController());
	        //获取页面
	        HtmlPage page = webClient.getPage(gQueryURL); 
	        webClient.waitForBackgroundJavaScript(10000);
	        //webClient.waitForBackgroundJavaScriptStartingBefore(10000);
	        //判断图形验证码是否弹出，获取i标签节点列表
	        DomNodeList<DomElement> nodelist=page.getElementsByTagName("i");
	        int iCode=0;
	        for(int i=0;i<nodelist.getLength();i++){
	        	DomElement domElement = (DomElement)nodelist.get(i);
	        	String txt=domElement.asXml();
	        	if(txt.indexOf("c_vcode")!=-1){//找出id为c_vcode的i标签
	        		iCode=i;
	        		break;
	        	}  		
	        }
	        DomElement domElement = (DomElement)nodelist.get(iCode);
	        if(domElement.asXml().indexOf("display: none;")!=-1){//没弹出
	        	//获取返回页面的地址列表div/ul/li标签
		    	HtmlDivision divKD=(HtmlDivision)page.getElementById("kd_content"); 
		    	DomNodeList<HtmlElement> nodeKD=divKD.getElementsByTagName("li");
		    	for(int m=0;m<nodeKD.size();m++){
		    		HtmlElement heLi=nodeKD.get(m);
		    		if(heLi.asText().equals(strCity)){//定位到城市
		    			page=(HtmlPage)heLi.click();
		    			HtmlDivision divKD1=(HtmlDivision)page.getElementById("kd_content"); 
				    	DomNodeList<HtmlElement> nodeKD1=divKD1.getElementsByTagName("li");
				    	for(int n=0;n<nodeKD1.size();n++){
				    		HtmlElement heLi1=nodeKD1.get(n);
				    		if(heLi1.asText().equals(strArea)){//定位到区域
				    			page=(HtmlPage)heLi1.click();
				    			break;
				    		}
				    	}
				    	break;
		    		}
		    	}
		    	//输入具体地址
		        HtmlTextInput kw =(HtmlTextInput)page.getElementByName("kw");
		        kw.setValueAttribute(strAddr);    
		        //获取a标签，提交查询
		        HtmlElement heAnchor =page.getHtmlElementById("srhBtn");
		        HtmlPage retPage = (HtmlPage) heAnchor.click();
		        // 等待JS驱动dom完成获得还原后的网页
		    	webClient.waitForBackgroundJavaScript(10000);
		    	//获取返回页面的地址列表div/ul/li标签
		    	HtmlDivision div=(HtmlDivision)retPage.getElementById("query_result"); 
		    	DomNodeList<HtmlElement> addNode=div.getElementsByTagName("li");
		    	for (int i=0;i<addNode.getLength();i++){
		    		HtmlElement heLi=addNode.get(i);
		    		gAddList.add(heLi.asText());
		    	}    	
		    	webClient.close(); 	
		    	return;
	        }else{//弹出，重新启动爬虫，避免图形验证码识别
	        	webClient.close();//关闭浏览器
	        	spider(strCity,strArea,strAddr);
	        	return;
	        }	        
		}catch (Exception e) {System.err.println( "Exception: " + e ); }
	}
	
	public void query(String path){
		
		try {
			//获取excel文件
			POIFSFileSystem fs=new POIFSFileSystem(new FileInputStream(path));
			//得到Excel工作簿对象    
			HSSFWorkbook wb = new HSSFWorkbook(fs);  
			//得到Excel工作表对象    
			HSSFSheet sheet = wb.getSheetAt(0);
			//取得有效的行数
			int rowcount = sheet.getLastRowNum(); 
			//得到Excel工作表的行    
			for (int i=1;i<=rowcount;i++){ //去掉第一行表头
				HSSFRow row = sheet.getRow(i);  
				//得到Excel工作表指定行的单元格    
				HSSFCell cellCity = row.getCell(0);//获取地市
				String strCity=cellCity.getStringCellValue();
				HSSFCell cellArea = row.getCell(1);//获取区域
				String strArea=cellArea.getStringCellValue(); 
				HSSFCell cellAddr = row.getCell(2);//获取地址
				String strAddr=cellAddr.getStringCellValue(); 
				//爬虫，返回列表结果
				spider(strCity,strArea,strAddr);
				//返回的地址写入excel表
				for(int j=0;j<gAddList.size();j++){
					HSSFCell cellRes = row.createCell(j+2+1);
					cellRes.setCellValue(new HSSFRichTextString(gAddList.get(j)));
				}
				gAddList.clear();
				/*Iterator<String> it = gAddList.iterator();
				while(it.hasNext()){
				  it.remove();
				}*/
				FileOutputStream out=new FileOutputStream(path);
				out.flush();
				wb.write(out);
				out.close();
			}		
			wb.close();			
			fs.close();
			 
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		
		
	}
	
	public static void main(String[] args) {
		CMHtml lession = new CMHtml();
		String path=System.getProperty("user.dir")+System.getProperty("file.separator")+"cmbb.xls";//增加文件分隔符，通用windows和linux系统
        lession.query(path);
    }
}
```







