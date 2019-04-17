# GNU glibc库中函数执行成功与失败的返回值 - 小灰笔记 - CSDN博客





2017年08月08日 22:38:29[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：187
个人分类：[C																[GNU](https://blog.csdn.net/grey_csdn/article/category/7086457)](https://blog.csdn.net/grey_csdn/article/category/6639462)








       最初学习C语言编程的时候，主函数是可以没有返回值的。工作之后，重新接触的编译基本上都要求主函数有一个int类型的返回值。起初我有些不明白，为什么很多历程中的返回值是0，后来在一个教程中看到有人返回值是一个宏，追本溯源之后才知道0原来是执行成功的返回值。

       具体的宏定义如下：

#defineEXIT_SUCCESS 0

#defineEXIT_FAILURE 1

       上面这是对于主函数的定义，那么对于一般函数呢？比如说，刚刚接触到的access函数。我查找了编译器中的头文件，没有找出一个比较合理的信息。借助搜索引擎，找到了gnu的网页上的解释如下：

The return valueis 0 if the access is permitted, and -1 otherwise. (Inother words, treated as a predicate function, access returns true ifthe requested access is *denied*.)

       从上面的解释可以知道：成功的时候返回0，但是失败的时候返回-1。现在稍微明确一点了，只是还是不清楚是不是这是一个比较共性的方式。或许查看一下这个功能实现的源代码会更能够弄清楚这些，也许执行的成功与失败都是定义好的宏，然后作为函数执行的返回值。从GNU的FTP下载了源代码，找到了相应的函数如下：

/* Copyright (C)1991-2017 Free Software Foundation, Inc.

   This file is part of the GNU C Library.



   The GNU C Library is free software; you canredistribute it and/or

   modify it under the terms of the GNU LesserGeneral Public

   License as published by the Free SoftwareFoundation; either

   version 2.1 of the License, or (at youroption) any later version.



   The GNU C Library is distributed in the hopethat it will be useful,

   but WITHOUT ANY WARRANTY; without even theimplied warranty of

   MERCHANTABILITY or FITNESS FOR A PARTICULARPURPOSE.  See the GNU

   Lesser General Public License for moredetails.



   You should have received a copy of the GNULesser General Public

   License along with the GNU C Library; ifnot, see

   <http://www.gnu.org/licenses/>.  */



#include<errno.h>

#include<stddef.h>

#include<unistd.h>



/* Test for accessto FILE without setting errno.   */

int

__access_noerrno(const char *file, int type)

{

  return -1;

}



/* Test for accessto FILE.  */

int

__access (constchar *file, int type)

{

  if (file == NULL || (type &~(R_OK|W_OK|X_OK|F_OK)) != 0)

    {

      __set_errno (EINVAL);

      return -1;

    }



  __set_errno (ENOSYS);

  return -1;

}

stub_warning(access)



weak_alias(__access, access)

     这下子算是终于死心了，其实这个函数的成功和失败的返回值没有什么宏。这当然还是不能够排除这是大家通用的一个数值对儿，但是至少从规范性上来说，似乎是少了一种强制性的约定。




