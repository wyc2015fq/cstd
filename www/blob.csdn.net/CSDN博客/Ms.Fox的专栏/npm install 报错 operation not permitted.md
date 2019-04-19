# npm install 报错 operation not permitted - Ms.Fox的专栏 - CSDN博客
2019年03月02日 14:26:25[lulu-lu](https://me.csdn.net/smbluesky)阅读数：17
个人分类：[JAVA](https://blog.csdn.net/smbluesky/article/category/6556788)
之前有时报这个，以为还真是权限不够，感觉有点奇怪，用管理员权限执行，有时还真有用，不过后面查了下，时缓存的问题，清理下缓存就行，不用管理员权限。
方法1、（我没有找到这个文件哦~~，所以第一种方法就算啦）
需要删除npmrc文件。
强调：不是nodejs安装目录npm模块下的那个npmrc文件
而是在C:\Users\{账户}\下的.npmrc文件..
方法2、（方法有效）
或者直接用命令清理就行，控制台输入：
npm cache clean --force
