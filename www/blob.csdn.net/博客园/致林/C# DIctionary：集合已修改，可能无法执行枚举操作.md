
# C\# DIctionary：集合已修改，可能无法执行枚举操作 - 致林 - 博客园






# [C\# DIctionary：集合已修改，可能无法执行枚举操作](https://www.cnblogs.com/bincoding/p/8649850.html)
C\#中直接对集合Dictionary进行遍历并修改其中的值，会报错，如下代码就会报错：集合已修改;可能无法执行枚举操作。代码如下
publicvoidForeachDic()     
{     
     Dictionary<String, Int32> dic =newDictionary<String, Int32>();   
     dic.Add("1",10);     
     dic.Add("2",20);     
     dic.Add("3",30);foreach(KeyValuePair<String, Int32> kvpindic)     
     {     
         Console.WriteLine(String.Format("Key:{0}; Value:{1}", kvp.Key, kvp.Value));     
         dic[kvp.Key]=100;//此操作会报错:集合已修改;可能无法执行枚举操作。}     
}
解决方法就是我们可以另外创建一个数组来循环修改集合值，代码如下：
privatevoidForeachDic()     
{     
     Dictionary<String, Int32> dic =newDictionary<String, Int32>();   
     dic.Add("1",10);     
     dic.Add("2",20);     
     dic.Add("3",30);    
     String[] keyArr= dic.Keys.ToArray<String>();for(inti =0; i < keyArr.Length; i++)     
     {     
         dic[keyStr[i]]= dic[keyStr[i]] +1;     
      }     
}






