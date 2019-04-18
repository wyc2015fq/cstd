# Java 使用POI 导出 百万级别的数据量的 Excel - z69183787的专栏 - CSDN博客
2018年07月18日 17:42:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1878
[https://blog.csdn.net/java_cainiao2016/article/details/78091812](https://blog.csdn.net/java_cainiao2016/article/details/78091812)
首先声明一下，这篇博客是我看到别人的之后然后修改了一下，博客原文的地址是：[http://blog.csdn.net/happyljw/article/details/52809244](http://blog.csdn.net/happyljw/article/details/52809244)
1.**首先介绍一下目前导出excel的几种格式：Excel 2003、Excel 2007**
 Excel 2003：**在****POI中****使用****HSSF****对象时，****excel 2003****最多****只允许存储****65536****条数据，一般用来处理较少的数据量。**这时对于百万级别数据，****Excel肯定****容纳不了。****
 Excel 2007：**当POI****升级到****XSSF****对象时，它可以直接支持****excel2007****以上版本，因为它采用****ooxml****格式。这时****excel****可以支持****1048576****条数据，****单个****sheet表****就支持近****104****万条数据了,虽然这时****导出****100****万数据能满足要求，但使用****XSSF****测试后发现****偶尔还是会发生堆溢出****，所以也不适合百万数据的导出。**
**在POI3.8之后新增加了一个类，SXSSFWorkbook，采用当数据加工时不是类似前面版本的对象，**
**它可以控制excel数据占用的内存，他通过控制在内存中的行数来实现资源管理，即当创建对象超过了设定的行数，**
**它会自动刷新内存，将数据写入文件，这样导致打印时，占用的CPU，和内存很少。**
**所以本文将使用SXXFWorkBook来实现百万级别数据量的导出。**
**2.下面是Java 代码部分**
```java
import java.io.FileOutputStream;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.xssf.streaming.SXSSFWorkbook;
 
/**
 * @Description: 处理百万级别的excel导出操作工具类
 * @author : hujingbo
 * @date : 2017-9-22 下午1:55:31
 */
public class BigDataExcelOutWrite {
	/**
	 * 数据库连接操作
	 * 
	 * @throws Exception
	 */
	public Connection getConnection() throws Exception {
 
		// 使用jdbc链接数据库
		Class.forName("com.mysql.jdbc.Driver").newInstance();
		String url = "jdbc:mysql://localhost:3306/bigdata?characterEncoding=UTF-8";
		String username = "root";
		String password = "123456";
 
		// 获取数据库连接
		Connection conn = DriverManager.getConnection(url, username, password);
		return conn;
	}
 
	/**
	 * 
	 * @Title: WriteExcel
	 * @Description: 执行导出Excel操作
	 * @param
	 * @return boolean
	 * @throws
	 */
	public boolean WriteExcel(boolean isClose) {
 
		String excelFile = "D:/bigData.xlsx";
		// 内存中只创建100个对象，写临时文件，当超过100条，就将内存中不用的对象释放。
		SXSSFWorkbook wb = new SXSSFWorkbook(100);
		Sheet sheet = null; // 工作表对象
		Row nRow = null; // 行对象
		Cell nCell = null; // 列对象
 
		try {
 
			Connection conn = getConnection();
			Statement stmt = conn.createStatement();
			String sql = "select * from hpa_normal_tissue limit 1000000";
			ResultSet rs = stmt.executeQuery(sql); // 获取执行结果
			ResultSetMetaData rsmd = rs.getMetaData(); // 获取执行结果的结构(rs.getMetaData().getTableName(1))就可以返回表名,rs.getMetaData().getColumnCount())
 
			long startTime = System.currentTimeMillis();
			System.out.println("开始执行时间 : " + startTime / 1000 + "m");
			int rowNo = 0; // 总行号
			int pageRowNo = 0; // 页行号
 
			while (rs.next()) {
				// 打印300000条后切换到下个工作表，可根据需要自行拓展，2百万，3百万...数据一样操作，只要不超过1048576就可以
				if (rowNo % 300000 == 0) {
					System.out.println("当前sheet页为:" + rowNo / 300000 );
					sheet = wb.createSheet("我的第" + (rowNo / 300000 + 1) + "个工作簿");// 建立新的sheet对象
					sheet = wb.getSheetAt(rowNo / 300000); // 动态指定当前的工作表
					pageRowNo = 1; // 每当新建了工作表就将当前工作表的行号重置为1
					
				    //定义表头
				    nRow = sheet.createRow(0);
				    Cell cel0 = nRow.createCell(0);  
				    cel0.setCellValue("第一行");
				    Cell cel2 = nRow.createCell(1);  
				    cel2.setCellValue("第二行");  
				    Cell cel3 = nRow.createCell(2);  
				    cel3.setCellValue("第三行");  
				    Cell cel4 = nRow.createCell(3); 
				    cel4.setCellValue("第四行");
				    Cell cel5 = nRow.createCell(4); 
				    cel5.setCellValue("第五行");
				    Cell cel6 = nRow.createCell(5); 
				    cel6.setCellValue("第六行");
				}
				rowNo++;
				nRow = sheet.createRow(pageRowNo++); // 新建行对象
 
				// 打印每行，每行有6列数据 rsmd.getColumnCount()==6 --- 列属性的个数
				for (int i = 0; i < rsmd.getColumnCount(); i++) {
					nCell = nRow.createCell(i);
					nCell.setCellValue(rs.getString(i + 1));
				}
 
				if (rowNo % 10000 == 0) {
					System.out.println("row no: " + rowNo);
				}
			}
 
			long finishedTime = System.currentTimeMillis(); // 处理完成时间
			System.out.println("数据读取完成耗时 : " + (finishedTime - startTime) / 1000 + "m");
			
			FileOutputStream fOut = new FileOutputStream(excelFile);//将数据写入Excel
			wb.write(fOut);
			fOut.flush(); // 刷新缓冲区
			fOut.close();
 
			long stopTime = System.currentTimeMillis(); // 写文件时间
			System.out.println("数据写入Excel表格中耗时 : " + (stopTime - startTime) / 1000 + "m");
 
			if (isClose) {
				this.close(rs, stmt, conn);
			}
 
		} catch (Exception e) {
			e.printStackTrace();
		}
		return false;
	}
 
	// 执行关闭流的操作
	private void close(ResultSet rs, Statement stmt, Connection conn)throws SQLException {
		rs.close();
		stmt.close();
		conn.close();
	}
	//测试方法
	public static void main(String[] args) {
		BigDataExcelOutWrite bdeo = new BigDataExcelOutWrite();
		bdeo.WriteExcel(true);
	}
}
```
执行之后的效果图
**![](https://img-blog.csdn.net/20170926101843103?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamF2YV9jYWluaWFvMjAxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
3.所需的sql文件和相关的jar包。
链接: https://pan.baidu.com/s/1nvOJgWL    密码: eqj9
