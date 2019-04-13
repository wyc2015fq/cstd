
# mongodb 动态更新列值 - 追求卓越,做到专业 - CSDN博客


2018年12月11日 10:09:48[Waldenz](https://me.csdn.net/enter89)阅读数：79


mongodb中需要修改某列值可以使用$set 设置。但如果是动态修改，如在原有值上append其他字符，或则对列值进行更为复杂的更新，则需要在shell中脚本更新。
如 将userid值，前加固定字符“test”。在IntelliShell中执行一下代码
```python
db.getCollection("Log")
  .find({"_id":"5bc05a6c4e96b32bfc8d03cc"})
  .forEach(
  	  function(e,i){
  	    e.UserId="test"+e.UserId;
  	    db.getCollection("Log").save(e);  	    
  	  }
  );
db.getCollection("Log").find({"_id":"5bc05a6c4e96b32bfc8d03cc"},{UserId:1});
```
执行结果：
![](https://img-blog.csdnimg.cn/2018121110033723.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

