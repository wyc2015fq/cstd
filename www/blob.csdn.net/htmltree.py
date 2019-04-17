import os
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
        self.tag_stack.append((tag, self.ind))
        attrs_dict={}
        for attr in attrs:
            attrs_dict[attr[0]]=attr[1]
        self.jj.append({'ind':self.ind, 'tag':tag, 'attrs':attrs_dict})
        if self.ind<100:
            self.ind+=1


    def handle_endtag(self, tag):
        end=len(self.tag_stack)-3
        end=max(end, 0)
        for i in range(len(self.tag_stack)-1, end, -1):
            if self.tag_stack[i][0]==tag:
                self.ind=self.tag_stack[i][1]
                self.tag_stack.pop()
                break

        return

    def handle_data(self, data):
        data = self.unescape(data)
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
        if attrs_dict['class'] in ['article-copyright', 'mylinks', 'blogStats', 'header', 'postTitle', 'author', 'article-title-box']:
            return parser_skip(parser, jj, i, l, out)

        if attrs_dict['class']=='cnblogs_code':
            text, ii = parser_code(parser, jj, i, l, out)
            return text, ii

    text, ii = parser_block(parser, jj, i, l, out, '\n', '\n')
    #text, ii = parser_text(parser, jj, i, l, out)
    return text, ii
    #return parser_text(parser, jj, i, l, out)

def parser_title(parser, jj, i, l, out):
    text, ii = parser_text(parser, jj, i, l, out)
    text = toline(text).strip()
    out['title']=text
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
    text = ''
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

        for tr in trs[1::]:
            tds = node_find_tag(jj, tr, l, 'td')
            rows.append(tds)
            cols = max(len(tds), cols)

        if cols>0:
            text1, i = parser_tr(parser, jj, i, ths, l, out, cols)
            text += text1 + '\n'
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
    for s in ['src', 'data-original-src', 'data-src']:
        if s in attrs_dict and len(attrs_dict[s].strip())>0:
            href = attrs_dict[s]

    if  len(href.strip())>0:
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

    if tag=='div' and 'id' in attrs_dict and attrs_dict['id']=='main':
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


FILTER = {
    'csdn': {'filter':node_filter_csdn, 'site':'https://blog.csdn.net', 'root':'CSDN博客'},
    'cnblogs': {'filter':node_filter_cnblogs, 'site':'https://www.cnblogs.com', 'root':'博客园'},
    'www.datakit.cn': {'filter':node_filter_datakit, 'site':'https://www.datakit.cn', 'root':'www.datakit.cn'},
    'hao.jobbole.com': {'filter':node_filter_jobbole, 'site':'http://hao.jobbole.com', 'root':'伯乐在线'},
    'blog.chinaunix.net': {'filter':node_filter_chinaunix, 'site':'http://blog.chinaunix.net', 'root':'ChinaUnix博客'},
    'www.jianshu.com': {'filter':node_filter_jianshu, 'site':'https://www.jianshu.com', 'root':'简书'},
    'baike': {'filter':node_filter_baike, 'site':'', 'root':'百度百科'},
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
    out={'title':'title', 'site':FILTER[aaa]['site']}
    for jjs in jjs_pair:
        i, l=jjs[0], jjs[1]
        while(i<l):
            text, i = parser_x(parser, jj, i, l, out)
            d += text

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
