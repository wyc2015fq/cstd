# 【转】UITextView检查已输入字符字数 - weixin_33985507的博客 - CSDN博客
2012年03月26日 10:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
- (void)textViewDidChange:(UITextView *)textView {
    NSInteger number = [textView.text length];
    if (number > 128) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"字符个数不能大于128" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
        [alert show];
        textView.text = [textView.text substringToIndex:128];
        number = 128;
        [alert release];
    }
//    self.statusLabel.text = [NSString stringWithFormat:@"%d/128",number];
}
