# 写一个iOS复杂表单的正确姿势 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年01月22日 15:50:07[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2562
# 本文来自简书，原文地址:[http://www.jianshu.com/p/4a3aad22f356](http://www.jianshu.com/p/4a3aad22f356)
# 前言
这几天项目的新需求中有个复杂的表单界面，在做的过程中发现要比想象中复杂很多，有好多问题需要处理。有很多东西值得写下来好好梳理下。
### 需求分析：
![](http://upload-images.jianshu.io/upload_images/988593-c5a11178cace2ad4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/320)
6创建网店1.png
上图便是UI根据需求给的高保真， 我们先根据这张图片来描述一下具体需求，明确一下我们都需要干些什么。
创建网店这个界面是一个复杂的表单，有“网店名称”、“网店主标签”、“网店简介”、“网店地址”、“网店座机”、“email”、“网店LOGO”、“网店封面图”这些项。大部分都是输入框，但也有几项有所不同。“网店地址”项，当被点击后会弹出一个`pickView`来选择“市&区”；“网店LOGO”和“网店封面图”是一样的，是选取图片的控件，要求既可以通过相册选取图片，也可以现场拍照选择。当被点击后，弹出一个`ActionSheet`来是以“拍照”或以“相册”来选取图片。当选取成功后拍照的背景图片变为被选取的图片，并在右上角出现一个删除按钮，可以删除还原再次选取。
表单中除了“email”外所有的项目都是必填的，且“网店名称”、“网店主标签”、“网店简介”和“网店座机”分别有30、20、500、15字的长度限制。“email”虽然为选填，但若填写了则会进行邮箱格式校验。对字数长度的限制要在输入过程中进行监听，若输入时超过限制，则输入框出现红色边框并出现提示文字。等最后点击了“提交”按钮后要进行数据校验，所有该填但未填，所有格式不正确的项都会出现红框和提示文字，当所有数据都合法后才可以提交给服务器。
需求大体就是如此。
这个界面我们还是以`tableView`来实现，由`cell`视图来表示图中所需填写的项目。那我们得先分析下这个界面需要写哪几种样式的`cell`。
该界面总共有4种样式的`cell`。4种样式的`cell`样式也有共同点，每个`cell`左边部分均为表示该行所要填写的项目名称，右边部分则为填写或者选取的内容值，这些值的显示形式有所不同。 `CreateShopTFCell`和`CreateShopTVCell`其实非常类似，右边首先是一个灰色的背景视图，只不过在灰色背景之上的前者是`textField`，而后者是`textView`；`CreateShopPickCell`右边则是两个灰色背景视图，点击之后便弹出一个`pickView`供你选取“市&区”；`CreateShopUploadPicCell`右边则是一个`UIImageView`，无图片被选取时默认是一个相机的图片，当被点击后弹出`ActionSheet`供你选择拍照还是从相册选取照片，选好照片后`UIImageView`的图片被替换，并在右上角出现红色的删除按钮。
如下图所示：
![](http://upload-images.jianshu.io/upload_images/988593-a8afd30b9960c8ef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/320)
6创建网店.png
### 正确地将视图和数据绑定：
我们假设已经写好了上面4种样式`cell`的代码，现在我们在控制器里为其填充数据。
我们首先定义一个表示`cell`数据的`CreateShopModel`。该`model`是为了给`cell`填充数据，可以看到它里面的属性就是`cell`上对应应该显示的数据项。
同时，我们在开头也定义了一个枚举`CreateShopCellType`来代表4种不同样式的`cell`，用于在`tableView`返回`cell`的代理方法里根据枚举值来返回相应样式的`cell`。
```
#import <Foundation/Foundation.h>
typedef enum : NSUInteger {
    CreateShopCellType_TF = 0, // textfield
    CreateShopCellType_TV, // textView
    CreateShopCellType_PICK, // picker
    CreateShopCellType_PIC, // upload picture
} CreateShopCellType;
@interface CreateShopModel : NSObject
@property (nonatomic, copy)NSString                    *title;  // 所要填写的项目名称
@property (nonatomic, copy)NSString                    *placeholder;
@property (nonatomic, copy)NSString                    *key; // 表单对应的字段
@property (nonatomic, copy)NSString                    *errText; // 校验出错时的提示信息
@property (nonatomic, strong)UIImage                    *image;     // 所选取的图片
@property (nonatomic, assign)CreateShopCellType         cellType; // cell的类型
@property (nonatomic, assign)NSInteger                 maxInputLength; // 最大输入长度限制
@end
```
我们在将`tableView`创建并添加在控制器的`view`上后便可以初始化数据源了。该界面`tableView`的数据源是`_tableViewData`数组，数据的每项元素是代表`cell`显示数据的`CreateShopModel`类型的`model`。准确地来说，这些数据是表单未填写之前的死数据，所以需要我们手动地给装入数据源数组中。而在输入框输入或者选取而得的数据则需要我们在输入之后将其捕获存储下来，以等到提交时提交给服务器，这个也有需要注意的坑点，后面再说。
```
- (void)intDataSource
{
     _tableViewData = [NSMutableArray array];
    CreateShopModel *nameModel = [[CreateShopModel alloc] init];
    nameModel.title = @"网店名称";
    nameModel.placeholder = @"请输入网店名称";
    nameModel.key = @"groupName";
    nameModel.cellType = CreateShopCellType_TF;
    nameModel.maxInputLength = 30;
    [_tableViewData addObject:nameModel];
    CreateShopModel *mainTagModel = [[CreateShopModel alloc] init];
    mainTagModel.title = @"网店主标签";
    mainTagModel.placeholder = @"请输入网店主标签";
    mainTagModel.key = @"tag";
    mainTagModel.cellType = CreateShopCellType_TF;
    mainTagModel.maxInputLength = 20;
    [_tableViewData addObject:mainTagModel];
    CreateShopModel *descModel = [[CreateShopModel alloc] init];
    descModel.title = @"网店简介";
    descModel.placeholder = @"请输入网店简介";
    descModel.key = @"introduction";
    descModel.cellType = CreateShopCellType_TV;
    descModel.maxInputLength = 500;
    [_tableViewData addObject:descModel];
    CreateShopModel *addressModel = [[CreateShopModel alloc] init];
    addressModel.title = @"网店地址";
    addressModel.placeholder = @"";
    addressModel.key = @"regionId";
    addressModel.cellType = CreateShopCellType_PICK;
    [_tableViewData addObject:addressModel];
    CreateShopModel *doorIDModel = [[CreateShopModel alloc] init];
    doorIDModel.title = @"";
    doorIDModel.placeholder = @"请输入详细门牌号";
    doorIDModel.key = @"address";
    doorIDModel.cellType = CreateShopCellType_TF;
    [_tableViewData addObject:doorIDModel];
    CreateShopModel *phoneModel = [[CreateShopModel alloc] init];
    phoneModel.title = @"网店座机";
    phoneModel.placeholder = @"请输入网店座机";
    phoneModel.key = @"telephone";
    phoneModel.cellType = CreateShopCellType_TF;
    phoneModel.maxInputLength = 15;
    [_tableViewData addObject:phoneModel];
    CreateShopModel *emailModel = [[CreateShopModel alloc] init];
    emailModel.title = @"email";
    emailModel.placeholder = @"请输入email(选填)";
    emailModel.key = @"contactMail";
    emailModel.cellType = CreateShopCellType_TF;
    [_tableViewData addObject:emailModel];
    CreateShopModel *logoModel = [[CreateShopModel alloc] init];
    logoModel.title = @"网店LOGO";
    logoModel.placeholder = @"";
    logoModel.key = @"logo";
    logoModel.urlKey = @"logoUrl";
    logoModel.cellType = CreateShopCellType_PIC;
    [_tableViewData addObject:logoModel];
    CreateShopModel *coverPicModel = [[CreateShopModel alloc] init];
    coverPicModel.title = @"网店封面图";
    coverPicModel.placeholder = @"";
    coverPicModel.key = @"cover";
    coverPicModel.urlKey = @"coverUrl";
    coverPicModel.cellType = CreateShopCellType_PIC;
    [_tableViewData addObject:coverPicModel];
    if(_tableView){
        [_tableView reloadData];
    }
}
```
现在我们的数据源准备好了，但是`tableView`还没做处理呢，要等`tableView`也配套完成后再刷新`tableView`就OK了。我们来看`tableView`代理方法。
```
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _tableViewData.count;
}
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    CreateShopModel *createModel = _tableViewData[indexPath.row];
    if(createModel.cellType == CreateShopCellType_TF){
        return [CreateShopTFCell cellHeight:createModel];
    }else if(createModel.cellType == CreateShopCellType_TV){
        return [CreateShopTVCell cellHeight:createModel];
    }else if(createModel.cellType == CreateShopCellType_PICK){
        return [CreateShopPickCell cellHeight:createModel];
    }else if(createModel.cellType == CreateShopCellType_PIC){
        return [CreateShopUploadPicCell cellHeight: createModel];
    }
    return 50.f;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    CreateShopModel *createModel = _tableViewData[indexPath.row];
    if(createModel.cellType == CreateShopCellType_TF)
    {
        static NSString *tfCellId = @"tfCellId";
        CreateShopTFCell *cell = [tableView dequeueReusableCellWithIdentifier:tfCellId];
        if(cell==nil)
        {
            cell = [[CreateShopTFCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:tfCellId];
            cell.cellDelegate = self;
        }
        [cell refreshContent:createModel formModel:_shopFormModel];
        return cell;
    }
    else if(createModel.cellType == CreateShopCellType_TV)
    {
        static NSString *tvCellId = @"tvCellId";
        CreateShopTVCell *cell = [tableView dequeueReusableCellWithIdentifier:tvCellId];
        if(cell==nil)
        {
            cell = [[CreateShopTVCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:tvCellId];
        }
        [cell refreshContent:createModel formModel:_shopFormModel];
        return cell;
    }
    else if(createModel.cellType == CreateShopCellType_PICK)
    {
        static NSString *pickCellId = @"pickCellId";
        CreateShopPickCell *cell = [tableView dequeueReusableCellWithIdentifier:pickCellId];
        if(cell==nil)
        {
            cell = [[CreateShopPickCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:pickCellId];
        }
        NSString *valueStr = [_shopFormModel valueForKey:createModel.key];
        if(valueStr.length>0){
            createModel.errText = @"";
        }
        [cell refreshContent:createModel formModel:_shopFormModel];
        return cell;
    }
    else if(createModel.cellType == CreateShopCellType_PIC)
    {
        static NSString *picCellId = @"picCellId";
        CreateShopUploadPicCell *cell = [tableView dequeueReusableCellWithIdentifier:picCellId];
        if(cell==nil)
        {
            cell = [[CreateShopUploadPicCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:picCellId];
        }
        id value = [_shopFormModel valueForKey:createModel.key];
        if([value isKindOfClass:[NSString class]]){
            NSString *valueStr = (NSString *)value;
            if(valueStr.length>0){
                createModel.errText = @"";
            }
        }
        else if([value isKindOfClass:[UIImage class]]){
            UIImage *valueImg = (UIImage *)value;
            if(valueImg){
                createModel.errText = @"";
            }
        }
        __weak CreateShopViewController *weakSelf = self;
        [cell refreshContent:createModel formModel:_shopFormModel editBlock:^(CreateShopModel *shop) {
            if (shop) {
                _shopFormModel.indexPath = indexPath;
                _shopFormModel.indexPathObj = shop;
                [weakSelf iconActionSheet];
            }
        }];
        return cell;
    }
    return nil;
}
```
首先比较简单的，在设置行高的代理方法里，根据该行数据所表示的`cellType`类型来设置相应的行高。
然后在返回`cell`的代理方法里，同样以`cellType`来判断返回相应样式的`cell`，并给该`cell`赋相应的数据`model`。但是我们注意到，给`cell`赋值的方法，除了传入我们前面说定义的`CreateShopModel`类型的`createModel`外，还有个名叫`_shopFormModel`参数被传入。`_shopFormModel`是什么，它代表什么意思？
`_shopFormModel`是`CreateShopFormModel`类型的一个实例对象，它用来表示这个表单需要提交的数据，它里面的每个属性基本上对应着表单提交给服务器的字段。我们最后不是要将表单数据作为参数去请求提交的接口吗？表单数据从哪里来，就从`_shopFormModel`中来。那`_shopFormModel`中的数据从哪里来？
```
#import <Foundation/Foundation.h>
@interface CreateShopFormModel : NSObject
@property (nonatomic, copy)NSString            *groupId;
@property (nonatomic, copy)NSString            *groupName;
@property (nonatomic, copy)NSString            *tag;
@property (nonatomic, copy)NSString            *introduction;
@property (nonatomic, copy)NSString            *regionId;
@property (nonatomic, copy)NSString            *cityId;
@property (nonatomic, copy)NSString            *address;
@property (nonatomic, copy)NSString            *telephone;
@property (nonatomic, copy)NSString            *contactMail;
@property (nonatomic, copy)NSString            *coverUrl;
@property (nonatomic, copy)NSString            *logoUrl;
@property (nonatomic, strong)UIImage        *logo;
@property (nonatomic, strong)UIImage        *cover;
@property (nonatomic, strong)NSIndexPath    *indexPath;
@property (nonatomic, strong)id             indexPathObj;
+ (CreateShopFormModel *)formModelFromDict:(NSDictionary *)dict;
-(BOOL)submitCheck:(NSArray*)dataArr;
@end
```
以`CreateShopTFCell`为例，它所表示的字段的数据是我们在输入框输入的，也就是说数据来自`textField`，`_shopFormModel`对象在控制器被传入`cell`的`refreshContent:formModel:`方法，在该方法内部，将参数`formModel`赋给成员变量`_formModel`。需要格外注意的是，`_shopFormModel`、`formModel`和
```
_
 formModel
```
是同一个对象，指向的是同一块内存地址。方法传递对象参数时只是“引用拷贝”，拷贝了一份对象的引用。既然这样，我们可以预想到，我们在`cell`内部，将`textField`输入的值赋给`_formModel`所指向的对象后，也即意味着控制器里的`_shopFormModel`也有数据了，因为它们本来就是同一个对象嘛！
事实正是如此。
可以看到我们在给`textField`添加的通知的回调方法`textFiledEditChanged:`里，将`textField`输入的值以`KVC`的方式赋值给了`_formModel`。此时`_formModel`的某属性，即该`cell`对应的表单的字段已经有了数据。同样的，在控制器中与`_formModel`指向同一块内存地址的`_shopFormModel`也有了数据。
```
- (void)clearCellData
{
    _titleLab.text = @"";
    _textField.text = @"";
    _textField.placeholder = @"";
    _checkTipLab.text = @"";
}
- (void)refreshContent:(CreateShopModel *)createModel formModel:(CreateShopFormModel *)formModel
{
    [self clearCellData];
    if(!createModel){
        return;
    }
    _createModel = createModel;
    _formModel = formModel;
    _titleLab.text = createModel.title;
    _textField.placeholder = createModel.placeholder;
    _textField.text = [_formModel valueForKey:createModel.key]; // 将_formModel的值以KVC的方式赋给textField
    if(createModel.errText.length>0){
        _bgView.layer.borderColor = HexColor(0xcc2929).CGColor;
        _checkTipLab.text = createModel.errText;
    }else{
        _bgView.layer.borderColor = PDColor_Dividing_line.CGColor;
        _checkTipLab.text = @"";
    }
}
- (void)textFiledEditChanged:(NSNotification *)obj
{
    UITextField *textField = (UITextField *)obj.object;
    NSString *toBeString = textField.text;
    [_formModel setValue:textField.text forKey:_createModel.key]; // 将textField中的值赋给_formModel
    if(_createModel.maxInputLength&&toBeString.length>0&&toBeString.length>_createModel.maxInputLength){
        _bgView.layer.borderColor = HexColor(0xcc2929).CGColor;
        _checkTipLab.text = [NSString stringWithFormat:@"最多%d个字",(int)_createModel.maxInputLength];
    }else{
        _bgView.layer.borderColor = PDColor_Dividing_line.CGColor;
        _checkTipLab.text = @"";
    }
    if([_createModel.key isEqualToString:@"contactMail"]){
        _createModel.errText = @"";
    }else{
        NSString *valueStr = [_formModel valueForKey:_createModel.key];
        if(valueStr.length>0){
            _createModel.errText = @"";
        }
    }
}
```
我们看到在`refreshContent:formModel:`方法中，`cell`上的死数据是被`CreateShopModel`的实例对象`createModel`赋值的，而在其后我们又以`KVC`的方式又将`_shopFormModel`的某属性的值赋给了`textField`。这是因为我们为了防止`cell`在复用的过程中出现数据错乱的问题，而在给`cell`赋值前先将每个视图上的数据都清空了（即`clearCellData`方法），需要我们重新赋过。（不过，如果你没清空数据的情况下，不再次给`textField`赋值好像也是没问题的。不会出现数据错乱和滑出屏幕再滑回来时从复用池取出`cell`后赋值时数据消失的问题。）
### 输入长度的限制：
需求中要求“网店名称”、“网店主标签”、“网店简介”、“网店座机”都有输入长度的限制，分别为30、20、500、15字数的限制。其实我们在上面初始化数据源的时候已经为每行的数据源`model`设置过字数限制了，即`maxInputLength`属性。
我们还是以`CreateShopTFCell`为例。
要在开始输入的时候监听输入的长度，若字数超过最大限制，则要出现红框，并且显示提示信息。那我们就得给`textField`开始输入时添加`valueChange`的观察，在`textField`输入结束时移除观察。
```
- (void)textFieldDidEndEditing:(UITextField *)textField
{
    [self clearNotification];
}
- (void)textFiledEditChanged:(NSNotification *)obj
{
    UITextField *textField = (UITextField *)obj.object;
    NSString *toBeString = textField.text;
    [_formModel setValue:textField.text forKey:_createModel.key]; // 将textField中的值赋给_formModel
    if(_createModel.maxInputLength&&toBeString.length>0&&toBeString.length>_createModel.maxInputLength){
        _bgView.layer.borderColor = HexColor(0xcc2929).CGColor;
        _checkTipLab.text = [NSString stringWithFormat:@"最多%d个字",(int)_createModel.maxInputLength];
    }else{
        _bgView.layer.borderColor = PDColor_Dividing_line.CGColor;
        _checkTipLab.text = @"";
    }
    if([_createModel.key isEqualToString:@"contactMail"]){
        _createModel.errText = @"";
    }else{
        NSString *valueStr = [_formModel valueForKey:_createModel.key];
        if(valueStr.length>0){
            _createModel.errText = @"";
        }
    }
}
-(void)addNotification
{
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(textFiledEditChanged:)
                                                name:@"UITextFieldTextDidChangeNotification"
                                              object:nil];
}
-(void)clearNotification{
    [[NSNotificationCenter defaultCenter]removeObserver:self
                                                   name:@"UITextFieldTextDidChangeNotification"
                                                 object:nil];
}
```
另外，可以看到在`textField`开始输入的回调方法里，调用了该`cell`的代理方法。该`cell`为什么要调用这个代理方法，它需要代理给别人来干什么？...其实这个和键盘遮挡的处理有关，下面我们慢慢解释。
### 处理键盘遮挡问题：
这个界面有很多行输入框，在自然情况下，下面的几个输入框肯定是在键盘弹出后高度之下的，也即会被键盘遮挡住，我们没法输入。这时就一定处理键盘遮挡问题了。
关于键盘遮挡问题，其实我在以前的一篇笔记中就写过了：[UITextField一箩筐——输入长度限制、自定义placeholder、键盘遮挡问题](http://www.jianshu.com/p/d127225aece9)
我们要处理键盘遮挡问题，也就是要实现当键盘弹出时，被遮挡住的输入框能上移到键盘高度之上；当键盘收回时，输入框又能移回原来的位置。那么首先第一步，我们得能获取到键盘弹出或者收回这个动作的时机，在这个时机我们再按需要移动输入框的位置。系统提供了表示键盘弹出和收回的两个观察的`key`，分别为`UIKeyboardWillShowNotification`和`UIKeyboardWillHideNotification`。注册这两个观察者，然后在两者的回调方法里实现输入框位移就大功告成了。
因为键盘遮挡的处理有可能是比较普遍的需求，所以在公司的项目架构设计里是把上面两个关于键盘的观察是注册在`APPDelegate.m`中的，并定义了一个有关键盘遮挡处理的协议，协议里定义了一个方法。具体需要具体处理，由需要处理键盘遮挡问题的控制器来实现该协议方法，具体实现怎么移动界面元素来使键盘不遮挡输入框。这么说现在`CreateShopViewController`控制器需要处理键盘遮挡问题，那么就需要设置它为`APPDelegate`的代理，并由它实现所定义的协议吗？其实不用，公司项目所有的控制器都是继承于基类`CommonViewController`，在基类中实现了比较基本和普遍的功能，其实在基类中便定义了下面的方法来设置控制器为`APPDelegate`的代理，不过需要属性`isListensKeyboard`为`YES`。下面这个方法在`CommonViewController`中是在`viewWillAppear:`方法中调用的。那我们在子类`CreateShopViewController`中需要做的仅仅只要在`viewWillAppear`之前设置`isListensKeyboard`属性为`YES`，便会自动设置将自己设为`APPDelegate`的代理。然后在`CreateShopViewController`控制器里实现协议所定义的方法，实现具体的输入框移动问题。
CommonViewController.m
```
-(void)initListensKeyboardNotificationDelegate
{
    if (!self.isListensKeyboard) {
        return;
    }
    if (!self.appDelegate) {
        self.appDelegate=(AppDelegate*)[[UIApplication sharedApplication] delegate];
    }
    [self.appDelegate setKeyboardDelegate:self];
}
```
CreateShopViewController.m
```
#pragma mark - keyboard delegate
- (void)keyboardChangeStatus:(KeyboardChangeType)changeType
                  beginFrame:(CGRect)beginFrame
                    endFrame:(CGRect)endFrame
                    duration:(CGFloat)duration
                    userInfo:(NSDictionary *)info
{
    if(changeType == KeyboardWillShow)
    {
        CGFloat keyBoard_h = [info[UIKeyboardFrameEndUserInfoKey] CGRectValue].size.height;
        CGFloat newSizeh = _tableView.contentSize.height + keyBoard_h;
        [UIView animateWithDuration:duration animations:^{
            [_tableView setContentSize:CGSizeMake(PDWidth_mainScreen, newSizeh)];
            CGFloat set_y = _inputY+50.f+keyBoard_h-_tableView.bounds.size.height;
            if(set_y>0){
                [_tableView setContentOffset:CGPointMake(0, set_y)];
            }
        }];
    }
    else if(changeType == KeyboardWillHide)
    {
        [UIView animateWithDuration:duration animations:^{
            [_tableView setContentSize:CGSizeMake(PDWidth_mainScreen, _tableView.contentSize.height)];
        }];
    }
}
```
可以看到在该代理方法的实现里。当键盘弹出时，我们首先将`tableView`的`contentSize`在原来的基础上增加了键盘的高度`keyBoard_h`。然后将`tableView`的`contentOffset`值变为`set_y`，这个`set_y`的值是通过计算而来，但是计算它的`_inputY`这个变量代表什么意思？
我们可以回过头去看看`tableView`返回`cell`的代理方法中，当为`CreateShopTFCell`时，我们设置了当前控制器为其`cell`的代理。
`cell.cellDelegate = self;`
并且我们的控制器`CreateShopViewController`也实现了该`cell`的协议`CreateShopTFCellDelegate`，并且也实现了协议定义的方法。
```cpp
```cpp
#pragma mark - tfCell delegate
- (void)cellBeginInputviewY:(CGFloat)orginY
{
    _inputY = orginY;
}
```
```
原来上面的`_intputY`变量就是该协议方法从`cell`里的调用处传递而来的`orginY`参数值。我们回过头看上面的代码，该协议方法是在`textField`的开始输入的回调方法里调用的，给协议方法传入的参数是`self.frame.origin.y`，即被点击的`textField`在手机屏幕内所在的`Y`坐标值。
可以看到，处理键盘遮挡问题，其实也不是改变输入框的坐标位置，而是变动`tableView`的`contentSize`和`contentOffset`属性。
### 选取地址的实现：
`CreateShopPickCell`实现里地址的选取和显示。有左右两个框框，点击任何一个将会从屏幕下方弹出一个选取器，选取器有“市”和“区”两列数据对应两个框框，选取器左上方是“取消”按钮，右上方是“确定”按钮。点击“取消”，选取器弹回，并不进行选取；点击“确定”，选取器弹回，选取选择的数据。
![](http://upload-images.jianshu.io/upload_images/988593-a0c43e787b6c7fba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/320)
WechatIMG1.png
`CreateShopPickCell`的界面元素布局没什么可说的，值得一说的是弹出的`pickView`视图，是在`cell`的填充数据的方法中创建的。
```
- (void)refreshContent:(CreateShopModel *)createModel formModel:(CreateShopFormModel *)formModel;
{
    [self clearCellData];
    if(!createModel){
        return;
    }
    [self createPickerView]; // 创建pickView
    _createModel = createModel;
    _formModel = formModel;
    _titleLab.text = createModel.title;
    if(formModel.regionId.length>0){
        ShopAddressModel *area=[[ShopAddressModel alloc]init];
        area.addresssId=formModel.regionId;
        [_pickView dafaultArea:area];
    }else{
        _cityLab.text = @"市";
        _cityLab.textColor = HexColor(0xc8c8c8);
        _areaLab.text = @"区";
        _areaLab.textColor = HexColor(0xc8c8c8);
    }
    if(createModel.errText.length>0){
        _cityBgView.layer.borderColor = HexColor(0xcc2929).CGColor;
        _areaBgView.layer.borderColor = HexColor(0xcc2929).CGColor;
        _checkTipLab.text = createModel.errText;
    }else{
        _cityBgView.layer.borderColor = PDColor_Dividing_line.CGColor;
        _areaBgView.layer.borderColor = PDColor_Dividing_line.CGColor;
        _checkTipLab.text = @"";
    }
}
```
这里只是创建了`pickView`的对象，并设置了数据源`items`，已经点击之后的回调`block`，而并未将其添加在父视图上。
要将选取的“市&区”的结果从`CustomPickView`中以`block`回调到`cell`来，将数据赋给`_formModel`。并且当有了数据后`UILabel`的文本颜色也有变化。
```
-(void)createPickerView
{
    if (!_pickView) {
        _pickView= [[CustomPickView alloc] init];
    }
    [_pickView setItems:[ShopAddressModel cityAddressArr]];
    [_pickView SelectedBlock:^(ShopAddressModel *city, ShopAddressModel *area) {
        if (city) {
            [_formModel setValue:city.addresssId forKey:_createModel.key];
            _cityLab.text = city.name;
            _cityLab.textColor = PDColor_Title_Black;
        }
        if (area) {
            [_formModel setValue:area.addresssId forKey:_createModel.key];
            _areaLab.text = area.name;
            _areaLab.textColor = PDColor_Title_Black;
        }
        if(city){
            _cityBgView.layer.borderColor = PDColor_Dividing_line.CGColor;
            _areaBgView.layer.borderColor = PDColor_Dividing_line.CGColor;
            _checkTipLab.text = @"";
            _createModel.errText=@"";
        }else{
            _cityBgView.layer.borderColor = HexColor(0xcc2929).CGColor;
            _areaBgView.layer.borderColor = HexColor(0xcc2929).CGColor;
            _checkTipLab.text = _createModel.errText;
        }
    }];
}
```
`pickView`的对象已经创建好，但是还未到弹出显示的时机。所谓时机，就是当左右两个框框被点击后。
可以看到`pickView`是被添加在`window`上的。并且调用了`pickView`的接口方法`showPickerView`方法，让其从屏幕底部弹出来。
```
- (void)cityGestureHandle:(UITapGestureRecognizer *)tapGesture
{
    [_superView endEditing:YES];
    [self showPicker];
}
- (void)areaGestureHandle:(UITapGestureRecognizer *)tapGesture
{
    [_superView endEditing:YES];
    [self showPicker];
}
-(void)showPicker
{
    [[PubicClassMethod getCurrentWindow] addSubview:_pickView];
    [_pickView showPickerView];
}
```
前面代码中给`pickView`设置数据源时，它的数据源有点特别，调用了`ShopAddressModel`的类方法`cityAddressArr`来返回有关地址的数据源数组。这是因为这里的地址数据虽然是从服务器接口请求的，但是一般情况不会改变，最好是从服务器拿到数据后缓存在本地，当请求失败或者无网络时仍不受影响。
`ShopAddressModel`类定义了如下几个属性和方法。
```
@interface ShopAddressModel : NSObject
@property (nonatomic, copy)NSString            *addresssId;
@property (nonatomic, copy)NSString            *name;
@property (nonatomic, strong)NSArray        *subArr;
#pragma mark - 地址缓存
+ (void)saveAddressArr:(NSArray *)addressArr;
+(NSArray*)cityAddressArr;
+(NSArray*)addressArr;
#pragma mark - 解析
+ (ShopAddressModel *)addressModelFromDict:(NSDictionary *)dict;
@end
```
当我们我们从服务器拿到返回而来的地址数据后，调用`saveAddressArr:`方法，将数据缓存在本地。
```
+ (void)saveAddressArr:(NSArray *)addressArr
{
    if (addressArr && addressArr.count > 0) {
        NSData *data = [NSKeyedArchiver archivedDataWithRootObject:addressArr];
        [[NSUserDefaults standardUserDefaults] setObject:data forKey:@"saveAddressArr"];
    }else
    {
        [[NSUserDefaults standardUserDefaults]setObject:nil forKey:@"saveAddressArr"];
    }
    [[NSUserDefaults standardUserDefaults] synchronize];
}
```
当创建好`pickView`后以下面方法将本地缓存数据读出，赋给`items`作为数据源。
```
+(NSArray*)cityAddressArr
{
    NSArray *arr=[ShopAddressModel addressArr];
    ShopAddressModel *pro=[arr firstObject];
    if (pro.subArr.count>0) {
        return pro.subArr;
    }
    return nil;
}
+(NSArray*)addressArr
{
    NSData *data = [[NSUserDefaults standardUserDefaults] valueForKey:@"saveAddressArr"];
    NSArray *addrssArr=[NSKeyedUnarchiver unarchiveObjectWithData:data];
    if(addrssArr.count==0)
    {
        return nil;
    }
    NSMutableArray *areas=[[NSMutableArray alloc]init];
    for (int i=0; i<addrssArr.count; i++) {//省
        ShopAddressModel *prov=[ShopAddressModel  addressModelFromDict:addrssArr[i]];
        if (!prov) {
            continue;
        }
        NSMutableArray *cArr=[[NSMutableArray alloc]init];
        for (int j=0; j<prov.subArr.count; j++) {//市
            ShopAddressModel *city=[ShopAddressModel  addressModelFromDict:prov.subArr[j]];
            if (!city) {
                continue;
            }
            NSMutableArray *aArr=[[NSMutableArray alloc]init];
            for (int a=0; a<city.subArr.count; a++) {//区
                ShopAddressModel *are=[ShopAddressModel  addressModelFromDict:city.subArr[a]];
                if (!are) {
                    continue;
                }
                [aArr addObject:are];
            }
            if (aArr.count>0) {//市
                city.subArr=[NSArray arrayWithArray:aArr];
            }
            [cArr addObject:city];
         }
        if (cArr.count>0) {//省
            prov.subArr=[NSArray arrayWithArray:cArr];
        }
       [areas addObject:prov];
    }
    return areas;
}
```
注意：这也是为什么把创建`pickView`的代码放在了填充`cell`数据的`refreshContent:formModel:`里，而不在创建`cell`界面元素时一气创建`pickView`。因为那样当用户第一次打开这个界面，有可能数据来的比较慢，当代码执行到赋数据源`items`时，本地还没有被缓存上数据呢！这样用户第一次进入这个界面时弹出的`pickView`是空的，没有数据。而放在`refreshContent:formModel:`中是安全稳妥的原因是，每次从接口拿到数据后我们会刷新`tableView`，便会执行`refreshContent:formModel:`方法。它能保证先拿到数据，再设置数据源的顺序。
### 提交表单时校验数据：
在将表单数据提交前，要先校验所填写的表单是否有问题，该填的是否都填了，已填的数据格式是否是对的。若有问题，则要出现红框和提示信息提醒用户完善，等数据无误后才可以提交给服务器。
数据校验代码很繁长，写在控制器里不太好。因为它是对表单数据的校验，那我们就写在`CreateShopFormModel`里，这样既可以给控制器瘦身，也可以降低耦合度，数据的归数据，逻辑的归逻辑。
从前面`CreateShopFormModel.h`的代码里我们其实已经看到了这个校验方法：`submitCheck:`。若某条`CreateShopFormModel`实例的数据不达要求，则在相应的`CreateShopModel`数据源对象的`errText`属性赋值，意为提示信息。该方法的返回值类型为`BOOL`值，有数据不合格则返回`NO`。此时，在调用该方法的外部，应该将`tableView`重新加载，因为此时在该方法内部，已将数据格式不合格的提示信息赋值给了相应的数据源`model`。
```
- (BOOL)submitCheck:(NSArray*)dataArr
{
    BOOL isSubmit=YES;
    if(self.groupName.length==0){
        if (dataArr.count>0) {
            CreateShopModel *cellObj=dataArr[0];
            cellObj.errText=@"网店名不能为空";
        }
        isSubmit=NO;
    }
    if(self.groupName.length>0){
        if(dataArr.count>0){
            if(self.groupName.length>30){
                CreateShopModel *cellObj=dataArr[0];
                cellObj.errText=@"最多30个字";
                isSubmit=NO;
            }
        }
    }
    if(self.tag.length==0){
        if (dataArr.count>1) {
            CreateShopModel *cellObj=dataArr[1];
            cellObj.errText=@"标签不能为空";
        }
        isSubmit=NO;
    }
    if(self.introduction.length==0){
        if (dataArr.count>2) {
            CreateShopModel *cellObj=dataArr[2];
            cellObj.errText=@"简介不能为空";
        }
        isSubmit=NO;
    }
    if(self.introduction.length>0){
        if(dataArr.count>2){
            if(self.introduction.length>30){
                CreateShopModel *cellObj=dataArr[2];
                cellObj.errText=@"最多500个字";
                isSubmit=NO;
            }
        }
    }
    if(self.regionId.length==0){
        if (dataArr.count>3) {
            CreateShopModel *cellObj=dataArr[3];
            cellObj.errText=@"市区不能为空";
        }
        isSubmit=NO;
    }
    if(self.address.length==0){
        if (dataArr.count>4) {
            CreateShopModel *cellObj=dataArr[4];
            cellObj.errText=@"地址不能为空";
        }
        isSubmit=NO;
    }
    if(self.telephone.length==0){
        if (dataArr.count>5) {
            CreateShopModel *cellObj=dataArr[5];
            cellObj.errText=@"电话不能为空";
        }
        isSubmit=NO;
    }
    if (self.contactMail.length>0) {
        if (dataArr.count>6) {
            CreateShopModel *cellObj=dataArr[6];
            if(![PubicClassMethod isValidateEmail:self.contactMail]){
                cellObj.errText=@"邮箱格式不合法";
                isSubmit=NO;
            }
        }
    }
    if(self.logoUrl.length==0&&!self.logo){
        if (dataArr.count>7) {
            CreateShopModel *cellObj=dataArr[7];
            cellObj.errText=@"logo不能为空";
        }
        isSubmit=NO;
    }
    if(self.coverUrl.length==0&&!self.cover){
        if (dataArr.count>8) {
            CreateShopModel *cellObj=dataArr[8];
            cellObj.errText=@"封面图不能为空";
        }
        isSubmit=NO;
    }
    return isSubmit;
}
```
### 上传图片到七牛：
当点击了“提交”按钮后，先校验数据，若所填写的数据不合格，则给出提示信息，让用户继续完善数据；若数据无问题，校验通过，则开始提交表单。但是，这里有图片，图片我们是上传到七牛服务器的，提交表单是图片项提交的应该是图片在七牛的一个`url`。这个逻辑我在以前的这篇笔记已经捋过了[APP上传图片至七牛的逻辑梳理](http://www.jianshu.com/p/c2fd1bed6a4c)。
但是当时所有的逻辑都是写在控制器里的。我们这个“创建网店”的控制器已经很庞大了，写在控制器里不太好。所以在这里我将上传图片的逻辑拆分了出去，新建了一个类``QNUploadPicManager`。只暴露一个允许传入`UIImage`参数的接口方法，便可以通过`successBlock`来返回上传到七牛成功后的`url`。以及通过`failureBlock`来返回上传失败后的`error`信息。而将所有的逻辑封装在`QNUploadPicManager`内部，这样控制器里便精简了不少代码，清爽了许多。
QNUploadPicManager.h
```
@interface QNUploadPicManager : NSObject
- (void)uploadImage:(UIImage *)image successBlock:(void(^)(NSString *urlStr))successBlock failureBlock:(void(^)(NSError *error))failureBlock;
@end
```
QNUploadPicManager.m
```
#import "QNUploadManager.h"
#define kImageFilePath(name) [NSTemporaryDirectory() stringByAppendingPathComponent:name] // 图片路径
@implementation QNUploadPicManager
- (void)uploadImage:(UIImage *)image successBlock:(void(^)(NSString *urlStr))successBlock failureBlock:(void(^)(NSError *error))failureBlock
{
    NSString *logoFileName = [self fileNameWithPicture:image]; // fileName
    [self requestUploadToken:logoFileName successBlock:^(NSDictionary *dict) {
        [self uploadPicOnQNParameters:dict fileName:logoFileName complete:^(NSString *key, NSDictionary *resp) {
            [self getPictureUrlOnQN:key successBlock:^(NSString *urlStr) {
                successBlock(urlStr); // 成功回调
            } failure:^(NSError *error) {
                failureBlock(error);
            }];
        }];
    } failure:^(NSError *error) {
        failureBlock(error); // token获取失败回调
    }];
}
// get token
- (void)requestUploadToken:(NSString *)fileName successBlock:(void(^)(NSDictionary *dict))successBlock failure:(void(^)(NSError *error))failureBlock
{
    NSDictionary * parameters=[[NSDictionary alloc] initWithObjectsAndKeys:
                               @(1), @"count",
                               nil];
    NSString *url = [NSString stringWithFormat:@"%@/cbs/%@/upload/token",HTTPSURLEVER,Interface_Version];
    url =[url stringByReplacingOccurrencesOfString:@"http" withString:@"https"];
    AFHTTPRequestOperationManager *mgr = [AFHTTPRequestOperationManager manager];
    [mgr.requestSerializer setValue:[[REDUserModel shareInstance] token] forHTTPHeaderField:@"x-auth-token"];
    [mgr.requestSerializer setValue:@"ios" forHTTPHeaderField:@"_c"];
    [mgr.securityPolicy setAllowInvalidCertificates:YES];
    [mgr GET:url parameters:parameters success:^(AFHTTPRequestOperation *operation, id responseObject) {
        // key:5425734430926807040
        successBlock(DealWithJSONValue(responseObject[@"b"][0]));
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        failureBlock(error);
    }];
}
// upload on QN
- (void)uploadPicOnQNParameters:(NSDictionary *)parameters fileName:(NSString *)fileName complete:(void(^)(NSString *key, NSDictionary *resp))complete
{
    QNUploadManager *uploader = [[QNUploadManager alloc] init];
    // 异步多线程
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSString *token = parameters[@"uploadToken"];
        NSString *key = parameters[@"key"];
        [uploader putFile:kImageFilePath(fileName) key:key token:token complete:^(QNResponseInfo *info, NSString *key, NSDictionary *resp) {
            // key:5425734430926807040
            complete(key, resp);
        } option:nil];
    });
}
//获取上传图片的url
- (void)getPictureUrlOnQN:(NSString *)token successBlock:(void(^)(NSString *urlStr))successBlock failure:(void(^)(NSError *error))failureBlock
{
    if(!token){
        token = @"";
    }
    NSDictionary * parameters=[[NSDictionary alloc] initWithObjectsAndKeys:
                               token, @"token",
                               nil];
    NSString *url = [NSString stringWithFormat:@"%@/cbs/%@/upload/url",HTTPSURLEVER,Interface_Version];
    url =[url stringByReplacingOccurrencesOfString:@"http" withString:@"https"];
    AFHTTPRequestOperationManager *mgr = [AFHTTPRequestOperationManager manager];
    [mgr.requestSerializer setValue:[[REDUserModel shareInstance] token] forHTTPHeaderField:@"x-auth-token"];
    [mgr.requestSerializer setValue:@"ios" forHTTPHeaderField:@"_c"];
    [mgr.securityPolicy setAllowInvalidCertificates:YES];
    [mgr GET:url parameters:parameters success:^(AFHTTPRequestOperation *operation, id responseObject) {
        if (!responseObject[@"b"] || [responseObject[@"b"] isEqual:[NSNull null]]) {
            return;
        }
        successBlock(DealWithJSONStringValue(responseObject[@"b"][@"url"]));
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        failureBlock(error);
    }];
}
// save in file
- (NSString *)fileNameWithPicture:(UIImage *)image
{
    if(!image){
        return @"";
    }
    UIImage *newImage = [PubicClassMethod imageWithImageSimple:image scaledToSize:CGSizeMake(80, 80)]; // 图片压缩
    NSData *imageData = UIImageJPEGRepresentation(newImage, 1);
    NSString *fileName = [NSString stringWithFormat:@"%d.png",arc4random()];
    BOOL isWrited = [imageData writeToFile:kImageFilePath(fileName) atomically:YES];
    if(isWrited){
        return fileName;
    }
    return @"";
}
@end
```
### 总结：
这个界面比较核心的一个问题就是：要在控制器里提交表单，那怎样把在`UITableViewCell`里的`textField`输入的数据传递给控制器？
 另外一个问题是一个逻辑比较复杂的界面，控制器势必会很庞大，应该有意的给控制器瘦身，不能把所有的逻辑都写在控制器里。有关视图显示的就考虑放入`UITableViewCell`，有关数据的就考虑放入`model`。这样既为控制器瘦身，也使代码职责变清晰，耦合度降低。
另外，今天2016最后一天班了，周日就坐车回家过年了。提前祝各位新春快乐。
