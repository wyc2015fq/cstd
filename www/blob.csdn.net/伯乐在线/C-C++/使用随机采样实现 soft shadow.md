# 使用随机采样实现 soft shadow - 文章 - 伯乐在线
原文出处： [妈妈说女孩子要自立自强](http://blog.csdn.net/candycat1992/article/details/8981370)
本篇文章主要针对《OpenGL 4 Sharding Language Cookbook》一书中第七章——Shadow的第四节Creating soft shadow edges with random sampling解释而得。
#### 锯齿问题（aliasing）
基于shadow map实现阴影效果有很多不是那么令人满意的地方，其中一个就是锯齿问题（aliasing）。出现锯齿的原因是因为shadow texture的大小往往小于屏幕大小，当把shadow texture渲染到屏幕上时，那些多余的像素就会显现出来锯齿。
最基本的shadow map的思想，我想大家都知道，就不说了。它在最后会根据shadow texture上的深度值来判断当前要渲染的fragment是否在阴影里，如果是，那么它的visibility为0，否则就是1。这样0和1的突变在图像边缘处就表现为锯齿。
#### PCF 方法
为了解决这个问题，一种通常的方法是使用PCF（percentage-closer filtering）。它的基本思想就是避免visibility从0到1的突变，它的基本实现是，当把某一fragment转换到阴影空间（也可以说是shadow texture上对应的某一个像素）时，visibility的取值由以该像素为中心的某一区域内一些采样点的visibility乘以它们的相对于中心点的百分比来决定的。当这个区域越大，采样点越多，它的边缘模糊效果就越好。通常简单的实现是，在生成shadow texture的时候采用线性插值，并在渲染时判断某fragment是否在阴影中时，对其在shadow texture上对应的像素的周围固定的若干像素（如取其左上、右上、左下、右下四个像素）进行采样，最后取平均值来模拟实现soft shadow。
但这个方法有几个缺点，一个是得到的边缘的blur效果不是那么明显，它是以采样区域的大小和采样点的数目决定的，但这势必会带来性能上的下降；另一个是对于一些完全在阴影里或者完全在阴影外面的点来说，这些采样完全是浪费的，因为所有采样点肯定要么是1，要么是0。而随机采样就是为了解决这两个问题而出现的。
#### 随机采样
顾名思义，它的采样是随机的，如下图所示。
![](http://jbcdn2.b0.upaiyun.com/2016/11/e6ffbfe7e6aa60ff0109f0844f4345c2.png)
上图显示的是一张shadow texture，上面的每一个像素都对应了一个visibility，取值为0或1。我们假设当前需要处理的fragment对应到shadow texture上后是十字架中心的那个像素点（下面简称为中心点）。现在我们的采样点来自它周围那些画“X ”的所有点，最后，该fragment的visibility就是这些采样点的visibility的均值。现在的问题是，这些采样点纹理坐标应该如何确定。我们假设中心点的纹理坐标是知道的，那么问题转化为，如何确定这些采样点的纹理坐标相对于中心点的偏移量。还有一个问题，就是对于不同的中心点，它们对应的采样点的偏移量是否一样。这里我们没有真正完全随机，也就是说，我们有一些候选的随机坐标组（这些坐标在一开始是随机生成的，每一组包含了随机采样所需的所有采样点偏移坐标），当需要采样时，我们从这些候选的随机坐标组中选择一组进行计算。当然，我们可以完全随机，也就是说计算每一个中心点的visibility时，它的所有采样点都在一定区域内随机选取，但是这样的代价是巨大的，因为每个fragment都需要重新计算采样点的坐标偏移。
再看这张图，我们发现所有的采样点包含在一个圆内，并且被分为了8个区域，每个区域随机取一个点，这就是我们算法的基本思想。这里我们引入两个变量，sampleU和sampleV，sampleU表示的圆被直线分割的区域数，sampleV表示圆环的个数，那么采样点的个数就是sampleU*sampleV。拿上面这张图来说，它的sampleU和sampleU都是4，而下面这张图sampleU为8，sampleV为6，所以它共有8*6 = 48个采样点。（好吧，请原谅它这么丑。。）
![](http://jbcdn2.b0.upaiyun.com/2016/11/1451ea4935379bd0cb4495b77a7ad12f.png)
前面所过，我们需要事先生成一些候选的采样偏移坐标组，然后把它们存储起来，在计算时再读取它们。这是通过一张三维纹理贴图来实现的。我们在引入一个vec3类型的变量OffsetTexSize，它定义了该三维纹理的width、height以及depth的大小，那么width*height就是我们候选坐标组的个数，而depth的值是sampleU*sampleV除以2。又没懂吧，我们看一下三维纹理的存储。
下图中的s t r分别对应我们之前说的width height 和depth，我们可以理解为，每一个小正方体存储了一个类型为vec4值，也就是该三维纹理坐标为(s, t, r)的点对应的值。而类似图中红色部分的一组正方体则存储了一个候选坐标组。这样的一组小正方体组共有width*height组，因此候选坐标组的个数为width*height。那么，depth值（也就是每一组小正方体的个数）不是因为等于sampleU*sampleV也就是采样点的个数吗？为什么是等于采样点总数的一半呢？这是因为，我们之前说过每个小正方体存储的是一个vec4类型的数据，一个vec4数据可以存储两个坐标，因此一个小正方体内就可以存储两个采样点偏移坐标，所以只需一半就可以存储所有的采样点偏移坐标。
![](http://jbcdn2.b0.upaiyun.com/2016/11/3411a4dbd17ee5f5ab3e5c76e77f5521.jpeg)
这样，坐标组的存储问题就解决了。剩下的问题是，如何生成这些坐标组。我们依旧假设sampleU=sampleV=4,那么其中一组偏移坐标的生成过程如下图。
![](http://jbcdn2.b0.upaiyun.com/2016/11/7355e10ec434a699416389b9874b8af3.png)
对于坐标为(u,v)的采样点，转换到最右图中，对应的就是从外往里数第v层圆环、从x轴正方向逆时针数第u个小区域内的点。它的计算公式为：
![](http://jbcdn2.b0.upaiyun.com/2016/11/f1a521a3bf674437b348bf3f8ba0259c.png)
其中Wx和Wy是真正偏移坐标。
该三维纹理生成过程的C++程序如下：


```
void SceneJitter::buildJitterTex()  
{  
    int size = jitterMapSize;  
    int samples = samplesU * samplesV;  
    int bufSize = size * size * samples * 2;  
    float *data = new float[bufSize];  
  
    for( int i = 0; i < size; i++ ) {  
        for(int j = 0; j < size; j++ ) {  
            for( int k = 0; k < samples; k += 2 ) {  
                int x1,y1,x2,y2;  
                x1 = k % (samplesU);  
                y1 = (samples - 1 - k) / samplesU;  
                x2 = (k+1) % samplesU;  
                y2 = (samples - 1 - k - 1) / samplesU;  
  
                vec4 v;  
                // Center on grid and jitter  
                v.x = (x1 + 0.5f) + jitter();  
                v.y = (y1 + 0.5f) + jitter();  
                v.z = (x2 + 0.5f) + jitter();  
                v.w = (y2 + 0.5f) + jitter();  
  
                // Scale between 0 and 1  
                v.x /= samplesU;  
                v.y /= samplesV;  
                v.z /= samplesU;  
                v.w /= samplesV;  
  
                // Warp to disk  
                int cell = ((k/2) * size * size + j * size + i) * 4;  
                data[cell+0] = sqrtf(v.y) * cosf( TWOPI * v.x );  
                data[cell+1] = sqrtf(v.y) * sinf( TWOPI * v.x );  
                data[cell+2] = sqrtf(v.w) * cosf( TWOPI * v.z );  
                data[cell+3] = sqrtf(v.w) * sinf( TWOPI * v.z );  
            }  
        }  
    }  
  
    glActiveTexture(GL_TEXTURE1);  
    GLuint texID;  
    glGenTextures(1, &texID);  
  
    glBindTexture(GL_TEXTURE_3D, texID);  
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, size, size, samples/2, 0, GL_RGBA, GL_FLOAT, data);  
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  
  
    delete [] data;  
}  
  
// Return random float between -0.5 and 0.5  
float SceneJitter::jitter() {  
    return ((float)rand() / RAND_MAX) - 0.5f;  
}
```
其对应的fragment shader中的关于阴影计算的部分如下：


```
void shadeWithShadow()  
{  
    vec3 ambient = Light.Intensity * Material.Ka;  
    vec3 diffAndSpec = phongModelDiffAndSpec();  
  
    ivec3 offsetCoord;  
    offsetCoord.xy = ivec2( mod( gl_FragCoord.xy, OffsetTexSize.xy ) );  
  
    float sum = 0.0;  
    int samplesDiv2 = int(OffsetTexSize.z);  
    vec4 sc = ShadowCoord;  
  
    for( int i = 0 ; i < 4; i++ ) {  
        offsetCoord.z = i;  
        vec4 offsets = texelFetch(OffsetTex,offsetCoord,0) * Radius * ShadowCoord.w;  
  
        sc.xy = ShadowCoord.xy + offsets.xy;  
        sum += textureProj(ShadowMap, sc);  
        sc.xy = ShadowCoord.xy + offsets.zw;  
        sum += textureProj(ShadowMap, sc);  
    }  
    float shadow = sum / 8.0;  
  
    if( shadow != 1.0 && shadow != 0.0 ) {  
        for( int i = 4; i < samplesDiv2; i++ ) {  
            offsetCoord.z = i;  
            vec4 offsets = texelFetch(OffsetTex, offsetCoord,0) * Radius * ShadowCoord.w;  
  
            sc.xy = ShadowCoord.xy + offsets.xy;  
            sum += textureProj(ShadowMap, sc);  
            sc.xy = ShadowCoord.xy + offsets.zw;  
            sum += textureProj(ShadowMap, sc);  
        }  
        shadow = sum / float(samplesDiv2 * 2.0);  
    }  
  
    FragColor = vec4(diffAndSpec * shadow + ambient, 1.0);  
  
    // Gamma correct  
    FragColor = pow( FragColor, vec4(1.0 / 2.2) );  
}
```
