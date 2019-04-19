# python 堆排序，双端队列，counter_用户5706034127_新浪博客
||分类：[算法](http://blog.sina.com.cn/s/articlelist_5706034127_5_1.html)|
- 
deffixUp(a): #在堆尾加入新元素，fixUp恢复堆的条件
- k=len(a)-1
- whilek>1anda[k//2]
- a[k//2],a[k]=a[k],a[k//2] 
- k=k//2
- 
- 
deffixDown(a): #取a[1]返回的值，然后把a[N]移到a[1]，fixDown来恢复堆的条件
- k=1
- N=len(a)-1
- while2*k<=N: 
- j=2*k 
- ifjanda[j]1]: 
- j+=1
- ifa[k]
- a[k],a[j]=a[j],a[k] 
- k=j 
- else: 
- break
- 
- 
definsert(a,elem): 
- a.append(elem) 
- fixUp(a) 
- 
- 
defdelMax(a): 
- maxElem=a[1] 
- N=len(a) 
- ifN<=1: 
- print('There\'s none element in the list') 
- return-1
- ifN==2: 
- returna[1] 
- else: 
- a[1]=a.pop() 
- fixDown(a) 
- returnmaxElem 
- 
- 
data=[-1,] #第一个元素不用，占位
- 
insert(data,26) 
- 
insert(data,5) 
- 
insert(data,77)
 
//堆排序分为fixup，fixdown。插入的时候，新节点插到最后需要一次fixup
//删除的时候，取出顶端元素，末端插入顶端，一次fixdown
//初始化的时候，从(n-1)/2到0一次进行fixdown，从堆的底层使之有序即可，相当于每次在堆顶操作
import heapq
class Item:
 def __init__(self,
name):
 self.name = name
 def
__repr__(self):
 return 'Item({!r})'.format(self.name)
class PriorityQueue:
 '优先级队列'
 def
__init__(self):
 self._queue = []
 self._index = 0
 def push(self, item,
priority):
 heapq.heappush(self._queue, (-priority,
self._index, item))
 self._index += 1
 def pop(self):
 return heapq.heappop(self._queue)[-1]
q = PriorityQueue()
q.push(Item('foo'), 1)
q.push(Item('bar'), 5)
q.push(Item('spam'), 4)
q.push(Item('grok'), 1)
print q.pop() # Item('bar')
print q.pop() # Item('spam')
print q.pop() # Item('foo')
print q.pop() # Item('grok')
from collections import *
'双端队列'
q = deque(range(5))
q.append(5)
q.appendleft(6)
print q
print q.pop()
print q.popleft()
print q.rotate(3) # rotate是队列的旋转操作，Right
rotate(正参数)是将右端的元素移动到左端，而Left rotate(负参数)则相反。
print q
print q.rotate(-1)
print q
'统计单词个数 1'
import re
string = """   Lorem ipsum dolor sit amet,
consectetur
 adipiscing elit. Nunc ut
elit id mi ultricies
 adipiscing. Nulla
facilisi. Praesent pulvinar,
 sapien vel feugiat
vestibulum, nulla dui pretium orci,
 non ultricies elit lacus
quis ante. Lorem ipsum dolor
 sit amet, consectetur
adipiscing elit. Aliquam
 pretium ullamcorper urna
quis iaculis. Etiam ac massa
 sed turpis tempor
luctus. Curabitur sed nibh eu elit
 mollis congue. Praesent
ipsum diam, consectetur vitae
 ornare a, aliquam a
nunc. In id magna pellentesque
 tellus posuere
adipiscing. Sed non mi metus, at lacinia
 augue. Sed magna nisi,
ornare in mollis in, mollis
 sed nunc. Etiam at justo
in leo congue mollis.
 Nullam in neque eget
metus hendrerit scelerisque
 eu non enim. Ut
malesuada lacus eu nulla bibendum
 id euismod urna sodales.
 """
words = re.findall(r'\w+', string) #This finds words in the
document
lower_words = [word.lower() for word in words] #lower all the
words
word_counts = Counter(lower_words) #counts the number each
time a word appears
print word_counts
'统计单词个数 2'
'统计单词位置 3'
s = "the quick brown fox jumps over the lazy dog"
words = s.split()
location = defaultdict(set)
for m, n in enumerate(words):
 location[n].add(m)
print location
'Counter'
li = ["Dog", "Cat", "Mouse","Dog","Cat", "Dog"]
a = Counter(li)
print a # Counter({'Dog': 3, 'Cat': 2, 'Mouse': 1})
print "{0} : {1}".format(a.values(),a.keys())
 # [1, 3, 2] : ['Mouse', 'Dog', 'Cat']
print(a.most_common(2)) # [('Dog', 3), ('Cat', 2)]
'树结构'
def tree(): return defaultdict(tree)
class Example(dict):
 def __getitem__(self,
item):
 try:
 return
dict.__getitem__(self, item)
 except KeyError:
 value =
self[item] = type(self)()
 return
value
a = Example()
a[1][2][3] = 4
a[1][3][3] = 5
a[1][2]['test'] = 6
'汉诺塔'
def Hanoi(n,A,B,C):
 if n==1:
 move(A,C)
 else:
 Hanoi(n-1,A,C,B)
 move(A,C)
 Hanoi(n-1,B,A,C)  
'斐波拉契数列'  
def fib_list(n):
 if n==1 or n==2:
 return 1
 else:
 return fib_list(n-1)+fib_list(n-2)
