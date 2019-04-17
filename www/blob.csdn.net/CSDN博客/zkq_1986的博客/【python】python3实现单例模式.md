# 【python】python3实现单例模式 - zkq_1986的博客 - CSDN博客





2017年12月29日 10:43:56[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：134







class A(object):

    __instance = None

    def __new__(cls, *args, **kwargs):

        if cls.__instance is None:

            cls.__instance = super(A, cls).__new__(cls)

        return cls.__instance

    def __init__(self, status_number):

        self.status_number = status_number



a1=A(1)

a2=A(1)

print(a1 == a2)              #输出 True



class B(object):

    def __init__(self, status_number):

        self.status_number = status_number



a1=B(1)

a2=B(1)

print(a1 == a2)              #输出 False









