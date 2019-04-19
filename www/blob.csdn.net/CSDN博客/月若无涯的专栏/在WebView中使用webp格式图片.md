# 在WebView中使用webp格式图片 - 月若无涯的专栏 - CSDN博客
置顶2016年05月31日 14:39:28[月若无涯](https://me.csdn.net/u010124617)阅读数：5602
    很早之前，我们的项目中就已经采用了webp格式，但是由于webView本身并不能解析webp格式，所以我们基于webView的文章详情页就无法使用到这项优化。 
    那么有没有什么办法能实现呢？当然是有的。  
    在开始技术讲解之前需要先说明，本文的技术方案，是基于本项目的情况：文章的正文大部分通过接口直接获取到，通过在客户端本地进行html正文组装，最后通过webView的loadHTMLString方法进行加载显示。普通的图片可以通过转换链接得到webp服务器获取到相应的webp版的图片。  
    本项目中，图片缓存使用了SDWebImage，并且开启了webp支持功能，那么我们对详情页webView的处理也会基于此来实现。 
    通过思考，方案其实还是比较明确的，就是替换html中图片链接，通过客户端下载webp图片，然后在通过js刷新出页面上的下完的图片，但实际开发中也遇到了一些坑，比如:  
- HTML解析库的setAttributeNamed不能增加属性
- webp服务器图片下载后的默认缓存时gif不能正常存储
- 下载完的图片不能实时通过js更改src为本地文件地址加载出来
最终的技术实现： 
1.对下载回来的html内容进行处理，获取所有图片链接，并进行webp链接处理转换 
    对html内容的解析处理我使用的是Objective-C-HMTL-Parser，但是该库已经多年不维护，这里有我fork后进行部分优化调整的版本：[https://github.com/YueRuo/Objective-C-HMTL-Parser](https://github.com/YueRuo/Objective-C-HMTL-Parser)
    处理html图片核心处理逻辑代码：  
```
@try {
    HTMLParser *parser = [[HTMLParser alloc] initWithString:htmlContent error:&error];
    HTMLNode *bodyNode = [parser body];
    if (error) {
        return;
    }
    //得到所有的img标签
    NSArray *inputNodes = [bodyNode findChildTags:@"img"];
    for (HTMLNode *inputNode in inputNodes) {
        NSString *imageSrc = [inputNode getAttributeNamed:@"src"];
        if (!imageSrc) {
            continue;
        }
        NSString *newSrc = [[GlobalVariable shareInstance] resizeWebpImageWithUrl:imageSrc size:CGSizeMake((SCREEN_WIDTH - 20) * 2, 0)];//根据原图片，得到webp服务器使用的图片链接，需要有webp处理服务器
        //检查本地图片缓存
        NSString *key = [[SDWebImageManager sharedManager] cacheKeyForURL:[NSURL URLWithString:newSrc]];
        NSString *localPath = [[SDImageCache sharedImageCache] defaultCachePathForKey:key];
        NSString *webpImage = newSrc;
        BOOL localExsit = [[NSFileManager defaultManager] fileExistsAtPath:localPath];
        if (localExsit) {
            newSrc = [NSString stringWithFormat:@"file://%@", localPath];
        }
        //存储疑似webp图片和原图片，如果newSrc和webp相同则说明本地没有缓存图片
        [_webpImageUrlDic setObject:webpImage forKey:newSrc];
        if(localExsit){
            setAttributeNamed(inputNode->_node, "src", [newSrc cStringUsingEncoding:NSUTF8StringEncoding]);
        }else{
            setAttributeNamed(inputNode->_node, "src", "详情页占位图@2x.png");
        }
        //给img标签中增加一个叫osrc的属性，便于后续处理
        setAttributeNamed(inputNode->_node, "osrc", [newSrc cStringUsingEncoding:NSUTF8StringEncoding]);
    }
    htmlContent = [NSMutableString stringWithString:parser.doc.rawContents];
}
@catch (NSException *exception) {
}
@finally {
    [webView loadHTMLString:htmlContent baseURL:baseUrl];
}
```
2.用原生方法下载webp图片，缓存到本地 
    下载之后会存储为jpg或png格式，这样就可以被webView进行本地加载，但是需要注意gif的存储特殊处理。 
    另外通过实验，直接通过js无法实时更新下载到本地的图片，只好通过图片的base64encode数据加载方式实现。 
    具体代码如下：    
```
- (void)webViewDidFinishLoad:(UIWebView *)web {
    //处理webp格式加载
    [_webpImageUrlDic enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
        if([obj isEqualToString:key]){//说明这图没有缓存，还需要下载
            [[SDWebImageDownloader sharedDownloader] downloadImageWithURL:[NSURL URLWithString:obj] options:0 progress:nil completed:^(UIImage *image, NSData *data, NSError *error, BOOL finished) {
                if (image&&finished) {
                    NSString *js;
                    NSRange range = [[obj lowercaseString] rangeOfString:@".gif"];//检查是否是gif
                    BOOL isGif = (range.location != NSNotFound);
                    if (!isGif) {
                        [[SDImageCache sharedImageCache] storeImage:image forKey:obj];
                        NSString *base64 = [UIImageJPEGRepresentation(image,1) base64EncodedStringWithOptions:0];
                        js = [NSString stringWithFormat:@"replaceWebPImg('%@','data:image/jpeg;base64,%@')",key,base64];
                    }else{//gif的图片如果直接存储，会变成jpg从而失去动画，因此要特殊处理
                        [[SDImageCache sharedImageCache] storeImage:image recalculateFromImage:false imageData:data forKey:key toDisk:true];
                        NSString *base64 = [data base64EncodedStringWithOptions:0];
                        js = [NSString stringWithFormat:@"replaceWebPImg('%@','data:image/gif;base64,%@')",key,base64];
                    }
                    [NSThread excuteInMainThread:^{
                        [webView stringByEvaluatingJavaScriptFromString:js];
                    } async:false];
                }
            }];
        } else {//缓存中存在，那么直接加载吧
            NSString *js;
            NSRange range = [[obj lowercaseString] rangeOfString:@".gif"];//检查是否是gif
            NSData* data = [NSData dataWithContentsOfFile:[key stringByReplacingOccurrencesOfString:@"file://" withString:@""]];
            NSString *base64 = [data base64EncodedStringWithOptions:0];
            BOOL isGif = (range.location != NSNotFound);
            if (!isGif) {
                js = [NSString stringWithFormat:@"replaceWebPImg('%@','data:image/jpeg;base64,%@')",obj,base64];
            }else{
                js = [NSString stringWithFormat:@"replaceWebPImg('%@','data:image/gif;base64,%@')",obj,base64];
            }
            [NSThread excuteInMainThread:^{
                [webView stringByEvaluatingJavaScriptFromString:js];
            } async:false];
        }
    }];
}
```
3.回调webView页面，用本地链接替换原有的图片 
    加载已下载好的图片，这里主要通过js来实现，即第2步中的replaceWebPImg方法，该js方法可通过提前置于html的模板中，或者webViewDidFinishLoad后采用js注入进去  
```matlab
replaceWebPImg = function(src, localPath) {
    var imgs = document.querySelectorAll('img[osrc="'+src+'"]'),len = imgs.length;;
    for (var i = 0; i < len; i++) {
        var img = imgs[i];
        img.src = localPath;
    }
}
```
    好再次总结一下整个流程：
- 对服务器返回的htmlContent数据进行相应处理，检查图片是否存在缓存，存在则使用本地地址为src，不存在则把图片的src替换成占位图。记录下图片地址，并增加属性做好标记。
- 图片的地址进行webp转换，通过客户端进行下载
- 下载后的图片，通过js方法进行src更改，并且赋值的base64的图片编码数据，因为给本地地址无法实时展示出来
    这篇写的比较简单，更详细的步骤请查阅上面的代码，里面我加上还算详细的注释，希望对想要在webView中使用webp图片的大家有所帮助。
