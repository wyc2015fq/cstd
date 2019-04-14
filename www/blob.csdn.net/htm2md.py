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
    'b': ('**', '**'),
    'i': ('*', '*'),
    'del': ('~~', '~~'),
    'td': ('|', ''),
    'th': ('|', ''),
    'span-katex-inline': ('$', '$'),
    'span-katex-display': ('\n$$\n', '\n$$\n'),
    'code-inline': ('`', '`'),
    'code-unkown': ('\n```\n', '\n```\n'),
    'code-python': ('\n```python\n', '\n```\n'),
    'code-php': ('\n```php\n', '\n```\n'),
    'code-java': ('\n```java\n', '\n```\n'),
    'code-cpp': ('\n```cpp\n', '\n```\n'),
    'code-matlab': ('\n```matlab\n', '\n```\n'),
    'code-bash': ('\n```bash\n', '\n```\n'),
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
        self.title = ''
        self.cols=0

    def handle_starttag(self, tag, attrs):
        #print "Encountered the beginning of a %s tag" % tag
        attrs_dict={}
        for attr in attrs:
            attrs_dict[attr[0]]=attr[1]

        attr = {'attr':''}
        if 'tr'==tag:
            self.cols=0
        if 'th'==tag:
            self.cols+=1

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

        if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='postDesc':
            self.is_end = 1

        if tag=='div' and 'class' in attrs_dict and attrs_dict['class']=='show-foot':
            self.is_end = 1

        if tag=='span' and 'class' in attrs_dict and 'id' in attrs_dict:
            if attrs_dict['id'].find("MathJax")>=0 or (attrs_dict['class'] in ['mo', 'mi']):
                is_skip = 1

        if tag=='div' and 'id' in attrs_dict:
            if attrs_dict['id'] in ['mylinks', 'blogStats', 'header']:
                is_skip = 1

        if tag=='h1' and 'class' in attrs_dict:
            if attrs_dict['class'] in ['title']:
                is_skip = 1

        if tag=='div' and 'class' in attrs_dict:
            if attrs_dict['class'] in ['article-copyright', 'mylinks', 'blogStats', 'header', 'postTitle', 'author', 'article-title-box']:
                is_skip = 1


        if tag=='nav' and 'class' in attrs_dict:
            if attrs_dict['class'] in ['navbar navbar-default navbar-fixed-top']:
                is_skip = 1


        if tag=='span' and 'class' in attrs_dict:
            if attrs_dict['class'] in ['katex--display', 'katex-display']:
                attr['attr'] = ('span-katex-display')
            if attrs_dict['class'] in ['katex--inline', 'katex-inline']:
                attr['attr'] = ('span-katex-inline')

        if tag=='pre' and 'class' in attrs_dict:
            if attrs_dict['class']=='brush:py;':
                attr['attr'] = 'code-python'
            if attrs_dict['class']=='php hljs':
                attr['attr'] = 'code-php'

        if tag=='code':
            attr['attr'] = 'code-unkown'
            attr['attr'] = 'code-inline'
            if 'class' in attrs_dict:
                for l in ['java', 'python', 'php', 'cpp', 'matlab', 'bash']:
                    if attrs_dict['class'].find(l)>=0:
                        attr['attr'] = 'code-'+l
                for l in ['language-py']:
                        attr['attr'] = 'code-python'

        if len(self.skip_stack)==1:
            if tag=='br' and self.is_end==0:
                self.markdown+='\n'

            if tag=='hr' and self.is_end==0:
                self.markdown+='\n---\n\n'
            if 'data-original-src' in attrs_dict:
                attrs_dict['src'] = attrs_dict['data-original-src']

            if tag=='img' and 'src' in attrs_dict and len(attrs_dict['src'].strip())>0 and self.is_end==0:
                attr['alt'] = ''
                if 'alt' in attrs_dict:
                    attr['alt']=attrs_dict['alt']
                attr['href']=attrs_dict['src']
                data1 = '!['+attr['alt']+']('+attr['href']+')'
                self.markdown+=data1



        if is_skip==1:
            self.skip_stack.append(tt)

        if self.all_tag_stack[-1][1]["attr"] in MARKDOWN:
            attr["attr"] = self.all_tag_stack[-1][1]["attr"]
        self.all_tag_stack.append((tag, attr))

    def handle_endtag(self,tag):
        self.all_tag_stack.pop()
        if self.skip_stack[-1]==len(self.all_tag_stack):
            self.skip_stack.pop()
        if self.is_end==1:
            return
        if tag in ['p', 'li','div', 'h1', 'h2', 'h3', 'h4', 'h5', 'h6']:
            self.markdown+='\n'

        if tag in ['tr']:
            self.markdown+='|\n'

        if tag in ['thead']:
            self.markdown+='|---'*self.cols + '|\n'


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

        if 'title' ==tag:
            data1 = data1.replace('/', '-').replace('\\', '-').replace('*', '-')
            self.title = data

        data1 = data1.replace("\r", "")
        if len(attrs['attr'])<4 or attrs['attr'][0:4]!='code':
            data1 = data1.replace("$$", "\n$$\n")
            data1 = data1.replace("#", "\\#")

        if tag in MARKDOWN:
            data1 = data1.replace("\n", "")
        #data1 = data1.replace("\(", "$").replace("\)", "$")

        if 'href' in attrs:
            data1 = '['+data1.strip()+']('+attrs['href']+')'


        if tag=='annotation':
            attr = 'span-katex-display'
            attrs = list(map(lambda x:x[1]['attr'], self.all_tag_stack))
            if 'span-katex-inline' in attrs:
                attr = 'span-katex-inline'

        data1 = data1.strip()
        if len(data1)>0:
            if attr in MARKDOWN:
                t = MARKDOWN[attr]
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

def htm2md(html_code):
    hp = MyHTMLParser()
    hp.feed(html_code)
    d = str(hp.markdown)
    title = hp.title
    d = d.replace('\n\n', '\n')
    hp.close()
    return title, d


if __name__ == '__main__':

    fn = './test1.html'
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
    