# startswith和endswith函数 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 16:45:44[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：438








当我们做文本处理的时候，经常要判断一个文本有没有以字串开始，或者结束。
Python为了实现这样的功能提供了两个函数：startswith, endswith




S.startswith(prefix[, start[, end]]) -> bool
如果字符串S以prefix开始，返回True，否则返回False。start和end是两个可以缺省的参数。分别是开始比较的位置和结束比较的位置。这个函数也可以写成S[start:end].startswith(prefix)。
S.endswith(suffix[, start[, end]]) -> bool
如果字符串S以suffix结束，返回True，否者返回False。与startswith类似，这个函数也可以写成S[start:end].endswith(suffix)。start和end仍然是从左数起。
做个实例：
>>> “fish”.startswith(”fi”)
True
>>> “fish”.startswith(”fi”,1)  #数字表示从第几个字符开始，包含0
False
>>> “fish”.endswith(”sh”)
True
>>> “fish”.endswith(”sh”,3)
False




if tarword[1].startswith('n'):   #若是名词

                    count += 5




上面都是使用的例子。




Python的这两个函数有个特别的地方——它的prefix和suffix参数不仅可以是字符串，还都可以是一个元组。只要其中一个成立，就返回True，也就是一种“或”的关系。比如：
if filename.endswith((’.gif’, ‘.jpg’, ‘.tiff’)):
print “%s是一个图片文件”%filename





