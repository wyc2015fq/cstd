# 图像处理之简单脸谱检测算法（Simple Face Detection Algorithm） - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年05月23日 23:09:53[gloomyfish](https://me.csdn.net/jia20003)阅读数：7853
所属专栏：[Java数字图像处理与特效](https://blog.csdn.net/column/details/image-process.html)









**图像处理之简单脸谱检测算法（Simple Face Detection Algorithm）**




**介绍基于皮肤检测之后的，寻找最大连通区域，完成脸谱检测的算法。大致的算法步骤如下：**

![](https://img-my.csdn.net/uploads/201205/23/1337785219_9119.png)




**原图如下：**

![](https://img-my.csdn.net/uploads/201205/23/1337785247_2176.png)




**每步处理以后的效果：**

![](https://img-my.csdn.net/uploads/201205/24/1337789621_8463.png)


程序运行，加载选择图像以后的截屏如下：

![](https://img-my.csdn.net/uploads/201205/23/1337786291_3850.png)


截屏中显示图片，是适当放缩以后，代码如下：



```java
Image scaledImage = rawImg.getScaledInstance(200, 200, Image.SCALE_FAST); // Java Image API， rawImage is source image
	g2.drawImage(scaledImage, 0, 0, 200, 200, null);
```



第一步：图像预处理，预处理的目的是为了减少图像中干扰像素，使得皮肤检测步骤可以得

到更好的效果，最常见的手段是调节对比度与亮度，也可以高斯模糊。关于怎么调节亮度与

对比度可以参见这里：[http://blog.csdn.net/jia20003/article/details/7385160](http://blog.csdn.net/jia20003/article/details/7385160)

这里调节对比度的算法很简单，源代码如下：



```java
package com.gloomyfish.face.detection;

import java.awt.image.BufferedImage;

public class ContrastFilter extends AbstractBufferedImageOp {
	
	private double nContrast = 30;
	
	public ContrastFilter() {
		System.out.println("Contrast Filter");
	}

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();
        double contrast = (100.0 + nContrast) / 100.0;
        contrast *= contrast;
        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );

        int[] inPixels = new int[width*height];
        int[] outPixels = new int[width*height];
        getRGB( src, 0, 0, width, height, inPixels );
        int index = 0;
        int ta = 0, tr = 0, tg = 0, tb = 0;
        for(int row=0; row<height; row++) {
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		ta = (inPixels[index] >> 24) & 0xff;
                tr = (inPixels[index] >> 16) & 0xff;
                tg = (inPixels[index] >> 8) & 0xff;
                tb = inPixels[index] & 0xff;
                
                // adjust contrast - red, green, blue
                tr = adjustContrast(tr, contrast);
                tg = adjustContrast(tg, contrast);
                tb = adjustContrast(tb, contrast);
                
                // output RGB pixel
                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
        	}
        }
        setRGB( dest, 0, 0, width, height, outPixels );
        return dest;
	}
	
	public int adjustContrast(int color, double contrast) {
		double result = 0;
        result = color / 255.0;
        result -= 0.5;
        result *= contrast;
        result += 0.5;
        result *=255.0;
        return clamp((int)result);
	}
	
	public static int clamp(int c) {
		if (c < 0)
			return 0;
		if (c > 255)
			return 255;
		return c;
	}

}
```



**注意：第一步不是必须的，如果图像质量已经很好，可以直接跳过。**

****

第二步：皮肤检测，采用的是基于RGB色彩空间的统计结果来判断一个像素是否为skin像

素，如果是皮肤像素，则设置像素为黑色，否则为白色。给出基于RGB色彩空间的五种皮

肤检测统计方法，最喜欢的一种源代码如下：



```java
package com.gloomyfish.face.detection;

import java.awt.image.BufferedImage;
/**
 * this skin detection is absolutely good skin classification,
 * i love this one very much
 * 
 * this one should be always primary skin detection 
 * from all five filters
 * 
 * @author zhigang
 *
 */
public class SkinFilter4 extends AbstractBufferedImageOp {

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		int width = src.getWidth();
        int height = src.getHeight();

        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );

        int[] inPixels = new int[width*height];
        int[] outPixels = new int[width*height];
        getRGB( src, 0, 0, width, height, inPixels );
        int index = 0;
        for(int row=0; row<height; row++) {
        	int ta = 0, tr = 0, tg = 0, tb = 0;
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		ta = (inPixels[index] >> 24) & 0xff;
                tr = (inPixels[index] >> 16) & 0xff;
                tg = (inPixels[index] >> 8) & 0xff;
                tb = inPixels[index] & 0xff;
                
                // detect skin method...
                double sum = tr + tg + tb;
                if (((double)tb/(double)tg<1.249) &&
	                ((double)sum/(double)(3*tr)>0.696) &&
	                (0.3333-(double)tb/(double)sum>0.014) &&
	                ((double)tg/(double)(3*sum)<0.108))
                {
                	tr = tg = tb = 0;
                } else {
                	tr = tg = tb = 255;
                }
                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
        	}
        }
        setRGB(dest, 0, 0, width, height, outPixels);
        return dest;
	}
}
```

**第三步：寻找最大连通区域**


使用连通组件标记算法，寻找最大连通区域，关于什么是连通组件标记算法，可以参见这里

[http://blog.csdn.net/jia20003/article/details/7483249](http://blog.csdn.net/jia20003/article/details/7483249)，里面提到的连通组件算法效率不高，所

以这里我完成了一个更具效率的版本，主要思想是对像素数据进行八邻域寻找连通，然后合

并标记。源代码如下：



```java
package com.gloomyfish.face.detection;

import java.util.Arrays;
import java.util.HashMap;

/**
 * fast connected component label algorithm
 * 
 * @date 2012-05-23
 * @author zhigang
 *
 */
public class FastConnectedComponentLabelAlg {
	private int bgColor;
	private int[] labels;
	private int[] outData;
	private int dw;
	private int dh;
	
	public FastConnectedComponentLabelAlg() {
		bgColor = 255; // black color
	}

	public int[] doLabel(int[] inPixels, int width, int height) {
		dw = width;
		dh = height;
		int nextlabel = 1;
		int result = 0;
		labels = new int[dw * dh/2];
		outData = new int[dw * dh];
		for(int i=0; i<labels.length; i++) {
			labels[i] = i;
		}
		
		// we need to define these two variable arrays.
		int[] fourNeighborhoodPixels = new int[8];
		int[] fourNeighborhoodLabels = new int[8];
		int[] knownLabels = new int[4];
		
		int srcrgb = 0, index = 0;
		boolean existedLabel = false;
		for(int row = 0; row < height; row ++) {
			for(int col = 0; col < width; col++) {
				index = row * width + col;
				srcrgb = inPixels[index] & 0x000000ff;
				if(srcrgb == bgColor) {
					result = 0; // which means no labeled for this pixel.
				} else {
					// we just find the eight neighborhood pixels.
					fourNeighborhoodPixels[0] = getPixel(inPixels, row-1, col); // upper cell
					fourNeighborhoodPixels[1] = getPixel(inPixels, row, col-1); // left cell
					fourNeighborhoodPixels[2] = getPixel(inPixels, row+1, col); // bottom cell
					fourNeighborhoodPixels[3] = getPixel(inPixels, row, col+1); // right cell
					
					// four corners pixels
					fourNeighborhoodPixels[4] = getPixel(inPixels, row-1, col-1); // upper left corner
					fourNeighborhoodPixels[5] = getPixel(inPixels, row-1, col+1); // upper right corner
					fourNeighborhoodPixels[6] = getPixel(inPixels, row+1, col-1); // left bottom corner
					fourNeighborhoodPixels[7] = getPixel(inPixels, row+1, col+1); // right bottom corner
					
					// get current possible existed labels
					fourNeighborhoodLabels[0] = getLabel(outData, row-1, col); // upper cell
					fourNeighborhoodLabels[1] = getLabel(outData, row, col-1); // left cell
					fourNeighborhoodLabels[2] = getLabel(outData, row+1, col); // bottom cell
					fourNeighborhoodLabels[3] = getLabel(outData, row, col+1); // right cell
					
					// four corners labels value
					fourNeighborhoodLabels[4] = getLabel(outData, row-1, col-1); // upper left corner
					fourNeighborhoodLabels[5] = getLabel(outData, row-1, col+1); // upper right corner
					fourNeighborhoodLabels[6] = getLabel(outData, row+1, col-1); // left bottom corner
					fourNeighborhoodLabels[7] = getLabel(outData, row+1, col+1); // right bottom corner
					
					knownLabels[0] = fourNeighborhoodLabels[0];
					knownLabels[1] = fourNeighborhoodLabels[1];
					knownLabels[2] = fourNeighborhoodLabels[4];
					knownLabels[3] = fourNeighborhoodLabels[5];
					
					existedLabel = false;
					for(int k=0; k<fourNeighborhoodLabels.length; k++) {
						if(fourNeighborhoodLabels[k] != 0) {
							existedLabel = true;
							break;
						}
					}
					
					if(!existedLabel) {
						result = nextlabel;
						nextlabel++;
					} else {
						int found = -1, count = 0;
						for(int i=0; i<fourNeighborhoodPixels.length; i++) {
							if(fourNeighborhoodPixels[i] != bgColor) {
								found = i;
								count++;
							}
						}
						
						if(count == 1) {
							result = (fourNeighborhoodLabels[found] == 0) ? nextlabel : fourNeighborhoodLabels[found];
						} else {
							result = (fourNeighborhoodLabels[found] == 0) ? nextlabel : fourNeighborhoodLabels[found];
							for(int j=0; j<knownLabels.length; j++) {
								if(knownLabels[j] != 0 && knownLabels[j] != result &&
										knownLabels[j] < result) {
									result = knownLabels[j];
								}
							}
							
							boolean needMerge = false;
							for(int mm = 0; mm < knownLabels.length; mm++ ) {
								if(knownLabels[0] != knownLabels[mm] && knownLabels[mm] != 0) {
									needMerge = true;
								}
							}
							
							// merge the labels now....
							if(needMerge) {
								int minLabel = knownLabels[0];
								for(int m=0; m<knownLabels.length; m++) {
									if(minLabel > knownLabels[m] && knownLabels[m] != 0) {
										minLabel = knownLabels[m];
									}
								}
								
								// find the final label number...
								result = (minLabel == 0) ? result : minLabel;
										
								// re-assign the label number now...
								if(knownLabels[0] != 0) {
									setData(outData, row-1, col, result);
								}
								if(knownLabels[1] != 0) {
									setData(outData, row, col-1, result);
								}
								if(knownLabels[2] != 0) {
									setData(outData, row-1, col-1, result);
								}
								if(knownLabels[3] != 0) {
									setData(outData, row-1, col+1, result);
								}
								
							}
						}
					}
				}
				outData[index] = result; // assign to label
			}
		}
		
		// post merge each labels now
		for(int row = 0; row < height; row ++) {
			for(int col = 0; col < width; col++) {
				index = row * width + col;
				mergeLabels(index);
			}
		}
		
		// labels statistic
		HashMap<Integer, Integer> labelMap = new HashMap<Integer, Integer>();
		for(int d=0; d<outData.length; d++) {
			if(outData[d] != 0) {
				if(labelMap.containsKey(outData[d])) {
					Integer count = labelMap.get(outData[d]);
					count+=1;
					labelMap.put(outData[d], count);
				} else {
					labelMap.put(outData[d], 1);
				}
			}
		}
		
		// try to find the max connected component
		Integer[] keys = labelMap.keySet().toArray(new Integer[0]);
		Arrays.sort(keys);
		int maxKey = 1;
		int max = 0;
		for(Integer key : keys) {
			if(max < labelMap.get(key)){
				max = labelMap.get(key);
				maxKey = key;
			}
			System.out.println( "Number of " + key + " = " + labelMap.get(key));
		}
		System.out.println("maxkey = " + maxKey);
		System.out.println("max connected component number = " + max);
		return outData;
	}

	private void mergeLabels(int index) {
		int row = index / dw;
		int col = index % dw;
		
		// get current possible existed labels
		int min = getLabel(outData, row, col);
		if(min == 0) return;
		if(min > getLabel(outData, row-1, col) && getLabel(outData, row-1, col) != 0) {
			min = getLabel(outData, row-1, col);
		}
		
		if(min > getLabel(outData, row, col-1) && getLabel(outData, row, col-1) != 0) {
			min = getLabel(outData, row, col-1);
		}
		
		if(min > getLabel(outData, row+1, col) && getLabel(outData, row+1, col) != 0) {
			min = getLabel(outData, row+1, col);
		}
		
		if(min > getLabel(outData, row, col+1) && getLabel(outData, row, col+1) != 0) {
			min = getLabel(outData, row, col+1);
		}
		
		if(min > getLabel(outData, row-1, col-1) && getLabel(outData, row-1, col-1) != 0) {
			min = getLabel(outData, row-1, col-1);
		}
		
		if(min > getLabel(outData, row-1, col+1) && getLabel(outData, row-1, col+1) != 0) {
			min = getLabel(outData, row-1, col+1);
		}
		
		if(min > getLabel(outData, row+1, col-1) && getLabel(outData, row+1, col-1) != 0) {
			min = getLabel(outData, row+1, col-1);
		}
		
		if(min > getLabel(outData, row+1, col+1) && getLabel(outData, row+1, col+1) != 0) {
			min = getLabel(outData, row+1, col+1);
		}

		if(getLabel(outData, row, col) == min)
			return;
		outData[index] = min;
		
		// eight neighborhood pixels
		if((row -1) >= 0) {
			
			mergeLabels((row-1)*dw + col);
		}
		
		if((col-1) >= 0) {
			mergeLabels(row*dw+col-1);
		}
		
		if((row+1) < dh) {
			mergeLabels((row + 1)*dw+col);
		}
		
		if((col+1) < dw) {
			mergeLabels((row)*dw+col+1);
		}
		
		if((row-1)>= 0 && (col-1) >=0) {
			mergeLabels((row-1)*dw+col-1);
		}
		
		if((row-1)>= 0 && (col+1) < dw) {
			mergeLabels((row-1)*dw+col+1);
		}
		
		if((row+1) < dh && (col-1) >=0) {
			mergeLabels((row+1)*dw+col-1);
		}
		
		if((row+1) < dh && (col+1) < dw) {
			mergeLabels((row+1)*dw+col+1);
		}
	}
	
	private void setData(int[] data, int row, int col, int value) {
		if(row < 0 || row >= dh) {
			return;
		}
		
		if(col < 0 || col >= dw) {
			return;
		}
		
		int index = row * dw + col;
		data[index] = value;
	}
	
	private int getLabel(int[] data, int row, int col) {
		// handle the edge pixels
		if(row < 0 || row >= dh) {
			return 0;
		}
		
		if(col < 0 || col >= dw) {
			return 0;
		}
		
		int index = row * dw + col;
		return (data[index] & 0x000000ff);
	}

	private int getPixel(int[] data, int row, int col) {
		// handle the edge pixels
		if(row < 0 || row >= dh) {
			return bgColor;
		}
		
		if(col < 0 || col >= dw) {
			return bgColor;
		}
		
		int index = row * dw + col;
		return (data[index] & 0x000000ff);
	}

	/**
	 * binary image data:
	 * 
	 * 255, 0,   0,   255,   0,   255, 255, 0,   255, 255, 255,
	 * 255, 0,   0,   255,   0,   255, 255, 0,   0,   255, 0,
	 * 255, 0,   0,   0,     255, 255, 255, 255, 255, 0,   0,
	 * 255, 255, 0,   255,   255, 255, 0,   255, 0,   0,   255
	 * 255, 255, 0,   0,     0,   0,   255, 0,   0,   0,   0
	 * 
	 * height = 5, width = 11
	 * @param args
	 */
	public static int[] imageData = new int[]{
		 255, 0,   0,   255,   0,   255, 255, 0,   255, 255, 255,
		 255, 0,   0,   255,   0,   255, 255, 0,   0,   255, 0,
		 255, 0,   0,   0,     255, 255, 255, 255, 255, 0,   0,
		 255, 255, 0,   255,   255, 255, 0,   255, 0,   0,   255,
		 255, 255, 0,   0,     0,   0,   255, 0,   0,   0,   0
	};
	
	public static void main(String[] args) {
		FastConnectedComponentLabelAlg ccl = new FastConnectedComponentLabelAlg();
		int[] outData = ccl.doLabel(imageData, 11, 5);
		for(int i=0; i<5; i++) {
			System.out.println("--------------------");
			for(int j = 0; j<11; j++) {
				int index = i * 11 + j;
				if(j != 0) {
					System.out.print(",");
				}
				System.out.print(outData[index]);
			}
			System.out.println();
		}
	}

}
```


找到最大连通区域以后，对最大连通区域数据进行扫描，找出最小点，即矩形区域左上角坐

标，找出最大点，即矩形区域右下角坐标。知道这四个点坐标以后，在原图上打上红色矩形

框，标记出脸谱位置。寻找四个点坐标的实现代码如下：



```java
private void getFaceRectangel() {
		int width = resultImage.getWidth();
        int height = resultImage.getHeight();
        int[] inPixels = new int[width*height];
        getRGB(resultImage, 0, 0, width, height, inPixels);
        
        int index = 0;
        int ta = 0, tr = 0, tg = 0, tb = 0;
        for(int row=0; row<height; row++) {
        	for(int col=0; col<width; col++) {
        		index = row * width + col;
        		ta = (inPixels[index] >> 24) & 0xff;
                tr = (inPixels[index] >> 16) & 0xff;
                tg = (inPixels[index] >> 8) & 0xff;
                tb = inPixels[index] & 0xff;
                if(tr == tg && tg == tb && tb == 0) { // face skin
                	if(minY > row) {
                		minY = row;
                	}
                	
                	if(minX > col) {
                		minX = col;
                	}
                	
                	if(maxY < row) {
                		maxY = row;
                	}
                	
                	if(maxX < col) {
                		maxX = col;
                	}
                }
        	}
        }
	}
```
**缺点：**


此算法不支持多脸谱检测，不支持裸体中的脸谱检测，但是根据人脸的

生物学特征可以进一步细化分析，支持裸体人脸检测。




写本文章的目的：本例为图像处理综合运行的一个简单实例。同时人脸检

测也是个感兴趣的话题。



