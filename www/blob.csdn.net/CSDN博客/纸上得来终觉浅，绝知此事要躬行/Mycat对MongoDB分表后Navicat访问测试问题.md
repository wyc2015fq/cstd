# Mycat对MongoDB分表后Navicat访问测试问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月21日 17:52:10[boonya](https://me.csdn.net/boonya)阅读数：1592








采用Mycat数据库中间件做MongoDB的单库分表后，通过Navicat去连Mycat操作，通过SQL语句查询可以轻松获取数据结果集，但是直接点击表名查询报错。推测可能有两种情况：（1）Navicat对MongoDB文档型的数据展示支持不好；（2）Mycat本身对客户端的响应适配输出未做处理。经过调试发现应该是Mycat的问题，主要是Mycat的MongoDBParser类解析的时候只支持SQLSelectStatement类型的，而不支持SHOW CREATE TABLE（对应的声明式语句类型是MySqlShowCreateTableStatement），错误输出提示“not a query sql statement”。

## SHOW CREATE TABLE

作用：显示用于创建给定表的CREATE TABLE语句。本语句对视图（trigger）也起作用。
基本语法：show create table table_name



```
mysql> SHOW CREATE TABLE show_table \G  
*************************** 1. row ***************************  
       Table: show_table  
Create Table: CREATE TABLE `show_table` (  
  `c1` int(11) NOT NULL AUTO_INCREMENT,  
  `c2` varchar(10) DEFAULT NULL,  
  `c3` int(10) DEFAULT NULL,  
  PRIMARY KEY (`c1`)  
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=latin1
```




## Navicat通过SHOW CREATE TABLE查询测试

选取两个表（t_subtable是MongoDB的表，ta是mysql数据表）分别进行查询：

![](https://img-blog.csdn.net/20170721165737931)

![](https://img-blog.csdn.net/20170721165831898)

可以看出通过Mycat查询MongoDB数据是有缺陷的。下面通过SQL查询语句去获取数据：

![](https://img-blog.csdn.net/20170721170808866)


由此可见，通过SQL select查询是可以查询分表数据集的。

## Mycat源码解析关键代码

首先我们来看MongoDB的SQL解析类，在io.mycat.backend.jdbc.mongodb.MongoSQLParser.java  类中执行的查询方法如下：



```java
public MongoData query() throws MongoSQLException {
		if (!(statement instanceof SQLSelectStatement)) {
			// return null;
			throw new IllegalArgumentException("not a query sql statement");
		}
		MongoData mongo = new MongoData();
		DBCursor c = null;
		SQLSelectStatement selectStmt = (SQLSelectStatement) statement;
		SQLSelectQuery sqlSelectQuery = selectStmt.getSelect().getQuery();
		int icount = 0;
		if (sqlSelectQuery instanceof MySqlSelectQueryBlock) {
			MySqlSelectQueryBlock mysqlSelectQuery = (MySqlSelectQueryBlock) selectStmt.getSelect().getQuery();
			if (mysqlSelectQuery.getFrom() != null) {
				BasicDBObject fields = new BasicDBObject();
				// 显示的字段
				for (SQLSelectItem item : mysqlSelectQuery.getSelectList()) {
					// System.out.println(item.toString());
					if (!(item.getExpr() instanceof SQLAllColumnExpr)) {
						if (item.getExpr() instanceof SQLAggregateExpr) {
							SQLAggregateExpr expr = (SQLAggregateExpr) item.getExpr();
							if (expr.getMethodName().equals("COUNT")||expr.getMethodName().equals("SUM")) {
								icount = 1;
								mongo.setField(getExprFieldName(expr), Types.BIGINT);
							}
							fields.put(getExprFieldName(expr), Integer.valueOf(1));
						} else {
							fields.put(getFieldName(item), Integer.valueOf(1));
						}
					}

				}

				// 表名
				SQLTableSource table = mysqlSelectQuery.getFrom();
				DBCollection coll = this._db.getCollection(table.toString());
				mongo.setTable(table.toString());

				SQLExpr expr = mysqlSelectQuery.getWhere();
				DBObject query = parserWhere(expr);
				// System.out.println(query);
				SQLSelectGroupByClause groupby = mysqlSelectQuery.getGroupBy();
				BasicDBObject gbkey = new BasicDBObject();
				if (groupby != null) {
					for (SQLExpr gbexpr : groupby.getItems()) {
						if (gbexpr instanceof SQLIdentifierExpr) {
							String name = ((SQLIdentifierExpr) gbexpr).getName();
							gbkey.put(name, Integer.valueOf(1));
						}
					}
					icount = 2;
				}
				int limitoff = 0;
				int limitnum = 0;
				if (mysqlSelectQuery.getLimit() != null) {
					limitoff = getSQLExprToInt(mysqlSelectQuery.getLimit().getOffset());
					limitnum = getSQLExprToInt(mysqlSelectQuery.getLimit().getRowCount());
				}

				if (icount == 1) {
					mongo.setCount(coll.count(query));
					mongo.setAggrResult(true);
				} else if (icount == 2) {
					BasicDBObject initial = new BasicDBObject();
					initial.put("num", 0);
					String reduce = "function (obj, prev) { " + "  prev.num++}";
					mongo.setGrouyBy(coll.group(gbkey, query, initial, reduce));
				} else {
					if ((limitoff > 0) || (limitnum > 0)) {
						c = coll.find(query, fields).skip(limitoff).limit(limitnum);
					} else {
						c = coll.find(query, fields);
					}

					SQLOrderBy orderby = mysqlSelectQuery.getOrderBy();
					if (orderby != null) {
						BasicDBObject order = new BasicDBObject();
						for (int i = 0; i < orderby.getItems().size(); i++) {
							SQLSelectOrderByItem orderitem = orderby.getItems().get(i);
							order.put(orderitem.getExpr().toString(),
									Integer.valueOf(getSQLExprToAsc(orderitem.getType())));
						}
						c.sort(order);
						// System.out.println(order);
					}
				}
				mongo.setCursor(c);
			} else {
				mongo = query2();
			}

		}
		return mongo;
}
```

上面提示只要查询类型不是SQLSelectStatement的都抛出非法参数异常了。


Mycat1.6使用的SQL解析是Druid的，在io.mycat.route.parser.druid.DruidParserFactory.java类中确实是支持了SHOW CREATE TABLE的，代码如下：



```java
public static DruidParser create(SchemaConfig schema, SQLStatement statement, SchemaStatVisitor visitor)
    {
        DruidParser parser = null;
        if (statement instanceof SQLSelectStatement)
        {
            if(schema.isNeedSupportMultiDBType())
            {
                parser = getDruidParserForMultiDB(schema, statement, visitor);

            }

            if (parser == null)
            {
                parser = new DruidSelectParser();
            }
        } else if (statement instanceof MySqlInsertStatement)
        {
            parser = new DruidInsertParser();
        } else if (statement instanceof MySqlDeleteStatement)
        {
            parser = new DruidDeleteParser();
        } else if (statement instanceof MySqlCreateTableStatement)
        {
            parser = new DruidCreateTableParser();
        } else if (statement instanceof MySqlUpdateStatement)
        {
            parser = new DruidUpdateParser();
        } else if (statement instanceof SQLAlterTableStatement)
        {
            parser = new DruidAlterTableParser();
        } else if (statement instanceof MySqlLockTableStatement) {
        	parser = new DruidLockTableParser();
        } else
        {
            parser = new DefaultDruidParser();
        }

        return parser;
    }
```
分析ShowSQL:




```java
/**
	 * 分析 SHOW SQL
	 */
	@Override
	public RouteResultset analyseShowSQL(SchemaConfig schema,
			RouteResultset rrs, String stmt) throws SQLSyntaxErrorException {
		
		String upStmt = stmt.toUpperCase();
		int tabInd = upStmt.indexOf(" TABLES");
		if (tabInd > 0) {// show tables
			int[] nextPost = RouterUtil.getSpecPos(upStmt, 0);
			if (nextPost[0] > 0) {// remove db info
				int end = RouterUtil.getSpecEndPos(upStmt, tabInd);
				if (upStmt.indexOf(" FULL") > 0) {
					stmt = "SHOW FULL TABLES" + stmt.substring(end);
				} else {
					stmt = "SHOW TABLES" + stmt.substring(end);
				}
			}
          String defaultNode=  schema.getDataNode();
            if(!Strings.isNullOrEmpty(defaultNode))
            {
             return    RouterUtil.routeToSingleNode(rrs, defaultNode, stmt);
            }
			return RouterUtil.routeToMultiNode(false, rrs, schema.getMetaDataNodes(), stmt);
		}
		
		/**
		 *  show index or column
		 */
		int[] indx = RouterUtil.getSpecPos(upStmt, 0);
		if (indx[0] > 0) {
			/**
			 *  has table
			 */
			int[] repPos = { indx[0] + indx[1], 0 };
			String tableName = RouterUtil.getShowTableName(stmt, repPos);
			/**
			 *  IN DB pattern
			 */
			int[] indx2 = RouterUtil.getSpecPos(upStmt, indx[0] + indx[1] + 1);
			if (indx2[0] > 0) {// find LIKE OR WHERE
				repPos[1] = RouterUtil.getSpecEndPos(upStmt, indx2[0] + indx2[1]);

			}
			stmt = stmt.substring(0, indx[0]) + " FROM " + tableName + stmt.substring(repPos[1]);
			RouterUtil.routeForTableMeta(rrs, schema, tableName, stmt);
			return rrs;

		}
		
		/**
		 *  show create table tableName
		 */
		int[] createTabInd = RouterUtil.getCreateTablePos(upStmt, 0);
		if (createTabInd[0] > 0) {
			int tableNameIndex = createTabInd[0] + createTabInd[1];
			if (upStmt.length() > tableNameIndex) {
				String tableName = stmt.substring(tableNameIndex).trim();
				int ind2 = tableName.indexOf('.');
				if (ind2 > 0) {
					tableName = tableName.substring(ind2 + 1);
				}
				RouterUtil.routeForTableMeta(rrs, schema, tableName, stmt);
				return rrs;
			}
		}
		
		//show variables; 这条语句只能在mysql中执行，不能在其它库，如mongodb中执行。
		if(upStmt.contains("SHOW VARIABLES")){
			//如果数据库配置了默认节点，而且默认节点是mysql类型的数据库。
			if(schema.getDataNode()!=null && schema.getDataNode().length()>0){
				if("mysql".equalsIgnoreCase(schema.getDefaultDataNodeDbType())){
					return RouterUtil.routeToSingleNode(rrs, schema.getDataNode(), stmt);
				}
			}
			
			//从mysql类型的节点中随机选择一个库.
			HashSet<String> set=new HashSet<>(schema.getAllDataNodes());
			HashMap<String, String> map=new HashMap<>(schema.getDataNodeDbTypeMap());
			for(Entry<String, String> entry:map.entrySet()){
				if(!"mysql".equalsIgnoreCase(entry.getValue())){
					set.remove(entry.getKey());
				}
			}
			if(set.size()>0){
				String[]array=new String[0];
				array=set.toArray(array);
				String node=array[new Random().nextInt(array.length)];
				return RouterUtil.routeToSingleNode(rrs, node, stmt);
			}
		}

		return RouterUtil.routeToSingleNode(rrs, schema.getRandomDataNode(), stmt);
	}
```



SQL对应查询类型处理io.mycat.server.ServerQueryHandler.java ，下面是核心方法：



```java
@Override
	public void query(String sql) {
		
		ServerConnection c = this.source;
		if (LOGGER.isDebugEnabled()) {
			LOGGER.debug(new StringBuilder().append(c).append(sql).toString());
		}
		//
		int rs = ServerParse.parse(sql);
		int sqlType = rs & 0xff;
		
		switch (sqlType) {
		//explain sql
		case ServerParse.EXPLAIN:
			ExplainHandler.handle(sql, c, rs >>> 8);
			break;
		//explain2 datanode=? sql=?
		case ServerParse.EXPLAIN2:
			Explain2Handler.handle(sql, c, rs >>> 8);
			break;
		case ServerParse.SET:
			SetHandler.handle(sql, c, rs >>> 8);
			break;
		case ServerParse.SHOW:
			ShowHandler.handle(sql, c, rs >>> 8);
			break;
		case ServerParse.SELECT:
			SelectHandler.handle(sql, c, rs >>> 8);
			break;
		case ServerParse.START:
			StartHandler.handle(sql, c, rs >>> 8);
			break;
		case ServerParse.BEGIN:
			BeginHandler.handle(sql, c);
			break;
		//不支持oracle的savepoint事务回退点
		case ServerParse.SAVEPOINT:
			SavepointHandler.handle(sql, c);
			break;
		case ServerParse.KILL:
			KillHandler.handle(sql, rs >>> 8, c);
			break;
		//不支持KILL_Query
		case ServerParse.KILL_QUERY:
			LOGGER.warn(new StringBuilder().append("Unsupported command:").append(sql).toString());
			c.writeErrMessage(ErrorCode.ER_UNKNOWN_COM_ERROR,"Unsupported command");
			break;
		case ServerParse.USE:
			UseHandler.handle(sql, c, rs >>> 8);
			break;
		case ServerParse.COMMIT:
			c.commit();
			break;
		case ServerParse.ROLLBACK:
			c.rollback();
			break;
		case ServerParse.HELP:
			LOGGER.warn(new StringBuilder().append("Unsupported command:").append(sql).toString());
			c.writeErrMessage(ErrorCode.ER_SYNTAX_ERROR, "Unsupported command");
			break;
		case ServerParse.MYSQL_CMD_COMMENT:
			c.write(c.writeToBuffer(OkPacket.OK, c.allocate()));
			break;
		case ServerParse.MYSQL_COMMENT:
			c.write(c.writeToBuffer(OkPacket.OK, c.allocate()));
			break;
        case ServerParse.LOAD_DATA_INFILE_SQL:
            c.loadDataInfileStart(sql);
            break;
		case ServerParse.MIGRATE:
			MigrateHandler.handle(sql,c);
			break;
		case ServerParse.LOCK:
        	c.lockTable(sql);
        	break;
        case ServerParse.UNLOCK:
        	c.unLockTable(sql);
        	break;
		default:
			if(readOnly){
				LOGGER.warn(new StringBuilder().append("User readonly:").append(sql).toString());
				c.writeErrMessage(ErrorCode.ER_USER_READ_ONLY, "User readonly");
				break;
			}
			c.execute(sql, rs & 0xff);
		}
	}
```



于是乎想着要将SHOW CREATE TABLE对应的MySqlShowCreateTableStatement类进行配型处理，不过改写成以下代码也不能解决Navicat数据库表不能点击的问题。



```java
public MongoData query() throws MongoSQLException {
		if (!(statement instanceof SQLSelectStatement)) {
			// 非查询语句处理
			if(statement instanceof MySqlShowCreateTableStatement){
				LOGGER.info("<<<<非查询(SELECT)语句MySqlShowCreateTableStatement>>>>"+statement.toString());
				MongoData mongo = new MongoData();
				MySqlShowCreateTableStatement createTableStatement= (MySqlShowCreateTableStatement) statement;
				SQLExpr expr = createTableStatement.getName();
				DBObject query = parserWhere(expr);
				BasicDBObject fields = new BasicDBObject();
				String table="t_subtable";
				mongo.setTable(table);
				DBCollection coll = this._db.getCollection(table);
				DBCursor dbCursor = coll.find(query, fields).skip(0).limit(100);
				mongo.setCursor(dbCursor);
				return mongo;
			}else{
				// return null;
				throw new IllegalArgumentException("not a query sql statement");
			}
		}
		MongoData mongo = new MongoData();
		DBCursor c = null;
         //......
}
```

通过上面的测试和代码可以看出：mycat支持的SQL类型里面对MongoDB支持欠佳，如果对界面操作要求不是很高可以完全采用SQL select方式进行查询。






