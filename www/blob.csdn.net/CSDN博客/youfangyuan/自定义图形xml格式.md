# 自定义图形xml格式 - youfangyuan - CSDN博客
2013年03月25日 23:35:23[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：559
```
<element>
	<polygon>
		<line startX="" startY="" endX="" endY="">
		<line/>
		<line startX="" startY="" endX="" endY="">
		<line/>
		<circularArc radiusX="" radiusY="" startAngle="" endAngle="">
		<circularArc/>
		<circularArcPoint startX="" startY="" endX="" endY="" middleX="" middleY="">
		<circularArcPoint/>
	<polygon/>
	<circle radiusX="" radiusY="" radius="">
	<circle/>
	<circleBoundry radiusX="" radiusY="" radius="">
	<circleBoundry>
	<polyline>
		<line startX="" startY="" endX="" endY="">
		<line/>
		<circularArc radiusX="" radiusY="" startAngle="" endAngle="">
		<circularArc/>
		<circularArcPoint startX="" startY="" endX="" endY="" middleX="" middleY="">
		<circularArcPoint/>
	<polyline/>
<element/>
```
polygon:定义多边形
line:定义直线
circularArc:定义圆弧
circle:圆面
circleBoundry:圆圈
polyLine:多边线
