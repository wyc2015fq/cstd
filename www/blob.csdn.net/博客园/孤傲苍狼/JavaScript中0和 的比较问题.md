# JavaScript中0和""的比较问题 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaScript中0和""的比较问题](https://www.cnblogs.com/xdp-gacl/p/3461619.html)



今天在公司的时候发现了一个很奇怪的Js的问题，以前也没有注意到，我从数据库中取出某一个字段的值，而这个字段值刚好是0，然后我在判断这个值是不是等于""时，就出现了如下的问题：

就是**JS认为0==""是成立的，也就是0==""的结果是true**，

验证代码如下：

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


 1 <script type="text/javascript">
 2     if(0==""){
 3         alert('0==""的结果是：'+(0==""));//true
 4     }
 5 
 6     if(0===""){
 7         alert("true");
 8     }else{
 9         alert('0===""的结果是：'+(0===""));//false
10     }
11 </script>


View Code
```

 后来改成了用0===""去比较，此时的结果就是false了，看来以后对于写判断值是不是等于""之前要先注意一下值是不是等于0先再做判断了，或者直接用===去判断，用严格的方式去判断！









