# 机器视觉 OpenCV—python 标注小工具（目标检测） - wsp_1138886114的博客 - CSDN博客





2018年10月15日 17:00:34[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：151








### 一、获取图像并展示

```python
# 获取 picture_dir 下文件的所有图片，并以24帧率显示

import os
from itertools import cycle

filenames = os.listdir("picture_dir")
img_iter = cycle([cv2.imread(os.sep.join(["picture_dir",x])) for x in filenames])
key = 0
while key!=27:
    cv2.imshow('picture_windows',next(img_iter))
    key = cv2.waitKey(42)
```

### 二、标注小工具（代码有问题）

```python
"""
    标注信息格式如下:
    ('file_name',(121,232),(656,675)) # 依次：文件名，左上角坐标，右下角坐标
"""

import os
import cv2
from tkinter.filedialog import askdirectory        # python GUI
from tkinter.messagebox import askyesno

WINDOW_NAME = 'simple Bounding Box Labeling Tool'  # 默认窗口名
FPS = 24                                           # 画面帧率
SUPPOTED_FORMATS = ['jpg','jpeg','png']            # 支持图像格式
DEFAULT_COLOR = {"Object":(255,0,0)}               # 物体框颜色
COLOR_GRAY = (192,192,192)                         # 信息显示背景和未定义物体框显示
BAR_HEIGHT  = 16

# 上下左右，ESC，Delete键的cv2.waitKey() 函数返回值
KEY_UP = 65362
KEY_DOWN = 65364
KEY_LEFT = 65361
KEY_RIGHT = 65363
KEY_ESC = 27
KEY_DELETE = 65535
KEY_EMPTY = 0    # 用于默认循环

get_bbox_name = '{}.bbox'.format

# 定义物体框标注工具类
class SimpleBBoxLabeling:
    def __init__(self,data_dir,fps=FPS,window_name=None):
        self._data_dir=  data_dir
        self.fps = fps
        self.window_name = window_name if window_name else WINDOW_NAME
        self._pt0 = None         # 正在画的左上角坐标
        self._pt1 = None         # 鼠标所在坐标
        self._drawing = False    # 当前画框状态
        self._cur_label = None   # 当前标注物体名称
        self._bboxes = []        # 当前图像对应的所有已标注框

        # 如果有用户自定义的标注信息则读取，否则使用默认物体和颜色
        label_path = '{}.labels'.format(self._data_dir)
        self.label_colors = DEFAULT_COLOR if not os.path.exists(label_path) else self.load_labels(label_path)

        # 获取已标注的文件列表和还未标注的文件列表(统一后缀名为小写)
        imagefiles = [x for x in os.listdir(self._data_dir) if x[x.rfind(".")+1:].lower() in SUPPOTED_FORMATS]
        labeled = [x for x in imagefiles if os.path.exists(get_bbox_name(x))]
        to_be_labeled = [x for x in imagefiles if x not in labeled]

        # 每次打开一个文件夹，自动从未标注的第一张开始
        self._filelist = labeled+to_be_labeled
        self._index = len(labeled)
        if self._index > len(self._filelist)-1:
            self._index = len(self._filelist)-1

    # 鼠标回调函数
    def __mouse_ops(self,event,x,y,flags,param):
        if event == cv2.EVENT_LBUTTONDOWN:   # 按下左键，左上角坐标开始画框，drawing状态为True
            self._drawing = True
            self._pt0 = (x,y)
        elif event == cv2.EVENT_LBUTTONUP:   # 松开左键，标记右下角坐标并保存，drawing状态为False
            self._drawing = False
            self._pt1 = (x,y)
            self._bboxes.append((self._cur_label,self._pt0,self._pt1))  #添加标注信息tuple
        elif event == cv2.EVENT_MOUSEMOVE:  # 实时更新右下角坐标
            self._pt1 = (x,y)
        elif event == cv2.EVENT_RBUTTONUP:  # 右击删除画好的框
            if self._bboxes:
                self._bboxes.pop()

    # 清除所有标注框和当前状态
    def _clean_bbox(self):
        self._pt0 = None
        self._pt1=  None
        self._drawing = False
        self._bboxes = []

	# 定义标注框和当前信息函数:在图像下方多出 BAR_HEIGHT区域用于显示文件名和标注信息
	def _draw_bbox(self,img):
		h,w = img.shape[:2]
		canvas = cv2.copyMakeBorder(img,0,BAR_HEIGHT,0,0,cv2.BORDER_CONSTANT,value=COLOR_GRAY)

		# 正在标注物体的信息，若左键按下，则显示两个点坐标，否则显示当前待标注物体的名称
		label_msg = '{}:{},{}'.format(self._cur_label,self._pt0,self._pt1) if self._drawing else 'Current label:{}'.format(self._cur_label)

		# 显示当前文件名，文件个数信息
		msg = '{}/{},{} | {}'.format(self._index+1,len(self._filelist),self._filelist[self._index],label_msg)
		cv2.putText(canvas,msg,(1,h+12),cv2.FONT_HERSHEY_SIMPLEX,0.5,(0,0,0),1)

		# 画出已经标好的框和对应名字
		for label,(bpt0,bpt1) in self._bboxes:
			label_color = self.label_colors[label] if label in self.label_colors else COLOR_GRAY
			cv2.rectangle(canvas,bpt0,bpt1,label_color,thickness=2)
			cv2.putText(canvas,label,(bpt0[0]+3,bpt1[1]+15),0.5,label_color,2)

		# 画正在标注的框和对应名字
		if self._drawing:
			label_color = self.label_colors[self._cur_label] if self._cur_label in self.label_colors else COLOR_GRAY
			if self._pt1[0] >= self._pt0[0] and self._pt1[1] >= self._pt0[1]:
				cv2.rectangle(canvas,self._pt0,self._pt1,label_color,thickness=2)
			cv2.putText(canvas,self._cur_label,(self._pt0[0]+3,self._pt0[1]+15),0.5,cv2.FONT_HERSHEY_SIMPLEX,0.5,label_color,2)
		return canvas
	
	# 利用repr() 函数导出标注框数据文件
	@staticmethod
	def export_bbox(filepath,bboxes):
		if bboxes:
			with open (filepath,'w') as file:
				for bbox in bboxes:
					line = repr(bbox)+"\n"
					file.write(line)
		elif os.path.exists(filepath):
			os.remove(filepath)

	# 利用eval() 函数读取标注框字符串数据
	@staticmethod
	def load_bbox(filepath):
		bboxes = []
		with open(filepath ,"r") as file:
			line = file.readline().rstrip()
			while line:
				bboxes.append(eval(line))
				line = file.readline().rstrip()
		return bboxes

	# 利用eval() 函数读取物体及颜色信息到数据
	@staticmethod
	def load_labels(filepath):
		label_colors = []
		with open(filepath ,"r") as file:
			line = file.readline().rstrip()
			while line:
				label,color = eval(line)
				label_colors[label] = color
				line = file.readline().rstrip()
		return label_colors

	# 读取图像文件和对应标注框信息（如果有的话）
	@staticmethod
	def load_sample(filepath):
		img = cv2.imread(filepath)
		bbox_filepath = get_bbox_name(filepath)
		bboxes = []
		if os.path.exists(bbox_filepath):
			bboxes = SimpleBBoxLabeling.load_bbox(bbox_filepath)
		return img,bboxes

	# 导出当前标注框信息并清空
	def _export_n_clean_bbox(self):
		bbox_filepath = os.sep.join([self._data_dir,get_bbox_name(self._filelist[self._index])])
		self.export_bbox(bbox_filepath,self._bboxes)
		self._clean_bbox()
	# 删除当前样本和对应的标注信息
	def __delete_current_sample(self):
		filename = self._filelist[self._index]
		filepath = os.sep.join([self._data_dir,filename])
		if os.path.exists(filepath):
			os.remove(filepath)
		filepath = get_bbox_name(filepath)
		if os.path.exists(filepath):
			os.remove(filepath)
		self._filelist.pop(self._index)
		print("{} is deleted!".format(filename))

	# OpenCV 循环窗口：主程序
	def start(self):
		last_filename = ''                   # 最近标注文件名，判断是否执行图像读取
		label_index = 0                      # 标注物体索引
		labels = self.label_colors.keys()    # 所有标注物体名称的列表
		n_labels = len(labels)               # 待标注物体种类数

		# 定义窗口和鼠标回调
		cv2.namedWindow(self.window_name)
		cv2.setMouseCallback(self.window_name,self.__mouse_ops)
		key = KEY_EMPTY                      # 默认0
		delay = int(1000/FPS)                # 每次循环的持续时间
		while key!=KEY_ESC:                  # 没按下ESC，则持续循环
			# 上下方向键用于选择当前标注物体
			if key == KEY_UP:
				if label_index == 0:
					pass
				else:
					label_index -=1
			elif key ==KEY_DOWN:
				if label_index == n_labels-1:
					pass
				else:
					label_index += 1
			# 左右方向键用于选择当前标注物体:
			elif key == KEY_LEFT:
				if self._index > 0:          # 首张图片无需清空上一张
					self._export_n_clean_bbox()
				self._index -=1
				if self._index < 0:
					self._index = 0
			elif key ==KEY_RIGHT:            # 末张图片无需清空上一张
				if self._index < len(self._filelist)-1:
					self._export_n_clean_bbox()
				self._index +=1
				if self._index > len(self._filelist) - 1:
					self._index = len(self._filelist) -1
			# 删除当前图片和对应的标注信息
			elif key == KEY_DELETE:
				if askyesno("Delete Sample","Are you Sure?"):
					self.__delete_current_sample()
					key = KEY_EMPTY
					continue

			# 如果键盘操作执行了换图片，则重新读取，更新图片
			filename = self._filelist[self._index]
			if filename != last_filename:
				filepath = os.sep.join([self._data_dir,filename])
				img,self._bboxes = self.load_sample(filepath)

			# 更新当前标注物体名称
			self._cur_label = labels[label_index]

			# 把标注和相关信息画在图片上并显示指定时间
			canvas = self._draw_bbox(img)
			cv2.imshow(self.window_name,canvas)
			key = cv2.waitKey(delay)

			# 当前文件名就是下次循环的老文件名
			last_filename = filename
		print("finished!")
		cv2.destroyAllWindows()

		# 如果推出程序，需要对当前文件进行保存
		self.export_bbox(os.sep.join([self._data_dir,get_bbox_name(filename)]),self._bboxes)
		print("labels updatad!")
if __name__ == "__main__":
	dir_with_images = askdirectory(title="where are the images?")
	labeling_task = SimpleBBoxLabeling(dir_with_images)
	labeling_task.start()
```

参考：
[https://blog.csdn.net/hax001/article/details/78315294](https://blog.csdn.net/hax001/article/details/78315294)



