# 关于使用jxl导出excel文件（2003） 单个sheet 65536 超出范围的 情况处理 - z69183787的专栏 - CSDN博客
2013年07月23日 18:54:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3288
```java
/**
 * 
 */
package com.wonders.task.excel.util;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import jxl.JXLException;
import jxl.Workbook;
import jxl.write.Alignment;
import jxl.write.Label;
import jxl.write.WritableCellFormat;
import jxl.write.WritableFont;
import jxl.write.WritableSheet;
import jxl.write.WritableWorkbook;
/** 
 * @ClassName: FileUtil 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013-5-15 上午9:52:01 
 *  
 */
public class FileUtil {
	public static void createXls(OutputStream os, String title, List head, List data) throws IOException, JXLException {
		WritableWorkbook wwb = Workbook.createWorkbook(os);
		WritableFont wfTitle = new WritableFont(WritableFont.ARIAL, 18, WritableFont.BOLD);
		WritableCellFormat wcfTitle = new WritableCellFormat(wfTitle);
		wcfTitle.setAlignment(Alignment.CENTRE);
		int iMaxLines = 60000;
		int totalLines = data.size();
		int page = totalLines / iMaxLines;
		System.out.println("page="+page);
		int lastLines = totalLines % iMaxLines;//尾页要写的行数
		if (totalLines == 0) {
            return;
        }
		if (page >= 1 && lastLines > 0) {
			page = page + 1;
        }
		if(page == 0){
			page = 1;
		}
	 
		// 合并标题单元格
		//sheet.mergeCells(0, 0, head.size()-1, 0);
		// 设置标题行高
		//sheet.setRowView(0, 1000);
		//sheet.setRowView(1, 1000);
		
		// title
		for(int p =0;p<page;p++){
			
			WritableSheet sheet = wwb.createSheet("sheet"+p, p);
			
			int iStartRow = 0;
			int iEndRow = 0;
			if (p == 0) {
                iStartRow = 0;
                if (page == 1) {
                    iEndRow = totalLines - 1;
                } else {
                    iEndRow = (p + 1) * iMaxLines - 1;
                }
            } else {
                iStartRow = p * iMaxLines;
                if (p == page - 1) {
                    iEndRow = totalLines - 1;
                } else {
                    iEndRow = (p + 1) * iMaxLines - 1;
                }
            }
			
			 List listPageValue = new ArrayList();//保存每页的数据
             for (int i = iStartRow; i <= iEndRow; i++) {
                 if (data.get(i) == null) {
                     continue;
                 }
                 System.out.println(data.get(i));
                 listPageValue.add(data.get(i));
             }
             
			
			
			Label labTitle = new Label(0, 0, title, wcfTitle);
			sheet.addCell(labTitle);
			
			// head
			WritableFont wfHead = new WritableFont(WritableFont.ARIAL, 12, WritableFont.BOLD);
			WritableCellFormat wcfHead = new WritableCellFormat(wfHead);
			for (int i=0; i<head.size(); i++) {
				Label labHead = new Label(i, 1, (String)head.get(i), wcfHead);
				sheet.addCell(labHead);
			}
			
			// data
			if (data != null && data.size() > 0) {
				if (data.get(0) instanceof Map) {
					// list中是Map
					for (int i=0; i<listPageValue.size(); i++) {
						System.out.println(i);
						Map m = (Map) listPageValue.get(i);
						Set s = m.keySet();
						Iterator itr = s.iterator();
						for (int j=0; itr.hasNext(); j++) {
							Object key = itr.next();
							try {
								Label labData = new Label(j, i+2, m.get(key).toString());
								sheet.addCell(labData);
							} catch (NullPointerException e){}
						}
					}
				} else if (data.get(0) instanceof List) {
					// list中是list
					for (int i=0; i<listPageValue.size(); i++) {
						List lst = (List) listPageValue.get(i);
						for (int j=0; j<lst.size(); j++) {
							try {
								Label labData = new Label(j, i+2, lst.get(j).toString());
								sheet.addCell(labData);
							} catch (NullPointerException e){}
						}
					}
				} else if (data.get(0) instanceof Object[]) {
					// list中是数组
					for (int i=0; i<listPageValue.size(); i++) {
						Object[] obj = (Object[]) listPageValue.get(i);
						for (int j=0; j<obj.length; j++) {
							try {
								Label labData = new Label(j, i+2, obj[j].toString());
								//System.out.println(obj[j]);
								sheet.addCell(labData);
							} catch (NullPointerException e){}
						}
					}
				} else {
					
				}
	
			}
			
			
			
		}
		wwb.write();
		wwb.close();
	}
}
```
