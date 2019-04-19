# EL表达式中fn函数 - 零度的博客专栏 - CSDN博客
2016年07月11日 10:42:39[零度anngle](https://me.csdn.net/zmx729618)阅读数：339标签：[jstl](https://so.csdn.net/so/search/s.do?q=jstl&t=blog)
个人分类：[JSTL																[EL](https://blog.csdn.net/zmx729618/article/category/6305856)](https://blog.csdn.net/zmx729618/article/category/6300359)
JSTL 使用表达式来简化页面的代码，这对一些标准的方法，例如bean的getter/setter方法，请求参数或者context以及 session中的数据的访问非常方便，但是我们在实际应用中经常需要在页面调用对象的某些方法，例如我需要调用字符串的length方法来获取字符串的 长度时，在以往的开发过程中我们必须把对象先转为String类，然后在调用其length方法，这样的代码繁琐而且容易出错。
      因此JSTL内置了几个用于字符串操作的方法，可以直接在表达式中使用，大大的简化了代码，提供代码的可读性。在JSTL的表达是中要使用一个函数，其格式如下
**      ${ns:methodName(args....)}**
     在使用这些函数之前必须在JSP中引入标准函数的声明
**      <%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>**
     下面是JSTL中自带的方法列表以及其描述
|**函数名**|**函数说明**|**使用举例**|
|----|----|----|
|fn:contains|判断字符串是否包含另外一个字符串|<c:if test="${fn:contains(name, searchString)}">|
|fn:containsIgnoreCase|判断字符串是否包含另外一个字符串(大小写无关)|<c:if test="${fn:containsIgnoreCase(name, searchString)}">|
|fn:endsWith|判断字符串是否以另外字符串结束|<c:if test="${fn:endsWith(filename, ".txt")}">|
|fn:escapeXml|把一些字符转成XML表示，例如<字符应该转为<|${fn:escapeXml(param:info)}|
|fn:indexOf|子字符串在母字符串中出现的位置|${fn:indexOf(name, "-")}|
|fn:join|将数组中的数据联合成一个新字符串，并使用指定字符格开|${fn:join(array, ";")}|
|fn:length|获取字符串的长度，或者数组的大小|${fn:length(shoppingCart.products)}|
|fn:replace|替换字符串中指定的字符|${fn:replace(text, "-", "•")}|
|fn:split|把字符串按照指定字符切分|${fn:split(customerNames, ";")}|
|fn:startsWith|判断字符串是否以某个子串开始|<c:if test="${fn:startsWith(product.id, "100-")}">|
|fn:substring|获取子串|${fn:substring(zip, 6, -1)}|
|fn:substringAfter|获取从某个字符所在位置开始的子串|${fn:substringAfter(zip, "-")}|
|fn:substringBefore|获取从开始到某个字符所在位置的子串|${fn:substringBefore(zip, "-")}|
|fn:toLowerCase|转为小写|${fn.toLowerCase(product.name)}|
|fn:toUpperCase|转为大写字符|${fn.UpperCase(product.name)}|
|fn:trim|去除字符串前后的空格|${fn.trim(name)}|
|函数|描述|
|----|----|
|fn:contains(string, substring)|如果参数string中包含参数substring，返回true|
|fn:containsIgnoreCase(string, substring)|如果参数string中包含参数substring（忽略大小写），返回true|
|fn:endsWith(string, suffix)|如果参数 string 以参数suffix结尾，返回true|
|fn:escapeXml(string)|将有特殊意义的XML (和HTML)转换为对应的XML character entity code，并返回|
|fn:indexOf(string, substring)|返回参数substring在参数string中第一次出现的位置|
|fn:join(array, separator)|将一个给定的数组array用给定的间隔符separator串在一起，组成一个新的字符串并返回。|
|fn:length(item)|返回参数item中包含元素的数量。参数Item类型是数组、collection或者String。如果是String类型,返回值是String中的字符数。|
|fn:replace(string, before, after)|返回一个String对象。用参数after字符串替换参数string中所有出现参数before字符串的地方，并返回替换后的结果|
|fn:split(string, separator)|返回一个数组，以参数separator 为分割符分割参数string，分割后的每一部分就是数组的一个元素|
|fn:startsWith(string, prefix)|如果参数string以参数prefix开头，返回true|
|fn:substring(string, begin, end)|返回参数string部分字符串, 从参数begin开始到参数end位置，包括end位置的字符|
|fn:substringAfter(string, substring)|返回参数substring在参数string中后面的那一部分字符串|
|fn:substringBefore(string, substring)|返回参数substring在参数string中前面的那一部分字符串|
|fn:toLowerCase(string)|将参数string所有的字符变为小写，并将其返回|
|fn:toUpperCase(string)|将参数string所有的字符变为大写，并将其返回|
|fn:trim(string)|去除参数string 首尾的空格，并将其返回|
