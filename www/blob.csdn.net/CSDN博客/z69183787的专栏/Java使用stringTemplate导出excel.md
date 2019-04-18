# Java使用stringTemplate导出excel - z69183787的专栏 - CSDN博客
2016年07月26日 17:47:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1361
maven：
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
st模板：
head.st：
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
body.st：
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
java：
```java
public static class Winning{
        private int id;
        private int prizeId;
        private int prizePackageId;
        private Date createTime;
        public Winning() {
        }
        public int getId() {
            return id;
        }
        public void setId(int id) {
            this.id = id;
        }
        public int getPrizeId() {
            return prizeId;
        }
        public void setPrizeId(int prizeId) {
            this.prizeId = prizeId;
        }
        public int getPrizePackageId() {
            return prizePackageId;
        }
        public void setPrizePackageId(int prizePackageId) {
            this.prizePackageId = prizePackageId;
        }
        public Date getCreateTime() {
            return createTime;
        }
        public void setCreateTime(Date createTime) {
            this.createTime = createTime;
        }
    }
    @Autowired
    private JdbcTemplate jdbcTemplate;
    @Test
    public void test() throws NoSuchMethodException, IllegalAccessException, InvocationTargetException, IOException {
        String sql = "xxxxxxx";
        List<Winning> target = jdbcTemplate.query(sql, new BeanPropertyRowMapper(Winning.class));
        long startTime = System.currentTimeMillis();
        StringTemplateGroup stGroup = new StringTemplateGroup("stringTemplate");
        //解决可能发生的中文乱码
        stGroup.setFileCharEncoding("UTF-8");
        //写入excel文件头部信息
        StringTemplate head =  stGroup.getInstanceOf("st/head");
        PrintWriter writer = new PrintWriter(new BufferedOutputStream(new FileOutputStream("D:/201607261706.xls")));
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
//        BeanInfo beanInfo=Introspector.getBeanInfo(clazz,Object.class);
//        PropertyDescriptor[] proDescrtptors=beanInfo.getPropertyDescriptors();
//        for(PropertyDescriptor propertyDescriptor : proDescrtptors){
//            title.add(propertyDescriptor.getName());
//            getMethods.add(propertyDescriptor.getReadMethod());
//        }
        int columnLength = title.size();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        //写入excel文件数据信息
        for(int i=0;i<sheets;i++){
            StringTemplate body =  stGroup.getInstanceOf("st/body");
            com.dianping.emidas.activity.excel.template.Worksheet worksheet = new Worksheet();
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
```
