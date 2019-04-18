# 手牵手，我们一起搭建VuePress博客 - weixin_33985507的博客 - CSDN博客
2019年01月30日 18:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：20
> 
平时Google搜问题的时候，很多方案都是链接到博客上的，有简书、博客园、思否等等，当然，还有很多个人搭建的博客，风格各异。慢慢的，自己又有了自搭博客的想法，一是作为自己工作踩坑的学习记录，二者万一哪天也能帮到别人解决了什么问题，岂不美哉
## 选定VuePress
前后利用工作之余想了几种方案，也考虑过Hexo，也是很火的博客搭建工具，但又想借此机会，学习一下新技术，VuePress是我所在一个技术交流群里一位大佬推荐的，我了解之后，确实很喜欢， 它有以下几个特点：
- 轻易上手，配置简单
- Vue驱动，享用Vue+webpack开发环境，可以自定义Vue组件
- 性能高效
> 
其实上面的优点官网都有介绍，我比较认可，再赘述一遍
详情请浏览[VuePress中文网](http://caibaojian.com/vuepress/)
## 开始安装
```
# 全局安装 VuePress
yarn global add vuepress # 或者用 npm install -g vuepress
```
为了方便省事，免去重新构建整个项目的时间，可以从官网demo那里clone下来整个项目，到后面根据自己的情况再去修改
```
# cd 到你要保存blog的文件夹， 然后clone
git clone git@github.com:docschina/vuepress.git
```
## 启动调试
```
cd vuepress    //进入项目根目录
yarn           //安装依赖
yarn dev       //启动调试服务
```
当终端中出现 `VuePress dev server listening at http://localhost:8080/`
就说明项目已经正常启动调试了，你可以在浏览器输入`http://localhost:8080/`预览整个博客项目
![1779431-457efa2c2de242d6.jpg](https://upload-images.jianshu.io/upload_images/1779431-457efa2c2de242d6.jpg)
Blog首页
## 修改项目结构
你看到的项目结构应该是下面这样的
```
.
├── .vuepress
│   ├── components  
│   ├── config.js   // 网站基础配置文件，主要修改的文件
│   └── public      // 存放网站静态资源，例如图片
├── config
│   └── README.md          
├── default-theme-config
│   └── README.md          
├── guide                  
│   ├── README.md
│   ├── assets.md
│   ├── basic-config.md
│   ├── custom-themes.md
│   ├── deploy.md
│   ├── getting-started.md
│   ├── i18n.md
│   ├── markdown.md
│   └── using-vue.md
├── zh                      // 我们需要的中文配置
|   ├── README.md           // 首页配置文件
│   ├── config
│   ├── default-theme-config
│   └── guide
└── README.md               // 首页配置文件
```
因为我们主要是中文界面，不考虑英文，所以，可以直接把`zh`文件夹里的所有文件移动到上层目录，覆盖原有的`guide`、 `default-theme-config`、`config`,以及首页配置文件`README.md`
完成之后应该是这样的目录结构
```
.
├── .vuepress
│   ├── components        
│   ├── config.js         
│   └── public            
├── config
│   └── README.md            // 可删除
├── default-theme-config     // 这个也可以删，或者改成自己需要的
│   └── README.md
├── guide                    // 也可删除，建议根据这个修改成自己的blog内容
│    ├── README.md
│    ├── assets.md
│    ├── basic-config.md
│    ├── custom-themes.md
│    ├── deploy.md
│    ├── getting-started.md
│    ├── i18n.md
│    ├── markdown.md
│    └── using-vue.md
├── README.md
```
## 网站基本配置
在`.vuepress`下的`config.js`里，包括了网站需要设置的标题、描述等基本信息
简单列举一些，因为没有留原来的，附上的是我修改后的
```
module.exports = {
  dest: 'vuepress', // yarn 打包生成的文件地址，在根目录下会有一个vuepress文件夹
  locales: {    // 语言配置
    '/': {
      lang: 'zh-CN',
      title: 'devZhang',    // 网站标题
      description: 'Something Good Will Be Happen!' // 网站描述，显示在标题下面
    }
  },
  head: [   // 额外添加到当前页面 <head>标签里的标签
    ['link', { rel: 'icon', href: `/logo.png` }],
    ['link', { rel: 'manifest', href: '/manifest.json' }],
    ['meta', { name: 'theme-color', content: '#3eaf7c' }],
    ['meta', { name: 'apple-mobile-web-app-capable', content: 'yes' }],
    ['meta', { name: 'apple-mobile-web-app-status-bar-style', content: 'black' }],
    ['link', { rel: 'apple-touch-icon', href: `/icons/apple-touch-icon-152x152.png` }],
    ['link', { rel: 'mask-icon', href: '/icons/safari-pinned-tab.svg', color: '#3eaf7c' }],
    ['meta', { name: 'msapplication-TileImage', content: '/icons/msapplication-icon-144x144.png' }],
    ['meta', { name: 'msapplication-TileColor', content: '#000000' }]
  ],
  serviceWorker: true,      // VuePress支持PWA配置
  theme: 'vue',             // 主题文件
```
通过上面的`serviceWorker`配置，配合`public`里的`manifest.json`配置`icons`图标使用。
## 配置网站首页
在`.docs`下的`README.md`就是网站首页配置文件，以下是我修改保留的东西，把没用的都删了
```
---
home: true                  // 是否启用首页默认布局
heroImage: /alone.png   //  首页显示的图片地址
actionText: Go Blog →   //  首页按钮的文字
actionLink: /js/            //  点击首页按钮后链接的路径
---
```
我的首页效果展示：
![1779431-9eb9a5e022aacccd.jpg](https://upload-images.jianshu.io/upload_images/1779431-9eb9a5e022aacccd.jpg)
image
## 配置网站结构
下面是我在官方Demo基础上修改后的网站结构，可供参考
```
themeConfig: {
    repo: 'dvxiaofan',  // github名称，可以加参数(/项目名)，直接到项目。 只写用户名就会直接到你github首页
    editLinks: false,   // 每篇文章底部是否显示查看原文和编辑， 不需要，就设置为false
    docsDir: 'docs',    
    locales: {      //  网站顶部的导航栏配置
      '/': {
        nav: [
          {
            text: 'Web前端',  // 显示名字
            items: [            // 下面有二级目录，就这样设置
                    {
                text: 'JS',
                link: '/js/'
                },
                {
                 text: 'CSS',
                 link: '/css/'
               },
               {
                 text: 'HTML',
                 link: '/html/'
               },
                    ]
                    },
                    {
                        text: '轮子',
                        items: [
                            {
                            text: 'Echarts',
                            link: '/echarts/'
                            },
                        {
                            text: 'Vue',
                            link: '/vue/'
                        },              
                        ]
                    },
          {
            text: '生活随笔',
            link: '/lifes/'
          },
          {
            text: '关于',
            link: '/about/'
          }
        ],
        sidebar: {  // 侧边栏配置
                    '/js/': jsSidebarConfig('JS相关'),
                    
        }
      }
    }
  }
}
// 侧边栏具体配置
function jsSidebarConfig (title) {
  return [
    {
      title,
      collapsable: false,   // 是否可折叠
      children: [               // 对应每一篇文章，省略了`.md`扩展名
        '',                     // 默认对应`README.md`
        'js01',
        'js02',
        'js03',
        'js04',
        'js05',
      ]
    }
  ]
}
```
网站顶部目录效果展示
![1779431-c6e68782b8841999.jpg](https://upload-images.jianshu.io/upload_images/1779431-c6e68782b8841999.jpg)
image
## 部署上线
在运行`yarn build`之后生成的是静态页面，我们需要把打包生成的`vuepress`文件夹中的内容托管到服务器，这样才能真能的通过网络浏览我们的博客。
暂时可以用GitHub来托管，省去了自己花钱买服务器和域名的繁琐。
我们需要在GitHub上建立两个仓库， 一个是平时新增维护整个博客用的， 一个用来托管博客打包后的文件
比如，我建立了一个`MyBlog` 和 `dvxiaofan.github.io`
> 
注意
这个`dvxiaofan.github.io`里前面的名字是你的GitHub用户名， 必须一模一样！我GitHub名是`dvxiaofan`,所以项目名就是前面的样子
第一个仓库就放整个项目，具体git操作不再赘述。
## 编写发布脚本
为了方便以后发布使用，打开项目跟目录下的 `scripts`文件夹里的 `deploy-gh.sh`文件， 修改成下面这样
```
cd vuepress     // 进入到打包好的输出文件夹
git init            
git add -A
git commit -m 'deploy'
git push -f git@github.com:dvxiaofan/dvxiaofan.github.io.git master // 代码上传到前面建立的第二个仓库的master分支， 你需要替换成自己的
```
## 查看成果
把上面的文章保存，在终端里cd到整个项目目录下，然后执行`yarn deploy-gh`， 脚本就会开始执行，知道最后结束，代码提交部署成功， 你就可以输入托管博客的查看地址`https://dvxiaofan.github.io`， 没问题的话，你应该会看到自己的博客了。
## 小结
这次就先写到这里，后期有时间会把博客部署到自己的域名上去，到时候再来更新文章
