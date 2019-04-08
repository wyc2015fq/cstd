import tomd
from tomd import Tomd


fn = '../test.txt'
f=open(fn,'rt',encoding='utf8')
t=f.read()
f.close()

#print(Tomd(t).markdown)
d = tomd.convert(t)
#print(d)
fn2 = './test.md'
f=open(fn2,'wt',encoding='utf8')
f.write(d)
f.close()
