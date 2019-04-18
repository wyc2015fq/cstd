# Java 使用stringTemplate导出大批量数据excel（百万级） - z69183787的专栏 - CSDN博客
2016年02月25日 14:56:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9851
参考资料：[http://bbs.51cto.com/thread-1074293-1-1.html](http://bbs.51cto.com/thread-1074293-1-1.html)
目前java框架中能够生成excel文件的的确不少，但是，能够生成大数据量的excel框架，我倒是没发现，一般数据量大了都会出现内存溢出，所以，生成大数据量的excel文件要返璞归真，用java的基础技术,IO流来实现。
   如果想用IO流来生成excel文件，必须要知道excel的文件格式内容，相当于生成html文件一样，用字符串拼接html标签保存到文本文件就可以生成一个html文件了。同理，excel文件也是可以的。怎么知道excel的文件格式呢？其实很简单，随便新建一个excel文件，双击打开，然后点击“文件”-》“另存为”，保存的类型为“xml表格”，保存之后用文本格式打开，就可以看到excel的字符串格式一览无遗了。
把下面的xml字符串复制到文本文件，然后保存为xls格式，就是一个excel文件。
```
<?xml version="1.0"?>
<?mso-application progid="Excel.Sheet"?>
<Workbook xmlns="urn:schemas-microsoft-com:office:spreadsheet"
xmlns:o="urn:schemas-microsoft-com:office:office"
xmlns:x="urn:schemas-microsoft-com:office:excel"
xmlns:ss="urn:schemas-microsoft-com:office:spreadsheet"
xmlns:html="http://www.w3.org/TR/REC-html40">
<DocumentProperties xmlns="urn:schemas-microsoft-com:office:office">
  <Created>1996-12-17T01:32:42Z</Created>
  <LastSaved>2000-11-18T06:53:49Z</LastSaved>
  <Version>11.9999</Version>
</DocumentProperties>
<OfficeDocumentSettings xmlns="urn:schemas-microsoft-com:office:office">
  <RemovePersonalInformation/>
</OfficeDocumentSettings>
<ExcelWorkbook xmlns="urn:schemas-microsoft-com:office:excel">
  <WindowHeight>4530</WindowHeight>
  <WindowWidth>8505</WindowWidth>
  <WindowTopX>480</WindowTopX>
  <WindowTopY>120</WindowTopY>
  <AcceptLabelsInFormulas/>
  <ProtectStructure>False</ProtectStructure>
  <ProtectWindows>False</ProtectWindows>
</ExcelWorkbook>
<Styles>
  <Style ss:ID="Default" ss:Name="Normal">
   <Alignment ss:Vertical="Bottom"/>
   <Borders/>
   <Font ss:FontName="宋体" x:CharSet="134" ss:Size="12"/>
   <Interior/>
   <NumberFormat/>
   <Protection/>
  </Style>
</Styles>
<Worksheet ss:Name="Sheet1">
  <Table ss:ExpandedColumnCount="2" ss:ExpandedRowCount="2" x:FullColumns="1"
   x:FullRows="1" ss:DefaultColumnWidth="54" ss:DefaultRowHeight="14.25">
   <Column ss:AutoFitWidth="0" ss:Width="73.5"/>
   <Row>
    <Cell><Data ss:Type="String">zhangzehao</Data></Cell>
    <Cell><Data ss:Type="String">zhangzehao</Data></Cell>
   </Row>
   <Row>
    <Cell><Data ss:Type="String">zhangzehao</Data></Cell>
   </Row>
  </Table>
  <WorksheetOptions xmlns="urn:schemas-microsoft-com:office:excel">
   <Selected/>
   <Panes>
    <Pane>
     <Number>3</Number>
     <ActiveRow>5</ActiveRow>
     <ActiveCol>3</ActiveCol>
    </Pane>
   </Panes>
   <ProtectObjects>False</ProtectObjects>
   <ProtectScenarios>False</ProtectScenarios>
  </WorksheetOptions>
</Worksheet>
<Worksheet ss:Name="Sheet2">
  <Table ss:ExpandedColumnCount="0" ss:ExpandedRowCount="0" x:FullColumns="1"
   x:FullRows="1" ss:DefaultColumnWidth="54" ss:DefaultRowHeight="14.25"/>
  <WorksheetOptions xmlns="urn:schemas-microsoft-com:office:excel">
   <ProtectObjects>False</ProtectObjects>
   <ProtectScenarios>False</ProtectScenarios>
  </WorksheetOptions>
</Worksheet>
<Worksheet ss:Name="Sheet3">
  <Table ss:ExpandedColumnCount="0" ss:ExpandedRowCount="0" x:FullColumns="1"
   x:FullRows="1" ss:DefaultColumnWidth="54" ss:DefaultRowHeight="14.25"/>
  <WorksheetOptions xmlns="urn:schemas-microsoft-com:office:excel">
   <ProtectObjects>False</ProtectObjects>
   <ProtectScenarios>False</ProtectScenarios>
  </WorksheetOptions>
</Worksheet>
</Workbook>
```
如果要生成千万级别以上的excel，除了这个关键点之外，还要控制IO流，如果有1000万记录，要迭代1000万次组装xml字符串，这样肯定占用相当大的内存，肯定内存溢出，所以，必须把组装的xml字符串分批用IO流刷新到硬盘里，如果是在web应用中，可以刷新到response中，web应用会自动把临时流保存到客户端的临时文件中，然后再一次性复制到你保存的路径。言归正传，分批刷新的话，可以迭代一批数据就flush进硬盘，同时把list，大对象赋值为空，显式调用垃圾回收器，表明要回收内存。这样的话，不管生成多大的数据量都不会出现内存溢出的，我曾经试过导出1亿的excel文件，都不会出现内存溢出，只是用了35分钟。
  当然，如果要把实现做的优雅一些，在组装xml字符串的时候，可以结合模板技术来实现，我个人喜好stringtemplate这个轻量级的框架，我给出的DEMO也是采用了模板技术生成的，当然velocity和freemarker都是可以，stringbuilder也行，呵呵。
   我为人比较懒，本意不是为了写个帖子的，只是想多赚点下载豆:lol1 ，这和赚钱一样谁不想？谁知道就写了那么多。同时鄙人知识寡陋，希望可以抛砖引玉。
综上：使用技术为 stringTemplate 
pom.xml：
```
<dependency>
            <groupId>antlr</groupId>
            <artifactId>antlr</artifactId>
            <version>2.7.7</version>
        </dependency>
        <dependency>
            <groupId>org.antlr</groupId>
            <artifactId>stringtemplate</artifactId>
            <version>3.2.1</version>
        </dependency>
```
template对象：
```java
class Row{
    private List<String> result;
    public List<String> getResult() {
        return result;
    }
    public void setResult(List<String> result) {
        this.result = result;
    }
}
class Worksheet{
	private String sheet;
	
	private int columnNum;
	
	private int rowNum;
    private List<String> title;
	
	private List<Row> rows;
	public String getSheet() {
		return sheet;
	}
	public void setSheet(String sheet) {
		this.sheet = sheet;
	}
	public List<Row> getRows() {
		return rows;
	}
	public void setRows(List<Row> rows) {
		this.rows = rows;
	}
	public int getColumnNum() {
		return columnNum;
	}
	public void setColumnNum(int columnNum) {
		this.columnNum = columnNum;
	}
	public int getRowNum() {
		return rowNum;
	}
	public void setRowNum(int rowNum) {
		this.rowNum = rowNum;
	}
    public List<String> getTitle() {
        return title;
    }
    public void setTitle(List<String> title) {
        this.title = title;
    }
}
```
模版文件（通用）：
excel 头模板
```
<?xml version="1.0"?>
<?mso-application progid="Excel.Sheet"?>
<Workbook xmlns="urn:schemas-microsoft-com:office:spreadsheet"
 xmlns:o="urn:schemas-microsoft-com:office:office"
 xmlns:x="urn:schemas-microsoft-com:office:excel"
 xmlns:ss="urn:schemas-microsoft-com:office:spreadsheet"
 xmlns:html="http://www.w3.org/TR/REC-html40">
 <DocumentProperties xmlns="urn:schemas-microsoft-com:office:office">
  <Created>1996-12-17T01:32:42Z</Created>
  <LastSaved>2013-08-02T09:21:24Z</LastSaved>
  <Version>11.9999</Version>
 </DocumentProperties>
 <OfficeDocumentSettings xmlns="urn:schemas-microsoft-com:office:office">
  <RemovePersonalInformation/>
 </OfficeDocumentSettings>
 <ExcelWorkbook xmlns="urn:schemas-microsoft-com:office:excel">
  <WindowHeight>4530</WindowHeight>
  <WindowWidth>8505</WindowWidth>
  <WindowTopX>480</WindowTopX>
  <WindowTopY>120</WindowTopY>
  <AcceptLabelsInFormulas/>
  <ProtectStructure>False</ProtectStructure>
  <ProtectWindows>False</ProtectWindows>
 </ExcelWorkbook>
 <Styles>
  <Style ss:ID="Default" ss:Name="Normal">
   <Alignment ss:Vertical="Bottom"/>
   <Borders/>
   <Font ss:FontName="宋体" x:CharSet="134" ss:Size="12"/>
   <Interior/>
   <NumberFormat/>
   <Protection/>
  </Style>
 </Styles>
```
body模板：
```
$worksheet:{
 <Worksheet ss:Name="$it.sheet$">
  <Table ss:ExpandedColumnCount="$it.columnNum$" ss:ExpandedRowCount="$it.rowNum$" x:FullColumns="1"
   x:FullRows="1" ss:DefaultColumnWidth="54" ss:DefaultRowHeight="14.25">
   <Row>
   $it.title:{
   <Cell><Data ss:Type="String">$it$</Data></Cell>
   }$
   </Row>
 $it.rows:{
 <Row>
 $it.result:{
 <Cell><Data ss:Type="String">$it$</Data></Cell>
 }$
   </Row>
 }$
  </Table>
 </Worksheet>
}$
```
实际处理类：传入list对象，利用反射获取对象属性名及属性值
```java
long startTimne = System.currentTimeMillis();
        StringTemplateGroup stGroup = new StringTemplateGroup("stringTemplate");
        //写入excel文件头部信息
        StringTemplate head =  stGroup.getInstanceOf("head");
        File file = new File("D:/output2.xls");
        PrintWriter writer = new PrintWriter(new BufferedOutputStream(new FileOutputStream(file)));
        writer.print(head.toString());
        writer.flush();
        int totalRowNum = listWinningRecordDTOList.size();
        int maxRowNum = 60000;
        int sheets = totalRowNum % 60000 == 0 ? (totalRowNum/maxRowNum) : (totalRowNum/maxRowNum +1);
        //excel单表最大行数是65535
        List record = listWinningRecordDTOList;
        List<String> title = new ArrayList<String>();
        List<Method> getMethods = new ArrayList<Method>();
        Class<?> clazz = record.get(0).getClass();
        Field[] fields = clazz.getDeclaredFields();
        if(fields != null && fields.length > 0){
            for(Field field : fields){
                if(!"serialVersionUID".equals(field.getName())) {
                    title.add(field.getName());
                    getMethods.add(clazz.getDeclaredMethod("get" + field.getName().substring(0, 1).toUpperCase() + field.getName().substring(1)));
                }
            }
        }
//        BeanInfo beanInfo=Introspector.getBeanInfo(clazz,Object.class);
//        PropertyDescriptor[] proDescrtptors=beanInfo.getPropertyDescriptors();
//        for(PropertyDescriptor propertyDescriptor : proDescrtptors){
//            title.add(propertyDescriptor.getName());
//            getMethods.add(propertyDescriptor.getReadMethod());
//        }
        int columnLength = title.size();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        //写入excel文件数据信息
        for(int i=0;i<sheets;i++){
            StringTemplate body =  stGroup.getInstanceOf("body");
            Worksheet worksheet = new Worksheet();
            worksheet.setTitle(title);
            worksheet.setSheet(" "+(i+1)+" ");
            worksheet.setColumnNum(columnLength);
            worksheet.setRowNum(maxRowNum+1);
            List<Row> rows = new ArrayList<Row>();
            int startIndex = i*maxRowNum;
            int endIndex = Math.min((i+1)*maxRowNum -1,totalRowNum-1);
            for(int j=startIndex;j<=endIndex;j++){
                Row row = new Row();
                List<String> result = new ArrayList<String>(columnLength);
                for(int n=0;n<columnLength;n++){
                    Object value = getMethods.get(n).invoke(record.get(j));
                    if(value == null){
                        result.add("");
                    }else{
                        if(value instanceof Date){
                            result.add(sdf.format((Date)value));
                        }else{
                            result.add(value.toString());
                        }
                    }
                }
                row.setResult(result);
                rows.add(row);
            }
            worksheet.setRows(rows);
            body.setAttribute("worksheet", worksheet);
            writer.print(body.toString());
            writer.flush();
            rows.clear();
            rows = null;
            worksheet = null;
            body = null;
            Runtime.getRuntime().gc();
            System.out.println("正在生成excel文件的 sheet"+(i+1));
        }
        //写入excel文件尾部
        writer.print("</Workbook>");
        writer.flush();
        writer.close();
        System.out.println("生成excel文件完成");
        long endTime = System.currentTimeMillis();
        System.out.println("用时="+((endTime-startTimne)/1000)+"秒");
```
整理后的公用类：
```java
import org.antlr.stringtemplate.StringTemplate;
import org.antlr.stringtemplate.StringTemplateGroup;
import java.io.*;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
/**
 * Created by Administrator on 2016/2/25.
 */
public class ExcelStUtil {
    public static void export(OutputStream outputStream,List target) throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        long startTime = System.currentTimeMillis();
        StringTemplateGroup stGroup = new StringTemplateGroup("stringTemplate");
        //解决可能发生的中文乱码
        stGroup.setFileCharEncoding("UTF-8");
        //写入excel文件头部信息
        StringTemplate head =  stGroup.getInstanceOf("st/head");
        PrintWriter writer = new PrintWriter(new BufferedOutputStream(outputStream));
        writer.print(head.toString());
        writer.flush();
        int totalRowNum = target.size();
        int maxRowNum = 60000;
        int sheets = totalRowNum % 60000 == 0 ? (totalRowNum/maxRowNum) : (totalRowNum/maxRowNum +1);
        //excel单表最大行数是65535
        List record = target;
        List<String> title = new ArrayList<String>();
        List<Method> getMethods = new ArrayList<Method>();
        Class<?> clazz = record.get(0).getClass();
        Field[] fields = clazz.getDeclaredFields();
        if(fields != null && fields.length > 0){
            for(Field field : fields){
                if(!"serialVersionUID".equals(field.getName())) {
                    title.add(field.getName());
                    getMethods.add(clazz.getDeclaredMethod("get" + field.getName().substring(0, 1).toUpperCase() + field.getName().substring(1)));
                }
            }
        }
//        BeanInfo beanInfo=Introspector.getBeanInfo(clazz,Object.class);
//        PropertyDescriptor[] proDescrtptors=beanInfo.getPropertyDescriptors();
//        for(PropertyDescriptor propertyDescriptor : proDescrtptors){
//            title.add(propertyDescriptor.getName());
//            getMethods.add(propertyDescriptor.getReadMethod());
//        }
        int columnLength = title.size();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        //写入excel文件数据信息
        for(int i=0;i<sheets;i++){
            StringTemplate body =  stGroup.getInstanceOf("st/body");
            Worksheet worksheet = new Worksheet();
            worksheet.setTitle(title);
            worksheet.setSheet(" "+(i+1)+" ");
            worksheet.setColumnNum(columnLength);
            worksheet.setRowNum(maxRowNum+1);
            List<Row> rows = new ArrayList<Row>();
            int startIndex = i*maxRowNum;
            int endIndex = Math.min((i+1)*maxRowNum -1,totalRowNum-1);
            for(int j=startIndex;j<=endIndex;j++){
                Row row = new Row();
                List<String> result = new ArrayList<String>(columnLength);
                for(int n=0;n<columnLength;n++){
                    Object value = getMethods.get(n).invoke(record.get(j));
                    if(value == null){
                        result.add("");
                    }else{
                        if(value instanceof Date){
                            result.add(sdf.format((Date)value));
                        }else{
                            result.add(value.toString());
                        }
                    }
                }
                row.setResult(result);
                rows.add(row);
            }
            worksheet.setRows(rows);
            body.setAttribute("worksheet", worksheet);
            writer.print(body.toString());
            writer.flush();
            rows.clear();
            rows = null;
            worksheet = null;
            body = null;
            Runtime.getRuntime().gc();
            System.out.println("正在生成excel文件的 sheet"+(i+1));
        }
        //写入excel文件尾部
        writer.print("</Workbook>");
        writer.flush();
        writer.close();
        System.out.println("生成excel文件完成");
        long endTime = System.currentTimeMillis();
        System.out.println("用时="+((endTime-startTime)/1000)+"秒");
    }
    public static void main(String[] args) throws IOException, NoSuchMethodException, IllegalAccessException, InvocationTargetException {
        System.out.println(Thread.currentThread().getContextClassLoader().getResource("").getPath());
        System.out.println(ExcelStUtil.class.getResource("").getPath());
        System.out.println(ExcelStUtil.class.getClassLoader().getResource("").getPath());
        List<Sample> result = new ArrayList<Sample>();
        for(int i=0;i<100;i++){
            result.add(new Sample("放大双方的"+String.valueOf(i),String.valueOf(i)));
        }
        //OutputStream outputStream = new FileOutputStream("D:/output2.xls");
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        ExcelStUtil.export(byteArrayOutputStream,result);
        //ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(byteArrayOutputStream.toByteArray());
        //解决可能发生的中文乱码
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(byteArrayOutputStream.toString().getBytes("UTF-8"));
        File file = new File("D:/output2.xls");
        OutputStream output = new FileOutputStream(file);
        BufferedOutputStream bufferedOutput = new BufferedOutputStream(output);
        //bufferedOutput.write(byteArrayOutputStream.toByteArray());
        bufferedOutput.write(byteArrayOutputStream.toString().getBytes("UTF-8"));
        bufferedOutput.flush();
        bufferedOutput.close();
    }
}
```

