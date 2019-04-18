# Python学起来-快递100信息抓取 - weixin_33985507的博客 - CSDN博客
2018年09月13日 20:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
感觉可以花来写字的时间不多，所以就随笔了。
近年被安利了一轮又一轮的Python，总是没有时间拿起来，前段时间有再多了解了一点lisp， vim， lua。Python 是不是传说中的那么神？试过就知道，因为有喜欢的上手语言， 学新东西总爱拿来做比对，比如PHP是不是有JS那样的简化版三元：
```
var a = b||c;
```
Python 是不是有三元，其实没有，只能 if else，下面的 TRUE|FALSE是条件，真就就A，否则就是B，有点绕啊：
```
v = "A" if TRUE|False else "B"
```
爬虫这里主要是请求页面，然后是正则表达式过滤，基础的有Python的数据类型 tuple， list， str。
```python
# coding = UTF-8
# Citizen's salary of 0 Dn
# Clerk's salary of 2 Dn
# Engineer's salary of 5 Dn
# Architect's salary of 8 Dn
# Quaestor's salary of 12 Dn
# Procurator's salary of 20 Dn
# Aedile's salary of 30 Dn
# Praetor's salary of 40 Dn
# Consul's salary of 60 Dn
# Proconsul's salary of 80 Dn
# Caesar's salary of 100 Dn
# Python Scraper
from pprint import pprint
from inspect import getmembers
import urllib.request
import json
import re
import os
# open the url and read
def getHtml(url):
    page = urllib.request.urlopen(url, None, timeout=19)
    html = page.read()
    page.close()
    return html
def getFile(url):
    file_name = url.split('/')[-1]
    u = urllib.request.urlopen(url, None, timeout=19)
    f = open(file_name, 'wb')
    block_sz = 8192
    while True:
        buffer = u.read(block_sz)
        if not buffer:
            break
        f.write(buffer)
    f.close()
    print ("Sucessful to download" + " " + file_name)
# compile the regular expressions and find
# all stuff we need
def getUrl(html):
    reg = r'(?:href|HREF)="?((?:http://)?.+?\.pdf)'
    url_re = re.compile(reg)
    url_lst = url_re.findall(html.decode('gb2312'))
    return(url_lst)
def getTS(html):
    reg = r'(\d+\.ts)'
    reg = re.compile(reg)
    items = reg.findall(html.decode('gb2312'))
    return(items)
def getAnchors(html):
    reg = r'(?=href="(https://www.kuaidi100.com/(global|all)/[^"]+)"(?=[^"])+>([^<]+)<)'
    reg = re.compile(reg)
    items = reg.findall(html.decode('utf8'))
    return(items)
def getExpressSql(items):
    sqls = [];
    for item in items:
        # pprint (item[1]+"=>"+item[0])
        html = getHtml(item[0])
        page = html.decode('utf8')
        page = page.replace("\r\n","").replace("\t", "")
        reg = r'(?=companyCode.+?value="(.+?)".+?(?=客服电话">([^<]*)<).+?(?=selectComBtn".+?title="(.+?)"(?=><img src="(.+?)")))'
        # reg = r'(?=companyCode.+?value="(.+?)")' (?=访问官网" href="(.*?)").+?
        # reg = r'(?=客服电话">(.+?)<)'
        fields = re.compile(reg).findall(page)
        vals = "".join( list(map(lambda x: str(x), fields)) )
        # sql = "insert into wxapp_express values(%s)"%(vals)
        sql = "insert into wxapp_express values "+vals
        sqls.append(sql)
        print( sql )
    return sqls
def express_scraper():
    url = 'https://www.kuaidi100.com/global/'
    url = 'https://www.kuaidi100.com/all/'
    html = getHtml(url)
    anchors = getAnchors(html)
    pprint( anchors )
    sqls = getExpressSql(anchors)
    pprint( sqls )
express_scraper()
```
抓取的是快递公司信息，并生成sql语句：
```
insert into wxapp_express values ('bangladesh', '', '孟加拉国(EMS)', 'https://cdn.kuaidi100.com/images/all/56/bangladesh.png')
insert into wxapp_express values ('canpost', '', '加拿大(Canada Post)', 'https://cdn.kuaidi100.com/images/all/56/canpost.png')
insert into wxapp_express values ('ceskaposta', '', '捷克（?eská po?ta）', 'https://cdn.kuaidi100.com/images/all/56/ceskaposta.png')
insert into wxapp_express values ('chronopostfren', '', '法国大包、EMS-英文(Chronopost France)', 'https://cdn.kuaidi100.com/images/all/56/chronopostfren.png')
insert into wxapp_express values ('correosdees', '', '西班牙(Correos de Espa?a)', 'https://cdn.kuaidi100.com/images/all/56/correosdees.png')
insert into wxapp_express values ('cypruspost', '', '塞浦路斯(Cyprus Post)', 'https://cdn.kuaidi100.com/images/all/56/cypruspost.png')
insert into wxapp_express values ('dpexen', '', 'Toll', 'https://cdn.kuaidi100.com/images/all/56/dpexen.png')
insert into wxapp_express values ('eltahell', '', '希腊EMS（ELTA Courier）', 'https://cdn.kuaidi100.com/images/all/56/eltahell.png')
insert into wxapp_express values ('emsukraine', '', '乌克兰EMS(EMS Ukraine)', 'https://cdn.kuaidi100.com/images/all/56/emsukraine.png')
insert into wxapp_express values ('fardarww', '', '颿达国际快递', 'https://cdn.kuaidi100.com/images/all/56/fardarww.png')
insert into wxapp_express values ('fastway', '', 'Fastway Ireland', 'https://cdn.kuaidi100.com/images/all/56/fastway.png')
insert into wxapp_express values ('fedexuk', '', 'FedEx-英国件（FedEx UK)', 'https://cdn.kuaidi100.com/images/all/56/fedexuk.png')
insert into wxapp_express values ('gatikwe', '', 'Gati-KWE', 'https://cdn.kuaidi100.com/images/all/56/gatikwe.png')
insert into wxapp_express values ('hkpost', '', '中国香港(HongKong Post)', 'https://cdn.kuaidi100.com/images/all/56/hkpost.png')
insert into wxapp_express values ('hrvatska', '', '克罗地亚（Hrvatska Posta）', 'https://cdn.kuaidi100.com/images/all/56/hrvatska.png')
insert into wxapp_express values ('interlink', '', 'Interlink Express', 'https://cdn.kuaidi100.com/images/all/56/interlink.png')
insert into wxapp_express values ('iparcel', '', 'UPS i-parcel', 'https://cdn.kuaidi100.com/images/all/56/iparcel.png')
insert into wxapp_express values ('italysad', '', 'Italy SDA', 'https://cdn.kuaidi100.com/images/all/56/italysad.png')
insert into wxapp_express values ('japanposten', '', '日本（Japan Post）', 'https://cdn.kuaidi100.com/images/all/56/japanposten.png')
insert into wxapp_express values ('kcs', '', 'KCS', 'https://cdn.kuaidi100.com/images/all/56/kcs.png')
insert into wxapp_express values ('koreapost', '', '韩国（Korea Post）', 'https://cdn.kuaidi100.com/images/all/56/koreapost.png')
insert into wxapp_express values ('lasership', '', 'LaserShip', 'https://cdn.kuaidi100.com/images/all/56/lasership.png')
insert into wxapp_express values ('latvia', '', '拉脱维亚(Latvijas Pasts)', 'https://cdn.kuaidi100.com/images/all/56/latvia.png')
insert into wxapp_express values ('ldxpress', '', '"><img src="https://cdn.kuaidi100.com/images/all/56/ldxpress.png" alt="LDXpress tracking" /></a><input name="postid" type="text" class="inp-search inp-default input-border" id="postid" placeholder="请输入您要查询的单号" autocomplete="off" value="" maxlength="100"><a id="query" onclick="query();" class="btn-default btn-search query-border"></a><a class="print-waybill" id="printWaybill"></a>      <p id="valideBox" class="serch-valicode hidden"><input name="valicode" type="text" class="inp-default inp-valicode" id="valicode" maxlength="8" placeholder="请您输入验证码"><span class="vali-images"><img id="valiimages" onclick="refreshCode()" alt="点击获取新验证码"></span><a onclick="refreshCode();" rel="nofollow">看不清，换一张</a></p>      <p id="telBox" class="pt10px hidden">请您输入电话号码：<input name="valicodetel" type="text" class="inp" id="valicodetel" maxlength="18" style="width:214px;"></p>      <ul class="input-tips hidden" id="inputTips"></ul>      <p class="box"></p>    </div>    <div id="errorTips" class="tips mt10px hidden"> <a onclick="$(\'#errorTips\').hide();" class="tips-close" title="关闭提示"></a>      <p id="errorMessage"></p>    </div>    <div id="example" class="font14px mt10px">例如，您可以输入：<a id="useTips"></a></div>    <div id="queryWait" class="loader-box hidden', 'https://cdn.kuaidi100.com/images/loading.gif')
insert into wxapp_express values ('lianbangkuaidi', '', '联邦快递', 'https://cdn.kuaidi100.com/images/all/56/lianbangkuaidi.png')
insert into wxapp_express values ('libanpost', '', '黎巴嫩(Liban Post)', 'https://cdn.kuaidi100.com/images/all/56/libanpost.png')
insert into wxapp_express values ('lithuania', '', '立陶宛（Lietuvos pa?tas）', 'https://cdn.kuaidi100.com/images/all/56/lithuania.png')
insert into wxapp_express values ('macedonia', '', '马其顿(Macedonian Post)', 'https://cdn.kuaidi100.com/images/all/56/macedonia.png')
insert into wxapp_express values ('maldives', '', '马尔代夫(Maldives Post)', 'https://cdn.kuaidi100.com/images/all/56/maldives.png')
insert into wxapp_express values ('malta', '', '马耳他（Malta Post）', 'https://cdn.kuaidi100.com/images/all/56/malta.png')
insert into wxapp_express values ('mexico', '', '墨西哥（Correos de Mexico）', 'https://cdn.kuaidi100.com/images/all/56/mexico.png')
insert into wxapp_express values ('mexicodenda', '', 'Mexico Senda Express', 'https://cdn.kuaidi100.com/images/all/56/mexicodenda.png')
insert into wxapp_express values ('moldova', '', '摩尔多瓦(Posta Moldovei)', 'https://cdn.kuaidi100.com/images/all/56/moldova.png')
insert into wxapp_express values ('mrw', '', 'MRW', 'https://cdn.kuaidi100.com/images/all/56/mrw.png')
insert into wxapp_express values ('multipack', '', 'Mexico Multipack', 'https://cdn.kuaidi100.com/images/all/56/multipack.png')
insert into wxapp_express values ('myhermes', '', 'MyHermes', 'https://cdn.kuaidi100.com/images/all/56/myhermes.png')
insert into wxapp_express values ('nigerianpost', '', '尼日利亚(Nigerian Postal)', 'https://cdn.kuaidi100.com/images/all/56/nigerianpost.png')
insert into wxapp_express values ('novaposhta', '', 'Nova Poshta', 'https://cdn.kuaidi100.com/images/all/56/novaposhta.png')
insert into wxapp_express values ('ocaargen', '', 'OCA Argentina', 'https://cdn.kuaidi100.com/images/all/56/ocaargen.png')
insert into wxapp_express values ('ontrac', '', 'OnTrac', 'https://cdn.kuaidi100.com/images/all/56/ontrac.png')
insert into wxapp_express values ('opek', '', 'OPEK', 'https://cdn.kuaidi100.com/images/all/56/opek.png')
insert into wxapp_express values ('portugalctt', '', '葡萄牙（Portugal CTT）', 'https://cdn.kuaidi100.com/images/all/56/portugalctt.png')
insert into wxapp_express values ('portugalseur', '', 'Portugal Seur', 'https://cdn.kuaidi100.com/images/all/56/portugalseur.png')
insert into wxapp_express values ('postenab', '', 'PostNord(Posten AB)', 'https://cdn.kuaidi100.com/images/all/56/postenab.png')
insert into wxapp_express values ('postennorge', '', '挪威（Posten Norge）', 'https://cdn.kuaidi100.com/images/all/56/postennorge.png')
insert into wxapp_express values ('postnl', '', '荷兰邮政(PostNL international registered mail)', 'https://cdn.kuaidi100.com/images/all/56/postnl.png')
insert into wxapp_express values ('purolator', '', 'Purolator', 'https://cdn.kuaidi100.com/images/all/56/purolator.png')
insert into wxapp_express values ('redexpress', '', 'Red Express', 'https://cdn.kuaidi100.com/images/all/56/redexpress.png')
insert into wxapp_express values ('romanian', '', '罗马尼亚（Posta Romanian）', 'https://cdn.kuaidi100.com/images/all/56/romanian.png')
insert into wxapp_express values ('ruidianyouzheng', '', '瑞典（Sweden Post）', 'https://cdn.kuaidi100.com/images/all/56/ruidianyouzheng.png')
insert into wxapp_express values ('safexpress', '', 'Safexpress', 'https://cdn.kuaidi100.com/images/all/56/safexpress.png')
insert into wxapp_express values ('saudipost', '', '沙特阿拉伯(Saudi Post)', 'https://cdn.kuaidi100.com/images/all/56/saudipost.png')
insert into wxapp_express values ('selektvracht', '', 'Selektvracht', 'https://cdn.kuaidi100.com/images/all/56/selektvracht.png')
insert into wxapp_express values ('serbia', '', '塞尔维亚(PE Post of Serbia)', 'https://cdn.kuaidi100.com/images/all/56/serbia.png')
insert into wxapp_express values ('seur', '', 'International Seur', 'https://cdn.kuaidi100.com/images/all/56/seur.png')
insert into wxapp_express values ('singpost', '', '新加坡小包(Singapore Post)', 'https://cdn.kuaidi100.com/images/all/56/singpost.png')
insert into wxapp_express values ('sinoex', '', '中外运速递-中文', 'https://cdn.kuaidi100.com/images/all/56/sinoex.png')
insert into wxapp_express values ('siodemka', '', 'Siodemka', 'https://cdn.kuaidi100.com/images/all/56/siodemka.png')
insert into wxapp_express values ('skynetmalaysia', '', 'SkyNet Malaysia', 'https://cdn.kuaidi100.com/images/all/56/skynetmalaysia.png')
insert into wxapp_express values ('southafrican', '', '南非（South African Post Office）', 'https://cdn.kuaidi100.com/images/all/56/southafrican.png')
insert into wxapp_express values ('swisspost', '', '瑞士(Swiss Post)', 'https://cdn.kuaidi100.com/images/all/56/swisspost.png')
insert into wxapp_express values ('tcixps', '', 'TCI XPS', 'https://cdn.kuaidi100.com/images/all/56/tcixps.png')
insert into wxapp_express values ('thailand', '', '泰国（Thailand Thai Post）', 'https://cdn.kuaidi100.com/images/all/56/thailand.png')
insert into wxapp_express values ('tnt', '', 'TNT', 'https://cdn.kuaidi100.com/images/all/56/tnt.png')
insert into wxapp_express values ('tntau', '', 'TNT Australia', 'https://cdn.kuaidi100.com/images/all/56/tntau.png')
insert into wxapp_express values ('tntitaly', '', 'TNT Italy', 'https://cdn.kuaidi100.com/images/all/56/tntitaly.png')
insert into wxapp_express values ('tntpostcn', '', 'TNT Post', 'https://cdn.kuaidi100.com/images/all/56/tntpostcn.png')
insert into wxapp_express values ('tntuk', '', 'TNT UK', 'https://cdn.kuaidi100.com/images/all/56/tntuk.png')
insert into wxapp_express values ('tollpriority', '', 'Toll Priority(Toll Online)', 'https://cdn.kuaidi100.com/images/all/56/tollpriority.png')
insert into wxapp_express values ('tunisia', '', '突尼斯EMS(Rapid-Poste)', 'https://cdn.kuaidi100.com/images/all/56/tunisia.png')
insert into wxapp_express values ('ukrpost', '', '乌克兰小包、大包(UkrPost)', 'https://cdn.kuaidi100.com/images/all/56/ukrpost.png')
insert into wxapp_express values ('ups', '', 'UPS', 'https://cdn.kuaidi100.com/images/all/56/ups.png')
insert into wxapp_express values ('upsfreight', '', 'UPS Freight', 'https://cdn.kuaidi100.com/images/all/56/upsfreight.png')
insert into wxapp_express values ('usps', '', 'USPS', 'https://cdn.kuaidi100.com/images/all/56/usps.png')
insert into wxapp_express values ('usps', '', 'USPS', 'https://cdn.kuaidi100.com/images/all/56/usps.png')
```
