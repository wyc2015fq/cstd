# iOS 聊天信息图文混排 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月07日 17:25:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1276
候，有一个功能点是要支持聊天信息的图文混排。当时听到这个需求的时候，大脑里第一反应是用webView来通过htmlstring来实现这个功能，不过webView是一个能不用最好不用的东西，因为它的内存管理特别可怕。所以我研究了其他方法，确实存在一个更好的方法，那就是使用NSTextAttachment和NSAttributedString。
     其实原理很简单，那就是把要混排的图片当成一个字符来显示，这就要用到NSAttributedString了。当我们需要在文字中高亮某个词语，或是改变行距等等，各种普通的NSString＋UILabel做不到的事情都可以用NSAttributedString来做。
     它有一个工厂方法，这是个iOS7.0新增的接口。
`+ (NSAttributedString *)attributedStringWithAttachment:(NSTextAttachment *)attachment NS_AVAILABLE(10_0, 7_0);`
     通过这个方法可以把NSTextAttachment转化成NSAttributedString 。只要把图片转换成一个NSTextAttachment就可以和其他NSAttributedString添加在一起了。
     那么，要如何把图片转换为NSTextAttachment呢？同样很简单。
NSTextAttachment有一个属性image，只需要把图片赋值给这个属性就可以了。
```
NSTextAttachment *textAttachment = [[NSTextAttachment alloc]init];
textAttachment.image = image;
```
     这样我们就得到了一个NSTextAttachment。然后再把他转换为NSAttributedString 。
`NSAttributedString *imageStr = [NSAttributedString attributedStringWithAttachment:textAttachment];`
     有了NSAttributedString，相信大家下一步都知道该怎么做了，我就不多说了。
     不过有一个问题，我目前还没有找到特别漂亮的方式来解决，那就是这样插入的图片大小和文字不太一样，是根据图片原先大小来的，目前我的解决方法是，设置NSTextAttachment的属性bounds，强行赋值来进行排版。
     来一张效果图
![](http://upload-images.jianshu.io/upload_images/1345254-e6e69a4bf121f2b0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
     看起来还不错，至此图文混排的基本功能就搞定了。
     但是到目前为止实现的功能仅仅是实现将指定的UIImage和文字混排在一起而已。然而我的最终目标是实现聊天信息中的图文混排。那么该怎么做呢？
     一开始我也是一头雾水，然后我想到了微信平时发emoji时候的场景，一下子豁然开朗。究竟是怎么回事呢？让我们来看一下微信里的emoji的使用的情况。
![](http://upload-images.jianshu.io/upload_images/1345254-d5cf4251bf20c06d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
EEAB01FD10C218C3C561A95051710844.png
     不难发现，当我们点击一个emoji表情的时候，在微信的文字发送框里，就会出现一个[xxx]格式的字符串，此时点击发送的话，就可以看到发出去的聊天信息里带有了emoji表情。
![](http://upload-images.jianshu.io/upload_images/1345254-10722cbadc571234.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
E5D772C423407013457AB1E757C2F403.png
     那么这个要如何做到呢？让我们来分析一下这个现象，很显然，微信把[调皮]和[撇嘴]转换为了图片显示，显示的原理很简单，就如上文我们说的，只需要初始化两个UIImage，然后赋值给NSTextAttachment并转换为NSSAttributedString就可以了。问题的核心在于如何知道哪些文字需要我们来转换。
     显然微信的做法是将所有方括号"[]"里面的文字替换为NSTextAttachment转换成的NSSAttributedString了。也就是说，我们需要找出所有在[]里的词组。如果想要做到这一点，
就不得不提到一个概念，正则表达式。
     不过有关正则表达式的知识实在是太多，而且我也只是略知一二，所以就不在这里献丑了。我就直接上代码了。简书里面markdown的转义我不是很会用，所以我就贴截图吧。
![](http://upload-images.jianshu.io/upload_images/1345254-89ba8f72c3516d67.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
4899FCB7-845A-4690-B440-213509849C48.png
     这段代码的意思就是用regex_emoji这段正则表达式去匹配字符，获得的结果存在resultArray数组里。
     其他东西我想也没什么好解释的，我就简单的解释一下这个正则表达式的含义。
最左边的\\[是用来匹配“[”的，最右边的\\]是用来匹配“]”的，而^\\[\\]代表了除[]以外的字符，在外面加一个[]*就表示匹配任意数量的除[]以外的字符，这样就可以匹配到[xxx]这个格式的字符了。
     让我们来打一下log检测一下，当text赋值为@"f[等等[b哦]]哦a[a哈f]]]"的时候，resultArray的结果为
![](http://upload-images.jianshu.io/upload_images/1345254-0170ed56c120a69f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
E1054269-8064-48FC-A884-E8AF246B17B3.png
     可以看到，log里的两个range确实正确的对应了我们想要的结果。接下来我们就要对这个数组进行解析。
```
for (NSInteger i = 0; i  < [resultArray count]; ++i) {
    NSTextCheckingResult *result = resultArray[i];
    NSRange range =  [result range];
    NSString *subStr = [text substringWithRange:range];
    NSLog(@"%@",subStr);
}
```
     看一下输出的内容，可以看出我们已经得到了要转换的文字了。
![](http://upload-images.jianshu.io/upload_images/1345254-baa4c22ad2a20ca6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
6ADBA36C-C61F-4112-9C4E-09B26E165128.png
     接下来要做的事就简单了，根据这个格式的字符去找对应名字的图片，然后再以NSTextAttachment的方式转换为NSAttributedString，然后根据相对应的NSRange去替换原先的文本，就可以了。
```bash
NSMutableDictionary *imageDic = [NSMutableDictionary dictionaryWithCapacity:2];
[imageDic setObject:imageStr forKey:@"image"];
[imageDic setObject:[NSValue valueWithRange:range] forKey:@"range"];
[imageArray addObject:imageDic];
```
     解释一下，imageStr就是已经转换为NSAttributedString的NSTextAttachment，这里它和range组成一个字典保存到一个imageArray里面去。
```
NSMutableAttributedString *attributeString = [[NSMutableAttributedString alloc]initWithString:text];
for (NSInteger i = [imageArray count]-1; i>=0; i--) {
    NSDictionary *dic = imageArray[i];
    NSRange range = [dic[@"range"] rangeValue];
    [attributeString replaceCharactersInRange:range withAttributedString:dic[@"image"]];
}
```
     遍历一下imageArray的数组，依次取出对应的范围和转换后的NSAttributedString，然后来替换对应的地方。注意这里使用的是倒序遍历。因为如果使用正序遍历的话，当替换掉一组词后，长度发生了变化，原先保存的NSRange可能就无法对应到正确的位置了。
     至此，图文混排的功能就真正实现了。最终的效果就如下图所示。
![](http://upload-images.jianshu.io/upload_images/1345254-9e7dce13c751c66f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
4E7B1485A528B3328E6266F824134FF1.png
![](http://upload-images.jianshu.io/upload_images/1345254-327cbf0058f542d8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
9EEFCD208B99D15DEBF7863F6BDCEECB.png
文／恋鳥（简书作者）
原文链接：http://www.jianshu.com/p/57a9bf3479a3
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
