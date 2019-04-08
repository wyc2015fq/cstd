import os



fn = './test.html'
f=open(fn,'rt',encoding='utf8')
t=f.read()
f.close()

MARKDOWN = {
    'h1': ('\n# ', '\n'),
    'h2': ('\n## ', '\n'),
    'h3': ('\n### ', '\n'),
    'h4': ('\n#### ', '\n'),
    'h5': ('\n##### ', '\n'),
    'h6': ('\n###### ', '\n'),
    'code': ('`', '`'),
    'ul': ('', ''),
    'ol': ('', ''),
    'li': ('- ', ''),
    'blockquote': ('\n> ', '\n'),
    'em': ('*', '*'),
    'strong': ('**', '**'),
    'block_code': ('\n```\n', '\n```\n'),
    'span': ('', ''),
    'p': ('\n', '\n'),
    'p_with_out_class': ('\n', '\n'),
    'inline_p': ('', ''),
    'inline_p_with_out_class': ('', ''),
    'b': ('**', '**'),
    'i': ('*', '*'),
    'del': ('~~', '~~'),
    'hr': ('\n---', '\n\n'),
    'thead': ('\n', '|------\n'),
    'tbody': ('\n', '\n'),
    'td': ('|', ''),
    'th': ('|', ''),
    'tr': ('', '\n'),
    'table': ('', '\n'),
    'e_p': ('', '\n')
}

from html.parser import HTMLParser
class MyHTMLParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.markdown=''
        self.tag_stack = []
        self.href=''
    def handle_starttag(self, tag, attrs):
        #print "Encountered the beginning of a %s tag" % tag
        if tag in MARKDOWN:
            self.tag_stack.append(tag)
        attrs_dict={}
        for attr in attrs:
            attrs_dict[attr[0]]=attr[1]

        if 'href' in attrs_dict:
            self.href = attrs_dict['href']

    def handle_endtag(self,tag):
        if tag in MARKDOWN:
            self.tag_stack.pop()

    def handle_data(self,data):
        if len(self.tag_stack)>0:
            tag = self.tag_stack[-1]
            if tag in MARKDOWN:
                t = MARKDOWN[tag]
                data1 = data
                if len(self.href)>0:
                    data1 = '['+data+']('+self.href+')'

                self.markdown +=t[0]+data1+t[1]
                self.href = ''


html_code = t
hp = MyHTMLParser()
hp.feed(html_code)
hp.close()
d = str(hp.markdown)
print(d)

#print(d)
fn2 = './test.md'
f=open(fn2,'wt',encoding='utf8')
f.write(d)
f.close()
