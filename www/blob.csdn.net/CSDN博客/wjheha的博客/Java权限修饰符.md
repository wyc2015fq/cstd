# Java权限修饰符 - wjheha的博客 - CSDN博客
2017年10月16日 19:37:37[wjheha](https://me.csdn.net/wjheha)阅读数：81标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java/Java Web](https://blog.csdn.net/wjheha/article/category/6775424)
Java访问控制权限也就是针对于封装做了一个总结，本次来记录四种访问控制权限：
|编号|范围|private|default|protected|public|
|----|----|----|----|----|----|
|1|在同一类中|T|T|T|T|
|2|在同一包的不同类||T|T|T|
|3|在不同包的子类|||T|T|
|4|在不同包的非子类||||T|
Java的封装性是以private、protected、default三种权限的定义。
对于权限的选择，给出以下建议：
- 声明属性就使用private
- 声明方法就使用public
关于一些命名要求：
- 类名称要求每一个单词的首字母大写，其余单词小写，例如：StudentInfo
- 属性名称第一个单词字母小写，而后每个单词首字母大写，例如：studentName
- 方法名称第一个单词字母小写，而后每个单词首字母大写，例如:getName()
- 常量名称使用大写字母表示，例如：MSG
- 包名称使用小写字母，例如：cn.wj.heha
