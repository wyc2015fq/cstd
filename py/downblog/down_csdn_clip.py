
from down import *
from htm2md import *
import sys

if __name__ == '__main__':
    t = gettext().decode('gbk')
    root, title, d = htm2md(t)
    save_txt_td(root, title, d)
    ##print(data)