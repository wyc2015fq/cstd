# Github秘籍 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月11日 15:19:35[boonya](https://me.csdn.net/boonya)阅读数：500标签：[github](https://so.csdn.net/so/search/s.do?q=github&t=blog)
个人分类：[Git/GitHub/SVN/TFS](https://blog.csdn.net/boonya/article/category/6068199)









如需更多内容请访问：[https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md)


# GitHub秘籍 ![Awesome](https://camo.githubusercontent.com/13c4e50d88df7178ae1882a203ed57b641674f94/68747470733a2f2f63646e2e7261776769742e636f6d2f73696e647265736f726875732f617765736f6d652f643733303566333864323966656437386661383536353265336136336531353464643865383832392f6d656469612f62616467652e737667)

Git 和 Github 秘籍，灵感来自于 [Zach Holman](https://github.com/holman) 在 2012 年 Aloha Ruby Conference 和 2013 年 WDCNZ 上所做的演讲：[Git and GitHub
 Secrets](http://www.confreaks.com/videos/1229-aloharuby2012-git-and-github-secrets)([slides](https://speakerdeck.com/holman/git-and-github-secrets)) 和
[More Git and GitHub Secrets](https://vimeo.com/72955426)([slides](https://speakerdeck.com/holman/more-git-and-github-secrets))。

*其他语言版本: [English](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.md), [한국어](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.ko.md), [日本語](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.ja.md), [简体中文](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md), [正體中文](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-tw.md).*

# [](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E7%9B%AE%E5%BD%95)目录
- [GitHub](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#github)- [不比较空白字符](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E4%B8%8D%E6%AF%94%E8%BE%83%E7%A9%BA%E7%99%BD%E5%AD%97%E7%AC%A6)
- [调整 Tab 字符所代表的空格数](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E8%B0%83%E6%95%B4-tab-%E5%AD%97%E7%AC%A6%E6%89%80%E4%BB%A3%E8%A1%A8%E7%9A%84%E7%A9%BA%E6%A0%BC%E6%95%B0)
- [查看某个用户的 Commit 历史](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E6%9F%A5%E7%9C%8B%E6%9F%90%E4%B8%AA%E7%94%A8%E6%88%B7%E7%9A%84-commit-%E5%8E%86%E5%8F%B2)
- [仓库克隆](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E4%BB%93%E5%BA%93%E5%85%8B%E9%9A%86)
- [分支](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E5%88%86%E6%94%AF)- [将某个分支与其他所有分支进行对比](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E5%B0%86%E6%9F%90%E4%B8%AA%E5%88%86%E6%94%AF%E4%B8%8E%E5%85%B6%E4%BB%96%E6%89%80%E6%9C%89%E5%88%86%E6%94%AF%E8%BF%9B%E8%A1%8C%E5%AF%B9%E6%AF%94)
- [比较分支](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E6%AF%94%E8%BE%83%E5%88%86%E6%94%AF)
- [比较不同派生库的分支](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E6%AF%94%E8%BE%83%E4%B8%8D%E5%90%8C%E6%B4%BE%E7%94%9F%E5%BA%93%E7%9A%84%E5%88%86%E6%94%AF)

- [Gists](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#gists)
- [Git.io](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#gitio)
- [键盘快捷键](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E9%94%AE%E7%9B%98%E5%BF%AB%E6%8D%B7%E9%94%AE)
- [整行高亮](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E6%95%B4%E8%A1%8C%E9%AB%98%E4%BA%AE)
- [用 Commit 信息关闭 Issue](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E7%94%A8-commit-%E4%BF%A1%E6%81%AF%E5%85%B3%E9%97%AD-issue)
- [链接其他仓库的 Issue](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E9%93%BE%E6%8E%A5%E5%85%B6%E4%BB%96%E4%BB%93%E5%BA%93%E7%9A%84-issue)
- [锁定项目对话功能](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E9%94%81%E5%AE%9A%E9%A1%B9%E7%9B%AE%E5%AF%B9%E8%AF%9D%E5%8A%9F%E8%83%BD)
- [设置 CI 对每条 Pull Request 都进行构建](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E8%AE%BE%E7%BD%AE-ci-%E5%AF%B9%E6%AF%8F%E6%9D%A1-pull-request-%E9%83%BD%E8%BF%9B%E8%A1%8C%E6%9E%84%E5%BB%BA)
- [Markdown 文件语法高亮](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#markdown-%E6%96%87%E4%BB%B6%E8%AF%AD%E6%B3%95%E9%AB%98%E4%BA%AE)
- [表情符](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E8%A1%A8%E6%83%85%E7%AC%A6)
- [图片 / GIF 动画](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E5%9B%BE%E7%89%87--gif-%E5%8A%A8%E7%94%BB)- [在 GitHub Wiki 中引用图片](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E5%9C%A8-github-wiki-%E4%B8%AD%E5%BC%95%E7%94%A8%E5%9B%BE%E7%89%87)

- [快速引用](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E5%BF%AB%E9%80%9F%E5%BC%95%E7%94%A8)
- [粘贴剪贴板中的图片到评论](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E7%B2%98%E8%B4%B4%E5%89%AA%E8%B4%B4%E6%9D%BF%E4%B8%AD%E7%9A%84%E5%9B%BE%E7%89%87%E5%88%B0%E8%AF%84%E8%AE%BA)
- [快速添加许可证文件](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E5%BF%AB%E9%80%9F%E6%B7%BB%E5%8A%A0%E8%AE%B8%E5%8F%AF%E8%AF%81%E6%96%87%E4%BB%B6)
- [任务列表](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E4%BB%BB%E5%8A%A1%E5%88%97%E8%A1%A8)- [Markdown 文件中的任务列表](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#markdown-%E6%96%87%E4%BB%B6%E4%B8%AD%E7%9A%84%E4%BB%BB%E5%8A%A1%E5%88%97%E8%A1%A8)

- [相对链接](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E7%9B%B8%E5%AF%B9%E9%93%BE%E6%8E%A5)
- [GitHub Pages 的元数据与插件支持](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#github-pages-%E7%9A%84%E5%85%83%E6%95%B0%E6%8D%AE%E4%B8%8E%E6%8F%92%E4%BB%B6%E6%94%AF%E6%8C%81)
- [查看 YAML 格式的元数据](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E6%9F%A5%E7%9C%8B-yaml-%E6%A0%BC%E5%BC%8F%E7%9A%84%E5%85%83%E6%95%B0%E6%8D%AE)
- [渲染表格数据](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E6%B8%B2%E6%9F%93%E8%A1%A8%E6%A0%BC%E6%95%B0%E6%8D%AE)
- [撤销 Pull Request](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E6%92%A4%E9%94%80-pull-request)
- [Diffs](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#diffs)- [可渲染文档的Diffs](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E5%8F%AF%E6%B8%B2%E6%9F%93%E6%96%87%E6%A1%A3%E7%9A%84diffs)
- [可比较的地图数据](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E5%8F%AF%E6%AF%94%E8%BE%83%E7%9A%84%E5%9C%B0%E5%9B%BE%E6%95%B0%E6%8D%AE)
- [在 Diff 中展开查看更多的上下文](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E5%9C%A8-diff-%E4%B8%AD%E5%B1%95%E5%BC%80%E6%9F%A5%E7%9C%8B%E6%9B%B4%E5%A4%9A%E7%9A%84%E4%B8%8A%E4%B8%8B%E6%96%87)
- [获取 Pull Request 的 diff 或 patch 文件](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E8%8E%B7%E5%8F%96-pull-request-%E7%9A%84-diff-%E6%88%96-patch-%E6%96%87%E4%BB%B6)
- [显示图片以及比较图片](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87%E4%BB%A5%E5%8F%8A%E6%AF%94%E8%BE%83%E5%9B%BE%E7%89%87)

- [Hub](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#hub)
- [贡献者指南](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E8%B4%A1%E7%8C%AE%E8%80%85%E6%8C%87%E5%8D%97)
- [Octicons](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#octicons)
- [GitHub 资源](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#github-%E8%B5%84%E6%BA%90)- [GitHub 相关演讲视频](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#github-%E7%9B%B8%E5%85%B3%E6%BC%94%E8%AE%B2%E8%A7%86%E9%A2%91)


- [Git](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#git)- [从工作区去除大量已删除文件](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E4%BB%8E%E5%B7%A5%E4%BD%9C%E5%8C%BA%E5%8E%BB%E9%99%A4%E5%A4%A7%E9%87%8F%E5%B7%B2%E5%88%A0%E9%99%A4%E6%96%87%E4%BB%B6)
- [上一个分支](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E4%B8%8A%E4%B8%80%E4%B8%AA%E5%88%86%E6%94%AF)
- [去除空白](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E5%8E%BB%E9%99%A4%E7%A9%BA%E7%99%BD)
- [检出 Pull Requests](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E6%A3%80%E5%87%BA-pull-requests)
- [没有任何改动的提交](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E6%B2%A1%E6%9C%89%E4%BB%BB%E4%BD%95%E6%94%B9%E5%8A%A8%E7%9A%84%E6%8F%90%E4%BA%A4)
- [美化 Git Status](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E7%BE%8E%E5%8C%96-git-status)
- [美化 Git Log](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E7%BE%8E%E5%8C%96-git-log)
- [Git 查询](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#git-%E6%9F%A5%E8%AF%A2)
- [合并分支](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E5%90%88%E5%B9%B6%E5%88%86%E6%94%AF)
- [修复有问题的提交以及自动合并](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E4%BF%AE%E5%A4%8D%E6%9C%89%E9%97%AE%E9%A2%98%E7%9A%84%E6%8F%90%E4%BA%A4%E4%BB%A5%E5%8F%8A%E8%87%AA%E5%8A%A8%E5%90%88%E5%B9%B6)
- [以网站方式查看本地仓库](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E4%BB%A5%E7%BD%91%E7%AB%99%E6%96%B9%E5%BC%8F%E6%9F%A5%E7%9C%8B%E6%9C%AC%E5%9C%B0%E4%BB%93%E5%BA%93)
- [Git 配置](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#git-%E9%85%8D%E7%BD%AE)- [Git 命令自定义别名](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#git-%E5%91%BD%E4%BB%A4%E8%87%AA%E5%AE%9A%E4%B9%89%E5%88%AB%E5%90%8D)
- [自动更正](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E8%87%AA%E5%8A%A8%E6%9B%B4%E6%AD%A3)
- [颜色输出](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#%E9%A2%9C%E8%89%B2%E8%BE%93%E5%87%BA)

- [Git 资源](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#git-%E8%B5%84%E6%BA%90)- [Git 参考书籍](https://github.com/tiimgreen/github-cheat-sheet/blob/master/README.zh-cn.md#git-%E5%8F%82%E8%80%83%E4%B9%A6%E7%B1%8D)





