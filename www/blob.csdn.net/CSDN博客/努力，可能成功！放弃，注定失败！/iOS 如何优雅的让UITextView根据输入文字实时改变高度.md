# iOS:如何优雅的让UITextView根据输入文字实时改变高度 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月24日 11:44:35[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2571
[demo](https://github.com/Flying-Einstein/UITextViewTest.git)
**前言：**
> 
UITextView的高度随着输入文字实时的改变是app中非常常见的功能，社交软件的文字输入框、评论框都会用到
网上有很多UITextView的高度随着输入文字实时改变的demo，笔者看了很多，很多虽然可以实现相应的功能但是有些细节实现的不是很好,所以笔者在参考前人的基础上，做了些许优化，希望能对读者有所帮助
**一言不合贴代码:**
- 创建UITextView
```
// 下面这一段代码，笔者就不费口舌了，读者应该都看的懂，就是创建一个外观类似于UITextField的UITextView
  self.contentTextView = [[UITextView alloc]initWithFrame:CGRectMake((kMainBoundsWidth-250)/2, kMainBoundsHeight/2-50, 250, 39)];
  self.contentTextView .layer.cornerRadius = 4;
  self.contentTextView .layer.masksToBounds = YES;
  self.contentTextView .delegate = self;
  self.contentTextView .layer.borderWidth = 1;
  self.contentTextView .font = [UIFont systemFontOfSize:14];
  self.contentTextView .layer.borderColor = [[[UIColor lightGrayColor] colorWithAlphaComponent:0.4] CGColor];
  //加下面一句话的目的是，是为了调整光标的位置，让光标出现在UITextView的正中间
  self.contentTextView.textContainerInset = UIEdgeInsetsMake(10,0, 0, 0);
  [self.view addSubview:self.contentTextView ];
```
- 实现的关键：UITextView 的一个代理方法`- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text；`
这个代理方法可能很多新手都不太常用，UITextView 每此输入字符都会走该方法，在这个方法中实时计算输入文字的高度，从而动态调整UITextView 的高度是最合适不过的。
1.计算输入文字高度的方法,之所以返回的高度值加22是因为UITextView有一个初始的高度值40，但是输入第一行文字的时候文字高度只有18，所以UITextView的高度会发生变化，效果不太好
```
- (float) heightForTextView: (UITextView *)textView WithText: (NSString *) strText{
    CGSize constraint = CGSizeMake(textView.contentSize.width , CGFLOAT_MAX);
    CGRect size = [strText boundingRectWithSize:constraint
                                             options:(NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading)
                                          attributes:@{NSFontAttributeName: [UIFont systemFontOfSize:14]}
                                             context:nil];
    float textHeight = size.size.height + 22.0;
    return textHeight;
}
```
2.在`- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text；`中一步步实现我们的效果:
```
- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    CGRect frame = textView.frame;
    float  height = [ self heightForTextView:textView WithText:textView.text];
    frame.size.height = height;
    [UIView animateWithDuration:0.5 animations:^{
            textView.frame = frame;
        } completion:nil];
    return YES;
}
```
如上面一段代码，我们在输入文字的时候实时计算textView中的文字，就可以得到我们想要的高度，效果如何呢？
![](http://upload-images.jianshu.io/upload_images/1595096-e49dd5141553e409.gif?imageMogr2/auto-orient/strip)
UITextView01
细心的读者可能发现了，在第二行输入第一个字的时候，frame没有更改，输入第二个字的时候才发生更改，第三行同样如此，什么原因呢？
笔者打断点调试后，终于找到问题的根源，回过头来再看`- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text;`这个方法，可以看到该方法有两个参数textView 和text，在方法我们计算的是textView.text的高度，但实际上这里存在一个问题，每次输入文字后调用该方法，此时输入的文字并不在textView.text中，而在另一个参数text中，走完该方法后每次输入的文字才加入到textView.text中。笔者选择的解决方案是将textView.text和text文字拼接起来
```
- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    CGRect frame = textView.frame;
    float height = [self heightForTextView:textView WithText:[NSString stringWithFormat:@"%@%@",textView.text,text]];
    frame.size.height = height;
    [UIView animateWithDuration:0.5 animations:^{
            textView.frame = frame;
        } completion:nil];
    return YES;
}
```
再来看下效果：
![](http://upload-images.jianshu.io/upload_images/1595096-08c71986fc85f2d9.gif?imageMogr2/auto-orient/strip)
UITextView02
可以看出，现在满足了我们刚才的要求，在第二行第一次输入的时候，可以更改frame了，但是新的问题又出现了，就是删除字符的时候，当第二行被删光的时候，没有直接计算frame，直到删除第一行某个文字的时候才会计算，这是为什么呢？
笔者调试过后，发现按删除键的时候，text的文字为`@""`,每次按删除键，调用该方法的时候，textView.text此时并没有把该字符删掉，所以计算的还是第一行加第二行文字的高度，因此我们可以根据text的内容做条件判断，如果text内容为`@""`,我们通过截取字符串的方式手动去掉一个字符
```
- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    CGRect frame = textView.frame;
    float height;
    if ([text isEqual:@""]) {
     height = [ self heightForTextView:textView WithText:[textView.text substringToIndex:[textView.text length] - 1]];
    }else{
     height = [self heightForTextView:textView WithText:[NSString stringWithFormat:@"%@%@",textView.text,text]];
     }
    frame.size.height = height;
    [UIView animateWithDuration:0.5 animations:^{
            textView.frame = frame;
        } completion:nil];
    return YES;
}
```
效果图：
![](http://upload-images.jianshu.io/upload_images/1595096-e6f92c139566df8a.gif?imageMogr2/auto-orient/strip)
UITextView03
基本上实现了需求，有一个小bug，造成了闪退，textView.text为空的时候，截取字符串会越界造成闪退，加一个条件判断就好了
终极代码:
```
- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    CGRect frame = textView.frame;
    float height;
    if ([text isEqual:@""]) {
        if (![textView.text isEqualToString:@""]) {
            height = [ self heightForTextView:textView WithText:[textView.text substringToIndex:[textView.text length] - 1]];
        }else{
            height = [ self heightForTextView:textView WithText:textView.text];
        }
    }else{
            height = [self heightForTextView:textView WithText:[NSString stringWithFormat:@"%@%@",textView.text,text]];
    }
    frame.size.height = height;
    [UIView animateWithDuration:0.5 animations:^{
            textView.frame = frame;
        } completion:nil];
    return YES;
}
```
**结语:**
> 
笔者只是实现了简单的需求，可能很多地方还不够完善，也可能存在bug，欢迎批评，指正，共同交流。
文／Flying_Einstein（简书作者）
原文链接：http://www.jianshu.com/p/9e960757de86
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
