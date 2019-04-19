# 标准模板库（STL）使用入门（下） - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [zaishaoyi](http://www.jobbole.com/members/zaishaoyi) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[Dmitry Korolev](https://www.topcoder.com/community/data-science/data-science-tutorials/power-up-c-with-the-standard-template-library-part-2/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
在这篇教程中我们将会使用[教程上篇中](http://blog.jobbole.com/87586/)的一些宏和预定义类型。
## 利用 map 创建 vector
正如你所知，map实际上包含的是元素对。因此你可以这样写：

C++
```
map<string, int> M; 
 // ... 
 vector< pair<string, int> > V(all(M)); // remember all(c) stands for 
 (c).begin(),(c).end()
```
现在vector中包含着和map 中相同的元素。当然，和map一样，向量也是有序的。在你既不想改变map中的元素，又想以map所不允许的方式使用元素索引时，这个特性就派上用场了。
## 容器间拷贝数据
让我们看一下 copy(…) 算法。算法的原型如下：

C++
```
copy(from_begin, from_end, to_begin);
```
这个算法从第一个区间向第二个区间拷贝元素。第二个区间中应该有足够的可用空间。请看下面的代码：

C++
```
vector<int> v1;
 vector<int> v2; 
 // ... 
 // Now copy v2 to the end of v1
 v1.resize(v1.size() + v2.size()); 
 // 确保 v1 有足够空间
 copy(all(v2), v1.end() - v2.size()); 
 // Copy v2 elements right after v1 ones
```
译者注：教程[上篇](http://blog.jobbole.com/87586/)中有宏定义：#define all(c) c.begin(), c.end()
copy 还有另一个用于连接的好特性是inserters。由于篇幅限制，不多加赘述。请看下面的代码：

C++
```
vector<int> v; 
 // ... 
 set<int> s; 
 // add some elements to set
 copy(all(v), inserter(s));
```
最后一行代码等价于：

C++
```
tr(v, it) { 
// remember traversing macros from Part I
      s.insert(*it); 
 }
```
既然已经有了标准函数，那么我们还有什么理由要使用自定义的宏（这些宏定义只能够在 GCC 环境下运行）呢？使用诸如 copy 的标准算法是 STL 的一个有效应用，因为可以使别人更容易理解你的代码。
push_back 使用 back_inserter 向 Vector 中插入元素 ，或者使用f ront_inserter 向 deque 容器中插入元素。在某些情况下，需要知道，不只 begin/end 可以作为 copy 的前两个参数，rbegin/ren d也可以。使用 rbegin/rend，将会逆序拷贝元素。
## 归并 list
归并队列是对有序 list 的另一个常见操作。假设你有两个有序 list，分别是 A 和 B。你想将这两个 list 归并成一个新列表。通常会有四种方式：
- ‘union’ the lists, R = A+B
- intersect the lists, R = A*B
- set difference, R = A*(~B) or R = A-B
- set symmetric difference, R = A XOR B
STL为这类任务提供了四种算法：set_union(…)、set_intersection(…)、set_difference(…) 和 set_symmetric_difference(…)。它们的调用方式相同，因此我们以 set_intersection 为例。一个常用原型如下：

C++
```
end_result = set_intersection(begin1, end1, begin2, end2, begin_result);
```
[begin1,end1) 和 [begin2,end2) 是输入的两个list。  ‘begin_result’  是只是输出结果起点的迭代器。但是输出结果list的大小是未知的。所以这个函数返回输出结果终点的迭代器（这决定了在输出结果中有多少个元素）。 关于使用细节，请看下面的例子：

C++
```
int data1[] = { 1, 2, 5, 6, 8, 9, 10 }; 
int data2[] = { 0, 2, 3, 4, 7, 8, 10 }; 
vector<int> v1(data1, data1+sizeof(data1)/sizeof(data1[0]));
vector<int> v2(data2, data2+sizeof(data2)/sizeof(data2[0])); 
vector<int> tmp(max(v1.size(), v2.size()); 
vector<int> res = vector<int> (tmp.begin(), set_intersection(all(v1), all(v2), tmp.begin());
```
最后一行，我们创建了一个新向量 res。它通过区间构造函数创建。区间以 tmp 的起点作为起点，以 set_intersection 算法结果作为结尾。这个算法会取 v1 和 v2 的交集，并将交集写到输出迭代器，从’tmp.begin()’开始写入。set_intersection 算法的返回值是结果数据集终点。
补充说明一点可能会帮助你深入地理解：如果你只是想得到交集中元素的数量，则使用 int cnt = set_intersection(all(v1), all(v2), tmp.begin()) – tmp.begin(); 即可。
实际上，我不会使用“vector<int> tmp”这种结构。我认为每调用一次“set_***”算法都开辟一次内存是不明智的。相反，我会定义一个类型合适并且空间充足的全局或者静态变量。请看下面的代码：

C++
```
set<int> s1, s2; 
for(int i = 0; i < 500; i++) { 
        s1.insert(i*(i+1) % 1000); 
        s2.insert(i*i*i % 1000); 
} 
static int temp[5000]; // greater than we need 
vector<int> res = vi(temp, set_symmetric_difference(all(s1), all(s2), temp)); 
int cnt = set_symmetric_difference(all(s1), all(s2), temp) – temp;
```
‘res’ 中包含两个输入数据集中存在差异的元素。
注意，使用这些算法，输入的数据集必须是有序的。因此，例外一点也需要牢记，由于set是有序的，我们可以使用set（或者不觉得pair麻烦的话，也可以使用map）作为这些算法的参数。
这类算法从一端开始排序，算法复杂度是 O(N1+N2)，N1 和 N2 是输入数据集的大小。
## 算术算法
另外一个有趣的算法是 accumulate(…)。如果我对一个int型的vector调用，并且将第三个参数设为0，accumulate(…) 会返回 vector 中元素之和。

C++
```
vector<int> v; 
// ... 
int sum = accumulate(all(v), 0);
```
accumulate()的返回值类型与第三个参数的类型一致。所以，当你不确定元素之和是否可以采用int型时，直接指定第三个参数的类型就可以了。

C++
```
vector<int> v; 
// ... 
long long sum = accumulate(all(v), (long long)0);
```
Accumulate也可以用来计算乘积。第四个参数标明了计算方法。如果你想获得乘积，则使用如下代码：

C++
```
vector<int> v; 
// ... 
double product = accumulate(all(v), double(1), multiplies<double>()); 
// don’t forget to start with 1 !
```
另外一个有趣的算法是inner_product(…)，它用来计算两个向量的数量积。例如：

C++
```
vector<int> v1; 
vector<int> v2; 
for(int i = 0; i < 3; i++) { 
      v1.push_back(10-i); 
      v2.push_back(i+1); 
} 
int r = inner_product(all(v1), v2.begin(), 0);
```
‘r’是这样的计算得来的：(v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2])，或者说是(10*1+9*2+8*3)，最终计算结果是52.
和“accumulate”算法一样，inner_product的返回值类型是由最后一个参数指定的。最后一个参数是返回结果的初始值。因此inner_product可以用于多维空间的超平面对象，这样调用就可以了：

C++
```
inner_product(all(normal), point.begin(), -shift)
```
现在你应该明白，inner_product只需要对迭代器进行递增，因此queue或者set也可以用作参数。用于计算特殊中间值的卷积滤波器可以这样来实现：

C++
```
set<int> values_ordered_data(all(data));
int n = sz(data); // int n = int(data.size());
vector<int> convolution_kernel(n);
for(int i = 0; i < n; i++) {
     convolution_kernel[i] = (i+1)*(n-i);
}
double result = double(inner_product(all(ordered_data), convolution_kernel.
```
当然，这些代码只是一个例子。老实说，将值拷贝到另一个vector然后排序会更快一些。
这样用也是可以的：

C++
```
vector<int> v; 
// ... 
int r = inner_product(all(v), v.rbegin(), 0);
```
上面的代码将会计算 V[0]*V[N-1] + V[1]+V[N-2] + … + V[N-1]*V[0]，其中N是‘v’中元素的个数。
## Nontrivial Sorting重要的排序
实际上，sort(…)采用了与STL相同的技术。
- 所有的比较都基于运算符‘<’
这意味着你只需重载’operator <‘即可。示例代码：

C++
```
struct fraction { 
      int n, d; // (n/d) 
      // ... 
      bool operator < (const fraction& f) const { 
           if(false) { 
                return (double(n)/d) < (double(f.n)/f.d); 
                // Try to avoid this, you're the TopCoder! 
           } 
           else { 
                return n*f.d < f.n*d; 
           } 
      } 
 }; 
 // ... 
 vector<fraction> v; 
 // ... 
 sort(all(v));
```
为了以防万一，你的对象应该有默认构造函数和拷贝构造函数（或许，还要重载赋值运算符——这条补充说明并非对TopCoders而言的）。
一定要牢记操作符 ‘ <’ 的原型：返回值是bool类型，有const修饰符，参数是const类型的引用。
另一个实现比较的方法是创建比较函数。特定的比较方式作为sort(…)算法的第三个参数传入。例如：按照极角大小对点排序（点的结构是pair<double,double>）

C++
```
typedef pair<double, double> dd; 
const double epsilon = 1e-6;
struct sort_by_polar_angle { 
       dd center; 
       // Constuctor of any type 
       // Just find and store the center 
       template<typename T> sort_by_polar_angle(T b, T e) { 
            int count = 0;
            center = dd(0,0); 
            while(b != e) { 
                        center.first += b->first;
                        center.second += b->second;
                   b++; 
                count++;
            } 
                   double k = count ? (1.0/count) : 0;
            center.first *= k;
                   center.second *= k;
       } 
 // Compare two points, return true if the first one is earlier 
 // than the second one looking by polar angle 
 // Remember, that when writing comparator, you should 
 // override not ‘operator <’ but ‘operator ()’ 
       bool operator () (const dd& a, const dd& b) const { 
            double p1 = atan2(a.second-center.second, a.first-center.first); 
            double p2 = atan2(b.second-center.second, b.first-center.first); 
            return p1 + epsilon < p2; 
       } 
 }; 
 // ... 
 vector<dd> points; 
 // ... 
       sort(all(points), sort_by_polar_angle(all(points)));
```
这段代码非常复杂，但是证明了STL的强大功能。应当指出，在这个例子中，所有的代码在编译的时候都是内置（inline）的，实际上执行起来是很快的。
也要注意对于两个相等的对象，操作符 ‘ <’ 会返回FALSE。这是非常重要的，接下来会解释为什么如此重要。
## 在map和set中使用自定义对象
set 和 map 中的元素是有序的。这是一个总体规则。所以，如果想在set或者map中使用你的对象，那么这些对象应该是可比的。你已经了解了STL中的比较规则：
- 所有的比较都基于运算符 ‘<’
也就是，你应该这样理解：要实现set或者map中元素有序，我只需要实现操作符“<”
假设我们要对point结构体（或者point类）进行操作。我们想让一些线段相交，并且取得这些焦点的集合（有点耳熟？）由于计算机精度有限，当一些点的坐标差别不大时，这些点将会是相同的。我们应该这样编码：

C++
```
const double epsilon = 1e-7; 
struct point { 
       double x, y; 
       // ... 
       // Declare operator < taking precision into account 
       bool operator < (const point& p) const { 
            if(x < p.x - epsilon) return true; 
            if(x > p.x + epsilon) return false; 
            if(y < p.y - epsilon) return true; 
            if(y > p.y + epsilon) return false; 
            return false; 
       } 
 };
```
现在，你可以使用set<point>或者map<point, string>，例如，查找某些点是否已经在交集中存在。更进一步，使用map<point, vector<int> >，获得相交在一点的所有线段索引的列表。
在STL中相等并不意味着相同，这是一个有趣的概念，但是此处我们不予深究。
## Vector中的内存管理
据说vector不会在每次push_back()的时候都重新开辟内存。实际上，调用push_back()的时候，vector开辟了多于当前所需的内存空间。当调用push_back()的时候，vector的大部分STL实现都开辟了双倍空间，并不需要每次都开辟分配内存。这或许在实际运用中并不好，因为你的程序占用了双倍的内存空间。有两种简易方法和一种复杂方法来处理这个问题。
第一种方法是使用vector的成员函数reserve()。这个函数使vector开辟多余的内存。在未达到reserve()指定的大小之前，vector不会再次调用push_back()时开辟内存。
考虑一下接下来的例子。你有一个1000个元素的向量，它开辟了1024大小的空间。你打算向vector中追加50个元素。如果你调用50次push_back()，vector开辟的内存控件将会是2048.但是如果在调用push_back()之前加上这句代码：

C++
```
v.reserve(1050);
```
Vector开辟的内存空间恰好容纳1050个元素.
如果你经常使用push_back，那么reserve()会使你受益匪浅。
顺便说一句，对于vector来说，在copy(…, back_inserter(v)) 之后使用v.reserve()是一种很好的模式。
另外一种情况：你希望某些操作之后，vector占用的内存不会增加。该如何摆脱潜在的内存追加呢？解决方案如下：

C++
```
vector<int> v; 
// ... 
vector<int>(all(v)).swap(v);
```
这段代码的含义是：创建一个与 ‘v’ 内容相同的临时向量，然后这个临时向量与 ‘v’ 互换。互换之后v中的多余内存将会相会小时。在SRMs中这个方案很少用到。
恰当但复杂的解决方案是为vector开发自定义的分配符，但这很明显不是本教程该讨论的内容。
## 用STL实现真正的算法
带着STL知识，我们继续这篇文章中最有意思的部分：如何实现真正高效的算法？
### 深度优先检索（DFS）
这里不再赘述DFS的原理——可以阅读 [gladius](http://www.topcoder.com/tc?module=MemberProfile&cr=268851&tab=alg) 所著《[Introduction to Graphs and Data Structures](http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=graphsDataStrucs2)》教程中的 [这一章](https://www.topcoder.com/community/data-science/data-science-tutorials/introduction-to-graphs-and-their-data-structures-section-2/#depth)——但是我将会展示STL如何有助于实现DFS。
首先，假设有一个无向图。在STL中，存储这个无向图最简单的方法是保存每个节点的相邻节点。最终生成结构体vector< vector<int> > W ，其中W[i] 是到节点 i 的相邻节点列表。接下来证明一下我们是按照DFS存储的。

C++
```
/*
Reminder from Part 1:
typedef vector<int> vi;
typedef vector<vi> vvi;
*/
 int N; // number of vertices 
 vvi W; // graph 
 vi V; // V is a visited flag 
 void dfs(int i) { 
       if(!V[i]) { 
            V[i] = true; 
            for_each(all(W[i]), dfs); 
       } 
 } 
 bool check_graph_connected_dfs() { 
       int start_vertex = 0; 
       V = vi(N, false); 
       dfs(start_vertex); 
       return (find(all(V), 0) == V.end()); 
 }
```
这样就证明完了。STL算法”for_each”为V中的每一个元素调用指定的函数”dfs”。在check_graph_connected()函数中我们首先创建一个访问标记数组（数组大小合适并且以0填充）。DFS调用完成之后，通过检查V中是否有值为0的元素——只需调用一下find()函数就可以实现——就可以确认我们是否访问到了所有结点，。
注意一下for_each：这个函数的最后一个参数，几乎可以是任何具有函数功能的值。不仅可以是全局函数，还可以是函数配接器，标准算法甚至是成员函数。如果是成员函数的话，则需要成员函数或者是成员函数引用的配接器，在此我们不讨论这个问题。
注：不建议使用vector<bool>。尽管在这个特定案例中这样使用没有问题，但最好还是避免这种做法。使用预定义的 ‘vi’  （vector<int>）。将“true”或者“false”作为int类型赋值给vi是没有问题的。尽管这样需要的内存是使用bool型的 8*sizeof(int)=8*4=32 倍，但是可以适应大多数情况并且在TopCoder上运行很快。
### 关于其他类型容器及其使用方法的简要介绍
Vector由于是最简单的数组容器，因此非常受欢迎。在大多数情况下，你只用到vector的数组功能。但是，有时你可能需要一个更高级的容器。
在 SRM（Single Round Match) 热期间，研究某个STL容器的全部功能并不是一个好的做法。如果不清楚需要使用什么容器，那么你最好使用vector、map或者set。例如，stack可以通过vector实现，并且如果你忘记了stack容器的符号，这种方式可以运行的更快一些。
STL提供了以下容器：list、stack、queue、deque、priority_queue。我发现在SRM中，list和deque很少用到（除了在某些特殊的基于这些容器的任务中会用到）。但是，queue和priority_queue仍然有必要介绍一下。
### Queue
Queue是一种具有三类操作的数据类型，所有操作的平均时间复杂度都是O(1)：在头部追加一个元素，在尾部移除一个元素，获取第一个无法访问的元素（“tail”）。换言之，queue是一个先进先出（FIFO）的缓冲区。
### 广度优先检索（BFS）
再次说明，如果你不熟悉BFS算法，请首先参照一下这篇Topcoder教程（[链接](https://www.topcoder.com/community/data-science/data-science-tutorials/introduction-to-graphs-and-their-data-structures-section-2/#breadth)）。在广度优先算法（BFS）中使用queue是非常便捷的，如下所示：

C++
```
/*
Graph is considered to be stored as adjacent vertices list.
Also we considered graph undirected.
vvi is vector< vector<int> >
W[v] is the list of vertices adjacent to v
*/
 int N; // number of vertices
 vvi W; // lists of adjacent vertices
 bool check_graph_connected_bfs() { 
      int start_vertex = 0; 
      vi V(N, false); 
      queue<int> Q; 
      Q.push(start_vertex); 
      V[start_vertex] = true; 
      while(!Q.empty()) { 
           int i = Q.front(); 
           // get the tail element from queue
           Q.pop(); 
           tr(W[i], it) { 
                if(!V[*it]) { 
                     V[*it] = true; 
                     Q.push(*it); 
                } 
           } 
      } 
      return (find(all(V), 0) == V.end()); 
 }  
注：#define tr(c,i) for(typeof((c).begin() i = (c).begin(); i != (c).end(); i++)
```
更确切地说，queue 支持 front()、back()、 push()（==push_back()）和 pop()（ ==pop_front()）操作。如果你会用到push_front()和pop_back()，就使用dequeue。Dequeue提供时间复杂度为O(1)的所有算法。
queue和map有一个有趣的应用，用于在一幅复杂的图中，通过BFS算法实现最短路径的检索。假设我们有一幅图，图中的节点代表着某些复杂的东西。如：

C++
```
pair< pair<int,int>, pair< string, vector< pair<int, int> > > > 
 (this case is quite usual: complex data structure may define the position in 
 some game, Rubik’s cube situation, etc…)
```
假设已知我们要查找的路径很短，并且路径上的位置节点很少。如果图中所有边的长度都为1，那么我们可以使用BFS在这幅图中检索最短路径。一段伪代码如下：

C++
```
// Some very hard data structure 
typedef pair< pair<int,int>, pair< string, vector< pair<int, int> > > > POS; 
// ... 
int find_shortest_path_length(POS start, POS finish) { 
     map<POS, int> D; 
     // shortest path length to this position 
     queue<POS> Q; 
     D[start] = 0; // start from here 
     Q.push(start); 
     while(!Q.empty()) { 
          POS current = Q.front(); 
          // Peek the front element 
          Q.pop(); // remove it from queue 
          int current_length = D[current];
          if(current == finish) { 
               return D[current]; 
               // shortest path is found, return its length 
          } 
          tr(all possible paths from 'current', it) { 
               if(!D.count(*it)) { 
               // same as if(D.find(*it) == D.end), see Part I
                    // This location was not visited yet 
                    D[*it] = current_length + 1; 
               } 
          } 
     } 
     // Path was not found 
     return -1; 
} 
// ...
```
然而，如果图中边长不相等，那么BFS算法就无效了。这时我们应该使用Dijkstra算法代替。通过 priority_queue可以实现这样一个Dijkstra算法，请继续看后面的内容。
### Priority_Queue
Priority_Queue是一个二进制堆。它是一个可以执行以下操作的数据结构：
- 压入任意元素
- 显示头部元素
- 弹出头部元素
STL中priority_queue的应用请看SRM307中[TrainRobber](http://community.topcoder.com/stat?c=problem_statement&pm=6448&rd=9987)问题。
### Dijkstra
在本文的最后一节，介绍一下如何利用STL容器实现稀疏图中的Dijkstra算法。请读[这篇教程](https://www.topcoder.com/community/data-science/data-science-tutorials/geometry-concepts-using-geometry-in-topcoder-problems/#dijkstra)了解一下Dijkstra算法。
假设我们有一幅带比重的有向图，这幅有向图是以vector<vector<pair<int,int>>>G 存储的，在G中
- G.size() 代表有向图中的节点数量
- G[i].size() 是从索引为i的节点直接可达的节点数量
- G[i][j].first 是从索引为i的节点直接可达的第j个节点的索引
- G[i][j].second 是连接索引为i的节点和索引为 G[i][j].first 节点的边长
我们假设在如下两个代码段中这样定义：

C++
```
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef vector<vii> vvii;
```
### 通过 priority_queue 实现 Dijstra 算法
非常感谢 [misof](https://www.topcoder.com/member-profile/misof/) 抽出时间给我解释为什么这个算法的时间复杂度很好，尽管没有从queue中移除独立的元素。

C++
```
vi D(N, 987654321); 
      // distance from start vertex to each vertex
      priority_queue<ii,vector<ii>, greater<ii> > Q; 
      // priority_queue with reverse comparison operator, 
      // so top() will return the least distance
      // initialize the start vertex, suppose it’s zero
      D[0] = 0;
      Q.push(ii(0,0));
      // iterate while queue is not empty
      while(!Q.empty()) {
            // fetch the nearest element
            ii top = Q.top();
            Q.pop();
            // v is vertex index, d is the distance
            int v = top.second, d = top.first;
            // this check is very important
            // we analyze each vertex only once
            // the other occurrences of it on queue (added earlier) 
            // will have greater distance
            if(d <= D[v]) {
                  // iterate through all outcoming edges from v
                  tr(G[v], it) {
                        int v2 = it->first, cost = it->second;
                        if(D[v2] > D[v] + cost) {
                              // update distance if possible
                              D[v2] = D[v] + cost;
                              // add the vertex to queue
                              Q.push(ii(D[v2], v2));
                        }
                  }
            }
      }
```
本文中我不想点评算法本身，但是你应该注意到priority_queue对象的定义。一般而言，priority_queue<ii>是可以用的，但是成员函数top()将会返回队列中最大的元素，而不是最小的。我常用的简单解决方案之一是在pair的第一个元素不存储偏移量而是存储偏移量的负值。如果你想以合适的方法实现队列的反转，你需要实现priority_queue的反转。priority_queue的第二个模板参数是容器的存储类型，第三个模板参数则是比较函数的指针。
### 通过 set 实现 Dijkstra
在向[Petr](https://www.topcoder.com/member-profile/Petr/)请教C#中Dijkstra的有效实现的时候，他给我讲述了这个方法。在Dijkstra算法的实现中，我们使用priority_queue向“已经分析过的结点”队列中追加元素，平均时间复杂度和最差时间复杂度都是O(log N)。但是，除了priority_queue还有一个容器为我们提供这个功能——就是set。经过大量的实践，我得出：基于priority_queue和基于set的Dijkstra算法是一样的效果。
基于set的代码如下：

C++
```
vi D(N, 987654321);
      // start vertex
      set<ii> Q;
      D[0] = 0;
      Q.insert(ii(0,0));
      while(!Q.empty()) {
           // again, fetch the closest to start element 
           // from “queue” organized via set
           ii top = *Q.begin();
           Q.erase(Q.begin());
           int v = top.second, d = top.first;
           // here we do not need to check whether the distance 
           // is perfect, because new vertices will always
           // add up in proper way in this implementation
           tr(G[v], it) {
                int v2 = it->first, cost = it->second;
                if(D[v2] > D[v] + cost) {
                     // this operation can not be done with priority_queue, 
                     // because it does not support DECREASE_KEY
                     if(D[v2] != 987654321) {
                           Q.erase(Q.find(ii(D[v2],v2)));
                     }
                     D[v2] = D[v] + cost;
                     Q.insert(ii(D[v2], v2));
                }
           }
      }
```
另外重要的一点：STL中的priority_queue不支持DECREASE_KEY 操作，如果你需要这个操作，你最好使用于set。
我曾经花费了大量的时间来弄明白为什么从queue（还有set）中移除元素和移除第一个元素一样快。
这两个实现有着同样的复杂度并且花费一样的时间。而且，我进行了实验，两种实现方式的效果几乎相同（时间差大约是%0.1）
对我而言，我更倾向于利用set实现Dijkstra算法，因为从逻辑上更容易理解，并且不需要记住greater<int>预示着重写。
## STL 之外的一些东西
读到这里，我希望你已经明白了STL是一个非常强大的工具，尤其对TopCoder SRMs来说。但是，在你使用STL之前，请记住哪些没有包含在STL中。
首先，STL没有BigInteger。如果SRM中的一个任务需要大量的运算，尤其是乘除运算，你有三种选择：
- 使用预先写好的模板
- 使用JAVA，如果你很熟练的话
- 说“啊，这真的不是我能解决的SRM任务”
我建议第一个选项。
在几何库中有一个几乎相同的问题。STL不支持几何学，所以你再次面临着上面的三个选项。
最后一件事情——有时很烦人的事情——是STL没有内部的字符串分割函数。如果这个ExampleBuilder插件的默认C++模板中包含这个函数，就更麻烦了。但是，我发现在一般的案例中使用istringstream(s)，在复杂的案例中使用sscanf(s.c_str(), …)就足够了。
通过这些说明，希望你能够认识到这篇文章的价值，也希望你能发现STL是C++中一个非常有用的附加项。祝你在竞赛中好运。
作者注：在本教程的两部分中，我都建议使用模板来减少实现某些功能的时间。这个建议一直适用于程序员。暂且不谈在SRM中使用模板是不是一个好的策略，在日常生活中，模板对于想理解代码的人来说是一件烦人的事情。我有时会依赖于模板，最终我决定不再使用。我鼓励你权衡使用模板类的利弊，然后做出自己的决定。
