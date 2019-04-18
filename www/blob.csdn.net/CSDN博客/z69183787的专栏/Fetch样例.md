# Fetch样例 - z69183787的专栏 - CSDN博客
2017年11月30日 12:49:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：309
个人分类：[前端-Fetch框架](https://blog.csdn.net/z69183787/article/category/7318554)
fetch(
     "http://sre-act.adp.test.sankuai.com/sre/query/cdKey/queryByCdKey?activityId=132&cdKeys=123",
    {credentials: 'include',method:'get',headers:{'x-requested-with':'XMLHttpRequest'}}).then(function(Response){
return Response.json();
}).then(function(a){console.log(a)});
