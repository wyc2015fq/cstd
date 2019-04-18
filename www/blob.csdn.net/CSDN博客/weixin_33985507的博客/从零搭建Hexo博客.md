# 从零搭建Hexo博客 - weixin_33985507的博客 - CSDN博客
2017年12月04日 22:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
## 前言
一直想搭建一个Next主题的Hexo博客，最近有点时间，肝吧！
目标是搭建出[Next作者大人那样的博客]
![2535326-aca20dc41ca9c95e.png](https://upload-images.jianshu.io/upload_images/2535326-aca20dc41ca9c95e.png)
image.png
## 初始化工程
```
yarn global add hexo-cli
hexo init my-blog
cd my-blog
hexo server # 此时打开浏览器访问 localhost:4000 就可以看到默认主题了
```
## 下载Next主题
在[Releases · iissnan/hexo-theme-next · GitHub](https://link.jianshu.com?t=https://github.com/iissnan/hexo-theme-next/releases)上找到最新的发布版本
```
cd my-blog/themes
curl -O "https://codeload.github.com/iissnan/hexo-theme-next/tar.gz/v5.1.3"
tar zxvf v5.1.3
mv hexo-theme-next-5.1.3 next
rm -f v5.1.3
```
## 修改Next主题
```
cd my-blog
vim themes/next/_config.yml
```
```
menu:       # menu里开启tag，archives
  home: / || home
  tags: /tags/ || tags
  archives: /archives/ || archive
auto_excerpt:   # 不现实全文
  enable: true  
  length: 150
scheme: Pisces  # 主题
avatar: /images/avatar.jpg  # 头像
```
## 写文章
```
cd my-blog
hexo new 从零搭建Hexo博客
```
执行完成后会发现 `source/_posts` 文件夹下会多出一个 `从零搭建Hexo博客.md` 在里面编辑内容即可。如果内容中有图片，将图片保存到 `source/images` 文件夹中，然后通过类似于 `[图片上传失败...(image-7352e5-1512397796194)]` 的方法访问它们。
## 开启标签功能
```
cd my-blog
hexo new page "tags"    # 创建tags页面
vim source/tags/index.md    # 编辑该页面
```
```
---
title: Tagcloud
date: 2017-12-04 21:40:21
type: "tags"
comments: false     # tags页面关闭标签功能
---
```
同时在 `从零搭建Hexo博客.md` 的头部写入 `tags: [Hexo,NexT]`
ps: 关于 tags 有个小坑，就是如果写错了 tag ，更正后，错误到 tag 不会被清除，此时需要执行 `hexo clean` 命令来清空错误到tag
## 部署
部署到 github page 上去
```
cd my-blog
yarn add hexo-deployer-git
vim _config.yml
```
编辑配置文件
```
deploy:
  type: git
  repo: https://github.com/zhanghongnian/zhanghongnian.github.io
  branch: 
  message:
```
然后依次执行
```
hexo g  # 生成静态文件
hexo d  # 发布到 github page 上去
```
此时访问 `https://zhanghongnian.github.io/` 发现已经有内容了，大功告成，Cheers！
## 后续目标
1.集成第三方服务   [第三方服务集成 - NexT 使用文档](https://link.jianshu.com?t=http://theme-next.iissnan.com/third-party-services.html#algolia-search)
- 搜索服务
- 评论功能
- 阅读次数
2.live 2d  [GitHub - EYHN/hexo-helper-live2d: Add the Sseexxyyy live2d to your hexo!](https://link.jianshu.com?t=https://github.com/EYHN/hexo-helper-live2d)
- live 2d.
## 参考链接
[建站 | Hexo](https://link.jianshu.com?t=https://hexo.io/zh-cn/docs/setup.html)
[开始使用 - NexT 使用文档](https://link.jianshu.com?t=http://theme-next.iissnan.com/getting-started.html)
[如何开启tags功能](https://link.jianshu.com?t=https://www.zhihu.com/question/29017171)
[github pages](https://link.jianshu.com?t=https://pages.github.com/)
