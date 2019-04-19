# MySQL表简化周期性统计递归树结构的保存过程 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年04月12日 17:53:38[boonya](https://me.csdn.net/boonya)阅读数：47标签：[mysql																[表设计](https://so.csdn.net/so/search/s.do?q=表设计&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[Mysql/Mycat](https://blog.csdn.net/boonya/article/category/6397509)
递归树结构的保存如果按照行来处理，如果是历史数据完全可以封装好JSON数据作为一条记录保存而无需保存成数据结构。对于这类数据需要考虑数据库记录条数，JSON封装格式和极简方式都是在不需要重新构造树结构的基础上实现的，避免了多行记录问题。
**目录**
[数据结构方式](#%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E6%96%B9%E5%BC%8F)
[表汇总](#%E8%A1%A8%E6%B1%87%E6%80%BB)
[轨迹统计年历史表(XH_GJTJ_YEAR_TB)](#%E8%BD%A8%E8%BF%B9%E7%BB%9F%E8%AE%A1%E5%B9%B4%E5%8E%86%E5%8F%B2%E8%A1%A8(XH_GJTJ_YEAR_TB))
[轨迹统计月历史表(XH_GJTJ_MONTH_TB](#%E8%BD%A8%E8%BF%B9%E7%BB%9F%E8%AE%A1%E6%9C%88%E5%8E%86%E5%8F%B2%E8%A1%A8(XH_GJTJ_MONTH_TB)
[轨迹统计周历史表(XH_GJTJ_WEEK_TB)](#%E8%BD%A8%E8%BF%B9%E7%BB%9F%E8%AE%A1%E5%91%A8%E5%8E%86%E5%8F%B2%E8%A1%A8(XH_GJTJ_WEEK_TB))
[轨迹统计日历史表(XH_GJTJ_DAY_TB)](#%E8%BD%A8%E8%BF%B9%E7%BB%9F%E8%AE%A1%E6%97%A5%E5%8E%86%E5%8F%B2%E8%A1%A8(XH_GJTJ_DAY_TB))
[事件统计年历史表(XH_SJTJ_YEAR_TB)](#%E4%BA%8B%E4%BB%B6%E7%BB%9F%E8%AE%A1%E5%B9%B4%E5%8E%86%E5%8F%B2%E8%A1%A8(XH_SJTJ_YEAR_TB))
[事件统计月历史表(XH_SJTJ_MONTH_TB](#%E4%BA%8B%E4%BB%B6%E7%BB%9F%E8%AE%A1%E6%9C%88%E5%8E%86%E5%8F%B2%E8%A1%A8(XH_SJTJ_MONTH_TB)
[事件统计周历史表(XH_SJTJ_WEEK_TB)](#%E4%BA%8B%E4%BB%B6%E7%BB%9F%E8%AE%A1%E5%91%A8%E5%8E%86%E5%8F%B2%E8%A1%A8(XH_SJTJ_WEEK_TB))
[事件统计日历史表(XH_SJTJ_DAY_TB)](#%E4%BA%8B%E4%BB%B6%E7%BB%9F%E8%AE%A1%E6%97%A5%E5%8E%86%E5%8F%B2%E8%A1%A8(XH_SJTJ_DAY_TB))
[JSON封装格式](#JSON%E5%B0%81%E8%A3%85%E6%A0%BC%E5%BC%8F)
[轨迹统计年表(XH_GJTJ_YEAR_TB)](#%E8%BD%A8%E8%BF%B9%E7%BB%9F%E8%AE%A1%E5%B9%B4%E8%A1%A8(XH_GJTJ_YEAR_TB))
[轨迹统计月表(XH_GJTJ_MONTH_TB)](#%E8%BD%A8%E8%BF%B9%E7%BB%9F%E8%AE%A1%E6%9C%88%E8%A1%A8(XH_GJTJ_MONTH_TB))
[轨迹统计周表(XH_GJTJ_WEEK_TB)](#%E8%BD%A8%E8%BF%B9%E7%BB%9F%E8%AE%A1%E5%91%A8%E8%A1%A8(XH_GJTJ_WEEK_TB))
[轨迹统计日表(XH_GJTJ_DAY_TB)](#%E8%BD%A8%E8%BF%B9%E7%BB%9F%E8%AE%A1%E6%97%A5%E8%A1%A8(XH_GJTJ_DAY_TB))
[事件统计年表(XH_SJTJ_YEAR_TB)](#%E4%BA%8B%E4%BB%B6%E7%BB%9F%E8%AE%A1%E5%B9%B4%E8%A1%A8(XH_SJTJ_YEAR_TB))
[事件统计月表(XH_SJTJ_MONTH_TB)](#%E4%BA%8B%E4%BB%B6%E7%BB%9F%E8%AE%A1%E6%9C%88%E8%A1%A8(XH_SJTJ_MONTH_TB))
[事件统计周表(XH_SJTJ_WEEK_TB)](#%E4%BA%8B%E4%BB%B6%E7%BB%9F%E8%AE%A1%E5%91%A8%E8%A1%A8(XH_SJTJ_WEEK_TB))
[事件统计日表(XH_SJTJ_DAY_TB)](#%E4%BA%8B%E4%BB%B6%E7%BB%9F%E8%AE%A1%E6%97%A5%E8%A1%A8(XH_SJTJ_DAY_TB))
[极简模式](#%E6%9E%81%E7%AE%80%E6%A8%A1%E5%BC%8F)
[轨迹统计表(XH_GJTJ_TB)](#%E8%BD%A8%E8%BF%B9%E7%BB%9F%E8%AE%A1%E8%A1%A8(XH_GJTJ_TB))
[事件统计表(XH_SJTJ_TB)](#%E4%BA%8B%E4%BB%B6%E7%BB%9F%E8%AE%A1%E8%A1%A8(XH_SJTJ_TB))
# 数据结构方式
## **表汇总**
|**序号**|**表名**|**表别名**|**备注**|
|----|----|----|----|
|1|XH_GJTJ_YEAR_TB|轨迹统计年历史表|年统计（次年计算）|
|2|XH_GJTJ_MONTH_TB|轨迹统计月历史表|月统计（12个月）（次月计算）|
|3|XH_GJTJ_WEEK_TB|轨迹统计周历史表|周统计（一年52个星期，从1月1日到12月31日为止不足7天仍然计算）顶格计算53周（下一周计算）|
|4|XH_GJTJ_DAY_TB|轨迹统计日历史表|可查看前天、昨天、今天（次日计算）|
|5|XH_SJTJ_YEAR_TB|事件统计年表(事件类型不固定)|年统计（次年计算）|
|6|XH_SJTJ_MONTH_TB|事件统计月历史表(事件类型不固定)|月统计（12个月）（次月计算）|
|7|XH_SJTJ_WEEK_TB|事件统计周历史表(事件类型不固定)|周统计（一年52个星期，从1月1日到12月31日为止不足7天仍然计算）顶格计算53周（下一周计算）|
|8|XH_SJTJ_DAY_TB|事件统计日历史表(事件类型不固定)|可查看前天、昨天、今天（次日计算）|
## **轨迹统计年历史表(XH_GJTJ_YEAR_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|NAME|显示名称|VARCHAR|50|0|机构名称或护林员姓名|
|3|HLY_ID|护林员|INT|20|0|表示护林员，存储机构时为空|
|4|NSJGID|组织机构|INT|11|0||
|5|YEAR|年度|INT|4|0|2019|
|6|TOTAL_TIME|总时长|DECIMAL|20|2|小时|
|7|TOTAL_MILEAGE|总里程|DECIMAL|20|2|公里|
|8|ZONE_TOTAL_TIME|围栏内时长|DECIMAL|20|2|小时|
|9|ZONE_TOTAL_MILEAGE|围栏内里程|DECIMAL|20|2|公里|
|10|TOTAL_DAY|总巡护天数|INT|20|0|天|
|11|AVE_TIME|平均时长|DECIMAL|4|2|小时/天|
|12|AVE_MILEAGE|平均里程|DECIMAL|5|2|公里/天|
|13|ALLSJ_TOTAL|事件总数|INT|20|0||
|14|UNDO_TOTAL|未处理事件数|INT|11|0||
|15|PARID|父节点ID|INT|11|0|据此可递归生成树|
## **轨迹统计月历史表(XH_GJTJ_MONTH_TB**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|NAME|显示名称|VARCHAR|50|0|机构名称或护林员姓名|
|3|HLY_ID|护林员|INT|20|0|表示护林员，存储机构时为空|
|4|NSJGID|组织机构|INT|11|0||
|5|YEAR|年度|INT|4|0|2019|
|6|MONTH|月份|INT|2|0|1-12|
|7|TOTAL_TIME|总时长|DECIMAL|20|2|小时|
|8|TOTAL_MILEAGE|总里程|DECIMAL|20|2|公里|
|9|ZONE_TOTAL_TIME|围栏内时长|DECIMAL|20|2|小时|
|10|ZONE_TOTAL_MILEAGE|围栏内里程|DECIMAL|20|2|公里|
|11|TOTAL_DAY|总巡护天数|INT|20|0|天|
|12|AVE_TIME|平均时长|DECIMAL|4|2|小时/天|
|13|AVE_MILEAGE|平均里程|DECIMAL|5|2|公里/天|
|14|ALLSJ_TOTAL|事件总数|INT|20|0||
|15|UNDO_TOTAL|未处理事件数|INT|11|0||
|16|PARID|父节点ID|INT|11|0|据此可递归生成|
## **轨迹统计周历史表(XH_GJTJ_WEEK_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|NAME|显示名称|VARCHAR|50|0|机构名称或护林员姓名|
|3|HLY_ID|护林员|INT|20|0|表示护林员，存储机构时为空|
|4|NSJGID|组织机构|INT|11|0||
|5|YEAR|年度|INT|4|0|2019|
|6|WEEK|周数|INT|2|0|1-53（本年度全年周数）|
|7|TOTAL_TIME|总时长|DECIMAL|20|2|小时|
|8|TOTAL_MILEAGE|总里程|DECIMAL|20|2|公里|
|9|ZONE_TOTAL_TIME|围栏内时长|DECIMAL|20|2|小时|
|10|ZONE_TOTAL_MILEAGE|围栏内里程|DECIMAL|20|2|公里|
|11|TOTAL_DAY|总巡护天数|INT|20|0|天|
|12|AVE_TIME|平均时长|DECIMAL|4|2|小时/天|
|13|AVE_MILEAGE|平均里程|DECIMAL|5|2|公里/天|
|14|ALLSJ_TOTAL|事件总数|INT|20|0||
|15|UNDO_TOTAL|未处理事件数|INT|11|0||
|16|PARID|父节点ID|INT|11|0|据此可递归生成树|
## **轨迹统计日历史表(XH_GJTJ_DAY_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|NAME|显示名称|VARCHAR|50|0|机构名称或护林员姓名|
|3|HLY_ID|护林员|INT|20|0|表示护林员，存储机构时为空|
|4|NSJGID|组织机构|INT|11|0||
|5|YEAR|年度|INT|4|0|2019|
|6|MONTH|月份|INT|2|0|1-12（提高查询效率条件）|
|7|DAY|日期|DATE|2|0|日期2019-04-10|
|8|TOTAL_TIME|总时长|DECIMAL|20|2|小时|
|9|TOTAL_MILEAGE|总里程|DECIMAL|20|2|公里|
|10|ZONE_TOTAL_TIME|围栏内时长|DECIMAL|20|2|小时|
|11|ZONE_TOTAL_MILEAGE|围栏内里程|DECIMAL|20|2|公里|
|12|TOTAL_DAY|总巡护天数|INT|20|0|天|
|13|AVE_TIME|平均时长|DECIMAL|4|2|小时/天|
|14|AVE_MILEAGE|平均里程|DECIMAL|5|2|公里/天|
|15|ALLSJ_TOTAL|事件总数|INT|20|0||
|16|UNDO_TOTAL|未处理事件数|INT|11|0||
|17|PARID|父节点ID|INT|11|0|据此可递归生成树|
## **事件统计年****历史****表****(XH_SJTJ_YEAR_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|NAME|显示名称|VARCHAR|50|0|机构名称或护林员姓名|
|3|HLY_ID|护林员|INT|20|0|表示护林员，存储机构时为空|
|4|NSJGID|组织机构|INT|11|0||
|5|YEAR|年度|INT|4|0|2019|
|6|TYPE_FIELDS|事件类型字段|VARCHAR|9000|0|YBJC,DFLM.....|
|7|TYPE_NAMES|事件类型名称|VARCHAR|9000|0|疫病监测,盗伐林木....|
|8|TYPE_VALUES|事件类型数量|VARCHAR|1000|0|20,30....|
|9|PARID|父节点ID|INT|11|0|据此可递归生成树|
## **事件统计月****历史****表****(XH_SJTJ_MONTH_TB**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|NAME|显示名称|VARCHAR|50|0|机构名称或护林员姓名|
|3|HLY_ID|护林员|INT|20|0|表示护林员，存储机构时为空|
|4|NSJGID|组织机构|INT|11|0||
|5|YEAR|年度|INT|4|0|2019|
|6|MONTH|月份|INT|2|0|1-12|
|7|TYPE_FIELDS|事件类型字段|VARCHAR|9000|0|YBJC,DFLM.....|
|8|TYPE_NAMES|事件类型名称|VARCHAR|9000|0|疫病监测,盗伐林木....|
|9|TYPE_VALUES|事件类型数量|VARCHAR|1000|0|20,30....|
|10|PARID|父节点ID|INT|11|0|据此可递归生成树|
## **事件统计周****历史****表****(XH_SJTJ_WEEK_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|NAME|显示名称|VARCHAR|50|0|机构名称或护林员姓名|
|3|HLY_ID|护林员|INT|20|0|表示护林员，存储机构时为空|
|4|NSJGID|组织机构|INT|11|0||
|5|YEAR|年度|INT|4|0|2019|
|6|WEEK|周数|INT|2|0|1-53（本年度全年周数）|
|7|TYPE_FIELDS|事件类型字段|VARCHAR|9000|0|YBJC,DFLM.....|
|8|TYPE_NAMES|事件类型名称|VARCHAR|9000|0|疫病监测,盗伐林木....|
|9|TYPE_VALUES|事件类型数量|VARCHAR|1000|0|20,30....|
|10|PARID|父节点ID|INT|11|0|据此可递归生成树|
## **事件统计日****历史****表****(XH_SJTJ_DAY_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|NAME|显示名称|VARCHAR|50|0|机构名称或护林员姓名|
|3|HLY_ID|护林员|INT|20|0|表示护林员，存储机构时为空|
|4|NSJGID|组织机构|INT|11|0||
|5|YEAR|年度|INT|4|0|2019|
|6|MONTH|月份|INT|2|0|1-12|
|7|DAY|日期|DATE|0|0|2019-04-10|
|8|TYPE_FIELDS|事件类型字段|VARCHAR|9000|0|YBJC,DFLM.....|
|9|TYPE_NAMES|事件类型名称|VARCHAR|9000|0|疫病监测,盗伐林木....|
|10|TYPE_VALUES|事件类型数量|VARCHAR|1000|0|20,30....|
|11|PARID|父节点ID|INT|11|0|据此可递归生成树|
# JSON封装格式
## **轨迹统计年表(XH_GJTJ_YEAR_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|YEAR|年度|INT|4|0|>=2015,<=当前年份|
|3|DATA|数据|TXT|默认|0|统计JOSN数据|
## **轨迹统计月表(XH_GJTJ_MONTH_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|YEAR|年度|INT|4|0|>=2015,<=当前年份|
|3|MONTH|月份|INT|2|0|1-12月|
|4|DATA|数据|TXT|默认|0|统计JOSN数据|
## **轨迹统计周表(XH_GJTJ_WEEK_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|YEAR|年度|INT|4|0|>=2015,<=当前年份|
|3|WEEK|周数|INT|2|0|1-53周|
|4|DATA|数据|TXT|默认|0|统计JOSN数据|
## **轨迹统计日表(XH_GJTJ_DAY_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|YEAR|年度|INT|4|0|>=2015,<=当前年份|
|3|MONTH|月份|INT|2|0|1-12月|
|4|DAY|日期|DATE|20|0|格式:yyyy-MM-dd|
|5|DATA|数据|TXT|默认|0|统计JOSN数据|
## **事件统计年表****(XH_SJTJ_YEAR_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|YEAR|年度|INT|4|0|>=2015,<=当前年份|
|3|DATA|数据|TXT|默认|0|统计JOSN数据|
## **事件统计月表****(XH_SJTJ_MONTH_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|YEAR|年度|INT|4|0|>=2015,<=当前年份|
|3|MONTH|月份|INT|2|0|1-12月|
|4|DATA|数据|TXT|默认|0|统计JOSN数据|
## **事件统计周表****(XH_SJTJ_WEEK_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|YEAR|年度|INT|4|0|>=2015,<=当前年份|
|3|WEEK|周数|INT|2|0|1-53周|
|4|DATA|数据|TXT|默认|0|统计JOSN数据|
## **事件统计日表****(XH_SJTJ_DAY_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|YEAR|年度|INT|4|0|>=2015,<=当前年份|
|3|MONTH|月份|INT|2|0|1-12月|
|4|DAY|日期|DATE|20|0|格式:yyyy-MM-dd|
|5|DATA|数据|TXT|默认|0|统计JOSN数据|
# 极简模式
## **轨迹****统计表****(XH_GJTJ_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|TYPE|类型|INT|1||YEAR:0,MONTH:1,WEEK:2,DAY:3|
|3|YEAR|年度|INT|4|0|>=2015,<=当前年份|
|4|MONTH|月份|INT|2|0|1-12月，-1标识无效|
|5|WEEK|周数|INT|2|0|1-53周，-1标识无效|
|6|DAY|日期|DATE|20|0|格式:yyyy-MM-dd，可为空|
|7|DATA|数据|TXT|默认|0|统计JOSN数据|
## **事件统计表****(XH_SJTJ_TB)**
|**序号**|**字段名**|**别名**|**字段类型**|**长度**|**精度**|**备注**|
|----|----|----|----|----|----|----|
|1|OBJECTID|OBJECTID|VARCHAR|255|0|唯一编号UUID|
|2|TYPE|类型|INT|1||YEAR:0,MONTH:1,WEEK:2,DAY:3|
|3|YEAR|年度|INT|4|0|>=2015,<=当前年份|
|4|MONTH|月份|INT|2|0|1-12月，-1标识无效|
|5|WEEK|周数|INT|2|0|1-53周，-1标识无效|
|6|DAY|日期|DATE|20|0|格式:yyyy-MM-dd，可为空|
|7|DATA|数据|TXT|默认|0|统计JOSN数据|
