# JAVA 判断一个字符串是不是一个合法的日期格式 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JAVA 判断一个字符串是不是一个合法的日期格式](https://www.cnblogs.com/xdp-gacl/p/3548307.html)



最近开发公司的项目，一直找不到合适的正则表达式可以判断一个字符串是否可以转成日期，今天发现可以采用SimpleDateFormat类的parse方法进行判断，如果转换不成功，就会出现异常，

具体代码如下：

```
1 public static boolean isValidDate(String str) {
 2       boolean convertSuccess=true;
 3 　　　　　// 指定日期格式为四位年/两位月份/两位日期，注意yyyy/MM/dd区分大小写；
 4        SimpleDateFormat format = new SimpleDateFormat("yyyy/MM/dd HH:mm");
 5        try {
 6 　　　　　// 设置lenient为false. 否则SimpleDateFormat会比较宽松地验证日期，比如2007/02/29会被接受，并转换成2007/03/01
 7           format.setLenient(false);
 8           format.parse(str);
 9        } catch (ParseException e) {
10           // e.printStackTrace();
11 // 如果throw java.text.ParseException或者NullPointerException，就说明格式不对
12            convertSuccess=false;
13        } 
14        return convertSuccess;
15 }
```















