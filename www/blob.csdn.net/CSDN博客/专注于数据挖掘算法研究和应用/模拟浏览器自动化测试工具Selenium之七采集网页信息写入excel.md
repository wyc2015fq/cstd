# 模拟浏览器自动化测试工具Selenium之七采集网页信息写入excel - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月11日 10:17:12[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2480








功能：从网页上采集信息写入excel，有鼠标移动到相关元素代码，参考如下：



```java
package com.test;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URL;
import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;
import org.openqa.selenium.By;
import org.openqa.selenium.Proxy;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.interactions.Actions;
import org.openqa.selenium.remote.CapabilityType;
import org.openqa.selenium.remote.DesiredCapabilities;

import com.util.Logs;

public class EBayCom {
	public static void main(String[] args) throws IOException {
		System.getProperties().setProperty("webdriver.chrome.driver","D:\\dev\\workspace\\ocweb\\libs\\chromedriver.exe");	
		WebDriver webDriver = new ChromeDriver();
		//打开excel表，准备采集入表
		//获取excel文件
		String path="D:"+System.getProperty("file.separator")+"tmp"+System.getProperty("file.separator")+"uee.xls";
		POIFSFileSystem fs=new POIFSFileSystem(new FileInputStream(path));
		//得到Excel工作簿对象    
		HSSFWorkbook wb = new HSSFWorkbook(fs);  
		//得到Excel工作表对象    
		HSSFSheet sheet = wb.getSheetAt(0);
		int lastrow=sheet.getLastRowNum();	
		HSSFRow row = sheet.createRow(lastrow+1);//第一行标题，第二行开始写入
		//访问网址
		try {
			webDriver.get("url");//访问网址
			//标题
			WebElement eleItemTitle = webDriver.findElement(By.id("itemTitle")); 		
			String txtItemTitle=eleItemTitle.getText();
			HSSFCell cellItemTitle=row.createCell(0);
			cellItemTitle.setCellValue(txtItemTitle);
			//图片
			WebElement elePic= webDriver.findElement(By.id("vi_main_img_fs")); 
			List<WebElement> eleImgs= elePic.findElements(By.tagName("img"));
			int i=1;//第二列
			for(WebElement img:eleImgs){
				Actions action=new Actions(webDriver); //模拟鼠标操作  
				action.moveToElement(img);  
				Thread.sleep(1000);//休息1秒
				WebElement eleBPic= webDriver.findElement(By.id("icImg")); 
				String src=eleBPic.getAttribute("src");
				//downloadImage(src,String.valueOf(i));
				HSSFCell cellsrc = row.createCell(i);
				cellsrc.setCellValue(src);
				i++;
			}		
			//Seller information
			WebElement eleSeller= webDriver.findElement(By.id("mbgLink"));
			String txtSeller=eleSeller.getText();
			HSSFCell cellSeller = row.createCell(4);
			cellSeller.setCellValue(txtSeller);
			String herf=eleSeller.getAttribute("href");
			HSSFCell cellhref= row.createCell(5);
			cellhref.setCellValue(herf);
			//产品描述Item Specifics
			WebElement eleItem=webDriver.findElement(By.id("desc_ifr"));
			//String spesrc=eleItem.getAttribute("src");//获取iframe页面网址
			webDriver.switchTo().frame(eleItem);//切换到iframe
			Thread.sleep(1000);//休息1秒
			WebElement eleinfo=webDriver.findElement(By.id("desc"));
			HSSFCell cellspecies= row.createCell(6);
			cellspecies.setCellValue(eleinfo.getText());
			FileOutputStream out=new FileOutputStream(path);
			out.flush();
			wb.write(out);
			out.close();
		}catch (Exception e) { System.err.println( "Exception: " + e );}
		wb.close();			
		fs.close();
		webDriver.close();
        webDriver.quit();
	}
	
	public static void downloadImage(String Imageurl,String filename) throws IOException{
		URL url = new URL(Imageurl);
		//打开网络输入流
		DataInputStream dis = new DataInputStream(url.openStream());
		String newImageName="D://tmp//"+filename+".jpg";
		//建立一个新的文件
		FileOutputStream fos = new FileOutputStream(new File(newImageName));
		byte[] buffer = new byte[1024];
		int length;
		//开始填充数据
		while((length = dis.read(buffer))>0){
			fos.write(buffer,0,length);
		}
		dis.close();
		fos.close();	
	}
}
```





