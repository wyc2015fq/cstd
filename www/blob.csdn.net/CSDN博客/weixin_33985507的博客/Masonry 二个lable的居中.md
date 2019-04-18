# Masonry 二个lable的居中 - weixin_33985507的博客 - CSDN博客
2016年06月16日 17:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：19
当你要把二个lable 居中显示在屏幕上。 而你不知道二个lable的长度
最好的是用  UISegmentedControl 控件。 他会自动帮你分
twoView 大lable  装二个小label  self.oneSelfName  self.oneSelfCircle
autoLayout 代码
[twoView mas_makeConstraints:^(MASConstraintMaker *make) {
make.top.mas_equalTo(self.oneSelfV.mas_bottom).offset(kpading10);
//make.centerX.mas_equalTo(self.mas_centerX);
make.centerX.mas_equalTo(self);
}];
[self.oneSelfName mas_makeConstraints:^(MASConstraintMaker *make) {
make.left.mas_equalTo(0);
make.centerY.mas_equalTo(0);
make.right.mas_equalTo(self.oneSelfCircle.mas_left).offset(-2);
}];
[self.oneSelfCircle mas_makeConstraints:^(MASConstraintMaker *make) {
make.left.mas_equalTo(self.oneSelfName.mas_right).offset(2);
//make.top.mas_equalTo(self.oneSelfName);
make.centerY.mas_equalTo(self.oneSelfName);
make.right.mas_equalTo(0);
}];
