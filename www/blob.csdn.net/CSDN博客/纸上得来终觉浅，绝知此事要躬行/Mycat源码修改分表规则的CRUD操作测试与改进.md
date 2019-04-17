# Mycat源码修改分表规则的CRUD操作测试与改进 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月26日 17:49:21[boonya](https://me.csdn.net/boonya)阅读数：1219








因为目前Mycat 1.6尚不支持既分库又分表的模式，所以需要对源码进行改造，目前实现单库分表的多种分表策略，包括：默认分表、按日期分表、取摸分表，三种规则。经过这几天对源码的改造，这里做一组分表规则的CRUD测试，记录于此。文章包括实现过程的两大部分的记录，即“全部支持子表路由操作”和“全部支持主表条件路由操作”，支持“全部支持主表条件路由操作”才是最终需求目标，因为对外部而言不知道去访问哪张子表。因此，需要每个外部操作Mycat的SQL语句都带上条件，这个条件可以是INSERT的列，也可以是SELECT、UPDATE和DELETE的条件。


# 全部支持子表路由操作

## 默认分表

默认分表的代码改动不大，只是针对CRUD相关的操作做了测试和调整。


### mycat插入数据测试：支持路由子表插入

默认分表本身insert语句如果是通过主表去插入会执行多次，效果如下：

![](https://img-blog.csdn.net/20170726171320175)

针对此问题已添加路由修复，修复后的插入方式如下：

![](https://img-blog.csdn.net/20170726171601423)

通过子表路由插入，插入如果不能路由到子表代码中做了异常抛出。

### Mycat删除数据测试：支持主表和子表路由删除

必须添加where子句条件删除，可以通过主表也支持路由子表去删除：

主表删除方式-删除前子表的数据 ：

![](https://img-blog.csdn.net/20170726174245499)

根据ID去删除：

![](https://img-blog.csdn.net/20170726174343247)

查询通过主表 删除后的结果：

![](https://img-blog.csdn.net/20170726174504170)


再接着测试子表通过路由的方式去删除：

![](https://img-blog.csdn.net/20170726174642593)

子表路由删除后的效果如下图：

![](https://img-blog.csdn.net/20170726174755421)

由此可见，删除功能得到验证。


### Mycat修改数据测试：支持子表路由修改

通过主表去修改修改提示成功但实际上作用行数是0，使用子表路由可以直接修改到要修改的数据：


![](https://img-blog.csdn.net/20170726173818600)


修改后效果如下：

![](https://img-blog.csdn.net/20170726174000411)


### Mycat查询数据测试：支持主表和子表路由查询

支持根据主表和子表进行查询：

![](https://img-blog.csdn.net/20170726172638166)


下面是子表的查询结果：

![](https://img-blog.csdn.net/20170726172739016)


因为取摸分表的规则与默认分表的方式很类似，我们下面来看取摸分表的测试。


## 取摸分表

取摸分表上一篇博客中有谈到，取摸分表需要一个ID取摸的条件，为避免解析上的麻烦，这个条件与where条件区分开来。SQL语句的末尾必须是符合类似/**vehicle_id=18928323**/这样标准的结尾。在写法上支持通过主表和子表路由的SQL语句，实际上是子表路由实现的。可以说，通过结尾这种方式来取摸实现都是通过子表路由实现的，直接屏蔽了通过主表操作，所以主表是语法正确，是伪操作。

### mycat插入数据测试：支持主表和子表路由插入

通过主表和子表分别插入一条数据：


![](https://img-blog.csdn.net/20170726181955018)

查询插入后的结果：

![](https://img-blog.csdn.net/20170726182121532)


### mycat删除数据测试：支持主表和子表路由删除

删除前的数据：



![](https://img-blog.csdn.net/20170726183237708)

通过主表和子表分别删除一条数据：

![](https://img-blog.csdn.net/20170726183913039)

查看执行结果：

![](https://img-blog.csdn.net/20170726184008776)

都执行成功了。


### mycat修改数据测试：支持子表路由修改

修改前数据如下图：



![](https://img-blog.csdn.net/20170726182429643)
通过主表和子表分别修改一条数据：

![](https://img-blog.csdn.net/20170726183047892)

查看修改后的结果：

![](https://img-blog.csdn.net/20170726183237708)

上面更新执行图片显示都成功了，但是实际上只有子表路由的成功了。


### mycat查询数据测试：支持主表和子表路由查询

通过主表和子表分别查询数据：


![](https://img-blog.csdn.net/20170726182429643)

有两个结果集，证明是查询成功的，请看结果2：

![](https://img-blog.csdn.net/20170726182530887)


## 按天分表

这种方式的分表实现的时候只要设置了deliveryField字段按条件路由，就放弃了通过主表操作功能，因为主表没法通过路由方式去定位子表，除非SELECT和UPDATE语句里面含有deliveryField属性配置的字段。如果非得让按天分表支持主表操作，可以参考取摸分表的规则实现。

### mycat插入数据测试：支持子表路由插入

插入一条数据：

![](https://img-blog.csdn.net/20170726192227834)

查看插入的数据：

![](https://img-blog.csdn.net/20170726192331244)


### mycat删除数据测试：支持子表路由删除

删除数据之前：

![](https://img-blog.csdn.net/20170726192728184)

执行删除一条数据：

![](https://img-blog.csdn.net/20170726193120215)

删除后的数据：

![](https://img-blog.csdn.net/20170726193227679)


### mycat修改数据测试：支持子表路由修改

修改之前的数据：

![](https://img-blog.csdn.net/20170726192331244)

修改一条数据：

![](https://img-blog.csdn.net/20170726192638419)

修改后的数据：

![](https://img-blog.csdn.net/20170726192728184)


### mycat查询数据测试：支持子表路由查询

![](https://img-blog.csdn.net/20170726192905384)


# 全部支持主表条件路由操作

最新需求:

1、支持条件路由：目的是不暴露实际路由的哪张表，但也支持子表名称方式这个无所谓只是写法不同而已，实际还是根据条件来实现路由的。
2、ID取摸算法需要支持字符串形式的ID，做Hash处理返回数字类型进行计算。
3、INSERT 语句不仅要支持单个的插入也要支持多个VALUES的插入，包括多个values不同条件的路由拆分。

改进说明：根据最新需求修改后，如果采用分表必须设置deliveryField字段用于标识条件字段，另外新增isStrictField字段用于标识路由是否严格按照条件处理isStrictField=“true”或“false”。isStrictField如果设置为"true",CRUD严格按照解析条件进行解析，如果不满足就抛出异常。

一般地，将isStrictField="false"就可以了，isStrictField在没有设置严格解析的条件的情况下后台代码会根据表名进行路由，是主表就操作主表（注：INSERT操作主要设置了deliveryField字段值就可以路由到子表进行插入，支持一个或多个values插入 ），是子表就操作子表，有条件直接根据条件路由。


过程和截图比较麻烦，代码修改现已实现，详细简要手册文档：[http://download.csdn.net/detail/boonya/9916993](http://download.csdn.net/detail/boonya/9916993)

下面附上一些核心代码。

## Druid解析方式基础



```java
public class ParserMain { 
    public static void main(String[] args) {
     String sql = "select id,name from user"; 
      // 新建 MySQL Parser 
      SQLStatementParser parser = new MySqlStatementParser(sql); 
      // 使用Parser解析生成AST，这里SQLStatement就是AST
      SQLStatement statement = parser.parseStatement(); 
      // 使用visitor来访问AST 
      MySqlSchemaStatVisitor visitor = new MySqlSchemaStatVisitor(); 
      statement.accept(visitor); 
      // 从visitor中拿出你所关注的信息        
      System.out.println(visitor.getColumns());
 }
}
```



## 改进路由解析子表分发

**io.mycat.route.util.RouterUtil.java**




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

		String oldTableName=null;
		
		TableConfig tc = schema.getTables().get(tableName);
		// 默认分表规则匹配表路由基础表:此种方式包括满足按天分表
		if(tc==null){
			oldTableName=new String(tableName).toUpperCase();
			String []strs=tableName.split("_");
			tableName=tableName.substring(0, tableName.length()-(strs[strs.length-1].length()+1));
			tc = schema.getTables().get(tableName);
			if(tc==null){
				String msg = "can't find table define in schema " + tableName + " schema:" + schema.getName();
				throw new SQLNonTransientException(msg);
			}
		}
		
		// **********解析条件字段路由处理*************开始*************
		routeToOneTableByContions(ctx, tc, rrs, tableName, oldTableName); 
		// **********解析条件字段路由处理*************结束*************
		
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
	
	/**
	 * 解析条件字段路由处理
	 */
	private static void routeToOneTableByContions(DruidShardingParseInfo ctx, TableConfig tc, RouteResultset rrs,String tableName,String oldTableName) throws SQLNonTransientException{
		// 新建 MySQL	Parser 
		SQLStatementParser parser = new MySqlStatementParser(ctx.getSql()); 
		// 使用Parser解析生成AST，这里SQLStatement就是AST 
		SQLStatement statement = parser.parseStatement(); 
		// 使用visitor来访问AST 
		MySqlSchemaStatVisitor visitor = new MySqlSchemaStatVisitor();
		statement.accept(visitor); 
		String childSequence="",routeTableName=null;
		boolean conditionIsOk=false;
		String tablePrex=tc.getSubTables().substring(0, tc.getSubTables().indexOf("$"));
		// 解析CRUD条件
		if(statement instanceof SQLSelectStatement||statement instanceof MySqlDeleteStatement||statement instanceof MySqlUpdateStatement){
			if(tc.getSubTableWay()==null||"".equals(tc.getSubTableWay())||tc.getSubTableWay().equals("DEFAULT")){
				List<Condition> conditions=visitor.getConditions();
				if(null!=conditions&&conditions.size()>0){
					for (Condition condition : conditions) {
						if(tc.getDeliveryField().toLowerCase().equals( condition.getColumn().getName().toLowerCase())
								&&condition.getOperator().equals("=")){ 
							childSequence=(String) condition.getValues().get(0);
							if(!ValidateUtils.Number(childSequence)){
								// 获取字符串的正整数hashCode值
								childSequence=Math.abs(childSequence.hashCode())+"";
							}
							int dValue=Integer.valueOf(childSequence)%tc.getDistTables().size();
							dValue=(dValue==0)?tc.getDistTables().size():dValue;
							routeTableName=tablePrex+dValue;
							conditionIsOk=true;
							break;
						}else{
							// 判断是否严格按照条件执行
							if(tc.isStrictByField()){
								String msg ="Can't according to conditions to route child table name,because isStrictByField is "+tc.isStrictByField();
								throw new SQLNonTransientException(msg);
							}else{
								if(Pattern.matches(".*_\\d*$",visitor.getCurrentTable())){
									routeTableName=visitor.getCurrentTable();
									oldTableName=routeTableName.toUpperCase();
									conditionIsOk=true;
								}
							}
						}
					}
				}else{
					if(Pattern.matches(".*_\\d*$",visitor.getCurrentTable())){
						routeTableName=visitor.getCurrentTable();
						oldTableName=routeTableName.toUpperCase();
						conditionIsOk=true;
					}
				}
			}
			if(tc.getSubTableWay().equals("BYDATE")){
				List<Condition> conditions=visitor.getConditions();
				if(null!=conditions&&conditions.size()>0){
					for (Condition condition : conditions) {
						if(tc.getDeliveryField().toLowerCase().equals( condition.getColumn().getName().toLowerCase())
								&&(condition.getOperator().equals(">")||condition.getOperator().equals("=")
										||condition.getOperator().equals(">="))){ 
							childSequence=(String) condition.getValues().get(0);
							childSequence=childSequence.split(" ")[0];
							// 验证是否是日期
							Date date=null;
							try {
								date=DateUtils.parseDate(childSequence, "yyyy-MM-dd");
							} catch (ParseException e) {
								String msg =condition.getColumn().getName() +condition.getOperator()+childSequence+" :the value is not a date by format 'yyyy-MM-dd'  or 'yyyy-MM-dd hh:mm:ss'";
								throw new SQLNonTransientException(msg);
							}
							routeTableName=tablePrex+DateUtils.getStrFormTime("yyyyMMdd", date);
							conditionIsOk=true;
							break;
						}else{
							// 判断是否严格按照条件执行
							if(tc.isStrictByField()){
								String msg ="Can't according to conditions to route child table name,because isStrictByField is "+tc.isStrictByField();
								throw new SQLNonTransientException(msg);
							}else{
								// 匹配表名是否是子表2000-2099年都可以
								if(Pattern.matches(".*_20\\d{6,6}",visitor.getCurrentTable())){
									routeTableName=visitor.getCurrentTable();
									oldTableName=routeTableName.toUpperCase();
									conditionIsOk=true;
									break;
								}
							}
						}
					}
				}else{
					// 匹配表名是否是子表2000-2099年都可以
					if(Pattern.matches(".*_20\\d{6,6}",visitor.getCurrentTable())){
						routeTableName=visitor.getCurrentTable();
						oldTableName=routeTableName.toUpperCase();
						conditionIsOk=true;
					}
				}
			}
			if(tc.getSubTableWay().equals("BYDELIVERY")){
				List<Condition> conditions=visitor.getConditions();
				if(null!=conditions&&conditions.size()>0){
					for (Condition condition : conditions) {
						if(tc.getDeliveryField().toLowerCase().equals( condition.getColumn().getName().toLowerCase())
								&&condition.getOperator().equals("=")){ 
							childSequence=(String) condition.getValues().get(0);
							if(!ValidateUtils.Number(childSequence)){
								// 获取字符串的正整数hashCode值
								childSequence=Math.abs(childSequence.hashCode())+"";
							}
							// 取摸处理
							int value=Integer.valueOf(childSequence)%tc.getDistTables().size();
							childSequence=value==0?tc.getDistTables().size()+"":value+"";
							routeTableName=tablePrex+childSequence;
							conditionIsOk=true;
							break;
						}else{
							// 判断是否严格按照条件执行
							if(tc.isStrictByField()){
								String msg ="Can't according to conditions to route child table name,because isStrictByField is "+tc.isStrictByField();
								throw new SQLNonTransientException(msg);
							}else{
								// 匹配表名是否是子表2000-2099年都可以
								if(Pattern.matches(".*_20\\d{6,6}",visitor.getCurrentTable())){
									routeTableName=visitor.getCurrentTable();
									oldTableName=routeTableName.toUpperCase();
									conditionIsOk=true;
									break;
								}
							}
						}
					}
				}else{
					if(Pattern.matches(".*_\\d*$",visitor.getCurrentTable())){
						routeTableName=visitor.getCurrentTable();
						oldTableName=routeTableName.toUpperCase();
						conditionIsOk=true;
					}
				}
			}
		}else if (statement instanceof MySqlInsertStatement){
			int count=0;
			// 默认分表插入
			if(tc.getSubTableWay()==null||"".equals(tc.getSubTableWay())||tc.getSubTableWay().equals("DEFAULT")){
				if(tc.isStrictByField()||(!"".equals(tc.getDeliveryField())&&null!=tc.getDeliveryField())){
					// 通过字段值解析是否存在条件字段
					childSequence=getInsertSqlCondition(ctx, tc, visitor);
					if(childSequence.split(",").length>1){
						// 多个插入
						StringBuilder sb=new StringBuilder();
						String values[]=childSequence.split(",");
						for (String sequence : values) {
							if(!ValidateUtils.Number(sequence)){
								// 获取字符串的正整数hashCode值
								sequence=Math.abs(sequence.hashCode())+"";
							}
							int dValue=Integer.valueOf(sequence)%tc.getDistTables().size();
							dValue=(dValue==0)?tc.getDistTables().size():dValue;
							routeTableName=tablePrex+dValue;
							if(count==0){
								sb.append(routeTableName);
							}else{
								sb.append(",").append(routeTableName);
							}
							routeTableName=null;
							count++;
						}
						routeTableName=sb.toString();
						conditionIsOk=routeTableName!=null;
					}else{
						// 验证表名是否正确
						boolean isContain=false;
						for (String subTable : tc.getDistTables()) {
							if(subTable.toLowerCase().contains(childSequence.toLowerCase())){
								isContain=true;
								break;
							}
						}
						if(!isContain){
							String msg ="subTableWay is 'DEFAULT' and deliveryField  property need a table_{ID} ,ID will use it to verify table suffix.";
							throw new SQLNonTransientException(msg);
						}
						routeTableName=tablePrex+childSequence;
						conditionIsOk=true;
					}
				}else{
					// 匹配表名是否是子表
					if(Pattern.matches(".*_\\d*$",visitor.getCurrentTable())){
						routeTableName=visitor.getCurrentTable();
						oldTableName=routeTableName.toUpperCase();
						conditionIsOk=true;
					}
				}
			}
			// 按天分表插入
			if(tc.getSubTableWay().equals("BYDATE")){
				if(tc.isStrictByField()||(!"".equals(tc.getDeliveryField())&&null!=tc.getDeliveryField())){
					// 通过字段值解析是否存在条件字段
					childSequence=getInsertSqlCondition(ctx, tc, visitor);
					if(childSequence.split(",").length>1){
						// 多个插入
						StringBuilder sb=new StringBuilder();
						String values[]=childSequence.split(",");
						for (String sequence : values) {
							// 验证是否是日期
							Date date=null;
							try {
								date=DateUtils.parseDate(sequence, "yyyy-MM-dd");
							} catch (ParseException e) {
								String msg ="insert into column  :the value is not a date by format 'yyyy-MM-dd' or 'yyyy-MM-dd hh:mm:ss'";
								throw new SQLNonTransientException(msg);
							}
							routeTableName=tablePrex+DateUtils.getStrFormTime("yyyyMMdd", date);
							if(count==0){
								sb.append(routeTableName);
							}else{
								sb.append(",").append(routeTableName);
							}
							routeTableName=null;
							count++;
						}
						routeTableName=sb.toString();
						conditionIsOk=routeTableName!=null;
					}else{
						// 验证是否是日期
						Date date=null;
						try {
							date=DateUtils.parseDate(childSequence, "yyyy-MM-dd");
						} catch (ParseException e) {
							String msg ="insert into column  :the value is not a date by format 'yyyy-MM-dd' or 'yyyy-MM-dd hh:mm:ss'";
							throw new SQLNonTransientException(msg);
						}
						routeTableName=tablePrex+DateUtils.getStrFormTime("yyyyMMdd", date);
						conditionIsOk=true;
					}
					
				}else{
					// 匹配表名是否是子表2000-2099年都可以
					if(Pattern.matches(".*_20\\d{6,6}",visitor.getCurrentTable())){
						routeTableName=visitor.getCurrentTable();
						oldTableName=routeTableName.toUpperCase();
						conditionIsOk=true;
					}
				}
			}
			// 取摸分表插入
			if(tc.getSubTableWay().equals("BYDELIVERY")){
				if(tc.isStrictByField()||(!"".equals(tc.getDeliveryField())&&null!=tc.getDeliveryField())){
					// 通过字段值解析是否存在条件字段
					childSequence=getInsertSqlCondition(ctx, tc, visitor);
					if(childSequence.split(",").length>1){
						// 多个插入
						StringBuilder sb=new StringBuilder();
						String values[]=childSequence.split(",");
						for (String sequence : values) {
							if(!ValidateUtils.Number(sequence)){
								// 获取字符串的正整数hashCode值
								sequence=Math.abs(sequence.hashCode())+"";
							}
							// 取摸处理
							int value=Integer.valueOf(sequence)%tc.getDistTables().size();
							childSequence=value==0?tc.getDistTables().size()+"":value+"";
							routeTableName=tablePrex+childSequence;
							if(count==0){
								sb.append(routeTableName);
							}else{
								sb.append(",").append(routeTableName);
							}
							routeTableName=null;
							count++;
						}
						routeTableName=sb.toString();
						conditionIsOk=routeTableName!=null;
					}else{
						if(!ValidateUtils.Number(childSequence)){
							// 获取字符串的正整数hashCode值
							childSequence=new String(childSequence.hashCode()+"").replaceAll("-", "").toString();
						}
						// 取摸处理
						childSequence=Integer.valueOf(childSequence)%tc.getDistTables().size()+"";
						routeTableName=tablePrex+childSequence;
						conditionIsOk=true;
					}
				}else{
					// 匹配表名是否是子表
					if(Pattern.matches(".*_\\d*$",visitor.getCurrentTable())){
						routeTableName=visitor.getCurrentTable();
						oldTableName=routeTableName.toUpperCase();
						conditionIsOk=true;
					}
				}
			}
        }
		if(tc.isStrictByField()){
			String msg ="Can't according to conditions to route child table name,because isStrictByField is "+tc.isStrictByField();
			throw new SQLNonTransientException(msg);
		}
		
		//处理SQL语句表名替换及大小写问题
		setNewSqlToCtxAndRrs(ctx, rrs, tableName, oldTableName, routeTableName, conditionIsOk);
		
	}
	
	/**
	 * 处理SQL语句表名替换及大小写问题
	 * @throws SQLNonTransientException
	 */
	private static void setNewSqlToCtxAndRrs(DruidShardingParseInfo ctx, RouteResultset rrs,String tableName,String oldTableName,String routeTableName,boolean conditionIsOk) throws SQLNonTransientException{
		String sql=ctx.getSql();
		if(sql.toUpperCase().contains("WHERE")&&!sql.toUpperCase().contains("UPDATE")){
			if(oldTableName!=null){
				int index=sql.toUpperCase().indexOf("WHERE");
				String startStr=sql.toUpperCase().substring(0, index);
				startStr=startStr.replace(oldTableName, tableName).toUpperCase();
				String endStr=sql.substring(index, sql.length());
				sql=startStr+endStr;
			}else{
				int index=sql.toUpperCase().indexOf("WHERE");
				String startStr=sql.toUpperCase().substring(0, index);
				String endStr=sql.substring(index, sql.length());
				sql=startStr+endStr;
			}
		}else if(sql.toUpperCase().contains("SELECT")||sql.toUpperCase().contains("DELETE")){
			if(oldTableName!=null){
				int index=sql.toUpperCase().indexOf(oldTableName);
				String startStr=sql.toUpperCase().substring(0, index+oldTableName.length());
				startStr=startStr.replace(oldTableName, tableName).toUpperCase();
				String endStr=sql.substring(index+oldTableName.length(), sql.length()).toLowerCase();
				sql=startStr+endStr;
			}else{
				sql=sql.toUpperCase();
			}
		}else if(sql.toUpperCase().contains("INSERT")){
			if(oldTableName!=null){
				int index=sql.indexOf("(");
				String startStr=sql.toUpperCase().substring(0, index);
				startStr=startStr.replace(oldTableName, tableName).toUpperCase();
				String endStr=sql.substring(index, sql.length());
				sql=startStr+endStr;
			}else{
				int index=sql.indexOf("(");
				String startStr=sql.toUpperCase().substring(0, index);
				String endStr=sql.substring(index, sql.length());
				sql=startStr+endStr;
			}
			// 验证插入多个插入必须保证日期一致
			/*if(routeTableName!=null&&routeTableName.split(",").length>1){
				 Set<String> tablesRouteSet = new HashSet<String>();
				 tablesRouteSet.addAll(Arrays.asList(routeTableName.split(","))); 
				 if(tablesRouteSet.size()>1){
					 String msg ="insert into by many values include different date,this case is not allowed";
					 throw new SQLNonTransientException(msg);
				 }
			}*/
		}else if(sql.toUpperCase().contains("UPDATE")){
			if(oldTableName!=null){
				int index=sql.toUpperCase().indexOf("SET");
				String startStr=sql.toUpperCase().substring(0, index);
				startStr=startStr.replace(oldTableName, tableName).toUpperCase();
				String endStr=sql.substring(index, sql.length());
				sql=startStr+endStr;
			}else{
				int index=sql.toUpperCase().indexOf("SET");
				String startStr=sql.toUpperCase().substring(0, index);
				String endStr=sql.substring(index, sql.length());
				sql=startStr+endStr;
			}
		}
		if(conditionIsOk){
			sql+="/**routeTable="+routeTableName+"**/";
		}
		rrs.setStatement(sql);
		ctx.setSql(sql);
	}
	
	/**
	 * 获取插入语句的条件值
	 * @throws SQLNonTransientException 
	 */
	private static String getInsertSqlCondition(DruidShardingParseInfo ctx, TableConfig tc,MySqlSchemaStatVisitor visitor) throws SQLNonTransientException{
		// 通过字段值解析是否存在条件字段 
		int index=-1,count=0;
		Collection<Column> columns=visitor.getColumns();
		for (Column column : columns) {
			if(column.getName().equals(tc.getDeliveryField())){
				index=count;
			}
			count++;
		}
		if(index==-1){
			String msg ="insert into colums not contains  "+tc.getDeliveryField()+" field.";
			throw new SQLNonTransientException(msg);
		}
		// 获取字段值 
		String sqlValues=ctx.getSql().substring(ctx.getSql().toUpperCase().indexOf("VALUES")+6, ctx.getSql().length());
		// 插入条件的值
		List<String> list=getInsertValues(ctx.getSql());
		boolean isManyInsert=list.size()>1;
		if(isManyInsert){
			// 多个插入解析
			StringBuilder sb=new StringBuilder();
			count=0;
			for (String sqlValues2 : list) {
				sqlValues2=sqlValues2.substring(sqlValues2.toUpperCase().indexOf("(")+1,sqlValues2.lastIndexOf(")"));
				
				String [] values=sqlValues2
						.replaceAll("\r", "")
						.replaceAll("\n", "")
						.toString()
						.split(",");
				String value=values[index].replaceAll("'", "").toString();
				if(count==0){
					sb.append(value.split(" ")[0]);
				}else{
					sb.append(",").append(value.split(" ")[0]);
				}
				count++;
			}
			return sb.toString();
		}else{
			// 单个插入解析
			sqlValues=sqlValues.substring(sqlValues.toUpperCase().indexOf("(")+1,sqlValues.lastIndexOf(")"));
			String [] values=sqlValues.split(",");
			String value=values[index].replaceAll("'", "").toString();
			return value.split(" ")[0];
		}
	}
	
	/**
	 * 获取插入values后面的值集
	 */
	public static List<String> getInsertValues(String sql) throws SQLNonTransientException{
		if("".equals(sql)||sql==null||!sql.toUpperCase().contains("INSERT")){
			String msg ="SQL must is a insert SQL.";
			throw new SQLNonTransientException(msg);
		}
		String values=sql.substring(sql.toUpperCase().indexOf("VALUES")+6, sql.length());
		if(values.contains("/**")){
			values=values.substring(0, values.indexOf("/**"));
		}
        String []strs=values.split(",");
		
		List<String> list=new ArrayList<String>();
		StringBuilder sb=null;
		for (int i = 0; i < strs.length; i++) {
			if(strs[i].contains("(")){
				if(sb==null){
					sb=new StringBuilder();
				}
				sb.append(strs[i]).append(",");
			}else if(strs[i].contains(")")){
				sb.append(strs[i]);
				list.add(sb.toString());
				sb=null;
			}else{
				sb.append(strs[i]).append(",");
			}
		}
		return list;
	}
	
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
			String msg = "can't suport district table  " + tableName + " schema:" + schema.getName() + " for global table ";
			LOGGER.warn(msg);
			throw new SQLNonTransientException(msg);
		} 
		String partionCol = tableConfig.getPartitionColumn();
		//String primaryKey = tableConfig.getPrimaryKey();
        //boolean isLoadData=false;
        
        Set<String> tablesRouteSet = new HashSet<String>();
        
        List<String> dataNodes = tableConfig.getDataNodes();
        if(dataNodes.size()>1){
			String msg = "can't suport district table  " + tableName + " schema:" + schema.getName() + " for mutiple dataNode " + dataNodes;
        	LOGGER.warn(msg);
			throw new SQLNonTransientException(msg);
        }
        String dataNode = dataNodes.get(0);
        
        List<String> insertValues=new ArrayList<String>();
        List<String> subTables =new ArrayList<String>();
		//主键查找缓存暂时不实现
        if(tablesAndConditions.isEmpty()){
        	if(orgSql.contains("/**")){
				String valueStr=orgSql.substring(orgSql.indexOf("/**")+3, orgSql.length()-3);
				String [] values=valueStr.split("=");
				valueStr=values[1].toLowerCase();
				values=valueStr.split(",");
				if(values.length>1){
					for (String value : values) {
						subTables.add(value);
					}
				}else{
					subTables.add(values[0]);
				}
				tablesRouteSet.addAll(subTables);
				//处理批量插入语句
				if(orgSql.toUpperCase().contains("INSERT")&&subTables.size()>1){
					String sql=orgSql.substring(0, orgSql.toUpperCase().indexOf("VALUES"));
				    List<String> list=getInsertValues(orgSql);
				    for (String item : list) {
						insertValues.add(new String(sql+" values "+item));
					}
				}
			}else{
				subTables = tableConfig.getDistTables();
            	tablesRouteSet.addAll(subTables);
			}
        }else{
        	for(Map.Entry<String, Map<String, Set<ColumnRoutePair>>> entry : tablesAndConditions.entrySet()) {
    			//boolean isFoundPartitionValue = partionCol != null && entry.getValue().get(partionCol) != null;
    			Map<String, Set<ColumnRoutePair>> columnsMap = entry.getValue();
    			
    			Set<ColumnRoutePair> partitionValue = columnsMap.get(partionCol);
    			if(partitionValue == null || partitionValue.size() == 0) {
    				if(orgSql.contains("/**")){
	    				String valueStr=orgSql.substring(orgSql.indexOf("/**")+3, orgSql.length()-3);
	    				String [] values=valueStr.split("=");
	    				subTables.add(values[1].toLowerCase());
	    				tablesRouteSet.addAll(subTables);
	    			}else{
	    				subTables = tableConfig.getDistTables();
	                	tablesRouteSet.addAll(subTables);
	    			}
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
        /*
		Object[] subTables =  tablesRouteSet.toArray();//去重复,多个values插入不需要去从重
		*/
		RouteResultsetNode[] nodes = new RouteResultsetNode[subTables.size()];
	    Map<String,Integer> dataNodeSlotMap=	rrs.getDataNodeSlotMap();
		for(int i=0;i<nodes.length;i++){
			String table = String.valueOf(subTables.get(i));
			String changeSql ="";
			if(insertValues.size()>0){
				changeSql=insertValues.get(i);
			}else{
				changeSql = orgSql;
			}
			nodes[i] = new RouteResultsetNode(dataNode, rrs.getSqlType(), changeSql);//rrs.getStatement()
			nodes[i].setSubTableName(table);
			nodes[i].setSource(rrs);
			if(rrs.getDataNodeSlotMap().containsKey(dataNode)){
				nodes[i].setSlot(rrs.getDataNodeSlotMap().get(dataNode));
			}
			if (rrs.getCanRunInReadDB() != null) {
				nodes[i].setCanRunInReadDB(rrs.getCanRunInReadDB());
			}
			if(dataNodeSlotMap.containsKey(table))  {
				nodes[i].setSlot(dataNodeSlotMap.get(table));
			}
			if(rrs.getRunOnSlave() != null){
				nodes[0].setRunOnSlave(rrs.getRunOnSlave());
			}
		}
		rrs.setNodes(nodes);
		rrs.setSubTables(tablesRouteSet);//路由作用的子表
		rrs.setFinishedRoute(true);
		return rrs;
	}
```




## 改进路由成功后插入多个VALUES不分发问题

**io.mycat.backend.jdbc.JDBCConnection.java**




```java
/**
	 * 获取插入values后面的值集
	 */
	public static List<String> getInsertValues(String sql) throws SQLNonTransientException{
		if("".equals(sql)||sql==null||!sql.toUpperCase().contains("INSERT")){
			String msg ="SQL must is a insert SQL.";
			throw new SQLNonTransientException(msg);
		}
		String values=sql.substring(sql.toUpperCase().indexOf("VALUES")+6, sql.length());
		if(values.contains("/**")){
			values=values.substring(0, values.indexOf("/**"));
		}
        String []strs=values.split(",");
		
		List<String> list=new ArrayList<String>();
		StringBuilder sb=null;
		for (int i = 0; i < strs.length; i++) {
			if(strs[i].contains("(")){
				if(sb==null){
					sb=new StringBuilder();
				}
				sb.append(strs[i]).append(",");
			}else if(strs[i].contains(")")){
				sb.append(strs[i]);
				list.add(sb.toString());
				sb=null;
			}else{
				sb.append(strs[i]).append(",");
			}
		}
		return list;
	}
	
	/**
	 * 通过配置条件插入适配语句
	 */
	private String getSqlByChildRouteTable(String sql) throws SQLNonTransientException{
		// 新建 MySQL	Parser 
		SQLStatementParser parser = new MySqlStatementParser(sql); 
		// 使用Parser解析生成AST，这里SQLStatement就是AST 
		SQLStatement statement = parser.parseStatement(); 
		// 解析日期符合的values
		Map<String,SchemaConfig> m=MycatServer.getInstance().getConfig().getSchemas();
		// 解析配置文件
		for (SchemaConfig config : m.values()) {
			Map<String,TableConfig> map=config.getTables();
			for(TableConfig tc:map.values()){
				if(null!=tc.getSubTables()&&!"".equals(tc.getSubTables())){
					String tablePrex=tc.getSubTables().substring(0, tc.getSubTables().indexOf("$"));
					// 当前操作子表
					if(sql.toUpperCase().contains(tablePrex.toUpperCase())){
						String startSql=sql.substring(0,sql.toUpperCase().indexOf("VALUES")+6);
						String columnStr=startSql.substring(startSql.indexOf("(")+1, startSql.indexOf(")"));
						String columns[]=columnStr.split(",");
						int position=-1;
						for (int i = 0; i < columns.length; i++) {
							if(columns[i].toUpperCase().contains(tc.getDeliveryField().toUpperCase())){
								position=i;
								break;
							}
						}
						List<String> values=getInsertValues(sql);
						// 插入语句执行
						if(statement instanceof MySqlInsertStatement){
							if(tc.getSubTableWay()==null||"".equals(tc.getSubTableWay())||tc.getSubTableWay().equals("DEFAULT")){
								// table_{ID}，id是条件值设置
								String str[]=startSql.substring(0, startSql.indexOf("(")).split(" ");
								String childTable=str[str.length-1];
								String lastDrex=childTable.substring(tablePrex.length(), childTable.length()).replaceAll("'", "").toString();
								int count=0;
								StringBuilder sbSQL=new StringBuilder();
								sbSQL.append(startSql);
								for (String value : values) {
									String column=value.substring(value.indexOf("("), value.indexOf(")")).split(",")[position].replaceAll("'", "").toString();
									if(!ValidateUtils.Number(column)){
										// 获取字符串的正整数hashCode值
										column=Math.abs(column.hashCode())+"";
									}
									int d=Integer.valueOf(column)%tc.getDistTables().size();
									d=(d==0)?tc.getDistTables().size():d;
									if(Integer.valueOf(lastDrex).intValue()==d){
										if(count==0){
											sbSQL.append(value);
											count++;
										}else{
											sbSQL.append(",").append(value);
										}
									}
								}
								sql=sbSQL.toString();
							}else if(tc.getSubTableWay().equals("BYDATE")){
								String str[]=startSql.substring(0, startSql.indexOf("(")).split(" ");
								String childTable=str[str.length-1];
								String lastDrex=childTable.substring(tablePrex.length(), childTable.length());
								String ymd[]=new String[4];
								for (int i = 0,j=lastDrex.length()/2; i < j; i++) {
									ymd[i]=lastDrex.substring(i*2,(i+1)*2);
								}
								StringBuilder sb=new StringBuilder();
								for (int i = 0; i < ymd.length; i++) {
									if(i==2||i==3){
										sb.append("-").append(ymd[i]);
									}else{
										sb.append(ymd[i]);
									}
								}
								int count=0;
								String date=sb.toString();
								StringBuilder sbSQL=new StringBuilder();
								sbSQL.append(startSql);
								for (String value : values) {
									String column=value.substring(value.indexOf("("), value.indexOf(")")).split(",")[position].replaceAll("'", "").toString();
									if(column.contains(date)){
										if(count==0){
											sbSQL.append(value);
											count++;
										}else{
											sbSQL.append(",").append(value);
										}
										
									}
								}
								sql=sbSQL.toString();
							}else if(tc.getSubTableWay().equals("BYDELIVERY")){
								// table_{ID}，id是条件值设置
								String str[]=startSql.substring(0, startSql.indexOf("(")).split(" ");
								String childTable=str[str.length-1];
								String lastDrex=childTable.substring(tablePrex.length(), childTable.length()).replaceAll("'", "").toString();
								int count=0;
								StringBuilder sbSQL=new StringBuilder();
								sbSQL.append(startSql);
								for (String value : values) {
									String column=value.substring(value.indexOf("("), value.indexOf(")")).split(",")[position].replaceAll("'", "").toString();
									if(!ValidateUtils.Number(column)){
										// 获取字符串的正整数hashCode值
										column=Math.abs(column.hashCode())+"";
									}
									int d=Integer.valueOf(column)%tc.getDistTables().size();
									d=(d==0)?tc.getDistTables().size():d;
									if(Integer.valueOf(lastDrex).intValue()==d){
										if(count==0){
											sbSQL.append(value);
											count++;
										}else{
											sbSQL.append(",").append(value);
										}
									}
								}
								sql=sbSQL.toString();
							}
						}
						break;
					}
				}
			}
		}
		return sql;
	}

	private void executeddl(ServerConnection sc, String sql)
			throws SQLException {
		Statement stmt = null;
		try {
			stmt = con.createStatement();
			if(sql.toUpperCase().contains("INSERT")){
				sql= getSqlByChildRouteTable(sql);//处理批量插入条件语句 
			}
			int count = stmt.executeUpdate(sql);
			OkPacket okPck = new OkPacket();
			okPck.affectedRows = count;
			okPck.insertId = 0;
			okPck.packetId = ++packetId;
			okPck.message = " OK!".getBytes();
			this.respHandler.okResponse(okPck.writeToBytes(sc), this);
		} finally {
			if (stmt != null) {
				try {
					stmt.close();
				} catch (SQLException e) {

				}
			}
		}
	}
```


以上测试数据源都是MongoDB，下面来看下Mysql的测试。

## 基于改进对Mysql 采用按天分表进行测试

配置：



```
<?xml version="1.0"?>  
<!DOCTYPE mycat:schema SYSTEM "schema.dtd">  
<mycat:schema xmlns:mycat="http://io.mycat/">  
  
        <schema name="TESTDB" checkSQLschema="false" sqlMaxLimit="100">  
                <table name="t_user"   primaryKey="u_id" autoIncrement="true"   dataNode="dn1" rule="mod-single" />  
  
                <table name="t_service" primaryKey="s_id" autoIncrement="true"   dataNode="dn2"  rule="mod-single"/>  
  
                <table name="t_location" primaryKey="F_ID" autoIncrement="true"  dataNode="dn1,dn2" rule="mod-long" />  
                
                <table name="t_subtable"   primaryKey="s_id" autoIncrement="true" subTables="t_subtable_$20170801-20170805" subTableWay="BYDATE" deliveryField="s_gpstime"  dataNode="dn1" />  
  
        </schema>  
        <dataNode name="dn1" dataHost="localhost1" database="m" />  
        <dataNode name="dn2" dataHost="localhost1" database="n" />  
  
        <dataHost name="localhost1" maxCon="1000" minCon="10" balance="1"  writeType="0" dbType="mysql" dbDriver="native" switchType="1"  slaveThreshold="100">  
                <heartbeat>show slave status</heartbeat>  
                <!-- can have multi write hosts -->  
                <writeHost host="hostM1" url="10.0.0.129:3306" user="root"   password="123456">  
                        <!-- can have multi read hosts -->  
                        <readHost host="hostS2" url="10.0.0.128:3306" user="root" password="123456" />  
                </writeHost>  
  
                <!-- <writeHost host="hostM2" url="localhost:3316" user="root" password="123456"/> -->  
        </dataHost>  
  
</mycat:schema>
```





插入：

![](https://img-blog.csdn.net/20170801205739719)


查询：

![](https://img-blog.csdn.net/20170801205809429)

子表数据：

![](https://img-blog.csdn.net/20170801210621157)


修改：

![](https://img-blog.csdn.net/20170801205831058)


删除：

![](https://img-blog.csdn.net/20170801205849966)





注意：如果表采用了自增主键，主键字段在插入语句中不用写出来，否则条件路由不能正确分发。







