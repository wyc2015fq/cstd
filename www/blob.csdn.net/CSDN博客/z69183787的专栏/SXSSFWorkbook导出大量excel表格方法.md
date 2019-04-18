# SXSSFWorkbook导出大量excel表格方法 - z69183787的专栏 - CSDN博客
2018年06月13日 19:18:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1528
前言：这几天遇到一个需求，需要从[数据库](https://www.2cto.com/database/)读取记录然后导出成excel表格，
在这个过程遇到很多问题，从最开始使用HSSFWorkbook到XSSFWorkbook，最后使用
SXSSFWorkbook。
话不多说先放伪代码
```java
public int exportExcel(){
 
    XSSFWorkbook xssfWb = null;
    SXSSFWorkbook sxssfWorkbook = null;
    SXSSFSheet sxssSheet = null;
    FileOutputStream fos = null;
    try {
        //导出excel时多少行flush
        int rowMaxCache = 100;
 
        xssfWb = new XSSFWorkbook();
        //rowMaxCache（可选）不声明默认100
        sxssfWorkbook = new SXSSFWorkbook(xssfWb, rowMaxCache);
        sxssSheet = (SXSSFSheet) sxssfWorkbook.createSheet("sheet标题名");
        //记录总数
        int listNum = 查询数据库;
        //内存缓存最大记录数，分次查询数据库
        int sourceMaxCache = 10000;
        //总共查询次数
        int findNum = 0;
        if (0 == (listNum % sourceMaxCache)){
            findNum = listNum / sourceMaxCache;
        }else {
            findNum = listNum / sourceMaxCache + 1;
        }
 
        for (int j = 0; j < findNum; j++){
             
            //mapParam作为查询条件限制，这里只设置设置数据库中的limit
            mapParam.put("start",j * sourceMaxCache + 1);
            mapParam.put("end",sourceMaxCache);
             
            //导出记录
            List<map<string,object>> list = 查询数据库操作(mapParam);
            //行数
            int exportNum = list.size();
            //列数
            int columnNum = list.get(0).size();
             
            // 首行 标题
            if (0 == j){
                Row row0 = sxssSheet.createRow(0);
                Cell sxssC1 = row0.createCell(0);
                sxssC1.setCellValue("标题");
                //设置单元格样式 setStyle
                sxssC1.setCellStyle(setStyle(sxssfWorkbook));
                //合并  参数1：起始行号 参数2：终止行号 参数3：起始列号 参数4：终止列号
                sxssSheet.addMergedRegion(new CellRangeAddress(0, 0, 0, exportNum));
            }
             
            //第二行 表头
            if (0 == j){
                Row row1 = sxssSheet.createRow(1);
                for (int k = 0; k < columnNum; k++){
                    // Row row, int column, String value, CellStyle style
                    // 行 ，列号，单元格值，样式（可选）
                    CellUtil.createCell(row1, 0, value, setStyle(sxssfWorkbook));
                }
            }
 
            //数据
            for (int i = 0; i < exportNum; i++) {
                Row row = sxssSheet.createRow(j * sourceMaxCache + i + 2);
                Map<string, object=""> map = list.get(i);
                // 数据
                for (int k = 0; k < columnNum; k++){
                    // Row row, int column, String value, CellStyle style
                    // 行 ，列号，单元格值，样式（可选）
                    CellUtil.createCell(row1, 0, value);
                }
            }
            //帮助gc回收内存
            list.clear();
        }
        //创建文件导出文件路径
        File f = new File(currStr);
        try {
            if (!f.exists()) {
                f.createNewFile();
            }
        } catch (IOException e1) {
            return "0";
        }
        fos = new FileOutputStream(f);
        sxssfWorkbook.write(fos);
 
    } catch (Exception e) {
        e.printStackTrace();
        return "0";
    }finally{
        try {
            if(fos != null){
                fos.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
```
<dependency>
    <groupid>org.apache.poi</groupid>
    poi</artifactid>
    <version>3.9</version>
</dependency>
 
<dependency>
    <groupid>org.apache.poi</groupid>
    poi-ooxml</artifactid>
    <version>3.9</version>
</dependency>
 
<dependency>
    <groupid>org.apache.poi</groupid>
    poi-ooxml-schemas</artifactid>
    <version>3.9</version>
</dependency>
```
总结，代码虽然能实现大量数据的excel表格导出，但还有几个优化问题，
1、内存占用优化：最开始我是把数据库数据全部读取出来导出，当数据量很大的时候，
tomcat内存占用飞速增长，当数量很大时可能影响tomcat性能甚至jvm内存溢出。
2、时间占用；为了解决上面内存占用问题，我采用了分批次查询数据库，来减小服务
器压力，但这样有引出另个问题，因为多次查询数据库，造成每次数据写完excel后会有等待
数据返回时间，而且对sql查询性能优化显得也更为重要。
3、样式问题：读者可能会奇怪我除了对表头和标题加入样式，正式数据却没有加入样式，
因为在我测试过程中，对每个单元加入样式等其他操作时，会严重影响性能，在一番网上查询
资料未解决后才得以放弃，因为楼主觉得为了样式而牺牲性能有些不值得（毕竟为每个单元格
设置样式可是行列相乘）
4、其他问题：虽然考虑了数据占用的内存，excel在导出之前还是保存在内存之中的，当
数据量很多时候，这一部分的内存也不得不考虑，楼主在解决这个问题时想到既然可以分批次查
询数据到内存，为何不可当EXCELrow达到一定量先写入磁盘然后继续其他操作，但进入[源码](https://www.2cto.com/ym/)
[?](https://www.2cto.com/kf/201711/698809.html#)
```
```java
SXSSFWorkbook.write(OutputStream stream)
```
```
查看到write方法最后关闭了流，不能再继续往内存EXCEL写如（总不能再引入文件流进行操作
吧那没有意义）暂时未找到解决方案，
