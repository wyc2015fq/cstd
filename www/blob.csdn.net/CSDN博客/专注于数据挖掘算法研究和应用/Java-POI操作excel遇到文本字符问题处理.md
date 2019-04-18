# Java-POI操作excel遇到文本字符问题处理 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月11日 11:54:44[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3835








1、问题：用poi读写excel，据工单编号匹配两张sheet的记录，提取cell文本格内容，发现相同字符无法匹配，用byte才发现，有部分文本带了乱码（ascii码是-62和-96，不知道是什么东西）；

2、解决：增加对字符文本的乱码处理，过滤掉乱码，同时用byte数组来匹配，代码如下：



```java
package dx;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;

public class order {
	public static void main(String[] args) {
		try {
			//获取excel文件
			String path="D:"+System.getProperty("file.separator")+"tmp"+System.getProperty("file.separator")+"order.xls";
			POIFSFileSystem fs=new POIFSFileSystem(new FileInputStream(path));
			//得到Excel工作簿对象    
			HSSFWorkbook wb = new HSSFWorkbook(fs);  
			//得到Excel工作表对象    
			HSSFSheet sheet1 = wb.getSheetAt(0);
			HSSFSheet sheet2 = wb.getSheetAt(1);
			
			//用第二张表上的工单编号去匹配第一张表上的工单编号，匹配在第一张第二列输入yes
			int rowCount2=sheet2.getLastRowNum();		
			for(int i=1;i<=rowCount2;i++){
				HSSFRow row2 = sheet2.getRow(i);
				HSSFCell orderNo2 = row2.getCell(0);
				String strNo2=orderNo2.getStringCellValue();
				
				strNo2=MessyCodeFilter(strNo2);//清除乱码
				byte[] byt2=strNo2.getBytes("UTF-8");
					
				//匹配第一张表的工单
				int rowCount1=sheet1.getLastRowNum();
				for(int j=1;j<=rowCount1;j++){
					HSSFRow row1 = sheet1.getRow(j);
					HSSFCell orderNo1 = row1.getCell(0);
					String strNo1=orderNo1.getStringCellValue();
					
					strNo1=MessyCodeFilter(strNo1);//清除乱码
					byte[] byt1=strNo1.getBytes("UTF-8");
					/*for(int ii=0;ii<byt1.length;ii++){
						System.out.print(ii+":"+byt1[ii]+"\r\n");
					}*/
						
					//if(strNo2==strNo1){//工单编号一致判断
					if(byt2.length==byt1.length){
						boolean bif=true;
					    for(int bi=0;bi<byt2.length;bi++){
					    	if(byt2[bi]!=byt1[bi]){
					    		bif=false;
					    		break;
					    	}
					    }
					    if(bif){
							HSSFCell yesno1=row1.getCell(1);	
							yesno1.setCellValue("yes");		
							HSSFCell yesno2 = row2.getCell(1);
							yesno2.setCellValue("yes");
							FileOutputStream out=new FileOutputStream(path);
							out.flush();
							wb.write(out);
							out.close();
							break;
					    }
					}
				}
				
			}	
			wb.close();			
			fs.close();
			 
		} catch (FileNotFoundException e) {e.printStackTrace();} 
		catch (IOException e) {e.printStackTrace();} 
	}
	
	//非ascii编码范围过滤
	private static String AsciiCodeFilter(String strCont) {  
			try{
				byte[] byteCon=strCont.getBytes("UTF-8");
				for(int i=0;i<byteCon.length;i++){
					if (byteCon[i]<0 || byteCon[i]>127){
						byteCon[i]=32;
					}	
				}
				return new String(byteCon).trim();
			} catch (Exception e) {  
	            e.printStackTrace();  
	        }  
			return strCont;
	}    
	
	//非有效数字、字母、字符过滤
	private static String MessyCodeFilter(String strCont) {  
        try {  
            Pattern p = Pattern.compile("\\s*|\t*|\r*|\n*");  
            Matcher m = p.matcher(strCont);  
            String after = m.replaceAll("");  
            String temp = after.replaceAll("\\p{P}", "");  
            char[] ch = temp.trim().toCharArray();  

            int length = (ch != null) ? ch.length : 0;  
            for (int i = 0; i < length; i++) {  
                char c = ch[i];  
                if (!Character.isLetterOrDigit(c)) {  
                    String str = "" + ch[i];  
                    //if (!str.matches("[\u4e00-\u9fa5]+")) {  
                    if (!str.matches("[0-9a-zA-Z\\u4e00-\\u9fa5]+")){  
                       //ch[i]='\0';//该为置空，也可以用位移
                   	   ch[i]=' ';//用空格
                    }  
                }  
            }  
            return  new String(ch).replace(" ", "").trim();
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
        return strCont.trim();
	}
}
```





