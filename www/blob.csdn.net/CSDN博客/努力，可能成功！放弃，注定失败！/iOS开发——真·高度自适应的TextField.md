# iOS开发——真·高度自适应的TextField - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月24日 08:13:25[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3133
### 产品需求：
由于单行的输入栏无法展示完整的用户地址，现要求在原有功能的基础上进行改进：地址栏文本超过一行时，多行显示。
### 思路：
原来的地址输入栏就是个TextField,由于TextField只能展示一行，故必须换控件。原本的功能必须保证，故这个控件可以总结为：**高度随文本内容增减而改变并且带placeholder的特殊TextField**。具有输入功能的特殊TextField，当然TextView。
### 自定义控件：
**主要步骤如下**
- 新建一个继承自`UIView`的控件`CQTextView`
- 在`CQTextView`上放一个`UITextView`作为输入控件
- 再在`CQTextView`上放一个`UILabel`作为显示placeholder的控件
- 输入控件随文本内容多少而改变高度
- 输入控件文本内容为空时展示placeholder的控件
**代码说明**
- .h文件
```
#import <UIKit/UIKit.h>
@interface CQTextView : UIView
@property (nonatomic,copy) NSString *placeholder;
@property (nonatomic,strong) UIFont *font;
@end
```
- .m文件
```
#import "CQTextView.h"
@interface CQTextView (){
    /** 记录初始化时的height,textview */
    CGFloat _initHeight;
}
@property (nonatomic,strong) UITextView *textView;
/** placeholder的label */
@property (nonatomic,strong) UILabel *placeholderLabel;
@end
@implementation CQTextView
/** 重写初始化方法 */
- (instancetype)initWithFrame:(CGRect)frame{
    if (self = [super initWithFrame:frame]) {
        // 记录初始高度
        _initHeight = frame.size.height;
        self.clipsToBounds = NO;
        // 添加textView
        self.textView = [[UITextView alloc]initWithFrame:self.bounds];
        [self addSubview:self.textView];
        self.textView.delegate = (id)self;
        self.textView.backgroundColor = [UIColor clearColor];
        // 添加placeholderLabel
        self.placeholderLabel = [[UILabel alloc]initWithFrame:CGRectMake(3, 0, frame.size.width - 3, frame.size.height)];
        [self addSubview:self.placeholderLabel];
        self.placeholderLabel.backgroundColor = [UIColor clearColor];
        self.placeholderLabel.textColor = [UIColor lightGrayColor];
    }
    return self;
}
// 赋值placeholder
- (void)setPlaceholder:(NSString *)placeholder{
    _placeholder = placeholder;
    self.placeholderLabel.text = placeholder;
    [self.placeholderLabel sizeToFit];
    self.placeholderLabel.center = self.textView.center;
}
// 赋值font
- (void)setFont:(UIFont *)font{
    self.textView.font = self.placeholderLabel.font = font;
    // 重新调整placeholderLabel的大小
    [self.placeholderLabel sizeToFit];
    self.placeholderLabel.center = self.textView.center;
}
/** textView文本内容改变时回调 */
- (void)textViewDidChange:(UITextView *)textView{
    // 计算高度
    CGSize size = CGSizeMake(self.textView.frame.size.width, CGFLOAT_MAX);
    NSDictionary *dic = [NSDictionary dictionaryWithObjectsAndKeys:self.textView.font,NSFontAttributeName, nil];
    CGFloat curheight = [textView.text boundingRectWithSize:size
                                                    options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingUsesFontLeading
                                                 attributes:dic
                                                    context:nil].size.height;
    // 如果高度小于初始化时的高度，则不赋值(仍采用最初的高度)
    if (curheight < _initHeight) {
        self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, self.frame.size.width, _initHeight);
        self.textView.frame = CGRectMake(self.textView.frame.origin.x, self.textView.frame.origin.y, self.textView.frame.size.width, _initHeight);
    }else{
        // 重新给frame赋值(改变高度)
        self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, self.frame.size.width, curheight+20);
        self.textView.frame = CGRectMake(self.textView.frame.origin.x, self.textView.frame.origin.y, self.textView.frame.size.width, curheight+20);
    }
    // 如果文本为空，显示placeholder
    if (textView.text.length == 0) {
        self.placeholderLabel.hidden = NO;
        self.placeholderLabel.center = self.textView.center;
    }else{
        self.placeholderLabel.hidden = YES;
    }
}
@end
```
- 使用
```
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    CQTextView *textView = [[CQTextView alloc]initWithFrame:CGRectMake(90, 90, 100, 30)];
    [self.view addSubview:textView];
    textView.backgroundColor = [UIColor redColor];
    textView.font = [UIFont systemFontOfSize:20];
    textView.placeholder = @"ss";
}
```
### 效果图
![](http://upload-images.jianshu.io/upload_images/1692043-1ef35e277108dacc.gif?imageMogr2/auto-orient/strip)
效果图.gif
### 后记：
建议大家都掌握这种控件的封装方法，因为在实际项目中的使用还是挺频繁的，比如我们常用的微信，它的输入框就类似于这种。
文／无夜之星辰（简书作者）
原文链接：http://www.jianshu.com/p/58267727ba5f
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
