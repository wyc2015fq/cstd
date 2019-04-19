# 角色表情Blendshape重定向方法 - 逍遥剑客 - CSDN博客
2019年01月31日 18:16:40[xoyojank](https://me.csdn.net/xoyojank)阅读数：216
注: 文章是美术同事写的, 我只是放这里记录一下
# 问题重点
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190131181337105.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r,size_16,color_FFFFFF,t_70)
需要将已有角色脸型模型的所有表情Blendshapes精确的重定向到另一个五官位置不一样，模型布线也不一样的角色上。首先因为两套角色五官不一样，所以单一通过Wrap变形器无法实现精确的重定向。其次，由于两个面部模型布线也不一样，也不能通过Blendshape直接实现变形。
# 解决办法
## （1）：首先将案例脸型用晶格变形来匹配我们目标角色的五官和脸型。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190131181350624.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r,size_16,color_FFFFFF,t_70)
晶格一定要设置到所有控制范围，否则超出晶格范围的点会失效
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190131181355665.png)
## （2）：将我们角色模型的五官模型包裹在脸部模型上。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190131181400627.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r,size_16,color_FFFFFF,t_70)
## （3）：将我们角色模型的五官模型包裹在脸部模型上。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190131181405301.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190131181410775.png)
## （4）：下面我们就可以通过案例模型来驱动角色模型了。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190131181415665.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r,size_16,color_FFFFFF,t_70)
## （5）：在案例脸对应Morph下把我们角色脸模型复制删除历史，存储为以后要用的shapes。（此步骤后期考虑脚本化操作）
## （6）：对于一些比较极端的表情Shapes需要手动修复一些点。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019013118142056.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r,size_16,color_FFFFFF,t_70)
# 相关文章
[Morph动画的转移](https://blog.csdn.net/xoyojank/article/details/73822815)
