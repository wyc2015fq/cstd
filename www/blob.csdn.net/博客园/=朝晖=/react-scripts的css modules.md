# react-scripts的css modules - =朝晖= - 博客园
# [react-scripts的css modules](https://www.cnblogs.com/dhcn/p/9848601.html)
    用create-react-app创建的项目基于react-scripts内建支持css modules，需要把css文件名改成filename.module.css，导入方式需要改成：
```
import styles from './Button.module.css'; // Import css modules stylesheet as styles
```
   class定义方式改成了：
```
<button className={styles.error}>Error Button</button>
```
    编译后这个error class被唯一化：
```
<button class="Button_error_ax7yz"></div>
```
    对比其他web框架的CSS组件隔离渲染，这个方案的弊病：导入语句和html代码都是强侵入。还有一个问题当文件名中间加了.module以后，就必须用上面的和导入方式，用styles.className方式定义class了,否则css Modules编译完，不加编译后处理的classname就无效了，这可真是强耦合的一个糟糕设计，just fro implemention。

