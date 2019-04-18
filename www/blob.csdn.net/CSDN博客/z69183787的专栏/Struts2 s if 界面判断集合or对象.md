# Struts2 s:if 界面判断集合or对象 - z69183787的专栏 - CSDN博客
2013年08月07日 09:24:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9954
判断 ArrayList size 是否为0 
<s:if test="list.size==0"> 
<s:if> 
<s:else> 
</s:else> 
判断 ArrayList size 是否为null 
<s:if test="list==null"> 
<s:if> 
<s:else> 
</s:else> 
OR 
<s:if test="null==user||user.isEmpty()"> 
集合为空 
</s:if><s:else> 
集合不为空 
</s:else> 
or 
<s:if test="null!=user&&!user.isEmpty()"> 
集合不为看空 
</s:if> 
又OR 
首先说说<s:if>判断字符串的问题： 
1、<s:if test="#session.user.username=='c'"> 
这样是从session中取出username的值，并且判断其是否为c，但是这样判断是不正确的，这样判断的话，根本判断不出来，要改成下面这样： 
<s:if test="#session.user.username=='c'.toString()"> 
这样判断才能正确判断，至于原因我也不知道，在网上看到struts2中可能它判断的是char类型。 
2、<s:if test="#session.user.username=='zhangsan'"> 
这样写的就是判断username是不是zhangsan，是String的判断，这个是不用加toString()的。 
3、<s:if test="#session.user.username==0"> 
这样写的就是判断username是不是0，是int的判断。 
下面说说判空的问题： 
1、<s:if test="#session.user.username==null"> 
struts2中的判空似乎只能这么写，判断非空可以这样写： 
<s:if test="#session.user.username!=null" > 
2、在网上还看到这样的判空操作： 
<s:if test="not empty(memberEducation.doctorSchoolName)"> 
我用了用，不过没有成功的进行判断。
