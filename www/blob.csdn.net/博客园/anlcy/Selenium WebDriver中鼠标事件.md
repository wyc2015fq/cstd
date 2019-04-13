
# Selenium WebDriver中鼠标事件 - anlcy - 博客园






# [Selenium WebDriver中鼠标事件](https://www.cnblogs.com/camilla/p/7697963.html)
鼠标点击操作
鼠标点击事件有以下几种类型：
清单 1. 鼠标左键点击
Actions action = new Actions(driver);action.click();// 鼠标左键在当前停留的位置做单击操作
action.click(driver.findElement(By.name(element)))// 鼠标左键点击指定的元素

清单 2. 鼠标右键点击
Actions action = new Actions(driver);
action.contextClick();// 鼠标右键在当前停留的位置做单击操作
action.contextClick(driver.findElement(By.name(element)))// 鼠标右键点击指定的元素

清单 3. 鼠标双击操作
Actions action = new Actions(driver);
action.doubleClick();// 鼠标在当前停留的位置做双击操作
var script = document.createElement('script');
script.src = 'http://static.pay.baidu.com/resource/baichuan/ns.js'; document.body.appendChild(script);
action.doubleClick(driver.findElement(By.name(element)))// 鼠标双击指定的元素

清单 4. 鼠标拖拽动作
Actions action = new Actions(driver);
// 鼠标拖拽动作，将 source 元素拖放到 target 元素的位置。  action.dragAndDrop(source,target);
// 鼠标拖拽动作，将 source 元素拖放到 (xOffset, yOffset) 位置，其中 xOffset 为横坐标，yOffset 为纵坐标。
action.dragAndDrop(source,xOffset,yOffset);
在这个拖拽的过程中，已经使用到了鼠标的组合动作，首先是鼠标点击并按住
(click-and-hold) source 元素，然后执行鼠标移动动作 (mouse move)，移动到 
target 元素位置或者是 (xOffset, yOffset) 位置，再执行鼠标的释放动作 (mouse 
release)。所以上面的方法也可以拆分成以下的几个执行动作来完成：
action.clickAndHold(source).moveToElement(target).perform();   action.release();

清单 5. 鼠标悬停操作
Actions action = new Actions(driver);
action.clickAndHold();// 鼠标悬停在当前位置，既点击并且不释放  action.clickAndHold(onElement);// 鼠标悬停在 onElement 元素的位置
action.clickAndHold(onElement) 这个方法实际上是执行了两个动作，首先是鼠标移动到元素 
onElement，然后再 clickAndHold, 所以这个方法也可以写成 
action.moveToElement(onElement).clickAndHold()。

清单 6. 鼠标移动操作
Actions action = new Actions(driver);
action.moveToElement(toElement);// 将鼠标移到 toElement 元素中点 // 将鼠标移到元素 toElement 的 (xOffset, yOffset) 位置，
//这里的 (xOffset, yOffset) 是以元素 toElement 的左上角为 (0,0) 开始的 (x, y) 坐标轴。
var cpro_psid ="u2572954"; var cpro_pswidth =966; var cpro_psheight =120;
action.moveToElement(toElement,xOffset,yOffset)
// 以鼠标当前位置或者 (0,0) 为中心开始移动到 (xOffset, yOffset) 坐标轴  action.moveByOffset(xOffset,yOffset);
action.moveByOffset(xOffset,yOffset) 这里需要注意，如果 xOffset 
为负数，表示横坐标向左移动，yOffset 为负数表示纵坐标向上移动。而且如果这两个值大于当前屏幕的大小，鼠标只能移到屏幕最边界的位置同时抛出 
MoveTargetOutOfBoundsExecption 的异常。 鼠标移动操作在测试环境中比较常用到的场景是需要获取某元素的 
flyover/tips，实际应用中很多 flyover 只有当鼠标移动到这个元素之后才出现，所以这个时候通过执行
moveToElement(toElement) 
操作，就能达到预期的效果。但是根据我个人的经验，这个方法对于某些特定产品的图标，图像之类的 flyover/tips 
也不起作用，虽然在手动操作的时候移动鼠标到这些图标上面可以出现 flyover, 但是当使用 WebDriver 
来模拟这一移动操作时，虽然方法成功执行了，但是 flyover 却出不来。所以在实际应用中，还需要对具体的产品页面做相应的处理。

清单 7. 鼠标释放操
Actions action = new Actions(driver);   action.release();// 释放鼠标





