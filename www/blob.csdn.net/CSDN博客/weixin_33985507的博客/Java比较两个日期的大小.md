# Java比较两个日期的大小 - weixin_33985507的博客 - CSDN博客
2015年04月10日 15:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
```
public static String getComparedSBQDate(String yxqq,String starttime){
        String str = starttime;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd", Locale.CHINA);
        try {
            Date d1 = sdf.parse(yxqq);
            Date d2 = sdf.parse(starttime);
            if(d1.before(d2)){
                 str = starttime;
            }else{
                 str = yxqq;
            }
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return str;
    }
```
