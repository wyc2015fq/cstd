# 使用alibaba Druid 对sql 语句进行校验 - z69183787的专栏 - CSDN博客
2017年10月18日 17:12:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4951
使用到了 Druid中的 实现了 访问者模式的 各类visitor 对 sql 进行读取与解析
```
<dependency>
	<groupId>com.alibaba</groupId>
	<artifactId>druid</artifactId>
        <version>1.0.17</version>
</dependency>
```
```java
List<SqlDto> results = new ArrayList<SqlDto>();
		MySqlStatementParser parser = new MySqlStatementParser(sqls);
		List<SQLStatement> stmtList = parser.parseStatementList();
		// 将AST通过visitor输出
		StringBuilder out = new StringBuilder();
		MySqlOutputVisitor visitor = new MySqlOutputVisitor(out);
		for (SQLStatement stmt : stmtList) {
			SqlDto sqlDto = new SqlDto();
			stmt.accept(visitor);
			out.append(";");
			if (stmt instanceof SQLSelectStatement) {
				sqlDto.setType(SqlType.SELECT);
				sqlDto.setSql(out.toString());
			} else if (stmt instanceof SQLInsertStatement) {
				sqlDto.setType(SqlType.INSERT);
				sqlDto.setSql(out.toString());
			} else if (stmt instanceof SQLUpdateStatement) {
				sqlDto.setType(SqlType.UPDATE);
				sqlDto.setSql(out.toString());
			} else if (stmt instanceof SQLDeleteStatement) {
				sqlDto.setType(SqlType.DELETE);
				sqlDto.setSql(out.toString());
			} else if (stmt instanceof SQLAlterTableStatement) {
				sqlDto.setType(SqlType.ALTER);
				sqlDto.setSql(out.toString());
				SQLAlterTableStatement alterStmt = (SQLAlterTableStatement) stmt;
				String tableName = alterStmt.getTableSource().toString().trim();
				int pos = tableName.indexOf('`');
				if (pos >= 0) {
					int end = tableName.lastIndexOf('`');
					tableName = tableName.substring(1, end);
				}
				sqlDto.setTableName(tableName);
			} else if (stmt instanceof SQLCreateTableStatement) {
				sqlDto.setType(SqlType.CREATE);
				sqlDto.setSql(out.toString());
			} else if (stmt instanceof SQLExplainStatement) {
				sqlDto.setType(SqlType.EXPLAIN);
				sqlDto.setSql(out.toString());
			} else if (stmt instanceof SQLDropTableStatement) {
				sqlDto.setType(SqlType.DROP);
				sqlDto.setSql(out.toString());
			} else if (stmt instanceof MySqlRenameTableStatement) {
				sqlDto.setType(SqlType.RENAME);
				sqlDto.setSql(out.toString());
			} else if (stmt instanceof MySqlReplaceStatement) {
				sqlDto.setType(SqlType.REPLACE);
				sqlDto.setSql(out.toString());
			} else {
				sqlDto.setType(SqlType.OTHER);
				sqlDto.setSql(out.toString());
			}
			results.add(sqlDto);
			out.setLength(0);
		}
		return results;
```
```java
public class SqlExplain {
    public static void main(String[] args){
        String sqls = "select ID from BCP_Prize; sel2ect name from BCP_Prize";
        MySqlStatementParser parser = new MySqlStatementParser(sqls);
        List<SQLStatement> stmtList = parser.parseStatementList();
        // 将AST通过visitor输出
        StringBuilder out = new StringBuilder();
        MySqlOutputVisitor visitor = new MySqlOutputVisitor(out);
        for (SQLStatement stmt : stmtList) {
            stmt.accept(visitor);
            System.out.println(out + ";");
            out.setLength(0);
        }
    }
}
```
![](https://img-blog.csdn.net/20171018174452888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
