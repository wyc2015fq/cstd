# python __mro__ c3算法 - Arthur的随笔 - CSDN博客
2011年11月29日 21:39:24[largetalk](https://me.csdn.net/largetalk)阅读数：1566
```python
def mro_c3(*bases):
    if len(bases) == 1:
        if not bases[0].__bases__:
            return bases
        else:
            return bases + mro_c3(*bases[0].__bases__)
        
    else:
        seqs = [list(mro_c3(C)) for C in bases] + [list(bases)]
        res = []
        while True:
            non_empty = list(filter(None, seqs))
            if not non_empty:
                return tuple(res)
            for seq in non_empty:
                candidate = seq[0]
                not_head = [ s for s in non_empty if candidate in s[1:]]
                if not_head:
                    candidate = None
                else:
                    break
            if not candidate:
                raise TypeError('adfadsf')
            res.append(candidate)
            for seq in non_empty:
                if seq[0] == candidate:
                    del seq[0]
```
比如：
```python
class A(object): pass
class B(dict ): pass
class CC(A, B): pass
```
求CC.__mro__步骤是：(下面mro既mro_c3)
(CC) + mro((A,B))
(CC)+[mro(A), mro(B)] + [[A, B]]
(CC) + [ (A, object),  (B, dict, object), [A,B]] ==> res =[]
(CC) + [ ( object),  (B, dict, object), [B]] ==> res =[A]
(CC) + [ (object),  (B, dict, object), [B]] ==> res =[A] #object 没有被加进res
(CC) + [ (object),  ( dict, object), []] ==> res =[A,B]
(CC) + [ (object),  ( dict, object)] ==> res =[A,B] #object 没有被加进res
(CC) + [ (object),  ( object)] ==> res =[A,B, dict]
(CC) + [ ] ==> res =[A,B, dict, object]
(CC) + res => (CC, A, B, dict, object)
