# 【Java字符操作】避免模糊查询%等占位符时无效 - Big Smile - CSDN博客
2018年03月17日 22:55:01[王啸tr1912](https://me.csdn.net/tr1912)阅读数：356
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
编程语言：java
框架：SSM
模糊查询实现方式：
```
<if test="goodsCode != null">
       and (goods_code like CONCAT(CONCAT('%', #{goodsCode}), '%')
</if>
```
用concat拼接mybatis传下来的值拼接到sql中
效果：
        在搜索框输入%字符时，即使数据中有带%的数据，也查询不出来。
解决：
        在传查询值的时候，需要进行转义，用下面的操作方法：
```java
/**
* 百分号，反斜杠，下划线转义-王啸-2018年1月5日13:03:59
* @param keyword
* @return
*/
private static String transfer(String keyword) {
    if(keyword.contains("%") || keyword.contains("_")){ 
    keyword = keyword.replaceAll("\\\\", "\\\\\\\\") 
	      .replaceAll("\\%", "\\\\%") 
	      .replaceAll("\\_", "\\\\_");
    }
    return keyword;
}
```
这个方法可以直接转义下划线，反斜杠和百分号，十分方便，并且用sql或者mybatis查询可以正常查询出来。            
