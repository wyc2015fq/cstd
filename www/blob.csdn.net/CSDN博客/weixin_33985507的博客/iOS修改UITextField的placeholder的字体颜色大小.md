# iOS修改UITextField的placeholder的字体颜色大小 - weixin_33985507的博客 - CSDN博客
2016年04月05日 10:44:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
设置UITextField的placeholder的颜色代码片段：
```
textField.placeholder = @"请输入用户名!";  
[textField setValue:[UIColor redColor] forKeyPath:@"_placeholderLabel.textColor"];  
[textField setValue:[UIFont boldSystemFontOfSize:16] forKeyPath:@"_placeholderLabel.font"];
```
或者直接在iOS6.0之后提供的attributedPlaceholder属性：
```
UITextField *textField = [[UITextField alloc] initWithFrame:CGRectMake(0, 0, 200, 200)];
NSString *holderText = @"请输入密码！";
NSMutableAttributedString *placeholder = [[NSMutableAttributedString alloc] initWithString:holderText];
[placeholder addAttribute:NSForegroundColorAttributeName
              value:[UIColor redColor]
              range:NSMakeRange(0, holderText.length)];
[placeholder addAttribute:NSFontAttributeName
              value:[UIFont boldSystemFontOfSize:16]
              range:NSMakeRange(0, holderText.length)];
textField.attributedPlaceholder = placeholder;
[cell.contentView addSubview:textField];
```
与上面那段代码是一样的效果。
_placeholderLabel说明
```
(lldb) po [textField valueForKey:@"_placeholderLabel"]
<UITextFieldLabel: 0x13fe835f0; frame = (0 0; 0 0); text = '请输入密码！'; opaque = NO; userInteractionEnabled = NO; layer = <_UILabelLayer: 0x13fe855f0>>
```
其实_placeholderLabel就是UITextFieldLabel类型，这是在有placeholder的情况下打印出来的，但是为什么知道内部叫\ _placeholderLabel呢？根据苹果的命名规范，猜测出来的，然后测试能否获取到。其实可以通过Runtime中获取类的属性来拿到这个私有属性。这不算私有API，这是利用KVC获取的，虽然苹果并不希望我们这么做，但是可以正常上架（在很多个App里使用过）。
说明
iOS6.0之后，有attributedPlaceholder属性，因此可以直接通过它设置。
在iOS6.0之前，可以通过KVC来设置_placeholderLabel的属性值。
