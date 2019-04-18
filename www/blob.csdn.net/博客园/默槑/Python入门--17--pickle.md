# Python入门--17--pickle - 默槑 - 博客园







# [Python入门--17--pickle](https://www.cnblogs.com/modaidai/p/7290582.html)





pickle模块，实际用途也很简单、单一，主要用于保存列表(list)，元祖(Tuple)，字典(dictionary)

　　　　　　当然，一定是大的列表、字典什么的，成千上万行的字典、列表，才能凸显出它的用武之地

实例

import pickle
my_list=['123','3.14','ee']　　　　　　　　　　#创建一个list
pickle_file=open('my_pickle1','wb')　　　　　  #以二进制的方式打开文件
pickle.dump(my_list,pickle_file)　　　　　　　 #使用pickle包中的dump函数把列表中的数据放入文件中
pickle_file.close() 　　　　　　　　　　　　　#关闭文件

pickle_file=open('my_pickle1','rb')　　　　　　#再以二进制的形式打开文件
my_list2=pickle.load(pickle_file)　　　　　　　#使用pickle包中的load函数把数据赋值给my_list2
print(my_list2)　　　　　　　　　　　　　　　　　　　　　　












