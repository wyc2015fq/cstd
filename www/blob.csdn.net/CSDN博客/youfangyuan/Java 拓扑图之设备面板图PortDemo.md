# Java 拓扑图之设备面板图PortDemo - youfangyuan - CSDN博客
置顶2016年02月20日 21:21:22[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：1327
```java
/**
 * 
 * @author Administrator
 * 
 */
public class PortDemo extends DemoPane {
	public static void main(String argv[]) {
		DemoUtil.show(new PortDemo());
	}
	BDataBox box = new BDataBox();
	BNetwork network = new BNetwork(box);
	public PortDemo() {
		init();
		this.setLayout(new BorderLayout());
		this.add(this.network, BorderLayout.CENTER);
	}
	void init() {
		BaseEquipment equipment = new BaseEquipment();
		equipment.setImage("/demo/databox/port/equipment.png");
		box.addElement(equipment);
		box.addElement(createText("OSM-4OC3-POS-SI", 60, 15));
		int count = 1;
		int x = 60;
		int y = 20;
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 17; col++) {
				box.addElement(createPort(String.valueOf(count++), x + col * 70, y + row * 55));
			}
		}
		x = 60;
		y = 130;
		for (int index = 0; index < 2 * 17; index++) {
			box.addElement(createLed(x + index * 40, y));
		}
	}
	/**
	 * 
	 */
	Text createText(String str, int x, int y) {
		Text text = new Text(str);
		text.putLabelColor(Color.white);
		text.setDisplayName(str);
		text.setLocation(x, y);
		return text;
	}
	Port createPort(String displayName, int x, int y) {
		Port port = new Port();
		port.setDisplayName(displayName);
		port.setImage("/demo/databox/port/port.png");
		port.setLocation(x, y);
		port.putLabelColor(Color.white);
		port.getAlarmState().setNewAlarmCount(BladeUtil.getRandomSeverity(), BladeUtil.getRandomInt(20));
		return port;
	}
	Port createLed(int x, int y) {
		Port port = new Port();
		port.putCustomDraw(true);
		port.setSize(20, 20);
		port.setLocation(x, y);
		
		port.putCustomDrawFillColor(Color.green);
		if(BladeUtil.getRandomBool())
			port.putCustomDrawFillColor(Color.red);
		port.putCustomDrawOutline(false);
		port.putLabelColor(Color.white);
		port.setDisplayName("T/R");
		return port;
	}
}
```
Java 拓扑图之设备面板图PortDemo![](https://img-blog.csdn.net/20160220212100968)
