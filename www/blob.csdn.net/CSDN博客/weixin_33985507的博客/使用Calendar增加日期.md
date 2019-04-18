# 使用Calendar增加日期 - weixin_33985507的博客 - CSDN博客
2014年08月25日 16:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
```
/**
    * @Description: 当前日期加上n个月返回long date
     */
    public static long getLongDateAddMonth(int n){
        Calendar cd=Calendar.getInstance();
        Date date=new Date();
        cd.setTime(date);
        cd.add(Calendar.MONTH, n); //将当前日期加n个月
        return cd.getTime().getTime();
    }
    
    /**
     * @Description 当前日期加上n天返回long date*/
    public static long getLongDateAddDay(int n){
        Calendar cd = Calendar.getInstance();
        Date date = new Date();
        cd.setTime(date);
        cd.add(Calendar.DAY_OF_MONTH, n);
        return cd.getTime().getTime();
    }
```
