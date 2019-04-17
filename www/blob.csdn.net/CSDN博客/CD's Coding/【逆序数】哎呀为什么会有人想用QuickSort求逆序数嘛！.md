# 【逆序数】哎呀为什么会有人想用QuickSort求逆序数嘛！ - CD's Coding - CSDN博客





2016年09月29日 00:01:48[糖果天王](https://me.csdn.net/okcd00)阅读数：2479








（这篇文章底端的图为什么这么大……不管了）


[--大家好我们第一个团本CD就通了PT而且打掉了H老一呢，看不懂这行的请当它不存在--]



事情，大概是这样的—— （没错这又是一篇我被作业算法血虐的心路历程大水文）

哦对了，得先解释一下，逆序数这东西呢，可以理解为冒泡排序的过程中，bubble一次算一次逆序，全部排序完毕之后bubble了多少次，那就是逆序数是多少。

官方一点的解释呢就是：

“对于n个不同的元素，先规定各元素之间有一个标准次序（例如n个 不同的自然数，可规定从小到大为标准次序），于是在这n个元素的任一排列中，当某两个元素的先后次序与标准次序不同时，就说有1个逆序。一个排列中所有逆序总数叫做这个排列的逆序数。”


哎呀求逆序数，开心，惬意，归并走起，刷刷刷——




```python
class SortAndCount_Merge():
    def __init__(self):
        self.inList = []
        
    def mergeAndCount(self, L, R):
        RC, i, j = 0, 0, 0
        ret = []
        for k in range(len(L) + len(R)):
            if i == len(L) or j == len(R):
                ret += L[i:] + R[j:]
                break
            elif L[i] > R[j]:
                ret.append(R[j])
                RC += len(L) - i
                # The Same as:
                # RC += (len(L) + len(R))/2 - i
                j += 1
            else :
                ret.append(L[i])
                i += 1
        return (RC, ret)

    def sortAndCount(self, A):
        if len(A) < 2: return (0, A)
        mid = len(A) / 2 
        L,R = A[:mid],A[mid:]
        RC_L, L = self.sortAndCount(L)
        RC_R, R = self.sortAndCount(R)
        # There can be a better method without recursive
        # Mark for advanced
        cnt, ret = self.mergeAndCount(L, R)
        cnt += RC_L + RC_R
        return (cnt, ret)
```


然后，悲伤如我，发现了题意是要求使用【快速排序】来求…… 唔，咱们用冒泡排序+插入排序+树状数组各求一次行不，快排这么伤脑子的事情能不能就不做了？QvQ

不能。

哦……



```python
class SortAndCount_QSort():
    def __init__(self, inList):
        self.A = inList
        self.cnt = 0
    
    def swap(self, pos1, pos2):
        l,r = min(pos1, pos2), max(pos1, pos2)
        self.cnt += (r - l)
        
        tmp = self.A[l]
        self.A[l] = self.A[r]
        self.A[r] = tmp

    def sortAndCount(self, lef, rig):
        if lef >= rig: return 
        pivot = lef
		for pos in xrange(lef+1, rig+1):
            if self.A[pos] < self.A[lef]:
                pivot += 1
                self.swap(pivot, pos)
        self.swap(lef, pivot)
        self.sortAndCount(lef, pivot-1)
        self.sortAndCount(pivot+1, rig)
        return (self.cnt, self.A)
```


于是心思缜密的我去对照了一下两个算法的答案……



然后把这段注释掉了QvQ



```python
""" There will be extra counts without modified-method. 
for pos in xrange(lef+1, rig+1):
	if self.A[pos] < self.A[lef]:
		pivot += 1
		self.swap(pivot, pos)
self.swap(lef, pivot)
""" #( counts QSORT:2502239417 > MERGE:2500572073 )
```






头疼，能不能分成以pivot为界线，分成 “左边的到了左边”，“左边的到了右边”，“右边的到了左边”和“右边的到了右边”来考虑呢，然后我就写了这么个可怕的东西——

然后……鄙人就是不服，可以的——



```python
class SortAndCount_QSort():
    def __init__(self, inList):
        self.A = inList
        self.cnt = 0
    
    def swap(self, pos1, pos2):
        l,r = min(pos1, pos2), max(pos1, pos2)
        # self.cnt += (r - l)
        
        tmp = self.A[l]
        self.A[l] = self.A[r]
        self.A[r] = tmp
    
    def addPartCnt(self, pivot, dir, sig):  
        ins, insp, crs, crsp = 0, [], 0, [] # inside/cross part
        for idx in xrange( pivot + sig, dir + sig, sig ):
            if self.A[ins] < self.A[pivot]:
                insp.append(self.A[idx])
                ins += 1
            else:
                crsp.append(self.A[idx])
                crs += 1
                self.cnt += ins + 1 
        return insp, crsp, crs
        
    """ Simplified to addPartCnt() '''
    def addLefCnt(self, lef):
        lposl, L2L, lposr, L2R = 0, [], 0, []
        for idx in xrange(pivot-1, lef-1, -1):
            if self.A[lposl] < self.A[pivot]:
                L2L.append(self.A[idx])
                lposl += 1
            else:
                L2R.append(self.A[idx])
                lposr += 1
                self.cnt += lposl + 1 
        return L2L, L2R, lposr
    
    def addRigCnt(self, rig):
        rposr, R2R, rposl, R2L = 0, [], 0, []
        for idx in xrange(pivot+1, rig+1, +1):
            if self.A[rposr] > self.A[pivot]:
                R2R.append(self.A[idx])
                rposr += 1
            else:
                R2L.append(self.A[idx])
                rposl += 1
                self.cnt += rposr + 1 
        return R2R, R2L, rposl
    """
    
    def mergeAndCount(self, pivot, lef, rig):
        if not lef <= pivot <= rig: return
        ll, lr = [], []
        crsL2R, crsR2L = 0, 0
        if lef < pivot : ll, lr, crsL2R = self.addPartCnt(pivot, lef, -1) # addLefCnt()
        if rig > pivot : rr, rl, crsR2L = self.addPartCnt(pivot, rig, +1) # addRigCnt()
        ll.reverse()
        lr.reverse()
        self.cnt += crsL2R * crsR2L
        ret = ll + rl + [self.A[pivot]] + lr + rr
        if lef != 0: ret = self.A[:lef] + ret
        if rig != self.A.__len__()-1: ret = ret + self.A[rig+1:]
        self.A = ret
        
    def sortAndCount(self, lef, rig):
        if lef >= rig: return 
        pivot = lef
        self.mergeAndCount(pivot, lef, rig)
        self.sortAndCount(lef, pivot-1)
        self.sortAndCount(pivot+1, rig)
        return (self.cnt, self.A)
```


看呐我还发现了那两段是一样的，还合并成了一个函数是不是很聪明，是不是可以加分！



刚好 @ZoeCUR 来问我这道题，我三五句话给她讲懂之后，她瞬间表示了解，算法GET，三分钟后，“这个还是简单，不就几行的事情么？” WHAT？！

经夫人一番指点果然豁然开朗……

然后我发现了……原来这就是一个……线性的……写出来只要16行的……代码……

对不起是我的错，我想多了，对不起人民对不起国家：（没错前面都是废话都是废代码亲爱的读者你发现了吗？）




好的其实就是个线性的这么简单的东西QwQ—— （听说作业被抄袭也要算0分，这段先隐藏一下等作业提交截止了再发不好意思~）



```python
class SortAndCount_QSort():
    def __init__(self):
        self.cnt = 0
    
    def swap(self, pos1, pos2):
        l,r = min(pos1, pos2), max(pos1, pos2)
        # self.cnt += (r - l)
        
        tmp = self.A[l]
        self.A[l] = self.A[r]
        self.A[r] = tmp
   
    def sortAndCount(self, inList):
        c, L, R = 0, [], []
        if inList.__len__() <= 1 : return c, inList

        for idx in xrange(1, inList.__len__()) :
            if(inList[idx] < inList[0]):
                c += idx - 1 - L.__len__()
                L.append(inList[idx])
            else: R.append(inList[idx])
        c += L.__len__()
        lcnt, L = self.sortAndCount(L)
        rcnt, R = self.sortAndCount(R)
        return lcnt + c + rcnt, L + [inList[0]] + R
        
        """ There will be extra counts without modified-method. 
        for pos in xrange(lef+1, rig+1):
            if self.A[pos] < self.A[lef]:
                pivot += 1
                self.swap(pivot, pos)
        self.swap(lef, pivot)
        """ #( counts QSORT:2502239417 > MERGE:2500572073 )
```






输出结果：



```
E:\UCAS\计算机算法设计与分析\Homework\091M4041H - Assignment1_DandC>python A08.py

[ANSWER]  Merge Version : 1.21399998665 sec.
the number of inversions in Q8.txt is:  2500572073
Check Completed: List Sorted.

[ANSWER]  Qsort Version : 0.953000068665 sec.
the number of inversions in Q8.txt is:  2500572073
Check Completed: List Sorted.
```




唔，为了伸手党们，感觉该有的还是得有……像什么Main函数啊，读入输出啥的也贴一下好啦~



```python
def readFile(filename):
    with open(filename,'r') as f:
        inList = [int(x) for x in f.readlines()]
        return inList

def check(A):
    a, b = A, xrange(1,100001)
    for pos in b:
        if a[pos-1] != pos:
            return "Unmatch at", pos
    return "Check Completed: List Sorted."
    
def printAnswer(mode, t, filename, cnt, ret):
    print "\n[ANSWER] ", mode, "Version :", t, "sec."
    print "the number of inversions in", filename, "is: ", cnt
    print check(ret) #,"\nList:\n",ret
        
if __name__ == "__main__":
    filename = "Q8.txt"
    inList   = readFile(filename)
    capacity = len(inList)
    
    t, SacM  = time.time(), SortAndCount_Merge()
    cnt, ret = SacM.sortAndCount(inList)
    printAnswer("Merge", time.time() - t, filename, cnt, ret)
    
    t, SacQ  = time.time(), SortAndCount_QSort()
    cnt, ret = SacQ.sortAndCount(inList)
    printAnswer("Qsort", time.time() - t, filename, cnt, ret)
```





![](https://img-blog.csdn.net/20171009081058329?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








事情，大概是这样的——

哦对了，得先解释一下，逆序数这东西呢，可以理解为冒泡排序的过程中，bubble一次算一次逆序，全部排序完毕之后bubble了多少次，那就是逆序数是多少。

官方一点的解释呢就是：

“对于n个不同的元素，先规定各元素之间有一个标准次序（例如n个 不同的自然数，可规定从小到大为标准次序），于是在这n个元素的任一排列中，当某两个元素的先后次序与标准次序不同时，就说有1个逆序。一个排列中所有逆序总数叫做这个排列的逆序数。”








