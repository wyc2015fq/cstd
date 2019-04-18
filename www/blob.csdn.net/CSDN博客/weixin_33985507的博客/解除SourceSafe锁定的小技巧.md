# 解除SourceSafe锁定的小技巧 - weixin_33985507的博客 - CSDN博客
2008年02月19日 12:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
SourceSafe久了就会产生很多问题，如SourceSafe数据库中数据丢失，链接丢失，文件被破坏。在我们产品组使用SourceSafe不到一个月，每次以管理员身份进入SourceSafe都会提示需要修复。让人烦不胜烦。虽然已经计划迁移到Team Foundation上，但是测试工作没有做完，迁移也就一拖再拖。
昨天下班的时候，实在忍不住了，决定将SourceSafe修复一下。运行修复命令以后，就直接回家了，没等修复完。但是今天上班以后进入SourceSafe，发现进不去了，总是提示"SourceSafe Database is Locked by Administrator。"看帮助，是说如果在做修复的时候数据库就会被锁定。众所周知，SourceSafe修复程序使用一个叫analyze.exe的实用程序。但是打开任务管理器却没有发现analyze.exe程序的踪影，也没看到有什么相关进程在运行，折腾了半天，大家伙可都是等着SourceSafe开始工作啊。
正在犯愁之际，突然想到analyze.exe中输入的参数是data目录，也许在data目录下可以找到蛛丝马迹，果然在data目录下找到一个lock文件夹，但是lock文件夹中什么内容都没有，找到loggedin目录，不经意发现一个叫admin.lck的文件，看后缀，应该是个什么锁文件，尝试着将这个文件删除，SourceSafe就可以进入了。
想其原因大概是昨晚停电的时候，还没有修复完，admin.lck这个还残留的原因吧。
