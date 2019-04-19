# JAVA画SVG图(Batik框架)GVTComponentDemo - youfangyuan - CSDN博客
2013年05月21日 22:03:39[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：4192
```java
package example;
import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;
import org.apache.batik.bridge.BridgeContext;
import org.apache.batik.bridge.DocumentLoader;
import org.apache.batik.bridge.GVTBuilder;
import org.apache.batik.bridge.UserAgent;
import org.apache.batik.bridge.UserAgentAdapter;
import org.apache.batik.bridge.svg12.SVG12BridgeContext;
import org.apache.batik.dom.svg.SAXSVGDocumentFactory;
import org.apache.batik.dom.svg.SVGOMDocument;
import org.apache.batik.gvt.GraphicsNode;
import org.apache.batik.swing.gvt.JGVTComponent;
import org.apache.batik.util.XMLResourceDescriptor;
import org.w3c.dom.Document;
public class GVTComponentDemo {
    public static void main(String[] args) {
        // Create a new JFrame.
        JFrame f = new JFrame("Batik");
        GVTComponentDemo app = new GVTComponentDemo(f);
        // Add components to the frame.
        f.getContentPane().add(app.createComponents());
        // Display the frame.
        f.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        });
        f.setSize(400, 400);
        f.setVisible(true);
    }
    // The frame.
    protected JFrame frame;
    // The "Load" button, which displays up a file chooser upon clicking.
    protected JButton button = new JButton("Load...");
    
    protected JGVTComponent gvtComponent = new JGVTComponent();
    public GVTComponentDemo(JFrame f) {
        frame = f;
    }
    public JComponent createComponents() {
        JPanel p = new JPanel(new FlowLayout(FlowLayout.LEFT));
        p.add(button);
        
        final JPanel panel = new JPanel(new BorderLayout());
        panel.add("Center", gvtComponent);
        panel.add("North", p);
        
        // Set the button action.
        button.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                JFileChooser fc = new JFileChooser(".//images");
                int choice = fc.showOpenDialog(panel);
                if (choice == JFileChooser.APPROVE_OPTION) {
                    File f = fc.getSelectedFile();
                    try {
                        loadSVGDocument(f.toURL().toString());
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    }
                }
            }
        });
        return panel;
    }
    
    /**
     * 加载SVG文档
     * @param url
     */
    protected void loadSVGDocument(String url)
    {
        try { 
            String parser = XMLResourceDescriptor.getXMLParserClassName(); 
            SAXSVGDocumentFactory f = new SAXSVGDocumentFactory(parser); 
            Document doc = f.createDocument(url);
            SVGOMDocument svgDocument = (SVGOMDocument) doc;
            BridgeContext bridgeContext = createBridgeContext((SVGOMDocument) doc);
            GVTBuilder builder = new GVTBuilder();
            GraphicsNode gvtRoot = builder.build(bridgeContext, svgDocument);
            
            gvtComponent.setGraphicsNode(gvtRoot);
        } catch (IOException ex) { 
            ex.printStackTrace();
        }
    }
    
    /**
     * Creates a new bridge context.
     */
    protected BridgeContext createBridgeContext(SVGOMDocument doc) {
    	
    	UserAgent userAgent = new UserAgentAdapter();
    	DocumentLoader loader = new DocumentLoader(userAgent);
        BridgeContext result;
        if (doc.isSVG12()) {
            result = new SVG12BridgeContext(userAgent, loader);
        } else {
            result = new BridgeContext(userAgent, loader);
        }
        return result;
    }
}
```
