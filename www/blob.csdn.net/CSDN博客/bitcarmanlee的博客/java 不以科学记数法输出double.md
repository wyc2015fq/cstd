
# java 不以科学记数法输出double - bitcarmanlee的博客 - CSDN博客


2017年08月02日 10:17:57[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：829


很多时候我们需要输出double数字的字符串形式。但是java默认的double输出方式为科学记数法，显然不符合我们的要求，以下两种方法都能达到我们的目的。
## 1.使用DecimalFormat类
```python
public
```
```python
static
```
```python
void
```
```python
t1
```
```python
() {
        Double num1 =
```
```python
100000000.0
```
```python
;
        System.
```
```python
out
```
```python
.println(num1);
```
```python
// 1.0E8
```
```python
DecimalFormat decimalFormat =
```
```python
new
```
```python
DecimalFormat(
```
```python
"#,##0.00"
```
```python
);
        System.
```
```python
out
```
```python
.println(decimalFormat.format(num1));
```
```python
// 100,000,000.00
```
```python
}
```
## 2.使用BigDecimal的toString方法
```python
public
```
```python
static
```
```python
void
```
```python
t2
```
```python
() {
        Double num2 =
```
```python
100000000.123456
```
```python
;
        BigDecimal bigDecimal =
```
```python
new
```
```python
BigDecimal(num2);
        System.
```
```python
out
```
```python
.println(num2);
```
```python
// 11.00000000123456E8
```
```python
String res = bigDecimal.toString();
        System.
```
```python
out
```
```python
.println(res);
```
```python
// 100000000.12345600128173828125
```
```python
BigDecimal bigDecimal2 =
```
```python
new
```
```python
BigDecimal(
```
```python
"100000000.123456"
```
```python
);
        System.
```
```python
out
```
```python
.println(bigDecimal2.toString());
```
```python
// 100000000.123456
```
```python
}
```

