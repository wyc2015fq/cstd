# SpringMVC-4.2+ExcelView视图进行Excel文件下载 - z69183787的专栏 - CSDN博客
2018年09月09日 11:23:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：273
jar包方面大家可自行下载，springMvc4.2+版本+poi-3.10.1版本
```
</dependency>
<dependency>
    <groupId>org.apache.poi</groupId>
    <artifactId>poi</artifactId>
    <version>3.10.1</version>
</dependency>
<dependency>
    <groupId>org.apache.poi</groupId>
    <artifactId>poi-ooxml</artifactId>
    <version>3.10.1</version>
</dependency>
```
1、4.2以后，使用了AbstractXlsxView和AbstractXlsxStreamingView代替了原有的AbstractExcelView 来进行excel下载的视图构建
**这里有一个问题，如果数据为空，则默认调用父类生成的excel会无法打开，**
**最后发现原因是，空的SXSSFWorkbool没有创建sheet，导致excel打开需要修复，这里创建一个空sheet即可，可参见[https://blog.csdn.net/z69183787/article/details/82734931](https://blog.csdn.net/z69183787/article/details/82734931)**
```java
import com.dianping.emidas.manage.biz.external.mp.util.ExcelExportUtil;
import org.apache.commons.collections.CollectionUtils;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.xssf.streaming.SXSSFWorkbook;
import org.springframework.web.servlet.view.document.AbstractXlsxStreamingView;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;
import java.util.Map;
public class MpManageExcelView extends AbstractXlsxStreamingView {
    @Override
    protected SXSSFWorkbook createWorkbook(Map<String, Object> model, HttpServletRequest request) {
        List<?> data = (List<?>) model.get("data");
        if(CollectionUtils.isNotEmpty(data)) {
            return ExcelExportUtil.export(data);
        }
        return super.createWorkbook(model, request);
    }
    @Override
    protected void buildExcelDocument(Map<String, Object> model, Workbook workbook,
                                      HttpServletRequest request, HttpServletResponse response)
            throws Exception {
        //解决空sheet无法现在的问题
        if(workbook.getNumberOfSheets() == 0){
            workbook.createSheet("sheet1");
        }
        String filename = model.get("fileName") + ".xlsx"; // 取下载时客户端Excel的名称
        //处理中文文件名
        String reqCharset = request.getCharacterEncoding(); /*根据request的getCharacterEncoding得到请求时的编码*/
        filename = new String(filename.getBytes(reqCharset), "ISO8859-1");
        response.setCharacterEncoding(reqCharset);
        // 若想下载时自动填好文件名，则需要设置响应头的"Content-disposition"
        response.setHeader("Content-disposition", "attachment;filename=" + filename);
    }
}
```
方法其实显而易见，重写buildExcelDocument ，对model对象及poi中的workbook对象进行修改及赋值，最终response进行输出。可以在方法中对workbook进行createsheet及row的一些poi操作。也可以和例子中一样，重写父类中的createWorkbook，创建自己的workbook对象。父类中原逻辑为创建了一个空的 workbook对象，供build方法使用
```java
*/
public abstract class AbstractXlsxStreamingView extends AbstractXlsxView {
	/**
	 * This implementation creates a {@link SXSSFWorkbook} for streaming the XLSX format.
	 */
	@Override
	protected SXSSFWorkbook createWorkbook(Map<String, Object> model, HttpServletRequest request) {
		return new SXSSFWorkbook();
	}
```
当然，当真正进行下载时的整体逻辑在父类AbstractXlsxView中已经以模板模式的方式给出了定义，定义了方法的大致业务逻辑及骨架，通过子类自行进行重写，实现自身逻辑
```java
@Override
	protected final void renderMergedOutputModel(
			Map<String, Object> model, HttpServletRequest request, HttpServletResponse response) throws Exception {
		// Create a fresh workbook instance for this render step.
		Workbook workbook = createWorkbook(model, request);
		// Delegate to application-provided document code.
		buildExcelDocument(model, workbook, request, response);
		// Set the content type.
		response.setContentType(getContentType());
		// Flush byte array to servlet output stream.
		renderWorkbook(workbook, response);
	}
```
剩下就是一些工具类和实际的controller类的代码：
Excel工具类：
```java
import org.apache.commons.lang3.StringUtils;
import org.apache.poi.ss.usermodel.*;
import org.apache.poi.xssf.streaming.SXSSFWorkbook;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
public class ExcelExportUtil {
    private static final Logger logger = LoggerFactory.getLogger(ExcelExportUtil.class);
    private static final DateFormat df = new SimpleDateFormat("yyyy-MM-dd");
    private static final int MAX_SHEET_ROWS = 60000;
    public static SXSSFWorkbook export(List<?> dataList){
        // data
        if (dataList==null || dataList.size()==0) {
            throw new RuntimeException("data is empty,can not generate excel file");
        }
        //获取对象类型注解
        Class<?> targetClass = dataList.get(0).getClass();
        String sheetName = targetClass.getSimpleName();
        ExcelSheet excelSheet = targetClass.getAnnotation(ExcelSheet.class);
        if(excelSheet != null && StringUtils.isNotBlank(excelSheet.name())){
            sheetName = excelSheet.name();
        }
        // 解析字段
        List<Field> fields = new ArrayList<Field>();
        if (targetClass.getDeclaredFields()!=null && targetClass.getDeclaredFields().length>0) {
            for (Field field: targetClass.getDeclaredFields()) {
                if (Modifier.isStatic(field.getModifiers())) {
                    continue;
                }
                fields.add(field);
            }
        }
        if (fields==null || fields.size()==0) {
            throw new RuntimeException("data field can not be empty");
        }
        SXSSFWorkbook wb = new SXSSFWorkbook(100);
        int dataSize = dataList.size();
        int index = 0;
        do{
            List<?> subList = dataList.subList(index,Math.min(dataSize,index+MAX_SHEET_ROWS));
            resolveSheet(wb,fields,subList,sheetName+"-"+index);
            index += MAX_SHEET_ROWS;
        }while (index < dataSize);
        return wb;
    }
    /**
     * 处理一定数量的sheet
     * @param workbook
     * @param dataList
     */
    private static void resolveSheet(Workbook workbook,List<Field> fields,List<?> dataList,String sheetName){
        Sheet sheet = workbook.createSheet(sheetName);
        //head
        Row headRow = sheet.createRow(0);
        CellStyle headStyle = generateHeadStyle(workbook);
        for (int i = 0; i < fields.size(); i++) {
            Field field = fields.get(i);
            ExcelField excelField = field.getAnnotation(ExcelField.class);
            String fieldName = field.getName();
            if(excelField != null && StringUtils.isNotBlank(excelField.column())){
                fieldName = excelField.column();
            }
            Cell cellX = headRow.createCell(i, Cell.CELL_TYPE_STRING);
            cellX.setCellStyle(headStyle);
            cellX.setCellValue(String.valueOf(fieldName));
        }
        //data
        for (int dataIndex = 0; dataIndex < dataList.size(); dataIndex++) {
            int rowIndex = dataIndex+1;
            Object rowData = dataList.get(dataIndex);
            Row rowX = sheet.createRow(rowIndex);
            for (int i = 0; i < fields.size(); i++) {
                Field field = fields.get(i);
                try {
                    field.setAccessible(true);
                    Object fieldValue = field.get(rowData);
                    Cell cellX = rowX.createCell(i, Cell.CELL_TYPE_STRING);
                    cellX.setCellValue(generateValue(fieldValue));
                } catch (IllegalAccessException e) {
                    logger.error(e.getMessage(), e);
                    throw new RuntimeException(e);
                }
            }
        }
    }
    /**
     *
     * @param obj
     * @return
     */
    private static String generateValue(Object obj){
        if(obj == null) return StringUtils.EMPTY;
        if(obj instanceof Date){
            return df.format(obj);
        }
        return String.valueOf(obj);
    }
    /**
     * 生成head样式
     * @param wb
     * @return
     */
    private static CellStyle generateHeadStyle(Workbook wb){
        CellStyle style = wb.createCellStyle();
        /*****************************使用默认颜色**************************************************/
        // 填充色
        style.setFillForegroundColor(IndexedColors.ORANGE.getIndex());
        style.setFillPattern(CellStyle.BIG_SPOTS);
        // 字体颜色
        Font font = wb.createFont();
        font.setColor(IndexedColors.BLACK.getIndex());
        style.setFont(font);
        return style;
    }
}
```
controller：
```java
@RequestMapping(value = "/download", method = RequestMethod.GET)
    public ModelAndView download(@RequestParam String beginTime,
                                 @RequestParam String endTime) {
        List<MpPrizeExportVo> result = manageFade.export(beginTime,endTime);
        Map<String,Object> map = new HashMap<>();
        map.put("fileName","数据下载-"+beginTime+"-"+endTime);
        map.put("data",result);
        return new ModelAndView(new MpManageExcelView(),map);
    }
```
