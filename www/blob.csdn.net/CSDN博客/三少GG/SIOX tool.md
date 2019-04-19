# SIOX tool - 三少GG - CSDN博客
2013年06月04日 14:40:50[三少GG](https://me.csdn.net/scut1135)阅读数：1067

[**http://www.siox.org/**](http://www.siox.org/)
[A fair look at the SIOX tool in Inkscape 0.46 and GIMP 2.4](http://howto.nicubunu.ro/siox_inkscape_gimp/)
A couple of years ago when the [SIOX](http://www.siox.org/) (Simple Interactive Object Extraction) surfaced the web, a lot of enthusiasm and expectations were created. Now with released or almost released applications using it, we can have an
 objective look at it and draw practical conclusions, not only talk about the demos which may be (at least partly) doctored.
![](http://nicubunu.ro/pictures/inkscape.png)With fast release cycles (about two releases a year)[Inkscape](http://inkscape.org/) was the first to have a working SIOX implementation in a release,
 back in**Inkscape 0.44**.
Inkscape is a **vector drawing** application, so it had to implement the algorithm in its specific way, namely as a part of the**tracing** tool, which convert from raster to vector, so the result is not pixel-perfect, but it does not
 even tries that.
Another downside is that Inkscape will use only the starting photo and one single path defining the region of interest (without a mark for the sure foreground), so the result is far from perfect and the tool is not really interactive.
The receipt is simple: import the photo, draw a freehand shape covering it, select both and run a trace by colors:
![screenshot](http://nicubunu.ro/pictures/siox_inkscape.png)
![](http://nicubunu.ro/pictures/gimp.png)[GIMP](http://gimp.org/) got the tool even earlier in its**development branch**, but only the soon-to-be-released stable
**GIMP 2.4** will put it in the hands of the larger audience.
Here are a lot of options: brush sizes, feather edges, smoothing and you can mark both the sure background and the sure foreground, but after the SIOX job is done, it still needed to adjust a little using classic selection tools.
The receipt is more complex: open the photo and use the foreground extraction tool, it will offer a lasso-like tool to mark the region of interest around the subject and then a brush to sample the foreground. You can change the brush size and/or zoom in and
 out and even an eraser to unselect foreground. When ready, press **Enter** to get the subject selected, but you still have to use the real lasso selection to adjust it more:
![screenshot](http://nicubunu.ro/pictures/siox_gimp.png)
I think I have some [solid experience](http://howto.nicubunu.ro/) with both application, so my**conclusion** is obviously biased: SIOX may look like a timesaver for some operations, but in the end you will still need a loot of additional
 work to improve its results, you will not save that much time, so for me it is mostly a nice gimmick.
As particular applications, Inkscape is really lacking a way to define the sure foreground so it will give satisfactory results only for selected images but with GIMP you can zoom in and use smaller brushes (like in the good old**Quick Mask** way)
 and select anything you want. 
Maybe if you want vectorization for complex images is better to extract the foreground in GIMP and trace in Inkscape the already extracted foreground.
### 2.8. Foreground Select
**Figure 14.30. The“Foreground Select”tool in the Toolbox**
![The Foreground Select tool in the Toolbox](http://docs.gimp.org/2.8/en/images/toolbox/FG-select-toolbox.png)
This tool lets you extract the foreground from the active layer or from a selection. It is based on the SIOX method (Simple Interactive Object Extraction). You can visit its Web page at[[SIOX]](http://docs.gimp.org/2.8/en/bibliography.html#bibliography-online-siox).
#### 2.8.1. Directions for use
The creation of a selection with this tool works in a couple of steps:
- 
*Roughly select the foreground*you want to extract. When you select this tool, the mouse pointer goes with the lasso icon. It actually
 works like the Fuzzy Select tool. Select as little as possible from the background.
As soon as you release the mouse button, the non selected part of the image is covered with a dark blue mask. If the selection is not closed, its ends will be linked automatically
 together by a straight line. The mouse pointer goes now with the Paint-brush icon for the next step.
**Figure 14.31. The foreground is roughly selected**
![The foreground is roughly selected](http://docs.gimp.org/2.8/en/images/toolbox/FG-select-ex1.png)
- 
*Draw a line through the foreground*: using the paintbrush, whose size can be changed in options, draw a continuous line in the selected foreground
 going over colors which will be kept for the extraction. The color used to draw the line is of no importance; not using the same color as foreground is better. Be careful not painting background pixels.
**Figure 14.32. The line drawn on the foreground**
![The line drawn on the foreground](http://docs.gimp.org/2.8/en/images/toolbox/FG-select-ex2.png)
In this example, it is important that the line goes over the yellow capitulum of the flower.
- 
When you release the mouse button, all non-selected areas are in dark:
**Figure 14.33. The area which will be selected**
![The area which will be selected](http://docs.gimp.org/2.8/en/images/toolbox/FG-select-ex3.png)
- 
You still have to press the**Enter**key
 to get the wanted selection:
**Figure 14.34. Foreground is selected**
![Foreground is selected](http://docs.gimp.org/2.8/en/images/toolbox/FG-select-ex4.png)
Until you press**Enter**,
 you can't undo this selection by**Ctrl**+**Z**nor
 bySelect→None,
 and the Undo History is not concerned. To delete this selection, you must select another tool.
#### 2.8.2. Activating the Tool
You can activate the Foreground Select tool in two ways:
- 
by clicking on the tool icon![](http://docs.gimp.org/2.8/en/images/toolbox/stock-tool-foreground-select-22.png)in
 the Toolbox,
- 
throughTools→Selection
 Tools→Foreground
 Selectin the image menu.
- 
This tool has no shortcut, but you can set one usingEdit→Preferences→Interface→Configure
 Keyboard Shortcuts→Tools→Foreground
 Select
#### 2.8.3. Key modifiers (Defaults)
- **Ctrl**
By pressing the**Ctrl**key,
 you can switching between foreground and background selection painting.
#### 2.8.4. Options
**Figure 14.35. “Foreground Select”tool options**
![Foreground Select tool options](http://docs.gimp.org/2.8/en/images/toolbox/FG-select-options.png)
Normally, tool options are displayed in a window attached under the Toolbox as soon as you activate a tool. If they are not, you can access them from the image menu bar throughWindows→Dockable
 Windows→Tool
 Optionswhich opens the option window of the selected tool.
- Mode; Antialiasing; Feather edges
See[Selection
 Tools](http://docs.gimp.org/2.8/en/gimp-tools-selection.html#gimp-tool-select)for help with options that are common to all these tools. Only options that are specific to this tool are explained here.
- Contiguous
If this option is enabled, only the area contiguous to the stroke will be selected. Otherwise all the areas with same colors will be selected.
**Figure 14.36. “Contiguous”option effect**
![Contiguous option effect](http://docs.gimp.org/2.8/en/images/toolbox/FG-select-contig1.png)
Two separated areas with the same color. On the left, only the left area is marked.
![Contiguous option effect](http://docs.gimp.org/2.8/en/images/toolbox/FG-select-contig2.png)
The Contiguous option is checked: only the area close to the painted line is selected.
![Contiguous option effect](http://docs.gimp.org/2.8/en/images/toolbox/FG-select-contig3.png)
The Contiguous option is not checked: both areas, although they are separated, are selected.
- Interactive refinement
Here are some options to work more precisely on your selection:
- Mark foreground
default option. The foreground color of the Toolbox is used to paint. Colors covered by the painted line will be used for extraction.
- Mark background
You can access this option either by clicking on the radio button or, more simply, by pressing the**Ctrl**key.
 The mouse pointer goes with a small eraser icon. The used color is the background color of Toolbox. The pixels of the selection which have the same color as the“erased”pixels will NOT be extracted.
- Small brush / Large brush
This slider lets you adapt the size of the brush used to paint the line. A small brush fits well thin details.
- Smoothing
Smaller values give a more accurate selection border but may introduce holes in the selection.
- Preview color
You can select between Red, Green and Blue to mask the image background.
- Color Sensitivity
This option uses the[L*a*b](http://docs.gimp.org/2.8/en/glossary.html#glossary-lab)color
 model. If your image contains many pixels of the same color in different tones, you can increase the sensibility of the selection for this color.
