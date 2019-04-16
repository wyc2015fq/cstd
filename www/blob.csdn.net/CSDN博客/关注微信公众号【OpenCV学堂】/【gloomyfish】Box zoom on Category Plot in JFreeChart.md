# 【gloomyfish】Box zoom on Category Plot in JFreeChart - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年05月06日 22:21:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：4819








Background:



currently JFreechart did not support domain axis zoom with category plot, the domain and value axis is zoomable only for XYPlot, however when category dataset contains huge categories while user could not select some categories to view by box zoom. the category
 plot is becoming un-usable one for user. obviously user would like to see box zoom with category plot.



Summary:



from box zoom on XYPlot in JFreechart, i read all relevant source code about zooming in JFreeChart and i found that there is a way to support box zoom on category plot by following steps:



a. support drawing the zoom rectangle in category data area (plot)

b. identify the domain axis and each category start point on domain axis.

c. measure the each category start point with zoom box

d. remove any categories if the start coordinate value is out of zoom rectangle.



Basic Design:



in order to support box zoom on category plot, we need to overwrite following methods which has been implemented in ChartPanel by JFreeChart:



1. mousePressed() - record the start zoom point

2. mouseDragged() - draw zoom box rectangle on category plot

3. mouseReleased() - zoom in the categories which is selected in rectangle.

4. paintComponent() - supporting to draw zoom rectangle



Run Result:

mouse selected rectangle - box zoom

![box zoom](http://hi.csdn.net/attachment/201105/6/0_1304691030lBLd.gif)



zooming the rectangle

![box zoom 2](http://hi.csdn.net/attachment/201105/6/0_1304691135pD7B.gif)



Code Implementation:



```java
package test.it;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Paint;
import java.awt.event.MouseEvent;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;

import javax.swing.JPanel;
import javax.swing.JPopupMenu;

import org.jfree.chart.ChartPanel;
import org.jfree.chart.ChartRenderingInfo;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.CategoryAxis;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.Zoomable;
import org.jfree.chart.renderer.category.BarRenderer;
import org.jfree.data.category.CategoryDataset;
import org.jfree.data.category.DefaultCategoryDataset;
import org.jfree.experimental.chart.plot.CombinedCategoryPlot;
import org.jfree.ui.ApplicationFrame;
import org.jfree.ui.RefineryUtilities;

/**
 * A demo for the {@link CombinedCategoryPlot} class.
 */
public class CombinedCategoryPlotDemo1 extends ApplicationFrame {

    /**
	 * 
	 */
	private static final long serialVersionUID = 8114720685282689420L;

	/**
     * Creates a new demo instance.
     *
     * @param title  the frame title.
     */
    public CombinedCategoryPlotDemo1(String title) {
        super(title);
        JPanel chartPanel = createDemoPanel();
        chartPanel.setPreferredSize(new java.awt.Dimension(500, 270));
        setContentPane(chartPanel);
    }

    /**
     * Creates a dataset.
     *
     * @return A dataset.
     */
    public static CategoryDataset createDataset2() {

        DefaultCategoryDataset result = new DefaultCategoryDataset();

        String series1 = "Third";
        String series2 = "Fourth";

        String type1 = "Type 1";
        String type2 = "Type 2";
        String type3 = "Type 3";
        String type4 = "Type 4";
        String type5 = "Type 5";
        String type6 = "Type 6";
        String type7 = "Type 7";
        String type8 = "Type 8";

        result.addValue(11.0, series1, type1);
        result.addValue(14.0, series1, type2);
        result.addValue(13.0, series1, type3);
        result.addValue(15.0, series1, type4);
        result.addValue(15.0, series1, type5);
        result.addValue(17.0, series1, type6);
        result.addValue(17.0, series1, type7);
        result.addValue(18.0, series1, type8);

        result.addValue(15.0, series2, type1);
        result.addValue(17.0, series2, type2);
        result.addValue(16.0, series2, type3);
        result.addValue(18.0, series2, type4);
        result.addValue(14.0, series2, type5);
        result.addValue(14.0, series2, type6);
        result.addValue(12.0, series2, type7);
        result.addValue(11.0, series2, type8);

        return result;

    }

    /**
     * Creates a chart.
     *
     * @return A chart.
     */
    private static JFreeChart createChart() {

        CategoryDataset dataset2 = createDataset2();
        NumberAxis rangeAxis2 = new NumberAxis("Value");
        rangeAxis2.setStandardTickUnits(NumberAxis.createIntegerTickUnits());

        CategoryAxis domainAxis = new CategoryAxis("Category");
        CategoryPlot plot = new CategoryPlot(dataset2, domainAxis, new NumberAxis("Range"), new BarRenderer());
        JFreeChart result = new JFreeChart(
                "Combined Domain Category Plot Demo",
                new Font("SansSerif", Font.BOLD, 12), plot, true);
        return result;

    }

    /**
     * Creates a panel for the demo (used by SuperDemo.java).
     *
     * @return A panel.
     */
    public static JPanel createDemoPanel() {
        JFreeChart chart = createChart();
        return new ChartPanel(chart){
        	/**
			 * 
			 */
			private static final long serialVersionUID = -4857405671081534981L;
			private Point2D zoomPoint = null;
        	private Rectangle2D zoomRectangle = null;
        	private boolean fillZoomRectangle = true;
        	private JPopupMenu popup;
        	
        	private Paint zoomOutlinePaint = Color.blue;
        	private Paint zoomFillPaint = new Color(0, 0, 255, 63);
            
        	public void mousePressed(MouseEvent e) {
        		if (e.isPopupTrigger()) {
        			if(popup == null) {
        				popup = createPopupMenu(true,true,true,true);
        			}
                    if (this.popup != null) {
                        displayPopupMenu(e.getX(), e.getY());
                        return;
                    }
                }
        		
        		PlotOrientation orientation = ((Zoomable)this.getChart().getPlot()).getOrientation();
        		System.out.println("Orientation --->> " + orientation.toString());
        		if(orientation == PlotOrientation.HORIZONTAL) {
        			return;
        		}
        		
        		if (this.zoomRectangle == null) {
                    Rectangle2D screenDataArea = getScreenDataArea(e.getX(), e.getY());
                    if (screenDataArea != null) {
                        this.zoomPoint = getPointInRectangle(e.getX(), e.getY(), screenDataArea);
                    } else {
                        this.zoomPoint = null;
                    }
                }
        	}
        	
        	private Point2D getPointInRectangle(int x, int y, Rectangle2D area) {
                double xx = Math.max(area.getMinX(), Math.min(x, area.getMaxX()));
                double yy = Math.max(area.getMinY(), Math.min(y, area.getMaxY()));
                return new Point2D.Double(xx, yy);
            }

        	public void mouseReleased(MouseEvent e) {
        		if (e.isPopupTrigger()) {
        			if(popup == null) {
        				popup = createPopupMenu(true,true,true,true);
        			}
                    if (this.popup != null) {
                        displayPopupMenu(e.getX(), e.getY());
                        zoomRectangle = null;
                        return;
                    }
                }
        		if(this.getChart().getCategoryPlot().getDataset().getColumnCount() < 2) {
        			repaint();
        			zoomRectangle = null;
        			return;        			
        		}
        		if (zoomRectangle == null) {
        			// do nothing
        		} else {
        			// do something here. zoom rectangle with data
        			System.out.println("fucking........");
        			System.out.println("reset dataset here");
        			CategoryDataset dataset = this.getChart().getCategoryPlot().getDataset();
        			System.out.println("category count = " + dataset.getColumnCount());
        			System.out.println("category type = " + dataset.getRowCount());
        			Comparable[] rowKeys = new Comparable[dataset.getRowCount()];
        			rowKeys[0] = dataset.getRowKey(0);
        			rowKeys[1] = dataset.getRowKey(1);
        			Comparable[] columnKeys = new Comparable[dataset.getColumnCount()];
        			for(int i=0; i<columnKeys.length; i++) {
        				columnKeys[i] = dataset.getColumnKey(i);
        			}
        			double[] endValueAxis = new double[dataset.getColumnCount()];
        			double[] startValueAxis = new double[dataset.getColumnCount()];
        			double minX = zoomRectangle.getBounds2D().getMinX();
        			double maxX = zoomRectangle.getBounds2D().getMaxX();
                    CategoryPlot plot = this.getChart().getCategoryPlot(); 
                    ChartRenderingInfo info = this.getChartRenderingInfo(); 
                    Rectangle2D dataArea = info.getPlotInfo().getDataArea();         			
        			CategoryAxis categoryaxis=this.getChart().getCategoryPlot().getDomainAxis();
        			for(int i=0; i<dataset.getColumnCount(); i++) {
        				endValueAxis[i] = categoryaxis.getCategoryEnd(i, dataset.getColumnCount(), dataArea, plot.getDomainAxisEdge());
        				startValueAxis[i] = categoryaxis.getCategoryStart(i, dataset.getColumnCount(), dataArea, plot.getDomainAxisEdge());
        			}
        			for(int i=0; i<endValueAxis.length; i++) {
        				if(minX > startValueAxis[i] || maxX < startValueAxis[i]) {
        					DefaultCategoryDataset defaultDataset = (DefaultCategoryDataset)dataset;
        					defaultDataset.removeValue(rowKeys[0], columnKeys[i]);
        					defaultDataset.removeValue(rowKeys[1], columnKeys[i]);
        				}
        			}        			
        		}
        		zoomRectangle = null;
        	}

        	public void mouseDragged(MouseEvent e) {
        		// if no initial zoom point was set, ignore dragging...
                if (this.zoomPoint == null) {
                    return;
                }
                
        		Graphics2D g2 = (Graphics2D) getGraphics();
        		Rectangle2D scaledDataArea = getScreenDataArea((int) this.zoomPoint.getX(), (int) this.zoomPoint.getY());
        		double ymax = Math.min(e.getY(), scaledDataArea.getMaxY());
        		double xmax = Math.min(e.getX(), scaledDataArea.getMaxX());
                this.zoomRectangle = new Rectangle2D.Double(this.zoomPoint.getX(), this.zoomPoint.getY(),
                		xmax - this.zoomPoint.getX(), ymax - this.zoomPoint.getY());
                repaint();
                g2.dispose();
        	}
        	
        	public void paintComponent(Graphics g) {
        		super.paintComponent(g);
        		Graphics2D g2 = (Graphics2D) g.create();
        		drawZoomRectangle(g2, false);
        		g2.dispose();
        	}
        	
        	private void drawZoomRectangle(Graphics2D g2, boolean xor) {
                if (this.zoomRectangle != null) {
                    if (xor) {
                         // Set XOR mode to draw the zoom rectangle
                        g2.setXORMode(Color.gray);
                    }
                    if (this.fillZoomRectangle) {
                        g2.setPaint(this.zoomFillPaint);
                        g2.fill(this.zoomRectangle);
                    }
                    else {
                        g2.setPaint(this.zoomOutlinePaint);
                        g2.draw(this.zoomRectangle);
                    }
                    if (xor) {
                        // Reset to the default 'overwrite' mode
                        g2.setPaintMode();
                    }
                }
            }
        };
    }

    /**
     * Starting point for the demonstration application.
     *
     * @param args  ignored.
     */
    public static void main(String[] args) {
        String title = "Combined Category Plot Demo 1";
        CombinedCategoryPlotDemo1 demo = new CombinedCategoryPlotDemo1(title);
        demo.pack();
        RefineryUtilities.centerFrameOnScreen(demo);
        demo.setVisible(true);
    }

}
```




I just adding some codes in the category demo program with JFreeChart, the code implementation need to be improved in future.



Drawback:



could not restore to original dataset since i just removed the categories, one way is to implement this like this:

just take back original dataset when there is only one category in plot.



Discussion:

...



