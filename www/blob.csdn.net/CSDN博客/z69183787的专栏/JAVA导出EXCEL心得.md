# JAVA导出EXCEL心得 - z69183787的专栏 - CSDN博客
2014年12月01日 13:38:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2281
原文地址：http://blog.csdn.net/lb_mail/article/details/6548045#comments
最近一段时间，一直在做统计报表，其中含有大量复杂的报表的导出工作，对不同的第三方工具（例如POI、JXL和JXLS等）算是略有研究吧，下面就给大家分享一下我的使用心得：
一、POI
对于POI而言，可以说是导出EXCEL的基础，其他的第三方工具都是在其基础上做的封装，但是可操作性上由于要逐行逐列的进行操作，所以可操作性上来说比较繁琐，个人不推荐使用。
二、JXL
JXL丰富了许多的函数来解决POI操作性上的缺陷，但其性能方面值得商榷，下面对JXL在解决复杂报表中遇到的问题进行剖析， 
1、实现不同excel间sheet的拷贝
问题背景：某集团有五十个成员单位，先要求一次性导出所有成员单位的信息到一个EXCEL中，即此EXCEL中含有五十个sheet页签
解决思路：首先，使用循环，将每个成员单位的信息逐个导出，然后将这些单个的EXCEL汇总到最后的汇总EXCEL中 
//导出多个sheet的excel(不同模板)
public CmsException exportMoreSheetExcel(String templatePath,String srcPath,String targetPath,String sheetName,Map map){
CmsException excption = new CmsException();
try {
exportOnlyMemberExcel(templatePath,srcPath,map);
try {
copySheet(srcPath,targetPath,sheetName);
} catch (BiffException e) {
addError(e,excption);
} catch (WriteException e) {
addError(e,excption);
}
} catch (ParsePropertyException e1) {
addError(e1,excption);
} catch (IOException e1) {
addError(e1,excption); 
} 
return excption;
}
//导出某成员单位信息的单个excel
public void exportOnlyMemberExcel(String templatePath,String exportPath,Map map) throws ParsePropertyException, IOException{
// 根据模板生成报表文件
XLSTransformer transformer = new XLSTransformer();
transformer.transformXLS(templatePath, map, exportPath);
}
//不同excel文件中sheet之间的拷贝
public void copySheet(String srcPath,String targetPath,String sheetName) throws IOException, BiffException, WriteException{
Workbook workbook = Workbook.getWorkbook(new FileInputStream(srcPath)); 
Workbook wb = Workbook.getWorkbook(new FileInputStream(targetPath));
// 利用已经创建的Excel工作薄创建新的可写入的Excel工作薄 
WritableWorkbook wwb = Workbook.createWorkbook(new File(targetPath), wb);
boolean isExist = false;
int indexOfWorkbook = 0;
Sheet srcSheet = workbook.getSheet(0);
WritableSheet tarSheet = null;
//判断某sheet名称的sheet页是否已经存在
for(int i=0;i<wwb.getNumberOfSheets();i++){
tarSheet = wwb.getSheet(i);
if(sheetName.equals(tarSheet.getName())){
isExist = true;
indexOfWorkbook = i;
break; 
} 
}
if(!isExist){
wwb.importSheet(sheetName, wwb.getNumberOfSheets(), srcSheet);
}else{
wwb.removeSheet(indexOfWorkbook);
wwb.importSheet(sheetName, indexOfWorkbook, srcSheet);
}
wwb.write();
wwb.close();
}
上面的红色代码WritableWorkbook wwb = Workbook.createWorkbook(new File(targetPath), wb);如果文件太大，将相当耗时，这正是JXL导出的瓶颈所在，如果文件不是很大，由于JXL提供了例如SHEET的拷贝等功能，使用JXL还是很可取的。
2、空指针问题
正如问题所描述的，如果我在导出EXCEL的过程中，先将所有的单个EXCEL使用importSheet拷贝到汇总EXCEL中，然后执行wwb.write();操作，将报空指针异常，必须逐次读取excel，WritableWorkbook wwb = Workbook.createWorkbook(new File(targetPath), wb);
三、JXLS
由于JXLS中使用了模板，再加上Unit工具包中丰富的操作方法，所以使用JXLS导出excel是非常可取的。
1、导出单个EXCEL
//导出某成员单位信息的单个excel
public void exportOnlyMemberExcel(String templatePath,String exportPath,Map map) throws ParsePropertyException, IOException{
// 根据模板生成报表文件
XLSTransformer transformer = new XLSTransformer();
transformer.transformXLS(templatePath, map, exportPath);
}
2、一次性导出所有sheet（必须要有相同模板）
//一次性导出所有sheet页(有相同模板)
public CmsException transformXLSFile(List resultList,String templatePath,String exportPath,List sheetNames){
CmsException exception = new CmsException();
XLSTransformer transformer = new XLSTransformer(); 
HSSFWorkbook wb = null; 
InputStream templateStream = null;
try{
templateStream = new FileInputStream(templatePath);
}catch (FileNotFoundException e){
addError(e,exception);
}
for(int i = 0;i < (int)Math.ceil((double)resultList.size()/200);i++){ 
List subList = new ArrayList(); 
int j = ((i+1)*200 < resultList.size()) ? (i+1)*200 : resultList.size(); 
subList = resultList.subList(i*200,j); 
HSSFWorkbook workBook = transformer.transformMultipleSheetsList(templateStream , subList , sheetNames, "map", new HashMap(), 0);
if(i == 0){ 
for (int k = 0;k < workBook.getNumberOfSheets();k++)
workBook.setSheetName(k, workBook.getSheetName(k), HSSFWorkbook.ENCODING_UTF_16);
wb = workBook; 
}else{ 
for(int k = 0;k < workBook.getNumberOfSheets();k++){ 
HSSFSheet newSheet = wb.createSheet((String) sheetNames.get(i)); 
HSSFSheet srcSheet = workBook.getSheetAt(k);
workBook.setSheetName(k, workBook.getSheetName(k), HSSFWorkbook.ENCODING_UTF_16);
Util.copySheets(newSheet, srcSheet); 
Util.copyPageSetup(newSheet, srcSheet); 
Util.copyPrintSetup(newSheet, srcSheet); 
} 
} 
try{
OutputStream os = new BufferedOutputStream(new FileOutputStream(exportPath)); 
wb.write(os); 
os.flush(); 
os.close(); 
}catch(IOException e){
addError(e,exception); 
} 
}
try {
templateStream.close();
} catch (IOException e) {
addError(e,exception);
} 
return exception;
} 
3、sheet名称乱码问题
以下代码可解决乱码问题：
workBook.setSheetName(k, workBook.getSheetName(k), HSSFWorkbook.ENCODING_UTF_16);
以上就是本人小小的一点心得，希望大家多多交流，谢谢～～
