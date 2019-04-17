# Mycat 实现MongoDB的BETWEEN AND和多IN&NOT IN查询 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年08月24日 15:31:06[boonya](https://me.csdn.net/boonya)阅读数：5170








在实现过程中第一就是分析怎么去实现，其次在mycat哪个部分实现。首先我们知道BETWEEN AND是一个区间值查询，而IN 是一个多值且的关系，其实最终目的都是封装为MongoDB支持的数据库操作语句，如：db.getCollection('t_alarm_aggr_20170822').find({ "F_START_TIME" : { "$gte" : "2017-08-21 00:00:00" , "$lte" : "2017-08-23 23:55:00"} , "F_ID" : { "$in" : [5000000001064]}}); 这样就实现了我们的需求了。

## BETWEEN AND查询

主要操作io.mycat.route.util.RouterUtil.java类：



```java
/**
	 * @throws SQLNonTransientException 
	 * 解析betweenSQL 
	 * @MethodName: parseSQLToChangeBetweenSql 
	 * @Description: 
	 * @param ctx
	 * @throws
	 */
	private static void parseSQLToChangeBetweenSql( DruidShardingParseInfo ctx) throws SQLNonTransientException{
	      if(ctx==null){
            return; // 没有上下文:no route to find
        }
        String tempStr=new String(ctx.getSql().toString()).replaceAll(" ", "").toUpperCase().trim();
        String endSql="";
		if(tempStr.contains("NOTBETWEEN")){
			throw new IllegalArgumentException("Not support' WHERE $condition NOT BETWEEN $a AND $b ' sql statement :"+ctx.getSql());
		}else if(tempStr.contains("BETWEEN")){
			String sql=ctx.getSql().toUpperCase();
			String conditions=StringUtil.keepInnerSpaceStr(sql.substring(sql.indexOf("WHERE")+5, sql.length())); 
			String strs[] =conditions.split("AND");
			String conditionField="";
			String values[]=new String [2];
			int index=-1;
			for (int i = 0; i < strs.length; i++) {
				if(strs[i].contains("BETWEEN")){
					String temp=StringUtil.keepInnerSpaceStr(strs[i]);
					conditionField=temp.split(" ")[0];
					values[0]=temp.split(" ")[2]+" "+temp.split(" ")[3];
					values[1]=StringUtil.keepInnerSpaceStr(strs[i+1]);
					index=i;
					break;
				}
			}
			Date dates[]=new Date [2];
			for (int i = 0; i < values.length; i++) {
				try {
					dates[i]=DateUtil.parseDate(StringUtil.removeBackquote(values[i]), DateUtil.DEFAULT_DATE_PATTERN); 
				} catch (ParseException e) {
					throw new SQLNonTransientException("date time format must be:"+DateUtil.DEFAULT_DATE_PATTERN);
				}
			}
			StringBuffer changeSql=new StringBuffer();
			if(dates[0].getTime()<dates[1].getTime()){
				changeSql.append(conditionField+" >= "+values[0]).append(" AND ").append(conditionField+" <= "+values[1]);
			}else{
				changeSql.append(conditionField+" <= "+values[0]).append(" AND ").append(conditionField+" >= "+values[1]);
			}
			String newSql=StringUtil.keepInnerSpaceStr(ctx.getSql().substring(0,sql.indexOf("WHERE")+5));
			for (int i = 0; i < strs.length; i++) {
				if(i<index){
					newSql+=" "+strs[i]+" AND ";
				}else if(i==index){
					newSql+=" "+changeSql.toString();
				}else if(i>index+1){
					newSql+=" AND "+strs[i];
				}
			}
			ctx.setSql(newSql);
		}
	}
```

寻找路由调用：




```java
/**
	 * 多表路由
	 */
	public static RouteResultset tryRouteForTables(SchemaConfig schema, DruidShardingParseInfo ctx,
			RouteCalculateUnit routeUnit, RouteResultset rrs, boolean isSelect, LayerCachePool cachePool)
			throws SQLNonTransientException {
		
		List<String> tables = ctx.getTables();
		
		if(schema.isNoSharding()||(tables.size() >= 1&&isNoSharding(schema,tables.get(0)))) {
			if(Pattern.matches(".*_201\\d{5,5}", tables.get(0))){//支持分表, t_location_20170704这种表，删除结尾的_20170704后再查找路由
				TableConfig tc=schema.getTables().get( tables.get(0).substring(0,  tables.get(0).length()-9));
				return routeToSingleNode(rrs, tc.getDataNodes().get(0), ctx.getSql());
			}
			return routeToSingleNode(rrs, schema.getDataNode(), ctx.getSql());
		}
		
		// 处理BETWEEN AND 查询
		SQLStatementParser parser = new MySqlStatementParser(ctx.getSql()); 
		SQLStatement statement = parser.parseStatement(); 
		if(statement instanceof SQLSelectStatement){
			// =======between and query to change SQL=======
			parseSQLToChangeBetweenSql(ctx);
			rrs.setStatement(ctx.getSql());
		}

		//只有一个表的
		if(tables.size() == 1) {
			return RouterUtil.tryRouteForOneTable(schema, ctx, routeUnit, tables.get(0), rrs, isSelect, cachePool);
		}

		Set<String> retNodesSet = new HashSet<String>();
		//每个表对应的路由映射
		Map<String,Set<String>> tablesRouteMap = new HashMap<String,Set<String>>();

		//分库解析信息不为空
		Map<String, Map<String, Set<ColumnRoutePair>>> tablesAndConditions = routeUnit.getTablesAndConditions();
		if(tablesAndConditions != null && tablesAndConditions.size() > 0) {
			//为分库表找路由
			RouterUtil.findRouteWithcConditionsForTables(schema, rrs, tablesAndConditions, tablesRouteMap, ctx.getSql(), cachePool, isSelect);
			if(rrs.isFinishedRoute()) {
				return rrs;
			}
		}

		//为全局表和单库表找路由
		for(String tableName : tables) {
			TableConfig tableConfig = schema.getTables().get(tableName.toUpperCase());
			if(tableConfig==null && Pattern.matches(".*_201\\d{5,5}", tableName)){//支持分表, t_location_20170704这种表，支持结尾的_20170704后再查找路由
				tableConfig=schema.getTables().get(tableName.substring(0, tableName.length()-9));
			}
			if(tableConfig == null) {
				String msg = "can't find table define in schema "+ tableName + " schema:" + schema.getName();
				LOGGER.warn(msg);
				throw new SQLNonTransientException(msg);
			}
			if(tableConfig.isGlobalTable()) {//全局表
				if(tablesRouteMap.get(tableName) == null) {
					tablesRouteMap.put(tableName, new HashSet<String>());
				}
				tablesRouteMap.get(tableName).addAll(tableConfig.getDataNodes());
			} else if(tablesRouteMap.get(tableName) == null) { //余下的表都是单库表
				tablesRouteMap.put(tableName, new HashSet<String>());
				tablesRouteMap.get(tableName).addAll(tableConfig.getDataNodes());
			}
		}

		boolean isFirstAdd = true;
		for(Map.Entry<String, Set<String>> entry : tablesRouteMap.entrySet()) {
			if(entry.getValue() == null || entry.getValue().size() == 0) {
				throw new SQLNonTransientException("parent key can't find any valid datanode ");
			} else {
				if(isFirstAdd) {
					retNodesSet.addAll(entry.getValue());
					isFirstAdd = false;
				} else {
					retNodesSet.retainAll(entry.getValue());
					if(retNodesSet.size() == 0) {//两个表的路由无交集
						String errMsg = "invalid route in sql, multi tables found but datanode has no intersection "
								+ " sql:" + ctx.getSql();
						LOGGER.warn(errMsg);
						throw new SQLNonTransientException(errMsg);
					}
				}
			}
		}

		if(retNodesSet != null && retNodesSet.size() > 0) {
			String tableName = tables.get(0);
			TableConfig tableConfig = schema.getTables().get(tableName.toUpperCase());
			if(tableConfig==null && Pattern.matches(".*_201\\d{5,5}", tableName)){//支持分表, t_location_20170704这种表，支持结尾的_20170704后再查找路由
				tableConfig=schema.getTables().get(tableName.substring(0, tableName.length()-9));
			}
			if(tableConfig.isDistTable()){
				routeToDistTableNode(tableName,schema, rrs, ctx.getSql(), tablesAndConditions, cachePool, isSelect);
				return rrs;
			}
			
			if(retNodesSet.size() > 1 && isAllGlobalTable(ctx, schema)) {
				// mulit routes ,not cache route result
				if (isSelect) {
					rrs.setCacheAble(false);
					routeToSingleNode(rrs, retNodesSet.iterator().next(), ctx.getSql());
				}
				else {//delete 删除全局表的记录
					routeToMultiNode(isSelect, rrs, retNodesSet, ctx.getSql(),true);
				}

			} else {
				routeToMultiNode(isSelect, rrs, retNodesSet, ctx.getSql());
			}

		}
		return rrs;

	}
```
关键调用：




```java
// 处理BETWEEN AND 查询
		SQLStatementParser parser = new MySqlStatementParser(ctx.getSql()); 
		SQLStatement statement = parser.parseStatement(); 
		if(statement instanceof SQLSelectStatement){
			// =======between and query to change SQL=======
			parseSQLToChangeBetweenSql(ctx);
			rrs.setStatement(ctx.getSql());
		}
```




## 多个WHERE IN 查询

支持多个IN查询，主要操作io.mycat.backend.jdbc.mongodb.MongoSQLParser.java类：



```java
/**
     * WHERE IN 查询
     * @MethodName: parseWhereIn 
     * @Description: 
     * @param expr
     * @param query
     * @throws
     */
    private void parseWhereIn(SQLSelectStatement selectStmt,SQLExpr expr,DBObject query ){
        if(expr==null){
            return; // 没有条件:no where conditions return this
        }
        String conditionStr=new String(expr.toString()).replaceAll(" ", "").toUpperCase().trim();
        if(conditionStr.contains("NOTIN(")){
            throw new IllegalArgumentException("Not support' WHERE $condition NOT IN ' sql statement");
        }else if(conditionStr.contains("IN(")){
            String sql=new String(selectStmt.getSelect().toString()).toUpperCase();
            String conditions=StringUtil.keepInnerSpaceStr(sql.substring(sql.lastIndexOf("WHERE")+5, sql.length())); 
            String manyIns[]=conditions.split("AND ");
            for (int a = 0; a < manyIns.length; a++) {
                if(manyIns[a].contains(" IN")){
                    String filedStr=StringUtil.keepInnerSpaceStr(manyIns[a].substring(0,manyIns[a].lastIndexOf("IN")));
                    String currentConditions=StringUtil.keepInnerSpaceStr(manyIns[a].substring(manyIns[a].lastIndexOf("(")+1, manyIns[a].lastIndexOf(")")));
                    String strs[]=filedStr.split(" ");
                    String field="";
                    for (int i = strs.length-1; i >=0 ; i++) {
                        if(!"".equals(strs[i].trim())){
                            field=strs[i].trim();
                            break;
                        }
                    }
                    String [] values=currentConditions.split(",");
                    BasicDBList blist=new BasicDBList();
                    for (int i = 0; i < values.length; i++) {
                        if(ValidateUtils.Number(values[i].trim())){
                            blist.add(Long.valueOf(values[i].trim()));
                        }else{
                            blist.add(values[i].trim());
                        }
                    }
                    BasicDBObject obj=new BasicDBObject();
                    obj.put("$in", blist);
                    query.put(field, obj);
                }
            }
        }
    }
```

支持NOT IN 查询：



```java
/**
	 * WHERE IN 查询
	 * @MethodName: parseWhereIn 
	 * @Description: 
	 * @param expr
	 * @param query
	 * @throws
	 */
	private void parseWhereIn(SQLSelectStatement selectStmt,SQLExpr expr,DBObject query ){
		if(expr==null){
			return; // 没有条件:no where conditions return this
		}
		String conditionStr=new String(expr.toString()).replaceAll(" ", "").toUpperCase().trim();
		if(conditionStr.contains("IN(")||conditionStr.contains("NOTIN(")){
			String sql=new String(selectStmt.getSelect().toString()).toUpperCase();
			String conditions=StringUtil.keepInnerSpaceStr(sql.substring(sql.lastIndexOf("WHERE")+5, sql.length())); 
			String manyIns[]=conditions.split("AND");
			for (int a = 0; a < manyIns.length; a++) {
				String temp=new String(manyIns[a]).trim();
				if(temp.contains(" IN")&&!temp.contains("NOT IN")){
					String filedStr=StringUtil.keepInnerSpaceStr(manyIns[a].substring(0,manyIns[a].lastIndexOf(" IN")));
					String currentConditions=StringUtil.keepInnerSpaceStr(manyIns[a].substring(manyIns[a].lastIndexOf("(")+1, manyIns[a].lastIndexOf(")")));
					String strs[]=filedStr.split(" ");
					String field="";
					for (int i = strs.length-1; i >=0 ; i++) {
						if(!"".equals(strs[i].trim())){
							field=strs[i].trim();
							break;
						}
					}
					String [] values=currentConditions.split(",");
					BasicDBList blist=new BasicDBList();
					for (int i = 0; i < values.length; i++) {
						if(ValidateUtils.Number(values[i].trim())){
							blist.add(Long.valueOf(values[i].trim()));
						}else{
							blist.add(values[i].trim());
						}
					}
					BasicDBObject obj=new BasicDBObject();
					obj.put("$in", blist);
					query.put(field, obj);
				}else if(temp.contains(" IN")&&temp.contains("NOT IN")){
					String filedStr=StringUtil.keepInnerSpaceStr(manyIns[a].substring(0,manyIns[a].lastIndexOf("NOT IN")));
					String currentConditions=StringUtil.keepInnerSpaceStr(manyIns[a].substring(manyIns[a].lastIndexOf("(")+1, manyIns[a].lastIndexOf(")")));
					String strs[]=filedStr.split(" ");
					String field="";
					for (int i = strs.length-1; i >=0 ; i++) {
						if(!"".equals(strs[i].trim())){
							field=strs[i].trim();
							break;
						}
					}
					String [] values=currentConditions.split(",");
					BasicDBList blist=new BasicDBList();
					for (int i = 0; i < values.length; i++) {
						if(ValidateUtils.Number(values[i].trim())){
							blist.add(Long.valueOf(values[i].trim()));
						}else{
							blist.add(values[i].trim());
						}
					}
					BasicDBObject obj=new BasicDBObject();
					obj.put("$nin", blist);
					query.put(field, obj);
				}
			}
		}
	}
```
执行查询调用：






```java
public MongoData query() throws MongoSQLException, SQLNonTransientException {
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
				
				// parse where IN query 
				parseWhereIn(selectStmt, expr, query);
				// parse where IN query 
				
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
					/***********distinct***********/
					if(selectStmt.getSelect().toString().toLowerCase().contains("distinct")){
						DBCursor cursor=coll.find(query);
						Map<String,DBObject> distinctMap=new HashMap<String,DBObject>();
						for (DBObject dbObject : cursor) {
							String key=dbObject.toString().hashCode()+"";
							if(!distinctMap.containsKey(key)){
								distinctMap.put(key, dbObject);
							}
						}
						mongo.setCount(distinctMap.size());
					}else{
						mongo.setCount(coll.count(query));
					}
					/***********distinct***********/
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

核心调用：




```java
// 表名
				SQLTableSource table = mysqlSelectQuery.getFrom();
				DBCollection coll = this._db.getCollection(table.toString());
				mongo.setTable(table.toString());
				SQLExpr expr = mysqlSelectQuery.getWhere();
				DBObject query = parserWhere(expr);
				
				// parse where IN query 
				parseWhereIn(selectStmt, expr, query);
				// parse where IN query
```

## 实现分段查询原理

看完下面的内容就明白了：



```
db.getCollection('t_alarm_aggr_20170822').find({});

db.getCollection('t_alarm_aggr_20170822').find({ "F_START_TIME" : { "$gte" : "2017-08-21 00:00:00" , "$lte" : "2017-08-23 23:55:00"}});

db.getCollection('t_alarm_aggr_20170822').find({ "F_START_TIME" : { "$gte" : "2017-08-21 00:00:00" , "$lte" : "2017-08-23 23:55:00"} , "F_ID" : { "$in" : [5000000001063,5000000001064]}}).sort({"F_START_TIME":-1,"F_ALARM_ID":-1});
```

注：实际上只要满足mongodb的shell命令就可以了。





## 测试验证修改效果

BETWEEN AND:




```
mysql> SELECT F_ID, F_VEHICLE_ID, F_ALARMBAK, F_START_TIME, F_START_ID, F_END_TIME, F_STATUS, F_DSpeed, F_Speed, F_ALARM_ID, F_ENTERPRISE_ID FROM T_ALARM_AGGR WHERE F_START_TIME BETWEEN '2017-08-21 00:00:00' AND  '2017-08-23 23:55:00';
ERROR 2006 (HY000): MySQL server has gone away
No connection. Trying to reconnect...
Connection id:    108
Current database: cvnavidb

+---------------+------------------+------------+---------------------+---------------+---------------------+----------+----------+---------+------------+-----------------+
| F_ID          | F_VEHICLE_ID     | F_ALARMBAK | F_START_TIME        | F_START_ID    | F_END_TIME          | F_STATUS | F_DSpeed | F_Speed | F_ALARM_ID | F_ENTERPRISE_ID |
+---------------+------------------+------------+---------------------+---------------+---------------------+----------+----------+---------+------------+-----------------+
| 5000000001063 | 4908520417546251 | NULL       | 2017-08-21 00:26:00 | 5000000468156 | 2017-08-21 00:26:00 |        0 | NULL     | NULL    |         48 | NULL            |
| 5000000001064 | 4908520417546251 | NULL       | 2017-08-21 00:26:01 | 5000000468160 | 2017-08-21 00:26:01 |        0 | NULL     | NULL    |         49 | NULL            |
| 5000000001063 | 4908520417546251 | NULL       | 2017-08-21 10:26:00 | 5000000468156 | 2017-08-21 10:26:00 |        0 | NULL     | NULL    |         48 | NULL            |
| 5000000001063 | 4908520417546251 | NULL       | 2017-08-22 00:26:00 | 5000000468156 | 2017-08-22 00:26:00 |        0 | NULL     | NULL    |         48 | NULL            |
| 5000000001064 | 4908520417546251 | NULL       | 2017-08-22 00:26:01 | 5000000468160 | 2017-08-22 00:26:01 |        0 | NULL     | NULL    |         49 | NULL            |
| 5000000001063 | 4908520417546251 | NULL       | 2017-08-22 10:26:00 | 5000000468156 | 2017-08-22 10:26:00 |        0 | NULL     | NULL    |         48 | NULL            |
| 5000000001063 | 4908520417546251 | NULL       | 2017-08-23 00:26:00 | 5000000468156 | 2017-08-23 00:26:00 |        0 | NULL     | NULL    |         48 | NULL            |
| 5000000001064 | 4908520417546251 | NULL       | 2017-08-23 00:26:01 | 5000000830671 | 2017-08-23 00:26:01 |        0 | NULL     | NULL    |         49 | NULL            |
+---------------+------------------+------------+---------------------+---------------+---------------------+----------+----------+---------+------------+-----------------+
8 rows in set (0.25 sec)

mysql>
```

WHERE IN:




```
mysql> SELECT F_ID, F_VEHICLE_ID, F_ALARMBAK, F_START_TIME, F_START_ID, F_END_TIME, F_STATUS, F_DSpeed, F_Speed, F_ALARM_ID, F_ENTERPRISE_ID FROM T_ALARM_AGGR WHERE F_START_TIME >= '2017-08-21 00:00:00' AND  F_START_TIME<='2017-08-23 23:55:00'  AND F_ID IN (5000000001064);
+---------------+------------------+------------+---------------------+---------------+---------------------+----------+----------+---------+------------+-----------------+
| F_ID          | F_VEHICLE_ID     | F_ALARMBAK | F_START_TIME        | F_START_ID    | F_END_TIME          | F_STATUS | F_DSpeed | F_Speed | F_ALARM_ID | F_ENTERPRISE_ID |
+---------------+------------------+------------+---------------------+---------------+---------------------+----------+----------+---------+------------+-----------------+
| 5000000001064 | 4908520417546251 | NULL       | 2017-08-22 00:26:01 | 5000000468160 | 2017-08-22 00:26:01 |        0 | NULL     | NULL    |         49 | NULL            |
| 5000000001064 | 4908520417546251 | NULL       | 2017-08-21 00:26:01 | 5000000468160 | 2017-08-21 00:26:01 |        0 | NULL     | NULL    |         49 | NULL            |
| 5000000001064 | 4908520417546251 | NULL       | 2017-08-23 00:26:01 | 5000000830671 | 2017-08-23 00:26:01 |        0 | NULL     | NULL    |         49 | NULL            |
+---------------+------------------+------------+---------------------+---------------+---------------------+----------+----------+---------+------------+-----------------+
3 rows in set (0.00 sec)

mysql> SELECT F_ID, F_VEHICLE_ID, F_ALARMBAK, F_START_TIME, F_START_ID, F_END_TIME, F_STATUS, F_DSpeed, F_Speed, F_ALARM_ID, F_ENTERPRISE_ID FROM T_ALARM_AGGR WHERE F_START_TIME >= '2017-08-21 00:00:00' AND  F_START_TIME<='2017-08-23 23:55:00'  AND F_ID IN (50000000010613,5000000001064);
+---------------+------------------+------------+---------------------+---------------+---------------------+----------+----------+---------+------------+-----------------+
| F_ID          | F_VEHICLE_ID     | F_ALARMBAK | F_START_TIME        | F_START_ID    | F_END_TIME          | F_STATUS | F_DSpeed | F_Speed | F_ALARM_ID | F_ENTERPRISE_ID |
+---------------+------------------+------------+---------------------+---------------+---------------------+----------+----------+---------+------------+-----------------+
| 5000000001063 | 4908520417546251 | NULL       | 2017-08-23 00:26:00 | 5000000468156 | 2017-08-23 00:26:00 |        0 | NULL     | NULL    |         48 | NULL            |
| 5000000001064 | 4908520417546251 | NULL       | 2017-08-23 00:26:01 | 5000000830671 | 2017-08-23 00:26:01 |        0 | NULL     | NULL    |         49 | NULL            |
| 5000000001063 | 4908520417546251 | NULL       | 2017-08-22 00:26:00 | 5000000468156 | 2017-08-22 00:26:00 |        0 | NULL     | NULL    |         48 | NULL            |
| 5000000001064 | 4908520417546251 | NULL       | 2017-08-22 00:26:01 | 5000000468160 | 2017-08-22 00:26:01 |        0 | NULL     | NULL    |         49 | NULL            |
| 5000000001063 | 4908520417546251 | NULL       | 2017-08-21 00:26:00 | 5000000468156 | 2017-08-21 00:26:00 |        0 | NULL     | NULL    |         48 | NULL            |
| 5000000001063 | 4908520417546251 | NULL       | 2017-08-22 10:26:00 | 5000000468156 | 2017-08-22 10:26:00 |        0 | NULL     | NULL    |         48 | NULL            |
| 5000000001064 | 4908520417546251 | NULL       | 2017-08-21 00:26:01 | 5000000468160 | 2017-08-21 00:26:01 |        0 | NULL     | NULL    |         49 | NULL            |
| 5000000001063 | 4908520417546251 | NULL       | 2017-08-21 10:26:00 | 5000000468156 | 2017-08-21 10:26:00 |        0 | NULL     | NULL    |         48 | NULL            |
+---------------+------------------+------------+---------------------+---------------+---------------------+----------+----------+---------+------------+-----------------+
8 rows in set (0.01 sec)
```

多个WHERE IN:

![](https://img-blog.csdn.net/20170828183940543)


注：上面的测试注意验证功能实现，实际开发中肯定有问题需要做更细致的调整。

**修改过源码后打包请备份jar包到其他路径并删除原来的jar在进行上传！ 下图是因为没有删除导致修改程序包不起作用：**

![](https://img-blog.csdn.net/20170828143818351)






