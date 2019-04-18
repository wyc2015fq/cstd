# java导出大量的excel - z69183787的专栏 - CSDN博客
2016年02月23日 16:25:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2663
废话少说，直入主题
基本思路为  创建一个临时文件 写入数据 导出数据  删除临时文件
首先需要两个jar包
antlr和stringtemplate
创建数据库中的类Row
```java
private String name1;
     
    private String name2;
     
    private String name3;
    public String getName1() {
        return name1;
    }
    public void setName1(String name1) {
        this.name1 = name1;
    }
    public String getName2() {
        return name2;
    }
    public void setName2(String name2) {
        this.name2 = name2;
    }
    public String getName3() {
        return name3;
    }
    public void setName3(String name3) {
        this.name3 = name3;
    }
```
然后需要创建对应的Worksheet类
```java
<span style="font-size:10px;">private int columnNum;
    private int rowNum;
    private List<Row> rows;
    private List<SfOrder> orders;
    public List<SfOrder> getOrders() {
        return orders;
    }
    public void setOrders(List<SfOrder> orders) {
        this.orders = orders;
    }
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
    }</span>
```
然后需要写两个文件分别为
head.st
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
和body.st
```
﻿ $worksheet:{
 <Worksheet ss:Name="$it.sheet$">
  <Table ss:ExpandedColumnCount="$it.columnNum$" ss:ExpandedRowCount="$it.rowNum$" x:FullColumns="1"
   x:FullRows="1" ss:DefaultColumnWidth="54" ss:DefaultRowHeight="14.25">
 $it.rows:{
   <Row>
    <Cell><Data ss:Type="String">$it.name1$</Data></Cell>
   </Row>
 }$
  </Table>
 </Worksheet>
}$
```
下面就是程序代码
首先你要从数据中查询数据，假设你查询的数据为List数据
```java
//我这里的list是从map中获取的你可以直接获取
            List<Row> list=(List<Row>) result.get("list");
              list=(List<SfOrder>) result.get("list");
              if(list.size()==0){
                  return null;
              }
            long startTimne = System.currentTimeMillis();
            StringTemplateGroup stGroup = new StringTemplateGroup("stringTemplate");    
            stGroup.setFileCharEncoding("UTF-8");//设置编码，否则有乱码，导出excel格式不正确
            //写入excel文件头部信息
            StringTemplate head =  stGroup.getInstanceOf("/template/head");
            String path=request.getSession().getServletContext().getRealPath("/upload/excel/test1.xls");
            File file = new File(path);
            PrintWriter writer = new PrintWriter(new BufferedOutputStream(new FileOutputStream(file)));
            writer.print(head.toString());
            writer.flush();
            int sheets = 10;
            //excel单表最大行数是65535
            int maxRowNum = 3000;
            int maxRowNumLast=0;
            //计算要分几个sheet
            sheets=(list.size()%maxRowNum==0) ? list.size()/maxRowNum:list.size()/maxRowNum+1;
            //计算最后一个sheet有多少行
            maxRowNumLast=list.size()-(sheets-1)*maxRowNum;
```
上面为预备阶段，下面则直接向文件写数据
```java
//写入excel文件数据信息
            for(int i=0;i<sheets;i++){
                Integer nums=0;
                StringTemplate body =  stGroup.getInstanceOf("/template/body");
                Worksheet worksheet = new Worksheet();
                worksheet.setSheet(" "+(i+1)+" ");
                worksheet.setColumnNum(3);
                worksheet.setRowNum(maxRowNum);
                List<Row> orders = new ArrayList<Row>();
                if(i==(sheets-1)){
                    for(int j=0;j<maxRowNumLast;j++){
                        nums=i*maxRowNumLast+j;
                        SfOrder order=new SfOrder();
                        order.setOrderId(list.get(nums).getOrderId());
                        orders.add(order);
                    }
                }else{
                    for(int j=0;j<maxRowNum;j++){
                        nums=i*maxRowNum+j;
                        Row order=new Row();
                        order.setOrderId(list.get(nums).getOrderId());
                        orders.add(order);
                    }
                }
                worksheet.setOrders(orders);
                body.setAttribute("worksheet", worksheet);
                writer.print(body.toString());
                writer.flush();
                orders.clear();
                orders = null;
                worksheet = null;
                body = null;
                Runtime.getRuntime().gc();
                System.out.println("正在生成excel文件的 sheet"+(i+1));
            }
```
下面则写入服务器磁盘
```java
//写入excel文件尾部
            writer.print("</Workbook>");
            writer.flush();
            writer.close();
            System.out.println("生成excel文件完成");
            long endTime = System.currentTimeMillis();
            System.out.println("用时="+((endTime-startTimne)/1000)+"秒");
```
下面要读取文件,即正式的导出
```java
//创建file对象
            File upload=new File(path);
            //设置response的编码方式
            response.setContentType("application/vnd.ms-excel");
            response.setHeader("Content-Disposition","attachment;filename=ceshi.xls");
            //读出文件到i/o流
            FileInputStream fis=new FileInputStream(upload);
            BufferedInputStream buff=new BufferedInputStream(fis);
            byte [] b=new byte[1024];//相当于我们的缓存
            long k=0;//该值用于计算当前实际下载了多少字节
            //从response对象中得到输出流,准备下载
            OutputStream myout=response.getOutputStream();
            //开始循环下载
            while(k<upload.length()){
                int j=buff.read(b,0,1024);
                k+=j;
                //将b中的数据写到客户端的内存
                myout.write(b,0,j);
            }
            //将写入到客户端的内存的数据,刷新到磁盘
            myout.flush();
            myout.close();
```
如果需要可以，比如这个文件是动态的我们可以让这个文件是动态的，就要将此文件删除
```java
File fileDel = new File(path);  
                      // 如果文件路径所对应的文件存在，并且是一个文件，则直接删除  
                      if (fileDel.exists() && file.isFile()) {  
                       fileDel.delete()
                       }
```
这样就大功告成啦，导出所用的时间大部分是花费在数据库查询中，当然这也是你数据库功底的问题啦
