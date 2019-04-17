# onItemSeleted 获得当前position对应的item内容 - DEVELOPER - CSDN博客





2014年03月23日 16:11:39[学术袁](https://me.csdn.net/u012827205)阅读数：1131











**我们已经用习惯了onItemSeleted ，去对当前对应的position做各种处理。可是有一点估计很多人都忽略了。那就是在其对应的position上，获取对应的数据信息。**

**请看一段代码片：**






```java
@Override
 public void onItemSeleted(AdapterView<?> parent, View view, int position,
            long id) {
        // TODO Auto-generated method stub
        Object ob = parent.getItemAtPosition(position);//很经典，因为在这里可以直接得到相应的数据信息
       String sex = ob.toString();

    }
```
**是不是和有收![奋斗](http://static.blog.csdn.net/xheditor/xheditor_emot/default/struggle.gif)获？！**



