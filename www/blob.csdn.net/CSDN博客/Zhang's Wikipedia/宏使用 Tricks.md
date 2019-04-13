
# 宏使用 Tricks - Zhang's Wikipedia - CSDN博客


2018年03月20日 23:49:15[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：112个人分类：[tricks](https://blog.csdn.net/lanchunhui/article/category/6143385)



人为地定义一些“无意义”的宏（宏名本身有意义），以起到提升代码程序的可读性。
## 1. IN/OUT
指定参数用于输入还是输出：
```python
#define IN
```
```python
#define OUT
```
```python
void InvertMatrix(
```
```python
IN
```
```python
Matrix originalMatrix,
```
```python
OUT
```
```python
Matrix *resultMatrix
)
```
```python
;
```
```python
//
```
```python
IN
```
```python
OUT
```
```python
连用，表示直接在输入对象上进行修改后输出，也即修改是
```
```python
in
```
```python
-place（就地）的
void ChangeSentenceCase(
```
```python
IN
```
```python
StringCase desiredCase,
```
```python
IN
```
```python
OUT
```
```python
Sentence *sentenceToEdit
)
```
```python
;
```

