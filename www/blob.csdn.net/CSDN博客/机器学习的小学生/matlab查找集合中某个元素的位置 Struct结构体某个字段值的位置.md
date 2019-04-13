
# matlab查找集合中某个元素的位置-Struct结构体某个字段值的位置 - 机器学习的小学生 - CSDN博客


2017年05月15日 12:00:40[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：5428



```python
% 查找集合中某个元素的位置
```
```python
C =
```
```python
{
```
```python
'xlh'
```
```python
,
```
```python
'gyl'
```
```python
,
```
```python
'xyh'
```
```python
}
```
```python
;
c = strcmp(C,
```
```python
'gyl'
```
```python
);
```
```python
% logical array
```
```python
ind0 =
```
```python
find
```
```python
(c==
```
```python
1
```
```python
);
```
```python
% Struct结构体某个字段值的位置
```
```python
S =
```
```python
repmat
```
```python
(struct(
```
```python
'name'
```
```python
,
```
```python
[]
```
```python
,
```
```python
'age'
```
```python
,
```
```python
[]
```
```python
,
```
```python
'sex'
```
```python
,
```
```python
[]
```
```python
),
```
```python
3
```
```python
,
```
```python
1
```
```python
);
S(
```
```python
1
```
```python
).name =
```
```python
'xlh'
```
```python
; S(
```
```python
1
```
```python
).age =
```
```python
'20'
```
```python
;S(
```
```python
1
```
```python
).sex =
```
```python
'男'
```
```python
;
S(
```
```python
2
```
```python
).name =
```
```python
'gyl'
```
```python
; S(
```
```python
2
```
```python
).age =
```
```python
'19'
```
```python
;S(
```
```python
2
```
```python
).sex =
```
```python
'女'
```
```python
;
S(
```
```python
3
```
```python
).name =
```
```python
'xyh'
```
```python
; S(
```
```python
3
```
```python
).age =
```
```python
'5'
```
```python
;S(
```
```python
3
```
```python
).sex =
```
```python
'男'
```
```python
;
s = strcmp(
```
```python
{S.name}
```
```python
,
```
```python
'gyl'
```
```python
);
```
```python
% {S.name} :convert to cell
```
```python
ind =
```
```python
find
```
```python
(s==
```
```python
1
```
```python
);
```

