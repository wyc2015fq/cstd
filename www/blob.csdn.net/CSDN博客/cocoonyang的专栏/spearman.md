
# spearman - cocoonyang的专栏 - CSDN博客


2017年11月09日 20:45:42[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：110


使用JavaScript实现的计算Spearman 相关性系数的函数
```python
'use strict'
```
```python
;
```
```python
var
```
```python
DEBUG =
```
```python
0
```
```python
;
module.exports =
```
```python
function
```
```python
( )
```
```python
{
```
```python
this
```
```python
.spearman =
```
```python
function
```
```python
( x, y )
```
```python
{
```
```python
// Checking parameters
```
```python
if
```
```python
(
```
```python
undefined
```
```python
==
```
```python
typeof
```
```python
( x ) )
        {
```
```python
return
```
```python
;
         }
```
```python
if
```
```python
(
```
```python
undefined
```
```python
==
```
```python
typeof
```
```python
( y ) )
        {
```
```python
return
```
```python
;
         }
```
```python
var
```
```python
len_x = x.length;
```
```python
var
```
```python
len_y = y.length;
```
```python
var
```
```python
len = len_x < len_y ? len_x : len_y;
```
```python
if
```
```python
( len <=
```
```python
0
```
```python
)
        {
```
```python
return
```
```python
;
        }
```
```python
var
```
```python
x_mean =
```
```python
0.0
```
```python
;
```
```python
var
```
```python
y_mean =
```
```python
0.0
```
```python
;
```
```python
for
```
```python
(
```
```python
var
```
```python
i =
```
```python
0
```
```python
; i < len; i++ )
        {
             x_mean +=
```
```python
parseFloat
```
```python
(x[i]);
             y_mean +=
```
```python
parseFloat
```
```python
(y[i]);
        }
        x_mean = x_mean / len;
        y_mean = y_mean / len;
```
```python
var
```
```python
tmp_u =
```
```python
0.0
```
```python
;
```
```python
for
```
```python
(
```
```python
var
```
```python
i =
```
```python
0
```
```python
; i < len; i++ )
        {
             tmp_u += (x_mean - x[i]) * (y_mean - y[i]);
        }
```
```python
//console.log( 'tmp_u =', tmp_u );
```
```python
var
```
```python
tmp_d_x =
```
```python
0.0
```
```python
;
```
```python
for
```
```python
(
```
```python
var
```
```python
i =
```
```python
0
```
```python
; i < len; i++ )
        {
             tmp_d_x += (x_mean - x[i]) * (x_mean - x[i]);
        }
```
```python
var
```
```python
tmp_d_y =
```
```python
0.0
```
```python
;
```
```python
for
```
```python
(
```
```python
var
```
```python
i =
```
```python
0
```
```python
; i < len; i++ )
        {
             tmp_d_y += (y_mean - y[i]) * (y_mean - y[i]);
        }
```
```python
var
```
```python
rho = tmp_u /
```
```python
Math
```
```python
.sqrt( tmp_d_x * tmp_d_y )
```
```python
if
```
```python
( DEBUG ){ 
    console.log(
```
```python
'             Report   '
```
```python
)
    console.log(
```
```python
'-------------------------------'
```
```python
)
    console.log(
```
```python
'    Data Set Number ='
```
```python
, len  );
    console.log(
```
```python
'    Mean Vaule of X ='
```
```python
, x_mean  );
    console.log(
```
```python
'    Mean Vaule of Y ='
```
```python
, y_mean  );
    console.log(
```
```python
'Spearman cor of X-Y ='
```
```python
, rho  );
}
```
```python
return
```
```python
rho;
    }
}
```

