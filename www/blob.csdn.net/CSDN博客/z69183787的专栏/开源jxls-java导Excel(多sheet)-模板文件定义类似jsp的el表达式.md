# 开源jxls - java导Excel(多sheet) - 模板文件定义类似jsp的el表达式 - z69183787的专栏 - CSDN博客
2016年05月14日 17:42:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3841
想不想像写[jsp](http://www.2cto.com/kf/web/jsp/)一样的去定义一个excel模板，然后再像用jstl和el表达式那样往excel模板里面传入变量，现在用jxls全部可以轻松搞定了，核心代码非常简单。
[java]  
//调用引擎生成excel报表  
XLSTransformer transformer = new XLSTransformer();  
Workbook workbook = transformer.transformMultipleSheetsList(is, objects, listSheetNames, "list", new HashMap(), 0);  www.2cto.com
workbook.write(new FileOutputStream("c:\\xlsExportUtils.xls"));  
下面来看看excel模板是怎么定义的。
![](http://up.2cto.com/2013/0131/20130131091738461.jpg)
是不是和jsp里面的jstl很像。
下面是这个测试例子的完整代码：
[java]  
import java.io.FileInputStream;  
import java.io.FileOutputStream;  
import java.io.IOException;  
import java.util.ArrayList;  
import java.util.HashMap;  
import java.util.List;  
import java.util.Map;  
import net.sf.jxls.exception.ParsePropertyException;  
import net.sf.jxls.transformer.XLSTransformer;  
import org.apache.poi.openxml4j.exceptions.InvalidFormatException;  
import org.apache.poi.ss.usermodel.Workbook;  
/**   
* @author 鲁炬 
* 
*/  
public class XlsExportUtil {  
  public static void createExcel() throws ParsePropertyException, InvalidFormatException, IOException {}  
  @SuppressWarnings("rawtypes")  
  public static void main(String[] args) throws ParsePropertyException, InvalidFormatException, IOException {  
    //获取Excel模板文件  
    String fileDir = XlsExportUtil.class.getResource("").getFile();  
    String filePath = fileDir + "XlsExportUtilTemplate.xls";  
    System.out.println("excel template file:" + filePath);  
    FileInputStream is = new FileInputStream(filePath);  
    //创建测试数据  
    Map<String, Object> map = new HashMap<String, Object>();  
    List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();  
    Map<String, Object> map1 = new HashMap<String, Object>();  
    map1.put("name", "电视");  
    map1.put("price", "3000");  
    map1.put("desc", "3D电视机");  
    map1.put("备注", "中文测试");  
    Map<String, Object> map2 = new HashMap<String, Object>();  
    map2.put("name", "空调");  
    map2.put("price", "2000");  
    map2.put("desc", "变频空调");  
    map1.put("备注", "测试中文");  
    list.add(map1);  
    list.add(map2);  
    map.put("list", list);  
    ArrayList<List> objects = new ArrayList<List>();  
    objects.add(list);  
    objects.add(list);  
    objects.add(list);  
    objects.add(list);  
    //sheet的名称  
    List<String> listSheetNames = new ArrayList<String>();  
    listSheetNames.add("1");  
    listSheetNames.add("2");  
    listSheetNames.add("3");  
    listSheetNames.add("4");  
    //调用引擎生成excel报表  
    XLSTransformer transformer = new XLSTransformer();  
    Workbook workbook = transformer.transformMultipleSheetsList(is, objects, listSheetNames, "list", new HashMap(), 0);  
    workbook.write(new FileOutputStream("c:\\xlsExportUtils.xls"));  
  }  
}  
**其中：newSheetNames 的 size 与 objects的size 一致 ，每一个sheet展示的数据 与这2个参数 的 index 一一 对应**
该方法里面的参数说明如下：
        1）is：即Template文件的一个输入流
        2）newSheetNames：即形成Excel文件的时候Sheet的Name
        3）objects：即我们传入的对应每个Sheet的一个[Java](http://lib.csdn.net/base/17)对象，这里传入的List的元素为一个Map对象
        4）beanName：这个参数在jxls对我们传入的List进行解析的时候使用，而且，该参数还对应Template文件中的Tag，例如，beanName为map，那么在Template文件中取值的公式应该定义成${map.get("property1")}；如果beanName为payslip，公式应该定义成${payslip.get("property1")}
        5）beanParams：这个参数在使用的时候我的代码没有使用到，这个参数是在如果传入的objects还与其他的对象关联的时候使用的，该参数是一个HashMap类型的参数，如果不使用的话，直接传入new HashMap()即可
        6）startSheetNo：传入0即可，即SheetNo从0开始
