# 配合dedecms内容模型实现后台输入栏目id前端输出文章列表 - weixin_33985507的博客 - CSDN博客
2016年09月07日 16:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
　　为了简化开发的工作量，也方便编辑快速操作，决定将后台进行重新设置。配合dedecms内容模型实现后台输入栏目id前端输出文章列表，这样制作科室专题页也变快了很多。比如，我们添加一个“科室专家栏目id”的字段zjid，那么我们可以通过下面的代码来进行调用
```
{dede:arclist  addfields='zjid' row='1' channelid='24' }            
[field:zjid runphp="yes"]
global $dsql;
$sql="SELECT dede_archives.*  FROM   dede_archives left JOIN  dede_addon24  ON dede_archives.id=dede_addon24.aid  WHERE  dede_archives.typeid=@me";
$dsql->SetQuery($sql);
$dsql->Execute();
@me="";
while($row = $dsql->GetArray()){
@me.="<li><a href=\"/plus/view.php?aid=".$row['id']."\"><img  src=\"http://***.com".$row['litpic']."\"><br>".$row['title']."</a></li> ";
}
[/field:zjid]
{/dede:arclist}
```
　　channelid是内容模型的。将后台输入的id作为输出文章栏目的id，再将此栏目的文章罗列出来，不过这个的文章链接是动态的url，如果你已经做了全站伪静态了，可以加上nofollow标签
后台的效果如下
![](https://images2015.cnblogs.com/blog/567218/201609/567218-20160907165127223-106329809.jpg)
前端的效果如下
![](https://images2015.cnblogs.com/blog/567218/201609/567218-20160907165439504-16143729.jpg)
　　想做后台简化开发的朋友可以和ytkah相互交流一下，一起进步
