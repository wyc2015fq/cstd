# 做一个WAP网站 - 左直拳的马桶_日用桶 - CSDN博客
2011年04月19日 23:45:00[左直拳](https://me.csdn.net/leftfist)阅读数：5264
做一个WAP网站，其实也没有那么神秘，甚至比PC版的网站还要容易些，因为很少图片和样式，返璞归真。
一、用什么来开发？
据说以前的手机版要用什么WML，我没学过，一听头都大了，时间花费不起。幸亏现在手机都支持HTML，所以，就用HTML，也就是我们平时在电脑上做的网站，驾轻就熟，轻车熟路。
二、识别手机浏览器，自动转向WAP版。
在主站上加入识别功能，发现客户端是手机浏览器的话，自动转向WAP版。代码抄自互联网
    #region 手机检测
    /**Wap网关特有和某些手机终端浏览器特有的头信息*/
    private static string[] mobileHeaders = new string[]{
    "HTTP_X_UP_CALLING_LINE_ID",//cmwap情况下中国移动的手机号码，目前基本没有这个head了
    "x-up-calling-line-id",//uniwap情况下下中国联通手机号码，例如：x-up-calling-line-id=8615***618119，某些移动网关也用这个header放手机号码
    "x-wap-profile",//手机终端才会有的uaprof文件url，例如：x-wap-profile="http://nds1.nds.nokia.com/uaprof/Nokia5800d-1r100-2G.xml "
    "X_WAP_PROFILE",//X_WAP_PROFILE=http://nds1.nds.nokia.com/uaprof/N6670r100.xml
    "X-Nokia-MusicShop-Bearer",//诺基亚手机送来的网络类型，例如X-Nokia-MusicShop-Bearer=GPRS/3G
    "X-Nokia-MusicShop-Version",//诺基亚网关送的参数，例如：X-Nokia-MusicShop-Version=11.1014.15
    "x-up-bear-type",//wap网关的参数，疑似上网类型,例如：x-up-bear-type=GPRS，或x-up-bear- type=GPRS/EDGE，X-Up-Bearer-Type=CSD,x-up-bear-type=CDMA CSD,x-up-bear-type=TD-SCDMA
    "X-Nokia-BEARER",//诺基亚wap网关的参数，疑似上网类型,例如：X-Nokia-BEARER=WCDMA，或X-Nokia-BEARER=GPRS
    "X-Nokia-Gateway-Id",//诺基亚wap网关的参数，例如：X-Nokia-Gateway-Id=NBG/3.0/1，或X-Nokia-gateway-id=NWG/4.1/Build4.1.04
    "X-Nokia-MSISDN",//诺基亚wap网关的参数，疑似用户手机号码，例如X-Nokia-MSISDN=861860*******
    "x-source-id",//cmwap网关参数，例如：x-source-id=220.206.156.33，也有：X-Source-ID=FZGGSN31
    "X-Nokia-CONNECTION_MODE",//wap网关参数，例如：X-Nokia-CONNECTION_MODE=TCP
    "X-Nokia-MaxDownlinkBitrate",//诺基亚wap网关参数，含义未知，例如：X-Nokia-MaxDownlinkBitrate=0
    "X-Nokia-MaxUplinkBitrate",//诺基亚wap网关参数，含义未知，例如：X-Nokia-MaxUplinkBitrate=0
    "Bearer-Indication",//不明参数，例如：Bearer-Indication=11,    Bearer-Indication=gsm_gprs_ipv4
    "X_NETWORK_INFO",//X_NETWORK_INFO=GPRS,86136********,10.100.246.217,cmwap,unsecured
    "x-NAS-Identifier",//含义不明，例如：x-NAS-Identifier=fj-telecom-cdma-pdsn
    "x-online-host",//cmwap客户端向cmwap指定header，不知道怎么的移动把它送给服务器了,例如：x-online-host=b.wap.618119.com:80
    "X_WAP_CLIENTID",//疑似加密处理后的用户身份信息，例如X_WAP_CLIENTID=618119**********
    //以下是OperaMini特有的header ,see: http://dev.opera.com/articles/view/opera-mini-request-headers/
    "X-OperaMini-Features",//OperaMini特有的客户端特性描述。例如：X-OperaMini- Features=advanced, download, file_system，X-OperaMini-Features=advanced, file_system, camera, touch, folding
    "X-OperaMini-Phone-UA",//OperaMini特有的传原始UserAgent的header。例如：X-OperaMini- Phone-UA=BlackBerry8310/4.5.0.174 Profile/MIDP-2.0 Configuration/CLDC-1.1 VendorID/107，此时的User-Agent=Opera/9.80 (BlackBerry; Opera Mini/5.1.22555/20.2485; U; zh) Presto/2.5.25
    "X-OperaMini-Phone",//OperaMini特有的自己提取的机型信息，品牌和型号直接用井号隔开，信息不靠谱，例如：X-OperaMini-Phone=BlackBerry # BlackBerry 8310
    "X-OperaMini-UA",//OperaMini特有的ua描述信息，例如：X-OperaMini-UA=Opera Mini/5.1.22555/hifi/om511china/zh
    };
    /**Wap网关Via头信息中特有的描述信息*/
    private static string[] mobileGateWayHeaders = new string[]{
    "ZXWAP",//中兴提供的wap网关的via信息，例如：Via=ZXWAP GateWay,ZTE Technologies，
    "chinamobile.com",//中国移动的诺基亚wap网关，例如：Via=WTP/1.1 GDSZ-PB-GW003-WAP07.gd.chinamobile.com (Nokia WAP Gateway 4.1 CD1/ECD13_D/4.1.04)
    "monternet.com",//移动梦网的网关，例如：Via=WTP/1.1 BJBJ-PS-WAP1-GW08.bj1.monternet.com. (Nokia WAP Gateway 4.1 CD1/ECD13_E/4.1.05)
    "infoX",//华为提供的wap网关，例如：Via=HTTP/1.1 GDGZ-PS-GW011-WAP2 (infoX-WISG, Huawei Technologies)，或Via=infoX WAP Gateway V300R001, Huawei Technologies
    "XMS 724Solutions HTG",//国外电信运营商的wap网关，不知道是哪一家
    "wap.lizongbo.com",//自己测试时模拟的头信息
    "Bytemobile",//貌似是一个给移动互联网提供解决方案提高网络运行效率的，例如：Via=1.1 Bytemobile OSN WebProxy/5.1
    };
    /**电脑上的IE或Firefox浏览器等的User-Agent关键词*/
    private static string[] pcHeaders = new string[]{
    "Windows 98",
    "Windows ME",
    "Windows 2000",
    "Windows XP",
    "Windows NT",
    "Ubuntu",
    };
    /**手机浏览器的User-Agent里的关键词*/
    private static string[] mobileUserAgents = new string[]{
    "Nokia",//诺基亚，有山寨机也写这个的，总还算是手机，Mozilla/5.0 (Nokia5800 XpressMusic)UC AppleWebkit(like Gecko) Safari/530
    "SAMSUNG",//三星手机 SAMSUNG-GT-B7722/1.0+SHP/VPP/R5+Dolfin/1.5+Nextreaming+SMM-MMS/1.2.0+profile/MIDP-2.1+configuration/CLDC-1.1
    "MIDP-2",//j2me2.0，Mozilla/5.0 (SymbianOS/9.3; U; Series60/3.2 NokiaE75-1 /110.48.125 Profile/MIDP-2.1 Configuration/CLDC-1.1 ) AppleWebKit/413 (KHTML, like Gecko) Safari/413
    "CLDC1.1",//M600/MIDP2.0/CLDC1.1/Screen-240X320
    "SymbianOS",//塞班系统的，
    "MAUI",//MTK山寨机默认ua
    "UNTRUSTED/1.0",//疑似山寨机的ua，基本可以确定还是手机
    "Windows CE",//Windows CE，Mozilla/4.0 (compatible; MSIE 6.0; Windows CE; IEMobile 7.11)
    "iPhone",//iPhone是否也转wap？不管它，先区分出来再说。Mozilla/5.0 (iPhone; U; CPU iPhone OS 4_1 like Mac OS X; zh-cn) AppleWebKit/532.9 (KHTML, like Gecko) Mobile/8B117
    "iPad",//iPad的ua，Mozilla/5.0 (iPad; U; CPU OS 3_2 like Mac OS X; zh-cn) AppleWebKit/531.21.10 (KHTML, like Gecko) Version/4.0.4 Mobile/7B367 Safari/531.21.10
    "Android",//Android是否也转wap？Mozilla/5.0 (Linux; U; Android 2.1-update1; zh-cn; XT800 Build/TITA_M2_16.22.7) AppleWebKit/530.17 (KHTML, like Gecko) Version/4.0 Mobile Safari/530.17
    "BlackBerry",//BlackBerry8310/2.7.0.106-4.5.0.182
    "UCWEB",//ucweb是否只给wap页面？ Nokia5800 XpressMusic/UCWEB7.5.0.66/50/999
    "ucweb",//小写的ucweb,貌似是uc的代理服务器,Mozilla/6.0 (compatible; MSIE 6.0;) Opera ucweb-squid
    "BREW",//很奇怪的ua，例如：REW-Applet/0x20068888 (BREW/3.1.5.20; DeviceId: 40105; Lang: zhcn) ucweb-squid
    "J2ME",//,很奇怪的ua，只有J2ME四个字母
    "YULONG",//宇龙手机，YULONG-CoolpadN68/10.14 IPANEL/2.0 CTC/1.0
    "YuLong",//还是宇龙
    "COOLPAD",//宇龙酷派,YL-COOLPADS100/08.10.S100 POLARIS/2.9 CTC/1.0
    "TIANYU",//天语手机,TIANYU-KTOUCH/V209/MIDP2.0/CLDC1.1/Screen-240X320
    "TY-",//天语，TY-F6229/701116_6215_V0230 JUPITOR/2.2 CTC/1.0
    "K-Touch",//还是天语,K-Touch_N2200_CMCC/TBG110022_1223_V0801 MTK/6223 Release/30.07.2008 Browser/WAP2.0
    "Haier",//海尔手机，Haier-HG-M217_CMCC/3.0 Release/12.1.2007 Browser/WAP2.0
    "DOPOD",//多普达手机,
    "Lenovo",//联想手机，Lenovo-P650WG/S100 LMP/LML Release/2010.02.22 Profile/MIDP2.0 Configuration/CLDC1.1
    "LENOVO",//联想手机，比如：LENOVO-P780/176A
    "HUAQIN",//华勤手机
    "AIGO-",//爱国者居然也出过手机，AIGO-800C/2.04 TMSS-BROWSER/1.0.0 CTC/1.0
    "CTC/1.0",//含义不明
    "CTC/2.0",//含义不明
    "CMCC",//移动定制手机，K-Touch_N2200_CMCC/TBG110022_1223_V0801 MTK/6223 Release/30.07.2008 Browser/WAP2.0
    "DAXIAN",//大显手机,DAXIAN X180 UP.Browser/6.2.3.2(GUI) MMP/2.0
    "MOT-",//摩托罗拉，MOT-MOTOROKRE6/1.0 LinuxOS/2.4.20 Release/8.4.2006 Browser/Opera8.00 Profile/MIDP2.0 Configuration/CLDC1.1 Software/R533_G_11.10.54R
    "SonyEricsson",//索爱手机，SonyEricssonP990i/R100 Mozilla/4.0 (compatible; MSIE 6.0; Symbian OS; 405) Opera 8.65 [zh-CN]
    "GIONEE",//金立手机
    "HTC",//HTC手机
    "ZTE",//中兴手机，ZTE-A211/P109A2V1.0.0/WAP2.0 Profile
    "HUAWEI",//华为手机，
    "webOS",//palm手机，Mozilla/5.0 (webOS/1.4.5; U; zh-CN) AppleWebKit/532.2 (KHTML, like Gecko) Version/1.0 Safari/532.2 Pre/1.0
    "GoBrowser",//3g GoBrowser.User-Agent=Nokia5230/GoBrowser/2.0.290 Safari
    "IEMobile",//Windows CE手机自带浏览器，
    "WAP2.0",//支持wap 2.0的
    };
    /**
    * 根据当前请求的特征，判断该请求是否来自手机终端，主要检测特殊的头信息，以及user-Agent这个header
    * @param request http请求
    * @return 如果命中手机特征规则，则返回对应的特征字符串
    */
    public static bool IsFromMobile(HttpRequest request)
    {// 识别当前请求，针对来自手机的请求则跳转到wap页面。
        return GetFromMobileRule(request) != null;
    }
    public static string GetFromMobileRule(HttpRequest request)
    {
        string userAgent = request.UserAgent;
        if (userAgent != null)
        {
            foreach (string mheader in pcHeaders)
            {
                if (userAgent.IndexOf(mheader) != -1)
                {
                    return null;
                }
            }
        }
        foreach (string mheader in mobileHeaders)
        {// 只要存在网关特有的header，肯定是手机
            if (request.Headers.Get(mheader) != null && request.Headers.Get(mheader).Length > 0)
            {
                return mheader;
            }
        }
        string via = request.Headers.Get("Via");
        if (via != null)
        {
            foreach (string mheader in mobileGateWayHeaders)
            {// 是不是wap网关的描述
                if (via.IndexOf(mheader) != -1)
                {
                    return mheader;
                }
            }
        }
        if (userAgent != null)
        {
            foreach (string mheader in mobileUserAgents)
            {
                if (userAgent.IndexOf(mheader) != -1)
                {
                    return mheader;
                }
            }
        }
        return null;
    }
    #endregion
基本都是检测Request.UserAgent
注意：国内很流行的UC浏览器，没有什么明显的特征，以上方法无法检测
三、手机浏览器功能很弱
javascript?不支持，不要用。CSS，可以；链接，可以。字体？不要设置什么font-size=12px，在某些浏览器上，几乎无法辨认。所以索性不设置字体大小，由浏览器自动调整。
四、页面缓存问题很严重。
发现手机浏览器，缓存情况非常严重，一个页面浏览一次，以后打开，都是同样内容。一定要做一些声明。我从新浪手机版微博上抄了一些
，做成母版页，所有页面都继承之。
<%@ Master Language="C#" AutoEventWireup="true" CodeFile="base.master.cs" Inherits="master_base" %>
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head id="Head1" runat="server">
    <meta http-equiv="Cache-Control" content="no-cache"/>
    <meta name="viewport" content="width=device-width; initial-scale=1.0; minimum-scale=1.0; maximum-scale=2.0"/>
    <meta name="MobileOptimized" content="240"/>
五、服务器端有些功能无法使用，否则会报错
如检测请求来源，Request.UrlReferrer，这些页面在手机上运行会报错。
六、UC浏览器
个人感觉这是个很烂的浏览器，但在国内据说却很火。这可能是真的，我的手机，有自带的浏览器，也有这个UCWEB。
严格意义上来说，UCWEB不是一个浏览器，据说你用它来上网的时候，其实是先访问它，由它来装载、解释、处理你要访问的页面，再返回给你。
所以有时候在别的手机浏览器上好好的，在UCWEB这里，就出问题。
比如，它不支持一些样式，象字体加粗，下划线，等等
不支持服务器端的重定向Response.Redirect()。准确的是，是部分支持。比如说，在a页面Response.Redirect("b.aspx")，它确实转向了b，但浏览器的地址，还是停留在a页面上，没有转换为b地址。假如a和b都在同一个目录下，看不出有问题，但假如不是，象a转向了../b，然后b转向c，就报错了，因为a目录下没有c页面。
用UC来访问一些操作COOKIE 、SESSION的页面也经常出问题。将手机的缓存、历史记录清掉以后，又正常。
UC唯一的优点是回退功能很快。
