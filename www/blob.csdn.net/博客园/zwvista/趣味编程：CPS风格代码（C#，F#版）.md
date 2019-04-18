# 趣味编程：CPS风格代码（C#，F#版） - zwvista - 博客园

## [趣味编程：CPS风格代码（C#，F#版）](https://www.cnblogs.com/zwvista/p/7698492.html)

### CPS风格代码（C#版）

```
using System;

namespace fp
{
    class CPS
    {
        static int add(int x, int y) => x + y;
        static int square(int x) => x * x;
        static int pythagoras(int x, int y) => add(square(x), square(y));
        static void add_cps(int x, int y, Action<int> f) => f(add(x, y));
        static void square_cps(int x, Action<int> f) => f(square(x));
        static void pythagoras_cps(int x, int y, Action<int> f) =>
            square_cps(x, x_squared =>
            square_cps(y, y_squared =>
            add_cps(x_squared, y_squared, f)));

        public static void Main(string[] args)
        {
            Console.WriteLine(pythagoras(3, 4)); // 25
            pythagoras_cps(3, 4, x => Console.WriteLine(x)); // 25
        }
    }
}
```

### CPS风格代码（F#版）

```
open System

let add x y = x + y
let square x = x * x
let pythagoras x y = add (square x) (square y)

let add_cps x y f = f (add x y)
let square_cps x f = f (square x)
let pythagoras_cps x y f =
    square_cps x (fun x_squared ->
    square_cps y (fun y_squared ->
    add_cps x_squared y_squared f))
    
printfn "%d" (pythagoras 3 4) // 25
pythagoras_cps 3 4 (printfn "%d") // 25
```


