# C 语言随机数生成器的实现分析 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [卢亮](http://www.jobbole.com/members/lulyon) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
本文是为了记录和澄清一个由来已久的关于C语言随机数生成器的误解。
目前所看到的所有公开的关于C随机数生成器的中文资料，都提到经典的线性同余法( LCG, linear congruential generator)，并认为是默认的实现方法。这个说法并不准确。以GCC为例，GLIBC的确实现了线性同余法，但是实现的代码块分支在日常使用中不会执行到，线性同余法为C语言默认随机数生成器的说法已过时。
本文将以GLIBC源代码为例，结合掌握的文档，做一回搬运工，总结描述一下GLIBC中随机数生成器的实现。
## 线性同余法
线性同余法，LCG(linear congruential generator)，是经典的伪随机数产生器算法，速度快，容易理解实现。 LCG 算法数学上基于公式：
X(n+1) = (a * X(n) + c) % m
其中，各系数为：模m, 系数a, 0 < a < m，增量c, 0 <= c < m，原始值(种子) 0 <= X(0) < m 。其中参数c, m, a比较敏感，或者说直接影响了伪随机数产生的质量。
GLIBC中对LCG的实现，取a = 1103515245, c = 12345, m = 134217728，即
X(n+1) = (1103515245 * X(n) + 12345) & 2147483647
由于LCG计算简单，极省内存，很适合内存和计算资源比较紧张的嵌入式环境。但LCG有一个严重的缺陷，即产生的伪随机数强依赖于上一次生成的随机数，且重复周期等于随机范围，不能用于随机数要求高的场合。
原因是单状态生成器在每次*rand()*调用时不会生成完全的伪随机数，实际做的是以伪随机的顺序遍历(0~2^31)范围内的数。意味着当获取到一个为伪随机数时，在当前周期内不会再获取到同一个数，只有在经过2^31次*rand()*调用之后，才会获取这个数（而且只会获取到这个数）。
## 线性累加反馈法
线性累加反馈法，即LAFM（linear additive feedback method），以下是GLIBC使用的线性累加反馈法的流程描述。其中，2147483647 = 2^31 – 1，4294967296 = 2^32. 所有变量都是整数。 对于给定的种子常量s, 初始化序列r0…r33通过以下步骤计算：
- r(0) = s
- r(i) = (16807 * r(i-1) ) % 2147483647 (i = 1…30)
- r(i) = r(i-31) (i = 31…33)
注意数乘16807的结果应该由足够大的整数类型存储，避免取模操作之前发生值溢出。r(i-1)在乘法操作已经是32位整数，r(i)计算结果确保是0到2147483646之间的正整数, 即使r(i-1)为负数。
从r34开始的伪随机序列，通过以下的线性反馈循环来计算：
4. r(i) = (r(i-3) + r(i-31)) % 4294967296 (i ≥ 34)
忽略掉r0…r343序列，rand()函数输出的伪随机数o(i)为：
5. o(i) = r(i+344) >> 1
r(i+344)的个位数字移除，生成31位随机数o(i)。
以下为模拟步骤1~4的代码：


```
r[0] = seed;
for (i=1; i<31; i++) {
r[i] = (16807LL * r[i-1]) % 2147483647;
if (r[i] < 0) {
  r[i] += 2147483647;
}
for (i=31; i<34; i++) r[i] = r[i-31];
for (i=34; i<344; i++) r[i] = r[i-31] + r[i-3];
for (i=344; i<MAX; i++) r[i] = r[i-31] + r[i-3];
```
## GLIBC的实现
GLIBC实现了以上两种算法。LAFM生成器标记为 *TYPE*1*, *TYPE*2*, *TYPE_3* 和 *TYPE*4* 类型，LCG 生成器标记为 *TYPE*0*。相比LCG，LAFM生成器预先生成有很多初始状态，消除了LCG生成器的周期性遍历的属性，在同一个周期内，可以多次获取到相同的随机数。
为了提高随机数生成的时间和空间效率，在计算伪随机序列时GLIBC使用指针指向包含前驱随机值的数组，写法与按上述公式步骤直译的方式有所不同。


```
int
__random_r (buf, result)
 struct random_data *buf;
 int32_t *result;
{
  int32_t *state;
  if (buf == NULL || result == NULL)
goto fail;
  state = buf->state;
  if (buf->rand_type == TYPE_0)
{
  int32_t val = state[0];
  val = ((state[0] * 1103515245) + 12345) & 0x7fffffff;
  state[0] = val;
  *result = val;
}
  else
{
  int32_t *fptr = buf->fptr;
  int32_t *rptr = buf->rptr;
  int32_t *end_ptr = buf->end_ptr;
  int32_t val;
  val = *fptr += *rptr;
  /* Chucking least random bit.  */
  *result = (val >> 1) & 0x7fffffff;
  ++fptr;
  if (fptr >= end_ptr)
    {
      fptr = state;
      ++rptr;
    }
  else
    {
      ++rptr;
      if (rptr >= end_ptr)
    rptr = state;
    }
  buf->fptr = fptr;
  buf->rptr = rptr;
}
  return 0;
 fail:
  __set_errno (EINVAL);
  return -1;
}
```
具体使用哪个生成器依赖于初始状态集合，由**initstate()**函数生成：


```
int
__initstate_r (seed, arg_state, n, buf)
 unsigned int seed;
 char *arg_state;
 size_t n;
 struct random_data *buf;
{
  if (buf == NULL)
goto fail;
  int32_t *old_state = buf->state;
  if (old_state != NULL)
{
  int old_type = buf->rand_type;
  if (old_type == TYPE_0)
    old_state[-1] = TYPE_0;
  else
    old_state[-1] = (MAX_TYPES * (buf->rptr - old_state)) + old_type;
}
  int type;
  if (n >= BREAK_3)
type = n < BREAK_4 ? TYPE_3 : TYPE_4;
  else if (n < BREAK_1)
{
  if (n < BREAK_0)
    goto fail;
  type = TYPE_0;
}
  else
type = n < BREAK_2 ? TYPE_1 : TYPE_2;
  int degree = random_poly_info.degrees[type];
  int separation = random_poly_info.seps[type];
  buf->rand_type = type;
  buf->rand_sep = separation;
  buf->rand_deg = degree;
  int32_t *state = &((int32_t *) arg_state)[1]; /* First location.  */
  /* Must set END_PTR before srandom.  */
  buf->end_ptr = &state[degree];
  buf->state = state;
  __srandom_r (seed, buf);
  state[-1] = TYPE_0;
  if (type != TYPE_0)
state[-1] = (buf->rptr - state) * MAX_TYPES + type;
  return 0;
 fail:
  __set_errno (EINVAL);
  return -1;
}
```
## 总结
LCG生成器在状态数组(*buf->state*)长度为8字节时才会使用。 状态数组长度更大时则会启用LAFM生成器。通常在使用*rand()*方法时，会使用*srand()*设置种子常量，这时状态数组默认就是128字节, 所以实际会启用LAFM生成器。
最后，以上分析如果有偏差，很可能是作者对资料或代码的理解问题，欢迎即时反馈。
参考资料：
- 纯线性同余随机数生成器介绍
http://www.cnblogs.com/xkfz007/archive/2012/03/27/2420154.html
- random/initstate源代码
https://github.com/lattera/glibc/blob/master/stdlib/random_r.c
- glibc rand function implementation
http://stackoverflow.com/questions/18634079/glibc-rand-function-implementation
- The GLIBC random number generator
http://www.mscs.dal.ca/~selinger/random/
