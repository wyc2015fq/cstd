import os

MARKDOWN = {
    'h1': ('\n# ', ''),
    'h2': ('\n## ', ''),
    'h3': ('\n### ', ''),
    'h4': ('\n#### ', ''),
    'h5': ('\n##### ', ''),
    'h6': ('\n###### ', ''),
    'ul': ('', ''),
    'ol': ('', ''),
    'li': ('- ', ''),
    'blockquote': ('\n> ', '\n'),
    'em': ('*', '*'),
    'strong': ('**', '**'),
    'block_code': ('\n```\n', '\n```\n'),
    'span': ('', ''),
    'a': ('', ''),
    'p': ('', ''),
    'div': ('', ''),
    'inline_p': ('', ''),
    'inline_p_with_out_class': ('', ''),
    'b': ('**', '**'),
    'i': ('*', '*'),
    'del': ('~~', '~~'),
    'hr': ('\n---', '\n\n'),
    'thead': ('\n', '|------\n'),
    'tbody': ('\n', '\n'),
    'td': ('', ''),
    'th': ('', ''),
    'tr': ('', '\n'),
    'table': ('', '\n'),
    'annotation': ('', ''),
    'span-katex-inline': ('$', '$'),
    'span-katex-display': ('\n$$\n', '\n$$\n'),
    'code': ('\n```\n', '\n```\n'),
    'code-python': ('\n```python\n', '\n```\n'),
    'e_p': ('', '\n')
}
# coding = utf-8
def clearBlankLine():
    file1 = open('text1.txt', 'r', encoding='utf-8') # 要去掉空行的文件
    file2 = open('text2.txt', 'w', encoding='utf-8') # 生成没有空行的文件
    try:
        for line in file1.readlines():
            if line == '\n':
                line = line.strip("\n")
            file2.write(line)
    finally:
        file1.close()
        file2.close()


# katex--inline
# katex--display
from html.parser import HTMLParser
class MyHTMLParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.all_tag_stack = [('',{'attr':''})]
        self.skip_stack=[0]
        self.markdown=''
        self.is_end = 0

    def handle_starttag(self, tag, attrs):
        #print "Encountered the beginning of a %s tag" % tag
        attrs_dict={}
        for attr in attrs:
            attrs_dict[attr[0]]=attr[1]


        attr = {'attr':''}

        if 'title' ==tag:
            attr['attr'] = 'h1'

        if 'href' in attrs_dict:
            attr['href'] = attrs_dict['href']

        is_skip = 0

        if tag=='span' and 'class' in attrs_dict and attrs_dict['class']=='katex-html':
            is_skip = 1

        if tag in ['script', 'mrow']:
            is_skip = 1

        tt = len(self.all_tag_stack)

        if tag in MARKDOWN:
            attr['attr'] = (tag)

        if tag=='script' and 'type' in attrs_dict:
            #if attrs_dict['type'].find('MathJax')>=0:
            if attrs_dict['type']=='math/tex':
                attr['attr'] = ('span-katex-inline')
                is_skip = 0

            if attrs_dict['type']=='math/tex; mode=display':
                attr['attr'] = ('span-katex-display')
                is_skip = 0

        if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='hide-article-box hide-article-pos text-center':
            self.is_end = 1

        if tag=='span' and 'class' in attrs_dict and 'id' in attrs_dict:
            if attrs_dict['id'].find("MathJax-Span")>=0 or (attrs_dict['class'] in ['mo', 'mi']):
                is_skip = 1


        if tag=='span' and 'class' in attrs_dict:
            if attrs_dict['class'] in ['katex--display', 'katex-display']:
                attr['attr'] = ('span-katex-display')
            if attrs_dict['class'] in ['katex--inline', 'katex-inline']:
                attr['attr'] = ('span-katex-inline')

        if tag=='pre' and 'class' in attrs_dict:
            if attrs_dict['class']=='brush:py;':
                attr['attr'] = 'code-python'

        if tag=='code' and 'class' in attrs_dict:
            if attrs_dict['class']=='language-python':
                attr['attr'] = 'code-python'

        if tag=='br' and self.is_end==0:
            self.markdown+='\n'

        if tag=='img' and 'src' in attrs_dict and len(attrs_dict['src'].strip())>0 and self.is_end==0:
            attr['alt'] = ''
            if 'alt' in attrs_dict:
                attr['alt']=attrs_dict['alt']
            attr['href']=attrs_dict['src']
            data1 = '!['+attr['alt']+']('+attr['href']+')'
            self.markdown+=data1



        if is_skip==1:
            self.skip_stack.append(tt)

        self.all_tag_stack.append((tag, attr))

    def handle_endtag(self,tag):
        self.all_tag_stack.pop()
        if self.skip_stack[-1]==len(self.all_tag_stack):
            self.skip_stack.pop()
        if self.is_end==1:
            return
        if tag in ['p', 'div', 'h1', 'h2', 'h3', 'h4', 'h5', 'h6']:
            self.markdown+='\n'


    def handle_data(self,data):
        if self.is_end==1:
            return
        tag, attrs = self.all_tag_stack[-1]

        if tag in ['meta', 'link', 'style']:
            return

        attr = attrs['attr']
        if 'span-katex-display'==attr:
            asdf=0

        #if attr not in MARKDOWN:            return
        if attrs['attr'] == 'code-python':
            asdf = 0

        if len(self.skip_stack)>1:
            return

        data1 = data
        data1 = data1.replace("\r", "")
        if tag in MARKDOWN:
            data1 = data1.replace("\n", "")
        #data1 = data1.replace("\(", "$").replace("\)", "$")

        if 'href' in attrs:
            data1 = '['+data1+']('+attrs['href']+')'


        if tag=='annotation':
            attr = 'span-katex-display'
            attrs = list(map(lambda x:x[1]['attr'], self.all_tag_stack))
            if 'span-katex-inline' in attrs:
                attr = 'span-katex-inline'

        data1 = data1.strip()
        if len(data1)>0:
            if attr in MARKDOWN:
                t = MARKDOWN[attr]
                if 'span-katex-display'==attr:
                    asdf=0
                data1 = t[0]+data1+t[1]

        self.markdown+=data1


import chardet
import win32clipboard as w
import win32con

def autotext(t):
    codec = chardet.detect(t)
    return t.decode(codec['encoding'])

#获取剪切板内容
def gettext():
    w.OpenClipboard()
    print(dir(w))
    t = (w.GetClipboardFormatName(win32con.CF_TEXT))
    t = w.GetClipboardData(win32con.CF_TEXT)
    w.CloseClipboard()
    return t

#str1 = gettext()
#str1 = autotext(str1)
#print(str1)

fn = './test5.html'
f=open(fn,'rb')
t=f.read()
t = autotext(t)
f.close()


html_code = t
hp = MyHTMLParser()
hp.feed(html_code)
d = str(hp.markdown)
d = d.replace('\n\n', '\n')
hp.close()
# print(d)

#print(d)
fn2 = './test.md'
f=open(fn2,'wt',encoding='utf8')
f.write(d)
f.close()
