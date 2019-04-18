# python类继承，父类和子类的构造函数初始化调用 - weixin_33985507的博客 - CSDN博客
2018年07月02日 11:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
```
# class Info(object):
#     def __init__(self,name,age,score):
#         self.Name = name
#         self.Age = age
#         self.Score = score
#
#
# class Student(object):
#     def __init__(self,info):
#         self.Info = info
#
#     # Home = "地球"
#     # __Sxt = "女"
#     #
#     # def GetSex(self):
#     #     return Student.__Sxt
#     #
#     # def __init__(self,name,age,score,comp):
#     #     self.Name = name
#     #     self.Age = age
#     #     self.Score = score
#     #     self.__Comp = comp
#     #
#     # def GetComp(self):
#     #     return self.__Comp
#     #
#     # def SetComp(self,comp):
#     #     self.__Comp = comp
#
# # stu = Student("gou",21,98)
# #
# # stu.Home = "火星"
# # print(stu.Home)
# # print(stu.__class__.Home)
# # print(Student.Home)
#
# # stu1 = Student("a",11,33,"mac")
# # print(stu1.GetComp())
# # stu1.SetComp("deal")
# # print(stu1.GetComp())
# #
# # print(stu1.GetSex())
#
# # info = Info("a",18,98)
# stu = Student(Info("a",18,98))
#
# print(type(stu))
# print(type(stu.Info))
# print(stu.Info.Name)
#  信息基类
class MemberInfo(object):
    __name = None
    __age = None
    __score = None
    def __init__(self,name,age,score):
        print("parent init")
        self.__name = name
        self.__age = age
        self.__score = score
    def GetName(self):
        return self.__name
class Student(MemberInfo):
    def __init__(self,name,age,score,comp):
        MemberInfo.__init__(self,name,age,score)
        print("child init")
        self.__Comp = comp
    def GetComp(self):
        return self.__Comp
#
# # stu = Student("a",15,98)
# stu = Student("mac")
# print(stu.GetComp())
stu = Student("gou",18,98,"mac")
print(stu.GetName())
print(stu.GetComp())
```
