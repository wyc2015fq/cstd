# arcgis 画各种element(LineElement, PolygonElement) - youfangyuan - CSDN博客
2013年03月25日 23:18:40[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：3164
画圆弧
```java
private IElement createPolylineElement() throws Exception
	{
		ISegmentCollection segmentCollection = new Path();
		
		CircularArc arc = new CircularArc();
		Point start = new Point();
		Point end = new Point();
		Point middle = new Point();
		start.putCoords(200, 200);
		end.putCoords(400, 200);
		middle.putCoords(300, 250);
		arc.constructThreePoints(start, end, middle, false);
		segmentCollection.addSegment(arc, null, null);
		
		Polyline polyline = new Polyline();
		IGeometry geometry = (IGeometry)(segmentCollection);
		polyline.addGeometry(geometry, null, null);
		
		ILineSymbol lineSymbol = MakeNewSimpleLineSymbol();
		LineElement lineElement = new LineElement();
		lineElement.setSymbol(lineSymbol);
		lineElement.setGeometry(polyline);
		
		return lineElement;
	}
```
