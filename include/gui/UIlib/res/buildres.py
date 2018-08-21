
import sys
sys.path.append('../../../../py')
import bin2txt

#print(dir(bin2txt))

# 列出windows目录下的所有文件和文件名
with open("res.hpp", 'w+') as f_output:
	bin2txt.dir2txt("./", ['.bmp', '.ico'], f_output, 16)

