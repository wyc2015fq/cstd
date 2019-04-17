# Mycat 源码修改-实现分表规则：按天分表和取摸分表查询 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月24日 23:12:37[boonya](https://me.csdn.net/boonya)阅读数：3088








Mycat 源码修改-实现分表规则：按天和取摸功能。之前修改过源码，发现其实没什么高深的只需要自己耐心点，多花点时间去调试就可以做到了。通过调试，找到自己想要改的地方，这是关键的：在代码中表现为修改相应的配置规则、相关的路由直达问题。按天分表的在我的[前一篇博客（Mycat 开发调试环境配置-开启二次开发之路）](http://blog.csdn.net/boonya/article/details/75365905)中有讲述，只是这里的规则改了下，所以不再主讲按天分表。这里重点讲的是取摸分表查询。

## TableConfig构造改进

### 新增io.mycat.config.model.TableConfig.java类属性



```java
private final String subTables;
private final String subTableWay;
private final String deliveryField;
```



### io.mycat.config.model.TableConfig.java构造方法



```java
public TableConfig(String name, String primaryKey, boolean autoIncrement,boolean needAddLimit, int tableType,
			String dataNode,Set<String> dbType, RuleConfig rule, boolean ruleRequired,
			TableConfig parentTC, boolean isChildTable, String joinKey,
			String parentKey,String subTables,String subTableWay,String deliveryField) {
		
		if (name == null) {
			throw new IllegalArgumentException("table name is null");
		} else if (dataNode == null) {
			throw new IllegalArgumentException("dataNode name is null");
		}
		this.subTables=subTables;
		this.subTableWay=subTableWay;
		this.deliveryField=deliveryField;
		this.primaryKey = primaryKey;
		this.autoIncrement = autoIncrement;
		this.needAddLimit=needAddLimit;
		this.tableType = tableType;
		this.dbTypes=dbType;
		if (ruleRequired && rule == null) {
			throw new IllegalArgumentException("ruleRequired but rule is null");
		}

		this.name = name.toUpperCase();
		
		String theDataNodes[] = SplitUtil.split(dataNode, ',', '$', '-');
		if (theDataNodes == null || theDataNodes.length <= 0) {
			throw new IllegalArgumentException("invalid table dataNodes: " + dataNode);
		}
		dataNodes = new ArrayList<String>(theDataNodes.length);
		for (String dn : theDataNodes) {
			dataNodes.add(dn);
		}
		
		if(subTables!=null && !subTables.equals("")){
			if(subTableWay==null ||subTableWay.equals("")||subTableWay.equals("DEFAULT")){
				// 原始按顺序分表方式
				String sTables[] = SplitUtil.split(subTables, ',', '$', '-');
				if (sTables == null || sTables.length <= 0) {
					throw new IllegalArgumentException("invalid table subTables");
				}
				this.distTables = new ArrayList<String>(sTables.length);
				for (String table : sTables) {
					LOGGER.info("默认分表信息——dataNode："+dataNode+",table:"+table); 
					distTables.add(table);
				}
			}else if(subTableWay.equals("BYDELIVERY")){
				// 取摸分表方式
				String sTables[] = SplitUtil.split(subTables, ',', '$', '-');
				if (sTables == null || sTables.length <= 0) {
					throw new IllegalArgumentException("invalid table subTables");
				}
				if(deliveryField==null||deliveryField.equals("")){
					throw new IllegalArgumentException("invalid table deliveryField");
				}
				this.distTables = new ArrayList<String>(sTables.length);
				for (String table : sTables) {
					LOGGER.info("取摸分表信息——dataNode："+dataNode+",table:"+table); 
					distTables.add(table);
				}
			}else if(subTableWay.equals("BYDATE")){
				// 按日分表方式
				String str=subTables.substring(subTables.indexOf("$")+1, subTables.length());
				String splitChar="-";
				if(str==null||str.length() <= 0||!str.contains(splitChar)){
					throw new IllegalArgumentException("invalid table subTables:you should set date like $20170717-20180716 or $20170717-?");
				}
				String [] strDates=str.split("-");
				if(strDates.length!=2) {
					throw new IllegalArgumentException("invalid table subTables:you should set date like $20170717-20180716 or $20170717-?");
				}
				String dateStart=strDates[0];//开始日期
				String dateEnd=strDates[1];// 结束日期
				if(dateStart.equals(dateEnd)){
					throw new IllegalArgumentException("invalid table subTables:you should set date like $20170717-20180716 or $20170717-?");
				}
				String  dateFormat="yyyyMMdd";
				Date begin=null;
				Date end=null;
				try {
					begin=DateUtils.parseDate(dateStart, dateFormat);
					end=DateUtils.parseDate(dateEnd.equals("?")?DateUtils.getStrFormTime(dateFormat, new Date()):dateEnd, dateFormat);
				} catch (ParseException e) {
					throw new IllegalArgumentException("invalid table subTables:you should set date like $20170717-20180716 or $20170717-?");
				}
				if(begin.getTime()>end.getTime()){
					throw new IllegalArgumentException("invalid table subTables:start date can't bigger than end date");
				}
				// 生成对应的按日期表集合
				List<Date> dates=DateUtils.getBetweenDates(begin, end);
				this.distTables = new ArrayList<String>(dates.size());
				String tablePrex=subTables.substring(0, subTables.indexOf("$"));
				for (Date date : dates) {
					String table=tablePrex+DateUtils.getStrFormTime(dateFormat, date);
					LOGGER.info("按天分表信息——dataNode："+dataNode+",table:"+table); 
					distTables.add(table);
				}
			}else{
				throw new IllegalArgumentException("invalid table subTableWay");
			}
		}else{
			this.distTables = new ArrayList<String>();
		}	
		
		this.rule = rule;
		this.partitionColumn = (rule == null) ? null : rule.getColumn();
		partionKeyIsPrimaryKey=(partitionColumn==null)?primaryKey==null:partitionColumn.equals(primaryKey);
		this.ruleRequired = ruleRequired;
		this.childTable = isChildTable;
		this.parentTC = parentTC;
		this.joinKey = joinKey;
		this.parentKey = parentKey;
		if (parentTC != null) {
			locateRTableKeySql = genLocateRootParentSQL();
			secondLevel = (parentTC.parentTC == null);
		} else {
			locateRTableKeySql = null;
			secondLevel = false;
		}
}
```
注：建议构造重写，构造函数涉及io.mycat.config.loader.xml.XMLSchemaLoader.java类的修改。



## Schema配置新增Table标签属性

### schema.dtd定义table标签新增属性

新增属性字段：subTableWay和deliveryField




```
<!ELEMENT table (property*,(childTable*))>
<!ATTLIST table name CDATA #REQUIRED>
<!ATTLIST table nameSuffix CDATA #IMPLIED>
<!ATTLIST table dataNode CDATA #REQUIRED>
<!ATTLIST table rule NMTOKEN #IMPLIED>
<!ATTLIST table ruleRequired NMTOKEN #IMPLIED>
<!ATTLIST table primaryKey NMTOKEN #IMPLIED>
<!ATTLIST table subTables CDATA #IMPLIED>
<!ATTLIST table subTableWay CDATA #IMPLIED>
<!ATTLIST table deliveryField CDATA #IMPLIED>
<!ATTLIST table autoIncrement NMTOKEN #IMPLIED>
<!ATTLIST table needAddLimit NMTOKEN #IMPLIED>
<!ATTLIST table type NMTOKEN #IMPLIED>
```




### schema.xml连上MongoDB的配置



```
<?xml version="1.0"?>
<!DOCTYPE mycat:schema SYSTEM "schema.dtd">
<mycat:schema xmlns:mycat="http://io.mycat/">

    <!-- **********默认分表   start   -->
    <!-- subTableWay: DEFAULT(默认) BYDATE（按天）  BYDELIVERY(取摸)-->
    <!--  <table name="t_subtable" primaryKey="_ID"   subTables="t_subtable_$1-5"  dataNode="dn1" /> -->
    <!--  <table name="t_subtable" primaryKey="_ID"   subTables="t_subtable_$1-5" subTableWay="DEFAULT"  dataNode="dn1" /> -->
    <!-- **********默认分表   end   -->
    
    <!-- **********按天分表   start   -->
    <!-- subTableWay: DEFAULT(默认) BYDATE（按天,?:表示今天的日期字符串）  BYDELIVERY(取摸)-->
    <!--  <table name="t_subtable" primaryKey="_ID"   subTables="t_subtable_$20170701-20170901" subTableWay="BYDATE" dataNode="dn1" /> -->
    <!--  <table name="t_subtable" primaryKey="_ID"   subTables="t_subtable_$20170701-?" subTableWay="BYDATE" dataNode="dn1" /> -->
    <!-- **********按天分表    end   -->
    
    <!-- **********取摸分表   start   -->
    <!-- subTableWay: DEFAULT(默认) BYDATE（按天）  BYDELIVERY(取摸)-->
    <!--  deliveryField： 标识取摸的字段，如车辆ID -->
    <!--  <table name="t_subtable" primaryKey="_ID"   subTables="t_subtable_$1-5" subTableWay="BYDELIVERY" deliveryField="vehicle_id" dataNode="dn1" /> -->
    <!-- **********取摸分表   end   -->
    
	 <schema name="TESTDB" checkSQLschema="false" sqlMaxLimit="100">
             <table name="people" primaryKey="_ID"   dataNode="dn1" /> 
             <!-- subTableWay: DEFAULT(默认) BYDATE（按天）  BYDELIVERY(取摸)-->
             <table name="t_subtable" primaryKey="_ID"   subTables="t_subtable_$1-5" subTableWay="BYDELIVERY" deliveryField="vehicle_id" dataNode="dn1" />
      </schema>

      <dataNode name="dn1" dataHost="localhost1" database="mongo-test-db" /> 

      <dataHost name="localhost1" maxCon="1000" minCon="1" balance="0" writeType="0" dbType="mongodb" dbDriver="jdbc">
			<heartbeat>select user()</heartbeat>
			<writeHost host="hostM" url="mongodb://192.168.1.4/" user="admin" password="123456" ></writeHost>
	  </dataHost>
		
</mycat:schema>
```




## 路由到分表子表的取摸改进

据下图进行一步步分析 ：

![](https://img-blog.csdn.net/20170724230456249)


### io.mycat.route.impl.DruidMycatRouteStrategy.java类解析路由策略



```java
public RouteResultset routeNormalSqlWithAST(SchemaConfig schema,
			String stmt, RouteResultset rrs, String charset,
			LayerCachePool cachePool) throws SQLNonTransientException {
		
		/**
		 *  只有mysql时只支持mysql语法
		 */
		SQLStatementParser parser = null;
		if (schema.isNeedSupportMultiDBType()) {
			parser = new MycatStatementParser(stmt);
		} else {
			parser = new MySqlStatementParser(stmt); 
		}

		MycatSchemaStatVisitor visitor = null;
		SQLStatement statement;
		
		/**
		 * 解析出现问题统一抛SQL语法错误
		 */
		try {
			statement = parser.parseStatement();
            visitor = new MycatSchemaStatVisitor();
		} catch (Exception t) {
	        LOGGER.error("DruidMycatRouteStrategyError", t);
			throw new SQLSyntaxErrorException(t);
		}

		/**
		 * 检验unsupported statement
		 */
		checkUnSupportedStatement(statement);


		DruidParser druidParser = DruidParserFactory.create(schema, statement, visitor);
		druidParser.parser(schema, rrs, statement, stmt,cachePool,visitor);

		/**
		 * DruidParser 解析过程中已完成了路由的直接返回
		 */
		if ( rrs.isFinishedRoute() ) {
			return rrs;
		}
		
		/**
		 * 没有from的select语句或其他
		 */
        DruidShardingParseInfo ctx=  druidParser.getCtx() ;
        if((ctx.getTables() == null || ctx.getTables().size() == 0)&&(ctx.getTableAliasMap()==null||ctx.getTableAliasMap().isEmpty()))
        {
		    return RouterUtil.routeToSingleNode(rrs, schema.getRandomDataNode(), druidParser.getCtx().getSql());
		}

		if(druidParser.getCtx().getRouteCalculateUnits().size() == 0) {
			RouteCalculateUnit routeCalculateUnit = new RouteCalculateUnit();
			druidParser.getCtx().addRouteCalculateUnit(routeCalculateUnit);
		}
		
		SortedSet<RouteResultsetNode> nodeSet = new TreeSet<RouteResultsetNode>();
		for(RouteCalculateUnit unit: druidParser.getCtx().getRouteCalculateUnits()) {
			RouteResultset rrsTmp = RouterUtil.tryRouteForTables(schema, druidParser.getCtx(), unit, rrs, isSelect(statement), cachePool);
			if(rrsTmp != null) {
				for(RouteResultsetNode node :rrsTmp.getNodes()) {
					nodeSet.add(node);
				}
			}
		}
		
		RouteResultsetNode[] nodes = new RouteResultsetNode[nodeSet.size()];
		int i = 0;
		for (RouteResultsetNode aNodeSet : nodeSet) {
			nodes[i] = aNodeSet;
			  if(statement instanceof MySqlInsertStatement &&ctx.getTables().size()==1&&schema.getTables().containsKey(ctx.getTables().get(0))) {
				  RuleConfig rule = schema.getTables().get(ctx.getTables().get(0)).getRule();
				  if(rule!=null&&  rule.getRuleAlgorithm() instanceof SlotFunction){
					 aNodeSet.setStatement(ParseUtil.changeInsertAddSlot(aNodeSet.getStatement(),aNodeSet.getSlot()));
				  }
			  }
			i++;
		}		
		rrs.setNodes(nodes);		
		
		//分表
		/**
		 *  subTables="t_order$1-2,t_order3"
		 *目前分表 1.6 开始支持 幵丏 dataNode 在分表条件下只能配置一个，分表条件下不支持join。
		 */
		if(rrs.isDistTable()){
			return this.routeDisTable(statement,rrs);
		}
		
		return rrs;
	}
```

注：主要关注



       for(RouteCalculateUnit unit: druidParser.getCtx().getRouteCalculateUnits()) {
            RouteResultset rrsTmp = RouterUtil.tryRouteForTables(schema, druidParser.getCtx(), unit, rrs, isSelect(statement), cachePool);
            if(rrsTmp != null) {
                for(RouteResultsetNode node :rrsTmp.getNodes()) {
                    nodeSet.add(node);
                }
            }
        }


### io.mycat.route.util.RouterUtil.java多表路由中的单表路由



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

注：主要关注//只有一个表的 if(tables.size() == 1) { return RouterUtil.tryRouteForOneTable(schema, ctx, routeUnit, tables.get(0), rrs, isSelect, cachePool); }



### io.mycat.route.util.RouterUtil.java支持单节点单表路由代码



```java
/**
 *
 * 单表路由
 */
public static RouteResultset tryRouteForOneTable(SchemaConfig schema, DruidShardingParseInfo ctx,
			RouteCalculateUnit routeUnit, String tableName, RouteResultset rrs, boolean isSelect,
			LayerCachePool cachePool) throws SQLNonTransientException {
		
		if (isNoSharding(schema, tableName)) {
			return routeToSingleNode(rrs, schema.getDataNode(), ctx.getSql());
		}

		TableConfig tc = schema.getTables().get(tableName);
		if(Pattern.matches(".*_201\\d{5,5}", tableName)){//支持分表, t_location_20170704这种表，删除结尾的_20170704后再查找路由
			tc=schema.getTables().get(tableName.substring(0, tableName.length()-9));
		}
		if(tc == null) {
			String msg = "can't find table define in schema " + tableName + " schema:" + schema.getName();
			LOGGER.warn(msg);
			throw new SQLNonTransientException(msg);
		}
		
		if(tc.isDistTable()){
			return routeToDistTableNode(tableName,schema,rrs,ctx.getSql(), routeUnit.getTablesAndConditions(), cachePool,isSelect);
		}
		
		if(tc.isGlobalTable()) {//全局表
			if(isSelect) {
				// global select ,not cache route result
				rrs.setCacheAble(false);
				return routeToSingleNode(rrs, tc.getRandomDataNode(),ctx.getSql());
			} else {//insert into 全局表的记录
				return routeToMultiNode(false, rrs, tc.getDataNodes(), ctx.getSql(),true);
			}
		} else {//单表或者分库表
			if (!checkRuleRequired(schema, ctx, routeUnit, tc)) {
				throw new IllegalArgumentException("route rule for table "
						+ tc.getName() + " is required: " + ctx.getSql());

			}
			if(tc.getPartitionColumn() == null && !tc.isSecondLevel()) {//单表且不是childTable
//				return RouterUtil.routeToSingleNode(rrs, tc.getDataNodes().get(0),ctx.getSql());
				return routeToMultiNode(rrs.isCacheAble(), rrs, tc.getDataNodes(), ctx.getSql());
			} else {
				//每个表对应的路由映射
				Map<String,Set<String>> tablesRouteMap = new HashMap<String,Set<String>>();
				if(routeUnit.getTablesAndConditions() != null && routeUnit.getTablesAndConditions().size() > 0) {
					RouterUtil.findRouteWithcConditionsForTables(schema, rrs, routeUnit.getTablesAndConditions(), tablesRouteMap, ctx.getSql(), cachePool, isSelect);
					if(rrs.isFinishedRoute()) {
						return rrs;
					}
				}

				if(tablesRouteMap.get(tableName) == null) {
					return routeToMultiNode(rrs.isCacheAble(), rrs, tc.getDataNodes(), ctx.getSql());
				} else {
					return routeToMultiNode(rrs.isCacheAble(), rrs, tablesRouteMap.get(tableName), ctx.getSql());
				}
			}
		}
}
```
注：重点是if(tc.isDistTable()){ return routeToDistTableNode(tableName,schema,rrs,ctx.getSql(), routeUnit.getTablesAndConditions(), cachePool,isSelect); }



### 修改 io.mycat.route.util.RouterUtil.java类路由分表节点



```java
/**
 *分表路由处理
 */
private static RouteResultset routeToDistTableNode(String tableName, SchemaConfig schema, RouteResultset rrs,
            String orgSql, Map<String, Map<String, Set<ColumnRoutePair>>> tablesAndConditions,
            LayerCachePool cachePool, boolean isSelect) throws SQLNonTransientException {
        
        TableConfig tableConfig = schema.getTables().get(tableName);
        if(tableConfig == null) {
            String msg = "can't find table define in schema " + tableName + " schema:" + schema.getName();
            LOGGER.warn(msg);
            throw new SQLNonTransientException(msg);
        }
        if(tableConfig.isGlobalTable()){
            String msg = "can't suport district table  " + tableName + " schema:" + schema.getName() + " for global table ";
            LOGGER.warn(msg);
            throw new SQLNonTransientException(msg);
        } 
        String partionCol = tableConfig.getPartitionColumn();
//        String primaryKey = tableConfig.getPrimaryKey();
        boolean isLoadData=false;
        
        Set<String> tablesRouteSet = new HashSet<String>();
        
        List<String> dataNodes = tableConfig.getDataNodes();
        if(dataNodes.size()>1){
            String msg = "can't suport district table  " + tableName + " schema:" + schema.getName() + " for mutiple dataNode " + dataNodes;
            LOGGER.warn(msg);
            throw new SQLNonTransientException(msg);
        }
        String dataNode = dataNodes.get(0);
        
        //主键查找缓存暂时不实现
        if(tablesAndConditions.isEmpty()){
            // 取摸查询====开始======
            if(tableConfig.getSubTableWay().equals("BYDELIVERY")&&tableConfig.getDeliveryField()!=null&&!tableConfig.getDeliveryField().equals("")){
                long vechicleId=-1;
                String hintStart="/**";
                String hintEnd="**/";
                if(!orgSql.contains(hintStart)&&!orgSql.contains(hintEnd)){
                    String msg = "incorrect sql rule  by subTables='BYDELIVERY' property deliveryField='"+tableConfig.getDeliveryField()+"'";
                    msg+=">>:select SQL would  like to end with /**"+tableConfig.getDeliveryField()+"="+108392232+"**/";
                    LOGGER.warn(msg);
                    throw new SQLNonTransientException(msg);
                }else{
                    String valueStr=orgSql.substring(orgSql.indexOf(hintStart)+3, orgSql.length()-3);
                    String [] values=valueStr.split("=");
                    try {
                        vechicleId=Long.parseLong(values[1]);
                    } catch (NumberFormatException e) {
                        String msg = " subTables='BYDELIVERY' property deliveryField='"+tableConfig.getDeliveryField()+"' value is not a number.";
                        LOGGER.warn(msg);
                        throw new SQLNonTransientException(msg);
                    }
                    if(values.length<2||!tableConfig.getDeliveryField().equalsIgnoreCase(values[0])){
                        String msg = " subTables='BYDELIVERY' property deliveryField='"+tableConfig.getDeliveryField()+"' is not the same file end with the SQL";
                        LOGGER.warn(msg);
                        throw new SQLNonTransientException(msg);
                    }
                }
                // 取摸方式处理
                List<String> subTables = new ArrayList<String>();
                int count=tableConfig.getDistTables().size();
                String table=tableConfig.getName();
                String ruleTable=tableConfig.getSubTables().substring(0,tableConfig.getSubTables().indexOf("$")); 
                String connectStr=ruleTable.length()==table.length()?"":ruleTable.substring(table.length(), ruleTable.length()); 
                vechicleId=vechicleId%count==0?count:vechicleId%count;
                String delvieryTable=new String(table+connectStr+vechicleId).toLowerCase();
                for (String subTable : tableConfig.getDistTables()) {
                     if(subTable.equals(delvieryTable)){
                         subTables.add(subTable);// 取摸表
                     }
                }
                tablesRouteSet.addAll(subTables);
            }else{
                List<String> subTables = tableConfig.getDistTables();
                tablesRouteSet.addAll(subTables);
            }
            // 取摸查询====结束======
        }else{
            for(Map.Entry<String, Map<String, Set<ColumnRoutePair>>> entry : tablesAndConditions.entrySet()) {
                boolean isFoundPartitionValue = partionCol != null && entry.getValue().get(partionCol) != null;
                Map<String, Set<ColumnRoutePair>> columnsMap = entry.getValue();
                
                Set<ColumnRoutePair> partitionValue = columnsMap.get(partionCol);
                if(partitionValue == null || partitionValue.size() == 0) {
                    // 条件取摸方式=========开始====
                    //tablesRouteSet.addAll(tableConfig.getDistTables());
                    if(tableConfig.getSubTableWay().equals("BYDELIVERY")&&tableConfig.getDeliveryField()!=null&&!tableConfig.getDeliveryField().equals("")){
                        long vechicleId=-1;
                        String hintStart="/**";
                        String hintEnd="**/";
                        if(!orgSql.contains(hintStart)&&!orgSql.contains(hintEnd)){
                            String msg = "incorrect sql rule  by subTables='BYDELIVERY' property deliveryField='"+tableConfig.getDeliveryField()+"'";
                            msg+=">>:select SQL would  like to end with /**"+tableConfig.getDeliveryField()+"="+108392232+"**/";
                            LOGGER.warn(msg);
                            throw new SQLNonTransientException(msg);
                        }else{
                            String valueStr=orgSql.substring(orgSql.indexOf(hintStart)+3, orgSql.length()-3);
                            String [] values=valueStr.split("=");
                            try {
                                vechicleId=Long.parseLong(values[1]);
                            } catch (NumberFormatException e) {
                                String msg = " subTables='BYDELIVERY' property deliveryField='"+tableConfig.getDeliveryField()+"' value is not a number.";
                                LOGGER.warn(msg);
                                throw new SQLNonTransientException(msg);
                            }
                            if(values.length<2||!tableConfig.getDeliveryField().equalsIgnoreCase(values[0])){
                                String msg = " subTables='BYDELIVERY' property deliveryField='"+tableConfig.getDeliveryField()+"' is not the same file end with the SQL";
                                LOGGER.warn(msg);
                                throw new SQLNonTransientException(msg);
                            }
                        }
                        // 取摸方式处理
                        List<String> subTables = new ArrayList<String>();
                        int count=tableConfig.getDistTables().size();
                        String table=tableConfig.getName();
                        String ruleTable=tableConfig.getSubTables().substring(0,tableConfig.getSubTables().indexOf("$")); 
                        String connectStr=ruleTable.length()==table.length()?"":ruleTable.substring(table.length(), ruleTable.length()); 
                        vechicleId=vechicleId%count==0?count:vechicleId%count;
                        String delvieryTable=new String(table+connectStr+vechicleId).toLowerCase();
                        for (String subTable : tableConfig.getDistTables()) {
                             if(subTable.equals(delvieryTable)){
                                 subTables.add(subTable);// 取摸表
                             }
                        }
                        tablesRouteSet.addAll(subTables);
                        orgSql=orgSql.substring(0,orgSql.indexOf(hintStart));
                    }else{
                        List<String> subTables = tableConfig.getDistTables();
                        tablesRouteSet.addAll(subTables);
                    }
                    // 条件取摸方式=========结束====
                } else {
                    for(ColumnRoutePair pair : partitionValue) {
                        AbstractPartitionAlgorithm algorithm = tableConfig.getRule().getRuleAlgorithm();
                        if(pair.colValue != null) {
                            Integer tableIndex = algorithm.calculate(pair.colValue);
                            if(tableIndex == null) {
                                String msg = "can't find any valid datanode :" + tableConfig.getName()
                                        + " -> " + tableConfig.getPartitionColumn() + " -> " + pair.colValue;
                                LOGGER.warn(msg);
                                throw new SQLNonTransientException(msg);
                            }
                            String subTable = tableConfig.getDistTables().get(tableIndex);
                            if(subTable != null) {
                                tablesRouteSet.add(subTable);
                                if(algorithm instanceof SlotFunction){
                                    rrs.getDataNodeSlotMap().put(subTable,((SlotFunction) algorithm).slotValue());
                                }
                            }
                        }
                        if(pair.rangeValue != null) {
                            Integer[] tableIndexs = algorithm
                                    .calculateRange(pair.rangeValue.beginValue.toString(), pair.rangeValue.endValue.toString());
                            for(Integer idx : tableIndexs) {
                                String subTable = tableConfig.getDistTables().get(idx);
                                if(subTable != null) {
                                    tablesRouteSet.add(subTable);
                                    if(algorithm instanceof SlotFunction){
                                        rrs.getDataNodeSlotMap().put(subTable,((SlotFunction) algorithm).slotValue());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Object[] subTables =  tablesRouteSet.toArray();
        RouteResultsetNode[] nodes = new RouteResultsetNode[subTables.length];
        Map<String,Integer> dataNodeSlotMap=    rrs.getDataNodeSlotMap();
        for(int i=0;i<nodes.length;i++){
            String table = String.valueOf(subTables[i]);
            String changeSql = orgSql;
            nodes[i] = new RouteResultsetNode(dataNode, rrs.getSqlType(), changeSql);//rrs.getStatement()
            nodes[i].setSubTableName(table);
            nodes[i].setSource(rrs);
            if(rrs.getDataNodeSlotMap().containsKey(dataNode)){
                nodes[i].setSlot(rrs.getDataNodeSlotMap().get(dataNode));
            }
            if (rrs.getCanRunInReadDB() != null) {
                nodes[i].setCanRunInReadDB(rrs.getCanRunInReadDB());
            }
            if(dataNodeSlotMap.containsKey(table))  {
                nodes[i].setSlot(dataNodeSlotMap.get(table));
            }
            if(rrs.getRunOnSlave() != null){
                nodes[0].setRunOnSlave(rrs.getRunOnSlave());
            }
        }
        rrs.setNodes(nodes);
        rrs.setSubTables(tablesRouteSet);
        rrs.setFinishedRoute(true);
        
        return rrs;
}
```
注：无论有无where查询都是可以的（测试过程中发现错误已修改）。



## 测试取摸分表查询语句

### 非where条件取摸查询

请对比参考如下两个图：


![](https://img-blog.csdn.net/20170724231004131)





![](https://img-blog.csdn.net/20170724231033344)





### where条件取摸查询

请对比参考如下两个图：

![](https://img-blog.csdn.net/20170725000244434)




![](https://img-blog.csdn.net/20170725000256970)


可见，取摸查询已实现。

### 插入或取摸插入表数据注意事项

这里需要特别注意mysql数据库关键字问题，下面是数据库关键字测试问题：如 name、table、by等等。

下面两个SQL插入语句在Druid解析是有区别的：



```
-- 正确解析语句
INSERT into t_subtable(_id,title,`name`,`table`,description,likes,`by`) VALUES('59759679dc07a955104a9f1b','mongo','boonya4','t_subtable_4','database','100','boonya') /**vehicle_id=128943434354**/
-- 错误解析语句
INSERT into t_subtable(_id,title,name,table,description,likes,by) VALUES('59759679dc07a955104a9f1b','mongo','boonya4','t_subtable_4','database','100','boonya') /**vehicle_id=128943434354**/
```

#### MongoDB API插入数据

![](https://img-blog.csdn.net/20170725121614021)


#### Mycat插入数据

![](https://img-blog.csdn.net/20170725121529209)

![](https://img-blog.csdn.net/20170725122126521)

#### Mycat带关键字列插入异常



```
(io.mycat.route.impl.DruidMycatRouteStrategy:DruidMycatRouteStrategy.java:78) 
2017-07-25 12:04:25,114 [WARN ][$_NIOREACTOR-1-RW] ServerConnection [id=1, schema=TESTDB, host=192.168.1.7, user=root,txIsolation=3, autocommit=true, schema=TESTDB]INSERT into t_subtable(_id,title,name,table,description,likes,by) VALUES('59759679dc07a955104a9f1b','mongo','boonya4','t_subtable_4','database','100','boonya') /**vehicle_id=128943434354**/ err:java.sql.SQLSyntaxErrorException: com.alibaba.druid.sql.parser.ParserException: ERROR. token : BY, pos : 64 java.sql.SQLSyntaxErrorException: com.alibaba.druid.sql.parser.ParserException: ERROR. token : BY, pos : 64
	at io.mycat.route.impl.DruidMycatRouteStrategy.routeNormalSqlWithAST(DruidMycatRouteStrategy.java:79)
	at io.mycat.route.impl.AbstractRouteStrategy.route(AbstractRouteStrategy.java:81)
	at io.mycat.route.RouteService.route(RouteService.java:133)
	at io.mycat.server.ServerConnection.routeEndExecuteSQL(ServerConnection.java:276)
	at io.mycat.server.ServerConnection.execute(ServerConnection.java:222)
	at io.mycat.server.ServerQueryHandler.query(ServerQueryHandler.java:136)
	at io.mycat.net.FrontendConnection.query(FrontendConnection.java:317)
	at io.mycat.net.FrontendConnection.query(FrontendConnection.java:337)
	at io.mycat.net.handler.FrontendCommandHandler.handle(FrontendCommandHandler.java:71)
	at io.mycat.net.FrontendConnection.rawHandle(FrontendConnection.java:478)
	at io.mycat.net.FrontendConnection.handle(FrontendConnection.java:460)
	at io.mycat.net.AbstractConnection.onReadData(AbstractConnection.java:321)
	at io.mycat.net.NIOSocketWR.asynRead(NIOSocketWR.java:190)
	at io.mycat.net.AbstractConnection.asynRead(AbstractConnection.java:273)
	at io.mycat.net.NIOReactor$RW.run(NIOReactor.java:102)
	at java.lang.Thread.run(Thread.java:745)
Caused by: com.alibaba.druid.sql.parser.ParserException: ERROR. token : BY, pos : 64
	at com.alibaba.druid.sql.parser.SQLExprParser.primary(SQLExprParser.java:587)
	at com.alibaba.druid.sql.dialect.mysql.parser.MySqlExprParser.primary(MySqlExprParser.java:167)
	at com.alibaba.druid.sql.parser.SQLExprParser.expr(SQLExprParser.java:94)
	at com.alibaba.druid.sql.parser.SQLExprParser.exprList(SQLExprParser.java:903)
	at com.alibaba.druid.sql.dialect.mysql.parser.MySqlStatementParser.parseInsert(MySqlStatementParser.java:2014)
	at com.alibaba.druid.sql.dialect.mysql.parser.MySqlStatementParser.parseInsert(MySqlStatementParser.java:193)
	at com.alibaba.druid.sql.parser.SQLStatementParser.parseStatementList(SQLStatementParser.java:199)
	at com.alibaba.druid.sql.parser.SQLStatementParser.parseStatement(SQLStatementParser.java:2065)
	at io.mycat.route.impl.DruidMycatRouteStrategy.routeNormalSqlWithAST(DruidMycatRouteStrategy.java:75)
	... 15 more
 (io.mycat.server.ServerConnection:ServerConnection.java:281)
```





## 附录阅读

附录是我摘录可能以后用得着的东西，权当备忘。


如果想了解Mycat路由解析可以参考：[Mycat的SQL解析和路由](http://blog.csdn.net/flashflight/article/details/52348323)


如果想了解Mycat后端通信模块可以参考：[Mycat源码篇 : MyCat线程模型分析](http://blog.csdn.net/d6619309/article/details/52330133)


如果想了解Mycat事务管理可以参考：[Mycat源码篇 : MyCat事务管理机制分析](http://blog.csdn.net/d6619309/article/details/52330334)


如果想了解mycat具体的分片取模可以参考：[mycat 取模分片，ER分片](http://blog.csdn.net/convict_eva/article/details/52022696)[](http://blog.csdn.net/convict_eva/article/details/52022696)

如果想了解Mysql全局序列号可以参考：[mycat 使用mysql实现全局序列号](http://blog.csdn.net/convict_eva/article/details/51917499)




