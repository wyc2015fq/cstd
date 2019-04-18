# Redis中PipeLine使用(二)---批量get与批量set - z69183787的专栏 - CSDN博客
2016年08月30日 18:16:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6058

批量查询的相关问题总结
再做测试之前首先向redis中批量插入一组数据
```java
1-->1
2-->2
3-->3
4-->4
5-->5
6-->6
```
现在批量get数据
```java
for (Entry<String,String> entry :map.entrySet())     
        {
             pipe.get(entry.getKey().getBytes());
        }
        List<Object> list=pipe.syncAndReturnAll();
        for(Object o:list){
            byte[] tmp=(byte[])o;
            System.out.println("---->"+new String(tmp));
        }
```
其打印结果是：
![这里写图片描述](https://img-blog.csdn.net/20151104174446312)
然而这并不是我们想要的，那怎样才能知道打印的这个value对应的key呢？
这样的应用场景很多，我们首先批量从redis get数据出来，然后将get的数据与内存中的数据进行运行在批量写入数据库！
这是需要引入一个HashMap
```java
HashMap<String, String> map=new HashMap<String, String>();
        map.put("1","1");
        map.put("2","2");
        map.put("3","3");
        map.put("4","4");
        map.put("5","5");
        map.put("6","6");
        Pipeline pipe=redis.pipelined();
        HashMap<byte[], Response<byte[]>> newMap=new HashMap<byte[], Response<byte[]>>();
        for (Entry<String,String> entry :map.entrySet()) {
            newMap.put(entry.getKey().getBytes(), pipe.get(entry.getKey().getBytes()));
        }
        pipe.sync();
        for (Entry<byte[], Response<byte[]>> entry :newMap.entrySet()) {
            Response<byte[]> sResponse=(Response<byte[]>)entry.getValue();
            System.out.println(new String(entry.getKey())+"-----"+new String(sResponse.get()).toString());
        }
```
![这里写图片描述](https://img-blog.csdn.net/20151104175736476)
批量get之后与内存的数据想加再批量set
```java
HashMap<String, String> map=new HashMap<String, String>();
        map.put("1","1");
        map.put("2","2");
        map.put("3","3");
        map.put("4","4");
        map.put("5","5");
        map.put("6","6");
        Pipeline pipe=redis.pipelined();
        HashMap<byte[], Response<byte[]>> newMap=new HashMap<byte[], Response<byte[]>>();
        for (Entry<String,String> entry :map.entrySet()) {
            newMap.put(entry.getKey().getBytes(), pipe.get(entry.getKey().getBytes()));
        }
        pipe.sync();
        for (Entry<byte[], Response<byte[]>> entry :newMap.entrySet()) {
            Response<byte[]> sResponse=(Response<byte[]>)entry.getValue();
            long temp=Long.valueOf(Long.parseLong(map.get(new String(entry.getKey())))+Long.parseLong(new String(sResponse.get()).toString()));
            map.put(new String(entry.getKey()), Long.toString(temp));
        }
        for (Entry<String,String> entry :map.entrySet()) {
           pipe.set(entry.getKey().getBytes(), entry.getValue().getBytes());
        }
        pipe.sync();
```
