# iOS长按图片保存实现方法 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月01日 09:23:56[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3390
1、添加手势识别：
```
UITapGestureRecognizer*tap =   [[UITapGestureRecognizeralloc]initWithTarget:selfaction:@selector(imgTapCliclk:)];
   UILongPressGestureRecognizer*longTap = [[UILongPressGestureRecognizeralloc]initWithTarget:selfaction:@selector(imglongTapClick:)];
```
2、imaglongTapClick：
```
-(void)imglongTapClick:(UILongPressGestureRecognizer*)gesture
{
if(gesture.state==UIGestureRecognizerStateBegan)
{
UIActionSheet*actionSheet = [[UIActionSheetalloc]initWithTitle:@"保存图片"delegate:self cancelButtonTitle:@"取消"destructiveButtonTitle:nil otherButtonTitles:@"保存图片到手机",nil];
actionSheet.actionSheetStyle=UIActionSheetStyleBlackOpaque;
[actionSheetshowInView:self];
UIImageView*img = (UIImageView*)[gestureview];
_sentImg= img;
}
}
 - (void)actionSheet:(UIActionSheet*)actionSheet didDismissWithButtonIndex:  (NSInteger)buttonIndex
{
if(buttonIndex ==0) {
UIImageWriteToSavedPhotosAlbum(_sentImg.image,self,@selector(imageSavedToPhot  osAlbum:didFinishSavingWithError:contextInfo:),nil);
}
}
```
# pragma mark --- UIActionSheetDelegate---
```
- (void)imageSavedToPhotosAlbum:(UIImage*)image didFinishSavingWithError:  (NSError*)error contextInfo:(void*)contextInfo
{
NSString*message =@"呵呵";
if(!error) {
message =@"成功保存到相册";
 UIAlertView*alert = [[UIAlertViewalloc]initWithTitle:@"提示"message:messagedelegate:selfcancelButtonTitle:@"确定"otherButtonTitles:nil];
[alertshow];
}else
{
message = [errordescription];
UIAlertView*alert = [[UIAlertViewalloc]initWithTitle:@"提    示"message:messagedelegate:selfcancelButtonTitle:@"确定"otherButtonTitles:nil];
[alertshow];
}
}
```
文／轻斟浅醉17（简书作者）
原文链接：http://www.jianshu.com/p/537945866b58
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
