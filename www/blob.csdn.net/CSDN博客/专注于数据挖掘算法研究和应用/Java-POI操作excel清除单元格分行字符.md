# Java-POI操作excel清除单元格分行字符 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年01月13日 14:52:19[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5107








1、需求背景：

      有时需将一份大的excel文件导入到hdfs文件，用POI组件来导入excel文件并输出成txt文本，此时一些单元格中带有分行字符的就给输出带来麻烦，一般MR是按照行来处理，所以需要将单元格内的文本清除掉分行字符，变成一行；




2、解决思路：

    excel单元格输入时的回车符，有软回车Alt+Enter和硬回车Enter，对应的ASCII码如下表：

![](https://img-blog.csdn.net/20170113145701279?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 如此，只要替换对应字符的编码即可。



3、参考代码如下：



```java
package csc.xls;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;

public class ExcelRead {
	//读取excel文件
	public boolean xlsRead(String path) {
		StringBuffer strbuf=new StringBuffer();
		try{
			//获取excel文件
			String file=path+"all.xls";
			POIFSFileSystem fs=new POIFSFileSystem(new FileInputStream(file));
			//得到Excel工作簿对象    
			HSSFWorkbook wb = new HSSFWorkbook(fs);  
			//得到Excel工作表对象    
			HSSFSheet st = wb.getSheetAt(0);
			System.out.println(st.getLastRowNum());
			for(int i=1;i<=st.getLastRowNum();i++){//去掉表头，遍历每一行
				HSSFRow row = st.getRow(i);			
				HSSFCell cellContent = row.getCell(0);//第1列，投诉内容
				String content=cellContent.getStringCellValue();
				//content=content.replaceAll("[\\t\\n\\r]", "");//将内容区域的回车换行去除 
				//删除excel中回车符，软回车Alt+Enter）和硬回车Enter
				for(int j=10;j<14;j++){
					content = content.replaceAll(String.valueOf((char)j), "");
				}
				HSSFCell cellLabel = row.getCell(1);//第2列，越级标记
				String label=cellLabel.getStringCellValue();
				if(label.equals("否"))
					strbuf.append("0"+"|"+content+"\r\n");
				else strbuf.append("1"+"|"+content+"\r\n");
			}			
			wb.close();			
			fs.close();		
		}catch (Exception e) {
			System.out.println(e.getMessage());
			return false;
		} 
		//写入文件
		try{
			File file=new File(path+"traindata.txt");
			if(!file.exists()) file.createNewFile();
			FileOutputStream out=new FileOutputStream(file,true); 
			out.write(strbuf.toString().getBytes("utf-8"));
			out.close();
		}catch (IOException e){
			e.printStackTrace();
		}
		return true;
	}
	
	
	public static void main(String[] args) {
		String path="D:"+System.getProperty("file.separator")+"tmp"+System.getProperty("file.separator");
		ExcelRead er=new ExcelRead();
		er.xlsRead(path);
	}
}
```







