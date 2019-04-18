# 一句SQL，判断char列的值是否组成回文字符串 - Soul Joy Hub - CSDN博客

2016年07月02日 22:41:32[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：722


Table t has 2 columns: 

id INT; 

value CHAR(1); 

Column id starts from 0, increased by 1 each row 

Column value is a single character string 

Table t has at least 1 row 

String s is a palindrome when: 

s[i] = s[s.length - i - 1] for i = 0 … (s.length – 1) / 2 

E.g.: a, aba, abba. 

Q: Write one SQL statement to check if the string composed of value of t ordered by id is a palindrome (case sensitive). 

Output “Y” or “N”. 
![这里写图片描述](https://img-blog.csdn.net/20160702222652376)

```
with tmp as 
(
    (select (select count(1) from t1)-1-id as id,value from t1)
except
    (select id,value from t1)
)
select 
case when count(1)=0 
then 'Y' else 'N' end 
from tmp;
```

