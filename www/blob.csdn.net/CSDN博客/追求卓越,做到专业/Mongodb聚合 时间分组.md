
# Mongodb聚合 时间分组 - 追求卓越,做到专业 - CSDN博客


2018年12月10日 12:25:56[Waldenz](https://me.csdn.net/enter89)阅读数：231


查询collection中，按天分组，每天的数据数量，并按照数量count倒序
`// Requires official MongoShell 3.6+
use MediaNetDB;
db.getCollection("TestCollection").aggregate(
    [
        { 
            "$project" : {
                "Day" : {
                    "$dateToString" : {
                        "format" : "%Y-%m-%d", 
                        "date" : "$AddDateTime"
                    }
                }, 
                "Time" : {
                    "$dateToString" : {
                        "format" : "%H:%M:%S:%L", 
                        "date" : "$AddDateTime"
                    }
                }
            }
        }, 
        { 
            "$group" : {
                "_id" : {
                    "Day" : "$Day"
                }, 
                "count" : {
                    "$sum" : 1.0
                }
            }
        }, 
        { 
            "$project" : {
                "_id" : 0.0, 
                "Day" : "$_id.Day", 
                "Count" : "$count"
            }
        }, 
        { 
            "$sort" : {
                "Count" : -1.0
            }
        }
    ], 
    { 
        "allowDiskUse" : false
    }
);`![](https://img-blog.csdnimg.cn/20181210122510946.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

