# Tuple - 左直拳的马桶_日用桶 - CSDN博客
2015年01月28日 18:25:04[左直拳](https://me.csdn.net/leftfist)阅读数：4332
如果我们想在一个方法中返回多个值，多个不同类型的值，该怎么办呢？
数组，List，Dictionary，各有千秋，但不一定都适合我们的要求。它们都可以算是个集合，更适合多行的情况。
如果定义一个类，有时又显得小题大做了点。
定义结构(struct)是个好主意，但毕竟还是要定义，郑重其事，并且往往还要引用结构所在的命名空间、类名，还是太麻烦了。
十月革命一声打炮，给我们送来了 Tuple 。
Tuple就是一个复合类型，可同时支持10个属性，并且每个属性还可以不同类型。你可以将它看成系统提供的结构。
有例子有J8：
```
Tuple<string, int> t = adapter(viewId, list);
ViewBag.ViewId = t.Item1;//Item1按照声明，自然是string
ViewBag.ObjectTypeId = t.Item2;//Item2按照声明，自然是int
Tuple<string, int> adapter(string viewId,IList<ViewTag> list)
{
    string myViewId = viewId;
    if (myViewId == "")
    {
        myViewId = list[list.Count - 1].ViewId.ToString();
    }
    return new Tuple<string,int>(myViewId,list[list.Count - 1].ObjectTypeId);
}
```
