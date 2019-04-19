# Java Gis 拓扑图（Google 地图） - youfangyuan - CSDN博客
置顶2016年02月13日 20:50:20[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：2127
```java
public class GoogleDemo extends GisDemoPane {
	public GoogleDemo() {
		init();
	}
	void init() {
		Node node1 = createNode("Manchester", 53.488046, -2.241211);
		
		Node node2 = createNode("Paris", 48.893615, 2.329102);
		Node node3 = createNode("Berlin", 52.522906, 13.403320);
		Node node4 = createNode("Praha", 50.092393, 14.414063);
	
		box.addElement(node1);
		box.addElement(node2);
		box.addElement(node3);
		box.addElement(node4);
		
		box.addElement(createLink(node1, node2));
		box.addElement(createLink(node2, node3));
		box.addElement(createLink(node3, node4));
		this.network.setTileSource(new GoogleTileSource());
	}
	private static Node createNode(String displayName, double lat, double lon) {
		Node node2 = new Node();
		node2.setDisplayName(displayName);
		node2.putCustomDraw(true);
		node2.putCustomDrawFillColor(BladeUtil.getRandomColor());
		node2.putCustomDrawOutline(false);
		node2.setGeoPosition(new Point3D(lat, lon));
		return node2;
	}
	
	private static Link createLink(Node from, Node to){
		Link link = new Link(from, to);
		link.putLink3D(true);
		link.putLinkColor(BladeUtil.getRandomColor());
		link.putLinkFlowingColor(BladeUtil.getRandomColor());
		link.putLinkFlowing(true);
		link.putLinkWidth(8);
		link.putLinkFlowingWidth(4);
		return link;
	}
	public static void main(String argv[]) {
		GisDemoUtil.show(new GoogleDemo());
	}
}
```
![](https://img-blog.csdn.net/20160213204934499)
