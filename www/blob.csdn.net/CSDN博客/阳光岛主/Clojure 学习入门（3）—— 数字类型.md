
# Clojure 学习入门（3）—— 数字类型 - 阳光岛主 - CSDN博客

2013年12月22日 19:32:14[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：6233所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



**一、算术运算**
加法+：加法函数(+)接受任意数值类型的参数，返回它们的和；没有参数时返回0。
```python
$clojure
Clojure 1.2.1
user=> (+)
0
user=> (+ 1)
1
user=> (+ 1 2 3 4)
10
```
减法-：减法函数(-)接受任意数值类型的参数。如果只有一个参数，则返回它相反的数。当有多个参数时，返回第一个参数减去后面所有参数的结果。
```python
user=> (- 1)  
-1  
user=> (- 10 2)  
8  
user=> (- 10 2 3 4)  
1
```
乘法*：乘法函数 (*) 接受任意数值类型的参数并返回它们的乘积，如果只有一个参数，返回1。
```python
user=> (*)  
1  
user=> (* 1)  
1  
user=> (* 1 2 3)  
6  
user=> (* 0 1 2 3)  
0
```
除法/：除法函数 (/) 接受任意数值类型的参数。第一个参数是分子，其他任意参数是分母。如果没有分母，则函数返回 1 / 分子，否则返回分子除以分母。
```python
user=> (/)
java.lang.IllegalArgumentException: Wrong number of args (0) passed to: core$-SLASH- (NO_SOURCE_FILE:0)
user=> (/ 2)
1/2
user=> (/ 2 10)
1/5
user=> (/ 1 2 4 6)
1/48
```

商quot：商函数 (quot) 接受两个数值类型参数并返回第一个参数除以第二个参数的整数商。
```python
user=> (quot 10 3)
3
user=> (quot 10 -3)
-3
user=> (quot -10 3)
-3
user=> (quot 11 3)
3
user=> (quot 12 3)
4
user=> (quot -5.9 3)
-1
user=> (quot 15 0)
java.lang.ArithmeticException: Divide by zero (NO_SOURCE_FILE:0)
```

注意：商函数quot与/函数不是等价的
```python
user=> (= (/ 4 2) (quot 4 2))
true
user=> (= (/ 3 2) (quot 3 2))
false
user=> (/ 3 2)
3/2
user=> (quot 3 2)
1
```

取余rem：余数函数 (rem) 接受两个数值类型参数并返回第一个参数除以第二个参数的余数。
```python
user=> (rem 10 9)
1
user=> (rem 2 2)
0
user=> (rem 10 -3)
1
user=> (rem -10 3)
-1
user=> (rem -10 -3)
-1
```

取模mod：取模函数（mod）接收两个数值类型参数，如果两个参数为正整数或者同为负整数，则与rem函数返回值一致；如果其中有一个负数，则结果为rem返回值与第二个参数之和。
```python
user=> (mod 10 9)
1
user=> (mod 2 2)
0
user=> (mod 10 -3)
-2
user=> (mod -10 3)
2
user=> (mod -10 -3)
-1
user=> (mod 10 3)
1
```

(rem 10 -3)和(mod 10 -3)结果不一致，分别为1，-2。-2是如何得到的？由于（rem 10 -3）的返回值为1，那么（mod 10 -3）的返回值为(+ 1 (rem 10 -3))=-2
贴上mod函数源码，仅供参考。
```python
(defn mod  
  "Modulus of num and div. Truncates toward negative infinity."  
  {:added "1.0"  
   :static true}  
  [num div]   
  (let [m (rem num div)]   
    (if (or (zero? m) (= (pos? num) (pos? div)))  
      m   
      (+ m div))))
```
增量函数inc：函数 (inc) 接受一个数值类型参数并返回它的值加1。
```python
user=> (inc 1)  
2  
user=> (inc -1)  
0
```
减量函数 (dec)：减量函数 (dec) 接受一个数值类型参数并返回它的值减1。
```python
user=> (dec 1)  
0  
user=> (dec -1)  
-2
```
最大函数max：最大数函数 (max) 接受任意数值类型的参数并返回最大的。
```python
user=> (max 5 -1 2 3)  
5
```
最小函数min：最小数函数 (min) 接受任意数值类型的参数并返回最小的。
```python
user=> (min 5 -1 2 3.9)  
-1
```
精度函数with-precision：精度函数with-precision针对大数据值操作的精度、小数点运算函数。
```python
user=> (with-precision 10 (/ 1M 3))
0.3333333333M
user=> (with-precision 2 (/ 100M 3))
33M
user=> (with-precision 4 (/ 100M 3))
33.33M
```
**二、比较运算：**
值相等=：值相等(=)函数接受若干参数，比较若干参数值是否相等。一个参数时，返回true。参数可以为nil。与java中的equals一致。
```python
user=> (= 1)
true
user=> (= 1 1)
true
user=> (= 1 1.0)
true
user=> (= 1 2)
false
user=> (= 1 1 1)
true
user=> (= 1 1 2)
false
user=> (= nil nil)
true
```
等价值==：等价值函数接受若干参数，比较若干参数值是否等价。一个参数时，返回true。参数可以为nil、keyword等
```python
user=> (== 1 1.0)
true
user=> (== :foo)
true
user=> (== nil)
true
user=> (== nil nil)
java.lang.NullPointerException (NO_SOURCE_FILE:0)
user=> (== 1 1)
true
user=> (== 1 2)
false
```
值不同函数not=：等价于(not (= obj1 obj2))
```python
user=> (not= 1 1)  
false  
user=> (not= 1 2)  
true  
user=> (not= 1 1.0)  
true  
user=> (not= true true)  
false  
user=> (not= true false)  
true  
user=> (not= true true false)  
true
```
小于 (<) ：小于函数 (<) 接受任意数值类型的参数，如果它们按升序排列返回true，否则返回false。
```python
user=> (< 5 10)  
true  
user=> (< 5 10 9)  
false  
user=> (< 1)  
true
```
大于 (>) ：大于函数 (>) 接受任意数值类型的参数，如果它们按降序排列返回true，否则返回false。
```python
user=> (> 5 2)
true
user=> (> 5 10)
false
user=> (> 5 10 9)
false
user=> (> 1)
true
user=> (> 0)
true
user=> (> -1)
true
```
大于等于 (>=)：大于等于函数 (>=) 接受任意数值类型的参数，如果它们按降序排列或顺序相等返回true，否则返回false。
```python
user=> (>= 10 5 5)
true
user=> (>= 10 5 6)
false
user=> (>= 10 5 4)
true
```
小于等于 (<=)：小于等于函数 (<=) 接受任意数值类型的参数，如果它们按升序排列或顺序相等返回true，否则返回false。
```python
user=> (<= 2 5 5)
true
user=> (<= 2 5 4)
false
user=> (<= 2 5 6)
true
```
**三、验证函数：**
0检查zero?：0检查函数 (zero?) 接受一个数值类型参数，如果是0返回true，否则返回false
```python
user=> (zero? 0.0)
true
user=> (zero? 0)
true
user=> (zero? 1)
false
user=> (zero? 0.1)
false
```
正数检查pos?：正数检查函数 (pos?) 接受一个数值类型参数，如果是大于0返回true，否则返回false
```python
user=> (pos? -2)
false
user=> (pos? 1.2)
true
user=> (pos? 0)
false
user=> (pos? +0)
false
user=> (pos? 0.1)
true
```
负数检查neg?：负数检查函数 (neg?) 接受一个数值类型参数，如果是小于0返回true，否则返回false
```python
user=> (neg? -3)  
true  
user=> (neg? 0)  
false
```
偶数检查even?：
```python
user=> (even? 10)
true
user=> (even? 3)
false
user=> (even? 0)
true
user=> (even? -10)
true
```
奇数检查odd?：
```python
user=> (odd? 10)
false
user=> (odd? 3)
true
user=> (odd? 0)
false
user=> (odd? -10)
false
user=> (odd? -9)
true
```

数值检查number?：数值检查函数 (number?) 接受一个参数，如果是数值返回true，否则返回false
```python
user=> (number? 3.2)  
true  
user=> (number? "2")  
false
```
**四、强制转换：**
强制转换支持以下类型：
```python
byte  Coerce to byte.   
short  Coerce to short.   
int  Coerce to int.   
long  Coerce to long.   
float  Coerce to float.   
double  Coerce to double.   
bigint  Coerce to BigInteger.   
bigdec  Coerce to BigDecimal.   
num  Coerce to Number.   
rationalize  returns the rational value of num
```

示例：
```python
user=> (double 12)
12.0
user=> (short 10000)
10000
user=> (short 1000000000)
java.lang.IllegalArgumentException: Value out of range for short: 1000000000 (NO_SOURCE_FILE:0)
user=> (int 22.2)
22
user=> (long 22.2)
22
user=> (float 22)
22.0
user=> (bigint 100000000000)
100000000000
user=> (bigdec 100000000000)
100000000000M
user=> (num 22.22)
22.22
user=> (num "22.22")
java.lang.ClassCastException: java.lang.String cannot be cast to java.lang.Number (NO_SOURCE_FILE:0)
user=> (rationalize 22.22)
1111/50
user=> (rationalize 0.5)
1/2
```




