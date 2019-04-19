# iOS常用的封装方法 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月17日 13:46:17[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1058

  做开发也有一段时间了，看了好多大神的代码，总体感觉他们写的代码简洁，好看，然而在对比下我写的代码，混乱，无序，简直不堪入目啊！
                总体来说大神们的代码封装的都比较好，对一个项目要重复用到的代码他们都会封装起来，这样用起来的时候也比较方便，也不用重复的去写这段代码了！
               下面是我在开发中封装到的一些经常使用的一些方法，如有写错的地方或者大家有更好的方法，还请大家给我提出来，大家一起学习！谢谢！
1.常用控件方法的封装：
#import<Foundation/Foundation.h>
#import<UIKit/UIKit.h>
@interface MyUtil :NSObject
//创建标签的方法
+ (UILabel *)createLabelFrame:(CGRect)frame text:(NSString
 *)text textColor:(UIColor *)textColor textAlignment:(NSTextAlignment)textAlignment numberOfLines:(NSInteger)numberOfLines;
+ (UILabel *)createLabelFrame:(CGRect)frame text:(NSString
 *)text color:(UIColor *)textColor;
//创建按钮的方法
+ (UIButton *)createBtnFrame:(CGRect)frame title:(NSString
 *)title bgImageName:(NSString *)bgImageName target:(id)target action:(SEL)action;
//创建图片视图的方法
+ (UIImageView *)createImageViewFrame:(CGRect)frame imageName:(NSString
 *)imageName;
//类型的英文转化成中文
+ (NSString *)transferCateName:(NSString *)name;
@end
方法的实现：
#import"MyUtil.h"
@implementation MyUtil
+(UILabel *)createLabelFrame:(CGRect)frame text:(NSString
 *)text textColor:(UIColor *)textColor textAlignment:(NSTextAlignment)textAlignment numberOfLines:(NSInteger)numberOfLines
{
UILabel *label = [[UILabelalloc]initWithFrame:frame];
    label.text = text;
    label.textAlignment = textAlignment;
    label.textColor = textColor;
    label.numberOfLines = numberOfLines;
return label;
}
+(UILabel *)createLabelFrame:(CGRect)frame text:(NSString
 *)text color:(UIColor *)textColor
{
return [selfcreateLabelFrame:frametext:texttextColor:textColortextAlignment:NSTextAlignmentCenternumberOfLines:1];
}
+(UIButton *)createBtnFrame:(CGRect)frame title:(NSString
 *)title bgImageName:(NSString *)bgImageName target:(id)target action:(SEL)action
{
UIButton *btn = [UIButtonbuttonWithType:UIButtonTypeCustom];
    btn.frame = frame;
    [btnsetTitle:titleforState:UIControlStateNormal];
    [btnsetTitleColor:[UIColorblackColor]forState:UIControlStateNormal];
    [btnsetBackgroundImage:[UIImageimageNamed:bgImageName]forState:UIControlStateNormal];
    [btnaddTarget:targetaction:actionforControlEvents:UIControlEventTouchUpInside];
return btn;
}
+(UIImageView *)createImageViewFrame:(CGRect)frame imageName:(NSString
 *)imageName
{
UIImageView *imageView = [[UIImageViewalloc]initWithFrame:frame];
    imageView.image = [UIImageimageNamed:imageName];
return imageView;
}
+ (NSString *)transferCateName:(NSString *)name
{
if ([name isEqualToString:@"Business"]) {
return@"商业";
    }elseif ([nameisEqualToString:@"Weather"])
 {
return@"天气";
    }elseif ([nameisEqualToString:@"Tool"])
 {
return@"工具";
    }elseif ([nameisEqualToString:@"Travel"])
 {
return@"旅行";
    }elseif ([nameisEqualToString:@"Sports"])
 {
return@"体育";
    }elseif ([nameisEqualToString:@"Social"])
 {
return@"社交";
    }elseif ([nameisEqualToString:@"Refer"])
 {
return@"参考";
    }elseif ([nameisEqualToString:@"Ability"])
 {
return@"效率";
    }elseif ([nameisEqualToString:@"Photography"])
 {
return@"摄影";
    }elseif ([nameisEqualToString:@"News"])
 {
return@"新闻";
    }elseif ([nameisEqualToString:@"Gps"])
 {
return@"导航";
    }elseif ([nameisEqualToString:@"Music"])
 {
return@"音乐";
    }elseif ([nameisEqualToString:@"Life"])
 {
return@"生活";
    }elseif ([nameisEqualToString:@"Health"])
 {
return@"健康";
    }elseif ([nameisEqualToString:@"Finance"])
 {
return@"财务";
    }elseif ([nameisEqualToString:@"Pastime"])
 {
return@"娱乐";
    }elseif ([nameisEqualToString:@"Education"])
 {
return@"教育";
    }elseif ([nameisEqualToString:@"Book"])
 {
return@"书籍";
    }elseif ([nameisEqualToString:@"Medical"])
 {
return@"医疗";
    }elseif ([nameisEqualToString:@"Catalogs"])
 {
return@"商品指南";
    }elseif ([nameisEqualToString:@"FoodDrink"])
 {
return@"美食";
    }elseif ([nameisEqualToString:@"Game"])
 {
return@"游戏";
    }elseif([nameisEqualToString:@"All"]){
return@"全部";
    }
returnnil;
}
@end
2.裁剪图片方法的封装：
- (UIImage*)clipImage:(UIImage*)bigImage
 inRect:(CGRect)rect
{
CGImageRefimageRef =CGImageCreateWithImageInRect(bigImage.CGImage,
 rect);
UIImage*image = [UIImageimageWithCGImage:imageRef];
returnimage;
}
实现方法：
//创建图片显示
CGRect frame =CGRectMake(100*j,100*i,100,100);
UIImage *tmpImage = [selfclipImage:imageinRect:frame];
3.封装的下载方法：
#import
<Foundation/Foundation.h>
@classMyDownloader;
@protocol MyDownloaderDelegate <NSObject>
- (void)downloadFail:(MyDownloader *)downloader error:(NSError
 *)error;
- (void)downloadFinish:(MyDownloader *)downloader;
@end
@interface MyDownloader :NSObject<NSURLConnectionDelegate,NSURLConnectionDataDelegate>
- (void)downloadWithURLString:(NSString *)urlString;
@property (nonatomic,readonly)NSData
 *receiveData;
@property (nonatomic,assign)NSInteger
 type;
@property (nonatomic,weak)id<MyDownloaderDelegate>
 delegate;
@end
实现方法：
#import
"MyDownloader.h"
@implementation MyDownloader
{
NSURLConnection *_conn;
NSMutableData *_receiveData;
}
-(instancetype)init
{
self = [superinit];
if (self) {
_receiveData = [NSMutableDatadata];
    }
returnself;
}
-(void)downloadWithURLString:(NSString *)urlString
{
NSURL *url = [NSURLURLWithString:urlString];
NSURLRequest *request = [NSURLRequestrequestWithURL:url];
_conn = [NSURLConnectionconnectionWithRequest:requestdelegate:self];
}
#pragma mark - NSURLConnection代理
-(void)connection:(NSURLConnection *)connection didFailWithError:(NSError
 *)error
{
    [self.delegatedownloadFail:selferror:error];
}
-(void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse
 *)response
{
    [_receiveDatasetLength:0];
}
-(void)connection:(NSURLConnection *)connection didReceiveData:(NSData
 *)data
{
    [_receiveDataappendData:data];
}
-(void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    [self.delegatedownloadFinish:self];
}

4.block封装的下载方法：
#import
<Foundation/Foundation.h>
typedefvoid (^FINISHBLOCK)(NSData *receiveData);
typedefvoid (^FAILBLOCK)(NSError *error);
@interface MyDownloader :NSObject<NSURLConnectionDataDelegate,NSURLConnectionDelegate>
{
NSMutableData *_receiveData;
NSURLConnection *_conn;
//下载成功结束
//void (^_myFinishBlock)(NSData *data);
FINISHBLOCK _myFinishBlock;
//下载失败时调用
//void (^_myFailBlock)(NSError *error);
FAILBLOCK _myFailBlock;
}
//下载的方法
/*
 @param urlString:下载的字符串链接
 @param finishBlock:下载成功时调用的代码块
 @param failBlock:下载失败时调用的代码块
 */
/*
- (void)downloadWithURLString:(NSString *)urlString
                       finish:(void (^) (NSData *receiveData))finishBlock
                         fail:(void (^)(NSError *error))failBlock;
 */
- (void)downloadWithURLString:(NSString *)urlString
                       finish:(FINISHBLOCK)finishBlock
                         fail:(FAILBLOCK)failBlock;

@end
实现方法：
#import
"MyDownloader.h"
@implementation MyDownloader
-(instancetype)init
{
if (self = [superinit]) {
_receiveData = [NSMutableDatadata];
    }
returnself;
}
-(void)downloadWithURLString:(NSString *)urlString finish:(void
 (^)(NSData *))finishBlock fail:(void (^)(NSError *))failBlock
{
if (_myFinishBlock != finishBlock) {
_myFinishBlock = finishBlock;
    }
if (_myFailBlock != failBlock) {
_myFailBlock = failBlock;
    }
//下载
NSURL *url = [NSURLURLWithString:urlString];
NSURLRequest *request = [NSURLRequestrequestWithURL:url];
_conn = [NSURLConnectionconnectionWithRequest:requestdelegate:self];
}
#pragma mark - NSURLConnection代理
-(void)connection:(NSURLConnection *)connection didFailWithError:(NSError
 *)error
{
if (_myFailBlock) {
_myFailBlock(error);
    }
}
-(void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse
 *)response
{
    [_receiveDatasetLength:0];
}
-(void)connection:(NSURLConnection *)connection didReceiveData:(NSData
 *)data
{
    [_receiveDataappendData:data];
}
-(void)connectionDidFinishLoading:(NSURLConnection *)connection
{
if (_myFinishBlock) {
_myFinishBlock(_receiveData);
    }
}
@end
5.数据库的封装：
#import
<Foundation/Foundation.h>
#import
"UserModel.h"
@interface DBManager :NSObject
+ (DBManager *)sharedManager;
//增
- (void)addUserModel:(UserModel *)model;
//删
- (void)deleteUserWithUserId:(int)userId;
//改
//根据userId修改数据(userId是不变化的)
- (void)updateUserModel:(UserModel *)model userId:(int)userId;
//查
- (NSArray *)searchAllUsers;
@end
实现方法：
#import
"DBManager.h"
#import
"FMDatabase.h"
/*
使用数据库时
 1.导入fmdatabase第三方库
 2.导入系统libsqlite3.dylib
 3.fmdatabase不支持ARC，五个.m文件添加-fno-objc-arc
 */
@implementation DBManager
{
//数据库文件的关联对象
FMDatabase *_dataBase;
}
+(DBManager *)sharedManager
{
staticDBManager *manager =nil;
@synchronized(self){
if (manager ==nil) {
            manager = [[DBManageralloc]init];
        }
    }
return manager;
}
-(instancetype)init
{
if (self = [superinit]) {
//创建数据库文件操作对象
        [selfcreateDatabase];
    }
returnself;
}
- (void)createDatabase
{
//数据库文件的路径
NSString *path = [NSHomeDirectory()stringByAppendingPathComponent:@"Documents/user.sqlite"];
NSLog(@"%@",path);
//1、初始化FMDataBase对象
_dataBase = [[FMDatabasealloc]initWithPath:path];
//2、打开数据库
BOOL isOpen = [_dataBaseopen];
if (isOpen) {
//3、创建表格
//NSData -> blob
//int -> integer
//NSString -> varchar(255)
//if not exists:如果表格不存在，创建表格
//存在就不创建
//主键:primary key,值唯一，不能重复
//主键自动增长:autoincrement
NSString *createSql =@"create table if not exists user (userId integer primary key autoincrement, userName varchar(255), age integer,headImage blob)";
BOOL ret = [_dataBaseexecuteUpdate:createSql];
if (ret) {
NSLog(@"表格创建成功");
        }else{
NSLog(@"%@",_dataBase.lastErrorMessage);
        }
    }else{
NSLog(@"数据库打开失败");
    }
}
//判断是否存在
- (BOOL)isExists:(NSString *)userName
{
NSString *sql =@"select * from user where userName = ?";
FMResultSet *rs = [_dataBaseexecuteQuery:sql, userName];
if ([rsnext]) {
returnYES;
    }
returnNO;
}
//增加一条记录
-(void)addUserModel:(UserModel *)model
{
//判断用户名是否存在
BOOL isExists = [selfisExists:model.userName];
if (isExists) {
NSLog(@"用户名已存在");
return;
    }
//?是占位符，表示要传一个值
NSString *insertSql =@"insert into user (userName, age, headImage) values (?, ? ,?)";
//图片转换成二进制
NSData *imageData =UIImagePNGRepresentation(model.headImage);
BOOL ret = [_dataBaseexecuteUpdate:insertSql,model.userName,@(model.age),
 imageData];
if (!ret) {
NSLog(@"%@",_dataBase.lastErrorMessage);
    }
}
//查询
-(NSArray *)searchAllUsers
{
NSString *selectSql =@"select * from user";
FMResultSet *rs = [_dataBaseexecuteQuery:selectSql];
NSMutableArray *array = [NSMutableArrayarray];
while ([rsnext]) {
UserModel *model = [[UserModelalloc]init];
        model.userId = [rsintForColumn:@"userId"];
        model.userName = [rsstringForColumn:@"userName"];
        model.age = [rsintForColumn:@"age"];
NSData *data = [rsdataForColumn:@"headImage"];
        model.headImage = [UIImageimageWithData:data];
        [array
addObject:model];
    }
return array;
}
//删除
-(void)deleteUserWithUserId:(int)userId
{
NSString *deleteSql =@"delete from user where userId=?";
BOOL ret = [_dataBaseexecuteUpdate:deleteSql,@(userId)];
if (!ret) {
NSLog(@"%@",_dataBase.lastErrorMessage);
    }
}
//修改
-(void)updateUserModel:(UserModel *)model userId:(int)userId
{
NSString *sql =@"update user set userName=?, age=?, headImage=? where userId=?";
//图片转换成二进制数据
NSData *data =UIImagePNGRepresentation(model.headImage);
BOOL ret = [_dataBaseexecuteUpdate:sql, model.userName,@(model.age),
 data,@(userId)];
if (!ret) {
NSLog(@"%@",_dataBase.lastErrorMessage);
    }
}
@end
6.UIAlertController的封装方法：
- (void)createAlertViewText:(NSString *)text
{
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"温馨提示" message:text preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
    }];
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
    }];
    [alertController addAction:okAction];
    [alertController addAction:cancelAction];
    [self presentViewController:alertController animated:YES completion:nil];
}
实现方法：
- (void)searchBtnClick
{
    [_licenseNumber resignFirstResponder];
    _dp.hidden = YES;
    _lineView.hidden = YES;
    _endBtn.hidden = YES;
    _showOrHidden = YES;
    NSMutableArray *licens = ((AppDelegate *)[UIApplication sharedApplication].delegate).licen;
    for (int i=0; i<licens.count; i++) {
        if ([_licenseNumber.text isEqualToString: licens[i]]) {
            [self createData];
        }
    }
    if ([_licenseNumber.text  isEqual: @""])
    {
        [self createAlertViewText:@"车牌号不能为空"];
    }
    if ([_dateBtn.titleLabel.text  isEqual: @"查询日期"])
    {
        [self createAlertViewText:@"请选择日期"];
    }
}

