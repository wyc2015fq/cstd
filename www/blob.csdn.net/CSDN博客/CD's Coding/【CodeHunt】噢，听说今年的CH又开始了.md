# 【CodeHunt】噢，听说今年的CH又开始了 - CD's Coding - CSDN博客





2016年04月23日 17:48:58[糖果天王](https://me.csdn.net/okcd00)阅读数：729








## 0x00 前言

好难过QAQ，我知道今年的CH的时候已经快结束了…… 

到头来只写了第二题这一道题 
![00.02](http://ww4.sinaimg.cn/mw690/499103a7jw1f36p69b9ujj20ej05oq43.jpg)

不行，是时候练练C#了……
[History_Articles] 
[CH常用战略回忆 _CodeHunt战场 @ 编程之美2015资格赛](http://blog.csdn.net/okcd00/article/details/45138293)
[Linq_2015编程之美挑战赛初赛 CodeHunt赛区](http://blog.csdn.net/okcd00/article/details/45288993)
[BOP2014_CodeHunt （新手初见杀~）](http://blog.csdn.net/okcd00/article/details/23539071)
[16/04/23] 一气呵成地练一个小时好了 

[??/??/??] 嘛，虽说写了时间轴但也不一定会有下次就是了

## 0x01 Target

Zone: Pex4Fun

## 0x02 Start

闲话不多说，浪费的都是时间

### 00.01
- Attempt Time : 1
- `return -x;`

### 00.02
- Attempt Time : 1
- `return x+1;`

### 00.03
- Attempt Time : 2
- `return Math.Max(x,y);`  X
- `return x+y;`

### 00.04
- Attempt Time : 1
- `return x*x;`

### 00.05
- Can you write code that determines if the array is sorted?
- Attempt Time : 1
- 1-Star (Src Below)  **Remained**.

```
for(int i=1;i<a.Length;i++)
    if (a[i]<a[i-1]) return false;
return true;
```

### 00.06
- Can you write code that reverses the array?
- Attempt Time : 1
- (Src Below)

```
Array.Reverse(a);
return a;
```

### 00.07
- Can you write code that sorts the array?
- Attempt Time : 1
- (Src Below)

```
Array.Sort(a);
return a;
```

### 00.08
- Attempt Time : 2
- `return s[s.Length-1];` X
- 2-Star (Src Below)  **Remained**.

```
int len = s.Length;
if(len==0) return '\0';
for(int i=1;i<s.Length;i++)
    c+=(int)s[i];
return (char)(c/s.Length);
```

### 00.09
- Attempt Time : 2
- `return x-x%4;` X
- `return x-x%32;` X
- 0-Star (Src Below)  **Remained**.

Time’s up! Stop here.





