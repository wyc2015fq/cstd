# iOS使用脚本批量打渠道包 - 月若无涯的专栏 - CSDN博客
2015年06月18日 14:19:05[月若无涯](https://me.csdn.net/u010124617)阅读数：4454
最近我们接到了新的需求，需要打出类似xx001-xx100共100个这样的ipa渠道包，不需要签名。（这批ipa包后续会用企业证书签名，不会影响AppStore的） 
这些包所有的功能、内容都是一样的，区别在于不同的包在统计、连接我们服务器时会提供一个不同的字符串标示。 
如果按照常规的打包方式，我们需要不断的修改工程中的值，然后一个个打包。。。这种毫无技术含量的活怎么能忍受！
简单思考之后可以想到，我们可以通过使用shell脚本来实现批量打包，渠道信息可以保存在应用的info.plist里面。
批量处理思路： 
1. 通过改变某个设置或变量，引起工程中info.plist中某个值改变 
2. 使用xcodebuild命令进行打包等处理
细分的话其实又可以有几种做法： 
1.使用多个target 
这种的优点是可以更深度的定制信息，参见唐巧的文章：[http://blog.devtang.com/blog/2013/10/17/the-tech-detail-of-ape-client-1/](http://blog.devtang.com/blog/2013/10/17/the-tech-detail-of-ape-client-1/)
缺点是对于我们这种简单需求来说，操作略繁琐，代价较高。 
脚本和下面的2类似，只是变量是target而已。
2.使用多个buildSetting 
不同的build active architecture 设置不同的User-Defined值 
第一版脚本由我的同事提供，采用的就是这种方式，比上面那种方式稍微简单了点 
思路和步骤如下： 
1. 在Xcode的build设置中，通过拷贝release生成多个build active architecture，分别以渠道名命名 
2. User-Defined属性下增加名为Channel的key，针对不同的build设置不同的渠道值 
3. info.plist中增加Channel键，值引用User-Defined下的设置${Channel} 
（由于方案不够完美，该脚本就不在此贴出了）
当我看到这种方案的时候其实还是不满足的，因为需要改动工程配置等相关信息，还是很麻烦。
对于我们来说，打出来的包其实只有info.plist中一个预设的值不一样而已，其实可以不需要那么多的target或者那么多的buildsetting，只需要在打包时，用脚本改一下info.plist中对应的键值即可。 
查找资料发现，命令修改plist值已经有内置的工具PlistBuddy，不过这工具使用时需要使用其绝对路径才能引用到，路径为/usr/libexec/PlistBuddy，工具的更多功能有兴趣的可以自己搜一下。 
调整后，脚本为：
```bash
channels=( 渠道1 渠道2 渠道3 )
for i in ${channels[@]}
do
    xcodebuild -project ./你的工程.xcodeproj -scheme 你的scheme名称 -configuration Release archive -archivePath ./$i
    /usr/libexec/PlistBuddy -c "Set :Channel ""$i" ./$i.xcarchive/Products/Applications/*.app/info.plist
    rm -Rf ./$i.ipa
    xcodebuild -exportArchive —exportFormat ipa -archivePath ./archive.xcarchive -exportPath ./$i.ipa -exportWithOriginalSigningIdentity
    rm -Rf ./$i.xcarchive
done
```
好了，完事具备，可以批量打包了
打包没有问题，嗯，不过好像很费时间啊。。。
回过头来我们再分析下打包流程： 
1. 使用xcodebuild命令根据工程生成xcarchive文件 
2. 使用xcarchive文件导出ipa，此过程中可以签名
对于一次打包过程来说，期间生成了xx.xcarchive文件，这个文件此时还没有进行任何的签名之类的操作，其实我们可以直接修改这个xcarchive包中对应app文件内的plist，然后再导出成ipa，这样就可以共用一个xcarchive文件，节省下了N多次打包出该文件的时间。 
好了调整一下得到我们的最终版：
```bash
xcodebuild -project ./你的工程.xcodeproj -scheme 你的scheme名称 -configuration Release archive -archivePath ./archive
channels=( 渠道1 渠道2 渠道3 )
for i in ${channels[@]}
do
    /usr/libexec/PlistBuddy -c "Set :Channel ""$i" ./archive.xcarchive/Products/Applications/*.app/info.plist
    rm -Rf ./$i.ipa
    xcodebuild -exportArchive —exportFormat ipa -archivePath ./archive.xcarchive -exportPath ./$i.ipa -exportWithOriginalSigningIdentity
done
rm -Rf ./archive.xcarchive
```
需要注意的是 
1. 项目的info.plist文件中要提前设置一个Channel的键，值写上默认渠道。 
2. 工程中相应的证书等还是要配置好，否则可能打包失败 
3. 本文中打出来的ipa都是未签名的，需要签名的话自行搜索下，比较简单
将这脚本保存为archive文件，丢在工程目录下，使用方式： 
打开终端，执行两行命令
```bash
cd 你的工程目录
```
`./archive`
好了，打完收工，这样你在工程中，使用
`NSString* channel = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"Channel"];`
拿到的就是渠道信息了。
最后总结下这套方案的优点： 
1. 工程配置几乎不需要改动，只要一次性在info.plist中增加需要的默认键值 
2. 后续有更多渠道时，只需要更新脚本即可，对于项目理论上不需要进行任何改动
PS：本方案中修改的是info.plist，其实也可以自定义一个文件用来修改，方案总体一致，细节可以微调。
希望对大家有帮助。
