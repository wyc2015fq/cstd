
# 常用HTML标签3：表单 - 尹成的技术博客 - CSDN博客

2018年11月14日 16:13:58[尹成](https://me.csdn.net/yincheng01)阅读数：57个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



html表单
表单用于搜集不同类型的用户输入，表单由不同类型的标签组成，实现一个特定功能的表单区域（比如：注册），首先应该用`<form>`标签来定义表单区域整体，在此标签中再使用不同的表单控件来实现不同类型的信息输入，具体实现及注释可参照以下伪代码：
`<!-- form定义一个表单区域,action属性定义表单数据提交的地址，method属性定义提交的方式。   -->
<form action="http://www..." method="get">
<!-- label标签定义表单控件的文字标注，input类型为text定义了一个单行文本输入框  -->
<p>
<label>姓名：</label><input type="text" name="username" />
</p>
<!-- input类型为password定义了一个密码输入框  -->
<p>
<label>密码：</label><input type="password" name="password" />
</p>
<!-- input类型为radio定义了单选框  -->
<p>
<label>性别：</label>
<input type="radio" name="gender" value="0" /> 男
<input type="radio" name="gender" value="1" /> 女
</p>
<!-- input类型为checkbox定义了单选框  -->
<p>
<label>爱好：</label>
<input type="checkbox" name="like" value="sing" /> 唱歌
<input type="checkbox" name="like" value="run" /> 跑步
<input type="checkbox" name="like" value="swiming" /> 游泳
</p>
<!-- input类型为file定义上传照片或文件等资源  -->
<p>
<label>照片：</label>
<input type="file" name="person_pic">
</p>
<!-- textarea定义多行文本输入  -->
<p>
<label>个人描述：</label>
<textarea name="about"></textarea>
</p>
<!-- select定义下拉列表选择  -->
<p>
<label>籍贯：</label>
<select name="site">
    <option value="0">北京</option>
    <option value="1">上海</option>
    <option value="2">广州</option>
    <option value="3">深圳</option>
</select>
</p>
<!-- input类型为submit定义提交按钮，还可以用图片控件代替submit按钮提交，一般会导致提交两次，不建议使用。如：
     <input type="image" src="xxx.gif">
-->
<p>
<input type="submit" name="" value="提交">
<!-- input类型为reset定义重置按钮  -->
<input type="reset" name="" value="重置">
</p>
</form>`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

