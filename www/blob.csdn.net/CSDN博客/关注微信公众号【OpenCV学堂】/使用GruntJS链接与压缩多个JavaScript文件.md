# 使用GruntJS链接与压缩多个JavaScript文件 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年08月01日 17:17:27[gloomyfish](https://me.csdn.net/jia20003)阅读数：4445
个人分类：[JavaScript开发](https://blog.csdn.net/jia20003/article/category/1471265)









使用GruntJS链接与压缩多个JavaScript文件

自己写了个简单的HTML5 Canvas的图表库，可以支持饼图，折线图，散点图，盒子图

柱状图，同时支持鼠标提示，绘制过程动画效果等。最终我想把这些多个JS文件变成

一个JS文件发布出去，于是我的问题来啦，怎么把这些JS文件搞成一个啊，群里有个

朋友告诉我，GruntJS – JavaScript多文件编译，风格检查，链接与压缩神器。Google了一

把终于帮我完成这个任务，算是入门，分享一下过程。



一什么是GruntJS

不想翻译英文，自己看它的网站吧->http://gruntjs.com/

二：安装与运行

它的官方教程说的不是很清楚，有点让第一次看的人云里雾里的。我总结一下，GruntJS

是基于与依赖服务器node.js的。所以首先第一步是下载并安装node.js,下载地址：

[http://nodejs.org/download/](http://nodejs.org/download/)



第二步：运行安装grunt命令行工具– 目的是为了使用grunt命令
只有在windows的命令行窗口中运行：npm install -g grunt-cli即可。
更具体的解释参见这里：[http://gruntjs.com/getting-started](http://gruntjs.com/getting-started)



第三步：在项目的根目录创建project.json与Gruntfile.js两个文件

因为grunt的task运行要依赖于这两个文件。

其中创建project.json文件方法可以通过命令行实现：nmp init我创建project.json

内容如下：

```java
{

 "name": "fishchart",

 "version": "0.0.1",

 "description": "html5 canvas chart library",

 "author": "zhigang",

 "license": "BSD",

 "devDependencies": {

   "grunt": "~0.4.1",

   "grunt-contrib-uglify": "~0.2.2",

   "grunt-contrib-jshint": "~0.6.2",

   "grunt-contrib-concat": "~0.3.0"

  }

}
```

使用命令创建时候，如果你不知道写什么直接回车跳过即可。


三： 安装与使用Grunt Plug-in完成javascript文件链接与压缩

1.  安装javascript文件链接插件支持

npm install grunt-contrib-concat --save-dev
2. 安装javascript文件压缩插件支持

npm install grunt-contrib-uglify --save-dev

3. 在Gruntfile.js文件中配置选项，加载与定义task



```java
module.exports = function(grunt) {
 
    grunt.initConfig({
 
        //our JSHint options
        jshint: {
            all: ['main.js'] //files to lint
        },
 
        //our concat options
        concat: {
            options: {
                separator: ';' //separates scripts
            },
            dist: {
                src: ['js/*.js', 'js/**/*.js'], //Grunt mini match for your scripts to concatenate
                dest: 'js/fishchart_v0.0.1.js' //where to output the script
            }
        },
 
        //our uglify options
        uglify: {
            js: {
                files: {
                    'js/fishchart_v0.0.1.js': ['js/fishchart_v0.0.1.js'] //save over the newly created script
                }
            }
        }
 
    });
 
    //load our tasks
    grunt.loadNpmTasks('grunt-contrib-jshint');
    grunt.loadNpmTasks('grunt-contrib-concat');
    grunt.loadNpmTasks('grunt-contrib-uglify');
 
    // default tasks to run
    // grunt.registerTask('default', ['jshint', 'concat', 'uglify']);
	grunt.registerTask('development', ['jshint']);
	grunt.registerTask('production', ['jshint', 'concat', 'uglify']);
	}
```


四：运行结果

![](https://img-blog.csdn.net/20130801171525187)

最后还想赞一下，这个东西太好用啦！




