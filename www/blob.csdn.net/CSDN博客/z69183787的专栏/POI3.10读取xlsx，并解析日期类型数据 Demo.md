# POI3.10读取xlsx，并解析日期类型数据 Demo - z69183787的专栏 - CSDN博客
2017年05月04日 14:43:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1954
```
<dependency>
            <groupId>org.apache.poi</groupId>
            <artifactId>poi</artifactId>
            <version>3.10-FINAL</version>
        </dependency>
        <dependency>
            <groupId>org.apache.poi</groupId>
            <artifactId>poi-ooxml</artifactId>
            <version>3.10-FINAL</version>
        </dependency>
        <dependency>
            <groupId>org.apache.poi</groupId>
            <artifactId>poi-ooxml</artifactId>
            <version>3.10-FINAL</version>
        </dependency>
        <dependency>
            <groupId>commons-fileupload</groupId>
            <artifactId>commons-fileupload</artifactId>
            <version>1.3.1</version>
        </dependency>
        <dependency>
            <groupId>commons-io</groupId>
            <artifactId>commons-io</artifactId>
            <version>2.4</version>
        </dependency>
        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-lang3</artifactId>
            <version>3.3.2</version>
        </dependency>
        <dependency>
            <groupId>commons-logging</groupId>
            <artifactId>commons-logging</artifactId>
            <version>1.1.3</version>
        </dependency>
        <dependency>
            <groupId>commons-collections</groupId>
            <artifactId>commons-collections</artifactId>
            <version>3.2.1</version>
        </dependency>
```
```java
package excel;
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import org.apache.commons.collections.CollectionUtils;
import org.apache.http.HttpEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.util.EntityUtils;
import org.apache.poi.xssf.usermodel.XSSFCell;
import org.apache.poi.xssf.usermodel.XSSFRow;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
/**
 * Created by Administrator on 2017/5/4.
 */
public class Prize517 {
    public static void main(String[] args) throws IOException {
        List<String> a = new ArrayList<>();
        List<Integer> result = readExcel2007();
        
    }
    public static List<Integer> readExcel2007(){
        List<Integer> result = new ArrayList<>();
        try{
            File excelFile = new File("C:\\Users\\Administrator\\Desktop\\517菜品.xlsx");
            FileInputStream is = new FileInputStream(excelFile);// 获取文件输入流
            XSSFWorkbook workbook2007 = new XSSFWorkbook(is);// 创建Excel2003文件对象
            XSSFSheet sheet = workbook2007.getSheetAt(0);// 取出第一个工作表，索引是0
            int c = sheet.getLastRowNum();
            System.out.println("rows="+c);
            // 开始循环遍历行，表头不处理，从1开始
            XSSFCell cell;
            for (int i = 1; i <= c; i++) {
                XSSFRow row = sheet.getRow(i);// 获取行对象
                if (row == null) {// 如果为空，不处理
                    continue;
                }
                //菜名
                cell = row.getCell(0);
                System.out.println(getStringCellValue(cell));
                //份数
                cell = row.getCell(1);
                System.out.println(getStringCellValue(cell));
                //价值
                cell = row.getCell(2);
                System.out.println(getStringCellValue(cell));
                //使用规则
                cell = row.getCell(3);
                System.out.println(getStringCellValue(cell));
                //使用期限开始
                cell = row.getCell(4);
                System.out.println(getStringCellValue(cell));
                //使用期限结束
                cell = row.getCell(5);
                System.out.println(getStringCellValue(cell));
                //门店列表
                cell = row.getCell(6);
                System.out.println(getStringCellValue(cell));
            }
        }catch(Exception e){
            e.printStackTrace();
        }
        return result;
    }
    /**
     * 获取单元格数据内容为字符串类型的数据
     *
     * @param cell Excel单元格
     * @return String 单元格数据内容
     */
    private static String getStringCellValue(XSSFCell cell) {
        String strCell = "";
        switch (cell.getCellType()) {
            case XSSFCell.CELL_TYPE_STRING:
                strCell = cell.getStringCellValue();
                break;
            case XSSFCell.CELL_TYPE_NUMERIC:
                if (XSSFDateUtil.isCellDateFormatted(cell)) {
                    //  如果是date类型则 ，获取该cell的date值
                    strCell = new SimpleDateFormat("yyyy-MM-dd").format(XSSFDateUtil.getJavaDate(cell.getNumericCellValue()));
                } else { // 纯数字
                    cell.setCellType(XSSFCell.CELL_TYPE_STRING);
                    strCell = String.valueOf(cell.getStringCellValue());
                }
                break;
            case XSSFCell.CELL_TYPE_BOOLEAN:
                strCell = String.valueOf(cell.getBooleanCellValue());
                break;
            case XSSFCell.CELL_TYPE_BLANK:
                strCell = "";
                break;
            default:
                strCell = "";
                break;
        }
        if (strCell.equals("") || strCell == null) {
            return "";
        }
        if (cell == null) {
            return "";
        }
        return strCell;
    }
    public static String httpGet(String url) throws IOException {
        HttpGet httpget = new HttpGet(url);
        CloseableHttpClient httpClient =  HttpClientBuilder.create().build();
        CloseableHttpResponse response = httpClient.execute(httpget);
        HttpEntity httpEntity = response.getEntity();
        String result = EntityUtils.toString(httpEntity);
        return result;
    }
}
```
