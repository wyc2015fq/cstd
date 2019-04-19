# 从VB与C语言的插入算法代码的不同看逻辑运算的差别 - Big Smile - CSDN博客
2016年06月11日 16:41:38[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1420
        从上次的算法课到现在为止，我才把插入排序的算法在VB上实现出来，其中有一个原因就是同样的逻辑代码，在C语言中就是不报错，在VB中就是总报错，而且还是下标越界的错误，这就让我很是奇怪，之后把C语言的代码改动了之后才明白了其中的区别和用法所在，所以今天才弄完有关VB 的插入排序算法，借此来和大家分享一下。
# 一、C语言中的排序算法
算法源码如下：
```cpp
void insort(int number[]) {
	int i, j, k, tmp;     //声明变量
	
	for(j = 1; j < MAX;j++){
		tmp = number[j];     //当前待插入的数值
		i = j - 1;
		while(i>-1 && tmp < number[i]) {     //选择插入位，
			number[i+1] = number[i];       //不满足的数字左移动一个数组空间
			i--;        
		
		}
		number[i+1] = tmp;         //把待排序的数插入数组相应位置中
	}
}
```
看起来很是简单，执行也是可以执行的，可是到了VB中这么写就会出错：
```
For j = 1 To n
        t = a(j)      '<span style="font-family: Arial, Helvetica, sans-serif;">当前待插入的数值</span>
        k = j - 1
        DoEvents
       While (k>-1 and t <= a(k))                
            a(k + 1) = a(k)         '不满足的数字左移动一个数组空间   
            k = k - 1
           DoEvents
       Wend
        a(k + 1) = t             '把待排序的数插入数组相应位置中
Next j
```
报错了：
![](https://img-blog.csdn.net/20160611162351868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 二、问题的解决与修正
        首先看调试器中的变量值，k=-1，在数组中确实没有-1这个索引的值，所以导致了数组的报错。
![](https://img-blog.csdn.net/20160611162654266?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
于是为了避免这个错误，查了好多的方法，最后把程序改成了这样：
```
For j = 1 To n
        t = a(j)
        k = j - 1
        DoEvents
       Do While (t <= a(k))
            
            a(k + 1) = a(k)
            k = k - 1
            If k = -1 Then
                Exit Do
            End If
           DoEvents
        Loop
        Wend
        a(k + 1) = t
    Next j
```
这样就可以避免错误了，但是这是为什么呢，为什么在C语言中就不会报这样的错误呢？
# 三、错误分析
        造成这种错误的原因主要是因为不同语言的解释器在编译这两种语言的代码的时候在解释逻辑判断的时候方法是不同的。上述代码用到了逻辑与操作，也就是VB 中的and运算符和C语言中的&&运算符。
        我们都知道，与运算只要有一个条件是False，那么最后运算结果就是False，C语言在执行这个运算的时候，判断第一个条件，发现是False，之后就不会再判断第二个条件了，但是在VB中他是两个条件都考虑的，所以才会出现这个错误，导致了逻辑一样的代码，换了一种语言却不能正确执行了，这一点很是重要。
# 四、DoEvents小发现
        不知道大家用VB写过排序的计时程序了没有，不知道大家是否出现了这样的问题，就是一点开始排序的瞬间，整个VB的程序就死去了，而且自己的写的计时程序好像到了最后才会显示结果，不能实时的显示，这个时候DoEvents这个命令就会帮我们大忙了，在循环中用DoEvents之后，程序执行的时候会释放CPU来处理其他的事情，对于软件体验有很大的用处，但是对于排序来说会增加排序的时间，这里需要考虑使用的地方和频率。
