# opengl画圆弧，画圆 - youfangyuan - CSDN博客
2013年08月15日 22:07:37[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：3929
```cpp
```
void DrawCircle(float cx, float cy, float r, int num_segments) 
```cpp
{ 
	float theta = 2 * 3.1415926 / float(num_segments); 
	float tangetial_factor = tanf(theta);//calculate the tangential factor 
	float radial_factor = cosf(theta);//calculate the radial factor 
	
	float x = r;//we start at angle = 0 
	float y = 0; 
    
	glBegin(GL_LINE_LOOP); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
        
		//calculate the tangential vector 
		//remember, the radial vector is (x, y) 
		//to get the tangential vector we flip those coordinates and negate one of them 
		float tx = -y; 
		float ty = x; 
        
		//add the tangential vector 
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
        
		//correct using the radial factor 
		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd(); 
}
```
void DrawCircle(float cx, float cy, float r, int num_segments) { glBegin(GL_LINE_LOOP); for(int ii = 0; ii < num_segments; ii++) { float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle float x = r * cosf(theta);//calculate
 the x component float y = r * sinf(theta);//calculate the y component glVertex2f(x + cx, y + cy);//output vertex } glEnd(); }
```cpp
```
![](https://img-blog.csdn.net/20130815220656125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91ZmFuZ3l1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```cpp
void DrawArc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments) 
{ 
	float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
	float tangetial_factor = tanf(theta);
	float radial_factor = cosf(theta);
	
	float x = r * cosf(start_angle);//we now start at the start angle
	float y = r * sinf(start_angle); 
    
	glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
	for(int ii = 0; ii < num_segments; ii++)
	{ 
		glVertex2f(x + cx, y + cy);
		float tx = -y; 
		float ty = x; 
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd(); 
}
```
```cpp
void DrawCircle(float cx, float cy, float r, int num_segments) 
{ 
	float theta = 2 * 3.1415926 / float(num_segments); 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;
	float x = r;//we start at angle = 0 
	float y = 0; 
    
	glBegin(GL_LINE_LOOP); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
        
		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd(); 
}
```
[http://slabode.exofire.net/circle_draw.shtml](http://slabode.exofire.net/circle_draw.shtml)
