
# linux驱动增加work工作队列和获取唤醒锁操作 - 嵌入式Linux - CSDN博客

2015年06月10日 08:34:52[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1023



```python
diff --git a/android-lte/kernel/drivers/input/keyboard/tca8418_keypad.c b/android-lte/kernel/drivers/input/keyboard/tca8418_keypad.c
index 187ff5b..682109b 100755
--- a/android-lte/kernel/drivers/input/keyboard/tca8418_keypad.c
+++ b/android-lte/kernel/drivers/input/keyboard/tca8418_keypad.c
@@ -141,6 +141,8 @@ struct tca8418_keypad {
 
 	struct i2c_client *client;
 	struct input_dev *input;
+	struct delayed_work tca8418_work;
+	struct wake_lock w_wake_lock;
 #ifdef CONFIG_HAS_EARLYSUSPEND
 	struct early_suspend early_suspend;
 #endif
@@ -188,6 +190,7 @@ static int tca8418_read_byte(struct tca8418_keypad *keypad_data,
 
 	return 0;
 }
+#if 1
 /*
 [函数描述] 把从寄存器里面读出来的值进行转化 5X6 原本应该读出来的值是 0~29 但是现在值不正确
 [作者]韦启发
@@ -260,42 +263,17 @@ static void tca8418_read_keypad(struct tca8418_keypad *keypad_data)
 
 	input_sync(keypad_data->input);
 }
-
+#endif
 /*
  * Threaded IRQ handler and this can (and will) sleep.
  */
 static irqreturn_t tca8418_irq_handler(int irq, void *dev_id)
 {
 	struct tca8418_keypad *keypad_data = dev_id;
-	u8 reg;
-	int error;
-	enable_irq_wake(irq);
-	printk("%s\n", __FUNCTION__);
-
-	error = tca8418_read_byte(keypad_data, REG_INT_STAT, &reg);
-	if (error) {
-		dev_err(&keypad_data->client->dev,
-			"unable to read REG_INT_STAT\n");
-		goto exit;
-	}
+	printk("%s\n",__func__);
+	enable_irq_wake(irq);/*深度休眠唤醒*/
 	
-	//printk("%s, reg = %d\n", __FUNCTION__, reg);
-
-	if (reg & INT_STAT_OVR_FLOW_INT)
-		dev_warn(&keypad_data->client->dev, "overflow occurred\n");
-
-	if (reg & INT_STAT_K_INT)
-		tca8418_read_keypad(keypad_data);
-
-exit:
-	/* Clear all interrupts, even IRQs we didn't check (GPI, CAD, LCK) */
-    //printk("%s failed\n", __FUNCTION__);
-	reg = 0xff;
-	error = tca8418_write_byte(keypad_data, REG_INT_STAT, reg);
-	if (error)
-		dev_err(&keypad_data->client->dev,
-			"unable to clear REG_INT_STAT\n");
-
+	schedule_delayed_work(&keypad_data->tca8418_work,0);/*延迟启动中断队列*/
 	return IRQ_HANDLED;
 }
 #ifdef CONFIG_PM_SLEEP
@@ -344,7 +322,8 @@ static int __devinit tca8418_configure(struct tca8418_keypad *keypad_data)
 		printk("tca8418_configure faile\n");
 		return -ENODEV;
 	}
-
+	error |= tca8418_read_byte(keypad_data, REG_CFG, &reg_r);/*键盘读寄存器，用来确认设置的是否正确*/
+	printk("%s REG_CFG[0x%x]=[0x%x]\n",__func__,REG_CFG,reg_r);
 
 	/* Assemble a mask for row and column registers */
 	reg  =  ~(~0 << keypad_data->rows);
@@ -373,6 +352,53 @@ static int __devinit tca8418_configure(struct tca8418_keypad *keypad_data)
 	return error;
 }
 
+static void tca8418_key_keypad_data(struct work_struct *work)
+{
+	struct tca8418_keypad *keypad_data = container_of((struct delayed_work*)work, struct tca8418_keypad,tca8418_work);
+	u8 reg;
+	int error;
+	wake_lock(&keypad_data->w_wake_lock);/*使能唤醒锁*/
+	disable_irq_nosync(keypad_data->irq);/*屏蔽中断*/
+	printk("%s start\n", __FUNCTION__);
+
+
+	error = tca8418_read_byte(keypad_data, REG_INT_STAT, &reg);
+	if (error) {
+		dev_err(&keypad_data->client->dev,
+			"unable to read REG_INT_STAT\n");
+		goto exit;
+	}
+	else
+		printk("%s reg = %d\n",__func__, reg);
+
+
+	if (reg & INT_STAT_OVR_FLOW_INT)
+		dev_warn(&keypad_data->client->dev, "overflow occurred\n");
+
+	if (reg & INT_STAT_K_INT)
+		tca8418_read_keypad(keypad_data);
+
+exit:
+	/* Clear all interrupts, even IRQs we didn't check (GPI, CAD, LCK) */
+    //printk("%s failed\n", __FUNCTION__);
+	reg = 0xff;
+	error = tca8418_write_byte(keypad_data, REG_INT_STAT, reg);
+	if (error){
+		dev_err(&keypad_data->client->dev,"unable to clear REG_INT_STAT\n");
+		error = tca8418_write_byte(keypad_data, REG_INT_STAT, reg);
+		if(error)
+		{
+			schedule_delayed_work(&keypad_data->tca8418_work,usecs_to_jiffies(300));/*延迟300ms启动中断队列*/
+			printk("%s unable to clear REG_INT_STAT again\n",__func__);
+		}else{
+			printk("%s\n clear REG_INT_STAT success",__func__);
+		}
+	}
+	printk("%s end\n", __FUNCTION__);
+    enable_irq(keypad_data->irq);/*重新开启中断*/
+	wake_unlock(&keypad_data->w_wake_lock);/*屏蔽唤醒锁*/
+}
+
 static int __devinit tca8418_keypad_probe(struct i2c_client *client,
 					  const struct i2c_device_id *id)
 {
@@ -496,6 +522,9 @@ static int __devinit tca8418_keypad_probe(struct i2c_client *client,
 
 	if (pdata->irq_is_gpio)
 		client->irq = gpio_to_irq(client->irq);
+	/*初始化wakelock锁，工作队列，和申请中断*/
+	wake_lock_init(&keypad_data->w_wake_lock, WAKE_LOCK_SUSPEND, "tca8418_wakelock");
+	INIT_DELAYED_WORK(&keypad_data->tca8418_work,tca8418_key_keypad_data);
 
 	error = request_threaded_irq(client->irq, NULL, tca8418_irq_handler,
 				     IRQF_TRIGGER_FALLING,
```


