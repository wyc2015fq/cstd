# 一维Peak finding 算法

2015年05月12日 10:43:00 [gaobohello1987](https://me.csdn.net/gaobohello1987) 阅读数：2690



在一维数据中，peak是指某个数组元素大于它前后的数组元素，例如数组：A = ｛2，6，7，8，4，78，33｝，其中8和78分别称为peak。对于数据的边界元素来讲，由于与它相邻的元素只有一个，因此只要它大于与它相邻的那个元素，那么它也被认为是一个peak。

本文讲的peak finding算法是指在一个一维数组中如何快速找到一个peak，而不是寻找出数组中的所有peak。

首先来介绍一个简单粗暴的算法，就是按顺序扫描数组元素，直到找到一个peak为止，具体代码如下：



```python
def algorithm1(array):
    #如果数组为空，则结束。
    if len(array) <= 0:
        return None
 
    count = 0
    while(count < len(array)):
        testElem = array[count]
        if count == 0:
            adjacentElem = array[count + 1]
            if testElem >= adjacentElem:
                return testElem
        elif count == len(array) - 1:
            adjacengElem = array[count - 1]
            if testElem >= adjacentElem:
                return testElem
        else:
            adjacentElem1 = array[count - 1]
            adjacentElem2 = array[count + 1]
            if testElem >= adjacentElem1 and testElem >= adjacentElem2:
                return testElem
        count = count + 1
def main():
    array = [2,6,8,9,45,87,94,8]
    print algorithm1(array)
if __name__ == "__main__":
    main()

```

另外一个算法借助binary search的思想，首先查看数组中间的元素，如果它是一个peak则直接返回，否则如果其左边的元素大，则递归处理左半数组，如果右边元素较大，则处理右半数组。具体代码：





```python
def algorithm2(array):
    print "start.............."
    if len(array) <= 0:
        return None
    count = len(array) // 2
    testElem = array[count]
    if count == 0:
        return testElem
    else:
        adjacentElem1 = array[count - 1]
        adjacentElem2 = array[count + 1]
        if (testElem >= adjacentElem1) and (testElem >= adjacentElem2):
            return testElem
        else:
            if testElem < adjacentElem1:
                newArray = array[0:count]
            else:
                newArray = array[count + 1:len(array)]
            return algorithm2(newArray)
def main():
    array = [5,6,76,475,765,7667,7899]
    print algorithm2(array)
if __name__ == "__main__":
    main()
```

第一个算法在最坏情况下要遍历整个数组，并且要比较相邻元素的大小(每次比较是常数时间的操作)，因此时间复杂度是T(n) = n * θ(1)。

第二个算法采用二分的思想，每次递归调用时数组的长度都减小了一半，因此T(n) = T(n/2) + θ(1)，按照递归树分析此递归式，最终时间复杂度是T(n)=log(2)n。

当n=1000000时，第一种算法执行了13 sec，而第二种算法只需要0.001 sec，可见在数据量大的情况下，算法上复杂度的提高还是非常有效的。

*这里在第二个算法代码执行时，如果把return algorithm2(newArray)的return改为print，则程序的执行结果是：



```python
>>> 
start..............
[765, 7667, 7899]
start..............
[7899]
start..............
7899
None
None
>>> 
```

可以看出程序总共递归调用了三次，然而只有最内层返回了7899，其他两层都没有返回，因此最后打印出了两个None，这里的None不是程序第四行中打印出来的。因此看出递归时必须在调用算法的前面采用return语句。



检波形的波峰与波谷

```matlab

function [maxtab, mintab]=peakdet(v, delta, x)
%PEAKDET Detect peaks in a vector
%        [MAXTAB, MINTAB] = PEAKDET(V, DELTA) finds the local
%        maxima and minima ("peaks") in the vector V.
%        MAXTAB and MINTAB consists of two columns. Column 1
%        contains indices in V, and column 2 the found values.
%      
%        With [MAXTAB, MINTAB] = PEAKDET(V, DELTA, X) the indices
%        in MAXTAB and MINTAB are replaced with the corresponding
%        X-values.
%
%        A point is considered a maximum peak if it has the maximal
%        value, and was preceded (to the left) by a value lower by
%        DELTA.

% Eli Billauer, 3.4.05 (Explicitly not copyrighted).
% This function is released to the public domain; Any use is allowed.

maxtab = [];
mintab = [];

v = v(:); % Just in case this wasn't a proper vector

if nargin < 3
  x = (1:length(v))';
else 
  x = x(:);
  if length(v)~= length(x)
    error('Input vectors v and x must have same length');
  end
end
  
if (length(delta(:)))>1
  error('Input argument DELTA must be a scalar');
end

if delta <= 0
  error('Input argument DELTA must be positive');
end

mn = Inf; mx = -Inf;
mnpos = NaN; mxpos = NaN;

lookformax = 1;

for i=1:length(v)
  this = v(i);
  if this > mx, mx = this; mxpos = x(i); end
  if this < mn, mn = this; mnpos = x(i); end
  
  if lookformax
    if this < mx-delta
      maxtab = [maxtab ; mxpos mx];
      mn = this; mnpos = x(i);
      lookformax = 0;
    end  
  else
    if this > mn+delta
      mintab = [mintab ; mnpos mn];
      mx = this; mxpos = x(i);
      lookformax = 1;
    end
  end
end
```

