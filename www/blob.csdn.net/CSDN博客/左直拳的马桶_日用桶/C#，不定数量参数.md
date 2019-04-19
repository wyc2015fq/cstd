# C#，不定数量参数 - 左直拳的马桶_日用桶 - CSDN博客
2013年02月04日 18:52:40[左直拳](https://me.csdn.net/leftfist)阅读数：6521
声明：
```
void Merge(params int[] arWk)
    {
        int[] quota = new int[arWk.Length];
        ...
    }
```
调用
`Merge(182846, 15303);`
`Merge(182846, 15303,1,2,3,4);`
