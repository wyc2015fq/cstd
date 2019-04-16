# $.ajax返回的JSON格式的数据后无法执行success的解决方法 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [$.ajax返回的JSON格式的数据后无法执行success的解决方法](https://www.cnblogs.com/xdp-gacl/p/3484633.html)



　　近段时间做项目，在项目使用了ajax技术，遇到了一个奇怪的问题："**$.ajax返回的JSON格式的数据无法执行success**"，代码是这样写的：

```
1 $.ajax({ 
 2 .. 
 3 dataType:'json', 
 4 ... 
 5 success:function(jsonData){ 
 6 //不执行success 
 7 }, 
 8 error:function(error){ 
 9 //总是执行这个error 
10 } 
11 });
```

　　百思不得其解啊，为什么会这样子呢？检查了一下返回的JSON字符串，我觉得JSON字符串的格式应该没有问题的，于是在网上查了一下，发现有还真有人遇到了和我一样的问题，找出原因所在"**是因为我指定了dataType:'json',这样指定之后，对Json格式的要求就非常严格起来了**，起初我的json格式是这样写的："{result:{\"success\":\"true\",\"meg\":\"success\"}}",可这样写不行，得改成这样写："{\"success\":true,\"mesg\":\"success\"}"，如果说是bool类型，则不用加引号，其它的键/值都需要加引号。**jquery 1.4以后对json格式变严格了,也就是说必须要这种格式的{"键":"值","键":"值"};像原来的{键:值，键:值}和{'键':'值','键':'值'}这种都是错误的，不合标准，所以jquery返回error。**以前没有注意到这样的问题，这次遇到了，也找到了解决办法，在此记录一下！









