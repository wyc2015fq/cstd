# [eZ publish] (Tranfer) Build a form use the information collector - weixin_33985507的博客 - CSDN博客
2010年04月21日 10:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
### Creating a custom form
To do this, log in to the Administration Interface and click on the **Setup** tab. In the left menu, click the **Classes** link: 
![4_click_on_the_classes_link_large.png](http://share.ez.no/var/community/storage/images/media/images/4_click_on_the_classes_link_large2/317757-1-eng-GB/4_click_on_the_classes_link_large.png)
![5_content_class_group_large.png](http://share.ez.no/var/community/storage/images/media/images/5_content_class_group_large2/317760-1-eng-GB/5_content_class_group_large.png)
![7_new_class_button_large.png](http://share.ez.no/var/community/storage/images/media/images/7_new_class_button_large2/317763-1-eng-GB/7_new_class_button_large.png)
![8_class_edit_interface_large.png](http://share.ez.no/var/community/storage/images/media/images/8_class_edit_interface_large2/317766-1-eng-GB/8_class_edit_interface_large.png)
*Add attribute *
Note that the attributes that will collect information (through form fields presented to the visitor) need to have the **Information collector** checkbox marked: 
![10_information_collector_checkbox_large.png](http://share.ez.no/var/community/storage/images/media/images/10_information_collector_checkbox_large2/317772-1-eng-GB/10_information_collector_checkbox_large.png)
*Information collector checkbox *
Rather than repeating theexplanation of the steps to add each attribute, the information for allthe attributes is summarized in the table below: 
|Name|Identifier|Datatype|Required|Searchable|Information collector|
|----|----|----|----|----|----|
|Form Name|form_name|Text line|Yes|Yes|No|
|Form Introduction|form_introduction|XML block|No|Yes|No|
|Given Name|given_name|Text line|Yes|No|Yes|
|Family Name|family_name|Text line|Yes|No|Yes|
|Date of Birth|date_of_birth|Date|Yes|No|Yes|
|Contact Phone Number|contact_phone_number|Text line|Yes|No|Yes|
|Email Address|email_address|E-mail|Yes|No|Yes|
|Postal Address|postal_address|Text block|Yes|No|Yes|
|Please send me information    about the club's tennis tournaments|option_play_in_tournaments|Checkbox|No|No|Yes|
|Please send me information    about tennis classes and professional coaching|option_classes_and_coaching|Checkbox|No|No|Yes|
|I would like to receive the    monthly club newsletter by email|option_club_newsletter|Checkbox|No|No|Yes|
|Please keep me informed    about the club's social events and competitions|option_social_events_            competitions|Checkbox|No|No|Yes|
|Can we help by answering any    questions?|other_questions|Text block|No|No|Yes|
|Membership Coordinator    E-mail Address|membership_coordinator_email_            address|E-mail|Yes|No|No|
When using the first method, thepage will automatically reload with the attribute order changed.However, if the attributes are renumbered using the second method, youmust click the **Apply** button (or **OK** button if you are finished editing the class) to store the changes. 
### Saving the new class
Once you have entered all the relevant attribute information, click the **OK** button to add the new class. 
The next step is to modify the appropriate INI settings in order to configure the form. 
### Defining how information is collected
The *settings/collect.ini* file contains some useful form options. 
Recall from the [first article in this series](http://share.ez.no/articles/ez-publish/how-to-configure-an-ez-publish-site)that eZ Publish reads basic settings from the default INI files, whichare overriden by any settings in the specific siteaccess overrides,which are overridden by settings in the global override files. Youshould never modify the default INI files. 
The settings below should be created in the file *collect.ini.append.php* located either: 
- 
In  the directory for the siteaccess (in  *settings/siteaccess/[name_of_siteaccess]/*) that will display  the form; or
- 
In the override directory  (in *settings/override/*), meaning it will apply to all  siteaccesses.
If the override file does not already exist, you must create it. 
The sections below show thesettings that need to be included in the INI file. Note that linesstarting with hash symbols are comments. 
#### [InfoSettings] block
The setting below specifies thatthere is an information collector based on the class ID“membership_application_form”, and that the identifier for theinformation collector is the same as the class name. 
[InfoSettings]# Matches class id or identifier to information collection typeTypeList[membership_application_form]=membership_application_form
#### [EmailSettings] block
Sometimes it is useful to send anemail summarizing the results of each form submission. This is notrequired for objects such as polls, but we will use it in ourmembership application form. 
[EmailSettings]# Matches class id or identifier to information collection type# SendEmailList[poll]=disabledSendEmailList[membership_application_form]=enabled
#### [DisplaySettings] block
The DisplayList setting specifies the page that should be loaded after the form is submitted. There are three main options: 
[DisplaySettings]# result  - Display IC result, for instance poll result or your form data# redirect - Redirect to a specific url# node    - Redirect back to content node# Matches class id or identifier to information collection type#DisplayList[poll]=resultDisplayList[membership_application_form]=result
#### [CollectionSettings] block
Some forms need to be available to anonymous users. This is determined by the setting below. 
[CollectionSettings]# if enabled then information from anonymous users can be collected# CollectAnonymousData=enabled# Same as CollectAnonymousData but is a list of IC types and# their override settings, if specified it will override default settingCollectAnonymousDataList[membership_application_form]=enabled
# How information collection is handled in terms of user identification## multiple - each user can submit multiple data# unique   - one set of data per user, if already exists give a warning# overwrite - one set of data per user but new entry overwrites old oneCollectionUserData=multiple# Matches class id or identifier to information collection type#CollectionUserDataList[feedback]=multipleCollectionUserDataList[membership_application_form]=multiple
### Making the form visible in the site’s menus
eZ Publish does not automaticallyinclude objects of a new class in the default top or left site menus.For example, the top menu displays the objects that are directly belowthe top-level node of the Content branch, but only for the classesspecified in *menu.ini*. Similarly, the left menu displays theobjects that are directly below the second-level node that is currentlybeing viewed, but also only for the classes specified in *menu.ini*. The new Membership Application Form class needs an entry in the relevant *menu.ini* file. 
This example uses the Website Interface extension, so either of these settings override files can be edited: 
*/extension/ezwebin/settings/menu.ini.append.php*
OR 
*/settings/override/menu.ini.append.php*
The entries added are shown below. 
![13_menu_ini_settings_large.png](http://share.ez.no/var/community/storage/images/media/images/13_menu_ini_settings_large2/317778-1-eng-GB/13_menu_ini_settings_large.png)
*menu.ini settings *
This may not be necessary for allforms if you do not need them to be visible in the top or left menus(for example, if you are just going to link to the form in the body ofan article). In our case, we have added the appropriate entries to boththe TopIdentifierList setting (objects of these classes are shown inthe flat_top menu template at *extension/ezwebin/design/ezwebin/templates/menu/flat_top.tpl*), and the LeftIdentifiersList setting (objects of these classes are shown in the flat_left menu template at *extension/ezwebin/design/ezwebin/templates/menu/flat_left.tpl*). 
![14_menu_areas_large.png](http://share.ez.no/var/community/storage/images/media/images/14_menu_areas_large2/317781-1-eng-GB/14_menu_areas_large.png)
*Menu areas *
