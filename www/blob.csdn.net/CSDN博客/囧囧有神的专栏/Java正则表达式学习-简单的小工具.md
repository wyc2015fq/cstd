# Java正则表达式学习-简单的小工具 - 囧囧有神的专栏 - CSDN博客





2010年11月18日 10:48:00[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：1379标签：[正则表达式																[java																[button																[regex																[import																[input](https://so.csdn.net/so/search/s.do?q=input&t=blog)
个人分类：[语言及算法](https://blog.csdn.net/liujiyong7/article/category/1098491)





      最近在学习Java正则表达式，但是对一个正则表达式的测试，总得写一个程序才行。很不方便，于是自己做了一个简单的小工具。

自己做着玩的，水平有限，界面很简陋。



打开之后是这个样子：

![](http://hi.csdn.net/attachment/201011/18/0_1290047754AaEp.gif)



输入正则表达式和要匹配的文本后，点击Matcher按钮，就会将匹配结果显示在下面的table中（下面的是网址的正则表达式）：

![](http://hi.csdn.net/attachment/201011/18/0_1290047887Ww59.gif)



Reset按钮是从头开始匹配的意思



下面是源代码,只有两个类

主类 显示逻辑：

package frame1;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import javax.swing.*;
import javax.swing.table.TableModel;
public class RegexTest extends JFrame {
	/**
	 * @version 0.1 11/17/10
	 * @author Jiong
	 */
	private static final long serialVersionUID = 1L;
	private int int_height = 400, int_width = 600; 
	private java.awt.Toolkit tk = java.awt.Toolkit.getDefaultToolkit();
	
	public JTextField txtf_regex = new JTextField();
	public JTextArea txta_content = new JTextArea();
	
	public JButton button_match = new JButton("Matcher");
	public JButton button_reset = new JButton("Reset");
	private TableModel tablemodle  = new MyTableModle(this);
	public JTable table_result = new JTable(tablemodle); 
	public JScrollPane panel_result;
	
	public static void main(String[] args) {
		RegexTest t = new RegexTest();
		t.init();
		t.showUI();
		
	}
	private void showUI() {
		txtf_regex.setPreferredSize(new Dimension(this.int_width-10,25));
		txta_content.setPreferredSize(new Dimension(this.int_width-10,this.int_height/2));
		txtf_regex.setText("Please input Regular Expressions...");
		txtf_regex.addFocusListener(new FocusListener(){
			@Override
			public void focusGained(FocusEvent e) {
				// TODO Auto-generated method stub
				JTextField t = (JTextField)e.getSource();
				if(t.getText().equals("Please input Regular Expressions..."))
				t.setText("");
				
			}
			@Override
			public void focusLost(FocusEvent e) {
				// TODO Auto-generated method stub
				JTextField t = (JTextField)e.getSource();
				String str = t.getText().trim();
				if(str.equals(""))
				t.setText("Please input Regular Expressions...");
			}
			
		});
		txta_content.setText("Please input Content...");
		txta_content.addFocusListener(new FocusListener(){
			@Override
			public void focusGained(FocusEvent e) {
				// TODO Auto-generated method stub
				JTextArea t = (JTextArea)e.getSource();
				if(t.getText().equals("Please input Content..."))
				t.setText("");
			}
			@Override
			public void focusLost(FocusEvent e) {
				// TODO Auto-generated method stub
				JTextArea t = (JTextArea)e.getSource();
				String str = t.getText().trim();
				if(str.equals(""))
				t.setText("Please input Content...");
			}
			
		});
		this.add(txtf_regex);
		this.add(txta_content);
		
		button_match.addActionListener((ActionListener) tablemodle);
		this.add(button_match);
		
		button_reset.addActionListener((ActionListener) tablemodle);
		button_reset.setEnabled(false);
		this.add(button_reset);
		panel_result = new JScrollPane(table_result);
		panel_result.setPreferredSize(new Dimension(this.int_width,200));
		
		
		this.add(panel_result);
		//this.pack();
		this.setVisible(true);
	}
	private void init() {
		// TODO Auto-generated method stub
		this.setTitle("TestRegex");
		this.setSize(new Dimension(this.int_width,this.int_height));
		this.setLocation((tk.getScreenSize().width   //设置窗口在正中间
				-getWidth())/2,(tk.getScreenSize().height-getHeight())/2);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
	}
} 

第二个类 控制逻辑和数据

package frame1;
import javax.swing.*;
import javax.swing.event.TableModelListener;
import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
public class MyTableModle implements javax.swing.table.TableModel,ActionListener{
	private RegexTest regexTest;
	public MyTableModle(RegexTest regexTest) {
		this.regexTest = regexTest;
	}
	@Override
	public void addTableModelListener(TableModelListener l) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public Class<?> getColumnClass(int columnIndex) {
		// TODO Auto-generated method stub
		return Object.class;
	}
	@Override
	public int getColumnCount() {
		// TODO Auto-generated method stub
		return 3;
	}
	private String[] names = {"分组","命名","捕获"};
	@Override
	
	public String getColumnName(int columnIndex) {
		// TODO Auto-generated method stub
		return names[columnIndex];
	}
	@Override
	public int getRowCount() {
		// TODO Auto-generated method stub
		if(m == null) return 0;
		return m.groupCount()+1;
	}
	@Override
	public Object getValueAt(int rowIndex, int columnIndex) {
		// TODO Auto-generated method stub
		if(columnIndex==0) 
			return "$"+rowIndex;
		if(columnIndex==1) 
			return " ";
		if(columnIndex==2) 
			return m.group(rowIndex);
		return null;
	}
	@Override
	public boolean isCellEditable(int rowIndex, int columnIndex) {
		// TODO Auto-generated method stub
		return false;
	}
	@Override
	public void removeTableModelListener(TableModelListener l) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
		// TODO Auto-generated method stub
		
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////
	Pattern p = null;
	Matcher m = null;
	String regex, content = null;
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		JButton button = (JButton)e.getSource();	
		if(button.equals(regexTest.button_match)) {
			regex = this.regexTest.txtf_regex.getText();
			content = this.regexTest.txta_content.getText();
			if(m==null&&p==null) {
				try {
					p = Pattern.compile(regex);
				} catch ( java.util.regex.PatternSyntaxException e1) {
					// TODO Auto-generated catch block
					//e1.printStackTrace();
					System.out.println("正则表达式语法错误");
					return ;
				}
				m = p.matcher(content);
				regexTest.button_reset.setEnabled(true);
			}
	
			if(!m.find()) {
				m = null ;
				p = null ;
				regexTest.button_reset.setEnabled(false);
			}
//			} else {
//				regexTest.txta_content.setSelectionStart(m.start());
//				regexTest.txta_content.setSelectionEnd(m.end());
//				regexTest.txta_content.setSelectedTextColor(Color.red);
//System.out.println("start:"+m.start()+" end:"+m.end());
//System.out.println("section:"+regexTest.txta_content.getSelectedText()+regexTest.txta_content.getSelectedTextColor());
//javax.swing.SwingUtilities.updateComponentTreeUI(regexTest.txta_content);
//			}
		} else {
			m = null ;
			p = null ;
			regexTest.button_reset.setEnabled(false);
		}
		javax.swing.SwingUtilities.updateComponentTreeUI(regexTest.panel_result);	
	}
}](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=regex&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)




