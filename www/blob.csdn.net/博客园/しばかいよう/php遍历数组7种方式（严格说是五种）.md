
# php遍历数组7种方式（严格说是五种） - しばかいよう - 博客园






数组：
\$arr = array(1,2,3,4,5);
第一种：foreach (最常见的)
foreach (\$arr as \$v){
echo \$v;
}第二种：for
for(\$i=0;\$i<count(\$arr);\$i++){
echo \$arr[\$i];
}第三种：while each(7.2之后被废弃了)
while(list(\$key,\$value)=each(\$arr))
{
echo \$value;
}第四种：array_map
\$arr = array_map(function (\$item) {
echo \$item;
}, \$arr);第五种：array_walk
array_walk(\$arr, function (&\$item) {
echo \$item;
});第六种：each
while (list(\$key, \$value) = each(\$arr)) {    echo \$value;}第七种：多个内置组合
reset(\$arr); //指针切换到头部，其实默认就是指向开头，可以忽略这句while (\$value = current(\$arr)) {
// key() 获取当前指针的元素下标
echo \$value;
// next() 指针移到下一个
next(\$arr);
}以上也是一道面试题，如有其它方式请加评论




