# C++中STL用法总结 - 心纯净，行致远 - CSDN博客





2018年11月16日 09:42:27[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：52标签：[STL](https://so.csdn.net/so/search/s.do?q=STL&t=blog)
个人分类：[【C++/STL】](https://blog.csdn.net/zhanshen112/article/category/8367378)









### 1.1 什么是STL？

STL（Standard Template Library），即标准模板库，是一个具有工业强度的，高效的C++程序库。它被容纳于C++标准程序库（C++ Standard Library）中，是ANSI/ISO C++标准中最新的也是极具革命性的一部分。该库包含了诸多在计算机科学领域里所常用的基本数据结构和基本算法。为广大C++程序员们提供了一个可扩展的应用框架，高度体现了软件的可复用性。



STL的一个重要特点是数据结构和算法的分离。尽管这是个简单的概念，但这种分离确实使得STL变得非常通用。例如，由于STL的sort()函数是完全通用的，你可以用它来操作几乎任何数据集合，包括链表，容器和数组；



STL另一个重要特性是它不是面向对象的。为了具有足够通用性，STL主要依赖于模板而不是封装，继承和虚函数（多态性）——OOP的三个要素。你在STL中找不到任何明显的类继承关系。这好像是一种倒退，但这正好是使得STL的组件具有广泛通用性的底层特征。另外，由于STL是基于模板，内联函数的使用使得生成的代码短小高效；



从逻辑层次来看，在STL中体现了泛型化程序设计的思想，引入了诸多新的名词，比如像需求（requirements），概念（concept），模型（model），容器（container），算法（algorithmn），迭代子（iterator）等。与OOP（object-oriented programming）中的多态（polymorphism）一样，泛型也是一种软件的复用技术；从实现层次看，整个STL是以一种类型参数化的方式实现的，这种方式基于一个在早先C++标准中没有出现的语言特性--模板（template）。

### 1.2 STL内容介绍

STL中六大组件：

1）容器（Container），是一种数据结构，如list，vector，和deques ，以模板类的方法提供。为了访问容器中的数据，可以使用由容器类输出的迭代器；



2）迭代器（Iterator），提供了访问容器中对象的方法。例如，可以使用一对迭代器指定list或vector中的一定范围的对象。迭代器就如同一个指针。事实上，C++的指针也是一种迭代器。但是，迭代器也可以是那些定义了operator*()以及其他类似于指针的操作符地方法的类对象；



3）算法（Algorithm），是用来操作容器中的数据的模板函数。例如，STL用sort()来对一个vector中的数据进行排序，用find()来搜索一个list中的对象，函数本身与他们操作的数据的结构和类型无关，因此他们可以在从简单数组到高度复杂容器的任何数据结构上使用；



4）仿函数（Function object）



5）迭代适配器（Adaptor）



6）空间配制器（allocator）



下面我将依次介绍STL的这三个主要组件。

### 1.2.1 容器



STL中的容器有队列容器和关联容器，容器适配器（congtainer adapters：stack,queue，priority queue），位集（bit_set），串包(string_package)等等。

（1）序列式容器（Sequence containers），每个元素都有固定位置－－取决于插入时机和地点，和元素值无关，vector、deque、list；

Vectors：将元素置于一个动态数组中加以管理，可以随机存取元素（用索引直接存取），数组尾部添加或移除元素非常快速。但是在中部或头部安插元素比较费时；

Deques：是“double-ended queue”的缩写，可以随机存取元素（用索引直接存取），数组头部和尾部添加或移除元素都非常快速。但是在中部或头部安插元素比较费时；

Lists：双向链表，不提供随机存取（按顺序走到需存取的元素，O(n)），在任何位置上执行插入或删除动作都非常迅速，内部只需调整一下指针；



（2）关联式容器（Associated containers），元素位置取决于特定的排序准则，和插入顺序无关，set、multiset、map、multimap；

Sets/Multisets：内部的元素依据其值自动排序，Set内的相同数值的元素只能出现一次，Multisets内可包含多个数值相同的元素，内部由二叉树实现，便于查找；

Maps/Multimaps：Map的元素是成对的键值/实值，内部的元素依据其值自动排序，Map内的相同数值的元素只能出现一次，Multimaps内可包含多个数值相同的元素，内部由二叉树实现，便于查找；

容器类自动申请和释放内存，无需new和delete操作。vector基于模板实现，需包含头文件vector。  

```cpp
//1.定义和初始化
        vector<int> vec1;    //默认初始化，vec1为空
        vector<int> vec2(vec1);  //使用vec1初始化vec2
        vector<int> vec3(vec1.begin(),vec1.end());//使用vec1初始化vec2
        vector<int> vec4(10);    //10个值为的元素
        vector<int> vec5(10,4);  //10个值为的元素
    //2.常用操作方法
        vec1.push_back(100);            //添加元素
        int size = vec1.size();         //元素个数
        bool isEmpty = vec1.empty();    //判断是否为空
        cout<<vec1[0]<<endl;        //取得第一个元素
        vec1.insert(vec1.end(),5,3);    //从vec1.back位置插入个值为的元素
        vec1.pop_back();              //删除末尾元素
        vec1.erase(vec1.begin(),vec1.end());//删除之间的元素，其他元素前移
        cout<<(vec1==vec2)?true:false;  //判断是否相等==、！=、>=、<=...
        vector<int>::iterator iter = vec1.begin();    //获取迭代器首地址
        vec1.clear();                 //清空元素
    //3.遍历
        //下标法
        int length = vec1.size();
        for(int i=0;i<length;i++)
        {
           cout<<vec1[i];
        }
        cout<<endl<<endl;
        //迭代器法
        vector<int>::const_iterator iterator = vec1.begin();
        for(;iterator != vec1.end();iterator++)
        {
           cout<<*iterator;
        }
```

### 1.2.2.STL迭代器

Iterator（迭代器）模式又称Cursor（游标）模式，用于提供一种方法顺序访问一个聚合对象中各个元素, 而又不需暴露该对象的内部表示。或者这样说可能更容易理解：Iterator模式是运用于聚合对象的一种模式，通过运用该模式，使得我们可以在不知道对象内部表示的情况下，按照一定顺序（由iterator提供的方法）访问聚合对象中的各个元素。

迭代器的作用：能够让迭代器与算法不干扰的相互发展，最后又能无间隙的粘合起来，重载了*，＋＋，＝＝，！＝，＝运算符。用以操作复杂的数据结构，容器提供迭代器，算法使用迭代器；常见的一些迭代器类型：iterator、const_iterator、reverse_iterator和const_reverse_iterator,实例如下：

```cpp
#include <iostream>
    #include <vector>
    using namespace std;
    int main()
    {
        vector<int> v;
        v.push_back(3);  //数组尾部插入3
        v.push_back(2);
        v.push_back(1);
        v.push_back(0);
        cout << " 下标 " << v[3] << endl;
        cout << " 迭代器 " << endl;
        for(vector<int>::iterator i = v.begin();i!= v.end();++i)
        {
            cout << *i << " ";
        }
        cout << endl;
        //在第一个元素之前插入111  insert begin+n是在第n个元素之前插入
        v.insert(v.begin(),111);
        //在最后一个元素之后插入222 insert end + n 是在n个元素之后插入
        v.insert(v.end(),222);
        for(vector<int>::iterator i = v.begin();i!= v.end();++i)
        {
            cout << *i << " ";
        }
        cout << endl;
     
        vector<int> arr(10);
        for(int i = 0; i < 10; i++)
        {
            arr[i] = i;
        }
        for(vector<int>::iterator i = arr.begin();i!= arr.end();++i)
        {
            cout << *i << " ";
        }
        cout << endl;
        //删除 同insert
        arr.erase(arr.begin());
        for(vector<int>::iterator i = arr.begin();i!= arr.end();++i)
         {
            cout << *i << " " ;
         }
        cout << endl ;
        arr.erase(arr.begin(),arr.begin()+5);
        for(vector<int>::iterator i = arr.begin();i!= arr.end();++i)
        {
            cout << *i << " " ;
        }
        cout << endl ;
        return 0 ;
     }
```

运行结果：

# ![](https://img-blog.csdn.net/20170111222806178?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlhb3h1ZXpob25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

实例2：数组转置 (<algorithm> reverse)

     reverse(v.begin(),v.end())

```cpp
#include<iostream>
    #include<vector>
    #include<algorithm>
    using namespace std;
    int main()
    {
        vector<int> v;
        for(int i = 0; i < 10; ++i)
        {
            v.push_back(i);
        }
        for(vector<int>::iterator it = v.begin(); it != v.end(); ++it)
        {
            cout << *it << " ";
        }
        cout << endl;
        reverse(v.begin(),v.end());
        for(vector<int>::iterator it = v.begin(); it != v.end(); ++it)
        {
            cout << *it << " ";
        }
        cout << endl;
        return 0;
    }
```

运行结果：

# ![](https://img-blog.csdn.net/20170111223128757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlhb3h1ZXpob25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 1.2.3、算法



函数库对数据类型的选择对其可重用性起着至关重要的作用。举例来说，一个求方根的函数，在使用浮点数作为其参数类型的情况下的可重用性肯定比使用整型作为它的参数类性要高。而C++通过模板的机制允许推迟对某些类型的选择，直到真正想使用模板或者说对模板进行特化的时候，STL就利用了这一点提供了相当多的有用算法。它是在一个有效的框架中完成这些算法的——你可以将所有的类型划分为少数的几类，然后就可以在模版的参数中使用一种类型替换掉同一种类中的其他类型。



STL提供了大约100个实现算法的模版函数，比如算法for_each将为指定序列中的每一个元素调用指定的函数，stable_sort以你所指定的规则对序列进行稳定性排序等等。只要我们熟悉了STL之后，许多代码可以被大大的化简，只需要通过调用一两个算法模板，就可以完成所需要的功能并大大地提升效率。



算法部分主要由头文件<algorithm>，<numeric>和<functional>组成。

<algorithm>是所有STL头文件中最大的一个（尽管它很好理解），它是由一大堆模版函数组成的，可以认为每个函数在很大程度上都是独立的，其中常用到的功能范围涉及到比较、交换、查找、遍历操作、复制、修改、移除、反转、排序、合并等等。

<numeric>体积很小，只包括几个在序列上面进行简单数学运算的模板函数，包括加法和乘法在序列上的一些操作。

<functional>中则定义了一些模板类，用以声明函数对象。



STL中算法大致分为四类：

1）非可变序列算法：指不直接修改其所操作的容器内容的算法。

2）可变序列算法：指可以修改它们所操作的容器内容的算法。

3）排序算法：对序列进行排序和合并的算法、搜索算法以及有序序列上的集合操作。

4）数值算法：对容器内容进行数值计算。



以下对所有算法进行细致分类并标明功能：
**<一>查找算法(13个)：判断容器中是否包含某个值**

adjacent_find: 在iterator对标识元素范围内，查找一对相邻重复元素，找到则返回指向这对元素的第一个元素的ForwardIterator。否则返回last。重载版本使用输入的二元操作符代替相等的判断。

binary_search: 在有序序列中查找value，找到返回true。重载的版本实用指定的比较函数对象或函数指针来判断相等。

count: 利用等于操作符，把标志范围内的元素与输入值比较，返回相等元素个数。

count_if: 利用输入的操作符，对标志范围内的元素进行操作，返回结果为true的个数。

equal_range: 功能类似equal，返回一对iterator，第一个表示lower_bound，第二个表示upper_bound。

find: 利用底层元素的等于操作符，对指定范围内的元素与输入值进行比较。当匹配时，结束搜索，返回该元素的一个InputIterator。

find_end: 在指定范围内查找"由输入的另外一对iterator标志的第二个序列"的最后一次出现。找到则返回最后一对的第一个ForwardIterator，否则返回输入的"另外一对"的第一个ForwardIterator。重载版本使用用户输入的操作符代替等于操作。

find_first_of: 在指定范围内查找"由输入的另外一对iterator标志的第二个序列"中任意一个元素的第一次出现。重载版本中使用了用户自定义操作符。

find_if: 使用输入的函数代替等于操作符执行find。

lower_bound: 返回一个ForwardIterator，指向在有序序列范围内的可以插入指定值而不破坏容器顺序的第一个位置。重载函数使用自定义比较操作。

upper_bound: 返回一个ForwardIterator，指向在有序序列范围内插入value而不破坏容器顺序的最后一个位置，该位置标志一个大于value的值。重载函数使用自定义比较操作。

search: 给出两个范围，返回一个ForwardIterator，查找成功指向第一个范围内第一次出现子序列(第二个范围)的位置，查找失败指向last1。重载版本使用自定义的比较操作。

search_n: 在指定范围内查找val出现n次的子序列。重载版本使用自定义的比较操作。


**<二>排序和通用算法(14个)：提供元素排序策略**

inplace_merge: 合并两个有序序列，结果序列覆盖两端范围。重载版本使用输入的操作进行排序。

merge: 合并两个有序序列，存放到另一个序列。重载版本使用自定义的比较。

nth_element: 将范围内的序列重新排序，使所有小于第n个元素的元素都出现在它前面，而大于它的都出现在后面。重载版本使用自定义的比较操作。

partial_sort: 对序列做部分排序，被排序元素个数正好可以被放到范围内。重载版本使用自定义的比较操作。

partial_sort_copy: 与partial_sort类似，不过将经过排序的序列复制到另一个容器。

partition: 对指定范围内元素重新排序，使用输入的函数，把结果为true的元素放在结果为false的元素之前。

random_shuffle: 对指定范围内的元素随机调整次序。重载版本输入一个随机数产生操作。

reverse: 将指定范围内元素重新反序排序。

reverse_copy: 与reverse类似，不过将结果写入另一个容器。

rotate: 将指定范围内元素移到容器末尾，由middle指向的元素成为容器第一个元素。

rotate_copy: 与rotate类似，不过将结果写入另一个容器。

sort: 以升序重新排列指定范围内的元素。重载版本使用自定义的比较操作。

stable_sort: 与sort类似，不过保留相等元素之间的顺序关系。

stable_partition: 与partition类似，不过不保证保留容器中的相对顺序。


**<三>删除和替换算法(15个)**

copy: 复制序列

copy_backward: 与copy相同，不过元素是以相反顺序被拷贝。

iter_swap: 交换两个ForwardIterator的值。

remove: 删除指定范围内所有等于指定元素的元素。注意，该函数不是真正删除函数。内置函数不适合使用remove和remove_if函数。

remove_copy: 将所有不匹配元素复制到一个制定容器，返回OutputIterator指向被拷贝的末元素的下一个位置。

remove_if: 删除指定范围内输入操作结果为true的所有元素。

remove_copy_if: 将所有不匹配元素拷贝到一个指定容器。

replace: 将指定范围内所有等于vold的元素都用vnew代替。

replace_copy: 与replace类似，不过将结果写入另一个容器。

replace_if: 将指定范围内所有操作结果为true的元素用新值代替。

replace_copy_if: 与replace_if，不过将结果写入另一个容器。

swap: 交换存储在两个对象中的值。

swap_range: 将指定范围内的元素与另一个序列元素值进行交换。

unique: 清除序列中重复元素，和remove类似，它也不能真正删除元素。重载版本使用自定义比较操作。

unique_copy: 与unique类似，不过把结果输出到另一个容器。


**<四>排列组合算法(2个)：提供计算给定集合按一定顺序的所有可能排列组合**

next_permutation: 取出当前范围内的排列，并重新排序为下一个排列。重载版本使用自定义的比较操作。

prev_permutation: 取出指定范围内的序列并将它重新排序为上一个序列。如果不存在上一个序列则返回false。重载版本使用自定义的比较操作。


**<五>算术算法(4个)**

accumulate: iterator对标识的序列段元素之和，加到一个由val指定的初始值上。重载版本不再做加法，而是传进来的二元操作符被应用到元素上。

partial_sum: 创建一个新序列，其中每个元素值代表指定范围内该位置前所有元素之和。重载版本使用自定义操作代替加法。

inner_product: 对两个序列做内积(对应元素相乘，再求和)并将内积加到一个输入的初始值上。重载版本使用用户定义的操作。

adjacent_difference: 创建一个新序列，新序列中每个新值代表当前元素与上一个元素的差。重载版本用指定二元操作计算相邻元素的差。


**<六>生成和异变算法(6个)**

fill: 将输入值赋给标志范围内的所有元素。

fill_n: 将输入值赋给first到first+n范围内的所有元素。

for_each: 用指定函数依次对指定范围内所有元素进行迭代访问，返回所指定的函数类型。该函数不得修改序列中的元素。

generate: 连续调用输入的函数来填充指定的范围。

generate_n: 与generate函数类似，填充从指定iterator开始的n个元素。

transform: 将输入的操作作用与指定范围内的每个元素，并产生一个新的序列。重载版本将操作作用在一对元素上，另外一个元素来自输入的另外一个序列。结果输出到指定容器。


**<七>关系算法(8个)**

equal: 如果两个序列在标志范围内元素都相等，返回true。重载版本使用输入的操作符代替默认的等于操作符。

includes: 判断第一个指定范围内的所有元素是否都被第二个范围包含，使用底层元素的<操作符，成功返回true。重载版本使用用户输入的函数。

lexicographical_compare: 比较两个序列。重载版本使用用户自定义比较操作。

max: 返回两个元素中较大一个。重载版本使用自定义比较操作。

max_element: 返回一个ForwardIterator，指出序列中最大的元素。重载版本使用自定义比较操作。

min: 返回两个元素中较小一个。重载版本使用自定义比较操作。

min_element: 返回一个ForwardIterator，指出序列中最小的元素。重载版本使用自定义比较操作。

mismatch: 并行比较两个序列，指出第一个不匹配的位置，返回一对iterator，标志第一个不匹配元素位置。如果都匹配，返回每个容器的last。重载版本使用自定义的比较操作。


**<八>集合算法(4个)**

set_union: 构造一个有序序列，包含两个序列中所有的不重复元素。重载版本使用自定义的比较操作。

set_intersection: 构造一个有序序列，其中元素在两个序列中都存在。重载版本使用自定义的比较操作。

set_difference: 构造一个有序序列，该序列仅保留第一个序列中存在的而第二个中不存在的元素。重载版本使用自定义的比较操作。

set_symmetric_difference: 构造一个有序序列，该序列取两个序列的对称差集(并集-交集)。


**<九>堆算法(4个)**

make_heap: 把指定范围内的元素生成一个堆。重载版本使用自定义比较操作。

pop_heap: 并不真正把最大元素从堆中弹出，而是重新排序堆。它把first和last-1交换，然后重新生成一个堆。可使用容器的back来访问被"弹出"的元素或者使用pop_back进行真正的删除。重载版本使用自定义的比较操作。

push_heap: 假设first到last-1是一个有效堆，要被加入到堆的元素存放在位置last-1，重新生成堆。在指向该函数前，必须先把元素插入容器后。重载版本使用指定的比较操作。

sort_heap: 对指定范围内的序列重新排序，它假设该序列是个有序堆。重载版本使用自定义比较操作。

...

参考：

http://www.cnblogs.com/giszhang/archive/2010/02/02/1661844.html

http://blog.csdn.net/conanswp/article/details/23297441

http://www.cnblogs.com/biyeymyhjob/archive/2012/07/22/2603525.html

http://blog.chinaunix.net/uid-24219701-id-2181266.html



