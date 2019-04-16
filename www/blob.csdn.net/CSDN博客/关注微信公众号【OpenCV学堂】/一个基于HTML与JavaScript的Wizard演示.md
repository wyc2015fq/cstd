# 一个基于HTML与JavaScript的Wizard演示 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年09月28日 17:34:31[gloomyfish](https://me.csdn.net/jia20003)阅读数：3097








整个过程模仿用户注册，分为三步完成，运行效果如下：

![](https://img-my.csdn.net/uploads/201209/28/1348825061_3808.png)


全部代码如下：



```
<html>
<head>
    <title>JavaScript Create User Wizard Example</title>
    <script type="text/javascript">
    	function handleWizardNext()
        {
            if (document.getElementById('ButtonNext').name == 'step2')
            {
            	document.getElementById('step1').style.display = 'none';
                document.getElementById('step2').style.display = '';
				document.getElementById('ButtonNext').name = 'step3';

                document.getElementById('ButtonPrevious').name = 'step1';
                document.getElementById('ButtonPrevious').disabled = '';

                document.getElementById('headstep1').style.backgroundColor = 'silver';
                document.getElementById('headstep2').style.backgroundColor = 'white';
            }
            else if(document.getElementById('ButtonNext').name == 'step3')
            {
            	document.getElementById('step2').style.display = 'none';
                document.getElementById('step3').style.display = '';

				document.getElementById('ButtonNext').name = '';
				document.getElementById('ButtonNext').disabled = 'disabled';

				document.getElementById('ButtonPrevious').name = 'step2';
                document.getElementById('ButtonPrevious').disabled = '';
				document.getElementById('SubmitFinal').disabled = '';

				document.getElementById('headstep2').style.backgroundColor = 'silver';
                document.getElementById('headstep3').style.backgroundColor = 'white';
            }

        }

        function handleWizardPre() {
        	// console.log("this is to go back previous page");
        	if (document.getElementById('ButtonPrevious').name == 'step1') {
        		// display/hide the relevative fieldset
        		document.getElementById('step2').style.display = 'none';
                document.getElementById('step1').style.display = '';

                // enable/disable buttons
				document.getElementById('ButtonNext').name = 'step2';
				document.getElementById('ButtonPrevious').name = '';
                document.getElementById('ButtonPrevious').disabled = 'disabled';
                document.getElementById('SubmitFinal').disabled = 'disabled';
                document.getElementById('SubmitFinal').name = '';

                // change navigation color
                document.getElementById('headstep2').style.backgroundColor = 'silver';
                document.getElementById('headstep1').style.backgroundColor = 'white';

        	} else if(document.getElementById('ButtonPrevious').name == 'step2') {
        		// display/hide the relevative fieldset
        		document.getElementById('step3').style.display = 'none';
                document.getElementById('step2').style.display = '';

                // enable/disable buttons
				document.getElementById('ButtonNext').name = 'step3';
				document.getElementById('ButtonNext').disabled = '';

				document.getElementById('ButtonPrevious').name = 'step1';
                document.getElementById('ButtonPrevious').disabled = '';
                
                document.getElementById('SubmitFinal').name = '';
                document.getElementById('SubmitFinal').disabled = 'disabled';

                // change navigation color
                document.getElementById('headstep3').style.backgroundColor = 'silver';
                document.getElementById('headstep2').style.backgroundColor = 'white';        		
        	}
        }
    </script>
    <style type="text/css">  
    	#userregisterizard {
    		width:500px;
    		height: 350px;
    		background-color:#EFEFEF;
    		border-style:solid;
			border-width:5px;
			border-color:#001111;
    	}
    	#userregisterizard #wizardpanel{  
            padding: 5px;  
            height: 250;  
        }
        #userregisterizard #buttons{  
            padding: 5px;  
            height: 40;  
        }
        #navigationbar {
        	padding: 2px; 
        	height: 50px;
        	background-color:silver;
        }  
    </style>
</head>
<body>
	<div id="userregisterizard">
		<div id="navigationbar">
			<label id="headstep1" style="background-color:white">Step 1: Register</label>  
			<label id="headstep2" style="background-color:silver">Step 2: Detail Profile</label>  
			<label id="headstep3" style="background-color:silver">Step 3: Finalize</label>
		</div>
		<div id="wizardpanel">
			<fieldset id="step1">
	  			<legend>Create User</legend>
				<label>Name<font color="red">*</font>: Enter your preferred user name</label><br>
				<input name="wsname" type="text"></br>
				<label>Password<font color="red">*</font>: Please use 8~16 characters that will protect your user account</label><br>
				<input name="uspassword" type="password"></br>
			</fieldset>

			<!-- edit yourself profile -->
			<fieldset id="step2" style="display:none">
				<legend>Edit Profile</legend>
				<label>Occupation<font color="red">*</font>: What do you do?</label><br>
				<input name="wsname" type="text"></br>
				<label>Company<font color="red">*</font>: Which company you are working for</label><br>
				<input name="company" type="text"></br>
				<label>Hometown<font color="red">*</font>: Where is you hometown</label><br>
				<input name="hometown" type="text"></br>
				<label>Mobilephone Number<font color="red">*</font>: Can we get your phone number?</label><br>
				<input name="phonenumber" type="text"></br>
			</fieldset>
			<fieldset id="step3" style="display:none">
	  			<legend>Welcome you</legend>
				<p>you can see the your user profile what you have entered.<p>
			</fieldset>
		</div>
		<div id="buttons">
			<input id="ButtonPrevious" type="button" value="Previous" disabled="disabled" name="" onclick="handleWizardPre()"/>
			<input id="ButtonNext" type="button" value="Next" name="step2" onclick="handleWizardNext()" />
			<input id="SubmitFinal" type="submit" value="Finish" disabled="disabled" />
		</div>
	</div>
</body>
</html>
```





