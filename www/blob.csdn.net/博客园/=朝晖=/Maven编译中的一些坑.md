# Maven编译中的一些坑 - =朝晖= - 博客园
# [Maven编译中的一些坑](https://www.cnblogs.com/dhcn/p/7124868.html)
      错误1：
- invalid LOC header  
      这个错误比较好解决，jar下载不完全，去到相应的Maven包目录，把东西删掉重新编译就行。
      错误2：
- java.lang.TypeNotPresentException: Type org/powermock/modules/junit4/PowerMockRunner not present  
     这种问题一般是Jar的包版本不对，建议换包版本，不要用beta，换成最新的release版本。有的时候，方法找不到，也可能是这个原因。当然有时候可以直接把tests都忽略不编译。
      待续

