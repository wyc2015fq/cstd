# [saiku] 优化多维度查询效率 - weixin_33985507的博客 - CSDN博客
2016年03月07日 14:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
1、优化查询精度
优化原因：当维度过多时，查询很复杂，效率较慢。因此看能否通过优化sql的方式来提高查询效率。
代码跟踪查看（维度多余3时查询条件里面有in，优化不用in关键字）
```
public abstract class AbstractQuerySpec implements QuerySpec {
            protected Map<String, String> nonDistinctGenerateSql(SqlQuery sqlQuery)｛
                String expr = column.generateExprString(sqlQuery);
                StarColumnPredicate predicate = getColumnPredicate(i);
                final String where = RolapStar.Column.createInExpr(
                    expr,
                    **predicate**,
                    column.getDatatype(),
                    sqlQuery);
                if (!where.equals("true")) {
                    sqlQuery.addWhere(where);
                }
```
```
public class Aggregation {                
createSegments｛
     new Segment(
　　　　star,
　　　　constrainedColumnsBitKey,
　　　　columns,
　　　　measure,
　　　　predicates,//这里 ： [true, true, true, F_JCJY_CODE_SSQ.SFDM={230000, 370000}] 当为具体数值不为true时使用了in(230000,370000)
　　　　Collections.<Segment.ExcludedRegion>emptyList(),compoundPredicateList);
｝
```
```
private StarColumnPredicate[] initPredicates() {
    StarColumnPredicate[] predicates =
        new StarColumnPredicate[columns.length];
    for (int j = 0; j < columns.length; j++) {
        Set<StarColumnPredicate> valueSet = valueSets[j];
        StarColumnPredicate predicate;
        if (valueSet == null) {
            predicate = LiteralStarPredicate.FALSE;
        } else {
            ValueColumnPredicate[] values =
                valueSet.toArray(
                    new ValueColumnPredicate[valueSet.size()]);
            // Sort array to achieve determinism in generated SQL.
            Arrays.sort(
                values,
                ValueColumnConstraintComparator.instance);
            predicate =
                new ListColumnPredicate(
                    columns[j],
                    Arrays.asList((StarColumnPredicate[]) values));
        }
        predicates[j] = predicate;
    }
    return predicates;
}
```
```
AggregationManager.loadAggregation(
cacheMgr,
cellRequestCount,
measuresList,
columns,
batchKey,
predicates,
groupingSetsCollector,
segmentFutures);
```
```
predicates = aggregation.optimizePredicates(columns, predicates);
public StarColumnPredicate[] optimizePredicates(
    RolapStar.Column[] columns,
    StarColumnPredicate[] predicates)
{
```
跟踪到最后，来到下面的代码处
```
if (MondrianProperties.instance().OptimizePredicates.get() || bloats[j] == 1){
    newPredicates[j] = new LiteralStarPredicate(columns[j], true);//设置为ture
}
```
第一个条件
```
MondrianProperties.instance().OptimizePredicates.get()
配置mondrian.properties里面的（默认也是true）
mondrian.rolap.aggregates.optimizePredicates=true
```
第二个条件
```
bloats[j] == 1
mondrian包里面的Aggregation类的optimizePredicates方法修改如下：
if (valueCount < 2) {
    bloats[i] = 1.0;//原来是0.0
    continue;
}
if (valueCount > maxConstraints) {           
    bloats[i] = 1.0;
    continue;
}
而 maxConstraints 是在 mondrian。properties里面配置的。
默认是：1000，表示如果in里面的值小于1000个时，采用in精细化查询，否则不带in查询语句。这里我们配置为1.
mondrian.rolap.maxConstraints=1
```
总结：需要修改的地方如下：
```
#Aggregation类
if (valueCount < 2) {
    bloats[i] = 1.0;//原来是0.0
    continue;
}
#mondrian.properties配置文件
mondrian.rolap.maxConstraints=1
mondrian.rolap.aggregates.optimizePredicates=true
```
注意：后来发现，查询效率慢主要是因为查询出的结果需要经过一系列的矩阵变换计算和递归计算，是作为数据的后处理操作。
如果报出内存溢出，请增大应用内存。
2、缓存结果
查询后将结果缓存，以便第二次查询时提高查询效率
配置mondrian.properties
```
//开启聚合表功能
mondrian.rolap.aggregates.ChooseByVolume=true
mondrian.rolap.aggregates.Read=true
mondrian.rolap.aggregates.Use=true
//缓存结果集
mondrian.rolap.cellBatchSize=0
mondrian.rolap.star.disableCaching=false 
mondrian.expCache.enable=true 
mondrian.rolap.EnableRolapCubeMemberCache=true
***mondrian.result.highCardChunkSize=50 ******mondrian.rolap.evaluate.MaxEvalDepth=1 ***//允许监控内存、显示维度前缀、空值处理********mondrian.util.memoryMonitor.enable=true ***************mondrian.olap.elements.NeedDimensionPrefix=true***************mondrian.olap.NullMemberRepresentation=\u7a7a********
//sql处理******mondrian.rolap.generate.formatted.sql=true**********mondrian.rolap.maxSqlThreads=1000********
```
3、其他方式
（1）使用聚合表
```
聚合表是数据仓库中包含事实数据的汇总信息的表
```
（2）建立物化视图
```
将查询的结果集先通过建立视图，定时跑批数据到视图中，直接查询视图得到结果，避免了查询时去计算。
简化计算和查询过程。
```
（3）退化维度
```
如果一个维度有很多可能，如学校作为维度有很多值。
这时候不再使用学校ID和表关联，而是直接将学校名称写到和他关联的表里面去。
这样就相当于少关联一张表，加快查询的效率。
```
