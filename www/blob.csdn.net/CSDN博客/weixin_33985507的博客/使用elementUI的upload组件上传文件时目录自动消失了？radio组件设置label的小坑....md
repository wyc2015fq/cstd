# 使用elementUI的upload组件上传文件时目录自动消失了？radio组件设置label的小坑... - weixin_33985507的博客 - CSDN博客
2018年01月31日 10:01:36[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
我是初学者，不对的地方请大家多多指教。
在使用vue和elementUI开发前端界面的过程中使用了upload组件，致点击上传按钮以后文件目录在界面上自动消失了，因为打算文件和表单其他参数一起上传，纳闷了很久，还以为需要把文件数据流缓存到浏览器中云云瞎猜，最后在看elementUI的文档过程中发现设置upload组件参数的：auto-upload=false，可以防止自动上传，还没上传文件就不会在界面上消失了，这个参数默认是为true，auto-upload=true会触发:before-upload="beforeUpload"
![clipboard.png](https://image-static.segmentfault.com/325/211/3252116432-5a71a87ad3c6b_articlex)
![clipboard.png](https://image-static.segmentfault.com/300/799/3007995783-5a72ace5d67da_articlex)
那么如果表单上有其他字段需要和上传文件一起提交怎么办呢？这是我们可以在表单的提交按钮触发的函数
![clipboard.png](https://image-static.segmentfault.com/304/160/3041602353-5a72af2facbbd_articlex)
this.$refs.upload.submit();也会触发beforeUpload，在这个函数里把表单里的数据复制给后台提供的字段就可以了
注意this.$refs.upload.submit();一定要放在params之前，先把上次文件数据流赋值在表单里的字段，在把整个表单转化为json对象params。
而且我发现element组件里有些属性在组件里应用时前面都不用加冒号的？？有些却要这个是为什么？
首先要明白'：'其实是vue里面的v-bind缩写
在element上应用Radio单选框组件对比了一下时发现
![clipboard.png](https://image-static.segmentfault.com/182/335/182335197-5a72b329b584c_articlex)
![clipboard.png](https://image-static.segmentfault.com/381/090/3810903319-5a72b346835d4_articlex)
在项目中应用时
![clipboard.png](https://image-static.segmentfault.com/155/679/1556794802-5a72baa976882_articlex)
报错
![clipboard.png](https://image-static.segmentfault.com/326/193/326193299-5a72bb17ebf75_articlex)
但是当我：label=“3”改为数字时不会报错，但是我就是想传字符串，查看了一下文档
发现当label的值是字符串的时候，不用加：也就是v-bind。如果加了：（v-bind）他会把label里的值识别为变量，而我并未有事先声明这三个变量，所以报错了，因为数字不能充当变量所以不会报错，我的个人理解是这个样子的，去掉引号就不会报错了
![clipboard.png](https://image-static.segmentfault.com/214/033/2140334382-5a72b8772b54e_articlex)
在学习的过程中发现我对框架的底层不太理解，如果原理明白，解决问题和学习框架的效率就回提高很多。
