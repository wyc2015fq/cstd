# Java使用Apache POI导出-注意前端不要使用Ajax异步请求 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年06月27日 15:44:49[boonya](https://me.csdn.net/boonya)阅读数：828








Java使用Apache POI导出-注意前端不要使用Ajax异步请求，如果你想弹出文件下载框，请勿使用异步下载。

## Apahce POI pom配置



```
<!--EXCEL导入导出 -->
		<dependency>
			<groupId>org.apache.poi</groupId>
			<artifactId>poi</artifactId>
			<version>3.9</version>
		</dependency>

		<dependency>
			<groupId>org.apache.poi</groupId>
			<artifactId>poi-ooxml</artifactId>
			<version>3.9</version>
		</dependency>

		<dependency>
			<groupId>org.apache.poi</groupId>
			<artifactId>poi-ooxml-schemas</artifactId>
			<version>3.9</version>
		</dependency>
```





## ExoportExcelUtil



```java
package com.boonya.common.utils.excel;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;
import java.util.Map;
import javax.servlet.http.HttpServletResponse;
import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFRichTextString;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.hssf.util.CellRangeAddress;
import org.apache.poi.xssf.usermodel.XSSFCell;
import org.apache.poi.xssf.usermodel.XSSFCellStyle;
import org.apache.poi.xssf.usermodel.XSSFRichTextString;
import org.apache.poi.xssf.usermodel.XSSFRow;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;
import com.cvnavi.common.utils.DateUtil;

@SuppressWarnings("deprecation")
public class ExportExcelUtil {

	public static String getFileName() {
		return DateUtil.toString(new Date(), "yyyyMMdd-HHmmss");
	}

	public static XSSFWorkbook getWorkbook(List<?> resultList, String[] showName, String[] resourceField,
			Class<?> resultObj, Map<String, Map<String, String>> formatMap) throws SecurityException,
			NoSuchMethodException, IllegalArgumentException, IllegalAccessException, InvocationTargetException {
		XSSFWorkbook workbook = new XSSFWorkbook();
		XSSFSheet sheet = workbook.createSheet("sheet1");
		sheet.setDefaultColumnWidth((short) 20);
		XSSFCellStyle centerStyle = workbook.createCellStyle();// 设置为水平居中
		centerStyle.setAlignment(XSSFCellStyle.ALIGN_CENTER);
		centerStyle.setVerticalAlignment(XSSFCellStyle.VERTICAL_CENTER);
		XSSFRow row;
		XSSFCell cell;
		createTitle(showName, sheet);
		// 下面是输出各行的数据 下面是输出各行的数据 下面是输出各行的
		for (int i = 0, len = resultList.size(); i < len; i++) {
			Object result = resultList.get(i);
			row = sheet.createRow(i + 1);
			// 创建第 i+1 行
			for (int j = 0; j < resourceField.length; j++) {
				cell = row.createCell(j);// 创建第 j 列
				Method method;
				method = resultObj.getMethod(resourceField[j]);
				// 这里用到了反射机制，通 这里用到了反射机制， 这里用到了反射机制 过方法名来取得对应方法返回的结果对象
				Object obj = method.invoke(result);
				if (obj != null) {
					if (formatMap != null && formatMap.containsKey(resourceField)) {
						cell.setCellValue(formatMap.get(resourceField).get(obj.toString()));
					} else {
						cell.setCellValue(obj.toString());
					}
				}
			}
		}
		return workbook;
	}

	/**
	 * 
	 * @Title: getWorkbook2 @Description: TODO(创建HSSFWorkbook) @param
	 * resultList @param showName @param resourceField @param resultObj @param
	 * formatMap @return @throws SecurityException @throws
	 * NoSuchMethodException @throws IllegalArgumentException @throws
	 * IllegalAccessException @throws InvocationTargetException 设定文件 @return
	 * HSSFWorkbook 返回类型 @author: pengjl @date 2017年6月27日 下午3:22:51 @throws
	 */
	public static HSSFWorkbook getWorkbook2(List<?> resultList, String[] showName, String[] resourceField,
			Class<?> resultObj, Map<String, Map<String, String>> formatMap) throws SecurityException,
			NoSuchMethodException, IllegalArgumentException, IllegalAccessException, InvocationTargetException {
		HSSFWorkbook workbook = new HSSFWorkbook();
		HSSFSheet sheet = workbook.createSheet("sheet1");
		sheet.setDefaultColumnWidth((short) 20);
		HSSFCellStyle centerStyle = workbook.createCellStyle();// 设置为水平居中
		centerStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);
		centerStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);
		HSSFRow row;
		HSSFCell cell;
		createTitle2(showName, sheet);
		// 下面是输出各行的数据 下面是输出各行的数据 下面是输出各行的
		for (int i = 0; i < resultList.size(); i++) {
			Object result = resultList.get(i);
			row = sheet.createRow(i + 1);
			// 创建第 i+1 行
			for (int j = 0; j < resourceField.length; j++) {
				cell = row.createCell(j);// 创建第 j 列
				Method method;
				method = resultObj.getMethod(resourceField[j]);
				// 这里用到了反射机制，通 这里用到了反射机制， 这里用到了反射机制 过方法名来取得对应方法返回的结果对象
				Object obj = method.invoke(result);
				if (obj != null) {
					if (formatMap != null && formatMap.containsKey(resourceField)) {
						cell.setCellValue(formatMap.get(resourceField).get(obj.toString()));
					} else {
						cell.setCellValue(obj.toString());
					}
				}
			}
		}
		return workbook;
	}

	/**
	 * @Title: createTitle2 @Description: TODO(多行表头) @param showName @param
	 * sheet 设定文件 @return void 返回类型 @author: pengjl @date 2017年6月27日
	 * 下午3:23:30 @throws
	 */
	private static void createTitle2(String[] showName, HSSFSheet sheet) {
		HSSFRow row = sheet.createRow(0); // 创建第 1 行，也就是输出表头 创建第
		HSSFCell cell;
		for (int i = 0; i < showName.length; i++) {
			cell = row.createCell(i);
			// 创建第 i 列 创建第
			cell.setCellValue(new HSSFRichTextString(showName[i]));
		}
	}

	private static void createTitle(String[] showName, XSSFSheet sheet) {
		XSSFRow row = sheet.createRow(0); // 创建第 1 行，也就是输出表头 创建第
		XSSFCell cell;
		for (int i = 0; i < showName.length; i++) {
			cell = row.createCell(i);
			// 创建第 i 列 创建第
			cell.setCellValue(new XSSFRichTextString(showName[i]));
		}
	}

	/**
	 * @Title: createWorkbook @Description: TODO(创建HSSFWorkbook) @param
	 * resultList @param showName @return 设定文件 @return HSSFWorkbook
	 * 返回类型 @author: pengjl @date 2017年6月27日 下午3:23:41 @throws
	 */
	public static HSSFWorkbook createWorkbook(List<List<Cell>> resultList, String[] showName) {
		HSSFWorkbook workbook = new HSSFWorkbook();
		HSSFSheet sheet = workbook.createSheet("sheet1");
		HSSFCellStyle centerStyle = workbook.createCellStyle();// 设置为水平居中
		centerStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);
		centerStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);
		createTitle2(showName, sheet);
		HSSFRow row = null;
		HSSFCell cell = null;
		int[][] arraSort;
		if (resultList.size() == 0) {
			arraSort = new int[0][0];
		} else {
			arraSort = new int[resultList.get(0).size()][resultList.size()];
		}
		for (int i = 0; i < resultList.size(); i++) {
			row = sheet.createRow(i + 1);
			// sheet.setColumnWidth(i + 1, 15);
			List<Cell> cellList = resultList.get(i);
			for (int j = 0; j < cellList.size(); j++) {
				cell = row.createCell(j);// 创建第 j 列
				cell.setCellValue(cellList.get(j).getValue());
				int b = cell.getStringCellValue().getBytes().length;
				arraSort[j][i] = b;
				if (cellList.get(j).getStyle() != null) {
					cell.setCellStyle(cellList.get(j).getStyle());
				}
			}
		}
		// 列的最大列宽值（不包括标题）
		int widthInfo[] = TwoMaxInfo(arraSort);
		// 与标题在比较列宽
		for (int i = 0; i < showName.length; i++) {
			// sheet.autoSizeColumn(i);
			// 算出列（包括标题的最大列宽）
			int maxWidthInfo = showName[i].getBytes().length > widthInfo[i] ? showName[i].getBytes().length
					: widthInfo[i];
			sheet.setColumnWidth(i, maxWidthInfo * 256);
		}
		return workbook;
	}

	public static int[] TwoMaxInfo(int[][] arraSort) {
		int[] arraySortInfo = null;
		arraySortInfo = new int[arraSort.length];
		int count = 0;
		for (int[] is : arraSort) {
			int[] arraInfo = is;
			Arrays.sort(arraInfo);
			arraySortInfo[count] = arraInfo[arraInfo.length - 1];
			count++;
		}
		return arraySortInfo;
	}

	/**
	 * @Title: createWorkbookAll @Description: TODO(创建HSSFWorkbook) @param
	 * vMap @param showName @return 设定文件 @return HSSFWorkbook 返回类型 @author:
	 * pengjl @date 2017年6月27日 下午3:20:04 @throws
	 */
	public static HSSFWorkbook createWorkbookAll(Map<String, List<List<Cell>>> vMap, String[] showName) {
		HSSFWorkbook workbook = new HSSFWorkbook();
		for (Map.Entry<String, List<List<Cell>>> entry : vMap.entrySet()) {
			HSSFSheet sheet = workbook.createSheet(entry.getKey());
			sheet.setDefaultColumnWidth((short) 15);
			HSSFCellStyle centerStyle = workbook.createCellStyle();// 设置为水平居中
			centerStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);
			centerStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);
			createTitle2(showName, sheet);
			HSSFRow row;
			HSSFCell cell;
			for (int i = 0; i < entry.getValue().size(); i++) {
				row = sheet.createRow(i + 1);
				List<Cell> cellList = entry.getValue().get(i);
				for (int j = 0; j < cellList.size(); j++) {
					cell = row.createCell(j);// 创建第 j 列
					cell.setCellValue(cellList.get(j).getValue());
					if (cellList.get(j).getStyle() != null) {
						cell.setCellStyle(cellList.get(j).getStyle());
					}
				}
			}
			for (int i = 0; i < showName.length; i++) {
				sheet.autoSizeColumn(i);
			}
		}
		return workbook;
	}

	/**
	 * @Title: workbook2InputStream @Description: TODO(导出Excel文件) @param
	 * workbook @param fileName @param response @return @throws Exception
	 * 设定文件 @return InputStream 返回类型 @author: pengjl @date 2017年6月27日
	 * 下午3:20:21 @throws
	 */
	public static InputStream workbook2InputStream(HSSFWorkbook workbook, String fileName, HttpServletResponse response)
			throws Exception {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		workbook.write(baos);
		baos.flush();
		byte[] aa = baos.toByteArray();
		InputStream excelStream = new ByteArrayInputStream(aa, 0, aa.length);
		response.setContentType("application/vnd.ms-excel;charset=UTF-8");
		response.setHeader("Content-Disposition", "attachment;filename=" + fileName + ".xls");
		response.getOutputStream().write(aa);
		response.setContentLength(aa.length);
		response.getOutputStream().flush();
		baos.close();
		return excelStream;

	}

	/**
	 * 
	 * @Title: workbook2InputStream @Description: TODO(导出Excel文件) @param
	 * workbook @param fileName @param response @return @throws Exception
	 * 设定文件 @return InputStream 返回类型 @author: pengjl @date 2017年6月27日
	 * 下午3:20:49 @throws
	 */
	public static InputStream workbook2InputStream(XSSFWorkbook workbook, String fileName, HttpServletResponse response)
			throws Exception {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		workbook.write(baos);
		baos.flush();
		byte[] aa = baos.toByteArray();
		InputStream excelStream = new ByteArrayInputStream(aa, 0, aa.length);
		response.setContentType("application/vnd.ms-excel;charset=UTF-8");
		response.setHeader("Content-Disposition", "attachment;filename=" + fileName + ".xlsx");
		response.getOutputStream().write(aa);
		response.setContentLength(aa.length);
		response.getOutputStream().flush();
		baos.close();
		return excelStream;

	}

	/**
	 * @Title: createWorkbookVarietyParam @Description:
	 * TODO(创建HSSFWorkbook) @param resultList @param showName @param
	 * headerName @return 设定文件 @return HSSFWorkbook 返回类型 @author: pengjl @date
	 * 2017年6月27日 下午3:21:22 @throws
	 */
	public static HSSFWorkbook createWorkbookVarietyParam(ArrayList<ArrayList<String>> resultList, String[] showName,
			ArrayList<String> headerName) {
		HSSFWorkbook workbook = new HSSFWorkbook();
		HSSFSheet sheet = workbook.createSheet("sheet1");
		sheet.setDefaultColumnWidth((short) 15);
		HSSFCellStyle centerStyle = workbook.createCellStyle();// 设置为水平居中
		centerStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);
		centerStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);
		/**
		 * 设置表头的样式
		 */
		HSSFCellStyle titylStyle = workbook.createCellStyle();
		createTitleVariety(showName, headerName, sheet, titylStyle);
		HSSFRow row;
		HSSFCell cell;
		if (resultList != null && resultList.size() > 0) {
			for (int i = 0; i < resultList.size(); i++) {
				ArrayList<String> rowResultList = resultList.get(i);
				if (headerName != null && headerName.size() > 0) {
					row = sheet.createRow((short) (i + 1 + headerName.size()));
				} else {
					row = sheet.createRow((short) (i + 1));
				}
				if (rowResultList != null && rowResultList.size() > 0) {

					for (int n = 0; n <= rowResultList.size(); n++) {
						cell = row.createCell(n);// 创建第 j 列
						cell.setCellStyle(centerStyle);
						if (n == 0) {
							// 为Excel表的第一列添加编号，表头为：序号；eg:1,2,3,4……
							cell.setCellValue(i + 1);
						} else if (rowResultList.get(n - 1) != null) {
							cell.setCellValue(rowResultList.get(n - 1).toString());
						} else {
							cell.setCellValue("");
						}
					}

				}
			}
		}

		return workbook;
	}

	/**
	 * @Title: createTitleVariety @Description: TODO(多行表头) @param
	 * showName @param headerName @param sheet @param titylStyle 设定文件 @return
	 * void 返回类型 @author: pengjl @date 2017年6月27日 下午3:24:24 @throws
	 */
	private static void createTitleVariety(String[] showName, ArrayList<String> headerName, HSSFSheet sheet,
			HSSFCellStyle titylStyle) {
		HSSFRow row;
		HSSFCell cell;
		titylStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);
		titylStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);
		if (headerName != null && headerName.size() > 0) {
			for (int i = 0; i < headerName.size(); i++) {
				row = sheet.createRow((short) i);
				if (i == 0) {
					cell = row.createCell(i);
					sheet.addMergedRegion(new CellRangeAddress(i, i, (short) 0, (short) showName.length));
					cell.setCellStyle(titylStyle);
					if (headerName.get(i) != null) {
						cell.setCellValue(new HSSFRichTextString(headerName.get(i).toString()));
					} else {
						cell.setCellValue(new HSSFRichTextString(""));
					}
				} else {
					cell = row.createCell(i - 1);
					sheet.addMergedRegion(new CellRangeAddress(i, i, (short) 0, (short) showName.length));
					if (headerName.get(i) != null) {
						cell.setCellValue(new HSSFRichTextString(headerName.get(i).toString()));
					} else {
						cell.setCellValue(new HSSFRichTextString(""));
					}
				}
			}
		}
		// 设置Excel字段
		if (headerName != null && headerName.size() > 0) {
			row = sheet.createRow((short) headerName.size());
		} else {
			row = sheet.createRow(0);
		}
		for (int n = 0; n <= showName.length; n++) {
			if (n == 0) {
				cell = row.createCell(n);
				cell.setCellStyle(titylStyle);
				cell.setCellValue(new HSSFRichTextString("序号"));
			} else {
				cell = row.createCell(n);
				cell.setCellStyle(titylStyle);
				cell.setCellValue(new HSSFRichTextString(showName[n - 1]));
			}
		}
	}
}
```




## 使用方式



```java
/**
	* @Title: export
	* @Description: TODO(单车导出&&导出前端Ajax请求不会弹出下载文件)
	* @param request
	* @param response
	* @param temperatureBean
	* @return    设定文件
	* @return String    返回类型
	* @author: pengjl
	* @date 2017年6月22日 下午8:38:43
	* @throws
	 */
	@RequestMapping("/export")
	public void export(HttpServletRequest request,HttpServletResponse response,TemperatureBean temperatureBean){
	    int mapType = Integer.parseInt(request.getParameter("mapType"));
        try {
            List<TemperatureView> viewList= iTemperatureService.queryTrack(temperatureBean, mapType,true);
            String[] showName = {"车牌号","GPS时间","速度(公里/时)","海拔(米)","位置","温度"};
            List<List<Cell>> dataList = new ArrayList<List<Cell>>();
            List<Cell> cellList = new ArrayList<Cell>();
            if(viewList.size()>0){
	            for (TemperatureView view : viewList) {
	                cellList = new ArrayList<Cell>();
	                cellList.add(new Cell(view.getTemperatureList().get(0).getPlateCode()));
	                cellList.add(new Cell(view.getGpsTime()));
	                cellList.add(new Cell(Double.parseDouble(view.getTemperatureList().get(0).getSpeed()+"")/10 + ""));
	                cellList.add(new Cell(view.getTemperatureList().get(0).getHigh()+""));
	                cellList.add(new Cell(view.getTemperatureList().get(0).getLocation()));
	                String temp = "" ;
	                for(TemperatureBean bean: view.getTemperatureList()){
	                    temp += bean.getAppendName()+":" + bean.getTemperature() + bean.getUnit() + "," ;
	                }
	                if(!temp.equals(""))
	                    temp = temp.substring(0,temp.length()-1);
	                cellList.add(new Cell(temp));
	                dataList.add(cellList);
	            }
            }
            String fileName = ExportExcel.getFileName();
            ExportExcel.workbook2InputStream(ExportExcel.createWorkbook(dataList, showName), fileName,response);
        } catch (Exception e) {
            e.printStackTrace();
        }
	}
```




## 测试URL

在浏览器中输入导出地址：http://127.0.0.1:8081/cvnavi-monitoryt/temperature/export.do?tokenStr=A2E4CE20DBC8A4EDAC0EC5B2AD25D022&startTime=2017-06-27%2000:00:00&endTime=2017-06-27%2023:59:59&plateCode=%E6%B2%AAAWG102&mapType=2&vehicleId=4427355843467897

![](https://img-blog.csdn.net/20170627154319988)


通过直接访问可以下载，故推测前端反映的无法导出是因为其使用了Ajax，文件本身是异步方式，所以浏览器不会弹出下载框。













