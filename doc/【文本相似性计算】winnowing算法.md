# 【文本相似性计算】winnowing算法

 																				2018年09月25日 12:22:57 					[夜风之南](https://me.csdn.net/weixin_43098787) 						阅读数：207 										

 									

# winnowing算法

------

## 算法原理

k-grams是指字符串中长度为k的子串，这些子串满足“高度邻接”（k值自定义）。比如一个字符串abcd，那么将abcd分解成3-grams就是abc和bcd两个子串，每个子串就称为一个3-gram。可以看到第二个子串仅仅是在第一个子串的基础上向后移了一个位置，所以对于一个长度为n的字符串，当我们将它分成k-grams后，将会形成n-k+1个子串，且每个子串长度为k。接下来就可以将每个k-gram哈希为一个整数，如果将所有哈希值作为文件的指纹用于比较，显然是不高效的，也没有那个必要，所以只需选取哈希值的一个子集最为文件指纹即可。一种方法是选取所有满足0  mod p（模p余0）的哈希值，下面是一个具体的例子：

> 原内容：
>  A do run run run, a do run run
>  删除无关内容：
>  adorunrunrunadorunrun
>  分解成5-grams：
>  adoru dorun orunr runru unrun nrunr runru unrun nruna runad unado nador adoru dorun orunr runru unrun
>  哈希5-grams为：
>  77 72 42 17 98 50 17 98 8 88 67 39 77 72 42 17 98
>  满足0 mod 4的哈希值： 72 8 88 72

这个方法有个缺点，它不能保证一定可以检测到文件中相同的部分。如果两个满足0 mod  p的哈希值之间隔了很远，那么在这两个哈希值之间的内容就算相同，也不会被检测到。为了解决这个问题，我们可以定义一个大小为w的窗口（w值自定义）来分割哈希值，窗口内的内容也是“高度邻接”的。如上例中使用窗口分割为：

> 使用w=4的窗口分割后：
>  (77, 72, 42, 17) (72, 42, 17, 98)
>  (42, 17, 98, 50) (17, 98, 50,17)
>  (98, 50, 17, 98) (50, 17, 98,8)
>  (17, 98, 8, 88) (98, 8, 88, 67)
>  (8, 88, 67, 39) (88, 67,39, 77)
>  (67, 39, 77, 72) (39, 77, 72, 42)
>  (77, 72, 42,17) (72, 42, 17, 98)

经过分割后，我们只要保证每个窗口至少选出一个哈希值，就能避免哈希值之间距离太远的问题。同时，因为长度为w的窗口实际上对应了原文中长度t=k+w-1的子串，所以该方法保证了原文件中所有长度为t的子串，只要相同就一定不会被漏掉。接下来的问题就是如何选取哈希值了。简单但是不太健壮的策略是，我们可以选取每个窗口内第i个的哈希值作为代表，但是这个方法不具备位置无关性（要求3），插入和删除一段内容都会对结果造成影响。实际上winnowing算法所实现的就是选取哈希值的策略。

Winnowing算法所采用的策略是：选取每个窗口中最小的哈希值（显然两个窗口可能共享同一个最小值）。如果存在多个最小值，则选取最右边的。该策略既保证选取足够的指纹信息，又保证了不会产生太庞大的指纹。窗口分割示例中的红色部分就是通过winnowing选取出的指纹，在实际应用中，还可以记录哈希值出现的位置，用于跟踪相似内容出现的位置。如[17,3  ] [17,6] [8,8] [39,11] [17,15]（下标从0开始，第二个值表示哈希值在原序列中出现的位置）。

## 代码实现

具体实现代码如下：

```python
import re
import hashlib


# 重写hash函数1
def hash(text):
    hash = 0
    for i in range(len(text)):
        hash += ord(text[i]) * (17**(len(text) - i - 1))
    return hash


# # 重写hash函数2
# def hash(text):
#     hashval = hashlib.sha1(text.encode('utf-8'))
#     hashval = hashval.hexdigest()[-4 :]
#     hashval = int(hashval, 16)  #using last 16 bits of sha-1 digest
#     return hashval


def pre_process_document(s):
    pattern = '[,.!?;:\s\-\{\}\[\]]' #，。！？；：“”、
    s = re.sub(pattern, '', s)
    s = s.lower()
    return s


def make_kgrams(s, k):
    grams = []
    start, end = 0, k
    while start < len(s) - k + 1:
        grams.append(s[start:end])
        start += 1
        end += 1
    return grams


def right_weight_min(key=lambda x: x):
    def r_min(l):
        cur_min, min_index, i = float('inf'), -1, 0 
        while i < len(l):
            if key(l[i]) <= cur_min:
                cur_min, min_index = key(l[i]), i
            i += 1
        return l[min_index]
    return r_min


def winnow(k_grams, k, t):
    min = right_weight_min(lambda x: x[0])
    fingerprints = {}
    hashes = [(hash(k_grams[i]), i) for i in range(len(k_grams))]
    windowSize = t - k + 1
    # to guarantee matches for a t-sized string, 1 of the 
    # t - k + 1 hashes which will match must be selected
    # as a fingerprint.
    w_start, w_end = 0, windowSize
    cur_min = None
    while w_end < len(hashes):
        window = hashes[w_start:w_end]
        new_min = min(window)
        if cur_min != new_min:
            fingerprints[new_min[1]] = new_min[0]
            cur_min = new_min
        w_start, w_end = w_start + 1, w_end + 1
    return fingerprints


def main():
    k, t = 5, 8
    k_grams = make_kgrams('adorunrunrunadorunrun', k)
    print(k_grams)
    fingerprint = winnow(k_grams, k, t)
    l = [[2,0],[3,1],[4,2],[5,3],[1,4]]
    print(fingerprint)



if __name__ == '__main__':
    main()
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364656667686970717273747576777879801234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768697071727374757677787980
```

## 参考资料

[1] Winnowing算法
 <https://blog.csdn.net/xywlzd/article/details/7994924?utm_source=copy>