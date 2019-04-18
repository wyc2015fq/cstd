# Go语言学习笔记（2） - zwvista - 博客园

## [Go语言学习笔记（2）](https://www.cnblogs.com/zwvista/p/9484645.html)

### 数组

```
var a [2]string
a[0] = "Hello"
a[1] = "World"
primes := [6]int{2, 3, 5, 7, 11, 13}
// 切片（半开区间）
var s []int = primes[1:4]
names := [4]string{
    "John",
    "Paul",
    "George",
    "Ringo",
}
a := names[0:2] // [John Paul]
b := names[1:3] // [Paul George]
b[0] = "XXX" // a == [John XXX]  b == [XXX George]
// 切片字面量
q := []int{2, 3, 5, 7, 11, 13}
r := []bool{true, false, true, true, false, true}
s := []struct {
    i int
    b bool
}{
    {2, true},
    {3, false},
    {5, true},
    {7, true},
    {11, false},
    {13, true},
}
// 省略上下界
s := []int{2, 3, 5, 7, 11, 13}
s = s[1:4] // [3 5 7]
s = s[:2] // [3 5]
s = s[1:] // [5]
```


