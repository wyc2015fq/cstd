# Java开发中的一些小技巧 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Java开发中的一些小技巧](https://www.cnblogs.com/xdp-gacl/p/3490276.html)



一、 Java获取URL地址中传递的参数

```
1     /**
 2      * 获取URL中的参数名和参数值的Map集合
 3      * @param url
 4      * @return
 5      */
 6     private Map<String, String> getUrlPramNameAndValue(String url){
 7     String regEx="(\\?|&+)(.+?)=([^&]*)";//匹配参数名和参数值的正则表达式
 8         Pattern p = Pattern.compile(regEx);  
 9         Matcher m = p.matcher(url);
10 　　　　　// LinkedHashMap是有序的Map集合，遍历时会按照加入的顺序遍历输出
11     Map<String, String> paramMap = new LinkedHashMap<String, String>();
12         while(m.find()){
13         String paramName = m.group(2);//获取参数名
14         String paramVal=m.group(3);//获取参数值
15             paramMap.put(paramName, paramVal);
16         }
17         return paramMap;
18     }
```

二、获取请求的URL地址

```
1     /**
 2      * 获取请求的URL地址
 3      * @return
 4      */
 5     public String getRequestUrl(){
 6     HttpServletRequest request = ServletActionContext.getRequest();
 7 　　　　　// request.getRequestURL()获取到的是不带参数的URL，request.getQueryString()获取到的是URL的参数部分，要想获取带参数的完整URL，就需要把这两部分拼凑起来
 8     String url = request.getRequestURL()+"?"+request.getQueryString();
 9     return url;
10         
11     }
```

三、获取请求的IP地址

```
1     /**
2      * 获取请求的IP地址
3      * @return
4      */
5     public String getRequestIpAddress(){
6         return ServletActionContext.getRequest().getRemoteAddr();
7     }
```

四：判断字符串是否能够转换成指定格式的日期

```
1     /**
 2     * 验证字符串是否能够转换成指定格式的日期
 3     * @param str
 4     * @return date
 5     */
 6     public static boolean isValidDate(String str ,String formater) {
 7       boolean convertSuccess=true;
 8        SimpleDateFormat format = new SimpleDateFormat(formater);
 9        try {
10           format.setLenient(false);
11           format.parse(str);
12        } catch (ParseException e) {
13           // e.printStackTrace();
14           //如果throw java.text.ParseException或者NullPointerException，就说明格式不对
15            convertSuccess=false;
16        } 
17        return convertSuccess;
18     }
```









