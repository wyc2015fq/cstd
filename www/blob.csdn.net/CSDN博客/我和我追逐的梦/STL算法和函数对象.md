
# STL算法和函数对象 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月21日 22:10:19[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2220


STL算法和函数对象 \#include<algorithm>
1． find() 在一个迭代器区间内查找一个特定元素，可以对任何类型容器的元素使用此算法。它会返回一个指示所找到元素的迭代器，或者是区间的末尾迭代器，可以查找容器中元素的一个子区间。
vector<int>::iterator it = find(myVector.begin()+1,myVector.end(),num);
2． find_if() 类似与find(),只不过它接受一些谓词函数回调，而不是一个带匹配的元素。
bool perfectScore(int num)
{ return (num>=100);}
vector<int>::iterator it = find(myVector.begin()+1,myVector.end(),perfectScore);
3． accumulate()  需要\#include<numeric>，而不是\#include<algorithm>
vector<double> nums;
…………code………..
double sum = accumulate(nums.begin(),nums,end(),0);
accumulate允许调用者指定要完成的操作
int product(int num1,int num2)
{
return (num1*num2);
}
……….code…………..
double mult = accumulate(nums.begin(),nums.end(),1,product);
………………………….
函数对象 \#include<functional>
可以在一个类中重载函数调用操作符，使得该类的对象可以用于替代函数指针，这些对象成为函数对象。
c++中为5个二元操作符提供了函数对象类模板：plus、minus、multiplies、divides、modulus、
与一元的negate。
plus<int> myPlus;
int res = myPlus(4,5);
如前面调用accumulate():
//double mult = accumulate(nums.begin(),nums.end(),1,product);
double mult = accumulate(nums.begin(),nums.end(),1,multiplies<int>());
C++中还提供了所有标准比较: equal_to、not_equal_to、less、greater、less_equal、greater_equal
优先队列priority_queue中默认的比较是less，可以修改为
//template<typename T,typename Container = vector<T>,typename Compare= less<T>>;
priority_queue<int,vector<int>,greater<int>> myQue;
C++中还为3个逻辑操作提供了函数对象类：logical_not、logical_and、logical_or
函数对象适配器
捆绑器：
因为find_if()函数只提供三个参数，所以第一个选出大于或等于num的数，则需要为其提供四个参数，所以可以用捆绑器。
vector<int>::iterator it= find_if(myVector.begin(),myVector.end(),
bind2nd(greater_equal<int>(),num));
bind2nd把num作为第二个参数捆绑至greater_equal。
把二元函数转换成了一元函数
bind1st把一个实参绑定为二元函数的第一个参数
去翻器：
选出第一个小于num的数
vector<int>::iterator it= find_if(myVector.begin(),myVector.end(),
not1(bind2nd(greater_equal<int>(),num)));
not1表示对一元函数进行操作。
对应的not2表示对二元函数进行操作。
调用成员函数：
如果你有一个包括对象的容器，有时可能想把一个类方法的指针作为回调传递给一个算法。例如：可能想对序列中的每个string对象调用empty()，从而找到包括string的vector中的第一个空string。不过如果只是向find_if()传递string：：empty()的一个指针，算法就没有办法知道它接收的是一个方法的指针，还是一个正常的函数指针或函数对象（调用一个类方法指针与调用一个常规函数指针有所不同，前者必须在一个对象的上下文中调用）。
对于上诉问题的解决：C++提供了一个mem_fun_ref()的转换函数。
vector<string> strings;
vector<string>::const_iterator it = find_if(strings.begin(),strings.end(),
men_fun_ref(&string::empty));
如果容器中是对象的指针而不是对象本身，必须使用适配器mem_fun()来调用成员函数。
vector<string*> strings;
vector<string>::const_iterator it = find_if(strings.begin(),strings.end(),
men_fun (&string::empty));
适配实际函数：
不能对函数适配器bind1st、bind2nd、not1、not2直接使用常规的函数指针，因为这些适配器需要其适配函数对象的特定typedef。因此，C++标准库提供的最后一个函数适配器ptr_fun()允许将常规的函数指针已某种形式包装，以便用于适配器。
编写自己的函数对象：
可以编写自己的函数对象来完成更特定的任务，而不只是预定义函数对象所提供的功能。如果想要对这些函数对象使用函数适配器，必须提供某种typedef。为此，最简单的方法就是从unary_function或binary_function派生你的函数对象类，究竟由哪个类派生取决于一个还是二个参数。这二个类在<functional>中定义，它们针对所提供的“函数”的参数和返回类型进行模块化。
class myIsDigit:public unary_function<char,bool>
{
public:
bool operator() (char c) const
{ return (::isdigit©);}
}
vector<string*> strings;
vector<string>::const_iterator it = find_if(strings.begin(),strings.end(),
not1(myIsDigit()));
工具算法
min()、max()、swap()
非修改算法
1． 搜索算法
find()、find_if()
find_first_of()同时查找多个值中的某一个。
adjacent_find()会查找彼此相等的二个连续元素的第一个实例。
search_n()查找彼此相等的n个连续元素的第一个实例。
search()、find_end()查找与一个指定元素序列匹配的子序列，分别从给定区间的开始和结束处开始查找。
min_element()、max_element()可以找出序列中最小和最大的元素。
2． 数值处理算法
accumulate()
count()、count_if()可以用于计算容器中有给定值的元素的个数。
3． 比较算法
可以采用三种不同的方法来比较整个元素区间：equal()、mismatch()、lexicographical_compare()。
equal()：如果所有位置对应元素相等，则返回true，否则为false。
mismatch()：返回的迭代器会指示各区间中对应元素中不相等的第一个元素。
lexicographical_compare()：如果第一个区间中所有元素都小于第二个区间中相应的元素，或者如果第一个区间比第二个区间短，而且到第一个区间结束之前的所有元素都比第二个区间中相应的元素小，返回true。
如果相比较二个相同类型容器中的元素，可以使用operator==或operator<，而不采用上述函数，上述函数主要用于比较不同类型容器中的元素序列。
4． 运算算法
for_each()：它会对给定区间的每个元素执行一个回调。回调可以按引用取参数，而且可以修改参数。
map<int,int>myMap;
……code…….
for_each(myMap.begin(),myMap.end(),&printPair());
5． 修改算法：
transform():类似于for_each()，区别在于transform()希望回调为每个调用生成一个新元素，它会把生成的元素保存在指定的目标区间中。如果想通过转换，将一个区间中的各个元素代之以调用回调后得到的结果，那么源和目标区间可以是一样的。
向一个vector中的各个元素增加100。
transform(myVector.begin(),myVector.end(),myVector.begin(),bind2nd(plus<int>(),100));
copy():将元素从一个区间复制到另一个区间。源和目标区间必须不同，但他们可以交叠。
copy(vecOne.begin(),vecOne.end(),vecTow().begin());
replace()和replace_if():会把一个区间中分别与某个值或某个谓词匹配的元素替换为一个新值。
replace_if(myVector.begin(),myVector.end(),bind2nd(less<int>(),0),0);
remove()和remove_if()：会从一个区间中删除某些特定的值，或某些符号谓词的值，这些元素不会从底层容器中删除，因为算法只能访问迭代器抽象，而不是访问容器。如果要删除，还要调用erase()。
remove_copy()和remove_cope_if(),它们不会修改源区间，而是把所有为删除的元素拷贝到一个不同的区间中去。
unique()：会删除重复的元素。
reverse()：就地逆置。
reverse_copy()：把逆置后的复制到另一个区间中去，源区间不变。
其他修改算法：iter_swap()，swap_ranges()，fill()，generate()，rotate()，next_permutation()和prev_permutation()。
6． 排序算法
大多数只对vector和deque有用，list有更高效的排序算法，关联容器已经排好序
sort()：使用快速排序。
stable_sort()使用归并排序。
一旦对区间元素排了序，就可以应用binary_search()算法查找元素。
merge()函数允许将二个有序区间合并在一起，并且保持有序。
堆排序：
make_heap()在线性时间内将一个元素区间转换成一个堆，最大元素即为区间的第一个元素。
push_heap()新增一个元素到末尾。
pop_heap()从堆中删除最大元素。
sort_heap()将一个对区间转化成一个完全有序的区间。
其他排序算法：
partition()、partial_sort()、nth_element().
random_shuffle()：随机洗牌算法。
7． 集合算法
include()：检查一个有序区间中的所有元素是否都包含在另一个有序区间中，这些元素可以采用任何顺序出现。
set_union()：并
set_intersection()：交
set_difference(): 差
set_symmetric_difference(); 对称差（异或）


