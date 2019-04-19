# 升级到mac 10.10之后使用pod出现问题的解决方法 - 月若无涯的专栏 - CSDN博客
2015年06月03日 12:10:24[月若无涯](https://me.csdn.net/u010124617)阅读数：579

**[ruby]**[view
 plain](http://blog.csdn.net/dqjyong/article/details/37958067#)[copy](http://blog.csdn.net/dqjyong/article/details/37958067#)
- 最新对mac 10.10的强大功能好奇，于是将系统升级到了10.10，结果发现使用pod出现了下面的问题：  
**[ruby]**[view
 plain](http://blog.csdn.net/dqjyong/article/details/37958067#)[copy](http://blog.csdn.net/dqjyong/article/details/37958067#)
- /System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/lib/ruby/2.0.0/rubygems/core_ext/kernel_require.rb:55:in `require': cannot load such file -- xcodeproj/prebuilt/universal.x86_64-darwin14-2.0.0/xcodeproj_ext (LoadError)  
-     from /System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/lib/ruby/2.0.0/rubygems/core_ext/kernel_require.rb:55:in `require'  
-     from /Library/Ruby/Gems/2.0.0/gems/xcodeproj-0.17.0/lib/xcodeproj/ext.rb:6:in `rescuein <top (required)>'  
-     from /Library/Ruby/Gems/2.0.0/gems/xcodeproj-0.17.0/lib/xcodeproj/ext.rb:3:in `<top (required)>'  
-     from /System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/lib/ruby/2.0.0/rubygems/core_ext/kernel_require.rb:55:in `require'  
-     from /System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/lib/ruby/2.0.0/rubygems/core_ext/kernel_require.rb:55:in `require'  
-     from /Library/Ruby/Gems/2.0.0/gems/xcodeproj-0.17.0/lib/xcodeproj.rb:30:in `<top (required)>'  
-     from /System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/lib/ruby/2.0.0/rubygems/core_ext/kernel_require.rb:55:in `require'  
-     from /System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/lib/ruby/2.0.0/rubygems/core_ext/kernel_require.rb:55:in `require'  
-     from /Library/Ruby/Gems/2.0.0/gems/cocoapods-0.33.1/lib/cocoapods.rb:2:in `<top (required)>'  
-     from /System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/lib/ruby/2.0.0/rubygems/core_ext/kernel_require.rb:55:in `require'  
-     from /System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/lib/ruby/2.0.0/rubygems/core_ext/kernel_require.rb:55:in `require'  
-     from /Library/Ruby/Gems/2.0.0/gems/cocoapods-0.33.1/bin/pod:32:in `<top (required)>'  
-     from /usr/bin/pod:23:in `load'  
-     from /usr/bin/pod:23:in `<main>'  
经过一番乱搜，具体的解决步骤如下:
  1. 打开 Xcode 6
  2. 进入 Preferences
  3. 点击Locations选项
  4. 将 Command Line Tools 版本变成 Xcode 6.0
  5. 卸载 cocoapods，在终端中输入 
**[ruby]**[view
 plain](http://blog.csdn.net/dqjyong/article/details/37958067#)[copy](http://blog.csdn.net/dqjyong/article/details/37958067#)
- $ sudo gem uninstall cocoapods  
  6. 安装xcodeproj，在终端中输入
**[ruby]**[view
 plain](http://blog.csdn.net/dqjyong/article/details/37958067#)[copy](http://blog.csdn.net/dqjyong/article/details/37958067#)
- $ sudo gem install xcodeproj  
  7. 安装cocoapods，在终端中输入
**[ruby]**[view
 plain](http://blog.csdn.net/dqjyong/article/details/37958067#)[copy](http://blog.csdn.net/dqjyong/article/details/37958067#)
- $ sudo gem install cocoapods  
  8. 测试pod是否安装成功，在终端中输入
**[ruby]**[view
 plain](http://blog.csdn.net/dqjyong/article/details/37958067#)[copy](http://blog.csdn.net/dqjyong/article/details/37958067#)
- $ pod --version   
- 
