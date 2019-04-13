
# 在GCC和Visual Studio中使用hash_map - 高科的专栏 - CSDN博客

2016年02月18日 11:15:48[高科](https://me.csdn.net/pbymw8iwm)阅读数：1577


熟悉STL或熟悉ACM/ICPC的话，其中的set, map, multiset, multimap一定用过无数次了，它们都是用平衡二叉树（红黑树）实现的，复杂度为O(lgn)。我们也知道set, map可以通过哈希来实现，复杂度只有O(1)，可惜直到现在，unsorted_set或hash_map都没能成为C++标准的一部分（C++0x，- -b）。不过无论在GNU GCC中还是Microsoft Visual Studio中都有对hash_set, hash_map, hash_multiset, hash_multimap的支持。
GCC中的hash_map定义在<ext/hash_map>文件，namespace__gnu_cxx中。要定义一个hash_map<int, int>非常简单：
|\#include
 <ext/hash_map>|
|using|namespace|__gnu_cxx;|
|hash_map<|int|,|int|>
 hm;|

在使用map时，如果我们想要改变元素顺序，或以自定义的struct/class作为key的时候，可以设定map第三个模板参数（默认是less<Key>，即operator<）。对于hash_map，我们需要设定其第三个(hash<Key>)和第四个模板参数(equal_to<Key>, operator==)。
|typedef|long|long|my_type;|
|typedef|int|any_type;|
|struct|my_hash
 {|
|size_t|operator()(|const|my_type&
 key)|const|{|
|return|(key
 >> 32) ^ key;|
|}|
|};|
|struct|my_equal_to
 {|
|bool|operator()(|const|my_type&
 lhs,|const|my_type&
 rhs)|const|{|
|return|lhs
 == rhs;|
|}|
|};|
|hash_map<my_type,
 any_type, my_hash, my_equal_to> my_hash_map;|

对与int等基本类型，系统提供有hash<int>等版本的模板特化，所以只需要指定前两个模板参数就足够了。实现了模板特化的有以下类型
|[|const|]|char|*,
 crope, wrope, [|signed||unsigned]|char|,
 [unsigned]|short|,
 [unsigned]|int|,
 [unsigned]|long|

如果需要的话，我们也可以为其他类型实现模板特化
|1|//
 hash_map<Key, Tp, HashFn=hash<Key>, EqualKey=equal_to<Key>, Alloc=allocator<Tp> >|
|2|\#include
 <cstdio>|
|3|\#include
 <utility>|
|4|\#include
 <hash_map>|
|5|using|namespace|std;|
|6|using|namespace|__gnu_cxx;|
|7|
|8|namespace|__gnu_cxx
 {|
|9|template|<>|
|10|struct|hash<pair<|int|,|int|>
 > {|
|11|size_t|operator()(|const|pair<|int|,|int|>&
 key)|const|{|
|12|return|key.first
 * key.second;|
|13|}|
|14|};|
|15|}|
|16|hash_map<pair<|int|,|int|>,|int|>
 hm;|


Visual C++的hash_map定义在<hash_map>文件，namespacestdext中，早先在namespace std中。其实现与GCC的不同，模板参数也不一样，比如上面的例子在VC++版本如下
|1|//
 hash_map<Key, Type, Traits=hash_compare<Key, less<Key> >, Allocator=allocator<pair<const Key, Type> > >|
|2|>|
|3|class|hash_map|
|4|\#include
 <cstdio>|
|5|\#include
 <utility>|
|6|\#include
 <hash_map>|
|7|using|namespace|std;|
|8|using|namespace|stdext;|
|9|
|10|template|<>|
|11|struct|hash_compare<pair<|int|,|int|>
 > {|
|12|//
 the mean number of elements per bucket|
|13|static|const|size_t|bucket_size
 = 4;|
|14|//
 the minimum number of buckets|
|15|static|const|size_t|min_buckets
 = 8;|
|16|//
 hash function|
|17|size_t|operator()(|const|pair<|int|,|int|>&
 key)|const|{|
|18|return|key.first
 * key.second;|
|19|}|
|20|//
 compare function|
|21|bool|operator()(|const|pair<|int|,|int|>&
 lhs,|const|pair<|int|,|int|>&
 rhs)|const|{|
|22|return|lhs
 < rhs;|
|23|}|
|24|};|
|25|hash_map<pair<|int|,|int|>,|int|>
 hm;|

相比前面的hash，上面的hash_compare显然要复杂不少。
不过二者提供的方法基本一致，也和std::map和其他STL容器相似。所以对于上面定义的hash_map，我们都可以用下面的代码进行测试
|1|...|
|2|int|main()
 {|
|3|int|n;|
|4|scanf|(|"%d"|,
 &n);|
|5|for|(|int|i
 = 0; i < n; ++i) {|
|6|hm[make_pair(i,
 i)] = i;|
|7|}|
|8|for|(hash_map<pair<|int|,|int|>,|int|>::iterator
 i = hm.begin(); i != hm.end(); ++i) {|
|9|printf|(|"%d
 "|,
 i->second);|
|10|}|
|11|printf|(|"\n%d
 / %d\n"|,
 hm.size(), hm.bucket_count());|
|12|return|0;|
|13|}|

n取12时，GCC 4.4.1得到的结果是
|0
 1 2 3 4 5 6 7 8 9 10 11|
|12
 / 193|

而Visual Studio 2010得到的结果是
|0
 4 8 1 3 5 7 9 11 2 6 10|
|12
 / 8|

由此我们可以看出二者在hash_map实现上的不同。__gnu_cxx::hash_map保持size<=bucket_count，而以193, 389, 769, 1543…这样近似成倍增长的质数作为bucket_count。stdext::hash_map则保持size<=bucket_size*bucket_count，bucket_count起初为min_buckets=8，不足时以8倍增长。
之所以我们要选择hash_map，是为了获得更高的效率。hash_map比map到底快多少呢，我们通过下面的程序来测试一下。通过__GNUC__和_MSC_VER这两个宏，我们可以把两个版本的代码写到一起
|1|\#include
 <map>|
|2|\#include
 <ctime>|
|3|\#include
 <cstdio>|
|4|using|namespace|std;|
|5|
|6|\#if
 defined(_MSC_VER)|
|7|\#
 include <hash_map>|
|8|using|stdext::hash_map;|
|9|\#elif
 defined(__GNUC__)|
|10|\#
 include <ext/hash_map>|
|11|using|__gnu_cxx::hash_map;|
|12|\#else|
|13|\#
 error 悲剧啊|
|14|\#endif|
|15|
|16|int|main()
 {|
|17|clock_t|S;|
|18|
|19|S
 =|clock|();|
|20|for|(|int|i
 = 0; i < 20; ++i) {|
|21|hash_map<|int|,|int|>
 H;|
|22|for|(|int|i
 = 0; i < (1 << 20); ++i) {|
|23|H[i]
 = i;|
|24|}|
|25|}|
|26|printf|(|"hash_map:
 %.2lfs\n"|,
 (|double|)(|clock|()
 - S) / CLOCKS_PER_SEC);|
|27|
|28|S
 =|clock|();|
|29|for|(|int|i
 = 0; i < 20; ++i) {|
|30|map<|int|,|int|>
 M;|
|31|for|(|int|i
 = 0; i < (1 << 20); ++i) {|
|32|M[i]
 = i;|
|33|}|
|34|}|
|35|printf|(|"map:
 %.2lfs\n"|,
 (|double|)(|clock|()
 - S) / CLOCKS_PER_SEC);|
|36|
|37|return|0;|
|38|}|

结果得到的是（gcc和VS的结果分别来自不同机器，因此没有可比性）
|*
 g++ -O2|
|hash_map:
 5.39s|
|map:
 12.35s|
|*
 VS2010 Release|
|hash_map:
 9.53s|
|map:
 9.44s|

发现g++里hash_map确实要比map快~~不少~~，而Visual Studio 2010就是个悲剧，信hash_map不如信春哥啊。
嘛，hash_map确实可能带来一些performance，但不那么stable，所以我们可以考虑优先使用hash_map，而将map最为fallback~~备胎~~。
|1|\#define
 USE_HASH_MAP ?|
|2|\#if
 USE_HASH_MAP|
|3|\#include
 <ext/hash_map>|
|4|typedef|__gnu_cxx::hash_map<|int|,|int|>
 Hash;|
|5|\#else|
|6|\#include
 <map>|
|7|typedef|std::map<|int|,|int|>
 Hash;|
|8|\#endif|


不过在浙大校赛这种judge是优秀的gcc，而比赛环境是混乱的VC6的比赛里。hash_map什么的还是能不用就不用吧……
This entry was posted on Thursday, March 25th, 2010 at 9:34 pm and is filed under[summary](http://blog.watashi.ws/category/gao/summary/).
 You can follow any responses to this entry through the[RSS 2.0](http://blog.watashi.ws/618/gcc-visual-studio-hash-map/feed/)feed. You can[leave
 a response](http://blog.watashi.ws/618/gcc-visual-studio-hash-map/#respond), or[trackback](http://blog.watashi.ws/618/gcc-visual-studio-hash-map/trackback/)from your own site.

