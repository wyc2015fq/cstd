# jdbctemplate 获取数据表结构的方法&注意事项 - z69183787的专栏 - CSDN博客
2017年11月20日 12:26:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：733
[https://www.cnblogs.com/liqiu/p/4502981.html](https://www.cnblogs.com/liqiu/p/4502981.html)
方法一 直接查询：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
SqlRowSet srcSqlRowSet = srcJdbcTemplate.queryForRowSet("SELECT * FROM tablename LIMIT 0"); //注意limit 0更合适
List<ColumnMetaData> columneMetaList = new LinkedList<ColumnMetaData>();
int columnCount;
SqlRowSetMetaData sqlRowSetMetaData = sqlRowSet.getMetaData();
columnCount = sqlRowSetMetaData.getColumnCount();
for (int i = 1; i <= columnCount; i++) {
    columneMetaList.add(new ColumnMetaData(sqlRowSetMetaData.getColumnName(i), sqlRowSetMetaData.getColumnType(i), sqlRowSetMetaData.getColumnTypeName(i))); //获取字段的名称、类型和描述
}
Collections.sort(columneMetaList);
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
方法一的ColumnMetaData类代码如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public class ColumnMetaData implements Comparable<ColumnMetaData> {
    private String name; // 字段名称
    private int type; // 字段类型
    private String typeName; // 字段类型名称
    private Object value; // 值
    
public ColumnMetaData(String columnName, int valueType, String typeName) {
        this.name = columnName;
        this.type = valueType;
        this.typeName = typeName;
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
方法二 使用RowCountCallbackHandler查询
```
String sql = "select * from "+ tableName + " limit 0";  
RowCountCallbackHandler rcch = new RowCountCallbackHandler();  
this.jdbcTemplateDao.query(sql, rcch);  
String[] coloumnName = rcch.getColumnNames();  
int[] coloumnType = rcch.getColumnTypes();
```
方法二效率高，但麻烦的是拿到了类型是数字，如果要获得字段的描述信息，比如：int还是varchar等，需要一个对照关系类：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public class SqlTypeAdapter {
public static String getTypeName(int type) throws SQLException {
        switch (type) {
            case Types.ARRAY:
                break;
            case Types.BIGINT:
                return "BIGINT";
            case Types.BINARY:
                return "BINARY";
            case Types.BIT:
                return "BIT";
            case Types.BLOB:
                return "BLOB";
            case Types.BOOLEAN:
                return "BOOLEAN";
            //..代码太多就不多写啦default:
                break;
        }
        return "VARCHAR";
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
