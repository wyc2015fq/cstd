# struts2 生成并下载 excel 文件 (jxl) - z69183787的专栏 - CSDN博客
2013年12月13日 16:33:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2550
```java
/**
 * 
 */
package com.wonders.stpt.innerWork.action;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.Namespace;
import org.apache.struts2.convention.annotation.ParentPackage;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Controller;
import org.apache.struts2.convention.annotation.Result;
import com.opensymphony.xwork2.ModelDriven;
import com.wonders.stpt.innerWork.model.vo.InnerWorkListVo;
import com.wonders.stpt.innerWork.service.InnerWorkService;
import com.wonders.stpt.innerWork.util.ExcelUtil;
import com.wonders.stpt.page.model.PageResultSet;
import com.wonders.stpt.util.ActionWriter;
/** 
 * @ClassName: InnerWorkListAction 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013年12月7日 下午7:51:41 
 *  
 */
@ParentPackage("struts-default")
@Namespace(value="/innerWorkList")
@Controller("innerListAction")
@Scope("prototype")
public class InnerWorkListAction extends AbstractParamAction implements ModelDriven<InnerWorkListVo>{
	/**
	 * 
	 */
	private static final long serialVersionUID = -6137396100679830447L;
	private InnerWorkListVo bo = new InnerWorkListVo();
	private ActionWriter aw = new ActionWriter(response);
	private InnerWorkService service;
	private PageResultSet<Map<String,Object>> pageResultSet;
	private String downloadFileName  ="部门内部工作";
	
	
	public String getDownloadFileName() {
		return downloadFileName;
	}
	public void setDownloadFileName(String downloadFileName) {
		this.downloadFileName = downloadFileName;
	}
	public PageResultSet<Map<String,Object>> getPageResultSet() {
		return pageResultSet;
	}
	public void setPageResultSet(PageResultSet<Map<String,Object>> pageResultSet) {
		this.pageResultSet = pageResultSet;
	}
	public InnerWorkListVo getBo() {
		return bo;
	}
	public void setBo(InnerWorkListVo bo) {
		this.bo = bo;
	}
	public InnerWorkService getService() {
		return service;
	}
	@Autowired(required=false)
	public void setService(@Qualifier("innerWorkService")InnerWorkService service) {
		this.service = service;
	}
	@Override
	public InnerWorkListVo getModel() {
		// TODO Auto-generated method stub
		return bo;
	}
	
	@Action(value="list")
	public String list(){
		this.pageResultSet = this.service.list(bo);
		aw.writeJson(pageResultSet);
		return null;
	}
	
	private InputStream excelFile;  
	
	public InputStream getExcelFile() {  
        return excelFile;  
    }  
  
    public void setExcelFile(InputStream excelFile) {  
        this.excelFile = excelFile;  
    }  
    
	@Action(value = "excel", results = { @Result(name = "success", type = "stream", params = {  
            "contentType", "application/vnd.ms-excel", "inputName",  
            "excelFile", "contentDisposition",  
            "attachment;filename=${downloadFileName}.xls", "bufferSize", "1024" }) })  
    public String export2() throws Exception {  
		this.downloadFileName = "部门内部工作";
		downloadFileName = URLEncoder.encode(downloadFileName, "utf-8");
		downloadFileName = downloadFileName.replace("+", "%20"); // encode后替换  解决空格问题
		this.pageResultSet = this.service.list(bo);
        ExcelUtil eu = new ExcelUtil();  
        List<String> head = new ArrayList<String>();
        head.add("工作内容");head.add("工作进度");
        head.add("计划完成时间");head.add("完成标志");
        head.add("完成状态");head.add("负责人");
        head.add("负责人领导");
        List<List<String>> data = new ArrayList<List<String>>();
        List<Map<String,Object>> map = this.pageResultSet.getList();
        for(Map<String, Object> m : map){
        	List<String> inner = new ArrayList<String>();
        	inner.add(m.get("JOBNAME").toString());
        	inner.add(m.get("PROCESS").toString());
        	inner.add(m.get("PFTIME").toString());
        	inner.add(m.get("FFLAG").toString());
        	inner.add(m.get("STATUS").toString());
        	inner.add(m.get("RPEOPLE").toString());
        	inner.add(m.get("RLEADER").toString());
        	data.add(inner);
        }
        ByteArrayOutputStream output = new ByteArrayOutputStream();  
        ExcelUtil.createXls(output, "部门内部工作", head, data);
        
        byte[] ba = output.toByteArray();  
        excelFile = new ByteArrayInputStream(ba);  
        output.flush();
        output.close();
        return "success";  
    }  
}
```
```java
/**
 * 
 */
package com.wonders.stpt.innerWork.util;
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
 * @ClassName: ExcelUtil 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013年12月13日 下午3:35:27 
 *  
 */
public class ExcelUtil {
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

