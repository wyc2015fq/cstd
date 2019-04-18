# python技巧31[python中使用enum] - weixin_33985507的博客 - CSDN博客
2011年03月08日 14:37:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
以下几种方法来模拟enum：（感觉方法一简单实用）
```
# way1
class Directions:
    up = 0
    down = 1
    left = 2
    right =3
    
print Directions.down
# way2
dirUp, dirDown, dirLeft, dirRight = range(4)
print dirDown
# way3
import collections
dircoll=collections.namedtuple('directions', ('UP', 'DOWN', 'LEFT', 'RIGHT'))
directions=dircoll(0,1,2,3)
print directions.DOWN
# way4
def enum(args, start=0):
    class Enum(object):
        __slots__ = args.split()
        def __init__(self):
            for i, key in enumerate(Enum.__slots__, start):
                setattr(self, key, i)
    return Enum()
e_dir = enum('up down left right')
print e_dir.down
# way5
# some times we need use enum value as string
Directions = {'up':'up','down':'down','left':'left', 'right':'right'}
print Directions['down']
```
问题：有的时候需要使用enum的值作为字符串使用，像way5中表示的，大家有没有更好的办法？
参考：[http://audbel.com/0/5009691](http://audbel.com/0/5009691)
完！
