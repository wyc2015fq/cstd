# [CareerCup] 6.6 Toggle Lockers 切换锁的状态 - Grandyang - 博客园







# [[CareerCup] 6.6 Toggle Lockers 切换锁的状态](https://www.cnblogs.com/grandyang/p/4762885.html)







6.6 There are 100 closed lockers in a hallway. A man begins by opening all 100 lockers. Next, he closes every second locker. Then, on his third pass, he toggles every third locker (closes it if it is open or opens it if it is closed). This process continues for 100 passes, such that on each pass i, the man toggles every ith locker. After his 100th pass in the hallway, in which he toggles only locker #100, how many lockers are open?



这道题说一个走廊上有100个闭合的锁，首先一个人走过去打开所有的锁，第二次他切换2的倍数的锁的状态，第三次他切换3的倍数的锁的状态，第n次他切换n的倍数的锁的状态，以此类推直到100次遍历后，问我们有多少个锁的状态是打开的。

看到这类的应用题，我不禁想起来了国内的被骂的很惨的数学题，比如有个蓄水池，以啥啥啥速度往里进水，又以啥啥啥速度排水，问多长时间能蓄满或是排空，要么就是有两个人相向而行，中间有条狗，以匀速往返跑，求两人相遇后狗跑了几个来回等等之类的题，很多实用主义者痛批此类题毫无实际意义，其实也不必那么较真，就是练练脑子而已，看人家国外不也用此类的题目来面试嘛。

那么我们来看这道题吧，还是先枚举个小例子来分析下，比如只有5个锁的情况，'X'表示关闭，‘√’表示打开，如下所示：

初始状态：    X    X    X    X    X

第一次：      √    √    √    √    √

第二次：      √     X    √    X    √

第三次：      √     X    X    X    √

第四次：      √     X    X    √    √

第五次：      √     X    X    √    X

那么最后我们发现五次遍历后，只有1号和4号锁是打开的，而且很巧的是它们都是平方数，是巧合吗，还是其中有什么玄机。我们仔细想想，对于第n个锁，只有当次数是n的因子的之后，才能改变锁的状态，即n能被当前次数整除，比如当n为36时，它的因数有(1,36), (2,18), (3,12), (4,9), (6,6), 可以看到前四个括号里成对出现的因数各不相同，括号中前面的数改变了锁状态，后面的数又变回去了，等于锁的状态没有发生变化，只有最后那个(6,6)，在次数6的时候改变了一次状态，没有对应其它的状态能将其变回去了，所以锁就一直是打开状态的。所以所有平方数都有这么一个相等的因数对，即所有平方数的锁都将会是打开的状态。














