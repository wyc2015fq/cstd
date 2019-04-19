# logstash 条件判断语句 - sxf_123456的博客 - CSDN博客
2017年09月01日 15:44:57[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：10383
                
logstash 条件判断语句
使用条件来决定filter和output处理特定的事件。logstash条件类似于编程语言。条件支持if、else if、else语句，可以嵌套。 
比较操作有： 
相等: ==, !=, <, >, <=, >= 
正则: =~(匹配正则), !~(不匹配正则) 
包含: in(包含), not in(不包含) 
布尔操作： 
and(与), or(或), nand(非与), xor(非或) 
一元运算符： 
!(取反) 
()(复合表达式), !()(对复合表达式结果取反) 
2、if[foo] in "String"在执行这样的语句是出现错误原因是没有找到叫做foo的field，无法把该字段值转化成String类型。所以最好要加field if exist判断。
判断字段是否存在，代码如下：
if ["foo"] {
  mutate {
    add_field => { "bar" => "%{foo}"}
  }
}
            
