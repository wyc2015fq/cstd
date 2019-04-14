import os
from html.parser import HTMLParser


tt=0
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
        if tt<200:
            print(self.ind, ' '*self.ind, tag)
        attrs_dict={}
        for attr in attrs:
            attrs_dict[attr[0]]=attr[1]
        self.jj.append({'ind':self.ind, 'tag':tag, 'attrs':attrs_dict})
        self.ind+=1
        

    def handle_endtag(self, tag):
        global tt
        tt=tt+1
        end=len(self.tag_stack)-4
        end=max(end, 0)
        for i in range(len(self.tag_stack)-1, end, -1):
            if self.tag_stack[i][0]==tag:
                self.ind=self.tag_stack[i][1]
                self.tag_stack.pop()
                break

        if tt<33:
            print(self.ind, ' '*self.ind, tag, '>')
        return

    def handle_data(self, data):
        if len(data.strip())>0:
            self.jj.append({'ind':self.ind, 'tag':'text', 'attrs':{'data':data}})


def get_tree(jj, beg):
    tt={'tag':jj[beg]['tag'], 'attrs':jj[beg]['attrs'], 'subs':[]}
    ind=jj[beg]['ind']+1
    for i in range(beg+1, len(jj)):
        ind2=jj[i]['ind']
        if ind2>=ind:
            if ind2==ind:
                tt['subs'].append(get_tree(jj, i))
        else:
            return tt
            
    return tt
 
def parser_x(parser, node):
    text=''
    tag=node['tag']
    if tag in parser:
        text += parser[tag](parser, node)
    else:
        text += parser['text'](parser, node)
        
    return text

def parser_text(parser, node):
    text=''
    if 'data' in node['attrs']:
        text += node['attrs']['data']

    for snode in node['subs']:
        text += parser_x(parser, snode)
    return text

def toline(text):
    return text.replace("\r", "").replace("\n", "")

def parser_script(parser, node):
    return ''

def parser_inline(parser, node, beg, end):
    text=''
    text+=parser_text(parser, node)
    return '\n' + beg + toline(text) + end + '\n'
    return text

def parser_h1(parser, node):
    return parser_inline(parser, node, '# ', '')

def parser_h2(parser, node):
    return parser_inline(parser, node, '## ', '')

def parser_h3(parser, node):
    return parser_inline(parser, node, '### ', '')

def parser_h4(parser, node):
    return parser_inline(parser, node, '#### ', '')

def parser_h5(parser, node):
    return parser_inline(parser, node, '##### ', '')

def parser_h6(parser, node):
    return parser_inline(parser, node, '###### ', '')


parser={
    '_filter': parser_h1,
    'title': parser_h1,
    'h1': parser_h1,
    'h2': parser_h2,
    'h3': parser_h3,
    'h4': parser_h4,
    'h5': parser_h5,
    'h6': parser_h6,
    'text': parser_text,
    'script': parser_script
}


import chardet
import win32clipboard as w
import win32con
import json

def autotext(t):
    codec = chardet.detect(t)
    return t.decode(codec['encoding'])


def node_filter(node):
    tag = node['tag']
    attrs = node['attrs']
    if tag=='title':
        return 0
    
    if tag=='div' and 'class' in attrs and attrs['class']=='blog-content-box':
        return 0
    
    if tag=='article' and 'class' in attrs and attrs['class']=='baidu_pl':
        return 1
    
    return 0

def node_count(node):
    out=1
    for snode in node['subs']:
        out += node_count(snode)
    return out

def node_find(node):
    out=[]
    if node_filter(node)==1:
        out.append(node)

    for snode in node['subs']:
        out += node_find(snode)

    return out


def savetext(fn, d):
    f=open(fn,'wt', encoding='utf8')
    f.write(d)
    f.close()
    #print(d)

if __name__ == '__main__':

    fn = './test1.html'
    f=open(fn,'rb')
    t=f.read()
    t = autotext(t)
    f.close()

    html_code = t
    hp = MyHTMLParser()
    hp.feed(html_code)
    jj=hp.jj
    hp.close()
    #print(json.dumps(jj, indent=2))
    savetext('jj.txt', json.dumps(jj, indent=2))
    jj=get_tree(jj, 0)
    jjs = node_find(jj)
    d = ''
    for jj in jjs:
        print('   ', node_count(jj))
        savetext('jj1.txt', json.dumps(jj, indent=2))
        d += parser_x(parser, jj)

    fn2 = './test.md'
    f=open(fn2,'wt',encoding='utf8')
    f.write(d)
    f.close()
    #print(d)


