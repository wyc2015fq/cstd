# 小心使用STL中map的[]操作符 - L_Andy的专栏 - CSDN博客

2014年10月30日 11:12:21[卡哥](https://me.csdn.net/L_Andy)阅读数：2055



 一个map就是一个（关键码（key），值（value））对偶的序列，它提供基于关键码的快速提取操作。也就是说，可以用下标运算符[]将关键码作为下标去执行查找，并返回对应的值。因此可以把map的使用方法想象成有特殊下标的数组。在很多时候用下标运算符[]来对map中的元素进行存取是非常方便和简单的；但是，如果map下标运算符[]运用不得当，也会造成意想不到的问题。

    我们知道，C++是不检查下标越界的。用超出数组下标范围的下标去访问数组元素的错误会在运行期出现，很有可能将程序搞崩溃。对于map而言，也没有类似的下标越界概念，但是却有作为下标的关键码（key）在map中不存在的现象。在这种情况下，如果运用不得当，就造成意想不到的问题，而且这种问题是属于比较隐蔽的问题。如果对map的下标运算符[]不是非常了解，将很难发现问题所在。

    比如说有下面的代码片断：

// 定义一个map
map<string, string> mapTelDir;

// 用[]操作符可以方便快速的给map添加内容
mapTelDir["StarLee"] ="13813131313";

// ...

// 修改关键码为LiXing的值
// 注意，这里的关键码在map里面并不存在
if (mapTelDir["LiXing"] =="13913131313")

{

    mapTelDir["LiXing"] ="13513131313";

}

// ...

// 输出
cout <<"The cellhone number of LiXing is: "<< mapTelDir["LiXing"] << endl;

     上面的代码看起来似乎没有任何问题，但是，最后一行的输出的手机号码却永远为空！原因就是在判断语句里面对map的下标运算符[]的错误使用。

    map的下标运算符[]的作用是：将关键码作为下标去执行查找，并返回对应的值；如果不存在这个关键码，就将一个具有该关键码和值类型的默认值的项插入这个map。

    对于上面的代码来说，if (mapTelDir["LiXing"] == "13913131313")这行代码首先去查询map中是否有关键码为LiXing的项，结果发现没有该项，于是就向map中插入了一项<"LiXing", "">（sting的默认值为空字符串）。这就造成if语句的判断结果永远为false，那样就不可能修改修改关键码为LiXing的值。

    为了更好的说明这个问题，可以看一下map下标运算符[]的实现代码：

// 下面的代码摘自VS2005中的..vcincludemap
mapped_type&operator[](const key_type& _Keyval)

    {    // find element matching _Keyval or insert with default mapped
        iterator _Where =this->lower_bound(_Keyval);
if (_Where ==this->end()
||this->comp(_Keyval, this->_Key(_Where._Mynode())))

            _Where =this->insert(_Where,

                value_type(_Keyval, mapped_type()));
return ((*_Where).second);

    }

    map的下标操作符[]使用上面的方法定义，使向map里插入值和使用关键码来访问相应的值变得非常方便而快速。虽然在使用的时候会出现有人写出类似上面那样的错误代码，但是这是程序员的错误，因为写出正确的代码是程序员的责任。这样做很符合C++的风格：非常灵活，但是要小心使用。

