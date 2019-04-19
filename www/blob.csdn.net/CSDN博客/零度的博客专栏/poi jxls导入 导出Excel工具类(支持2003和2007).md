# poi/jxls导入/导出Excel工具类(支持2003和2007) - 零度的博客专栏 - CSDN博客
2016年07月06日 10:48:38[零度anngle](https://me.csdn.net/zmx729618)阅读数：3683
1、ExportUtil导出工具类（根据模板导出），支持2003/2007不同excel格式文件
```
package org.nercita.bcp.util;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URLEncoder;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import net.sf.jxls.transformer.XLSTransformer;
import org.apache.commons.io.IOUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFFont;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.openxml4j.exceptions.InvalidFormatException;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.util.CellRangeAddress;
import org.apache.poi.xssf.usermodel.XSSFCellStyle;
import org.apache.poi.xssf.usermodel.XSSFFont;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;
import org.springframework.core.io.ClassPathResource;
import org.springframework.util.Assert;
/**
 * 导出工具类
 */
public class ExportUtil {
    public static String EXCEL_2007 = "Excel2007";
    public static String EXCEL_2003 = "Excel2003";
    private volatile Properties props = new Properties();
    private boolean inited = false;
    private volatile static ExportUtil util = new ExportUtil();
    private ExportUtil() {
        init();
    }
    public Properties getProps() {
        if (!inited)
            init();
        return props;
    }
    public String getProp(String key) {
        if (!inited)
            init();
        return props.getProperty(key);
    }
    public String getProp(String key, String defaultValue) {
        if (!inited)
            init();
        return props.getProperty(key, defaultValue);
    }
    private void init() {
        try {
            InputStream is = new ClassPathResource("export.properties").getInputStream();
            props.load(is);
            is.close();
            inited = true;
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public static ExportUtil getUtil() {
        if (util == null)
            util = new ExportUtil();
        return util;
    }
    /**
     * 设置下载文件中文件的名称
     *
     * @param pFileName
     * @param request
     * @return
     */
    public static String encodeFilename(String pFileName, HttpServletRequest request){
    	String filename = null;    
        String agent = request.getHeader("USER-AGENT");
    	try {
	    	    
	        if (null != agent){    
	            if (-1 != agent.indexOf("Firefox")) {//Firefox    
	                filename = "=?UTF-8?B?" + (new String(org.apache.commons.codec.binary.Base64.encodeBase64(pFileName.getBytes("UTF-8"))))+ "?=";    
	            }else if (-1 != agent.indexOf("Chrome")) {//Chrome    
	                filename = new String(pFileName.getBytes(), "ISO8859-1");    
	            } else {//IE7+    
	                filename = URLEncoder.encode(pFileName, "UTF-8");    
	                filename = StringUtils.replace(filename, "+", "%20");//替换空格    
	            }    
	        } else {    
	            filename = pFileName;    
	        }    
	       
    	} catch (Exception ex) {
    	}
    	 return filename;
   }
    //导出
    public static void writeResponse(String fileName, HttpServletRequest request, HttpServletResponse response, 
    		String version, String templatePath, Map<String, Object> beanParams,Map<Integer,List<CellFill>> mapFill) {
        writeResponse(fileName, request, response, version, templatePath, beanParams, false, null, null, mapFill);
    }
    
    public static void writeResponse(String fileName, HttpServletRequest request, HttpServletResponse response, 
    		String version, String templatePath, Map<String, Object> beanParams) {
        writeResponse(fileName, request, response, version, templatePath, beanParams, false, null, null, null);
    }
    public static void writeResponse(String fileName, HttpServletRequest request, HttpServletResponse response, String version, String templatePath, Map<String, Object> beanParams,
                                     boolean innerCollectionAccess) {
        writeResponse(fileName, request, response, version, templatePath, beanParams, innerCollectionAccess, null, null, null);
    }
    public static void writeResponse(String fileName, HttpServletRequest request, HttpServletResponse response, String version, String templatePath, Map<String, Object> beanParams,
                                     boolean innerCollectionAccess, Map<Integer, List<CellRangeAddress>> rangeMap) {
        writeResponse(fileName, request, response, version, templatePath, beanParams, innerCollectionAccess, rangeMap, null, null);
    }
    public static void writeResponse(String fileName, HttpServletRequest request, HttpServletResponse response, String version, String templatePath, Map<String, Object> beanParams,
                                     Map<Integer, List<CellRangeAddress>> rangeMap, XLSTransformer transformer) {
        writeResponse(fileName, request, response, version, templatePath, beanParams, false, rangeMap, transformer, null);
    }
    //
    public static void writeResponse(String fileName, HttpServletRequest request, HttpServletResponse response, 
    		String version, String templatePath, Map<String, Object> beanParams,
    		boolean innerCollectionAccess, Map<Integer, List<CellRangeAddress>> rangeMap, XLSTransformer trans,
    		Map<Integer,List<CellFill>> mapFill) {
        Assert.isTrue(StringUtils.isNotBlank(templatePath), "templatePath不可以为空字符串");
        XLSTransformer transformer = null;
        if (trans != null) {
            transformer = trans;
        } else {
            transformer = new XLSTransformer();
        }
        transformer.setJexlInnerCollectionsAccess(innerCollectionAccess);
        response.setCharacterEncoding("utf-8");
        if (EXCEL_2007.equals(version)) {
            fileName = encodeFilename(fileName + ".xlsx", request);
            response.setContentType("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet;charset=UTF-8");
        } else {
            fileName = encodeFilename(fileName + ".xls", request);
            response.setContentType("application/vnd.ms-excel;charset=UTF-8");
        }
        response.setHeader("Content-Disposition", "attachment;fileName=" + fileName);
        InputStream is = null;
        try {
            if("linux".equals(System.getProperty("os.name").toLowerCase())){
                if(!templatePath.startsWith("/")){
                    templatePath="/"+templatePath;
                }
            }
            is = new FileInputStream(templatePath);
            Workbook book = transformer.transformXLS(is, beanParams);
            if (rangeMap != null) {
                Iterator<Integer> index = rangeMap.keySet().iterator();
                while (index.hasNext()) {
                    Integer i = index.next();
                    Sheet sheet = book.getSheetAt(i);
                    Iterator<CellRangeAddress> cra = rangeMap.get(i).iterator();
                    while (cra.hasNext()) {
                        sheet.addMergedRegion(cra.next());
                    }
                }
            }
            
            //==========填充颜色，add by psh 
           if(mapFill!=null) {
        	   if (EXCEL_2003.equals(version)) {
        		   HSSFWorkbook workbook=(HSSFWorkbook)book;
        		   Iterator<Integer> sheetIndex = mapFill.keySet().iterator();
                   while (sheetIndex.hasNext()) {
                       Integer index = sheetIndex.next();
        			   Sheet sheet = workbook.getSheetAt(index);
        			   List<CellFill> list = mapFill.get(index);
        			   CellFill cellFill= null;
        			   for(int i=0;i<list.size();i++){
        				   cellFill= list.get(i);
        				   Row row = (Row)sheet.getRow(cellFill.getRow());
        				   if(row==null){
        					   continue;
        				   }
                           Cell cell = row.getCell(cellFill.getCol());
                           if(cell==null){
        					   continue;
        				   }
                           HSSFCellStyle sheetStyle = workbook.createCellStyle();     //Sheet样式
                           sheetStyle.setFillPattern(XSSFCellStyle.SOLID_FOREGROUND); //填充模式
                           sheetStyle.setFillForegroundColor(cellFill.getBgColor()); //前景色(单元格）的设定。HSSFColor.YELLOW.index——黄色
                           HSSFFont font=workbook.createFont();
                           font.setColor(cellFill.getFontColor());
                           sheetStyle.setFont(font);                 
                           cell.setCellStyle(sheetStyle);
        			   }
        		   }//end while
               }else {
            	   XSSFWorkbook workbook=(XSSFWorkbook)book;
            	   Iterator<Integer> sheetIndex = mapFill.keySet().iterator();
                   while (sheetIndex.hasNext()) {
                	   Integer index = sheetIndex.next();
        			   Sheet sheet = workbook.getSheetAt(index);
        			   List<CellFill> list = mapFill.get(index);
        			   CellFill cellFill= null;
        			   for(int i=0;i<list.size();i++){
        				   cellFill= list.get(i);
        				   Row row = (Row)sheet.getRow(cellFill.getRow());
        				   if(row==null){
        					   continue;
        				   }
                           Cell cell = row.getCell(cellFill.getCol());
                           if(cell==null){
        					   continue;
        				   }
                           XSSFCellStyle sheetStyle = workbook.createCellStyle();     //Sheet样式
                           sheetStyle.setFillPattern(XSSFCellStyle.SOLID_FOREGROUND); //填充模式
                           sheetStyle.setFillForegroundColor(cellFill.getBgColor()); //前景色(单元格）的设定。HSSFColor.YELLOW.index——黄色
                           XSSFFont font=workbook.createFont();
                           font.setColor(cellFill.getFontColor());
                           sheetStyle.setFont(font);                 
                           cell.setCellStyle(sheetStyle);
        			   }
        		   }//end while
               } //end 2007 	   
           }           
           //=====end 颜色===========
            
		   book.write(response.getOutputStream());				    
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InvalidFormatException e) {
            e.printStackTrace();
        } finally {
            IOUtils.closeQuietly(is);
        }
    }
    public static String getProperty(String property) {
        return getUtil().getProp(property);
    }
}
```
2、ImportUtil.java工具类，支持2003和2007不同格式excel文件。
```
package org.nercita.bcp.util;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;
import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFDateUtil;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;
import org.springframework.web.multipart.MultipartFile;
/**
 * excel文件导入工具类
 */
public class ImportUtil {
	
    public static String EXCEL_2007_ContentType = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    public static String EXCEL_2003_ContentType = "application/vnd.ms-excel";
	
	/**
	 * excel文件导入
	 * @param file
	 * @param ignoreRows 需要忽略掉的行数
	 * @return
	 * @throws FileNotFoundException
	 * @throws IOException
	 */
	public static String[][] importFile(MultipartFile file, int ignoreRows) throws FileNotFoundException, IOException {
		List<String[]> result = new ArrayList<String[]>();
		String contentType = file.getContentType();	 //内容类型	
		InputStream stream = file.getInputStream();  //输入流
		Workbook wb=null;
		//根据inputStream建立一个Excel对象
		if (EXCEL_2003_ContentType.equals(contentType)){
			wb =(Workbook) new HSSFWorkbook(stream);	//2003
		}else if(EXCEL_2007_ContentType.equals(contentType)) {
			wb =(Workbook) new XSSFWorkbook(stream);	//2007
		}
		int rowSize = 0;
		Cell cell = null;
		for (int sheetIndex = 0; sheetIndex < wb.getNumberOfSheets(); sheetIndex++) {
			Sheet st = wb.getSheetAt(sheetIndex);
			for (int rowIndex = ignoreRows; rowIndex <= st.getLastRowNum(); rowIndex++) {
				Row row = st.getRow(rowIndex);
				if (null == row) {
					continue;
				}
				int tempRowSize = row.getLastCellNum() + 1;
				if (tempRowSize > rowSize) {
					rowSize = tempRowSize;
				}
				String[] values = new String[rowSize];
				Arrays.fill(values, "");
				boolean hasValue = false;
				for (short columnIndex = 0; columnIndex <= row.getLastCellNum(); columnIndex++) {
					String value = "";
					cell = row.getCell(columnIndex);
					if (null != cell) {
						switch (cell.getCellType()) {
						case HSSFCell.CELL_TYPE_STRING:
							value = cell.getStringCellValue();
							break;
						case HSSFCell.CELL_TYPE_NUMERIC:
							if (HSSFDateUtil.isCellDateFormatted(cell)) {
								Date date = cell.getDateCellValue();
								if (null != date) {
									value = new SimpleDateFormat("yyyy-MM-dd").format(date);
								} else {
									value = "";
								}
							} else {
								DecimalFormat df = new DecimalFormat("0.#######");  								  
								//value = df.format(cell.getNumericCellValue()); 								
								value = String.valueOf(df.format(cell.getNumericCellValue()));
							}
							break;
						case HSSFCell.CELL_TYPE_FORMULA:
							// 公式生成的数据
							if (!"".equals(cell.getStringCellValue())) {
								value = cell.getStringCellValue();
							} else {
								value = cell.getNumericCellValue() + "";
							}
							break;
						case HSSFCell.CELL_TYPE_BLANK:
							break;
						case HSSFCell.CELL_TYPE_ERROR:
							value = "";
							break;
						case HSSFCell.CELL_TYPE_BOOLEAN:
							value = (cell.getBooleanCellValue() == true ? "Y" : "N");
							break;
						default:
							value = "";
						}
					}
					if (columnIndex == 0 && value.trim().equals("")) {
						break;
					}
					values[columnIndex] = rightTrim(value);
					hasValue = true;
					}
				if (hasValue) {
					result.add(values);
				}
			}
		}
		stream.close();
		String[][] returnArray = new String[result.size()][rowSize];
		for (int i = 0; i < returnArray.length; i++) {
			returnArray[i] = (String[]) result.get(i);
		}
		return returnArray;
	}
	
	/**
	 * excel文件导入
	 * @param file
	 * @param ignoreRows 需要忽略掉的行数
	 * @return
	 * @throws FileNotFoundException
	 * @throws IOException
	 */
	public static String[][] importFile(MultipartFile file,int sheetIndex, int ignoreRows,Integer colCnt) throws FileNotFoundException, IOException {
		List<String[]> result = new ArrayList<String[]>();
		String contentType = file.getContentType();	 //内容类型	
		InputStream stream = file.getInputStream();  //输入流
		Workbook wb=null;
		//根据inputStream建立一个Excel对象
		if (EXCEL_2003_ContentType.equals(contentType)){
			wb =(Workbook) new HSSFWorkbook(stream);	//2003
		}else if(EXCEL_2007_ContentType.equals(contentType)) {
			wb =(Workbook) new XSSFWorkbook(stream);	//2007
		}
		int rowSize = 0;
		Cell cell = null;
		Sheet st = wb.getSheetAt(sheetIndex);
		for (int rowIndex = ignoreRows; rowIndex <= st.getLastRowNum(); rowIndex++) {
			Row row = st.getRow(rowIndex);
			if (null == row) {
				continue;
			}
//			int tempRowSize = row.getLastCellNum() + 1;
			int tempRowSize = row.getPhysicalNumberOfCells();
			
			if(colCnt==null){
				if (tempRowSize > rowSize) {
					rowSize = tempRowSize;
				}
				colCnt=rowSize;
			}
			String[] values = new String[colCnt];
			Arrays.fill(values, "");
			boolean hasValue = false;
			for (short columnIndex = 0; columnIndex < colCnt; columnIndex++) {
				String value = "";
				cell = row.getCell(columnIndex);
				if (null != cell) {
					switch (cell.getCellType()) {
					case HSSFCell.CELL_TYPE_STRING:
						value = cell.getStringCellValue();
						break;
					case HSSFCell.CELL_TYPE_NUMERIC:
						if (HSSFDateUtil.isCellDateFormatted(cell)) {
							Date date = cell.getDateCellValue();
							if (null != date) {
								value = new SimpleDateFormat("yyyy-MM-dd").format(date);
							} else {
								value = "";
							}
						} else {
							DecimalFormat df = new DecimalFormat("0.#######");  								  
							//value = df.format(cell.getNumericCellValue()); 								
							value = String.valueOf(df.format(cell.getNumericCellValue()));
						}
						break;
					case HSSFCell.CELL_TYPE_FORMULA:
						// 公式生成的数据
						if (!"".equals(cell.getStringCellValue())) {
							value = cell.getStringCellValue();
						} else {
							value = cell.getNumericCellValue() + "";
						}
						break;
					case HSSFCell.CELL_TYPE_BLANK:
						break;
					case HSSFCell.CELL_TYPE_ERROR:
						value = "";
						break;
					case HSSFCell.CELL_TYPE_BOOLEAN:
						value = (cell.getBooleanCellValue() == true ? "Y" : "N");
						break;
					default:
						value = "";
					}
				}
				if (columnIndex == 0 && value.trim().equals("")) {
					break;
				}
				values[columnIndex] = rightTrim(value);
				hasValue = true;
				}
			if (hasValue) {
				result.add(values);
			}
		}
			
		stream.close();
		String[][] returnArray = new String[result.size()][colCnt];
		for (int i = 0; i < returnArray.length; i++) {
			returnArray[i] = (String[]) result.get(i);
		}
		return returnArray;
	}
	
	/**
	 * 去除字符串右侧空格(直接使用trim()将不能支持中间存在空格的单元格)
	 * @param str
	 * @return
	 */
	public static String rightTrim(String str) {
		if (str == null) {
			return "";
		}
		int length = str.length();
		for (int i = length - 1; i >= 0; i--) {
			if (0x20 != str.charAt(i)) {
				break;
			}
			length--;
		}
		return str.substring(0, length);
	}
}
```
