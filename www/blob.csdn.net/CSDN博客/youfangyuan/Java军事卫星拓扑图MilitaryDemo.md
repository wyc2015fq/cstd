# Java军事卫星拓扑图MilitaryDemo - youfangyuan - CSDN博客
置顶2016年01月15日 19:45:41[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：1068
![](https://img-blog.csdn.net/20160115194343669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```java
public class MilitaryDemo extends DemoPane {
	public static void main(String argv[]) {
		DemoUtil.show(new MilitaryDemo());
	}
	private BDataBox box = new BDataBox();
	private BNetwork network = new BNetwork(box);
	public MilitaryDemo() {
		this.add(this.network, BorderLayout.CENTER);
		init();
		
		ColorBackground background = new ColorBackground();
		background.setColor(new Color(23, 50, 7));
		background.setGradientColor(new Color(255, 255, 255));
		background.setGradient(true);
		network.setNetworkBackground(background);
	}
	void init() {
		Node f22 = new Node();
		f22.setImage("/demo/databox/military/F22.png");
		f22.setLocation(300, 100);
		Node surfaceNode = new Node();
		surfaceNode.setImage("/demo/databox/military/军舰50_50.png");
		Node satellite = new Node();
		satellite.setImage("/demo/databox/military/卫星.png");
		Node satelliteR = new Node();
		satelliteR.setImage("/demo/databox/military/sateliteAntenna.png");
		Node plane = new Node();
		plane.setImage("/demo/databox/military/伊尔76.png");
		plane.setLocation(200, 200);
		Node helicopter = new Node();
		helicopter.setImage("/demo/databox/military/直升机.png");
		box.addElement(f22);
		box.addElement(surfaceNode);
		box.addElement(satellite);
		box.addElement(satelliteR);
		box.addElement(plane);
		box.addElement(helicopter);
		RadarLink link1 = new RadarLink(plane, f22);
		link1.putRenderAlpha(0.5f);
		link1.putLinkColor(Color.green);
		box.addElement(link1);
		SignalLink link2 = new SignalLink(satellite, satelliteR);
		link2.putLinkColor(new Color(250, 128, 10));
		link2.putRenderAlpha(0.5f);
		box.addElement(link2);
		
		Link link3 = new Link(plane, helicopter);
		link3.putLinkColor(Color.green);
		link3.putLinkFlowingColor(Color.red);
		link3.putLinkFlowing(true);
		box.addElement(link3);
		
		Link link4 = new Link(plane, surfaceNode);
		link4.putLinkColor(Color.blue);
		link4.putLinkFlowingColor(Color.cyan);
		link4.putLinkFlowing(true);
		link4.putLinkWidth(5);
		box.addElement(link4);
	}
}
```
```java
public class RadarLink extends Link {
	public RadarLink() {
	}
	public RadarLink(Node from, Node to) {
		super(from, to);
	}
	@Override
	public Class<?> getUIClass() {
		return RadarLinkUI.class;
	}
}
```
```java
public class RadarLinkUI extends LinkUI {
	/**
	 * 雷达探测半径
	 */
	private int radarRadius = 30;
	public RadarLinkUI(BNetwork network, Link link) {
		super(network, link);
	}
	@Override
	public void paint(Graphics2D g2d) {
		Composite oldComp = null;
		if (this.renderAlpha != 1.0f) {
			oldComp = g2d.getComposite();
			AlphaComposite comp = AlphaComposite.getInstance(3, this.renderAlpha);
			g2d.setComposite(comp);
		}
		paintBody(g2d);
		if (oldComp != null)
			g2d.setComposite(oldComp);
	}
	/**
	 * 绘制连线
	 */
	public void paintBody(Graphics2D g2d) {
		Point fromPoint = this.getFromPoint();
		Point toPoint = this.getToPoint();
		double angle = this.getAngle();
		double deltaX1 = this.radarRadius * Math.cos(angle - Math.PI / 2);
		double deltaY1 = this.radarRadius * Math.sin(angle - Math.PI / 2);
		double deltaX2 = this.radarRadius * Math.cos(angle + Math.PI / 2);
		double deltaY2 = this.radarRadius * Math.sin(angle + Math.PI / 2);
		GeneralPath path = new GeneralPath();
		path.moveTo(fromPoint.getX(), fromPoint.getY());
		path.lineTo(toPoint.getX() + deltaX1, toPoint.getY() + deltaY1);
		path.lineTo(toPoint.getX(), toPoint.getY());
		path.lineTo(toPoint.getX() + deltaX2, toPoint.getY() + deltaY2);
		path.closePath();
		g2d.setColor(this.getLinkColor());
		g2d.fill(path);
	}
}
```
