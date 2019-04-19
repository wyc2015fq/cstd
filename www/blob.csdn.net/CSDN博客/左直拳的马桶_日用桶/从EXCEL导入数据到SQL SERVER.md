# 从EXCEL导入数据到SQL SERVER - 左直拳的马桶_日用桶 - CSDN博客
2006年06月22日 18:50:00[左直拳](https://me.csdn.net/leftfist)阅读数：5397
从EXCEL导入数据到SQL SERVER
左直拳
介绍两种途径将数据从EXCEL中导入到SQL SERVER。
一、在程序中，用ADO.NET。代码如下：
//连接串
string strConn = "Provider=Microsoft.Jet.OLEDB.4.0;Extended Properties=Excel 8.0;Data Source=" + [EXCEL文件，含路径] + ";";
OleDbConnection conn = new OleDbConnection(strConn); 
conn.Open();
DataTable dtSchema = conn.GetOleDbSchemaTable(OleDbSchemaGuid.Tables,newobject[] {null, null, null, "TABLE"});
DataSet ds = new DataSet();
//一个EXCEL文件可能有多个工作表，遍历之
foreach( DataRow dr in dtSchema.Rows )
{
string table = dr["TABLE_NAME"].ToString();
string strExcel = "SELECT * FROM [" + table + "]";
ds.Tables.Add(table);
OleDbDataAdapter myCommand = new OleDbDataAdapter(strExcel,conn); 
myCommand.Fill(ds,table);
}
conn.Close();
这样，读取出来的数据就藏在DataSet里了。
采用这种方式，数据库所在机器不必装有EXCEL。
二、在查询分析器里，直接写SQL语句：
如果是导入数据到现有表，则采用
INSERT INTO 表 SELECT * FROM OPENROWSET('MICROSOFT.JET.OLEDB.4.0'
,'Excel 5.0;HDR=YES;DATABASE=c:/test.xls',sheet1$)
的形式
如果是导入数据并新增表，则采用
SELECT * INTO 表 FROM OPENROWSET('MICROSOFT.JET.OLEDB.4.0'
,'Excel 5.0;HDR=YES;DATABASE=c:/test.xls',sheet1$)
的形式。
以上语句是将EXCEL文件里SHEET1工作表中所有的列都读进来，如果只想导部分列，可以
INSERT INTO 表(a1,a2,a3) SELECT a1,a2,a3 FROM OPENROWSET('MICROSOFT.JET.OLEDB.4.0'
,'Excel 5.0;HDR=YES;DATABASE=c:/test.xls',sheet1$)
其实可以将OPENROWSET('MICROSOFT.JET.OLEDB.4.0'
,'Excel 5.0;HDR=YES;DATABASE=c:/test.xls',sheet1$)当成一个表，例如我就写过这样一个句子：
INSERT INTO eval_channel_employee(channel,employee_id) 
SELECT CASE a.渠道 WHEN 'DIY' THEN 1 WHEN 'RDC' THEN 0 WHEN 'KCM' THEN 2 ELSE 3 END
,b.id FROM 
OPENROWSET('MICROSOFT.JET.OLEDB.4.0'
,'Excel 5.0;HDR=YES;DATABASE=c:/temp/name.xls',sheet1$) AS a,pers_employee b
WHERE a.员工编码=b.code
不管是哪种方式，哪种途径，系统都会默认将第一行上的内容作为字段名。
