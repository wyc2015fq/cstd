# NSAttributedString宽高计算小技巧 - 月若无涯的专栏 - CSDN博客
2015年06月30日 15:32:08[月若无涯](https://me.csdn.net/u010124617)阅读数：3031
通常对于CoreText之类自己实现绘制的控件来说，计算富文本的宽高其实需要依赖CTFramesetterSuggestFrameSizeWithConstraints这个方法。
但有些时候，我们可能只是使用UILable、UITextView等系统控件，设置了他们的attributedText，此时，计算控件宽高其实可以利用系统自身的方法。
例如： 
`label.attributedText = attributedString;`
只需要使用 
`h = [label sizeThatFits:CGSizeMake(100,CGFLOAT_MAX)].height`
即可获得相应的高度，即可用于后续的高度计算。
这个小技巧希望对大家有帮助。
当然，如果你使用autolayout，这种根据内容调整控件大小的工作则会更加简单，只要设置好相应的约束即可。
