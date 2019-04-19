# Java中HashMap - LC900730的博客 - CSDN博客
2017年12月13日 15:09:23[lc900730](https://me.csdn.net/LC900730)阅读数：90
- HashMap可以接收null键值，而HashTable不能。
- HashMap存储的是键值对
- HashMap是基于hash的原理的，我们使用put(key,value)存储对象到HashMap中，使用get(key)从HashMap中获取对象。当我们给put()方法传递键值对时候，我们先对键调用hashCode()方法，返回的hashCode用于找到bucket位置来存储Entry对象。
- HashMap是在bucket中存储键对象和值对象，作为map.entry对象。
