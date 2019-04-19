# 一些瞠目结舌的 C/C++ 代码技巧 - 文章 - 伯乐在线
【导读】：说到 C/C++ 代码技巧，也许会有童鞋说 `#define true false`，这是[属于 C/C++ 程序员离职前恶搞之类的抖机灵](http://blog.jobbole.com/93699/)。即便想，也不能干。别忘了有这样一句编程名言：「在编写代码的时候，你要经常想着，那个最终维护你代码的人可能将是一个有暴力倾向的疯子，并且他还知道你住在哪里。」
本文整理了两位知乎网友对《[你见过哪些令你瞠目结舌的C/C++代码技巧？](https://www.zhihu.com/question/37692782)》的回复分享，均已获授权。
### 一、[陈宇飞的分享](https://www.zhihu.com/question/37692782/answer/122061597)，218 顶
写几个folly（Facebook开源的c++库）里面的例子。
——四更，folly::future 是如何chain你的callback的 ——
folly::future 是一个酷炫屌炸天的库，FB内部大量的异步C++的代码都是基于future的。我这段只讲他怎么支持下列语法的：


```
folly::makeFuture().then([]() {
// 返回一个数字
return 10;
}).then([](int i) {
// 如果写 }).then([](string i) { 的话编译器会报错
// 但是写 }).then([](Try<int> i) { 的话编译器不会报错
// }).then([](int&& i) { 的话编译器也不会报错
// }).then([](Try<int> && i) { 的话编译器也不会报错
});
```
也就是说，callback B 是接在callbackA后面的话，callback A 如果返回的是 T，我们可以支持callback B接受 T&&, T&, T, Try<T>, Try<T>&& 废话不说先上代码。解释在代码后面，所以嫌代码长的可以直接滑过去看解释。

C++
```
/** When this Future has completed, execute func which is a function that
    takes one of:
      (const) Try<T>&&
      (const) Try<T>&
      (const) Try<T>
      (const) T&&
      (const) T&
      (const) T
      (void)
    Func shall return either another Future or a value.
    A Future for the return type of func is returned.
    Future<string> f2 = f1.then([](Try<T>&&) { return string("foo"); });
    The Future given to the functor is ready, and the functor may call
    value(), which may rethrow if this has captured an exception. If func
    throws, the exception will be captured in the Future that is returned.
    */
  template <
      typename F,
      typename FF = typename detail::FunctionReferenceToPointer<F>::type,
      typename R = detail::callableResult<T, FF>>
  typename R::Return then(F&& func) {
    typedef typename R::Arg Arguments;
    return thenImplementation<FF, R>(std::forward<FF>(func), Arguments());
  }
template<typename F, typename... Args>
struct callableWith {
    template<typename T,
             typename = detail::resultOf<T, Args...>>
    static constexpr std::true_type
    check(std::nullptr_t) { return std::true_type{}; };
    template<typename>
    static constexpr std::false_type
    check(...) { return std::false_type{}; };
    typedef decltype(check<F>(nullptr)) type;
    static constexpr bool value = type::value;
};
template<typename T, typename F>
struct callableResult {
  typedef typename std::conditional<
    callableWith<F>::value,
    detail::argResult<false, F>,
    typename std::conditional<
      callableWith<F, T&&>::value,
      detail::argResult<false, F, T&&>,
      typename std::conditional<
        callableWith<F, T&>::value,
        detail::argResult<false, F, T&>,
        typename std::conditional<
          callableWith<F, Try<T>&&>::value,
          detail::argResult<true, F, Try<T>&&>,
          detail::argResult<true, F, Try<T>&>>::type>::type>::type>::type Arg;
  typedef isFuture<typename Arg::Result> ReturnsFuture;
  typedef Future<typename ReturnsFuture::Inner> Return;
};
template<typename F, typename... Args>
using resultOf = decltype(std::declval<F>()(std::declval<Args>()...));
```
这里一大波template我们一个一个来。


```
FunctionReferenceToPointer
```
可以无视掉，你可以想象成FF就是F，[std::declval](http://en.cppreference.com/w/cpp/utility/declval) 让你把 F变成 F&&，所以可以用


```
template<typename F, typename... Args>
using resultOf = decltype(std::declval<F>()(std::declval<Args>()...));
```
这样的语法拿到F(Args args…) 的返回值，不管F是object还是lambda。这样，resultOf可以拿到我们的callback的返回type。现在我们得把这个返回值跟下一个函数的argument对应起来。这里我们用callableWith


```
template<typename F, typename... Args>
struct callableWith {
    template<typename T,
             typename = detail::resultOf<T, Args...>>
    static constexpr std::true_type
    check(std::nullptr_t) { return std::true_type{}; };
    template<typename>
    static constexpr std::false_type
    check(...) { return std::false_type{}; };
    typedef decltype(check<F>(nullptr)) type;
    static constexpr bool value = type::value;
};
```
这里check有两个specialization，一个在编译时候会返回true一个会返回false。注意只要不符合第一个specialization的都是false，也就是说resultOf没有成功，check(nullptr) 就是false type。这个技巧叫做Substitution Failure Is Not An Error SFINAE – cppreference.com 。再配倒数第二行的typedef，如果 F可以接受args，那么callableWith<F>(Args args…)::value == true type。
最后，把所有我们允许的类用std::conditional一个一个试过去

C++
```
template<typename T, typename F>
struct callableResult {
  typedef typename std::conditional<
    callableWith<F>::value,
    detail::argResult<false, F>,
    typename std::conditional<
      callableWith<F, T&&>::value,
      detail::argResult<false, F, T&&>,
      typename std::conditional<
        callableWith<F, T&>::value,
        detail::argResult<false, F, T&>,
        typename std::conditional<
          callableWith<F, Try<T>&&>::value,
          detail::argResult<true, F, Try<T>&&>,
          detail::argResult<true, F, Try<T>&>>::type>::type>::type>::type Arg;
  typedef isFuture<typename Arg::Result> ReturnsFuture;
  typedef Future<typename ReturnsFuture::Inner> Return;
};
```
我们就可以在编译时间确保我们可以支持我们所有想支持的7个类啦。那具体拿着第一个callback的返回值怎么传输到第二个callback上面做argument呢？这一段变种太多，我只给大家看最简单的变种：


```
// Variant: returns a value
// e.g. f.then([](Try<T>&& t){ return t.value(); });
template <class T>
template <typename F, typename R, bool isTry, typename... Args>
typename std::enable_if<!R::ReturnsFuture::value, typename R::Return>::type
Future<T>::thenImplementation(F&& func, detail::argResult<isTry, F, Args...>) {
  static_assert(sizeof...(Args) <= 1, "Then must take zero/one argument");
  typedef typename R::ReturnsFuture::Inner B;
  throwIfInvalid();
  Promise<B> p;
  p.core_->setInterruptHandlerNoLock(core_->getInterruptHandler());
  // grab the Future now before we lose our handle on the Promise
  auto f = p.getFuture();
  f.core_->setExecutorNoLock(getExecutor());
  // 这里注释省略，因为实在太长。
  setCallback_([ funcm = std::forward<F>(func), pm = std::move(p) ](
      Try<T> && t) mutable {
    if (!isTry && t.hasException()) {
      pm.setException(std::move(t.exception()));
    } else {
      pm.setWith([&]() { return funcm(t.template get<isTry, Args>()...); });
    }
  });
  return f;
}
template <class T>
template <class F>
void Future<T>::setCallback_(F&& func) {
  throwIfInvalid();
  core_->setCallback(std::forward<F>(func));
}
 protected:
  typedef detail::Core<T>* corePtr;
  // shared core state object
  corePtr core_;
```
core_ 是future 的member，也就是说我们设置的callback仅仅只是被加到callback里面去了，还没有被执行。只有当你执行future.get() 的时候值才会被你拿到


```
template <class T>
T Future<T>::get() {
  return std::move(wait().value());
}
template <class T>
Future<T>& Future<T>::wait() & {
  detail::waitImpl(*this);
  return *this;
}
template <class T>
void waitImpl(Future<T>& f) {
  // short-circuit if there's nothing to do
  if (f.isReady()) return;
  FutureBatonType baton;
  f.setCallback_([&](const Try<T>& /* t */) { baton.post(); });
  baton.wait();
  assert(f.isReady());
}
```
啊，所有的callback执行都被扔到baton里面去了，我先不在这里继续深挖了。总之


```
auto value = folly::makeFuture(cb1).then(cb2).get();
```
- 在编译的时候，是通过一系列declval/decltype/SFAINE/std::conditional 来确保类是对的；
- 在运行的时候 cb1被传到future.core_ -> baton -> 拿到值 -> 根据cb2是不是接受try来分叉 -> 值扔给 cb2
我把Promise/Baton的内容全部跳过去了，因为要把那两个也讲了就没完没了了，在template metaprogramming上面也没有future这么fancy。以后有机会再细写那两个库吧！
——–三更，讲点稍微实用一点的数据结构吧, 两个看起来风马牛不相及其实储存上一致的folly::Optional 和 folly::Indestructible ——
**folly::Optional**
[folly/Optional.h at master · facebook/folly · GitHub](http://github.com/facebook/folly/blob/master/folly/Optional.h)
C++里面不是所有的类都可以是null的，特别有的时候这个类是其他人硬塞给你的。而当你需要它可是你null的时候，你把这个类放到folly optional里面，它就可以是null啦。folly optional里面比较有意思的是它储存的机制。

C++
```
using Storage = typename std::conditional<std::is_trivially_destructible<Value>::value,
                              StorageTriviallyDestructible,
                              StorageNonTriviallyDestructible>::type;
Storage storage_;
  struct StorageTriviallyDestructible {
    // uninitialized
    uniocn { Value value; };
    bool hasValue;
    StorageTriviallyDestructible() : hasValue{false} {}
    void clear() {
      hasValue = false;
    }
  };
  struct StorageNonTriviallyDestructible {
    // uninitialized
    union { Value value; };
    bool hasValue;
    StorageNonTriviallyDestructible() : hasValue{false} {}
    ~StorageNonTriviallyDestructible() {
      clear();
    }
    void clear() {
      if (hasValue) {
        hasValue = false;
        value.~Value();
      }
    }
  };
```
StorageTriviallyDestructible 还稍微合理一点，StorageNonTriviallyDestructible.clear() 里面~Value() 绝对很少见。这是c++11里面unconstrained union的新玩法，因为在unconstrained union里面你必须有能力可以销毁一个non-POD类，所以c++语法开放了~Value() 这种语法，让你可以销毁这个值。这个功能在这里就被弄出了新玩法，被用来支持folly::Optional.clear()，这样就算是一个NonTriviallyDestructible的对象你也可以随时销毁它。
这种unconstrained union的启动机制也是比较麻烦的。folly::optional.set 长这样：


```
template<class... Args>
  void construct(Args&&... args) {
    const void* ptr = &storage_.value;
    // for supporting const types
    new(const_cast<void*>(ptr)) Value(std::forward<Args>(args)...);
    storage_.hasValue = true;
  }
```


```
new(const_cast<void*>(ptr)) Value(std::forward<Args>(args)...);
```
这个叫placement new，就是说你给new 一个地址，new直接在你给的地址上面initialize，而不是去heap里面占内存。有上面两个玩法的话，你就可以随时随地在c++启动，销毁这个值啦！
**folly::Indestructible**
如何确保你的meyer’s singleton永远不死？这样储存你的类：


```
union Storage {
    T value;
    template <typename... Args>
    explicit constexpr Storage(Args&&... args)
        : value(std::forward<Args>(args)...) {}
    ~Storage() {}
  };
```
看起来好像没有什么特殊的对不对？不要忘记这个T在这里肯定是个non trivially destructable的类。在这个union里面，既然你的destructor是空的，那么也就是说value永远被遗忘了。。。遗忘了。。。遗忘了。。
看到这里有人要开骂了，为什么不直接new一个值出来，不销毁就好了？这跟new一个新的值出来最大的差别是这个不可以被遗忘的值是可以被inline的，它用的内存不是heap里面的内存（至少value本身不在heap上面）。这在效率上的差别是不可小觑的。
folly::optional 跟 folly::indestructable 都是利用了新标准里面union的新特性。看来新玩法还是要多想
———谢谢各位踊跃点赞，这段是二更。要是过一千的话我就写MPMCQueue哦——–
f
olly::Conv 是可以把所有类转化成所有类的库。也不是所有啦，不过正常人用的到的都有。我这里只讲int转字符串，float/double 实在太麻烦。
先看数字转字符串里面算多少位数的


```
/**
 * Returns the number of digits in the base 10 representation of an
 * uint64_t. Useful for preallocating buffers and such. It's also used
 * internally, see below. Measurements suggest that defining a
 * separate overload for 32-bit integers is not worthwhile.
 */
inline uint32_t digits10(uint64_t v) {
#ifdef __x86_64__
  // For this arch we can get a little help from specialized CPU instructions
  // which can count leading zeroes; 64 minus that is appx. log (base 2).
  // Use that to approximate base-10 digits (log_10) and then adjust if needed.
  // 10^i, defined for i 0 through 19.
  // This is 20 * 8 == 160 bytes, which fits neatly into 5 cache lines
  // (assuming a cache line size of 64).
  static const uint64_t powersOf10[20] FOLLY_ALIGNED(64) = {
      1,
      10,
      100,
      1000,
      10000,
      100000,
      1000000,
      10000000,
      100000000,
      1000000000,
      10000000000,
      100000000000,
      1000000000000,
      10000000000000,
      100000000000000,
      1000000000000000,
      10000000000000000,
      100000000000000000,
      1000000000000000000,
      10000000000000000000UL,
  };
  // "count leading zeroes" operation not valid; for 0; special case this.
  if UNLIKELY (!v) {
    return 1;
  }
  // bits is in the ballpark of log_2(v).
  const uint8_t leadingZeroes = __builtin_clzll(v);
  const auto bits = 63 - leadingZeroes;
  // approximate log_10(v) == log_10(2) * bits.
  // Integer magic below: 77/256 is appx. 0.3010 (log_10(2)).
  // The +1 is to make this the ceiling of the log_10 estimate.
  const uint32_t minLength = 1 + ((bits * 77) >> 8);
  // return that log_10 lower bound, plus adjust if input >= 10^(that bound)
  // in case there's a small error and we misjudged length.
  return minLength + (uint32_t) (UNLIKELY (v >= powersOf10[minLength]));
#else
  uint32_t result = 1;
  for (;;) {
    if (LIKELY(v < 10)) return result;
    if (LIKELY(v < 100)) return result + 1;
    if (LIKELY(v < 1000)) return result + 2;
    if (LIKELY(v < 10000)) return result + 3;
    // Skip ahead by 4 orders of magnitude
    v /= 10000U;
    result += 4;
  }
#endif
}
```
下面那个for loop多友好，上面x86那段是什么鬼！
builtin_clzll 是gcc里面的一个函数，具体定义看这里[Other Builtins](http://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html) 简单来说就是算有几个开头的0的。这里面的0是2进制的0,所以63-leading zeros就是说二进制里面有几位数。


```
const uint32_t minLength = 1 + ((bits * 77) >> 8);
```
这个就是亮点了。高中数学没学好的话，这里强调一下
- log_10(v) =log_10(2) * log_2(v) ([The Change-of-Base Formula](//link.zhihu.com/?target=http%3A//www.purplemath.com/modules/logrules5.htm)) 。
- >> 8 就是除以以256
- bits在这里已经是log(2)了
- +1是因为 0.3010略小于77/256我们继续瞎，现在开始正式转换


```
/**
 * Copies the ASCII base 10 representation of v into buffer and
 * returns the number of bytes written. Does NOT append a \0. Assumes
 * the buffer points to digits10(v) bytes of valid memory. Note that
 * uint64 needs at most 20 bytes, uint32_t needs at most 10 bytes,
 * uint16_t needs at most 5 bytes, and so on. Measurements suggest
 * that defining a separate overload for 32-bit integers is not
 * worthwhile.
 *
 * This primitive is unsafe because it makes the size assumption and
 * because it does not add a terminating \0.
 */
inline uint32_t uint64ToBufferUnsafe(uint64_t v, char *const buffer) {
  auto const result = digits10(v);
  // WARNING: using size_t or pointer arithmetic for pos slows down
  // the loop below 20x. This is because several 32-bit ops can be
  // done in parallel, but only fewer 64-bit ones.
  uint32_t pos = result - 1;
  while (v >= 10) {
    // Keep these together so a peephole optimization "sees" them and
    // computes them in one shot.
    auto const q = v / 10;
    auto const r = static_cast<uint32_t>(v % 10);
    buffer[pos--] = '0' + r;
    v = q;
  }
  // Last digit is trivial to handle
  buffer[pos] = static_cast<uint32_t>(v) + '0';
  return result;
}
```
这里面开始甩说用pos慢了，估计原因是loop unrolling做不了，但是具体不好说，我得问问他。
peephole optimization简单来说就是一段短小精悍的代码可以被compiler 变得更短小精悍（[Peephole optimization](http://en.wikipedia.org/wiki/Peephole_optimization)），具体肯定也是实测过才敢拿出来讲。非常好读懂我就不BB了。不过用C++用的熟练的看到这种不差buffer大小的肯定非常不爽。为什么可以不查？！


```
/**
 * int32_t and int64_t to string (by appending) go through here. The
 * result is APPENDED to a preexisting string passed as the second
 * parameter. This should be efficient with fbstring because fbstring
 * incurs no dynamic allocation below 23 bytes and no number has more
 * than 22 bytes in its textual representation (20 for digits, one for
 * sign, one for the terminating 0).
 */
template <class Tgt, class Src>
typename std::enable_if<
  std::is_integral<Src>::value && std::is_signed<Src>::value &&
  IsSomeString<Tgt>::value && sizeof(Src) >= 4>::type
toAppend(Src value, Tgt * result) {
  char buffer[20];
  if (value < 0) {
    result->push_back('-');
    result->append(buffer, uint64ToBufferUnsafe(-uint64_t(value), buffer));
  } else {
    result->append(buffer, uint64ToBufferUnsafe(value, buffer));
  }
}
```
好了我服了。。。
———————–原先的答案——————————
**AtomicStruct**
[folly/AtomicStruct.h at master · facebook/folly · GitHub](http://github.com/facebook/folly/blob/master/folly/AtomicStruct.h)
类似于std::atomic, 但是任何小于8个byte的POD类都可以变成atomic的。实现的方法如下：
用一个unconstrained union 来存数据：


```
union {
  Atom<Raw> data;
  T typedData;
};
```
T是你的类，Atom 就是std::atomic，Raw是这么来的


```
typename Raw = typename detail::AtomicStructIntPick<sizeof(T)>::type
```


```
template <> struct AtomicStructIntPick<1> { typedef uint8_t type; };
template <> struct AtomicStructIntPick<2> { typedef uint16_t type; };
template <> struct AtomicStructIntPick<3> { typedef uint32_t type; };
template <> struct AtomicStructIntPick<4> { typedef uint32_t type; };
template <> struct AtomicStructIntPick<5> { typedef uint64_t type; };
template <> struct AtomicStructIntPick<6> { typedef uint64_t type; };
template <> struct AtomicStructIntPick<7> { typedef uint64_t type; };
template <> struct AtomicStructIntPick<8> { typedef uint64_t type; };
```
我看到这里已经开始瞎了。compare exchange是这样的


```
bool compare_exchange_weak(
          T& v0, T v1,
          std::memory_order mo = std::memory_order_seq_cst) noexcept {
    Raw d0 = encode(v0);
    bool rv = data.compare_exchange_weak(d0, encode(v1), mo);
    if (!rv) {
      v0 = decode(d0);
    }
    return rv;
  }
```
里面的encode／decode就是拿来骗编译器的memcpy。写了这么多废话，说白了就是为了让编译器开心的可以用各种std::atomc<int>**DiscriminatedPtr**
用法就是boost::variant，但是用DiscriminatedPtr没有任何多余的代价，就是一个指针的大小。为什么可以没有代价呢？应为64位系统里面其实只有48位拿来做地址了，剩下16位是没有被系统用起来的。所以要地址是这么读的


```
void* ptr() const {
    return reinterpret_cast<void*>(data_ & ((1ULL << 48) - 1));
  }
```
那前16个bit是存什么呢？存的是现有这个类的index。每次存的时候，会通过index找到对应的类


```
/**
   * Set this DiscriminatedPtr to point to an object of type T.
   * Fails at compile time if T is not a valid type (listed in Types)
   */
  template <typename T>
  void set(T* ptr) {
    set(ptr, typeIndex<T>());
  }
```
然后


```
void set(void* p, uint16_t v) {
    uintptr_t ip = reinterpret_cast<uintptr_t>(p);
    CHECK(!(ip >> 48));
    ip |= static_cast<uintptr_t>(v) << 48;
    data_ = ip;
  }
```
那typeIndex是什么鬼！？typeIndex是一个编译是通过递归制造出来的列表，可以在编译时制造出一个数字对应类的列表


```
template <typename... Types> struct GetTypeIndex;
template <typename T, typename... Types>
struct GetTypeIndex<T, T, Types...> {
  static const size_t value = 1;
};
template <typename T, typename U, typename... Types>
struct GetTypeIndex<T, U, Types...> {
  static const size_t value = 1 + GetTypeIndex<T, Types...>::value;
};
```
具体实现在这里 [folly/DiscriminatedPtrDetail.h at master · facebook/folly · GitHub](http://github.com/facebook/folly/blob/master/folly/detail/DiscriminatedPtrDetail.h%23L33)。 这样在编译时间你就可以知道你要的类是不是这个指针支持的类。要是对编译时的黑魔法感兴趣的话，可以从boost的index_sequence看起 [boost/fusion/support/detail/index_sequence.hpp](//link.zhihu.com/?target=http%3A//www.boost.org/doc/libs/1_60_0/boost/fusion/support/detail/index_sequence.hpp)
DiscriminatedPtr还支持visitor pattern，具体这里不细讲应为没有什么typeIndex以外的黑科技。具体用法可以参照boost::invariant [Tutorial – 1.61.0](//link.zhihu.com/?target=http%3A//www.boost.org/doc/libs/1_61_0/doc/html/variant/tutorial.html%23variant.tutorial.basic)先写着么多，要是有人看的话我就继续写。你们可得用力点赞啊！
### 二、[Shiky Chang 回答](https://www.zhihu.com/question/37692782/answer/74409370)，1225 顶
如果说「瞠目结舌」的话，IOCCC 上随便拿一篇获奖代码出来就足以让人下巴落地了。
[The International Obfuscated C Code Contest](//link.zhihu.com/?target=http%3A//www.ioccc.org/)
一个比较经典的例子是 1988 年得奖的代码，这个程序**直接估算字符面积求圆周率**，可读性算是比较友好的：
•westley.c•


```
#define _ F-->00||-F-OO--;
int F=00,OO=00;main(){F_OO();printf("%1.3f\n",4.*-F/OO/OO);}F_OO()
{
            _-_-_-_
       _-_-_-_-_-_-_-_-_
    _-_-_-_-_-_-_-_-_-_-_-_
  _-_-_-_-_-_-_-_-_-_-_-_-_-_
 _-_-_-_-_-_-_-_-_-_-_-_-_-_-_
 _-_-_-_-_-_-_-_-_-_-_-_-_-_-_
_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
 _-_-_-_-_-_-_-_-_-_-_-_-_-_-_
 _-_-_-_-_-_-_-_-_-_-_-_-_-_-_
  _-_-_-_-_-_-_-_-_-_-_-_-_-_
    _-_-_-_-_-_-_-_-_-_-_-_
        _-_-_-_-_-_-_-_
            _-_-_-_
}
```
注：这段程序实际上是 1989 年修正过的，由于 88 年原来程序代码没有考虑到 ANSI C 的编译标准，导致在处理例如


```
#define _ -i
-_
```
的时候，老旧的 K&R 框架和 ANSI C 结果不一样：K&R 是直接的


```
--i
```
而 ANSI C 编译结果实际上等同于


```
-(-i)
```
因此之前的程序现在运行的话出来的结果是 0.250，而不是 3.141。修正过的程序就没有这个问题了。
又比如 13 年有个只有一行的程序，可以判断从 Franklin Pierce 往后的 31 位美国总统是民主党还是共和党，这个就有点不知所云了：
•cable1.c•


```
main(int riguing,char**acters){puts(1[acters-~!(*(int*)1[acters]%4796%275%riguing)]);}
```
使用方法：


```
make cable1
./cable1 obama republican democrat
./cable1 bush republican democrat
```
总统名要小写，republican 和 democrat 顺序不能颠倒。
经 [@chua zier](https://www.zhihu.com/people/7fd4c376c5ac9d259b25d1351e55342d) 提醒，历史上的确有重名的美国总统，除了 Johnson 之外，还有 Theodore Roosevelt / Franklin D. Roosevelt，程序原作者注明用


```
./cable1 roosevelt republican democrat
```
表示 Theodore Roosevelt，而用


```
./cable1 fdr republican democrat
```
表示 Franklin D. Roosevelt。
这一行代码做了这么多事：首先查询输入的总统的名字，然后在一个 look-up table 里面找出对应的政治阵营，再输出出来。问题在于这 31 位总统名字存放在哪里？而这个 look-up table 又存放在哪里？
有趣的是，IOCCC 的评委还提到，你甚至可以用这个程序检测一些 IT 大佬的 Mac / PC 阵营：


```
./cable1 Cooper Mac PC
    ./cable1 Noll Mac PC
    ./cable1 Broukhis Mac PC
    ./cable1 Jobs Mac PC
    ./cable1 Gates Mac PC
    ./cable1 Ballmer Mac PC
```
难道这个程序暴露了 Ballmer 离开微软的真相？
最近几届比赛的代码为了增加混乱程度，代码越来越长，可读性也越来越差（不过话说回来，让可读性变得越来越差其实原本就是这个比赛的第一宗旨吧），不少代码甚至本身就是个 ASCII artwork……比如 11 年有一只阿卡林：
•akari.c•
![](http://ww4.sinaimg.cn/mw690/63918611gw1f822axfm4ej20go0e6td3.jpg)
为了保持美观我就直接上图了。源代码见此：[http://www.ioccc.org/2011/akari/akari.c](//link.zhihu.com/?target=http%3A//www.ioccc.org/2011/akari/akari.c)
**–––––––––– !!! 前方阿卡林军团高能预警 !!! ****––––––––––**
这个阿卡林程序实际上是一个图像 down-sampler，可以接受符合条件的 PGM / PPM 灰度图像或者 LF 换行（不支持 CR-LF）的 ASCII art 为输入，然后转换输出一个处理后的图像 / ASCII art。不过这个阿卡林最逆天的地方在于，它可以用**自身的源代码文本作为输入，输出生成另一个可以编译运行的程序的代码**！而且把这个生成的程序文本继续作为输入做进一步 down-sample，又可以生成一段可以编译的代码，如此反复，**可以套多达4层**！详细的食用方法如下：


```
make akari
./akari akari.c akari2.c
```
然后生成的**阿卡林·2号**是这个样子的：
•akari2.c•
![](http://ww3.sinaimg.cn/mw690/63918611gw1f822ayg5z2j20go0de79v.jpg)
看不清？请摘下眼镜或者退远了看。注意，**阿卡林·2号也是可以编译运行的**，她的功能是把输入的 ACSII 文本的每个字符中间插入空格以及每行之间插入空行，生成一段**“疏松”了的文本**。我们用阿卡林·2号自己做实验品**：**


```
make akari2.c
./akari2 <akari2.c> akari2fat.txt
```
成功了！生成了一只**阿卡林·2号·舒松**：
•akari2fat.txt•
![](http://ww2.sinaimg.cn/mw690/63918611gw1f822az4f21j20go0d4mzp.jpg)
阿卡林·2号还能干别的，她支持一个 rot13 参数：


```
./akari2 rot13 <akari2.c> akari2fat.txt
```
生成的是经过 [ROT13](http://en.wikipedia.org/wiki/ROT13) 仿射变换的文本，我们称之为**阿卡林·2号·舒松·加蜜**吧！
但是还没完……如果我们把原版阿卡林放进去再来一层呢？


```
./akari < akari.c | ./akari > akari3.c
```
于是**阿卡林·3号**诞生：
•akari3.c•


```
wm_aoi(n)
  /*ity,,[2*/{}char*y=
 (")M{lpduKtjsa(v""YY"
 "*yuruyuri") ;main(/*
/",U/  R)U*  Y0U= ="/\
*/){puts    (y+ 17/*
 "NR{I="       ){/=*
   =*         */);/*
   **/{      ;;}}
```
可怜的阿卡林·3号，由于“马赛克”（down-sample）次数太多，摘了眼镜也只能模糊看到一点点……我们来问问阿卡林·3号对于诞生的感受吧：


```
make akari3
./akari3
```
于是她回答：


```
yuruyuri
```
居然会说ゆるゆり！
最后，我们尝试生产一下**阿卡林·4号**：


```
./akari < akari.c | ./akari | ./akari > akari4.c
```
•akari4.c•


```
main
(){puts("Y"
"U RU YU "\
"RI"   )/*
 */   ;}
```
顺利生产！虽然内容已经直截了当了，不过我们还是采访一下她吧：


```
make akari4
./akari4
```
她的答复是：


```
YU RU YU RI
```
至此，阿卡林军团全部诞生！
![](http://ww3.sinaimg.cn/mw690/63918611gw1f822azp9w5j20go09dgq9.jpg)
不得不佩服作者构建代码的精妙程度。他的个人主页在这里：[uguu…](//link.zhihu.com/?target=http%3A//uguu.org/) （这位作者其实已经是这比赛的常客了，先后一共拿过 6 次不同的奖项。）
经 [@马琦明](//www.zhihu.com/people/7293a41395d86ba85201c9adc814b46a) 提醒，我又把上面这位作者的另一个作品搬出来了，13 年的 Most Catty——炮姐程序。这程序的代码长这个样子：
•misaka.c•
![](http://ww2.sinaimg.cn/mw690/63918611gw1f822b16nzgj20go0fqgvg.jpg)
源代码：[http://www.ioccc.org/2013/misaka/misaka.c](https://link.zhihu.com/?target=http%3A//www.ioccc.org/2013/misaka/misaka.c)
对的，当你看到原来是这个“御坂”的时候，你就知道，我们要开始造（kè）人（lóng）了……


```
make misaka
```
这个**御坂**的作用是把输入的 ASCII **横向连接**起来。首先连接两个自己试试：


```
./misaka misaka.c misaka.c > misaka2.c
```
“把两个御坂输入一个御坂，会生成什么？”“两个御坂。”
•misaka2.c•
![](http://ww2.sinaimg.cn/mw690/63918611gw1f822b28txhj20go07x455.jpg)
听起来很不可思议但是在这位作者的构建下完全不出意外地，上面这个**御坂-2** 居然也是可以编译运行的：


```
make misaka2.c
```
御坂-2 的功能是把输入的 ASCII **纵向连接**起来。那我们就试着纵向连接两个御坂：


```
./misaka2 misaka.c misaka.c > misaka3.c
```
于是**御坂-3** 诞生了：
•misaka3.c•
![](http://ww1.sinaimg.cn/mw690/63918611gw1f822b2mtnjj20go0tltry.jpg)
我们来运行一下这个御坂-3。你此时脑中的景象可能是这样的：
![](http://ww4.sinaimg.cn/mw690/63918611gw1f822b3rajij20go0m87cy.jpg)
但是你错了，御坂-3 会给你造出来更加精神污染的那只 long cat：


```
make misaka3
./misaka3
```
![](http://ww2.sinaimg.cn/mw690/63918611gw1f822b4nnjaj20go0a0q45.jpg)
没错就是这只喵：
![](http://ww2.sinaimg.cn/mw690/63918611gw1f822b550n8j20go0u5jvq.jpg)
这里其实有 Unix 的 cat 指令的梗……如果之前你在执行御坂-2 的时候，用了更多的御坂作为输入，例如 4 个：


```
./misaka2 misaka.c misaka.c misaka.c misaka.c > misaka4.c
make misaka4.c
./misaka4
```
那么**御坂-4** 会给你造一只更长的 looooong cat：
![](http://ww3.sinaimg.cn/mw690/63918611gw1f822b5sjepj20go0j4abu.jpg)
按作者的意思，你可以最多叠加 31 个御坂来生成一只 looo….ooong cat（具体上限由编译器的 sizeof(int) 决定）。
13 年还有浙大教授侯启明写的 ray tracer 程序，虽然代码本身存在争议是否符合比赛规则，例如为避免长度超限制而使用了一些压缩方法、程序是个死循环。如果这段程序可读性不是这么恶心的话其实还是非常值得钻研的，里面用到了很多有趣的数据结构和着色体系。
食用方法也很简单，把程序挂在那儿跑一晚上，强制退出，就可以看结果了。由于是无穷尽的递归，程序跑的时间越长，图像就越精致。详细的说明和源文件还是参考官网吧：
[Previous IOCCC Winners with spoilers](http://www.ioccc.org/years-spoiler.html%232012)
这里有个示例图。
![](http://ww1.sinaimg.cn/mw690/63918611gw1f822b7a0vyj20go0ciwhf.jpg)
侯老师还有另外三个作品上榜，一个是极其酷炫的 syntax highlightener，还有一个（**源代码本身就是 GUI 的**）科学计算器，后面这个已经有人 [@paintsnow](//www.zhihu.com/people/264ef4765d10a40dbfe3b5bb6480fa7f) 回答过了。最新一个是上个月刚刚公布的新一届获奖作品 MD5 without integers，但是这个的源码还没有公布，估计要等到明年了。
### 三、平方根倒数速算法
伯小乐看到有其他网友提到了平方根倒数速算法

C
```
float FastInvSqrt(float x) {
  float xhalf = 0.5f * x;
  int i = *(int*)&x;         // evil floating point bit level hacking
  i = 0x5f3759df - (i >> 1);  // what the fuck?
  x = *(float*)&i;
  x = x*(1.5f-(xhalf*x*x));
  return x;
}
```
这个技巧就不详细解释了，推荐大家重新再看看这篇文章《[数学之美：平方根倒数速算法中的神奇数字 0x5f3759df](http://blog.jobbole.com/105295/) 》。
