# iOS的密码管理系统 Keychain的介绍和使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年06月14日 15:31:56[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：167

# Keychain 的介绍
**Keychain** 是苹果公司 Mac OS(也包含 Mac OSX) 中的密码管理系统。
# Keychain的作用
**Keychain** 可以包含许多种类型的数据：密码（包括网站、FTP服务器、SSH账户、网络共享、无线网络、群组软件、加密磁盘镜像），私钥，电子证书、加密笔记等。
# Keychain 的四个方法介绍
#### 1、存储数据的方法
OSStatus SecItemAdd(CFDictionaryRef attributes, CFTypeRef * __nullable CF_RETURNS_RETAINED result)
@attributes : 是要添加的数据。
@result : 这是存储数据后，返回一个指向该数据的引用，如果不是使用该引用时可以传入 NULL 。
#### 2、根据条件查询数据
OSStatus SecItemCopyMatching(CFDictionaryRef query, CFTypeRef * __nullable CF_RETURNS_RETAINED result)
@query : 要查询数据的条件。
@result: 根据条件查询得到数据的引用。
#### 3、更新数据
OSStatus SecItemUpdate(CFDictionaryRef query, CFDictionaryRef attributesToUpdate)
__OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_2_0);
@query : 要更新数据的查询条件。
@attributesToUpdate : 要更新的数据。
#### 4、删除数据
OSStatus SecItemDelete(CFDictionaryRef query)
__OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_2_0);
@query : 删除的数据的查询条件。
#### 5、总结
以上四个方法，就是Keychain 的常用的四个方法，也就是 增、删、改、查 。一般应用这四个方法就可以完全满足。
# Keychain 的一些Key 和 Value 的解释和介绍
#### 1、设置信息的保密程度
![](https://upload-images.jianshu.io/upload_images/1521667-bbd531511658f952?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
这里写图片描述
#### 2、kSecClass 的可选value
![](https://upload-images.jianshu.io/upload_images/1521667-f53660b55aa4c6f1?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
这里写图片描述
#### 3、kSecClassGenericPassword 密码所包含的所有类型参数
![](https://upload-images.jianshu.io/upload_images/1521667-98e6b4212c98319c?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
这里写图片描述
其他密码的参数和普通密码的参数大致相同，就不一一列举。
# Keychain 的封装类KeyChainManager 的介绍和使用
#### 1、该类的 .h 文件
```
//
//  KeyChainManager.h
//  KeyChain
//
//  Created by MAC on 2017/11/8.
//  Copyright © 2017年 NetworkCode小贱. All rights reserved.
//
#import <Foundation/Foundation.h>
@interface KeyChainManager : NSObject
/*!
 保存数据
 @data  要存储的数据
 @identifier 存储数据的标示
 */
+(BOOL) keyChainSaveData:(id)data withIdentifier:(NSString*)identifier ;
/*!
 读取数据
 @identifier 存储数据的标示
 */
+(id) keyChainReadData:(NSString*)identifier ;
/*!
 更新数据
 @data  要更新的数据
 @identifier 数据存储时的标示
 */
+(BOOL)keyChainUpdata:(id)data withIdentifier:(NSString*)identifier ;
/*!
 删除数据
 @identifier 数据存储时的标示
 */
+(void) keyChainDelete:(NSString*)identifier ;
@end
```
#### 2、该类的 .m 文件
```
//
//  KeyChainManager.m
//  KeyChain
//  Created by MAC on 2017/11/8.
//  Copyright © 2017年 NetworkCode小贱. All rights reserved.
//
#import "KeyChainManager.h"
#import <Security/Security.h>
@implementation KeyChainManager
/*!
 创建生成保存数据查询条件
 */
+(NSMutableDictionary*) keyChainIdentifier:(NSString*)identifier {
    NSMutableDictionary * keyChainMutableDictionary = [NSMutableDictionary dictionaryWithObjectsAndKeys:(id)kSecClassGenericPassword,kSecClass,identifier,kSecAttrService,identifier,kSecAttrAccount,kSecAttrAccessibleAfterFirstUnlock,kSecAttrAccessible, nil];
    return keyChainMutableDictionary;
}
/*!
 保存数据
 */
+(BOOL) keyChainSaveData:(id)data withIdentifier:(NSString*)identifier{
    // 获取存储的数据的条件
    NSMutableDictionary * saveQueryMutableDictionary = [self keyChainIdentifier:identifier];
    // 删除旧的数据
    SecItemDelete((CFDictionaryRef)saveQueryMutableDictionary);
    // 设置新的数据
    [saveQueryMutableDictionary setObject:[NSKeyedArchiver archivedDataWithRootObject:data] forKey:(id)kSecValueData];
    // 添加数据
   OSStatus saveState = SecItemAdd((CFDictionaryRef)saveQueryMutableDictionary, nil);
    // 释放对象
    saveQueryMutableDictionary = nil ;
    // 判断是否存储成功
    if (saveState == errSecSuccess) {
        return YES;
    }
    return NO;
}
/*!
 读取数据
 */
+(id) keyChainReadData:(NSString*)identifier{
    id idObject = nil ;
    // 通过标记获取数据查询条件
    NSMutableDictionary * keyChainReadQueryMutableDictionary = [self keyChainIdentifier:identifier];
    // 这是获取数据的时，必须提供的两个属性
    // TODO: 查询结果返回到 kSecValueData
    [keyChainReadQueryMutableDictionary setObject:(id)kCFBooleanTrue forKey:(id)kSecReturnData];
    // TODO: 只返回搜索到的第一条数据
    [keyChainReadQueryMutableDictionary setObject:(id)kSecMatchLimitOne forKey:(id)kSecMatchLimit];
    // 创建一个数据对象
    CFDataRef keyChainData = nil ;
    // 通过条件查询数据
    if (SecItemCopyMatching((CFDictionaryRef)keyChainReadQueryMutableDictionary , (CFTypeRef *)&keyChainData) == noErr){
        @try {
            idObject = [NSKeyedUnarchiver unarchiveObjectWithData:(__bridge NSData *)(keyChainData)];
        } @catch (NSException * exception){
            NSLog(@"Unarchive of search data where %@ failed of %@ ",identifier,exception);
        }
    }
    if (keyChainData) {
        CFRelease(keyChainData);
    }
    // 释放对象
    keyChainReadQueryMutableDictionary = nil;
    // 返回数据
    return idObject ;
}
/*!
 更新数据
 @data  要更新的数据
 @identifier 数据存储时的标示
 */
+(BOOL)keyChainUpdata:(id)data withIdentifier:(NSString*)identifier {
    // 通过标记获取数据更新的条件
    NSMutableDictionary * keyChainUpdataQueryMutableDictionary = [self keyChainIdentifier:identifier];
    // 创建更新数据字典
    NSMutableDictionary * updataMutableDictionary = [NSMutableDictionary dictionaryWithCapacity:0];
    // 存储数据
    [updataMutableDictionary setObject:[NSKeyedArchiver archivedDataWithRootObject:data] forKey:(id)kSecValueData];
    // 获取存储的状态
    OSStatus  updataStatus = SecItemUpdate((CFDictionaryRef)keyChainUpdataQueryMutableDictionary, (CFDictionaryRef)updataMutableDictionary);
    // 释放对象
    keyChainUpdataQueryMutableDictionary = nil;
    updataMutableDictionary = nil;
    // 判断是否更新成功
    if (updataStatus == errSecSuccess) {
        return  YES ;
    }
    return NO;
}
/*!
 删除数据
 */
+(void) keyChainDelete:(NSString*)identifier {
    // 获取删除数据的查询条件
    NSMutableDictionary * keyChainDeleteQueryMutableDictionary = [self keyChainIdentifier:identifier];
    // 删除指定条件的数据
    SecItemDelete((CFDictionaryRef)keyChainDeleteQueryMutableDictionary);
    // 释放内存
    keyChainDeleteQueryMutableDictionary = nil ;
}
@end
```
# KeyChainManager 类的测试使用
#### 1、测试代码
```
// 存储数据
BOOL save = [KeyChainManager keyChainSaveData:@"思念诉说，眼神多像云朵" withIdentifier:Keychain];
if (save) {
    NSLog(@"存储成功");
}else {
    NSLog(@"存储失败");
}
// 获取数据
NSString * readString = [KeyChainManager keyChainReadData:Keychain];
NSLog(@"获取得到的数据:%@",readString);
// 更新数据
BOOL updata = [KeyChainManager keyChainUpdata:@"长发落寞，我期待的女孩" withIdentifier:Keychain];
if (updata) {
    NSLog(@"更新成功");
}else{
    NSLog(@"更新失败");
}
// 读取数据
NSString * readUpdataString = [KeyChainManager keyChainReadData:Keychain];
NSLog(@"获取更新后得到的数据:%@",readUpdataString);
// 删除数据
[KeyChainManager keyChainDelete:Keychain];
// 读取数据
NSString * readDeleteString = [KeyChainManager keyChainReadData:Keychain];
NSLog(@"获取删除后得到的数据:%@",readDeleteString);
```
#### 2、测试结果的展示
![](https://upload-images.jianshu.io/upload_images/1521667-afffb91a6b79f753?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
这里写图片描述
作者：爱恨的潮汐
链接：https://www.jianshu.com/p/781f394baf61
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
