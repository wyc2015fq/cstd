# 安卓开发中添加音效（soundpool方法） - CD's Coding - CSDN博客





2014年10月15日 03:00:15[糖果天王](https://me.csdn.net/okcd00)阅读数：1310








简单三部曲^_^




类中的声明部分需要事先声明好： 记得import哦



```java
SoundPool soundPool= new SoundPool(10, AudioManager.STREAM_SYSTEM, 5); 
    HashMap<Integer, Integer> soundMap = new HashMap<Integer, Integer>();
```

在OnCreate中添加一些声效

```java
soundMap.put(1, soundPool.load(this, R.raw.m_023, 1)); 
        soundMap.put(2, soundPool.load(this, R.raw.m_019, 1));
```
最后，在需要的地方调用即可



```java
if(list.size()==1) soundPool.play(soundMap.get(1), 1, 1, 0, 0, 1);
		else soundPool.play(soundMap.get(2), 1, 1, 0, 0, 1);
```










