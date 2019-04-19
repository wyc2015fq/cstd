# Java拓扑图开发-带宽示例 - youfangyuan - CSDN博客
置顶2015年12月28日 21:21:08[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：3764
```java
/**
 * 区域图表
 * 
 * @author Administrator
 * 
 */
public class AreaChart extends JComponent {
	public AreaChart() {
	}
	@Override
	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		int width = this.getWidth();
		int height = this.getHeight();
		g2d.setColor(this.background);
		g2d.fillRect(0, 0, width, height);
		double gridCellWidth = width * this.gridCellPercent;
		double gridCellHeight = height * this.gridCellPercent;
		g2d.setColor(this.gridColor);
		g2d.setStroke(this.gridStroke);
		for (int x = 0; x < width; x += gridCellWidth) {
			g2d.drawLine(x, 0, x, height);
		}
		for (int y = 0; y < height; y += gridCellHeight) {
			g2d.drawLine(0, y, width, y);
		}
		double x = 0;
		double y = height;
		final double delta = this.maxValue - this.minValue;
		GeneralPath path = new GeneralPath();
		path.moveTo(x, y);
		for (int index = 0; index < this.values.size(); index++) {
			double value = this.values.get(index);
			x += gridCellWidth;
			y = height - (value - this.minValue) / delta * height;
			path.lineTo(x, y);
		}
		path.lineTo(width, height);
		path.closePath();
		g2d.setColor(this.areaColor);
		g2d.fill(path);
	}
	public void setBackground(Color background) {
		this.background = background;
	}
	public void addValue(double value) {
		if (this.values.size() >= maxCount) {
			this.values.remove(0);
		}
		if (this.maxValue < value) {
			this.maxValue = value;
		}
		if (this.minValue > value) {
			this.minValue = value;
		}
		this.values.add(value);
	}
	public void setValues(List<Double> values) {
		this.values = values;
		for (int index = 0; index < values.size(); index++) {
			double value = values.get(index);
			
			if (this.maxValue < value) {
				this.maxValue = value;
			}
			if (this.minValue > value) {
				this.minValue = value;
			}
		}
	}
	public void setDrawGrid(boolean drawGrid) {
		this.drawGrid = drawGrid;
	}
	public void setGridColor(Color gridColor) {
		this.gridColor = gridColor;
	}
	public void setAreaColor(Color areaColor) {
		this.areaColor = areaColor;
	}
	public void setGridCellPercent(double gridCellPercent) {
		this.gridCellPercent = gridCellPercent;
	}
	/**
	 * 背景色
	 */
	Color background = Color.DARK_GRAY;
	/**
	 * 面积色
	 */
	Color areaColor = Color.green;
	/**
	 * 网格颜色
	 */
	Color gridColor = Color.LIGHT_GRAY;
	/**
	 * 绘制网格
	 */
	boolean drawGrid = true;
	/**
	 * 网格大小
	 */
	double gridCellPercent = 0.1;
	/**
	 * 
	 */
	Stroke gridStroke = new BasicStroke(1);
	/**
	 * 值
	 */
	List<Double> values = new ArrayList<Double>();
	/**
	 * 最大数量
	 */
	int maxCount = 10;
	/**
	 * 最大值
	 */
	double maxValue = Double.MIN_VALUE;
	/**
	 * 最小值
	 */
	double minValue = Double.MAX_VALUE;
}
```
![](https://img-blog.csdn.net/20151228211855889)
```java
void init() {
		NetworkNode Phoenix = new NetworkNode("Phoenix", NetworkNode.Type.BSC_Image);
		Phoenix.setName("Phoenix");
		addNetNode(box, Phoenix, 332654, 1120424);
		NetworkNode Los_Angeles = new NetworkNode("Los Angeles", NetworkNode.Type.Terminal);
		Los_Angeles.setName("Los Angeles");
		addNetNode(box, Los_Angeles, 340308, 1181434);
		NetworkNode San_Francisco = new NetworkNode("San Francisco", NetworkNode.Type.MSC_Image);
		San_Francisco.setName("San Francisco");
		addNetNode(box, San_Francisco, 374630, 1222506);
		NetworkNode Denver = new NetworkNode("Denver", NetworkNode.Type.NMW_Image);
		Denver.setName("Denver");
		addNetNode(box, Denver, 394421, 1045903);
		NetworkNode Jacksonville = new NetworkNode("Jacksonville", NetworkNode.Type.MSC_Image);
		Jacksonville.setName("Jacksonville");
		addNetNode(box, Jacksonville, 301955, 813921);
		NetworkNode Chicago = new NetworkNode("Chicago", NetworkNode.Type.Router);
		Chicago.setName("Chicago");
		addNetNode(box, Chicago, 415100, 873900);
		NetworkNode New_Orleans = new NetworkNode("New Orleans", NetworkNode.Type.Server);
		New_Orleans.setName("New Orleans");
		addNetNode(box, New_Orleans, 295716, 900430);
		NetworkNode Detroit = new NetworkNode("Detroit", NetworkNode.Type.Terminal);
		Detroit.setName("Detroit");
		addNetNode(box, Detroit, 421953, 830245);
		NetworkNode Kansas_City = new NetworkNode("Kansas City", NetworkNode.Type.Terminal);
		Kansas_City.setName("Kansas City");
		addNetNode(box, Kansas_City, 390559, 943442);
		NetworkNode Charlotte = new NetworkNode("Charlotte", NetworkNode.Type.MD);
		Charlotte.setName("Charlotte");
		addNetNode(box, Charlotte, 351337, 805036);
		NetworkNode Albuquerque = new NetworkNode("Albuquerque", NetworkNode.Type.BSC_Image);
		Albuquerque.setName("Albuquerque");
		addNetNode(box, Albuquerque, 350504, 1063902);
		NetworkNode New_York = new NetworkNode("New York", NetworkNode.Type.BSC_Image);
		New_York.setName("New York");
		addNetNode(box, New_York, 404251, 740023);
		NetworkNode Memphis = new NetworkNode("Memphis", NetworkNode.Type.MD_Image);
		Memphis.setName("Memphis");
		addNetNode(box, Memphis, 350858, 900256);
		NetworkNode Nashville = new NetworkNode("Nashville", NetworkNode.Type.BSC_Image);
		Nashville.setName("Nashville");
		addNetNode(box, Nashville, 360957, 864704);
		NetworkNode Dallas = new NetworkNode("Dallas", NetworkNode.Type.MSC_Image);
		Dallas.setName("Dallas");
		addNetNode(box, Dallas, 324700, 964800);
		NetworkNode El_Paso = new NetworkNode("El Paso", NetworkNode.Type.NMW_Image);
		El_Paso.setName("El Paso");
		addNetNode(box, El_Paso, 314531, 1062911);
		NetworkNode Houston = new NetworkNode("Houston", NetworkNode.Type.Terminal);
		Houston.setName("Houston");
		addNetNode(box, Houston, 294547, 952147);
		NetworkNode San_Antonio = new NetworkNode("San Antonio", NetworkNode.Type.Terminal);
		San_Antonio.setName("San Antonio");
		addNetNode(box, San_Antonio, 292526, 982936);
		NetworkNode Virginia_Beach = new NetworkNode("Virginia Beach", NetworkNode.Type.Router);
		Virginia_Beach.setName("Virginia Beach");
		addNetNode(box, Virginia_Beach, 365110, 755842);
		NetworkNode Seattle = new NetworkNode("Seattle", NetworkNode.Type.NMW_Image);
		Seattle.setName("Seattle");
		addNetNode(box, Seattle, 473623, 1221951);
		addNetLink(box, NetworkLink.Media.Fiber, New_York, Virginia_Beach);
		addNetLink(box, null, New_York, Detroit);
		addNetLink(box, NetworkLink.Media.CNET, Charlotte, Virginia_Beach);
		addNetLink(box, NetworkLink.Media.Electrical, Charlotte, Jacksonville);
		addNetLink(box, NetworkLink.Media.Fiber, Charlotte, Nashville);
		addNetLink(box, NetworkLink.Media.CNET, Memphis, Nashville);
		addNetLink(box, NetworkLink.Media.Electrical, New_Orleans, Nashville);
		addNetLink(box, NetworkLink.Media.Fiber, Jacksonville, Nashville);
		addNetLink(box, null, Detroit, Chicago);
		addNetLink(box, NetworkLink.Media.CNET, Memphis, Chicago);
		addNetLink(box, NetworkLink.Media.Electrical, Kansas_City, Chicago);
		addNetLink(box, NetworkLink.Media.Fiber, Memphis, Dallas);
		addNetLink(box, NetworkLink.Media.CNET, Kansas_City, Dallas);
		addNetLink(box, NetworkLink.Media.Electrical, San_Antonio, Dallas);
		addNetLink(box, NetworkLink.Media.Fiber, Houston, Dallas);
		addNetLink(box, NetworkLink.Media.CNET, Denver, Dallas);
		addNetLink(box, NetworkLink.Media.Electrical, Albuquerque, Dallas);
		addNetLink(box, NetworkLink.Media.Fiber, Albuquerque, Denver);
		addNetLink(box, null, Albuquerque, El_Paso);
		addNetLink(box, NetworkLink.Media.Electrical, Albuquerque, Phoenix);
		addNetLink(box, null, Los_Angeles, Phoenix);
		addNetLink(box, NetworkLink.Media.CNET, San_Francisco, Phoenix);
		addNetLink(box, NetworkLink.Media.Fiber, Seattle, San_Francisco);
		addNetLink(box, null, Seattle, Denver);
		addNetLink(box, NetworkLink.Media.CNET, San_Francisco, Denver);
		network.setBackground(new ImageBackground("/demo/resource/images/us_map.png"));
	}
```
```java
设置节点样式
		node.putLabelColor(Color.lightGray);
		node.addAttachment("areachart");
		List<Double> values = new ArrayList<Double>();
		for (int index = 1; index < 10; index++) {
			values.add(1.0 * BladeUtil.getRandomInt(100));
		}
		node.putAttachmentXOffset(-15);
		node.putAttachmentPosition(BladeConst.POSITION_TOPLEFT);
		node.putAttachmentProperty("areachart", "values", values);
		node.putAttachmentProperty("areachart", "background", Color.black);
		node.putAttachmentProperty("areachart", "gridcolor", Color.white);
		node.putAttachmentProperty("areachart", "areacolor", Color.red);
		node.putAttachmentProperty("areachart", "gridcellpercent", 0.25);
```
