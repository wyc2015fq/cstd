# [MyBatis]诡异的Invalid bound statement (not found)错误 - z69183787的专栏 - CSDN博客
2015年10月06日 19:21:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：122782
自从开始使用Maven管理项目，最近在配置MyBatis的Mapper，在Eclipse上调试时都是正常的，但是最近把项目迁移到 IntelliJ IDEA 上后发现不管是直接用Jetty调试，还是打成war发布均报错。
根据错误提示初步判断是配置的问题，检查的Spring和MyBatis所有配置文件，甚至在SVN上回退了代码，可问题均没有解决；无奈暂时换回了Eclipse继续Coding…
一直怀疑是迁移至IntelliJ IDEA造成的问题，于是自己新建了个Maven项目，调试MyBatis；可以依旧报错…无奈直接打包成War打算手动发布试试…还是不行。无奈解压了War包，发现包结构极为乱套….
在仔细检查了之前的项目后，发现Mapper.xml根本就没打包进去。妈蛋，竟然犯了一个弱智错误！
![IDEA 项目结构](http://b.cdn.rmbz.net/rmbznet/wp-content/uploads/2015/02/20150203110149.png)
根本就没打包到Spring配置的位置。
![IDEA 项目结构](http://b.cdn.rmbz.net/rmbznet/wp-content/uploads/2015/02/20150203110934.png)
Mapper文件应该放在Resources文件夹中！这个我竟然忘了！重新调整结构，问题顺利解决…
**其他原因导致此问题解决参考：**
**1.**
```
```xml
<
```
```xml
mapper
```
```xml
namespace
```
```xml
=
```
```xml
"me.tspace.pm.dao.UserDao"
```
```xml
>
```
```
 mapper的namespace写的不对！！！注意系修改。
**2.**UserDao的方法在UserDao.xml中没有，然后执行UserDao的方法会报此
**3.** UserDao的方法返回值是List<User>,而select元素没有正确配置ResultMap,或者只配置ResultType!
**4. **如果你确认没有以上问题,请任意修改下对应的xml文件,比如删除一个空行,保存.问题解决…
