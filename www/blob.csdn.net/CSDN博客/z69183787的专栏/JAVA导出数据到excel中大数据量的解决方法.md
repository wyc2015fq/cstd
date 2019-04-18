# JAVA导出数据到excel中大数据量的解决方法 - z69183787的专栏 - CSDN博客
2016年07月26日 16:29:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3475
web项目中需要有将数据导出excel的操作需求。曾经尝试过几种方法：
1、poi导出到excel
他是操作对象然后将结果写到excel中。
2、拼excel的html形式文件，将文件扩展名改为xls
这中方式是利用excel能够另存为html格式文件，文件包含excel的头信息。
以前用过的方法，没有考虑的问题
1、使用poi等导出时，没有考虑数据量的问题，大数据量无法满足，有个几千行jvm就哭了。
2、后改为拼html文件时，没有考虑excel中sheet的最大行数65536，超过这个行数就excel无法打开了。
综合考虑，并从网上找了些其他人的想法，最后用如下方案
继续沿用拼excel的html形式文件的方法，但在导出时，根据最大行数重新建立新的excel文件；
数据从数据库取出使用纯jdbc方式读数据，边读边向文件中写；
待所有数据写完，将各个小excel文件进行打包，输出到输出流中。
定义导出接口
```java
import java.io.OutputStream;
import java.util.Collection;
/**
 * 大数据量导出
 * 
 * @author lisen
 * 
 */
public interface ExportBigData {
	/**
	 * 导出sql查询出的数据,输出为压缩后的格式
	 * 
	 * @param os
	 *            输出流
	 * @param maxRow
	 *            每个excel的最大行数
	 * @param sql
	 *            查询语句
	 * @param sqlParams
	 *            查询参数
	 */
	public void exportToZip(final OutputStream os, final int maxRow,
			final String sql, final Object... sqlParams);
	/**
	 * 导出sql查询出的数据,输出为压缩后的格式
	 * 
	 * @param titles
	 *            标题
	 * @param os
	 *            输出流
	 * @param maxRow
	 *            每个excel的最大行数
	 * @param sql
	 *            查询语句
	 * @param sqlParams
	 *            查询参数
	 */
	public void exportToZip(final Collection<String> titles,
			final OutputStream os, final int maxRow, final String sql,
			final Object... sqlParams);
	public void exportToZip(final String[] titles, final OutputStream os,
			final int maxRow, final String sql, final Object... sqlParams);
	public void exportToZip(final String[] titles, final OutputStream os,
			final String sql, final Object... sqlParams);
	public void exportToZip(final Collection<String> titles,
			final OutputStream os, final String sql, final Object... sqlParams);
	public void exportToZip(final OutputStream os, final String sql,
			final Object... sqlParams);
}
```
大数据量导出抽象实现
```java
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.ResultSetExtractor;
import org.springframework.jdbc.support.JdbcUtils;
import com.cnaec.common.Constant;
/**
 * 大数据量导出 抽象类
 * 
 * @author lisen
 * @date 2013-10-30 上午10:40:33
 */
public abstract class AbstractExportBigData implements ExportBigData {
	/**
	 * 每个文件的最大行数 超过请求按默认算
	 */
	public static final int MAXROWS = 50000;
	/**
	 * 用于数据查询
	 */
	private JdbcTemplate jdbcTemplate;
	/**
	 * 构造函数
	 * 
	 * @param jdbcTemplate
	 */
	public AbstractExportBigData(JdbcTemplate jdbcTemplate) {
		this.jdbcTemplate = jdbcTemplate;
	}
	/**
	 * 临时文件前缀
	 * 
	 * @return
	 */
	abstract protected String getPrefix();
	/**
	 * 临时文件后缀
	 * 
	 * @return
	 */
	abstract protected String getSuffix();
	/**
	 * 删除临时文件
	 * 
	 * @param fileList
	 */
	protected void cleanTempFile(List<File> fileList) {
		for (File file : fileList) {
			System.out.println("-----删除文件," + file.getPath());
			file.delete();
		}
	}
	/**
	 * 创建临时文件
	 * 
	 * @return
	 * @throws IOException
	 */
	protected File createTempFile() throws IOException {
		return File.createTempFile(getPrefix(), getSuffix());
	}
	/**
	 * 数据输出
	 * 
	 * @param data
	 * @param fos
	 * @throws IOException
	 */
	protected void writeToOutputStream(String data, FileOutputStream fos)
			throws IOException {
		IOUtils.write(data, fos, Constant.ENCODING);
	}
	/**
	 * 文件开头的写入
	 * 
	 * @param fos
	 * @throws IOException
	 */
	abstract protected void writeHeaderToOutputStream(FileOutputStream fos)
			throws IOException;
	/**
	 * 文件结尾的写入
	 * 
	 * @param fos
	 */
	abstract protected void writeFooterToOutputStream(FileOutputStream fos)
			throws IOException;
	/**
	 * 一行数据的写入
	 * 
	 * @param rs
	 * @param fos
	 * @throws SQLException
	 * @throws IOException
	 */
	abstract protected void writeOneRowToOutputStream(ResultSet rs,
			FileOutputStream fos) throws SQLException, IOException;
	/**
	 * 写数据标题
	 * 
	 * @param titles
	 * @param fos
	 * @throws IOException
	 */
	abstract protected void writeTitleToOutputStream(Collection<String> titles,
			FileOutputStream fos) throws IOException;
	/**
	 * 打包 压缩成zip
	 * 
	 * @param os
	 *            压缩输出流
	 * @param fileList
	 *            被压缩的文件列表
	 * @throws IOException
	 */
	protected void doZip(OutputStream os, List<File> fileList)
			throws IOException {
		if (fileList != null && fileList.size() > 0) {
			byte[] buf = new byte[1024];
			ZipOutputStream out = new ZipOutputStream(os);
			for (File file : fileList) {
				FileInputStream in = new FileInputStream(file);
				out.putNextEntry(new ZipEntry(file.getName()));
				int len;
				while ((len = in.read(buf)) > 0) {
					out.write(buf, 0, len);
				}
				out.closeEntry();
				in.close();
			}
			System.out.println(os == null);
			out.close();
			System.out.println(os == null);
		}
	}
	/**
	 * 获取单个文件最大行数
	 * 
	 * @param maxRow
	 * @return
	 */
	protected int getMaxRows(int maxRow) {
		return maxRow < MAXROWS ? maxRow : MAXROWS;
	}
	protected String getColumnKey(String columnName) {
		return columnName;
	}
	protected Object getColumnValue(ResultSet rs, int index)
			throws SQLException {
		return JdbcUtils.getResultSetValue(rs, index);
	}
	@Override
	public void exportToZip(final Collection<String> titles,
			final OutputStream os, final int maxRow, final String sql,
			final Object... sqlParams) {
		// 每个文件最大行数
		final int max = getMaxRows(maxRow);
		jdbcTemplate.query(sql, sqlParams, new ResultSetExtractor() {
			@Override
			public Object extractData(ResultSet rs) throws SQLException,
					DataAccessException {
				// 文件收集器
				List<File> fileList = new ArrayList<File>();
				// 行数记录器
				int i = 0;
				// 临时文件
				File file = null;
				FileOutputStream fos = null;
				try {
					while (rs.next()) {
						// 达到最大行数 或者 新建的 创建新文件
						if (i == max || i == 0) {
							// 如果不是新文件 为这个文件写入文件尾
							if (file != null) {
								// 写文件尾
								writeFooterToOutputStream(fos);
								// 关闭流
								IOUtils.closeQuietly(fos);
							}
							// 创建临时文件
							file = createTempFile();
							// 打开流
							fos = FileUtils.openOutputStream(file);
							// 放进收集器里
							fileList.add(file);
							// 写文件头
							writeHeaderToOutputStream(fos);
							// 数据区标题栏
							writeTitleToOutputStream(titles, fos);
							i = 0;
						}
						i++;
						// 写实际一行数据
						writeOneRowToOutputStream(rs, fos);
					}
					if (file != null) {
						// 写文件尾
						writeFooterToOutputStream(fos);
						// 关闭流
						IOUtils.closeQuietly(fos);
					}
					// 打包
					doZip(os, fileList);
				} catch (IOException e) {
					// io异常
				} finally {
					IOUtils.closeQuietly(fos);
					// 清空临时文件
					cleanTempFile(fileList);
					fileList.clear();
					fileList = null;
				}
				return null;
			}
		});
	}
	@Override
	public void exportToZip(final OutputStream os, int maxRow, String sql,
			Object... sqlParams) {
		exportToZip(Collections.EMPTY_LIST, os, maxRow, sql, sqlParams);
	}
	@Override
	public void exportToZip(final String[] titles, OutputStream os, int maxRow,
			String sql, Object... sqlParams) {
		exportToZip(Arrays.asList(titles), os, maxRow, sql, sqlParams);
	}
	@Override
	public void exportToZip(final String[] titles, final OutputStream os,
			final String sql, final Object... sqlParams) {
		exportToZip(titles, os, MAXROWS, sql, sqlParams);
	}
	@Override
	public void exportToZip(final OutputStream os, final String sql,
			final Object... sqlParams) {
		exportToZip(os, MAXROWS, sql, sqlParams);
	}
	@Override
	public void exportToZip(final Collection<String> titles,
			final OutputStream os, final String sql, final Object... sqlParams) {
		exportToZip(titles, os, MAXROWS, sql, sqlParams);
	}
}
```
EXCEL导出的实现
```java
import java.io.FileOutputStream;
import java.io.IOException;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.Collection;
import java.util.List;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.web.util.HtmlUtils;
import com.avicinfo.v2.common.export.bigdata.AbstractExportBigData;
import com.avicinfo.v2.common.export.bigdata.ExportBigData;
/**
 * 用于excel大数据量导出
 * 
 * @author lisen
 * @date 2013-10-30 上午10:41:36
 */
public class BigDataToExcelImpl extends AbstractExportBigData implements
		ExportBigData {
	StringBuffer headStr = new StringBuffer(
			"<html xmlns:x=\"urn:schemas-microsoft-com:office:excel\">")
			.append("<head>")
			.append(
					"<meta http-equiv=\"content-type\" content=\"application/ms-excel; charset=UTF-8\"/>")
			.append("<!--[if gte mso 9]><xml>").append("<x:ExcelWorkbook>")
			.append("<x:ExcelWorksheets>").append("<x:ExcelWorksheet>").append(
					"<x:Name></x:Name>").append("<x:WorksheetOptions>").append(
					"<x:Print>").append("<x:ValidPrinterInfo />").append(
					"</x:Print>").append("</x:WorksheetOptions>").append(
					"</x:ExcelWorksheet>").append("</x:ExcelWorksheets>")
			.append("</x:ExcelWorkbook>").append("</xml><![endif]-->").append(
					"</head>").append("<body>").append("<table>");
	StringBuffer footStr = new StringBuffer("</table></body></html>");
	public BigDataToExcelImpl(JdbcTemplate jdbcTemplate) {
		super(jdbcTemplate);
	}
	/**
	 * 临时文件前缀
	 * 
	 * @return
	 */
	protected String getPrefix() {
		return "excelTemp";
	}
	/**
	 * 临时文件后缀
	 * 
	 * @return
	 */
	protected String getSuffix() {
		return ".xls";
	}
	/**
	 * 文件开头的写入
	 * 
	 * @param fos
	 * @throws IOException
	 */
	protected void writeHeaderToOutputStream(FileOutputStream fos)
			throws IOException {
		writeToOutputStream(headStr.toString(), fos);
	}
	/**
	 * 文件结尾的写入
	 * 
	 * @param fos
	 */
	protected void writeFooterToOutputStream(FileOutputStream fos)
			throws IOException {
		writeToOutputStream(footStr.toString(), fos);
	}
	/**
	 * 一行数据的写入
	 * 
	 * @param rs
	 * @param fos
	 * @throws SQLException
	 * @throws IOException
	 */
	protected void writeOneRowToOutputStream(ResultSet rs, FileOutputStream fos)
			throws SQLException, IOException {
		// 获取metaData;
		ResultSetMetaData rsmd = rs.getMetaData();
		int columnCount = rsmd.getColumnCount();
		writeToOutputStream("<tr>", fos);
		for (int i = 1; i <= columnCount; i++) {
			// String key = getColumnKey(rsmd.getColumnName(i));
			Object obj = getColumnValue(rs, i);
			writeToOutputStream("<td>"
					+ HtmlUtils.htmlEscape(obj == null ? "" : obj.toString())
					+ "</td>", fos);
		}
		writeToOutputStream("</tr>", fos);
	}
	protected void fileOutputStreamStatus(List<FileOutputStream> foList)
			throws IOException {
		System.out.println("共有文件输出流：" + foList.size());
		for (FileOutputStream fo : foList) {
			System.out.println("文件输出流："
					+ (fo == null ? "已清空" : fo.toString() + " : "
							+ (fo.getFD().valid())));
		}
	}
	@Override
	protected void writeTitleToOutputStream(Collection<String> titles,
			FileOutputStream fos) throws IOException {
		if (titles != null && titles.size() > 0) {
			writeToOutputStream("<tr>", fos);
			for (String title : titles) {
				writeToOutputStream("<td>"
						+ HtmlUtils.htmlEscape(title == null ? "" : title)
						+ "</td>", fos);
			}
			writeToOutputStream("</tr>", fos);
		}
	}
}
```
