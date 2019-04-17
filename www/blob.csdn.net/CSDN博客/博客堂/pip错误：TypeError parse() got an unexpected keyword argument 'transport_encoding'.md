# pip错误：TypeError: parse() got an unexpected keyword argument 'transport_encoding' - 博客堂 - CSDN博客





2018年01月03日 15:29:53[最小森林](https://me.csdn.net/u012052268)阅读数：925








# 1环境

Win10，ANACONDA3(64-bit)，Python3.6.2。ANACONDA Prompt中不能用pip命令安装包，并且是在安装了TensorFlow后才发生的。 

TypeError: parse() got an unexpected keyword argument ‘transport_encoding’   

# 2错误信息

报错如下：

[html] view plain copy 

Exception: 

Traceback (most recent call last): 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\basecommand.py”, line 215, in main 

    status = self.run(options, args) 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\commands\install.py”, line 335, in run 

    wb.build(autobuilding=True) 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\wheel.py”, line 749, in build 

    self.requirement_set.prepare_files(self.finder) 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\req\req_set.py”, line 380, in prepare_files 

    ignore_dependencies=self.ignore_dependencies)) 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\req\req_set.py”, line 554, in _prepare_file 

    require_hashes 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\req\req_install.py”, line 278, in populate_link 

    self.link = finder.find_requirement(self, upgrade) 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\index.py”, line 465, in find_requirement 

    all_candidates = self.find_all_candidates(req.name) 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\index.py”, line 423, in find_all_candidates 

    for page in self._get_pages(url_locations, project_name): 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\index.py”, line 568, in _get_pages 

    page = self._get_page(location) 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\index.py”, line 683, in _get_page 

    return HTMLPage.get_page(link, session=self.session) 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\index.py”, line 811, in get_page 

    inst = cls(resp.content, resp.url, resp.headers) 

  File “C:\ProgramData\Anaconda3\lib\site-packages\pip\index.py”, line 731, in **init**

    namespaceHTMLElements=False, 

TypeError: parse() got an unexpected keyword argument ‘transport_encoding’   
# 3解决办法

在cmd中输入：
`conda install pip  `
# 4报错原因

pip命令在安装TensorFlow前是可以正常使用的，在安装后才出现的问题，大概原因就是Python的新版本与TensorFlow可能有点不太兼容，这样更新pip与conda命令，就可以fix了。




