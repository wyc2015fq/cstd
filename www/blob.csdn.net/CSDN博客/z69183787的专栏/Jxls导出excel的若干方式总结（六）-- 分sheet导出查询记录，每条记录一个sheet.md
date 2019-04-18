# Jxls导出excel的若干方式总结（六）-- 分sheet导出查询记录，每条记录一个sheet - z69183787的专栏 - CSDN博客
2016年05月14日 17:43:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1951
**el表达式。注意：suplyArea后跟的是其对应实体类的属性名称。**
![](http://hi.csdn.net/attachment/201108/31/0_13147829654hSV.gif)
```java
/*
* 分sheet导出查询记录
*/
			SupplyArea obj = new SupplyArea();
			List suplyAreaList = saBiz.getRecords(qcList, obj);			
			String templateDir = "d:/excel/template/SupplyAreaMultipleSheets.xls";
			String targetDir="d:/excel/export/testMultipleSheets.xls";
			List sheetNames = new ArrayList();
			for(int i=0;i<suplyAreaList.size();i++){
				SupplyArea sa = (SupplyArea)suplyAreaList.get(i);
				sheetNames.add(sa.getSupplyAreaName());
			}
			InputStream is = new BufferedInputStream(new FileInputStream(templateDir));
			XLSTransformer transformer = new XLSTransformer();
			HSSFWorkbook resultWorkBook = transformer.transformMultipleSheetsList(is, suplyAreaList, sheetNames, "suplyArea", new HashMap(), 0);
			OutputStream os = new BufferedOutputStream(new FileOutputStream(targetDir));
			resultWorkBook.write(os);
```
