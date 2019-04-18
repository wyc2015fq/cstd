# ReactiveX 学习笔记（11）对 LINQ 的扩展 - zwvista - 博客园

## [ReactiveX 学习笔记（11）对 LINQ 的扩展](https://www.cnblogs.com/zwvista/p/9377021.html)

### Interactive Extensions（Ix）

本文的主题为对 Ix 库，对 LINQ 的扩展。

### Buffer

[Ix.NET Buffer](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Buffer.cs)
[Ix.NET BufferTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/BufferTest.cs)

Buffer 方法将源序列按照起始位置和元素个数分成若干组，形成序列的序列。

```
var rng = Enumerable.Range(0, 10);
var res = rng.Buffer(3).ToList();
// res.Count == 4
// res[0] == { 0, 1, 2 }
// res[1] == { 3, 4, 5 }
// res[2] == { 6, 7, 8 }
// res[3] == { 9 }
```

```
var rng = Enumerable.Range(0, 10);
var res = rng.Buffer(5).ToList();
// res.Count == 2
// res[0] == { 0, 1, 2, 3, 4 }
// res[1] == { 5, 6, 7, 8, 9 }
```

```
var rng = Enumerable.Empty<int>();
var res = rng.Buffer(5).ToList();
// res.Count == 0
```

```
var rng = Enumerable.Range(0, 10);
var res = rng.Buffer(3, 2).ToList();
// res.Count == 5
// res[0] == { 0, 1, 2 }
// res[1] == { 2, 3, 4 }
// res[2] == { 4, 5, 6 }
// res[3] == { 6, 7, 8 }
// res[4] == { 8, 9 }
```

```
var rng = Enumerable.Range(0, 10);
var res = rng.Buffer(3, 4).ToList();
// res.Count == 3
// res[0] == { 0, 1, 2 }
// res[1] == { 4, 5, 6 }
// res[2] == { 8, 9 }
```

### Case

[Ix.NET Case](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Case.cs)
[Ix.NET CaseTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/CaseTest.cs)

Case 方法根据回调函数的结果选择并返回 Dictionary 参数中的序列。

```
var x = 1;
var d = 'd';
var res = EnumerableEx.Case<int, char>(() => x, new Dictionary<int, IEnumerable<char>>
{
    { 0, new[] { 'a' } },
    { 1, new[] { 'b' } },
    { 2, new[] { 'c' } },
    { 3, EnumerableEx.Defer(() => new[] { d }) },
});
// res.Single() == 'b'
// res.Single() == 'b'
x = 0;
// res.Single() == 'a'
x = 2;
// res.Single() == 'c'
x = 3;
// res.Single() == 'd'
d = 'e';
// res.Single() == 'e'
x = 4;
Assert.True(res.IsEmpty());
```

```
var x = 1;
var d = 'd';
var res = EnumerableEx.Case<int, char>(() => x, new Dictionary<int, IEnumerable<char>>
{
    { 0, new[] { 'a' } },
    { 1, new[] { 'b' } },
    { 2, new[] { 'c' } },
    { 3, EnumerableEx.Defer(() => new[] { d }) },
}, new[] { 'z' });
// res.Single() == 'b'
// res.Single() == 'b'
x = 0;
// res.Single() == 'a'
x = 2;
// res.Single() == 'c'
x = 3;
// res.Single() == 'd'
d = 'e';
// res.Single() == 'e'
x = 4;
// res.Single() == 'z'
```

### Catch

[Ix.NET Catch](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Catch.cs)
[Ix.NET CatchTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/CatchTest.cs)

```
var ex = new MyException();
var res = EnumerableEx.Throw<int>(ex).Catch<int, MyException>(e => { Assert.Same(ex, e); return new[] { 42 }; }).Single();
// res == 42
```

```
var ex = new MyException();
var res = EnumerableEx.Throw<int>(ex).Catch<int, Exception>(e => { Assert.Same(ex, e); return new[] { 42 }; }).Single();
// res == 42
```

```
var ex = new MyException();
AssertThrows<MyException>(() =>
{
    EnumerableEx.Throw<int>(ex).Catch<int, InvalidOperationException>(e => { Assert.True(false); return new[] { 42 }; }).Single();
});
```

```
var xs = Enumerable.Range(0, 10);
var res = xs.Catch<int, MyException>(e => { Assert.True(false); return new[] { 42 }; });
// res == xs
```

```
var xss = new[] { Enumerable.Range(0, 5), Enumerable.Range(5, 5) };
var res = EnumerableEx.Catch(xss);
// res == Enumerable.Range(0, 5)
```

```
var xss = new[] { Enumerable.Range(0, 5), Enumerable.Range(5, 5) };
var res = xss.Catch();
// res == Enumerable.Range(0, 5)
```

```
var xss = new[] { Enumerable.Range(0, 5), Enumerable.Range(5, 5) };
var res = xss[0].Catch(xss[1]);
// res == Enumerable.Range(0, 5)
```

```
var xss = new[] { Enumerable.Range(0, 5).Concat(EnumerableEx.Throw<int>(new MyException())), Enumerable.Range(5, 5) };
var res = EnumerableEx.Catch(xss);
// res == Enumerable.Range(0, 10)
```

```
var xss = new[] { Enumerable.Range(0, 5).Concat(EnumerableEx.Throw<int>(new MyException())), Enumerable.Range(5, 5) };
var res = xss.Catch();
// res == Enumerable.Range(0, 10)
```

```
var xss = new[] { Enumerable.Range(0, 5).Concat(EnumerableEx.Throw<int>(new MyException())), Enumerable.Range(5, 5) };
var res = xss[0].Catch(xss[1]);
// res == Enumerable.Range(0, 10)
```

```
var e1 = new MyException();
var ex1 = EnumerableEx.Throw<int>(e1);
var e2 = new MyException();
var ex2 = EnumerableEx.Throw<int>(e2);
var e3 = new MyException();
var ex3 = EnumerableEx.Throw<int>(e3);
var xss = new[] { Enumerable.Range(0, 2).Concat(ex1), Enumerable.Range(2, 2).Concat(ex2), ex3 };
var res = xss.Catch();
var e = res.GetEnumerator();
HasNext(e, 0);
HasNext(e, 1);
HasNext(e, 2);
HasNext(e, 3);
AssertThrows<MyException>(() => e.MoveNext(), ex => ex == e3);
```

```
var xs = new[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
var res = xs.Catch<int, MyException>(e => { Assert.False(true); return new[] { 42 }; });
// res == xs
```

```
var xss = new[] { new[] { 0, 1, 2, 3, 4 }, new[] { 5, 6, 7, 8, 9 } };
var res = EnumerableEx.Catch(xss);
// res == Enumerable.Range(0, 5)));
```

```
var xss = new[] { new[] { 0, 1, 2, 3, 4 }, new[] { 5, 6, 7, 8, 9 } };
var res = xss.Catch();
// res == Enumerable.Range(0, 5)));
```

```
var xss = new[] { new[] { 0, 1, 2, 3, 4 }, new[] { 5, 6, 7, 8, 9 } };
var res = xss[0].Catch(xss[1]);
// res == Enumerable.Range(0, 5)));
```

```
var xss = new[] { new[] { 0, 1, 2, 3, 4 }.Concat(EnumerableEx.Throw<int>(new MyException())), new[] { 5, 6, 7, 8, 9 } };
var res = EnumerableEx.Catch(xss);
// res == Enumerable.Range(0, 10)));
```

```
var xss = new[] { new[] { 0, 1, 2, 3, 4 }.Concat(EnumerableEx.Throw<int>(new MyException())), new[] { 5, 6, 7, 8, 9 } };
var res = xss.Catch();
// res == Enumerable.Range(0, 10)));
```

```
var xss = new[] { new[] { 0, 1, 2, 3, 4 }.Concat(EnumerableEx.Throw<int>(new MyException())), new[] { 5, 6, 7, 8, 9 } };
var res = xss[0].Catch(xss[1]);
// res == Enumerable.Range(0, 10)));
```

```
var e1 = new MyException();
var ex1 = EnumerableEx.Throw<int>(e1);
var e2 = new MyException();
var ex2 = EnumerableEx.Throw<int>(e2);
var e3 = new MyException();
var ex3 = EnumerableEx.Throw<int>(e3);
var xss = new[] { new[] { 0, 1 }.Concat(ex1), new[] { 2, 3 }.Concat(ex2), ex3 };
var res = xss.Catch();
var e = res.GetEnumerator();
HasNext(e, 0);
HasNext(e, 1);
HasNext(e, 2);
HasNext(e, 3);
AssertThrows<MyException>(() => e.MoveNext(), ex => ex == e3);
```

### Concat

[Ix.NET Concat](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Concat.cs)
[Ix.NET ConcatTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/ConcatTest.cs)

Concat 方法将若干序列连接起来。

```
var res = new[]
{
    new[] { 1, 2, 3 },
    new[] { 4, 5 }
}.Concat();
// res == { 1, 2, 3, 4, 5 }
```

```
var i = 0;
var xss = Enumerable.Range(0, 3).Select(x => Enumerable.Range(0, x + 1)).Do(_ => ++i);
var res = xss.Concat().Select(x => i + " - " + x).ToList();
/*
res == {
    "1 - 0",
    "2 - 0",
    "2 - 1",
    "3 - 0",
    "3 - 1",
    "3 - 2",
}
*/
```

```
var res = EnumerableEx.Concat(
    new[] { 1, 2, 3 },
    new[] { 4, 5 }
);
// res == { 1, 2, 3, 4, 5 }
```

### Create

[Ix.NET Create](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Create.cs)
[Ix.NET CreateTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/CreateTest.cs)

Create 方法使用协程创建序列。

```
private static IEnumerator<int> MyEnumerator()
{
    yield return 1;
    yield return 2;
}
var hot = false;
var res = EnumerableEx.Create<int>(() =>
{
    hot = true;
    return MyEnumerator();
});
// hot == false
var e = res.GetEnumerator();
// hot == true
HasNext(e, 1);
HasNext(e, 2);
NoNext(e);
hot = false;
var f = ((IEnumerable)res).GetEnumerator();
// hot == true
```

```
var xs = EnumerableEx.Create<int>(async yield =>
{
    var i = 0;
    while (i < 10)
    {
        await yield.Return(i++);
    }
});
var j = 0;
foreach (var elem in xs)
{
    // elem == j
    j++;
}
// j == 10
```

```
var xs = EnumerableEx.Create<int>(async yield =>
{
    var i = 0;
    while (true)
    {
        if (i == 10)
        {
            await yield.Break();
            return;
        }

        await yield.Return(i++);
    }
});
var j = 0;
foreach (var elem in xs)
{
    // elem == j
    j++;
}
// j == 10
```

### Defer

[Ix.NET Defer](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Defer.cs)
[Ix.NET DeferTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/DeferTest.cs)

Defer 方法使用返回序列的回调函数创建序列。

```
var i = 0;
var n = 5;
var xs = EnumerableEx.Defer(() =>
{
    i++;
    return Enumerable.Range(0, n);
});
// i == 0
// xs.ToList() == Enumerable.Range(0, n)
// i == 1
n = 3;
// xs.ToList() == Enumerable.Range(0, n)
// i == 2
```

### Distinct / DistinctUntilChanged

[Ix.NET Distinct / DistinctUntilChanged](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Distinct.cs)
[Ix.NET DistinctTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/DistinctTest.cs)
[Ix.NET DistinctUntilChangedTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/DistinctUntilChangedTest.cs)

Distinct 方法去除序列中的重复元素。

DistinctUntilChanged 方法去除序列中邻近的重复元素。

```
var res = Enumerable.Range(0, 10).Distinct(x => x % 5).ToList();
// res == Enumerable.Range(0, 5)
}
```

```
private class MyEqualityComparer : IEqualityComparer<int>
{
    public bool Equals(int x, int y)
    {
        return x % 2 == y % 2;
    }

    public int GetHashCode(int obj)
    {
        return EqualityComparer<int>.Default.GetHashCode(obj % 2);
    }
}
var res = Enumerable.Range(0, 10).Distinct(x => x % 5, new MyEqualityComparer()).ToList();
// res =={ 0, 1 }
```

```
var res = new[] { 1, 2, 2, 3, 3, 3, 2, 2, 1 }.DistinctUntilChanged().ToList();
// res == { 1, 2, 3, 2, 1 }
```

```
var res = new[] { 1, 1, 2, 3, 4, 5, 5, 6, 7 }.DistinctUntilChanged(x => x / 2).ToList();
// res == { 1, 2, 4, 6 }
```

### Do / DoWhile

[Ix.NET Do / DoWhile](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Do.cs)
[Ix.NET DoTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/DoTest.cs)
[Ix.NET DoWhileTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/DoWhileTest.cs)

Do 方法让序列在枚举元素时执行指定的函数。

DoWhile 方法通过模拟 do ... while 语句生成新的序列：无限次重复源序列中的所有元素直到指定条件不再满足为止。

```
var n = 0;
Enumerable.Range(0, 10).Do(x => n += x).ForEach(_ => { });
// n == 45
```

```
var n = 0;
Enumerable.Range(0, 10).Do(x => n += x, () => n *= 2).ForEach(_ => { });
// n == 90
```

```
var ex = new MyException();
var ok = false;
AssertThrows<MyException>(() =>
    EnumerableEx.Throw<int>(ex).Do(x => { Assert.True(false); }, e => { Assert.Equal(ex, e); ok = true; }).ForEach(_ => { })
);
// ok == true
```

```
var obs = new MyObserver();
Enumerable.Range(0, 10).Do(obs).ForEach(_ => { });
// obs.Done == true
// obs.Sum == 45
```

```
private class MyObserver : IObserver<int>
{
    public int Sum;
    public bool Done;

    public void OnCompleted()
    {
        Done = true;
    }

    public void OnError(Exception error)
    {
        throw new NotImplementedException();
    }

    public void OnNext(int value)
    {
        Sum += value;
    }
}
var sum = 0;
var done = false;
Enumerable.Range(0, 10).Do(x => sum += x, ex => { throw ex; }, () => done = true).ForEach(_ => { });
// done == true
// sum == 45
```

```
var x = 5;
var res = EnumerableEx.DoWhile(EnumerableEx.Defer(() => new[] { x }).Do(_ => x--), () => x > 0).ToList();
// res == { 5, 4, 3, 2, 1 }
```

```
var x = 0;
var res = EnumerableEx.DoWhile(EnumerableEx.Defer(() => new[] { x }).Do(_ => x--), () => x > 0).ToList();
// res == { 0 }
```

### Expand

[Ix.NET Expand](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Expand.cs)
[Ix.NET ExpandTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/ExpandTest.cs)

```
var res = new[] { 0 }.Expand(x => new[] { x + 1 }).Take(10).ToList();
// res == Enumerable.Range(0, 10)
```

```
var res = new[] { 3 }.Expand(x => Enumerable.Range(0, x)).ToList();
var exp = new[] {
    3,
    0, 1, 2,
    0,
    0, 1,
    0
};
// res == exp
```

### Finally

[Ix.NET Finally](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Finally.cs)
[Ix.NET FinallyTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/FinallyTest.cs)

```
var done = false;
var xs = Enumerable.Range(0, 2).Finally(() => done = true);
// done == false
var e = xs.GetEnumerator();
// done == false
HasNext(e, 0);
// done == false
HasNext(e, 1);
// done == false
NoNext(e);
// done == true
```

```
var done = false;
var xs = Enumerable.Range(0, 2).Finally(() => done = true);
// done == false
var e = xs.GetEnumerator();
// done == false
HasNext(e, 0);
// done == false
e.Dispose();
// done == true
```

```
var done = false;
var ex = new MyException();
var xs = EnumerableEx.Throw<int>(ex).Finally(() => done = true);
// done == false
var e = xs.GetEnumerator();
// done == false
try
{
    HasNext(e, 0);
    Assert.True(false);
}
catch (MyException ex_)
{
    Assert.Same(ex, ex_);
}
// done == true
```

### ForEach

[Ix.NET ForEach](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/ForEach.cs)
[Ix.NET ForEachTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/ForEachTest.cs)

ForEach 方法枚举序列，为每一个元素调用指定函数，有不带下标和带下标两个版本。

```
var n = 0;
Enumerable.Range(5, 3).ForEach(x => n += x);
// n == 5 + 6 + 7
```

```
var n = 0;
Enumerable.Range(5, 3).ForEach((x, i) => n += x * i);
// n == 5 * 0 + 6 * 1 + 7 * 2
```

### For

[Ix.NET For](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/For.cs)
[Ix.NET ForTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/ForTest.cs)

For 方法枚举源序列，将每一个元素都映射为一个序列，然后连接这些序列。

```
var res = EnumerableEx.For(new[] { 1, 2, 3 }, x => Enumerable.Range(0, x)).ToList();
// res == { 0, 0, 1, 0, 1, 2 }
```

### Generate

[Ix.NET Generate](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Generate.cs)
[Ix.NET GenerateTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/GenerateTest.cs)

Generate 方法通过模拟 for 循环来生成序列。

```
var res = EnumerableEx.Generate(0, x => x < 5, x => x + 1, x => x * x).ToList();
// res == { 0, 1, 4, 9, 16 }
```

### Hide

[Ix.NET Hide](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Hide.cs)
[Ix.NET HideTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/HideTest.cs)

Hide 方法隐藏序列的真实类型。

```
var xs = new List<int> { 1, 2, 3 };
var ys = xs.Hide();
// ys is not List<int>
// xs == ys
```

### If

[Ix.NET If](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/If.cs)
[Ix.NET IfTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/IfTest.cs)

If 方法根据给定条件（谓词函数）的结果
- 在两个给定序列中选择其中之一。
- 在给定序列和空序列中选择其中之一。

```
var x = 5;
var res = EnumerableEx.If(() => x > 0, new[] { +1 }, new[] { -1 });
// res.Single() == +1
x = -x;
// res.Single() == -1
```

```
var x = 5;
var res = EnumerableEx.If(() => x > 0, new[] { +1 });
// res.Single() == +1
x = -x;
// res.IsEmpty() == true
```

### IgnoreElements

[Ix.NET IgnoreElements](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/IgnoreElements.cs)
[Ix.NET IgnoreElementsTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/IgnoreElementsTest.cs)

IgnoreElements 方法将源序列转换为空序列。

```
var n = 0;
Enumerable.Range(0, 10).Do(_ => n++).IgnoreElements().Take(5).ForEach(_ => { });
// n == 10
```

### IsEmpty

[Ix.NET IsEmpty](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/IsEmpty.cs)
[Ix.NET IsEmptyTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/IsEmptyTest.cs)

IsEmpty 方法判断序列是否为空。

```
// Enumerable.Empty<int>().IsEmpty() == true
// new[] { 1 }.IsEmpty() == false
```

### Max / MaxBy

[Ix.NET Max / MaxBy](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Max.cs)
[Ix.NET MaxTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/MaxTest.cs)
[Ix.NET MaxByTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/MaxByTest.cs)

Max / MaxBy 方法返回序列中的最大值。

Max 带有一个用于比较序列中元素的 comparer 参数（IComparer 接口的实例）。

MaxBy 带有一个用于转换序列中元素的 keySelector 参数（回调函数）。

```
private class Mod7Comparer : IComparer<int>
{
    public int Compare(int x, int y)
    {
        return Comparer<int>.Default.Compare(x % 7, y % 7);
    }
}
// new[] { 2, 5, 3, 7 }.Max(new Mod7Comparer()) == 5
```

```
var res = new[] { 2, 5, 0, 7, 4, 3, 6, 2, 1 }.MaxBy(x => x % 3);
// res == { 2, 5, 2 }
```

### Memoize

[Ix.NET Memoize](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Memoize.cs)
[Ix.NET MemoizeTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/MemoizeTest.cs)
``
### Min / MinBy

[Ix.NET Min / MinBy](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Min.cs)
[Ix.NET MinTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/MinTest.cs)
[Ix.NET MinByTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/MinByTest.cs)

Min / MinBy 方法返回序列中的最小值。

Min 带有一个用于比较序列中元素的 comparer 参数（IComparer 接口的实例）。

MinBy 带有一个用于转换序列中元素的 keySelector 参数（回调函数）。

```
private class Mod3Comparer : IComparer<int>
{
    public int Compare(int x, int y)
    {
        return Comparer<int>.Default.Compare(x % 3, y % 3);
    }
}
// new[] { 5, 3, 7 }.Min(new Mod3Comparer()) == 3
```

```
var res = new[] { 2, 5, 0, 7, 4, 3, 6, 2, 1 }.MinBy(x => x % 3);
// res == { 0, 3, 6 }
```

### OnErrorResumeNext

[Ix.NET OnErrorResumeNext](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/OnErrorResumeNext.cs)
[Ix.NET OnErrorResumeNextTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/OnErrorResumeNextTest.cs)

OnErrorResumeNext 方法将若干序列连接起来，忽略序列中抛出的异常。

```
var xs = new[] { 1, 2 };
var ys = new[] { 3, 4 };
var res = xs.OnErrorResumeNext(ys);
// res == { 1, 2, 3, 4 }
```

```
var xs = new[] { 1, 2 }.Concat(EnumerableEx.Throw<int>(new MyException()));
var ys = new[] { 3, 4 };
var res = xs.OnErrorResumeNext(ys);
// res == { 1, 2, 3, 4 }
```

```
var xs = new[] { 1, 2 };
var ys = new[] { 3, 4 };
var zs = new[] { 5, 6 };
var res = EnumerableEx.OnErrorResumeNext(xs, ys, zs);
// res == { 1, 2, 3, 4, 5, 6 }
```

```
var xs = new[] { 1, 2 }.Concat(EnumerableEx.Throw<int>(new MyException()));
var ys = new[] { 3, 4 };
var zs = new[] { 5, 6 };
var res = EnumerableEx.OnErrorResumeNext(xs, ys, zs);
// res == { 1, 2, 3, 4, 5, 6 }
```

```
var xs = new[] { 1, 2 };
var ys = new[] { 3, 4 };
var res = new[] { xs, ys }.OnErrorResumeNext();
// res == { 1, 2, 3, 4 }
```

```
var xs = new[] { 1, 2 }.Concat(EnumerableEx.Throw<int>(new MyException()));
var ys = new[] { 3, 4 };
var res = new[] { xs, ys }.OnErrorResumeNext();
// res == { 1, 2, 3, 4 }
```

### Publish

[Ix.NET Publish](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Publish.cs)
[Ix.NET PublishTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/PublishTest.cs)
``
### Repeat

[Ix.NET Repeat](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Repeat.cs)
[Ix.NET RepeatTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/RepeatTest.cs)

EnumerableEx.Repeat(n) 返回由元素 n 组成的无限序列。

x.Repeat() 返回由序列 x 中的所有元素无限次重复而生成的序列。

x.Repeat(n) 返回由序列 x 中的所有元素经过 n 次重复生成的序列。

```
var xs = EnumerableEx.Repeat(42).Take(1000);
// xs.All(x => x == 42) == true
// xs.Count() == 1000
```

```
var i = 0;
var xs = new[] { 1, 2 }.Do(_ => i++).Repeat();
var res = xs.Take(10).ToList();
// res.Count == 10
// res.Buffer(2).Select(b => b.Sum()).All(x => x == 3) == true
// i == 10
```

```
var i = 0;
var xs = new[] { 1, 2 }.Do(_ => i++).Repeat(5);
var res = xs.ToList();
// res.Count == 10
// res.Buffer(2).Select(b => b.Sum()).All(x => x == 3) == true
// i == 10
```

### Retry

[Ix.NET Retry](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Retry.cs)
[Ix.NET RetryTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/RetryTest.cs)

```
var xs = Enumerable.Range(0, 10);
var res = xs.Retry();
// res == xs
```

```
var xs = Enumerable.Range(0, 10);
var res = xs.Retry(2);
// res == xs
```

```
var ex = new MyException();
var xs = Enumerable.Range(0, 2).Concat(EnumerableEx.Throw<int>(ex));
var res = xs.Retry(2);
var e = res.GetEnumerator();
HasNext(e, 0);
HasNext(e, 1);
HasNext(e, 0);
HasNext(e, 1);
AssertThrows<MyException>(() => e.MoveNext(), ex_ => ex == ex_);
```

### Return

[Ix.NET Return](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Return.cs)
[Ix.NET ReturnTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/ReturnTest.cs)

Return 方法返回由一个元素组成的序列。
`// EnumerableEx.Return(42).Single() == 42`
### Scan

[Ix.NET Scan](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Scan.cs)
[Ix.NET ScanTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/ScanTest.cs)

Scan 方法转换序列：
- Scan 带有一个有返回值的二元函数和一个可选的初值。
- 结果序列的第 1 项没有初值时等于源序列的第 1 项，有初值时等于将二元函数应用到初值和源序列的第 1 项后所得到的结果。
- 结果序列的第 n + 1 项等于将二元函数应用到结果序列的第 n 项和源序列的第 n + 1 项后所得到的结果。

```
var res = Enumerable.Range(0, 5).Scan((n, x) => n + x).ToList();
// res == { 1, 3, 6, 10 }
```

```
var res = Enumerable.Range(0, 5).Scan(10, (n, x) => n - x).ToList();
// res == { 10, 9, 7, 4, 0 }
```

### SelectMany

[Ix.NET SelectMany](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/SelectMany.cs)
[Ix.NET SelectManyTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/SelectManyTest.cs)

SelectMany 方法将源序列中的所有元素都映射成指定序列，然后连接这些序列。

```
var res = new[] { 1, 2 }.SelectMany(new[] { 'a', 'b', 'c' }).ToList();
// res == { 'a', 'b', 'c', 'a', 'b', 'c' }
```

### Share

[Ix.NET Share](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Share.cs)
[Ix.NET ShareTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/ShareTest.cs)
``
### SkipLast

[Ix.NET SkipLast](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Skip.cs)
[Ix.NET SkipLastTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/SkipLastTest.cs)

SkipLast 方法略过序列尾部的 n 个元素，返回其余元素组成的序列。

```
var e = Enumerable.Range(0, 5);
var r = e.SkipLast(3).ToList();
// r == e.Take(2)
```

### StartWith

[Ix.NET StartWith](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/StartsWith.cs)
[Ix.NET StartWithTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/StartWithTest.cs)

StartWith 方法在序列之前插入指定的（1 个或多个）元素。

```
var e = Enumerable.Range(1, 5);
var r = e.StartWith(0).ToList();
// r == Enumerable.Range(0, 6)
```

### TakeLast

[Ix.NET TakeLast](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Take.cs)
[Ix.NET TakeLastTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/TakeLastTest.cs)

TakeLast 方法返回由序列尾部的 n 个元素组成的序列。

```
var e = Enumerable.Range(0, 5);
var r = e.TakeLast(3).ToList();
// r == e.Skip(2)
```

### Throw

[Ix.NET Throw](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Throw.cs)
[Ix.NET ThrowTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/ThrowTest.cs)
``
### Using

[Ix.NET Using](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/Using.cs)
[Ix.NET UsingTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/UsingTest.cs)
``
### While

[Ix.NET While](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive/While.cs)
[Ix.NET WhileTest](https://github.com/dotnet/reactive/blob/master/Ix.NET/Source/System.Interactive.Tests/WhileTest.cs)

While 方法通过模拟 while 语句生成新的序列：只要指定条件满足就无限次重复源序列中的所有元素。

```
var x = 5;
var res = EnumerableEx.While(() => x > 0, EnumerableEx.Defer(() => new[] { x }).Do(_ => x--)).ToList();
// res == { 5, 4, 3, 2, 1 }
```

```
var x = 0;
var res = EnumerableEx.While(() => x > 0, EnumerableEx.Defer(() => new[] { x }).Do(_ => x--)).ToList();
// res == { }
```


