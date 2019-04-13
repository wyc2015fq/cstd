
# 求整数n次方以及矩阵n次方快速解法 - bitcarmanlee的博客 - CSDN博客


2018年09月10日 22:38:58[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1581



## 1.求整数的n次方
现在想求$m^n$的值，如何用比较快速的方法求得上述值？
例如我们想求$12^{75}$的值，快速解法如下：
1.75的二进制数形式为1001011
2.$12^{75} = 12^{64} * 12^8 * 12 ^ 2 * 12$
具体求解的时候，我们先计算$12^1$，然后根据$12^1$求$12^2$，再根据$12^2$求$12^4$，以此类推，最后求$12^64$，即75的二进制数形式总共为多少位，我们就要在原基础上平方几次。这样，就将复杂度为n的计算降到了log(n)。
直接看代码
```python
public
```
```python
void
```
```python
power
```
```python
() {
```
```python
// 测试4^5的值
```
```python
int
```
```python
n =
```
```python
4
```
```python
, m =
```
```python
5
```
```python
;
```
```python
int
```
```python
power2num = n;
```
```python
int
```
```python
result =
```
```python
1
```
```python
;
```
```python
while
```
```python
(m !=
```
```python
0
```
```python
) {
```
```python
// 只有当最低位为1时，结果才乘上现在的值
```
```python
if
```
```python
((m &
```
```python
1
```
```python
) !=
```
```python
0
```
```python
)
                result *= power2num;
```
```python
// 每移位一次，幂方计算一次
```
```python
power2num *= power2num;
            m >>=
```
```python
1
```
```python
;
        }
    }
```
## 2.求矩阵的n次方
矩阵n次方的求法与整数n次方的求法思路类似。代码如下
先看两个矩阵相乘的代码
```python
public
```
```python
long
```
```python
[][]
```
```python
matrixMult
```
```python
(
```
```python
long
```
```python
[][] a,
```
```python
long
```
```python
[][] b) {
```
```python
// a的列必须与b的行相等
```
```python
assert
```
```python
a.length == b[
```
```python
0
```
```python
].length;
```
```python
int
```
```python
n = a.length, m = a[
```
```python
0
```
```python
].length, p = b[
```
```python
0
```
```python
].length;
```
```python
long
```
```python
[][] result =
```
```python
new
```
```python
long
```
```python
[n][p];
```
```python
//矩阵乘法的基本方式为三层循环
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
; i<n; i++) {
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
; j<p; j++) {
```
```python
for
```
```python
(
```
```python
int
```
```python
k=
```
```python
0
```
```python
; k<m; k++) {
                    result[i][j] += a[i][k] * b[k][j];
                }
            }
        }
```
```python
return
```
```python
result;
    }
```
```python
public
```
```python
long
```
```python
[][]
```
```python
matrixPower
```
```python
(
```
```python
long
```
```python
[][] matrix,
```
```python
int
```
```python
p) {
```
```python
long
```
```python
[][] result =
```
```python
new
```
```python
long
```
```python
[matrix.length][matrix[
```
```python
0
```
```python
].length];
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
; i<result.length; i++) {
            result[i][i] =
```
```python
1
```
```python
;
        }
```
```python
long
```
```python
[][] pingfang = matrix;
```
```python
for
```
```python
(; p !=
```
```python
0
```
```python
; p >>=
```
```python
1
```
```python
) {
```
```python
if
```
```python
((p &
```
```python
1
```
```python
) !=
```
```python
0
```
```python
) {
```
```python
// 注意result在前面
```
```python
result = matrixMult(result, pingfang);
            }
            pingfang = matrixMult(pingfang, pingfang);
        }
```
```python
return
```
```python
result;
    }
```

