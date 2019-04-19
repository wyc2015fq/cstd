import os
import html
from html.parser import HTMLParser


class MyHTMLParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.jj=[]
        self.ind=0
        self.jj.append({'ind':self.ind, 'tag':'text', 'attrs':{'data':''}})
        self.tag_stack=['']
        self.ind=1

    def handle_starttag(self, tag, attrs):
        attrs_dict={}
        for attr in attrs:
            attrs_dict[attr[0]]=attr[1]
        self.jj.append({'ind':self.ind, 'tag':tag, 'attrs':attrs_dict})
        if self.ind<100:
            if tag not in ['br', 'wbr']:
                self.tag_stack.append((tag, self.ind))
                self.ind+=1


    def handle_endtag(self, tag):
        end=len(self.tag_stack)-30
        end=max(end, 0)
        ind = self.jj[-1]['ind']
        for i in range(len(self.tag_stack)-1, end, -1):
            if self.tag_stack[i][0]==tag:
                self.ind=self.tag_stack[i][1]
                self.tag_stack.pop()
                break

        return

    def handle_data(self, data):
        data = html.unescape(data)
        self.jj.append({'ind':self.ind, 'tag':'text', 'attrs':{'data':data}})


def parser_x(parser, jj, i, l, out):
    tag='text'
    if jj[i]['tag'] in parser:
        tag = jj[i]['tag']

    return parser[tag](parser, jj, i, l, out)

def parser_text(parser, jj, i, l, out):
    text=''
    attrs = jj[i]['attrs']
    if 'data' in attrs:
        if 'in_code' in out and out['in_code']==1:
            text += attrs['data']
        else:
            if len(attrs['data'].strip())>0:
                text += attrs['data']

    ind=jj[i]['ind']
    ii = i+1
    text1=''
    while(ii<l):
        if jj[ii]['ind']<=ind:
            break
        t, ii = parser_x(parser, jj, ii, l, out)
        text1 += t

    if jj[i]['tag']=='a' and 'href' in attrs and text1[0:2]!='![':
        text1 = '['+text1.strip()+']('+attrs['href']+')'

    text += text1
    return text, ii

def parser_code_text(parser, jj, i, l, out):
    out['in_code'] = 1
    text, ii = parser_text(parser, jj, i, l, out)
    out['in_code'] = 0
    return text, ii

def toline(text):
    return text.replace("\r", "").replace("\n", "")

def parser_skip(parser, jj, i, l, out):
    ind=jj[i]['ind']
    ii = i+1
    while(ii<l):
        if jj[ii]['ind']<=ind:
            break
        ii+=1
    return '', ii

def parser_inline(parser, jj, i, l, out, beg, end):
    text, ii =parser_text(parser, jj, i, l, out)
    text = toline(text)
    if len(text)>0:
        if len(text)<(len(beg)+len(end)) or text[0:len(beg)]!=beg or text[-len(end):]!=end:
            text = beg + text + end
    return text, ii

def parser_h1(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n# ', '\n')

def parser_h2(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n## ', '\n')

def parser_h3(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n### ', '\n')

def parser_h4(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n#### ', '\n')

def parser_h5(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n##### ', '\n')

def parser_h6(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '\n###### ', '\n')

def parser_br(parser, jj, i, l, out):
    return '\n', i+1

def parser_mathjax_inline(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '$', '$')

def parser_mathjax_display(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '$', '$')

def parser_block(parser, jj, i, l, out, beg, end):
    text, ii = parser_text(parser, jj, i, l, out)
    text = beg + (text) + end
    return text, ii

def parser_p(parser, jj, i, l, out):
    return parser_block(parser, jj, i, l, out, '\n', '\n')

def parser_code_block(parser, jj, i, l, out, lang):
    text, ii = parser_code_text(parser, jj, i, l, out)
    text = '\n```'+lang+'\n' + (text.strip()) + '\n```\n'
    return text, ii

def parser_code(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    if 'class' in attrs_dict:
        cc=attrs_dict['class']
        for lang in ['java', 'python', 'php', 'cpp', 'matlab', 'bash', 'xml']:
            if cc.find(lang)>=0:
                return parser_code_block(parser, jj, i, l, out, lang)

        for lang in ['language-py']:
            if cc.find(lang)>=0:
                return parser_code_block(parser, jj, i, l, out, 'python')

    text, ii = parser_code_text(parser, jj, i, l, out)
    if text.count('\n')>0:
        text = '\n```\n' + (text.strip()) + '\n```\n'
    else:
        text = '`' + toline(text) +'`'
    return text, ii


def parser_span(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    if 'class' in attrs_dict:
        cls = attrs_dict['class']
        if (cls in ['mo', 'mi', 'katex-html']):
            return parser_skip(parser, jj, i, l, out)

        if cls in ['katex--display', 'katex-display']:
            return parser_mathjax_display(parser, jj, i, l, out)

        if cls in ['katex--inline', 'katex-inline']:
            return parser_mathjax_inline(parser, jj, i, l, out)

        if cls in ['cnblogs_code']:
            return parser_code(parser, jj, i, l, out)

    if 'id' in attrs_dict:
        if attrs_dict['id'].find("MathJax")>=0:
            return parser_skip(parser, jj, i, l, out)

    return parser_text(parser, jj, i, l, out)


def parser_pre(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    if 'class' in attrs_dict:
        cls = attrs_dict['class']
        if cls[0:6]=='brush:':
            lang = cls.split(':')[1]
            return parser_code_block(parser, jj, i, l, out, lang)

    return parser_text(parser, jj, i, l, out)


def parser_script(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    if 'type' in attrs_dict:
        if attrs_dict['type']=='math/tex':
            return parser_mathjax_inline(parser, jj, i, l, out)

        if attrs_dict['type']=='math/tex; mode=display':
            return parser_mathjax_display(parser, jj, i, l, out)

    return parser_skip(parser, jj, i, l, out)

def parser_li(parser, jj, i, l, out):
    #return parser_inline(parser, jj, i, l, out, '- ', '\n')
    return parser_block(parser, jj, i, l, out, '- ', '\n')

def parser_em(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '*', '*')

def parser_strong(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '**', '**')

def parser_del(parser, jj, i, l, out):
    return parser_inline(parser, jj, i, l, out, '~~', '~~')

def parser_blockquote(parser, jj, i, l, out):
    return parser_block(parser, jj, i, l, out, '\n> ', '\n')

def parser_div(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    if 'class' in attrs_dict:
        if attrs_dict['class'] in ['cnblogs_code_collapse', 'article-copyright', 'mylinks', 'blogStats', 'header', 'postTitle', 'author', 'article-title-box']:
            return parser_skip(parser, jj, i, l, out)

        if attrs_dict['class'] in ['cnblogs_code', 'crayon-plain-wrap']:
            text, ii = parser_code(parser, jj, i, l, out)
            return text, ii

    text, ii = parser_block(parser, jj, i, l, out, '\n', '\n')
    #text, ii = parser_text(parser, jj, i, l, out)
    return text, ii
    #return parser_text(parser, jj, i, l, out)

def parser_title(parser, jj, i, l, out):
    text, ii = parser_text(parser, jj, i, l, out)
    text = toline(text).strip()
    if len(text)>0 and len(out['title'])==0:
        out['title']=text
    text.replace('_', ' - ')
    text = '# ' + text + '\n'
    return text, ii

def node_find_tag(jj, i, l, tag):
    out=[]

    ind=jj[i]['ind']
    for ii in range(i+1, l):
        if jj[ii]['ind']<=ind:
            break
        if jj[ii]['tag']==tag:
            out.append(ii)

    return out

def parser_tr(parser, jj, i, ths, l, out, cols):
    text='|'
    for th in ths:
        text1, i = parser_inline(parser, jj, th, l, out, '', '')
        text += text1.strip() + '|'
    for j in range(len(ths), cols):
        text += ' |'
    return text, i

def get_attr(jji, name):
    if name in jji['attrs']:
        return jji['attrs'][name]
    return None

def parser_table(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    text = ''
    if 'class' in attrs_dict and attrs_dict['class']=='crayon-table':
        return parser_skip(parser, jj, i, l, out)

    trs = node_find_tag(jj, i, l, 'tr')
    if len(trs)>0:
        ths = node_find_tag(jj, trs[0], l, 'th')
        if len(ths)==0:
            ths = node_find_tag(jj, trs[0], l, 'td')
        rows=[]
        cols = len(ths)
        if len(trs)==1 and len(ths)==2:
            if get_attr(jj[ths[0]], 'class')=='gutter' and get_attr(jj[ths[1]], 'class')=='code':
                return parser_code(parser, jj, ths[1], l, out)

        if len(ths)==1:
            for tr in trs:
                text1, i = parser_x(parser, jj, tr, l, out)
                text += text1 + '\n'
            return text1, i

        for tr in trs[1::]:
            tds = node_find_tag(jj, tr, l, 'td')
            rows.append(tds)
            cols = max(len(tds), cols)

        if cols>0:
            text1, i = parser_tr(parser, jj, i, ths, l, out, cols)
            text += text1 + '\n'
            if len(trs)>1:
                text += '|----'*cols
                text += '|\n'
                for tds in rows:
                    text1,i = parser_tr(parser, jj, i, tds, l, out, cols)
                    text += text1 + '\n'

    return text, i+1

def parser_img(parser, jj, i, l, out):
    attrs_dict = jj[i]['attrs']
    alt = ''
    href = ''
    for s in ['src', 'data-original-src', 'data-src', 'real_src']:
        if s in attrs_dict and len(attrs_dict[s].strip())>0:
            href = attrs_dict[s]

    is_skip=0
    for skipimg in ['svg','ExpandedBlockStart.gif', 'ContractedBlock.gif', 'copycode.gif']:
        if href.find(skipimg)>0:
            is_skip = 1

    if  len(href.strip())>0 and is_skip==0:
        if 'alt' in attrs_dict:
            alt=attrs_dict['alt']

        if href[0:2]=='//':
            href = out['site'].split(':')[0]+':'+href

        if href[0]=='/':
            href = out['site']+'/'+href.strip('/')

        text = '!['+alt+']('+href+')'
        return text, i+1
    return parser_skip(parser, jj, i, l, out)


parser={
    'title': parser_title,
    'table': parser_table,
    'img': parser_img,
    'div': parser_div,
    'pre': parser_pre,
    'li': parser_li,
    'dt': parser_li,
    'em': parser_em,
    'i': parser_em,
    'b': parser_strong,
    'strong': parser_strong,
    'del': parser_del,
    'p': parser_p,
    'blockquote': parser_blockquote,
    'br': parser_br,
    'wbr': parser_skip,
    'h1': parser_h1,
    'h2': parser_h2,
    'h3': parser_h3,
    'h4': parser_h4,
    'h5': parser_h5,
    'h6': parser_h6,
    'text': parser_text,
    'code': parser_code,
    'span': parser_span,
    'style': parser_skip,
    'mi': parser_skip,
    'mo': parser_skip,
    'mrow': parser_skip,
    'script': parser_script
}


import chardet
import win32clipboard as w
import win32con
import json

def autotext(t):
    codec = chardet.detect(t)
    return t.decode(codec['encoding'])

def node_find(jj, i, l, node_filter):
    out=[]

    ind=jj[i]['ind']
    prev2 = 0
    for ii in range(i+1, l):
        if jj[ii]['ind']<=ind:
            break
        t = node_filter(jj[ii])
        if t>0:
            if t==1:
                prev2 = 0
                out.append((ii, ii+2))
            if t==2:
                prev2 = ii
            if t==3 and prev2>0:
                out.append((prev2, ii))

    return out


def node_filter_csdn(node):
    tag = node['tag']
    attrs = node['attrs']
    attrs_dict = attrs
    if tag=='title':
        return 1

    if tag=='div' and 'class' in attrs and attrs['class']=='blog-content-box':
        return 2

    if tag=='article' and 'class' in attrs and attrs['class']=='baidu_pl':
        return 0

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='hide-article-box hide-article-pos text-center':
        return 3

    return 0

def node_filter_cnblogs(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'id' in attrs_dict and attrs_dict['id'] in ['main', 'post']:
        return 2

    if tag=='div' and 'id' in attrs_dict and attrs_dict['id']=='blog_post_info_block':
        return 3

    return 0

def node_filter_datakit(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='container page-content':
        return 2

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='navbar panel-footer operate':
        return 3

    return 0


def node_filter_jobbole(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='article' and 'class' in attrs_dict and attrs_dict['class']=='rpost-entry':
        return 2

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='post-adds':
        return 3

    return 0


def node_filter_chinaunix(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='Blog_right1':
        return 2

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='Blog_con2_1 Blog_con3_2':
        return 3

    return 0

def node_filter_jianshu(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='article':
        return 2

    if tag=='div' and 'data-vcomp' in attrs_dict and attrs_dict['data-vcomp']=='free-reward-panel':
        return 3

    return 0

def node_filter_baike(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='promotion-declaration':
        return 2

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='side-content':
        return 3

    return 0

def node_filter_zhihu(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='Post-RichTextContainer':
        return 2

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='ContentItem-time':
        return 3

    return 0

def node_filter_sciencenet(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='bm_c':
        return 2

    if tag=='div' and 'id' in attrs_dict and attrs_dict['id']=='click_div':
        return 3

    return 0

def node_filter_sina(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='articalTag':
        return 2

    if tag=='div' and 'id' in attrs_dict and attrs_dict['id']=='share':
        return 3

    return 0


def node_filter_jobbole(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='entry':
        return 2

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='post-adds':
        return 3

    return 0


def node_filter_skywind(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='post':
        return 2

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='under':
        return 3

    return 0

def node_filter_sohu(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='article' and 'class' in attrs_dict and attrs_dict['class']=='article':
        return 2

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='statement':
        return 3

    return 0

def node_filter_51cto(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='artical-content-bak main-content':
        return 2

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='artical-copyright mt26':
        return 3

    return 0


def node_filter_51cto(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='artical-content-bak main-content':
        return 2

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='artical-copyright mt26':
        return 3

    return 0

def node_filter_gameinstitute(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 1

    attrs_dict = attrs

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='detail-data':
        return 2

    if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='art-foot':
        return 3

    return 0


FILTER = {
    'csdn': {'filter':node_filter_csdn, 'site':'https://blog.csdn.net', 'root':'CSDN博客'},
    'cnblogs': {'filter':node_filter_cnblogs, 'site':'https://www.cnblogs.com', 'root':'博客园'},
    'www.datakit.cn': {'filter':node_filter_datakit, 'site':'https://www.datakit.cn', 'root':'www.datakit.cn'},
    'hao.jobbole.com': {'filter':node_filter_jobbole, 'site':'http://hao.jobbole.com', 'root':'伯乐在线'},
    'blog.chinaunix.net': {'filter':node_filter_chinaunix, 'site':'http://blog.chinaunix.net', 'root':'ChinaUnix博客'},
    'www.jianshu.com': {'filter':node_filter_jianshu, 'site':'https://www.jianshu.com', 'root':'简书'},
    'baike': {'filter':node_filter_baike, 'site':'', 'root':'百度百科'},
    'zhihu.com': {'filter':node_filter_zhihu, 'site':'', 'root':'知乎'},
    'blog.sciencenet.cn': {'filter':node_filter_sciencenet, 'site':'', 'root':'科学网'},
    'blog.sina.com.cn': {'filter':node_filter_sina, 'site':'', 'root':'新浪博客'},
    'blog.jobbole.com': {'filter':node_filter_jobbole, 'site':'', 'root':'伯乐在线'},
    'www.skywind.me': {'filter':node_filter_skywind, 'site':'', 'root':'Skywind'},
    'www.sohu.com': {'filter':node_filter_sohu, 'site':'', 'root':'搜狐'},
    'blog.51cto.com': {'filter':node_filter_51cto, 'site':'', 'root':'51CTO博客'},
    'gameinstitute.qq.com': {'filter':node_filter_gameinstitute, 'site':'', 'root':'腾讯游戏学院'},
}

def savetext(fn, d):
    f=open(fn,'wt', encoding='utf8')
    f.write(d)
    f.close()
    #print(d)

def htm2md(t):
    html_code = t.replace('\r', '')
    hp = MyHTMLParser()
    hp.feed(html_code)
    jj=hp.jj
    hp.close()
    node_filter = node_filter_csdn
    cc = 0
    aaa = 'csdn'
    for aa in FILTER:
        tc = t.count(aa)
        if tc>cc:
            cc = tc
            aaa = aa
            node_filter = FILTER[aa]['filter']

    jjs_pair = node_find(jj, 0, len(jj), node_filter)
    d = ''
    out={'title':'', 'site':FILTER[aaa]['site']}
    for jjs in jjs_pair:
        i, l=jjs[0], jjs[1]
        while(i<l):
            text, i = parser_x(parser, jj, i, l, out)
            d += text

    if aaa=='zhihu.com':
        html = etree.HTML(html_code)
        aa='//a[@data-za-detail-view-element_name]/text()'
        ll = html.xpath(aa)
        if len(ll)>0:
            out['title'] = out['title'].replace('- ', ' - ' + ll[0] +' - ')


    if aaa=='www.jianshu.com':
        html = etree.HTML(html_code)
        aa='//div[@class="info"]/span[@class="name"]/a/text()'
        ll = html.xpath(aa)
        if len(ll)>0:
            out['title'] = out['title'].replace('- ', ' - ' + ll[0] +' - ')

    if aaa=='blog.sciencenet.cn':
        print(out['title'])
        out['title'] = out['title'].replace('科学网—', '') + ' - 科学网'
        #html = etree.HTML(html_code)

    if aaa=='blog.sina.com.cn':
        print(out['title'])
        out['title'] = out['title'].replace('_', ' - ')
        #html = etree.HTML(html_code)

    if aaa=='blog.jobbole.com':
        print(out['title'])
        html = etree.HTML(html_code)
        aa='//div[@class]/p/a[@rel="category tag"]/text()'
        ll = html.xpath(aa)[0]
        ll = ll.replace('/', '-').strip()
        out['title'] = out['title'].replace('文章 - 伯乐在线', ll + ' - 伯乐在线')
        #html = etree.HTML(html_code)

    if aaa=='www.skywind.me':
        print(out['title'])
        html = etree.HTML(html_code)
        aa='//div[@id]/a[@rel="category tag"]/text()'
        ll = html.xpath(aa)[0]
        ll = ll.replace('/', '-').strip()
        out['title'] = out['title'].replace('Skywind Inside » ', '').strip() + ' - ' + ll +' - Skywind'
        #html = etree.HTML(html_code)

    if aaa=='www.sohu.com':
        print(out['title'])
        html = etree.HTML(html_code)
        aa='//meta[@name="mediaid"]/@content'
        ll = html.xpath(aa)[0].strip()
        out['title'] = out['title'].strip() + ' - ' + ll +' - 搜狐'

    if aaa=='blog.51cto.com':
        print(out['title'])
        out['title'] = out['title'].replace('-', ' - ')

    if aaa=='gameinstitute.qq.com':
        print(out['title'])
        html = etree.HTML(html_code)
        aa='//span[@class="name"]/text()'
        ll = html.xpath(aa)[0].strip()
        out['title'] = out['title'].replace('-腾讯游戏学院', ' - ' + ll + ' - 腾讯游戏学院')

    d = d.replace('\n\n\n', '\n').replace('\n\n\n', '\n').replace('\n\n', '\n')
    return FILTER[aaa]['root'], out['title'], d

from down import *

if __name__ == '__main__':
    fn = './test1.html'
    f=open(fn,'rb')
    t=f.read()
    t = autotext(t)
    f.close()

    t = gettext().decode('gbk')
    root, title, d = htm2md(t)
    save_txt_td(root, title, d)
    print(title)
    fn2 = './test.md'
    f=open(fn2,'wt',encoding='utf8')
    f.write(d)
    f.close()
    #print(d)
