# iOS实现截屏 并合适保存 - weixin_33985507的博客 - CSDN博客
2013年11月12日 16:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
 本文转载至：http://blog.csdn.net/zeng11088/article/details/8664510
分类： [UIImageView](http://blog.csdn.net/zeng11088/article/category/1352506)2013-03-12 16:42122人阅读[评论](http://blog.csdn.net/zeng11088/article/details/8664510#comments)(0)收藏[举报](http://blog.csdn.net/zeng11088/article/details/8664510#report)
开发游戏时，往往会有这么一个需求：在某个成就达成或者破纪录时，需要截个屏，然后发送到微博上与好友/粉丝分享，虽然home ＋ 开机键组合可手动截屏，在Cocos2d有个CCRenderTexture类，借助该类可很容易实现代码截取功能。使用CCRenderTexture，我们可以截取游戏场景、某个Layer，甚至是精灵：
- /**游戏截图 
-  *@param node 需要截取的控件 
- */
- - (void)snapshotScreen:(CCNode*)node   
- {   
- //取得屏幕大小 
-     CGSize winSize = [[CCDirector sharedDirector]winSize];   
-     CCRenderTexture* renderTexture = [CCRenderTexture renderTextureWithWidth:winSize.width   
-                                                                       height:winSize.height];   
-     [renderTexture begin];   
-     [node visit];   
-     [renderTexture end];   
-     [renderTexture cleanup];   
-     UIImage *snapshot = [renderTexture getUIImageFromBuffer];   
- //把截图保存到相册里 
-     UIImageWriteToSavedPhotosAlbum(snapshot, nil, nil, nil);   
- }  
如果游戏支持高清模式，上面代码截出来的图是960 * 640大小的，这个尺寸如果要上传到微博上，文件的大小可能会超出限制，那么在上传前可用先把截图大小缩小点如480*320，以减少图片体积：
- /** 调整图片大小*/
- - (UIImage *) scaleFromImage: (UIImage *) image toSize: (CGSize) size   
- {   
-     UIGraphicsBeginImageContext(size);   
-     [image drawInRect:CGRectMake(0, 0, size.width, size.height)];   
-     UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();   
-     UIGraphicsEndImageContext();   
- return newImage;   
- }  
同时可以保存
**[cpp]**[view plain](http://blog.csdn.net/trandy/article/details/7826429)[copy](http://blog.csdn.net/trandy/article/details/7826429)
- 保存文件  
- 
- UIImage *m_imgFore=......;  
- 
- //png格式
- 
- NSData *imagedata=UIImagePNGRepresentation(m_imgFore);  
- 
- //JEPG格式
- 
- //NSData *imagedata=UIImageJEPGRepresentation(m_imgFore,1.0);
- 
- NSArray*paths=NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES);  
- 
- NSString *documentsDirectory=[paths objectAtIndex:0];   
- 
- NSString *savedImagePath=[documentsDirectorystringByAppendingPathComponent:@"saveFore.png"];  
- 
- [imagedata writeToFile:savedImagePath atomically:YES];  
- 
- 或者  
- 
- [fileManager createFileAtPath:[filePath stringByAppendingString:@"/image.png"] contents:data attributes:nil];    将图片保存为PNG格式  
- 
-  [fileManager createFileAtPath:[filePath stringByAppendingString:@"/image.jpg"] contents:data attributes:nil];   将图片保存为JPEG格式  
如果不是cocos2D开发，则使用如下代码：
方法1：
**[cpp]**[view plain](http://blog.csdn.net/trandy/article/details/7826429)[copy](http://blog.csdn.net/trandy/article/details/7826429)
- -(void)screenShots  
- {  
-     CGSize imageSize = [[UIScreen mainScreen] bounds].size;  
- if (NULL != UIGraphicsBeginImageContextWithOptions) {  
-         UIGraphicsBeginImageContextWithOptions(imageSize, NO, 0);  
-     }  
- else
-     {  
-         UIGraphicsBeginImageContext(imageSize);  
-     }  
- 
-     CGContextRef context = UIGraphicsGetCurrentContext();  
- 
- for (UIWindow * window in [[UIApplication sharedApplication] windows]) {  
- if (![window respondsToSelector:@selector(screen)] || [window screen] == [UIScreen mainScreen]) {  
-             CGContextSaveGState(context);  
-             CGContextTranslateCTM(context, [window center].x, [window center].y);  
-             CGContextConcatCTM(context, [window transform]);  
-             CGContextTranslateCTM(context, -[window bounds].size.width*[[window layer] anchorPoint].x, -[window bounds].size.height*[[window layer] anchorPoint].y);  
-             [[window layer] renderInContext:context];  
- 
-             CGContextRestoreGState(context);  
-         }  
-     }  
- 
-     UIImage *image = UIGraphicsGetImageFromCurrentImageContext();  
- 
-     UIGraphicsEndImageContext();  
-     UIImageWriteToSavedPhotosAlbum(image, self, nil, nil);  
-     NSLog(@"Suceeded!");  
- }  
(注意：方法1截图后会是竖屏，所以需要配合UIImage旋转的方法判断方向后旋转才能合适保存，见文章[旋转翻转UIImage 不是UIImageView 适用于源图像的处理，例如截图后旋转](http://blog.csdn.net/trandy/article/details/8207085))
所以再提供另外一方法：
方法2
**[cpp]**[view plain](http://blog.csdn.net/trandy/article/details/7826429)[copy](http://blog.csdn.net/trandy/article/details/7826429)
- UIView *view = [[[[[UIApplication sharedApplication] windows] objectAtIndex:1] subviews] lastObject];//获得某个window的某个subView
- 
-     NSInteger index = 0;//用来给保存的png命名
- for (UIView *subView in [view subviews]) {//遍历这个view的subViews
- if ([subView isKindOfClass:NSClassFromString(@"UIImageView")] || [subView isKindOfClass:NSClassFromString(@"UIThreePartButton")]) {//找到自己需要的subView
- //支持retina高分的关键
- if(UIGraphicsBeginImageContextWithOptions != NULL)  
-             {  
-                 UIGraphicsBeginImageContextWithOptions(subView.frame.size, NO, 0.0);  
-             } else {  
-                 UIGraphicsBeginImageContext(subView.frame.size);  
-             }              
- 
- //获取图像
-             [subView.layer renderInContext:UIGraphicsGetCurrentContext()];  
-             UIImage *image = UIGraphicsGetImageFromCurrentImageContext();  
-             UIGraphicsEndImageContext();  
- 
- //保存图像
-             NSString *path = [NSHomeDirectory() stringByAppendingFormat:@"/%d.png",index];  
- if ([UIImagePNGRepresentation(image) writeToFile:path atomically:YES]) {  
-                 index += 1;  
-                 NSLog(@"Succeeded!");  
-             }  
- else {  
-                 NSLog(@"Failed!");  
-             }  
-         }  
-     }  
