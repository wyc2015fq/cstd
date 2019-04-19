# JSONObject和JSONArray区别及基本用法 - xqhrs232的专栏 - CSDN博客
2018年11月23日 17:12:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：94
原文地址::[https://www.cnblogs.com/xuanbo/p/6913585.html](https://www.cnblogs.com/xuanbo/p/6913585.html)
相关文章
1、JSONObject、JSONArray方法使用详解----[https://blog.csdn.net/sunny_na/article/details/54630341](https://blog.csdn.net/sunny_na/article/details/54630341)
2、[JSONObject与JSONArray的使用区别](https://www.cnblogs.com/pd-love-wsf1314/p/3853768.html)----[https://www.cnblogs.com/pd-love-wsf1314/p/3853768.html](https://www.cnblogs.com/pd-love-wsf1314/p/3853768.html)
3、JSON的三种解析方式以及JSONObject、JSONArray区别----[https://blog.csdn.net/qq_32253371/article/details/78083391](https://blog.csdn.net/qq_32253371/article/details/78083391)
4、JSON学习（四）：JsonArray和JsonObject遍历方法----[https://blog.csdn.net/changhenshui1990/article/details/69950663](https://blog.csdn.net/changhenshui1990/article/details/69950663)
**一、JSONObject和JSONArray的数据表示形式**
JSONObject的数据是用 {  } 来表示的，
        例如：   { "id" : "123", "courseID" : "huangt-test", "title" : "提交作业", "content" : null  }  
而JSONArray，顾名思义是由JSONObject构成的数组，用  [ { } , { } , ......  , { } ]  来表示
       例如：   [ {  "id" : "123", "courseID" : "huangt-test", "title" : "提交作业" }  ,  {  "content" : null, "beginTime" : 1398873600000  "endTime" } ] ； 
        表示了包含2个JSONObject的JSONArray。
可以看到一个很明显的区别，一个用的是 {  }  ，一个最外面用的是 [  ]  ；
**二、如何从字符串String获得JSONObject对象和JSONArray对象**
**          JSONObject  jsonObject  = new JSONObject ( String  str);**
**         JSONArray jsonArray = new JSONArray(String    str  ) ;**
**三、如何从JSONArray中获得JSONObject对象**
       大家可以把JSONArray当成一般的数组来对待，只是获取的数据内数据的方法不一样
                   JSONObject   jsonObject = (JSONObject)jsonArray.get(i);
                   JSONObject   jsonObject  =  jsonArray.getJSONObject(i) ;  
  两者都可。第一种注意转换 
**四、获取JSON内的数据**
**      int   jid= jsonObject.getInt ( "id" ) ;    //  这里的jid得到的数据就是123.**
**      String  jcourse=jsonObject.getString( " courseID") ;   // 这里的jcourse得到的数据就是huangt-test.**
**     Strirng jcourse = jsonObject.get("courseID").toString();**
** 五、一般地 为键值对**
**                        eg:{  name:"xm", value:"张三"}**
**                         String jname = null;**
**                         if(jsonObject.get("name").equals("xm")){**
**                          jname=jsonObject.get("value").toString();**
**                        }**
