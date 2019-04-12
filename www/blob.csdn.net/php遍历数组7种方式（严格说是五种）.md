
# php遍历数组7种方式（严格说是五种） - しばかいよう - 博客园
[しばかいよう](https://www.cnblogs.com/chaihy/)

皆さん、こんにちは 欢迎加入技术讨论群599606903


随笔 - 88 文章 - 9 评论 - 2

[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/chaihy/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E3%81%97%E3%81%B0%E3%81%8B%E3%81%84%E3%82%88%E3%81%86)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/chaihy/rss)![订阅](//www.cnblogs.com/images/xml.gif)
[php遍历数组7种方式（严格说是五种）](https://www.cnblogs.com/chaihy/p/10615117.html)
数组：
$arr = array(1,2,3,4,5);
第一种：foreach (最常见的)
foreach ($arr as $v){
echo $v;
}第二种：for
for($i=0;$i<count($arr);$i++){
echo $arr[$i];
}第三种：while each(7.2之后被废弃了)
while(list($key,$value)=each($arr))
{
echo $value;
}第四种：array_map
$arr = array_map(function ($item) {
echo $item;
}, $arr);第五种：array_walk
array_walk($arr, function (&$item) {
echo $item;
});第六种：each
while (list($key, $value) = each($arr)) {    echo $value;}第七种：多个内置组合
reset($arr); //指针切换到头部，其实默认就是指向开头，可以忽略这句while ($value = current($arr)) {
// key() 获取当前指针的元素下标
echo $value;
// next() 指针移到下一个
next($arr);
}以上也是一道面试题，如有其它方式请加评论




posted on2018-10-28 15:09[しばかいよう](https://www.cnblogs.com/chaihy/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=10615117)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)







## 公告



Copyright ©2019 しばかいよう

