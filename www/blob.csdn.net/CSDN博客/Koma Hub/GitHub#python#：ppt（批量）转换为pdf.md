# GitHub#python#：ppt（批量）转换为pdf - Koma Hub - CSDN博客
2018年04月04日 09:40:15[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：939
1.单个文件转化
网址：[https://github.com/Ryuodan/PPTtoPDF-Converter](https://github.com/Ryuodan/PPTtoPDF-Converter)
支持python3，给出代码：
```python
import win32com.client
import os
in_file=input("Enter the path of the file")
out_file=os.path.splitext(in_file)[0]     # desktop\file.pptx 
powerpoint=win32com.client.Dispatch("Powerpoint.Application")
pdf=powerpoint.Presentations.Open(in_file,WithWindow=False)
pdf.SaveAs(out_file,32)
pdf.Close()
powerpoint.Quit()
```
运行：
```
D:\test>python PPTtoPDF.py
Enter the path of the fileD:\test\pic.pptx
```
过程中会显示：
![](https://img-blog.csdn.net/20180404093904705)
完成后：
![](https://img-blog.csdn.net/20180404093925170)
2.批量文件转化
源码网址：[https://github.com/jackbrookes/batch-ppt-to-pdf/blob/master/batch_ppt_to_pdf.py](https://github.com/jackbrookes/batch-ppt-to-pdf/blob/master/batch_ppt_to_pdf.py)
文章网址：[http://mp.weixin.qq.com/s/lPL_HaL3acflmp-Yz6OSIQ](http://mp.weixin.qq.com/s/lPL_HaL3acflmp-Yz6OSIQ)
依赖的库：comtypes，windows下的安装：
`D:\test>python -m pip install moviepy`
源代码：
```python
import comtypes.client
import os
def init_powerpoint():
    powerpoint = comtypes.client.CreateObject("Powerpoint.Application")
    powerpoint.Visible = 1
    return powerpoint
def ppt_to_pdf(powerpoint, inputFileName, outputFileName, formatType = 32):
    if outputFileName[-3:] != 'pdf':
        outputFileName = outputFileName + ".pdf"
    deck = powerpoint.Presentations.Open(inputFileName)
    deck.SaveAs(outputFileName, formatType) # formatType = 32 for ppt to pdf
    deck.Close()
def convert_files_in_folder(powerpoint, folder):
    files = os.listdir(folder)
    pptfiles = [f for f in files if f.endswith((".pptx", ".pptx"))]
    for pptfile in pptfiles:
        fullpath = os.path.join(cwd, pptfile)
        ppt_to_pdf(powerpoint, fullpath, fullpath)
if __name__ == "__main__":
    powerpoint = init_powerpoint()
    cwd = os.getcwd()
    convert_files_in_folder(powerpoint, cwd)
    powerpoint.Quit()
```
运行：
`D:\test>python main.py`
运行结束后，会将D:\test文件夹里以pptx为结尾的ppt全部转化为pdf格式。
运行前：
![](https://img-blog.csdn.net/2018040410320389)
运行后：
![](https://img-blog.csdn.net/20180404103224686)
3.其他的转化
office to pdf：[https://github.com/mlkyo/office2pdf](https://github.com/mlkyo/office2pdf)
ppt to pdf or picture：[https://github.com/vijayanandrp/PDF-to-PPT](https://github.com/vijayanandrp/PDF-to-PPT)
ppt to pdf：[https://github.com/GijsTimmers/ppt2pdf](https://github.com/GijsTimmers/ppt2pdf)
ppt or pdf to HTML or json：[https://github.com/lucaslukero/PDF-PPT-ODP-to-HTML-JSON-for-Tidys](https://github.com/lucaslukero/PDF-PPT-ODP-to-HTML-JSON-for-Tidys)
pdf to ppt：[https://github.com/uesseu/pdf2pptx](https://github.com/uesseu/pdf2pptx)
