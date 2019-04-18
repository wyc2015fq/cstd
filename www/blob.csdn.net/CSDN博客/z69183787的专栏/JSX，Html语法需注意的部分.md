# JSX，Html语法需注意的部分 - z69183787的专栏 - CSDN博客
2017年03月15日 17:53:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1615
在JSX中设置style属性与其它的属性不同， 
在JSX中我们通常是通过 `{}` 的方式插入值，但是设置style属性需要
```
{{
 }}
```
 ,否则系统会报错。
```
import classes form './myCss.css'
{/*JSX 中的注释方式*/}
{/*style属性必须使用 {{}} */}
<div style={{width: 20px; height=30px}}>
    { props.text }
</div>
{/*html class属性*/}
<div className="myClass"></div>
{/*html input标签, JSX中所有的标签都必须有闭标签*/}
<input type="text" />
{/*html label标签 */}
<label htmlFor="name" class="mayName"></label>
{/*调用.css 文件中的css属性, mycss 为css文件中的类*/}
<div className={classes['mycss']}></div>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
[](http://blog.csdn.net/HobbitHero/article/details/51925292#)[](http://blog.csdn.net/HobbitHero/article/details/51925292#)[](http://blog.csdn.net/HobbitHero/article/details/51925292#)[](http://blog.csdn.net/HobbitHero/article/details/51925292#)[](http://blog.csdn.net/HobbitHero/article/details/51925292#)[](http://blog.csdn.net/HobbitHero/article/details/51925292#)
