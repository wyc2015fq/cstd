
# Python 和 PHP 对腾讯云签名 hmac_sha256 算法实现 - 阳光岛主 - CSDN博客

2018年05月08日 22:45:18[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4443


开宗明义，米扑科技在使用腾讯云的API接口签名中，按照官方示例开发PHP、Python的接口，经常会提示签名错误
|1
|2
|3
|4
|5
|6
|7
|8
|9
|{
|"Response"|: {
|"Error"|: {
|"Code"|:|"InvalidParameter.SignatureFailure"|,
|"Message"|:|"The provided credentials could not be validated. Please check your signature is correct."
|},
|"RequestId"|:|"1ee6ae98-a971-ad9f-4ecc-abcd69ea1234"
|}
|}
|

本文原文，请参见米扑博客：
[Python 和 PHP 对腾讯云签名 hmac_sha256 算法实现](https://blog.mimvp.com/article/24338.html)

经过多次尝试探究，发现原因有二：
1）腾讯云官方示例不严谨，没有urlencode()或urllib.quote()编码导致提示签名错误
2）腾讯官方只提供了PHP示例，没有提供Python示例，两者签名函数有一些细节
直接给出干货，下面示例是**[米扑科技](https://mimvp.com/)**封装好的腾讯云签名函数，以飨读者。
腾讯云签名：[https://cloud.tencent.com/document/api/377/4214](https://cloud.tencent.com/document/api/377/4214)
阿里云签名：[https://help.aliyun.com/document_detail/35735.html](https://help.aliyun.com/document_detail/35735.html)
米扑的官网：[https://mimvp.com](https://mimvp.com/)
**PHP 签名示例**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|74
|75
|76
|77
|78
|79
|80
|81
|/**
|* 签名并获取URL结果，json格式返回
|*
|* 1. 查询弹性IP列表, DescribeAddresses
|* 2. 解绑弹性IP, DisassociateAddress
|* 3. 释放弹性IP, ReleaseAddresses
|* 4. 公网IP转弹性IP, TransformAddress
|*
|* @param string $req_action : DescribeAddresses, DisassociateAddress, ReleaseAddresses, TransformAddress
|* @param string $params : 以 & 开头， 如 &xxx=yyy
|*/
|function|qcloud_eip_sign(|$req_action|=|'DescribeAddresses'|,|$req_region|=|'ap-beijing'|,|$req_extra_params|=|''|,|$retry_NUM|=3) {
|global|$QCloud_SecretId|;
|global|$QCloud_SecretKey|;
|//  $req_action='DescribeAddresses'
|//  $req_region = 'ap-beijing';                 // ap-guangzhou
|$req_method|=|'GET'|;|// GET  POST
|$req_api|=|'eip.api.qcloud.com/v2/index.php'|;
|$req_version|=|'2017-03-12'|;
|$req_timestamp|=|strtotime|(|date|(|'YmdHis'|));|// 1402992826
|$req_nonce|= rand(1000, 1000000);|// 随机正整数
|$req_secretid|=|$QCloud_SecretId|;|// 密钥ID，用作参数
|$req_secretkey|=|$QCloud_SecretKey|;|// 密钥key，用作加密
|$req_signature_method|=|'HmacSHA256'|;|// HmacSHA1(默认), HmacSHA256
|$req_signature|=|''|;
|//  $req_uri = "https://eip.api.qcloud.com/v2/index.php?Action=DescribeAddresses
|//  &Version=2017-03-12
|//  &AddressIds.1=eip-hxlqja90
|//  &Region=ap-beijing
|//  &Timestamp=1402992826
|//  &Nonce=345122
|//  &Signature=pStJagaKsV2QdkJnBQWYBDByZ9YPBsOi
|//  &SecretId=AKIDpY8cxBD2GLGK9sT0LaqIczGLFxTsoDF6
|// 请求方法 + 请求主机 +请求路径 + ? + 请求字符串
|$req_params|= sprintf(|"Action=%s&Region=%s&Version=%s&Timestamp=%s&Nonce=%s&SecretId=%s&SignatureMethod=%s%s"|,|$req_action|,|$req_region|,|$req_version|,|$req_timestamp|,|$req_nonce|,|$req_secretid|,|$req_signature_method|,|$req_extra_params|);
|$req_params_array|=|explode|(|"&"|,|$req_params|);
|sort(|$req_params_array|);|// 以value排序，value值为 Action=DescribeAddresses 、 Region=ap-beijing
|$req_params2|= implode(|"&"|,|$req_params_array|);
|$req_uri|= sprintf(|"%s%s?%s"|,|$req_method|,|$req_api|,|$req_params2|);
|$req_signature|= urlencode(|base64_encode|(hash_hmac(|'sha256'|,|$req_uri|,|$req_secretkey|, true)));|// urlencode(xxx)
|$req_url|= sprintf(|"https://%s?%s&Signature=%s"|,|$req_api|,|$req_params2|,|$req_signature|);
|$res|= curl_url(|$req_url|);
|$retry_idx|= 0;
|while|(|empty|(|$res|) &&|$retry_idx|<|$retry_NUM|) {
|$retry_idx|+= 1;
|$res|= curl_url(|$req_url|);
|}
|if|(!|empty|(|$res|)) {
|$resJson|= json_decode(|$res|, true);
|$resJson|=|$resJson|[|'Response'|];
|echo|sprintf(|"<br><br> +++++ action : %s <br><br> resJson: "|,|$req_action|);
|print_r(|$resJson|);
|return|$resJson|;
|}
|else|{
|return|null;
|}
|}

|$req_action_query|=|'DescribeAddresses'|;|// 查询弹性IP
|$req_action_unbind|=|'DisassociateAddress'|;|// 解绑弹性IP
|$req_action_release|=|'ReleaseAddresses'|;|// 释放弹性IP
|$req_action_transform|=|'TransformAddress'|;|// 公网IP转弹性IP
|$req_region|=|'ap-guangzhou'|;
|$req_extra_params|=|''|;
|// 1. 查询弹性IP列表
|$resJson|= qcloud_eip_sign(|$req_action_query|,|$req_region|);
|var_dump(|$resJson|);
|

**运行结果：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|req_url: https:|//eip|.api.qcloud.com|/v2/index|.php?Action=DescribeAddresses&Nonce=585269&Region=ap-guangzhou&SecretId=AKIDSmAAAA2DABCDpTkBBBBMLMFwY0HM1234&SignatureMethod=HmacSHA256&Timestamp=1520429723&Version=2017-03-12&Signature=8U6i3BKBWYWoit3t1egIE9ZC%2BdWtI46QuHLc%2FbhaWWg%3D
|array (size=3)
|'TotalCount'|=> int 1
|'AddressSet'|=>
|array (size=1)
|0 =>
|array (size=11)
|'AddressId'|=> string|'eip-qy123abc'|(length=12)
|'AddressName'|=> null
|'AddressIp'|=> string|'111.230.123.234'|(length=15)
|'AddressStatus'|=> string|'BIND'|(length=4)
|'InstanceId'|=> string|'ins-fabc1234'|(length=12)
|'NetworkInterfaceId'|=> null
|'PrivateAddressIp'|=> string|'10.104.245.26'|(length=14)
|'IsArrears'|=> boolean|false
|'IsBlocked'|=> boolean|false
|'IsEipDirectConnection'|=> boolean|false
|'CreatedTime'|=> string|'2018-03-07T12:46:26Z'|(length=20)
|'RequestId'|=> string|'ad28067e-d1f9-4c47-932e-6bba1d123456'|(length=36)
|

**代码说明：**
1）函数抽象封装签名方法，方便管理维护，减少开发工作量
2）参数按照升序排列explode(xxx) —> sort($req_params_array) —> implode(xxx)
3）签名方法，需要添加 urlencode，否则经常提示签名错误，原因是未urlencode会有一些 空格、加号(+)、等号(=)等特殊字符
$req_signature = urlencode(base64_encode(hash_hmac('sha256', $req_uri, $req_secretkey, true)));// urlencode(xxx)

**Python 签名示例**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|74
|75
|76
|77
|78
|79
|\#!/usr/bin/env python
|\# -*- coding:utf-8 -*-
|\#
|\# mimvp.com
|\# 2018-01-08

|import|time, datetime, os, json
|import|urllib, urllib2
|import|hashlib, base64, hmac, random
|import|logging
|import|logging.handlers
|import|sys
|reload|(sys)
|sys.setdefaultencoding(|'utf-8'|)

|\#\# 腾讯云API接口签名
|def|qcloud_eip_sign(req_action|=|'DescribeAddresses'|, req_region|=|'ap-beijing'|,  req_extra_params|=|'', retry_NUM|=|3|):
|req_method|=|'GET'|\# GET  POST
|req_api|=|'eip.api.qcloud.com/v2/index.php'
|req_version|=|'2017-03-12'
|req_timestamp|=|int|(time.time())|\# 1520422452
|req_nonce|=|random.randint(|1000|,|1000000|)|\# 随机正整数
|req_secretid|=|QCLOUD_SecretId|\# 密钥ID，用作参数
|req_secretkey|=|QCLOUD_SecretKey|\# 密钥key，用作加密
|req_signature_method|=|'HmacSHA256'|\# HmacSHA1(默认), HmacSHA256
|req_signature|=|''
|\#     req_uri = "https://eip.api.qcloud.com/v2/index.php?Action=DescribeAddresses
|\#                 &Version=2017-03-12
|\#                 &AddressIds.1=eip-hxlqja90
|\#                 &Region=ap-beijing
|\#                 &Timestamp=1402992826
|\#                 &Nonce=345122
|\#                 &Signature=pStJagaKsV2QdkJnBQWYBDByZ9YPBsOi
|\#                 &SecretId=AKIDpY8cxBD2GLGK9sT0LaqIczGLFxTsoDF6
|\# 请求方法 + 请求主机 +请求路径 + ? + 请求字符串
|req_params|=|"Action=%s&Region=%s&Version=%s&Timestamp=%s&Nonce=%s&SecretId=%s&SignatureMethod=%s%s"|%|(req_action, req_region, req_version, req_timestamp, req_nonce, req_secretid, req_signature_method, req_extra_params)
|req_params_array|=|req_params.split(|'&'|)
|req_params_array|=|sorted|(req_params_array)|\# 以value排序，value值为 Action=DescribeAddresses 、 Region=ap-beijing
|req_params2|=|'&'|.join(req_params_array);
|req_uri|=|"%s%s?%s"|%|(req_method, req_api, req_params2)
|req_signature|=|urllib.quote( base64.b64encode(hmac.new(req_secretkey, req_uri, digestmod|=|hashlib.sha256).digest()) )|\# urllib.quote(xxx)
|req_url|=|"https://%s?%s&Signature=%s"|%|(req_api, req_params2, req_signature)
|logger.info(|'qcloud_eip_sign() - req_url: %s'|%|(req_url))
|res|=|spider_url(req_url)
|retry_idx|=|0|;
|while|not|res|and|retry_idx < retry_NUM:
|retry_idx|+|=|1
|res|=|spider_url(req_url)
|if|res :
|resJson|=|json.loads(res)
|resJson|=|resJson[|'Response'|]
|print|"<br><br> +++++ action : %s <br><br> resJson: "|%|(req_action,)
|return|resJson
|else|:
|return|None|;

|if|__name__|=|=|"__main__"|:
|req_action_query|=|'DescribeAddresses'|\# 查询弹性IP
|req_action_unbind|=|'DisassociateAddress'|\# 解绑弹性IP
|req_action_release|=|'ReleaseAddresses'|\# 释放弹性IP
|req_action_transform|=|'TransformAddress'|\# 公网IP转弹性IP
|req_region|=|'ap-guangzhou'
|req_extra_params|=|'';
|\# 1. 查询弹性IP列表
|resJson|=|qcloud_eip_sign(req_action_query, req_region)
|print|json.dumps(resJson)
|

**运行结果：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|req_url: https:|//eip|.api.qcloud.com|/v2/index|.php?Action=DescribeAddresses&Nonce=383782&Region=ap-guangzhou&SecretId=AKIDSmAAAA2DABCDpTkBBBBMLMFwY0HM1234&SignatureMethod=HmacSHA256&Timestamp=1520430569&Version=2017-03-12&Signature=Tsgwx2GV/%2BopDlHiMUg3H|/rpIbQ5jPfe9tW3w9Slom4|%3D
|{
|"Response"|: {
|"TotalCount"|: 1,
|"AddressSet"|: [
|{
|"AddressId"|:|"eip-qy123abc"|,
|"AddressName"|: null,
|"AddressIp"|:|"111.230.123.234"|,
|"AddressStatus"|:|"BIND"|,
|"InstanceId"|:|"ins-fabc1234"|,
|"NetworkInterfaceId"|: null,
|"PrivateAddressIp"|:|"10.104.245.26"|,
|"IsArrears"|:|false|,
|"IsBlocked"|:|false|,
|"IsEipDirectConnection"|:|false|,
|"CreatedTime"|:|"2018-03-07T12:46:26Z"
|}
|],
|"RequestId"|:|"c2ab3f7f-9796-4ade-afb1-6bba1d123456"
|}
|}
|

**代码说明：**
1）Python改写PHP代码，有一些细节，如 int(time.time())、random.randint(1000, 1000000)、sorted(req_params_array)等
2）参数按照升序排列xxx.split('&') —> sort($req_params_array) —> '&'.join(xxx)
3）签名方法，需要添加urllib.quote、base64.b64encode(xxx)、digest()等，否则经常提示签名错误
req_signature = urllib.quote( base64.b64encode(hmac.new(req_secretkey, req_uri, digestmod=hashlib.sha256).digest()) )\# urllib.quote(xxx)
Python 代码里，特别要注意 hmac 签名 sha256 后获取的是digest()，而不是hexdigest()这里错了会一直提示签名错误！
总结之PHP和Python的对应关系
**1） PHP 签名**
|1
|2
|3
|4
|5
|6
|7
|// sha1
|$hmac_sha1_str|=|base64_encode|(hash_hmac(|"sha1"|,|$data|,|$secret_access_key|));|// HMAC-SHA1加密
|$signature|= urlencode(|$hmac_sha1_str|);|// 编码URL
|// sha256
|$hmac_sha256_str|=|base64_encode|(hash_hmac(|"sha256"|,|$data|,|$secret_access_key|));|// HMAC-SHA256加密
|$signature|= urlencode(|$hmac_sha256_str|);|// 编码URL
|

**2）Python 签名**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|import|urllib, base54, hashlib, hmac
|\# sha1
|hmac_sha1_str|=|base64.b64encode( hmac.new(secret_access_key, data, digestmod|=|hashlib.sha1).digest() )
|signature|=|urllib.quote(hmac_sha1_str)
|\# sha256
|hmac_sha256_str|=|base64.b64encode( hmac.new(secret_access_key, data, digestmod|=|hashlib.sha256).digest() )
|signature|=|urllib.quote(hmac_sha256_str)

|\# sha256
|hmac_sha256_str|=|base64.b64encode( hmac.new(secret_access_key, data, digestmod|=|hashlib.sha256).hexdigest() )|\# 16进制，错误
|signature|=|urllib.quote(hmac_sha256_str)
|




