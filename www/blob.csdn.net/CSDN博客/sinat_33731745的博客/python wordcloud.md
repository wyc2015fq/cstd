
# python wordcloud - sinat_33731745的博客 - CSDN博客

2017年10月26日 22:01:17[Tao-Tao-Tao](https://me.csdn.net/sinat_33731745)阅读数：150



# wordcloud.WordCloud


```python
class wordcloud.WordCloud(font_path=None, width=400, height=200, margin=2, ranks_only=None, prefer_horizontal=0.9, mask=None, scale=1, color_func=None, max_words=200, min_font_size=4, stopwords=None, random_state=None, background_color='black', max_font_size=None, font_step=1, mode='RGB', relative_scaling=0.5, regexp=None, collocations=True, colormap=None, normalize_plurals=True)
```
参数说明：
font_path : string  文字字体，中文一定要设置。Font path to the font that will be used (OTF or TTF). Defaults to DroidSansMono path on a Linux machine. If you are on another OS or don’t have this font, you need to adjust this path.
# width : int (default=400)Width of the canvas.

# height : int (default=200)Height of the canvas.

# prefer_horizontal : float (default=0.90) 词语水平显示数量的比例。The ratio of times to try horizontal fitting as opposed to vertical. If prefer_horizontal < 1, the algorithm will try rotating the word if it doesn’t fit. (There is currently no built-in way to get only vertical
 words.)

# mask : nd-array or None (default=None) 词云图的形状，可以用系统的绘图工具画一个即可，可保存为tif格式。If not None, gives a binary mask on where to draw words. If mask is not None, width and height will be ignored and the shape of mask will be used instead. All white (\#FF or \#FFFFFF) entries
 will be considerd “masked out” while other entries will be free to draw on. [This changed in the most recent version!]

# scale : float (default=1)计算与绘制图像间的比例。对于较大的词云图像，使用比例而非较大的画布会显著提升绘图速度，但是可能会造成词语间的粗糙拟合。Scaling between computation and drawing. For large word-cloud images, using scale instead of larger canvas size is significantly faster, but might lead to a coarser fit for
 the words.

# min_font_size : int (default=4) 最小的字号。Smallest font size to use. Will stop when there is no more room in this size.

# font_step : int (default=1)Step size for the font. font_step > 1 might speed up computation but give a worse fit.

# max_words : number (default=200)  显示的最大词语数量The maximum number of words.

# stopwords : set of strings or None 过滤掉不显示的词语The words that will be eliminated. If None, the build-in STOPWORDS list will be used.

# background_color : color value (default=”black”) 背景颜色Background color for the word cloud image.

# max_font_size : int or None (default=None)  最大的字号Maximum font size for the largest word. If None, height of the image is used.

# mode : string (default=”RGB”)Transparent background will be generated when mode is “RGBA” and background_color is None.

# relative_scaling : float (default=.5)Importance of relative word frequencies for font-size. With relative_scaling=0, only word-ranks are considered. With relative_scaling=1, a word that is twice as frequent will have twice the size. If you want to consider
 the word frequencies and not only their rank, relative_scaling around .5 often looks good.

# color_func : callable, default=None。Callable with parameters word, font_size, position, orientation, font_path, random_state that returns a PIL color for each word. Overwrites “colormap”. See colormap for specifying a matplotlib colormap instead.

# regexp : string or None (optional)。Regular expression to split the input text into tokens in process_text. If None is specified, r"\w[\w']+" is used.

# collocations : bool, default=True。Whether to include collocations (bigrams) of two words.

# colormap : string or matplotlib colormap, default=”viridis”。Matplotlib colormap to randomly draw colors from for each word. Ignored if “color_func” is specified.

# normalize_plurals : bool, default=True。Whether to remove trailing ‘s’ from words. If True and a word appears with and without a trailing ‘s’, the one with trailing ‘s’ is removed and its counts are added to the version without trailing ‘s’ – unless the word
 ends with ‘ss’.


# Notes

# Larger canvases with make the code significantly slower. If you need a large word cloud, try a lower canvas size, and set the scale parameter.

# The algorithm might give more weight to the ranking of the words than their actual frequencies, depending on the max_font_size and the scaling heuristic.


# Attributes

# 1. words_
# (dict of string to float) Word tokens with associated frequency. .. versionchanged: 2.0 words_ is now a dictionary

# 2. layout_
# (list of tuples (string, int, (int, int), int, color))) Encodes the fitted word cloud. Encodes for each word the string, font size, position, orientation and color.

# Methods


# __init__(font_path=None, width=400, height=200, margin=2, ranks_only=None, prefer_horizontal=0.9, mask=None, scale=1, color_func=None, max_words=200, min_font_size=4, stopwords=None, random_state=None, background_color='black',
 max_font_size=None, font_step=1, mode='RGB', relative_scaling=0.5, regexp=None, collocations=True, colormap=None, normalize_plurals=True)[source]

# 1. fit_words(frequencies)   Create a word_cloud from words and frequencies.

# Alias to generate_from_frequencies.

# Parameters:
# frequencies : array of tuples
# A tuple contains the word and its frequency.
# Returns:
# self


# 2.  generate(text)
# Generate wordcloud from text.

# Alias to generate_from_text.

# Calls process_text and generate_from_frequencies.

# Returns:
# self

# 3. generate_from_frequencies(frequencies, max_font_size=None)
# Create a word_cloud from words and frequencies.

# Parameters:
# frequencies : dict from string to float
# A contains words and associated frequency.
# max_font_size : int
# Use this font-size instead of self.max_font_size
# Returns:
# self
# 4. generate_from_text(text)
# Generate wordcloud from text.

# Calls process_text and generate_from_frequencies.

# ..versionchanged:: 1.2.2
# Argument of generate_from_frequencies() is not return of process_text() any more.
# Returns:
# self

# 5. process_text(text)
# Splits a long text into words, eliminates the stopwords.

# Parameters:
# text : string
# The text to be processed.
# Returns:
# words : dict (string, int)
# Word tokens with associated frequency.
# ..versionchanged:: 1.2.2
# Changed return type from list of tuples to dict.
# Notes

# There are better ways to do word tokenization, but I don’t want to include all those things.

# 6.  recolor(random_state=None, color_func=None, colormap=None)
# Recolor existing layout.

# Applying a new coloring is much faster than generating the whole wordcloud.

# Parameters:
# random_state : RandomState, int, or None, default=None
# If not None, a fixed random state is used. If an int is given, this is used as seed for a random.Random state.
# color_func : function or None, default=None
# Function to generate new color from word count, font size, position and orientation. If None, self.color_func is used.
# colormap : string or matplotlib colormap, default=None
# Use this colormap to generate new colors. Ignored if color_func is specified. If None, self.color_func (or self.color_map) is used.
# Returns:
# self

# 7. to_array()
# Convert to numpy array.

# Returns:
# image : nd-array size (width, height, 3)
# Word cloud image as numpy matrix.

# 8. to_file(filename)
# Export to image file.

# Parameters:
# filename : string
# Location to write to.
# Returns:
# self

# ——————————————————————————————————————————————

# 自定义每个词语显示的颜色

```python
import numpy as np
from PIL import Image
from os import path
import matplotlib.pyplot as plt
import random

from wordcloud import WordCloud, STOPWORDS


def grey_color_func(word, font_size, position, orientation, random_state=None,
                    **kwargs):
    return "hsl(0, 0%%, %d%%)" % random.randint(60, 100)

d = path.dirname(__file__)

# read the mask image
# taken from
# http://www.stencilry.org/stencils/movies/star%20wars/storm-trooper.gif
mask = np.array(Image.open(path.join(d, "stormtrooper_mask.png")))

# movie script of "a new hope"
# http://www.imsdb.com/scripts/Star-Wars-A-New-Hope.html
# May the lawyers deem this fair use.
text = open("a_new_hope.txt").read()

# preprocessing the text a little bit
text = text.replace("HAN", "Han")
text = text.replace("LUKE'S", "Luke")

# adding movie script specific stopwords
stopwords = set(STOPWORDS)
stopwords.add("int")
stopwords.add("ext")

wc = WordCloud(max_words=1000, mask=mask, stopwords=stopwords, margin=10,
               random_state=1).generate(text)
# store default colored image
default_colors = wc.to_array()
plt.title("Custom colors")
plt.imshow(wc.recolor(color_func=grey_color_func, random_state=3),
           interpolation="bilinear")
wc.to_file("a_new_hope.png")
plt.axis("off")
plt.figure()
plt.title("Default colors")
plt.imshow(default_colors, interpolation="bilinear")
plt.axis("off")
plt.show()
```





