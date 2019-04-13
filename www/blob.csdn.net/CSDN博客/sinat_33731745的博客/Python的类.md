
# Python的类 - sinat_33731745的博客 - CSDN博客

2018年02月12日 23:37:21[Tao-Tao-Tao](https://me.csdn.net/sinat_33731745)阅读数：73



# 类的定义
```python
class
```
```python
Human
```
```python
(object)
```
```python
:
```
```python
def
```
```python
eat
```
```python
(self)
```
```python
:
```
```python
print(
```
```python
'I can eat'
```
```python
)
DoctorLiu = Human()
DoctorLiu.eat()
```
```python
I
```
```python
can eat
```
class Human(object)定义一个类叫做Human，Human类继承了object这个类，object类是python内的一个基础类，所有的类都继承它
def eat(self)定义了一个函数叫做eat，他的作用是输出一句话：I can eat，self作为一个参数传入，self为类本身（实际为实例化之后的类，即为DoctorLiu），self传入时，会将这个类所有的变量传入，目前未给self定义其他变量。
# 类的函数的传参
```python
class
```
```python
Human
```
```python
(object)
```
```python
:
```
```python
def
```
```python
eat
```
```python
(self,food=None)
```
```python
:
```
```python
if
```
```python
food:
            print(
```
```python
'I like eating %s'
```
```python
%food)
```
```python
else
```
```python
:
            print(
```
```python
'I can eat'
```
```python
)
DoctorLiu = Human()
DoctorLiu.eat()
DoctorLiu.eat(
```
```python
'shrimp'
```
```python
)
```
```python
I
```
```python
can eat
```
```python
I
```
```python
like eating shrimp
```
定义了一个函数叫做eat，参数food的默认值为None，if food 表示，若food不为None时，则该表达式为真，则输出:’I like eat %s’%food,%s为占位符，即先占据一个位置，用food的值来填充它。若 food 为None，则输出：I can eat。
# self的使用
```python
class
```
```python
Human
```
```python
(object)
```
```python
:
```
```python
def
```
```python
eat
```
```python
(self,food=None)
```
```python
:
```
```python
if
```
```python
food:
            like_food = food
            self.LIKE_FOOD = food
            print(
```
```python
'I like eating %s'
```
```python
%self.LIKE_FOOD)
            print(
```
```python
'I like eating %s'
```
```python
% like_food)
```
```python
else
```
```python
:
            print(
```
```python
'I can eat'
```
```python
)
```
```python
def
```
```python
Question
```
```python
(self)
```
```python
:
```
```python
print(
```
```python
'You like eating %s'
```
```python
%self.LIKE_FOOD)
```
```python
#print('You like %s'%like_food)
```
```python
DoctorLiu = Human()
DoctorLiu.eat()
DoctorLiu.eat(
```
```python
'shrimp'
```
```python
)
DoctorLiu.Question()
print(DoctorLiu.LIKE_FOOD)
```
```python
I
```
```python
can eat
```
```python
I
```
```python
like eating shrimp
```
```python
I
```
```python
like eating shrimp
You like eating shrimp
shrimp
```
在eat函数中，将food赋值给self.like_food，虽然这个赋值语句发生在eat这个函数内，但由于self的存在，self.like_food成为Human这一整个类的变量，他不仅可以在eat函数里面使用，也可以在Human这个类的其他函数里面直接使用，而like_food只能在eat这个函数内使用。
在Question函数中，若执行print(‘You like %s’%like_food)将会报错，因为like_food只属于eat这个函数，而self.LIKE_FOOD属于Human这个类，所以Human的任何函数都可以直接调用它
LIKE_FOOD已经成为DoctorLiu这个类的一部分，可以认为是DoctorLiu的一个属性，可以直接输出来看看，而like_food却不可以

# __init__的使用
```python
class
```
```python
Human
```
```python
(object)
```
```python
:
```
```python
def
```
```python
__init__
```
```python
(self)
```
```python
:
```
```python
self.num_hands=
```
```python
2
```
```python
self.num_legs=
```
```python
2
```
```python
DoctorLiu = Human()
print(
```
```python
"DoctorLiu有%s只手"
```
```python
%DoctorLiu.num_hands)
print(
```
```python
"DoctorLiu有%s条腿"
```
```python
%DoctorLiu.num_legs)
```
```python
DoctorLiu有2只手
DoctorLiu有2条腿
```
一个实例在创建时，会有一些自身所带有的变量，不需要在通过调用其他函数而获得，如Human是一个类型，而DoctorLiu是Human一个实例，DoctorLiu在被创建时，她自身就会有两条腿，两只手，这个属性是不需要后天去学习而获得，是与生俱来的。
而上一段代码中self.LIKE_FOOD，需要去执行eat这个函数，DoctorLiu才有了这个属性。

# __init__传参
```python
class
```
```python
Human
```
```python
(object)
```
```python
:
```
```python
def
```
```python
__init__
```
```python
(self,eyes,skin)
```
```python
:
```
```python
self.num_hands=
```
```python
2
```
```python
self.num_legs=
```
```python
2
```
```python
self.eyes = eyes
        self.skin = skin
DoctorLiu = Human(eyes=
```
```python
'大'
```
```python
,skin=
```
```python
'白'
```
```python
)
print(
```
```python
"DoctorLiu有%s只手"
```
```python
%DoctorLiu.num_hands)
print(
```
```python
"DoctorLiu有%s条腿"
```
```python
%DoctorLiu.num_legs)
print(
```
```python
"DoctorLiu有%s眼睛"
```
```python
%DoctorLiu.eyes)
print(
```
```python
"DoctorLiu有%s皮肤"
```
```python
%DoctorLiu.skin)
```
```python
DoctorLiu有2只手
DoctorLiu有2条腿
DoctorLiu有大眼睛
DoctorLiu有白皮肤
```
实例在创建的过程中，也可以对它进行赋值，和普通函数的赋值过程是一样的。

