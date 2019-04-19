# Java拓扑图之-总线布局BusLayoutDemo - youfangyuan - CSDN博客
置顶2016年01月11日 17:43:44[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：1253
```java
public class BusLayoutDemo extends DemoPane {
	public static void main(String argv[]) {
		DemoUtil.show(new BusLayoutDemo());
	}
	BDataBox box = new BDataBox();
	BNetwork network = new BNetwork(box);
	public BusLayoutDemo() {
		this.add(network, BorderLayout.CENTER);
		init();
		
		ColorBackground background = new ColorBackground();
		background.setColor(Color.blue);
		background.setGradient(true);
		background.setGradientColor(Color.blue.darker());
		
		network.setNetworkBackground(background);
	}
	void init() {
		ShapeNode BUS = new ShapeNode();
		BUS.setShapeNodeType(7);
		List<Element> elements = new ArrayList<Element>();
		NetworkNode Node1 = new NetworkNode("Node1", NetworkNode.Type.Office_ComputerTower);
		Node1.setName("Galileo");
		Node1.getAlarmState().addNewAlarm(AlarmSeverity.WARNING);
		elements.add(Node1);
		NetworkNode Node2 = new NetworkNode("Node2", NetworkNode.Type.Office_ComputerTower);
		Node2.setName("Voyager");
		elements.add(Node2);
		NetworkNode Node3 = new NetworkNode("Node3", NetworkNode.Type.Office_ComputerTower);
		Node3.setName("Sigma");
		Node3.getAlarmState().setNewAlarmCount(AlarmSeverity.CRITICAL, 3);
		elements.add(Node3);
		NetworkNode Node4 = new NetworkNode("Node4", NetworkNode.Type.Office_Terminal);
		Node4.setName("Apollo");
		elements.add(Node4);
		NetworkNode Node5 = new NetworkNode("Node5", NetworkNode.Type.Office_ComputerTower);
		Node5.setName("Liberty Bell");
		elements.add(Node5);
		NetworkNode Node6 = new NetworkNode("Node6", NetworkNode.Type.Office_ComputerTower);
		Node6.setName("Endeavour");
		Node6.getAlarmState().setNewAlarmCount(AlarmSeverity.WARNING, 4);
		elements.add(Node6);
		NetworkNode Node7 = new NetworkNode("Node7", NetworkNode.Type.Office_ComputerTower);
		Node7.setName("Atlantis");
		elements.add(Node7);
		NetworkNode Node8 = new NetworkNode("Node8", NetworkNode.Type.Office_ComputerTower);
		Node8.setName("Explorer");
		elements.add(Node8);
		NetworkNode Server1 = new NetworkNode("Server1", NetworkNode.Type.Office_Server2);
		Server1.setName("Mercury");
		Server1.getAlarmState().setNewAlarmCount(AlarmSeverity.WARNING, 1);
		elements.add(Server1);
		NetworkNode Server2 = new NetworkNode("Server2", NetworkNode.Type.Office_Server2);
		Server2.setName("Gemini");
		elements.add(Server2);
		NetworkNode Printer1 = new NetworkNode("Printer1", NetworkNode.Type.Office_LaserPrinter);
		Printer1.setName("Laser");
		elements.add(Printer1);
		NetworkNode Printer2 = new NetworkNode("Printer2", NetworkNode.Type.Office_InkjetPrinter);
		Printer2.setName("Inkjet");
		elements.add(Printer2);
		NetworkNode Fax = new NetworkNode("Fax", NetworkNode.Type.Office_Fax);
		Fax.setName("Fax");
		elements.add(Fax);
		NetworkNode Node9 = new NetworkNode("Node9", NetworkNode.Type.Office_Desktop);
		Node9.setName("Freedom");
		Node9.getAlarmState().setNewAlarmCount(AlarmSeverity.WARNING, 1);
		elements.add(Node9);
		NetworkNode Node10 = new NetworkNode("Node10", NetworkNode.Type.Office_Desktop);
		Node10.setName("Friendship");
		elements.add(Node10);
		NetworkNode Node11 = new NetworkNode("Node11", NetworkNode.Type.Office_Desktop);
		Node11.setName("Aurora");
		elements.add(Node11);
		BusLayout layout = new BusLayout(BUS, elements);
		layout.setBusWidth(800);
		layout.setLocation(200, 200);
		layout.setHorizontalOffset(100);
		layout.setBusHeight(400);
		layout.setBusSpace(100);
		layout.setMargin(50);
		layout.layout();
		
		CommonFunction.log(BUS.getPoints());
		box.addElement(BUS);
		box.addElements(elements);
		box.addElement(new NetworkLink(Node1, BUS));
		box.addElement(new NetworkLink(Node2, BUS));
		box.addElement(new NetworkLink(Node3, BUS));
		box.addElement(new NetworkLink(Node4, BUS));
		box.addElement(new NetworkLink(Node5, BUS));
		box.addElement(new NetworkLink(Node6, BUS));
		box.addElement(new NetworkLink(Node7, BUS));
		box.addElement(new NetworkLink(Node8, BUS));
		box.addElement(new NetworkLink(Node9, BUS));
		box.addElement(new NetworkLink(Node10, BUS));
		box.addElement(new NetworkLink(Node11, BUS));
		box.addElement(new NetworkLink(Server1, BUS));
		box.addElement(new NetworkLink(Server2, BUS));
		box.addElement(new NetworkLink(Printer1, BUS));
		box.addElement(new NetworkLink(Printer2, BUS));
		box.addElement(new NetworkLink(Fax, BUS));
	}
}
```
