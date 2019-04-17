# Java代码注释规范 - 博客堂 - CSDN博客





2017年02月15日 14:58:35[最小森林](https://me.csdn.net/u012052268)阅读数：388标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/u012052268/article/category/3058811)










代码注释是架起程序设计者与程序阅读者之间的通信桥梁，最大限度的提高团队开发合作效率。也是程序代码可维护性的重要环节之一。所以我们不是为写注释而写注释。下面说一下我们在诉求网二期开发中使用的代码注释规范，供大家参考下。



# [Eclipse中添加文档注释快捷键](http://www.cnblogs.com/laiboan/p/5517959.html)


例如：

/**

  * @param  

  * @return

  */

快捷键为：选中该类，或者该方法 然后 ALT + SHIFT +J

想更换为其他的快捷键：

Window-->Preferences-->General-->Keys;找到"add javadoc comment"更改自己喜欢的快捷键。





**原则：**

**类、类属性、类方法的注释必须使用 Javadoc 规范，**

**/****

** *内容**

** */**

**使用****javadoc格式，不得使用****// xxx 方式。**



**说明：在 IDE 编辑窗口中， Javadoc 方式会提示相关注释，生成 Javadoc 可以正确输出相应注释 ； 在 IDE 中，工程调用方法时，不进入方法即可悬浮提示方法、参数、返回值的意义，提高阅读效率。**



1、注释形式统一

在整个应用程序中，使用具有一致的标点和结构的样式来构造注释。如果在其它项目中发现它们的注释规范与这份文档不同，按照这份规范写代码，不要试图在既成的规范系统中引入新的规范。

2、注释内容准确简洁

内容要简单、明了、含义准确，防止注释的多义性，错误的注释不但无益反而有害。



**注释条件**：


1、基本注释（必须加）


(a)类（接口）的注释


(b)构造函数的注释


(c)方法的注释


(d)全局变量的注释


(e)字段/属性的注释

 备注：简单的代码做简单注释，注释内容不大于10个字即可，另外，持久化对象或VO对象的getter、setter方法不需加注释。具体的注释格式请参考下面举例。

2、特殊必加注释（必须加）


(a)典型[算法](http://lib.csdn.net/base/datastructure)必须有注释。


(b)在代码不明晰处必须有注释。


(c)在代码修改处加上修改标识的注释。


(d)在循环和逻辑分支组成的代码中加注释。


(e)为他人提供的接口必须加详细注释。

 备注：此类注释格式暂无举例。具体的注释格式自行定义，要求注释内容准确简洁。




**注释格式**：

1、单行(single-line)注释：“//……”

2、块(block)注释：“/*……*/”

3、文档注释：“/**……*/”

4、javadoc 注释标签语法

@author   对类的说明 标明开发该类模块的作者

@version   对类的说明 标明该类模块的版本

@see     对类、属性、方法的说明 参考转向，也就是相关主题

@param    对方法的说明 对方法中某参数的说明

@return   对方法的说明 对方法返回值的说明

@exception  对方法的说明 对方法可能抛出的异常进行说明




**参考举例**：

## 1类、接口注释 

## 例如：

/**

* 类的描述

* **@author** Administrator

* @Time 2012-11-2014:49:01

*

*/

**public****class**Test**extends** Button
 {

……

}

## 2.构造方法注释

例如:

**public****class** Test **extends** Button
 {

/**

   * 构造方法的描述

   * **@param** name

   *       按钮的上显示的文字

   */

**public** Test(String name){

……

  }

}

## 3.方法注释

例如

**public****class** Test **extends** Button
 {

/**

   * 为按钮添加颜色

   ***@param** color

按钮的颜色

*@return

*@exception  (方法有异常的话加)

*** @author** Administrator

* @Time2012-11-20 15:02:29



   */

**public****void**addColor(String
 color){

……

  }

}

## 4.全局变量注释

例如：

**public****final****class** String

**implements**[Java](http://lib.csdn.net/base/javase).io.Serializable,
 Comparable<String>,CharSequence

{

/** The value is used for characterstorage. */

**private****final****char**value[];

/** The offset is the first index of thestorage that is used. */

**private****final****int**offset;

/** The count is the number of charactersin the String. */

**private****final****int**count;

/** Cache the hash code for the string */

**private****int**hash; //
 Default to 0

……


}

## 5.字段/属性注释

例如：

**public****class** EmailBody **implements** Serializable{

**private** String id;

**private** String senderName;//发送人姓名

**private** String title;//不能超过120个中文字符

**private** String content;//邮件正文

**private** String attach;//附件，如果有的话

**private** String totalCount;//总发送人数

**private** String successCount;//成功发送的人数

**private** Integer isDelete;//0不删除 1删除

**private** Date createTime;//目前不支持定时所以创建后即刻发送

**private**Set<EmailList> EmailList;

……

}



