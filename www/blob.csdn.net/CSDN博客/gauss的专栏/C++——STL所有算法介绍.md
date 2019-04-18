# C++——STL所有算法介绍 - gauss的专栏 - CSDN博客
2013年01月08日 13:06:03[gauss](https://me.csdn.net/mathlmx)阅读数：263
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)
                
STL算法部分主要由头文件<algorithm>,<numeric>,<functional>组成。要使用 STL中的算法函数必须包含头文件<algorithm>，对于数值算法须包含<numeric>，<functional>中则定义了一些模板类，用来声明函数对象。
    STL中算法大致分为四类：
        1、非可变序列算法：指不直接修改其所操作的容器内容的算法。
        2、可变序列算法：指可以修改它们所操作的容器内容的算法。
        3、排序算法：包括对序列进行排序和合并的算法、搜索算法以及有序序列上的集合操作。
        4、数值算法：对容器内容进行数值计算。
    以下对所有算法进行细致分类并标明功能：
    <一>查找算法(13个)：判断容器中是否包含某个值
    adjacent_find:            在iterator对标识元素范围内，查找一对相邻重复元素，找到则返回指向这对元素的第一个元素的ForwardIterator。否则返回last。重载版本使用输入的二元操作符代替相等的判断。
    binary_search:            在有序序列中查找value，找到返回true。重载的版本实用指定的比较函数对象或函数指针来判断相等。
    count:                    利用等于操作符，把标志范围内的元素与输入值比较，返回相等元素个数。
    count_if:                 利用输入的操作符，对标志范围内的元素进行操作，返回结果为true的个数。
    equal_range:              功能类似equal，返回一对iterator，第一个表示lower_bound，第二个表示upper_bound。
    find:                     利用底层元素的等于操作符，对指定范围内的元素与输入值进行比较。当匹配时，结束搜索，返回该元素的一个InputIterator。
    find_end:                 在指定范围内查找"由输入的另外一对iterator标志的第二个序列"的最后一次出现。找到则返回最后一对的第一个ForwardIterator，否则返回输入的"另外一对"的第一个ForwardIterator。重载版本使用用户输入的操作符代替等于操作。
    find_first_of:            在指定范围内查找"由输入的另外一对iterator标志的第二个序列"中任意一个元素的第一次出现。重载版本中使用了用户自定义操作符。
    find_if:                  使用输入的函数代替等于操作符执行find。
    lower_bound:              返回一个ForwardIterator，指向在有序序列范围内的可以插入指定值而不破坏容器顺序的第一个位置。重载函数使用自定义比较操作。
    upper_bound:              返回一个ForwardIterator，指向在有序序列范围内插入value而不破坏容器顺序的最后一个位置，该位置标志一个大于value的值。重载函数使用自定义比较操作。
    search:                   给出两个范围，返回一个ForwardIterator，查找成功指向第一个范围内第一次出现子序列(第二个范围)的位置，查找失败指向last1。重载版本使用自定义的比较操作。
    search_n:                 在指定范围内查找val出现n次的子序列。重载版本使用自定义的比较操作。
    <二>排序和通用算法(14个)：提供元素排序策略
    inplace_merge:            合并两个有序序列，结果序列覆盖两端范围。重载版本使用输入的操作进行排序。
    merge:                    合并两个有序序列，存放到另一个序列。重载版本使用自定义的比较。
    nth_element:              将范围内的序列重新排序，使所有小于第n个元素的元素都出现在它前面，而大于它的都出现在后面。重载版本使用自定义的比较操作。
    partial_sort:             对序列做部分排序，被排序元素个数正好可以被放到范围内。重载版本使用自定义的比较操作。
    partial_sort_copy:        与partial_sort类似，不过将经过排序的序列复制到另一个容器。
    partition:                对指定范围内元素重新排序，使用输入的函数，把结果为true的元素放在结果为false的元素之前。
    random_shuffle:           对指定范围内的元素随机调整次序。重载版本输入一个随机数产生操作。
    reverse:                  将指定范围内元素重新反序排序。
    reverse_copy:             与reverse类似，不过将结果写入另一个容器。
    rotate:                   将指定范围内元素移到容器末尾，由middle指向的元素成为容器第一个元素。
    rotate_copy:              与rotate类似，不过将结果写入另一个容器。
    sort:                     以升序重新排列指定范围内的元素。重载版本使用自定义的比较操作。
    stable_sort:              与sort类似，不过保留相等元素之间的顺序关系。
    stable_partition:         与partition类似，不过不保证保留容器中的相对顺序。
    <三>删除和替换算法(15个)
    copy:                     复制序列
    copy_backward:            与copy相同，不过元素是以相反顺序被拷贝。
    iter_swap:                交换两个ForwardIterator的值。
    remove:                   删除指定范围内所有等于指定元素的元素。注意，该函数不是真正删除函数。内置函数不适合使用remove和remove_if函数。
    remove_copy:              将所有不匹配元素复制到一个制定容器，返回OutputIterator指向被拷贝的末元素的下一个位置。
    remove_if:                删除指定范围内输入操作结果为true的所有元素。
    remove_copy_if:           将所有不匹配元素拷贝到一个指定容器。
    replace:                  将指定范围内所有等于vold的元素都用vnew代替。
    replace_copy:             与replace类似，不过将结果写入另一个容器。
    replace_if:               将指定范围内所有操作结果为true的元素用新值代替。
    replace_copy_if:          与replace_if，不过将结果写入另一个容器。
    swap:                     交换存储在两个对象中的值。
    swap_range:               将指定范围内的元素与另一个序列元素值进行交换。
    unique:                   清除序列中重复元素，和remove类似，它也不能真正删除元素。重载版本使用自定义比较操作。
    unique_copy:              与unique类似，不过把结果输出到另一个容器。
    <四>排列组合算法(2个)：提供计算给定集合按一定顺序的所有可能排列组合
    next_permutation:         取出当前范围内的排列，并重新排序为下一个排列。重载版本使用自定义的比较操作。
    prev_permutation:         取出指定范围内的序列并将它重新排序为上一个序列。如果不存在上一个序列则返回false。重载版本使用自定义的比较操作。
    <五>算术算法(4个)
    accumulate:               iterator对标识的序列段元素之和，加到一个由val指定的初始值上。重载版本不再做加法，而是传进来的二元操作符被应用到元素上。
    partial_sum:              创建一个新序列，其中每个元素值代表指定范围内该位置前所有元素之和。重载版本使用自定义操作代替加法。
    inner_product:            对两个序列做内积(对应元素相乘，再求和)并将内积加到一个输入的初始值上。重载版本使用用户定义的操作。
    adjacent_difference:      创建一个新序列，新序列中每个新值代表当前元素与上一个元素的差。重载版本用指定二元操作计算相邻元素的差。
    <六>生成和异变算法(6个)
    fill:                     将输入值赋给标志范围内的所有元素。
    fill_n:                   将输入值赋给first到first+n范围内的所有元素。
    for_each:                 用指定函数依次对指定范围内所有元素进行迭代访问，返回所指定的函数类型。该函数不得修改序列中的元素。
    generate:                 连续调用输入的函数来填充指定的范围。
    generate_n:               与generate函数类似，填充从指定iterator开始的n个元素。
    transform:                将输入的操作作用与指定范围内的每个元素，并产生一个新的序列。重载版本将操作作用在一对元素上，另外一个元素来自输入的另外一个序列。结果输出到指定容器。
    <七>关系算法(8个)
    equal:                    如果两个序列在标志范围内元素都相等，返回true。重载版本使用输入的操作符代替默认的等于操作符。
    includes:                 判断第一个指定范围内的所有元素是否都被第二个范围包含，使用底层元素的<操作符，成功返回true。重载版本使用用户输入的函数。
    lexicographical_compare:  比较两个序列。重载版本使用用户自定义比较操作。
    max:                      返回两个元素中较大一个。重载版本使用自定义比较操作。
    max_element:              返回一个ForwardIterator，指出序列中最大的元素。重载版本使用自定义比较操作。
    min:                      返回两个元素中较小一个。重载版本使用自定义比较操作。
    min_element:              返回一个ForwardIterator，指出序列中最小的元素。重载版本使用自定义比较操作。
    mismatch:                 并行比较两个序列，指出第一个不匹配的位置，返回一对iterator，标志第一个不匹配元素位置。如果都匹配，返回每个容器的last。重载版本使用自定义的比较操作。
    <八>集合算法(4个)
    set_union:                构造一个有序序列，包含两个序列中所有的不重复元素。重载版本使用自定义的比较操作。
    set_intersection:         构造一个有序序列，其中元素在两个序列中都存在。重载版本使用自定义的比较操作。
    set_difference:           构造一个有序序列，该序列仅保留第一个序列中存在的而第二个中不存在的元素。重载版本使用自定义的比较操作。
    set_symmetric_difference: 构造一个有序序列，该序列取两个序列的对称差集(并集-交集)。
    <九>堆算法(4个)
    make_heap:                把指定范围内的元素生成一个堆。重载版本使用自定义比较操作。
    pop_heap:                 并不真正把最大元素从堆中弹出，而是重新排序堆。它把first和last-1交换，然后重新生成一个堆。可使用容器的back来访问被"弹出"的元素或者使用pop_back进行真正的删除。重载版本使用自定义的比较操作。
    push_heap:                假设first到last-1是一个有效堆，要被加入到堆的元素存放在位置last-1，重新生成堆。在指向该函数前，必须先把元素插入容器后。重载版本使用指定的比较操作。
    sort_heap:                对指定范围内的序列重新排序，它假设该序列是个有序堆。重载版本使用自定义比较操作。
转载自：[http://blog.csdn.net/nash635/archive/2010/04/10/5470572.aspx](http://blog.csdn.net/nash635/archive/2010/04/10/5470572.aspx)
补充：
一、函数对象： 因为很多的算法中多使用了函数对象
二元函数对象，V1和V2为输入，V3为结果
plus<T>:
  transform(V1.begin(), V1.end(), V2.begin(), V3.begin(),plus<double>());
其他的二元函数对象：minus,multiples,divieds,modulus.
二元断言函数对象，使用时需要bind2nd（）或bind1st（）来绑定比较对象。
less<T>:
  find_if(L.begin(), L.end(), bind2nd(less<int>(), 0));
其他的二元断言函数：equal_to,notequal_to,greater,greater_equal,less_equal,logical_and,logical_or
二元逻辑函数
binary_negate:
const char* wptr = find_if(str, str + MAXLEN,
                           compose2(not2(logical_or<bool>()),
                                    bind2nd(equal_to<char>(), ' '),
                                    bind2nd(equal_to<char>(), '/n')));
一元函数对象
negate：
transform(V1.begin(), V1.end(), V2.begin(),
          negate<int>());
一元断定函数对象
logical_not:
  transform(V.begin(), V.end(), V.begin(), logical_not<bool>());
一元逻辑函数
unary_negate:
二、函数对象发生器：主要用来填充序列
产生不重复的随机数：
// Generate unique random numbers from 0 to mod:
class URandGen {
  std::set<int> used;
  int limit;
public:
  URandGen(int lim) : limit(lim) {
    srand(time(0));
  }
  int operator()() {
    while(true) {
      int i = int(rand()) % limit;
      if(used.find(i) == used.end()) {
        used.insert(i);
        return i;
      }
    }
  }
};
const int sz = 10;
const int max = 50;
vector<int> x(sz), y(sz), r(sz);
//An integer random number generator:
URandGen urg(max);
generate_n(x.begin(), sz, urg);
三、函数对象适配器 ： 将函数转化为函数对象
ptr_fun:一般函数适配器
一元实例：
transform(first, last, first,
          compose1(negate<double>, ptr_fun(fabs)));
二元实例：
list<char*>::iterator item = 
              find_if(L.begin(), L.end(),
                      not1(binder2nd(ptr_fun(strcmp), "OK")));
not1：对一元的断定函数对象取反的适配器。
not2: 对二元的断定函数对象取反的适配器。
mem_fun与mem_fun_ref：类成员函数的适配器,区别是一个需要指针，而另一个仅需要一般对象。如下：
shape是一个指针变量，则foreach(v.begin(),v.end(),mem_fun(&shape::draw));
但如果shape是一般的变量，不是指针，则foreach（v.begin(),v.end(),mem_fun_ref(&shape::draw)）；
四、算法
拷贝：
copy（）
reverse_copy()
rotate_copy()
remove_copy()  拷贝不等于某值的元素到另一个序列。
remove_copy_if() 拷贝符合条件的到另一个序列。
填充和生成：
fill()
fill_n() 填充序列中的n个元素。
generate（）为序列中的每个元素调用gen（）函数。
排列：
next_permuttion() 后一个排列。
prev_permutation()
partition() 划分，将满足条件的元素移动到序列的前面。
stable_partition()
查找和替换：
find（）
binary_search() 在一个已经有顺序的序列上查找。
find_if()
search() 检查第二个序列是否在第一个序列中出现，且顺序相同。
删除：注意必须调用erase（）来真正删除
remove（）
unique（）删除相邻重复元素，最好现排序。
合并序列：
merge（）
数值算法：
accumulate（） 对序列的每个元素进行运算后求和。
transform（） 也可以对每个元素进行运算。
计数：
size（）总个数。
count（）等于某值的元素个数。
adjacent_difference 序列中的后一个减前与他相邻的前一个得到新的序列。
adiacent_find
五、所有的算法
accumlate ： iterator 对标志的序列中的元素之和，加到一个由 init 指定的初始值上。重载的版本不再做加法，而是传进来的二元操作符被应用到元素上。
adjacent_different ：创建一个新序列，该序列的每个新值都代表了当前元素与上一个元素的差。重载版本用指定的二元操作计算相邻元素的差。 
adjacent_find ：在 iterator 对标志的元素范围内，查找一对相邻的重复元素，如果找到返回一个 ForwardIterator ，指向这对元素的第一个元素。否则返回 last 。重载版本使用输入的二元操作符代替相等的判断。
binary_search ：在有序序列中查找 value ，如果找到返回 true 。重载的版本使用指定的比较函数对象或者函数指针来判断相等。 
copy ：复制序列。 
copy_backward ：除了元素以相反的顺序被拷贝外，别的和 copy 相同。 
count ：利用等于操作符，把标志范围类的元素与输入的值进行比较，并返回相等元素的个数。 
count_if ：对于标志范围类的元素，应用输入的操作符，并返回结果为 true 的次数。 
equal ：如果两个序列在范围内的元素都相等，则 equal 返回 true 。重载版本使用输入的操作符代替了默认的等于操作符。 
equal_range ：返回一对 iterator ，第一个 iterator 表示由 lower_bound 返回的 iterator ，第二个表示由 upper_bound 返回的iterator 值。
fill ：将输入的值的拷贝赋给范围内的每个元素。 
fill_n ：将输入的值赋值给 first 到 frist+n 范围内的元素。 
find ：利用底层元素的等于操作符，对范围内的元素与输入的值进行比较。当匹配时，结束搜索，返回该元素的一个 InputIterator 。
find_if ：使用输入的函数替代了等于操作符执行了 find 。 
find_end ：在范围内查找“由输入的另外一个 iterator 对标志的第二个序列”的最后一次出现。重载版本中使用了用户输入的操作符替代等于操作。 
find_first_of ：在范围内查找“由输入的另外一个 iterator 对标志的第二个序列”中的任意一个元素的第一次出现。重载版本中使用了用户自定义的操作符。
for_each ：依次对范围内的所有元素执行输入的函数。 
generate ：通过对输入的函数 gen 的连续调用来填充指定的范围。 
generate_n ：填充 n 个元素。 
includes ：判断 [first1, last1) 的一个元素是否被包含在另外一个序列中。使用底层元素的 <= 操作符，重载版本使用用户输入的函数。 
inner_product ：对两个序列做内积 ( 对应的元素相乘，再求和 ) ，并将内积加到一个输入的的初始值上。重载版本使用了用户定义的操作。 
inner_merge ：合并两个排过序的连续序列，结果序列覆盖了两端范围，重载版本使用输入的操作进行排序。 
iter_swap ：交换两个 ForwardIterator 的值。 
lexicographical_compare ：比较两个序列。重载版本使用了用户自定义的比较操作。 
lower_bound ：返回一个 iterator ，它指向在范围内的有序序列中可以插入指定值而不破坏容器顺序的第一个位置。重载函数使用了自定义的比较操作。 
max ：返回两个元素中的较大的一个，重载版本使用了自定义的比较操作。 
max_element ：返回一个 iterator ，指出序列中最大的元素。重载版本使用自定义的比较操作。 
min ：两个元素中的较小者。重载版本使用自定义的比较操作。 
min_element ：类似与 max_element ，不过返回最小的元素。 
merge ：合并两个有序序列，并存放到另外一个序列中。重载版本使用自定义的比较。 
mismatch ：并行的比较两个序列，指出第一个不匹配的位置，它返回一对 iterator ，标志第一个不匹配的元素位置。如果都匹配，返回每个容器的 last 。重载版本使用自定义的比较操作。
next_permutation ：取出当前范围内的排列，并将其重新排序为下一个排列。重载版本使用自定义的比较操作。 
nth_element ：将范围内的序列重新排序，使所有小于第 n 个元素的元素都出现在它前面，而大于它的都出现在后面，重载版本使用了自定义的比较操作。 
partial_sort ：对整个序列做部分排序，被排序元素的个数正好可以被放到范围内。重载版本使用自定义的比较操作。 
partial_sort_copy ：与 partial_sort 相同，除了将经过排序的序列复制到另外一个容器。 
partial_sum ：创建一个新的元素序列，其中每个元素的值代表了范围内该位置之前所有元素之和。重载版本使用了自定义操作替代加法。 
partition ：对范围内元素重新排序，使用输入的函数，把计算结果为 true 的元素都放在结果为 false 的元素之前。 
prev_permutation ：取出范围内的序列并将它重新排序为上一个序列。如果不存在上一个序列则返回 false 。重载版本使用自定义的比较操作。 
random_shuffle ：对范围内的元素随机调整次序。重载版本输入一个随机数产生操作。 
remove ：删除在范围内的所有等于指定的元素，注意，该函数并不真正删除元素。内置数组不适合使用 remove 和 remove_if 函数。 
remove_copy ：将所有不匹配的元素都复制到一个指定容器，返回的 OutputIterator 指向被拷贝的末元素的下一个位置。 
remove_if ：删除所有范围内输入操作结果为 true 的元素。 
remove_copy_if ：将所有不匹配的元素拷贝到一个指定容器。 
replace ：将范围内的所有等于 old_value 的元素都用 new_value 替代。 
replace_copy ：与 replace 类似，不过将结果写入另外一个容器。 
replace_if ：将范围内的所有操作结果为 true 的元素用新值替代。 
replace_copy_if ：类似与 replace_if ，不过将结果写入另外一个容器。 
reverse ：将范围内元素重新按反序排列。 
reverse_copy ：类似与 reverse ，不过将结果写入另外一个容器。 
rotate ：将范围内的元素移到容器末尾，由 middle 指向的元素成为容器第一个元素。 
rotate_copy ：类似与 rotate ，不过将结果写入另外一个容器。 
search ：给出了两个范围，返回一个 iterator ，指向在范围内第一次出现子序列的位置。重载版本使用自定义的比较操作。 
search_n ：在范围内查找 value 出现 n 次的子序列。重载版本使用自定义的比较操作。 
set_difference ：构造一个排过序的序列，其中的元素出现在第一个序列中，但是不包含在第二个序列中。重载版本使用自定义的比较操作。 
set_intersection ：构造一个排过序的序列，其中的元素在两个序列中都存在。重载版本使用自定义的比较操作。 
set_symmetric_difference ：构造一个排过序的序列，其中的元素在第一个序列中出现，但是不出现在第二个序列中。重载版本使用自定义的比较操作。 
set_union ：构造一个排过序的序列，它包含两个序列中的所有的不重复元素。重载版本使用自定义的比较操作。 
sort ：以升序重新排列范围内的元素，重载版本使用了自定义的比较操作。 
stable_partition ：与 partition 类似，不过它不保证保留容器中的相对顺序。 
stable_sort ：类似与 sort ，不过保留相等元素之间的顺序关系。 
swap ：交换存储在两个对象中的值。 
swap_range ：将在范围内的元素与另外一个序列的元素值进行交换。 
transform ：将输入的操作作用在范围内的每个元素上，并产生一个新的序列。重载版本将操作作用在一对元素上，另外一个元素来自输入的另外一个序列。结果输出到指定的容器。
unique ：清除序列中重复的元素，和 remove 类似，它也不能真正的删除元素。重载版本使用了自定义的操作。 
unique_copy ：类似与 unique ，不过它把结果输出到另外一个容器。 
upper_bound ：返回一个 iterator ，它指向在范围内的有序序列中插入 value 而不破坏容器顺序的最后一个位置，该位置标志了一个大于 value 的值。重载版本使用了输入的比较操作。
堆算法： C++ 标准库提供的是 max-heap 。一共由以下 4 个泛型堆算法。 
make_heap ：把范围内的元素生成一个堆。重载版本使用自定义的比较操作。 
pop_heap ：并不是真正的把最大元素从堆中弹出，而是重新排序堆。它把 first 和 last-1 交换，然后重新做成一个堆。可以使用容器的 back 来访问被“弹出“的元素或者使用 pop_back 来真正的删除。重载版本使用自定义的比较操作。
push_heap ：假设 first 到 last-1 是一个有效的堆，要被加入堆的元素在位置 last-1 ，重新生成堆。在指向该函数前，必须先把元素插入容器后。重载版本使用指定的比较。
sort_heap ：对范围内的序列重新排序，它假设该序列是个有序的堆。重载版本使用自定义
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/shineHoo/archive/2010/07/15/5736155.aspx](http://blog.csdn.net/shineHoo/archive/2010/07/15/5736155.aspx)
