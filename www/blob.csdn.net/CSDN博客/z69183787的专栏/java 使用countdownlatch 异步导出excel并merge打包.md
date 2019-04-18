# java 使用countdownlatch 异步导出excel并merge打包 - z69183787的专栏 - CSDN博客
2016年07月27日 15:21:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2519
个人分类：[Excel处理-导出](https://blog.csdn.net/z69183787/article/category/2748995)
```java
import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.tools.zip.ZipEntry;
import org.apache.tools.zip.ZipOutputStream;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import java.beans.IntrospectionException;
import java.beans.PropertyDescriptor;
import java.io.*;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.concurrent.CountDownLatch;
public class ThreadTest extends AbstractJUnit4SpringContextTests {
    public CountDownLatch main = new CountDownLatch(1);
    public CountDownLatch sub = null;
    public static final int maxRow = 60000;
    public static List<Method> getMethod = new ArrayList<Method>();
    public static List<File> fileList = new ArrayList<File>();
    @Autowired
    private JdbcTemplate jdbcTemplate;
    public class Exporter implements Runnable{
        private final CountDownLatch main;
        private final CountDownLatch sub;
        private final int beginIndex;
        public Exporter(CountDownLatch main,CountDownLatch sub,int beginIndex){
            this.main = main;
            this.sub = sub;
            this.beginIndex = beginIndex;
        }
        @Override
        public void run() {
            try {
                main.await();
                List<ExcelTest.Winning> target = jdbcTemplate.query(
                        "select ID,PrizeId,PrizePackageId,CreateTime from xxxxx where PrizePackageId=1121 AND id > 221711322 LIMIT " + beginIndex + "," + maxRow,
                        new BeanPropertyRowMapper(ExcelTest.Winning.class));
                generateExcel(target,beginIndex);
            }catch (InterruptedException  e){
            }finally {
                sub.countDown();
            }
        }
    }
    @org.junit.Test
    public void test() throws InterruptedException {
        String sql = "select count(*) from xxxxx where PrizePackageId=1121 AND id > 221711322";
        int count = jdbcTemplate.queryForInt(sql);
        int totalPage = count%maxRow == 0 ? (count/maxRow):(count/maxRow)+1;
        sub = new CountDownLatch(totalPage);
        try {
            getReflectInfo(ExcelTest.Winning.class,getMethod);
        } catch (IntrospectionException e) {
            logger.info(e);
        }
        for(int i =1;i<=totalPage;i++){
            int beginIndex = (i - 1) * maxRow;
            new Thread(new Exporter(main,sub,beginIndex)).start();
        }
        main.countDown();
        sub.await();
        //merge
        String zipName = "ZipExport.zip";
        try {
            int len = 0;
            byte[] buffers = new byte[1024];
            File file = new File("D:\\"+zipName);
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            ZipOutputStream zos = new ZipOutputStream(new FileOutputStream(file));
            try {
                //zos = new ZipOutputStream(bos);
                for (File f : fileList) {
                    ZipEntry entry = new ZipEntry(f.getName());
                    zos.putNextEntry(entry);
                    InputStream in = new FileInputStream(f);
                    while ((len = in.read(buffers)) != -1) {
                        zos.write(buffers, 0, len);
                    }
                    zos.setEncoding("gbk");//解决中文
                    zos.closeEntry();
                    in.close();
                }
                zos.flush();
                zos.close();
            } catch (Exception localException) {
            }
        }catch(Exception e){}
        System.out.println("Done");
    }
    private static void generateExcel(List target,int beginIndex){
        HSSFWorkbook workbook2003 = new HSSFWorkbook();
        // 创建工作表对象并命名
        HSSFSheet sheet = workbook2003.createSheet("学生信息统计表");
        // 遍历集合对象创建行和单元格
        for (int i = 0; i < target.size(); i++) {
            // 创建行
            HSSFRow row = sheet.createRow(i);
            Object object = target.get(i);
            for (int j=0;j< getMethod.size();j++) {
                try {
                    Object result = getMethod.get(j).invoke(object);
                    if (result instanceof Date) {
                        result = DateUtils.format((Date) result, DateUtils.getSecondFormatter());
                    }
                    HSSFCell cell = row.createCell(j);
                    cell.setCellValue(result.toString());
                } catch (Exception e) {
                }
            }
        }
        // 生成文件
        File file = new File("D:\\excelTest_"+beginIndex+ ".xls");
        FileOutputStream fos = null;
        try {
            fos = new FileOutputStream(file);
            workbook2003.write(fos);
            fileList.add(file);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (fos != null) {
                try {
                    fos.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
    private static void getReflectInfo(Class<?> c,List<Method> getMethod) throws IntrospectionException {
        Field[] fields = c.getDeclaredFields();
        for (int i = 0; i < fields.length; i++) {
            if (!fields[i].getName().equalsIgnoreCase("serialVersionUID") && !fields[i].getName().equalsIgnoreCase("$jacocoData")) {
                getMethod.add(new PropertyDescriptor(fields[i].getName(), c).getReadMethod());
            }
        }
    }
}
```
