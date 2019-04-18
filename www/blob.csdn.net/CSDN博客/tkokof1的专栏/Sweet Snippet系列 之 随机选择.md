# Sweet Snippet系列 之 随机选择 - tkokof1的专栏 - CSDN博客

2013年12月28日 17:16:54[tkokof1](https://me.csdn.net/tkokof1)阅读数：997


  1. 引子:

  平日工作学习时总会遇到一些令人欣喜的代码段子（Snippet），虽然都很短小，但是其间所含的道理都颇有意味，遂而觉得应该不时的将她们记下，一来算作复习整理，二来也给有兴趣的朋友做些参考，虽然题目说成了一个系列，但自己也不知道能写多少，大概准则估计也就是写到哪算哪了，今天算是第一篇，瞎扯扯随机选择 ：）

  2. 问题:

  题目很简单，如何等概率的选取一个集合中的元素，譬如我们现在有一个vector(C++)：

```cpp
vector<int> v = { 1, 2, 3, 4, 5 };
int selected = random_select(v);
```

  如何实现这个random_select 函数呢？其实有个很简单的方法，便是随机选取一个范围在[0, v.size()) 中的整数即可，代码大抵是这个样子：


```cpp
int random_number(int max) {
    return rand() % max;
}

int random_select(const vector<int>& v) {
    return v[random_number(v.size())];
}
```

  当然，我们还可以继续优化上述代码，譬如将random_select泛化等等，在此就不赘述了，仅从功能性角度来看，上面代码确实完成了我们的期望：“等概率”的随机选取了vector集合中的某个元素。（这里“等概率”之所以加上引号，是因为真实的选取结果其实并不是绝对等概率的，问题在于我们使用了rand()取余来获取随机数，而这种方法所产生的随机数大部分情况下都不是均匀分布的，S.T.L（注意是个人名：））对于此有个非常精彩的[讨论](http://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful)）

  但在现实生活中，很多情况下我们未必一开始就知道集合的长度，譬如我们目前只有begin和end两个前向迭代器：

```cpp
ForwardIterator begin = some_func_get_begin();
ForwardIterator end = some_func_get_end();
ForwardIterator iter = random_select(begin, end);
```

  那么这里的random_select应该如何实现才能保证等概率的选取这个集合中的元素呢？有个方法大概可以算是归约吧，就是首先使用迭代器遍历一遍集合，然后我们便可以知道集合的长度了，然后问题也就归约到之前的随机选取问题了。不管怎么说，这是一个可行的办法，但是明眼人一看就知道这种方法不是很简洁，甚至可以说有些坏味道，但是如果马上让我来实现一下random_select的话，我估摸着可能也会采取这种方法，尽管心里觉得别扭，幸而前些日子在[这里](http://book.douban.com/subject/7055340/)看到了一个更简单的方法，很有意思：）如果用代码实现的话，大概是这个样子：

  使用C++的一个完整示例：

```
#include <iostream>
#include <random>
#include <list>

using namespace std;

namespace {

class Random {
public:
    int Next(int min, int max) {
        uniform_int_distribution<> distribution(min, max);
        return distribution(m_engine);
    }

    int Next(int max) {
        return Next(0, max);
    }

private:
    mt19937 m_engine;
} random;

template<typename ForwardIterator>
ForwardIterator random_select(ForwardIterator begin, ForwardIterator end) {
    int count = 0;
    auto selected = end;

    for (auto iter = begin; iter != end; ++iter) {
        if (random.Next(count++) == 0) {
            selected = iter;
        }
    }

    return selected;
}

}

int main() {
    list<int> l = { 1, 2, 3, 4, 5 };

    int testCount = 32;
    for (int i = 0; i < testCount; ++i) {
        cout << *random_select(std::begin(l), std::end(l)) << " ";
        if ((i + 1) % 10 == 0) {
            cout << "\n";
        }
    }

    return 0;
}
```

  另有一个C#的完整示例，原理是一致的，也顺道贴一下：

```
using System;
using System.Collections.Generic;

namespace RandomSelect {
	
	class Program {
		
		static Random random = new Random();
		
		public static T RandomSelect<T>(IEnumerable<T> enumerable) {
			int count = 0;
			T selected = default(T);
			
			var enumerator = enumerable.GetEnumerator();
			while (enumerator.MoveNext()) {
				if (random.Next(++count) == 0) {
					selected = enumerator.Current;
				}
			}
			
			return selected;
		}
		
		public static void Main(string[] args) {
			var list = new List<int> { 1, 2, 3, 4, 5 };
			int testCount = 32;
			for (int i = 0; i < testCount; ++i) {
				Console.Write(RandomSelect(list));
				Console.Write(" ");
				if ((i + 1) % 10 == 0) {
					Console.WriteLine();
				}
			}
			Console.WriteLine();
			
			Console.Write("Press any key to continue . . .");
			Console.ReadKey(true);
		}
	
	}

}
```

3. 道理:

  不知你看懂了多少代码，其实上面代码砍头去尾之后，真正有趣的大概就是这么一段：


```cpp
for (auto iter = begin; iter != end; ++iter) {
        if (random.Next(count++) == 0) {
            selected = iter;
        }
    }
```

  可就这么几句代码，到底是如何保证等概率选取集合元素的呢？让我们来细究一下：

  不妨假设集合有n的元素，考虑第 i 个元素的最终选取概率 p(i)，不难看出以下关系：

  a. p(i) 与 p(1)、p(2) ... p(i-1) 没有依赖关系，即无论前i - 1元素的单次选取情况如何，都不影响第i个元素的最终选取。

  b. p(i) 依赖于 p(i+1)、p(i+2) ... p(n)，即如果第i个元素被最终选取，那么就意味着i+1、i+2 ... n 等元素都没有被单次选取。

  考虑到元素的单次选取概率都与元素位置有关，第i个元素单次选取概率为 1 / i，自然的，其单次不被选取的概率便是 1 - 1/i = (i - 1)/i

  则有p(i) = 1 / i * i/(i+1) * (i+1)/(i+2) ... (n-1)/n = 1/n

  于是我们有对于任意集合元素，其被最终选取的概率皆为 1/n ！Nice！

  好了，我的废话就这么多了，如果有机会的话继续瞎扯扯，希望吧 ：）



