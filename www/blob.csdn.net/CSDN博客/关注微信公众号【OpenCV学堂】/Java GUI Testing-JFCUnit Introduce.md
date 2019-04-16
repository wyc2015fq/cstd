# Java GUI Testing - JFCUnit Introduce - 关注微信公众号【OpenCV学堂】 - CSDN博客





2010年01月19日 20:32:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：2436








Java GUI Testing - JFCUnit Introduce

Background:

JFCUnit is an extension to the popular testing framework
[JUnit](http://www.junit.org/). This document assumes you are familiar with the usage of JUnit. If not, visit the main JUnit website where there are a number of links to some excellent resources on the subject.





Environment setup & JFCUnit installing…

Download below jars:
JUnit.jar 3.7 or greater, JFCUnit.jar, jakarta-regexp-1.5.jar

Install JRE1.4 or greater & Eclipse IDE



A Sample Java GUI & JFCUnit Test

1.Deploy downloaded jars into your project class path, see following image





2.create java GUI sample codes

**package**
 com.fish.ui;

**import**
 java.awt.BorderLayout;

**import**
 java.awt.FlowLayout;

**import**
 javax.swing.JButton;

**import**
 javax.swing.JDialog;

**import**
 javax.swing.JLabel;

**import**
 javax.swing.JPanel;

**import**
 javax.swing.JTextField;



**public****class** NewTestViewImpl
**extends** JDialog
**implements** TestView {



/**

*

*/

**private****static****final****long***serialVersionUID* = 1L;



**private** JTextField
nameField;

**private** JButton
submitButton;

**private** JButton
cancelButton;

**private** JLabel
nameLabel;

**private****boolean**proceed =
**false**;



**public** NewTestViewImpl(Object object) {

// super(object);

initComponents();

layoutComponent();

}



**private****void** initComponents() {

**this**.nameField
 = **new** JTextField(15);

**this**.submitButton
 = **new** JButton("OK");

**this**.cancelButton
 = **new** JButton("Cancel");

**this**.nameLabel
 = **new** JLabel("Test
 Name:");

}



**private****void** layoutComponent() {

JPanel topPane = **new** JPanel();

topPane.add(nameLabel);

topPane.add(nameField);

JPanel buttonPane = **new** JPanel();

buttonPane.setLayout(**new** FlowLayout());

buttonPane.add(submitButton);

buttonPane.add(cancelButton);

getContentPane().add(topPane, BorderLayout.*NORTH*);

getContentPane().add(buttonPane, BorderLayout.*LINE_END*);

pack();

}





**public** String getTestName() {

**return**nameField.getText();

}



**public****void** setTestName(String testName) {

**this**.nameField.setText(testName);

}



**public****void** display() {

**this**.setVisible(**true**);

}



**public****boolean** proceed() {

**return**proceed;

}



**public****boolean** cancelled() {

**return** !proceed;

}



**public****static****void** main(String[] args) {

**new** NewTestViewImpl(**null**).setVisible(**true**);

}



**public****void** setupJFCNames() {

nameField.setName("testNameField");

submitButton.setName("SubmitButton");

cancelButton.setName("CancelButton");

}



**public****void** addListener(Listener listener) {

submitButton.addActionListener(listener);

cancelButton.addActionListener(listener);

}





**public****void** closeView() {



}





**public****void** openView() {



}

}



3.JFCUnit test codes

**package**
 com.fish.ui.test;



**import**
 javax.swing.JButton;

**import**
 javax.swing.JComponent;

**import**
 javax.swing.JTextField;



**import**
 com.fish.ui.NewTestViewImpl;



**import**
 junit.extensions.jfcunit.JFCTestCase;

**import**
 junit.extensions.jfcunit.JFCTestHelper;

**import**
 junit.extensions.jfcunit.TestHelper;

**import**
 junit.extensions.jfcunit.finder.NamedComponentFinder;

**import**
 junit.framework.Test;

**import**
 junit.framework.TestSuite;



**public****class** NewTestViewImplTest
**extends** JFCTestCase {

**private** NewTestViewImpl
newTestViewImpl =
**null**;

**private** TestHelper
helper =
**null**;



**public** NewTestViewImplTest(String name) {

**super**(name);

}



**protected****void** setUp()
**throws** Exception {

**super**.setUp();

helper =
**new** JFCTestHelper();

newTestViewImpl =
**new** NewTestViewImpl(**null**);

newTestViewImpl.setupJFCNames();

newTestViewImpl.setVisible(**true**);

System.*out*.println("setup
 test case");

}



**protected****void** tearDown()
**throws** Exception {

newTestViewImpl =
**null**;

TestHelper.*cleanUp*(**this**);

**super**.tearDown();

System.*out*.println("setup
 test case");



}

**public****void** testUI() {

System.*out*.println("start
 testing ok button......");

NamedComponentFinder finder = **new** NamedComponentFinder(

JComponent.**class**,
"SubmitButton");

JButton submitButton = (JButton) finder.find(newTemplateViewImpl, 0);

*assertNotNull*("Could not find the submit button", submitButton);



finder = **new** NamedComponentFinder(

JComponent.**class**,
"CancelButton");

JButton CancelButton = (JButton) finder.find(newTemplateViewImpl, 0);

*assertNotNull*("Could not find the cancel button", CancelButton);



finder = **new** NamedComponentFinder(

JComponent.**class**,
"testNameField");

JTextField testNameField = (JTextField) finder.find(newTestViewImpl, 0);

*assertNotNull*("Could not find the cancel button", templateNameField);



newTestViewImpl.setTestName("mTestName");

*assertEquals*(testNameField.getText(), "mTestName");



//System.out.println(text);

testNameField.setText("");

*assertEquals*("Test field is not null","",newTestViewImpl.getTestName());



}

**public****static** Test suite() {

**return****new** TestSuite(NewTestViewImplTest.**class**);

}



**public****static****void** main(String args[]) {

junit.textui.TestRunner.*run*(*suite*());

}



}

4.Run JFCUnit test case using eclipse plug-in Junit Runner to load testcase and run.

More Information:

http://jfcunit.sourceforge.net/



