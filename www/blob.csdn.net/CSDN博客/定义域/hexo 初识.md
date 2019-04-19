# hexo 初识 - 定义域 - CSDN博客
2016年02月26日 15:25:39[定义域](https://me.csdn.net/u010278882)阅读数：3409
# 什么是 hexo
hexo 是博客框架，将支持的类型的文件转换成静态Web页面（html+css+javascript）。
# 发布一篇博客
## 工具安装
- 下载并安装[git](http://git-scm.com/download/win)
- 下载并安装[Node.js](https://nodejs.org/en/)
- 下载并安装hexo
`cmd`打开命令行窗口, 使用[npm](http://blog.csdn.net/u010278882/article/details/50748325)安装`hexo`
`npm install -g hexo-cli // 全局安装 hexo 模块`
## 创建博客目录
继续执行如下命令
```
hexo init <folder> // 初始化, 在指定目录中创建文件目录
cd <folder>
npm install // 安装 <folder> 中的全部项目依赖
```
## 创建一篇新文章
```
hexo new [layout] <title> // 标题中包含空格需要用引号将标题引起来
// 执行该命令后在`source/_posts`目录下生成文件
```
然后就可以打开该文件进行编辑。
## 生成博客页面
```
hexo generate
// 执行该命令后会生成`public`目录
```
## 本地查看
```
hexo server
// 执行命令后启动服务，访问`http://localhost:4000/`可查看博客
```
## 上传到 github
### 首次使用配置
- 安装 `hexo-deplorer-git`（执行如下指令）
`npm install hexo-deplorer-git --save`- 修改 `_config.yml` 中的`#deploy` 配置
```
deploy:
    type: git
    repo: <git-repo-url>
    branch: [branch]
    message: [message]
```
> 
如何获取 git-repo-url
a. 打开`github.com`，创建一个名为`username.github.io` repo。则 repo 名为 `http://github.com/username/username.github.io`。下面是手动提交站点资源。也可以直接在 `_config.yml`中配置。
b. 拉取项目
`git clone https://github.com/username/username.github.io`
c. 切换到项目目录，将站点资源文件放置到该目录
`cd username.github.io`
d. 提交项目
```
git add --all
git commit
git push -u origin master
```
# 自定义
> 
hexo 目录结构
`<folder>` 中生成的目录结构和含义如下
`_config.yml` 配置信息 
`package.json` hexo 博客框架模块信息 
`source` 存放博客源文件和其他文件。`Markdown`和`HTML`文件会被解析并放到`public`目录（解析时生成），其他类型文件/文件夹将会被复制到 `public` 目录下。对于以”*“开头的文件/文件夹，除`_post`文件夹外，其他以”*“开头的文件/文件夹和隐藏文件将被忽略。 
`themes` 生成页面时，根据该文件夹中的主题来生成某个主题的页面。 
`scaffolds` 生成页面时，根据该文件夹中的模板来生成页面。
## 文章引用资源
### 哪些文件作为资源?
`source`目录下除了文章和以”_”为前缀的文件/文件夹（”_post”目录除外）的所有文件都属于资源。
### 文章中如何引用资源?
方式1：不与文章相关的资源管理 
可以在 `source` 目录下新建用于存放资源的目录。引用通过绝对路径的`![](/folder/res.png)`方式来引用资源。（注意：`source`目录相当于发布后的站点根目录）。
方式2：与文章相关的资源管理 
将`_config.yml` 中的`post_asset_folder` 设置为 `true`。在生成过程中会自动在`source`目录下生成与文章同名的目录，可以将资源存放于该目录。引用通过相对路径`![](res.png)`的方式来应用资源。（注意：这时页面和资源位于同一目录）。
### 问题：归档页中的资源没有正确引用
最好不使用`Markdown`的方式来引用资源，使用`{% asset_img example.jpg This is an example image %}`来引用资源。使用`Hexo 3`中的新的标签插件语法能够保证在归档页中资源也得到正确的引用。
资源引用插件语法参考
- 文章 `{% post_path link %}`
- 资源 
- 资源 `{% asset_path path %}`
- 图片 `{% asset_img path [title] %}`
- 链接 `{% asset_link path [title] %}`
- 代码 
```
{% codeblock lang: java Hello.java http://www.baidu.com Hello.java %} 
System.out.println("Hello"); 
{% endcodblock %}
```
- Gist 
`{% gist gist_id [filename] %}`
- 引言 
```
{% blockquote [author[, source]] [link] [source_link_title] %} 
content 
{% endblockquote %}
```
### 参考
- [标签插件](https://hexo.io/zh-cn/docs/tag-plugins.html)
- [资源文件夹](https://hexo.io/zh-cn/docs/asset-folders.html)
## 主题
### 使用 NexT 主题
[NexT](http://theme-next.iissnan.com/five-minutes-setup.html)
中文作者，支持程度好，所以选用
### 参考
- [Hexo Themes 主题列表](https://hexo.io/themes/)
- [NexT 文档](http://theme-next.iissnan.com/)
# 参考
- [Hexo 文档](https://hexo.io/zh-cn/docs/)
- [Github Pages](https://pages.github.com/)
