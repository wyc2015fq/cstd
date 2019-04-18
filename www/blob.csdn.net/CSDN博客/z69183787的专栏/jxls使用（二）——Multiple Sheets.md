# jxls使用（二）——Multiple Sheets - z69183787的专栏 - CSDN博客
2016年05月14日 17:47:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2332
    jxls在生成Excel文件时，提供一个批量生成Excel Sheet的功能，即可以一次生成一个包含多个Sheet的Excel文件。但是，jxls在处理这类操作的时候会出现性能问题，当一次形成的Excel的Sheet数量超过500个的时候就会出现很严重的性能问题，运算到最后，系统会提示OutOfMemeroyError，这种问题的出现是由于jxls的实现方式决定的。
        如果，仔细查看jxls的源代码就会发现，无论是处理单个Sheet还是处理多个Sheet的时候，在XLSTransformer类的Transform处理的时候都是在内存中对Sheet进行处理，然后，在生成WorkBook。假如，需要一次形成的Sheet为500以上的时候，势必会出现严重的性能问题。
        针对以上的问题，如果我们的应用需要形成500Sheet以上的Excel时，我们可以采用人为干预的方式，即在进行处理前对Beans的个数进行限制，最好将Beans的个数限制在200个以内，即每次只生成包含200个Sheet的WorkBook，在WorkBook形成以后，再对WorkBook进行合并，然后，将合并的WorkBook输出形成Excel文件，这样处理的时候，对一个Excel的总的Sheet也需要进行控制，否则在运算的过程中也会出现OutOfMemeroyError，最好控制在3000个以内，如果，超过3000个建议形成多个Excel文件，下面我通过一个具体的企业应用来说明怎么来进行操作。
        我这个应用是Payslip导出的一个应用，具体要求是：
        按照Division和Costcenter来组织Payslip卡片，每个Costcenter下面的员工组织到一个Excel文件里面
        在具体实现的过程发现有一个Costcenter下面的员工数量超过了3000，开始是想是否能够一次将这个Costcenter下面的Payslip全部形成，结果遭到了可耻的失败。经过不断的测试以后，得到了以上的两个临界点的数字。下面说明一下我的具体实现：
        1）从[数据库](http://lib.csdn.net/base/14)按照Costcenter取出Payslip，得到一个payslipList对象。具体就是组装出取数据的SQL Script，利用JDBCTemplate取出数据。
        2）形成Costcenter对应的Excel文件的名称，如果，得到的List的size大于3000，则每3000个记录形成一个Excel文件
        3）将得到的List按照200进行分块处理，每200处理一次
        4）利用jxls提供的方法transformMultipleSheetsList（）对数据进行处理，得到一个HSSFWorkbook对象
        5）合并得到的HSSFWorkbook，输出形成Excel文件
具体代码如下：
java 代码
- publicvoid exportPayslip(String costcenterCode) throws IOException{
   
-       List payslipList = payslipDao.getPaylipByCostcenter(String costcenterCode);   
-       String xlsFileName;   
- 
- if (payslipList.size() <= 3000){
   
-            xlsFileName = "c:"+File.separator+costcenterCode+".xls";   
-            transformXLSFile(payslipList ,xlsFileName );   
-       } else {   
- for (int i =0;i<(int)Math.ceil((double)payslipList.size()/3000;i++)){
   
-                 xlsFileName = "c:"+File.separator+costcenterCode+"_"+new Integer(i+1).toString()+".xls";
   
- int k = ((i+1)*3000
 < payslipList.size()) ? (i+1)*3000 : resultList.size();   
-                 List resultList = payslipList.subList(i*3000,k);   
-                 transformXLSFile(resultList ,xlsFileName );   
-            }   
-       }   
- }   
- 
- privatevoid transformXLSFile(List resultList,String fileName){   
-      XLSTransformer transformer = new XLSTransformer();   
-      HSSFWorkBook resultWorkBook = null;   
- 
- for (int i = 0;i<(int)Math.ceil((double)resultList.size()/200;i++)){
   
-            List payslipMaps = new ArrayList();   
-            List sheetNames = new ArrayList();   
- 
-            InputStream xlsTemplateIO = new BufferedInputStream(getClass().getResourceAsStream("payslip.xls"));
   
- 
- int k = ((i+1)*200 <
 resultList.size()) ? (i+1)*200 : resultList.size();   
- 
-            payslipMaps = resultList.subList(i*200,k);   
- 
- for (int j=0;j
-                 Map payslip = (Map)resultList.get(i);   
-                 String sheetName = paysli.get("EMPLOYEE_ID");   
- 
-                 sheetNames.add(sheetName);   
-            }   
- 
-            HSSFWorkbook workBook = transformer.transformMultipleSheetsList(xlsTemplateIO , payslipMaps , sheetNames, "map", new HashMap(), 0);
   
- 
- if (i == 0){   
-                  resultWorkBook = workBook;   
-            } else {   
- for (int h = 0;h < workBook.getNumberOfSheets();h++){
   
- 
-                       HSSFSheet newSheet = resultWorkBook.createSheet( (String) sheetNames.get(i) );   
-                       HSSFSheet hssfSheet = workBook.getSheetAt(h);   
-                       Util.copySheets(newSheet, hssfSheet);   
-                       Util.copyPageSetup(newSheet, hssfSheet);   
-                       Util.copyPrintSetup(newSheet, hssfSheet);   
-                  }   
-            }   
- 
-            saveWorkbook(resultWorkbook, fileName);   
-            xlsTemplateIO.close();   
-      }   
- }   
- 
- privatevoid saveWorkbook(HSSFWorkbook resultWorkbook, String fileName) throws IOException{
   
-       OutputStream os = new BufferedOutputStream(new FileOutputStream(fileName));
   
-       resultWorkbook.write(os);   
-       os.flush();   
-       os.close();   
- }  
        以上代码有部分代码需要被放在try/catch语句里面，确保Stream能够被关闭，否则，会影响程序的运行性能。代码只代表个人的一种实现方式。
        以上代码有一个地方需要说明，就是jxls的transformMultipleSheetsList（）方法，该方法的声明如下：
java 代码
- public HSSFWorkbook transformMultipleSheetsList(InputStream is, List objects, List newSheetNames, String beanName, Map beanParams, int startSheetNum) throws ParsePropertyException {  
        该方法里面的参数说明如下：
        1）is：即Template文件的一个输入流
        2）newSheetNames：即形成Excel文件的时候Sheet的Name
        3）objects：即我们传入的对应每个Sheet的一个[Java](http://lib.csdn.net/base/17)对象，这里传入的List的元素为一个Map对象
        4）beanName：这个参数在jxls对我们传入的List进行解析的时候使用，而且，该参数还对应Template文件中的Tag，例如，beanName为map，那么在Template文件中取值的公式应该定义成${map.get("property1")}；如果beanName为payslip，公式应该定义成${payslip.get("property1")}
        5）beanParams：这个参数在使用的时候我的代码没有使用到，这个参数是在如果传入的objects还与其他的对象关联的时候使用的，该参数是一个HashMap类型的参数，如果不使用的话，直接传入new HashMap()即可
        6）startSheetNo：传入0即可，即SheetNo从0开始
        以上代码的原理与操作Simple Sheet的时候差别不大，主要是Template的文件的取值公式和调用的方法有所改变而已。关键是在[大数据](http://lib.csdn.net/base/20)量操作的时候注意对处理的数据进行分块处理即可，否则，系统运行性能将很难保证。以上代码，在我实际的使用过程中速度还是比较理想的，16000条数据的导出时间是15分钟左右。
