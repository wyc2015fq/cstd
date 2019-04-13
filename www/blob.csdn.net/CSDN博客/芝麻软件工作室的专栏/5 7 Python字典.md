
# 5.7 Python字典 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月02日 06:50:59[seven-soft](https://me.csdn.net/softn)阅读数：191个人分类：[Python																](https://blog.csdn.net/softn/article/category/6290759)



字典是Python 语言中唯一的映射类型。映射类型对象里哈希值(键) 和指向的对象(值)是一对多的关系。
字典就像一个地址簿，在那里你只通过知道他/她的名字，就可以找到地址或联系人详细信息。也就是说，我们使用键*(姓名)与值**(细节)相联系。注意，键必须是独一无二的，就像如果有两个完全相同的名字的人，你无法找到正确的信息。
注意，字典的关键字你只能使用不可变的对象（比如字符串），你可以使用不可变或可变的对象作为字典的值。这基本上意味着，简单地说，对于键你只能使用简单对象。
在字典中的一对键和值是通过使用冒号指定的，如，d = {key1 : value1, key2 : value2 }。注意，键值对用冒号分隔，彼此之间以逗号分隔，所有这些都是包含在一对大括号中。
记住，在字典中键-值对不以任何方式排序。如果你想要一个特定的顺序，那么你将不得不在使用前自己排序。
你将要使用的字典是dict类的对象或实例。\# 'ab'是英文address　book(地址簿)的首个字母
ab={'Swaroop':'swaroop@swaroopch.com',
'Larry':'larry@wall.org',
'Matsumoto':'matz@ruby-lang.org',
'Spammer':'spammer@hotmail.com'
}
print("Swaroop的地址是",ab['Swaroop'])
\# 删除一个键-值对
delab['Spammer']
print('
地址薄中有{0}个联系人
'.format(len(ab)))
forname,addressinab.items():
print('联系人 {0} 的地址是 {1}'.format(name,address))
\# 添加一个键-值对
ab['Guido']='guido@python.org'
if'Guido'inab:
print("
Guido的地址是", ab['Guido'])

输出：
Swaroop的地址是 swaroop@swaroopch.com
地址薄中有 3 个联系人
联系人 Larry 的地址是 larry@wall.org
联系人 Swaroop 的地址是 swaroop@swaroopch.com
联系人 Matsumoto 的地址是 matz@ruby-lang.org
Guido的地址是 guido@python.org
它是如何工作的：
我们使用已经讨论过的符号创建字典ab。然后我们通过使用在列表和元组中讨论过的索引操作符－－指定关键字来访问键-值对，遵守简单的语法。
我们可以使用我们的老朋友——del语句删除键值对，我们简单地指定字典和要删除的关键字的索引操作符，并将它传递给del语句。对于这个操作，没有必要知道对应于关键字的值。
接下来，我们我们使用字典的items方法，访问字典的每个键-值对的。它返回一个元组的列表，每个元组包含一对值--关键字及紧随其后的值。我们检索这对值并使用for..in循环为每一对分配给相应的变量name和address，然后在for块中打印这些值。
我们可以通过简单地使用索引操作符来访问一个键并分配值的方式添加新的键值对，像上面的例子中我们所做的添加Guido。
我们可以使用in操作符来检查一个键值对是否存在。
字典dict类的列表方法，请看help(dict)。
关键字参数和字典
有一点不同需要注意，如果你在您已经在使用字典的函数中使用关键字参数，只是这样想，这个键值对是在函数定义的参数列表中指定的，而当你在函数中访问变量，它只是访问字典的一个键(在编译器设计术语中称为符号表)。

