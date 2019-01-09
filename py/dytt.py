from urllib import request
import re

for i in range(3):
    url = 'http://www.ygdy8.com/html/gndy/dyzz/list_23_'+str(i)+'.html'
    headers = {
        'Referer': 'http://www.ygdy8.com/',
        'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.26 Safari/537.36 Core/1.63.5702.400 QQBrowser/10.2.1893.400'
    }
    req = request.Request(url, headers=headers)
    response = request.urlopen(req)
    html = response.read().decode('gbk', 'ignore')
    art = r'<a href="(.*?)" class="ulink">'
    i = re.findall(art, html)
    for n in i:
        url = 'http://www.ygdy8.com'+str(n)
        req = request.Request(url, headers=headers)
        response = request.urlopen(req)
        html1 = response.read().decode('gbk', 'ignore')
        asr = r'<title>(.*?)</title>'
        title = re.search(asr, html1).group(1)
        aer = r'<a href="(.*?)"><strong><font'
        cili = re.search(aer, html1).group(1)
        acr = r'bgcolor="#fdfddf"><a href="(.*?)">ftp'
        xunlei = re.search(acr, html1).group(1)
        print(title, cili, xunlei)
        print("\n")


