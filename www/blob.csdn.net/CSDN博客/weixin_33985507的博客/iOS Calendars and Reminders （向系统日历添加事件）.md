# iOS  Calendars and Reminders （向系统日历添加事件） - weixin_33985507的博客 - CSDN博客
2017年03月22日 21:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：16
# [Demo地址](https://link.jianshu.com?t=https://github.com/StartHui/todayWidget.git)
# 一、简介
[苹果官方关于日历和提醒简介](https://link.jianshu.com?t=https://developer.apple.com/library/content/documentation/DataManagement/Conceptual/EventKitProgGuide/Introduction/Introduction.html)
**Important:**An iOS app linked on or after iOS 10.0 must include in itsInfo.plistfile the usage description keys for the types of data it needs to access or it will crash. To access Reminders and Calendar data specifically, it must include[NSRemindersUsageDescription](https://link.jianshu.com?t=https://developer.apple.com/library/content/documentation/General/Reference/InfoPlistKeyReference/Articles/CocoaKeys.html#//apple_ref/doc/uid/TP40009251-SW16)and[NSCalendarsUsageDescription](https://link.jianshu.com?t=https://developer.apple.com/library/content/documentation/General/Reference/InfoPlistKeyReference/Articles/CocoaKeys.html#//apple_ref/doc/uid/TP40009251-SW15), respectively.
To access the user’s Calendar data
提示：iOS 10 之后如果app 想要访问日历和提醒事项，我们必须在 我们的app 的Info.plist文件中添加NSRemindersUsageDescription和NSCalendarsUsageDescription的key否则app会闪退。
![1383589-83c11a9362eb9f1d.png](https://upload-images.jianshu.io/upload_images/1383589-83c11a9362eb9f1d.png)
The EventKit framework helps you access users’ Calendar and Reminders information. Although two different apps display users’ calendar and reminder data, the same framework manipulates the data. Similarly, the database that stores this data, called the Calendar database, holds both calendar and reminder information.
![1383589-bff0ed695c75dc0d.png](https://upload-images.jianshu.io/upload_images/1383589-bff0ed695c75dc0d.png)
EventKit 可以帮助你访问用户的日历和提醒事项的信息，日历和提醒事项使用相同的框架来访问一个叫做Calendar database 的数据库。
EventKit not only allows your app to retrieve users’ existing calendar and reminder data, but it also lets your app create new events and reminders for any of their calendars. In addition, EventKit lets users edit and delete their events and reminders (collectively known as “calendar items”). More advanced tasks, such as adding alarms or specifying recurring events, can be achieved with EventKit as well. If a change to the Calendar database occurs from outside of your app, EventKit is able to detect the change by notification so your app can act appropriately. Changes made to calendar items with EventKit are automatically synced to the associated calendar (CalDAV, Exchange, and so on).
This document describes EventKit concepts and common programming tasks. You should read this document if you want to display or edit calendar events and/or reminder data from within your app. EventKit provides limited access to a user’s Calendar database; it does not include everything that would be desired for implementing a full-featured calendar or reminder app, such as adding attendees or accounts.
EventKit  允许你的app 访问已经存在于日历和提醒里面的事件，同时也允许你的app创建一个提醒事件添加到日历或者提醒中。用户可以编辑删除日历和提醒里面的事件。EventKit 还可以添加闹铃，和重复提醒的事件。
注意：EKEvent 和 EKReminder都继承于EKCalendarItem， EKCalendarItem类是日历事件和提醒的抽象超类，此类提供常用的属性和方法来访问日历项的属性，例如设置日历的功能，标题和位置，以及支持附加注释，显示与会者，设置多个闹钟和指定重复规则。EKCalendarItem 的calendarItemIdentifier 是它唯一标示，使用EKCalendarItem *item = [store calendarItemWithIdentifier:identifer];可以通过一个唯一标示获取一个EKCalendarItem对象。
# 二、读取日历和提醒中已存在的事件
⚠️注意：因为 EventStore 是 Calendar database 的数据库引擎，所以应该尽量少的对他进行创建和销毁，所以推荐使用EventStore的时候使用单例模式
[读取日历中的事件官方文档](https://link.jianshu.com?t=https://developer.apple.com/library/content/documentation/DataManagement/Conceptual/EventKitProgGuide/ReadingAndWritingEvents.html)
You can fetch, create, edit, and delete events from a user’s Calendar database using the[EKEventStore](https://link.jianshu.com?t=https://developer.apple.com/reference/eventkit/ekeventstore)class. You can fetch a custom set of events that match a predicate you provide, or you can fetch an individual event by its unique identifier. After you fetch an event, you can access its associated calendar information with the properties of the[EKEvent](https://link.jianshu.com?t=https://developer.apple.com/reference/eventkit/ekevent)class. Like wise, you can modify its calendar information by setting the properties of theEKEventclass. 
你可以使用EKEventStoreclass进行 查询、创建、编辑和删除 用户存于 Calendar database 中的事件，你可以使用谓词来查询出符合条件的事件集或者使用一个唯一标示来查询一个指定的事件。获取事件之后你可以对他进行一些操作。
## 1、使用谓词查询日历里面的某个时间范围内的事件
It’s common to fetch events that fall within a date range. TheEKEventStoremethod[eventsMatchingPredicate:](https://link.jianshu.com?t=https://developer.apple.com/reference/eventkit/ekeventstore/1507183-events)fetches all events that fall within the date range specified in the predicate you provide.（EKEventStore 可以查询谓词所提供的日期范围内的事件）。 
示例代码如下
+(NSArray*)fetchEventsWithStartDate:(NSDate*)startDate endDate:(NSDate*)enDate{
EKEventStore*store = [STCalendarReminderToolshareinstance];
NSPredicate*predicate = [storepredicateForEventsWithStartDate:startDateendDate:enDatecalendars:nil];
NSArray*events = [storeeventsMatchingPredicate:predicate];
NSIntegeri =1;
for(EKEvent*eventinevents) {
NSLog(@"第%zd个提醒%@",i,event);
i++;
}
returnevents;
}
## 2、使用唯一标示来查询事件
Using Unique Identifiers
If you know the event’s unique identifier because you fetched it previously with a predicate, you can use theEKEventStoremethod[eventWithIdentifier:](https://link.jianshu.com?t=https://developer.apple.com/reference/eventkit/ekeventstore/1507490-event)to fetch the event. If it is a recurring event, this method will return the first occurrence of the event. You can get an event’s unique identifier with the[eventIdentifier](https://link.jianshu.com?t=https://developer.apple.com/reference/eventkit/ekevent/1507437-eventidentifier)property.
**⚠️这种方式只能用来查询日历里面的事件**
 示例代码：
+(EKEvent*)fetchEventWithIdentifer:(NSString*)eventidentifer{
EKEventStore*store = [STCalendarReminderToolshareinstance];
EKEvent*event = [storeeventWithIdentifier:eventidentifer];
returnevent;
}
# 三、向日历中添加一个新的事件
[添加新的事件官方文档](https://link.jianshu.com?t=https://developer.apple.com/library/content/documentation/DataManagement/Conceptual/EventKitProgGuide/ReadingAndWritingEvents.html)
Create a new event with the eventWithEventStore: method of the EKEvent class.
You can edit the details of a new event or an event you previously fetched from the Calendar database by setting the event’s corresponding properties. Some of the details you can edit include:
The event’s title with the title property
The event’s start and end dates with the startDate and endDate properties
The calendar with which the event is associated with the calendar property
The alarms associated with the event with the alarms property (see Configuring Alarms for more details)
The event’s recurrence rule, if it is a repeating event, with the recurrenceRules property (see Creating Recurring Events for more details)
示例代码如下：
/**
* STCalendarReminderToolSaveSuccessBlock
* eventIdentifier
*/
typedef void (^STCalendarReminderToolSaveSuccessBlock)(NSString* eventIdentifier);
typedef void (^STCalendarReminderToolSaveFailBlock)(NSError *err);
/** 向日历添加一个事件
* title  事件标题
* notes  事件备注
* location 事件地址
* startDate 开始日期
* endDate   结束日期
* alarms 闹钟
* availability 事件调度
*/
+(void)saveEventWithTitle:(NSString *)title                 
   notes:(NSString *)notes             
    location:(NSString *)location           
     startDate:(NSDate *)startDate           
       endDate:(NSDate *)endDate               
    alarms:(NSArray*)alarms{
URL:(NSURL *)URL
availability:(EKEventAvailability)availability
successBlock:(STCalendarReminderToolSaveSuccessBlock)successBlock
failBlock:(STCalendarReminderToolSaveFailBlock)failBlock{
EKEventStore *store = [STCalendarReminderTool shareinstance];
[store requestAccessToEntityType:EKEntityTypeEvent
completion:
^(BOOL granted, NSError *error) {
dispatch_async(dispatch_get_main_queue(), ^{
if (error) {
//错误信息
return;
}
if (!granted) {
//被用户拒绝，不允许访问日历
return;
}
EKEvent *event = [EKEvent eventWithEventStore:store];
event.title = title;
event.notes = notes;
event.availability = availability;
event.startDate = startDate;
event.endDate = endDate;
event.location  = location;
event.alarms = alarms;
event.calendar = store.defaultCalendarForNewEvents;
event.URL = URL;
NSError *err = nil;
[store saveEvent:event span:EKSpanThisEvent error:&err];
if (!err) {
if (successBlock) {
successBlock(event.eventIdentifier);
}
}else{
if (failBlock) {
failBlock(err);
}
}
NSLog(@"eventIdentifier %@",event.eventIdentifier);
});
}];
}
# 四、删除事件
+(BOOL)deleteEventWithEventIdentifier:(NSString *)eventIdentifier{
EKEventStore *store = [STCalendarReminderTool shareinstance];
EKEvent *event = [store eventWithIdentifier:eventIdentifier];
// YES立即删除事件;否则，更改将批处理，直到调用commit：方法。
return  [store removeEvent:event span:EKSpanThisEvent commit:YES error:nil];
}
# 五、查询提醒
## 5.1 使用谓词查询一个时间范围里的提醒
EKEventStore *store = [STCalendarReminderTool shareinstance];
NSPredicate *predicate = [store predicateForIncompleteRemindersWithDueDateStarting:starDate
ending:endDate
calendars:[store calendarsForEntityType:EKEntityTypeReminder]];
[store fetchRemindersMatchingPredicate:predicate completion:^(NSArray *reminders) {
}];
## 5.2使用谓词 查询所有的提醒
EKEventStore *store      = [STCalendarReminderTool shareinstance];
NSPredicate  *predicate  = [store predicateForRemindersInCalendars:nil];
[store fetchRemindersMatchingPredicate:predicate completion:^(NSArray *reminders) {
NSInteger i = 1;
for (EKReminder *reminder in reminders) {
}
}];
## 5.3 使用唯一标示查询提醒
EKReminder 是继承于EKCalendarItem的（⚠️我这里也没有查到用唯一标示查询返回EKReminder的方法相关资料，我的做法是查询到EKCalendarItem然后把它强转成EKReminder），使用这个方法也可以查询日历里面的事件。
+(EKCalendarItem *)fetchReminderWithIdentier:(NSString *)identifer{
EKEventStore *store = [STCalendarReminderTool shareinstance];
EKCalendarItem *item = [store calendarItemWithIdentifier:identifer];
NSLog(@"item  item %@",item);
return item;
}
# 六、添加新的提醒和删除提醒
## 6.1添加新的提醒
示例代码：
/**
* STCalendarReminderToolSaveSuccessBlock
* eventIdentifier
*/
typedef void (^STCalendarReminderToolSaveSuccessBlock)(NSString* eventIdentifier);
typedef void (^STCalendarReminderToolSaveFailBlock)(NSError *err);
/*
* title  事件标题
* notes  事件备注
* startDate 开始日期
* endDate   结束日期
* alarms 闹钟
* priority 事件调度(1-4 高 5中   6-9低  0 不设置）
* completed
*/
+(void)saveEventIntoReminderWithTitle:(NSString *)title
notes:(NSString *)notes
startDate:(NSDate *)startDate
endDate:(NSDate *)endDate
alarm:(EKAlarm *)alarm
priority:(NSInteger)priority
completed:(BOOL)completed
successBlock:(STCalendarReminderToolSaveSuccessBlock)successBlock
failBlock:(STCalendarReminderToolSaveFailBlock)failBlock{
EKEventStore *store = [STCalendarReminderTool shareinstance];
[store requestAccessToEntityType:EKEntityTypeReminder
completion:
^(BOOL granted, NSError *error) {
dispatch_async(dispatch_get_main_queue(), ^{
if (error) {
if (failBlock) {
failBlock(error);
}
return;
}
if (!granted) {
//被用户拒绝，不允许访问提醒
return;
}
EKReminder *reminder = [EKReminder reminderWithEventStore:store];
[reminder setCalendar:[store defaultCalendarForNewReminders]];
reminder.title       = title;
reminder.notes       = notes;
reminder.completed   = completed;
reminder.priority    = priority;
[reminder addAlarm:alarm];
NSCalendar *calender = [NSCalendar currentCalendar];
[calender setTimeZone:[NSTimeZone systemTimeZone]];
NSInteger flags      = NSCalendarUnitYear | NSCalendarUnitMonth |
NSCalendarUnitDay |NSCalendarUnitHour | NSCalendarUnitMinute |
NSCalendarUnitSecond;
NSDateComponents* startDateComp = [calender components:flags fromDate:startDate];
startDateComp.timeZone = [NSTimeZone systemTimeZone];
reminder.startDateComponents = startDateComp;
NSDateComponents* endDateComp = [calender components:flags fromDate:startDate];
endDateComp.timeZone   = [NSTimeZone systemTimeZone];
reminder.dueDateComponents = endDateComp;
NSError *err;
[store saveReminder:reminder commit:YES error:&err];
if (!err) {
if (successBlock) {
successBlock(reminder.calendarItemIdentifier);
}
}else{
if (failBlock) {
failBlock(err);
}
}
});
}];
}
## 6.2 删除一个提醒
+(BOOL)deleteReminderWithIdentifer:(NSString *)identifier{
EKEventStore *store = [STCalendarReminderTool shareinstance];
EKCalendarItem *item = [store calendarItemWithIdentifier:identifier];
EKReminder *reminder = (EKReminder *)item;
return  [store removeReminder:reminder commit:YES error:nil];
}
# 七、使用系统提供的视图控制器添加、修改或删除事件
先导入#import <EventKitUI/EventKitUI.h>
## 7.1添加一个事件
EKEventEditViewController *editVC = [[EKEventEditViewController alloc] init];
editVC.eventStore = [STCalendarReminderTool shareStoreinstance];
editVC.editViewDelegate = self;
[self presentViewController:editVC animated:YES completion:nil];
遵循EKEventEditViewDelegate 代理
实现代理方法
#pragma -mark EKEventEditViewDelegate
-(void)eventEditViewController:(EKEventEditViewController *)controller didCompleteWithAction:(EKEventEditViewAction)action{
[self dismissViewControllerAnimated:YES completion:^{
}];
}
## 7.2 修改事件
如果要修改一个事件直接把他传给EKEventEditViewController，如下：
EKEventEditViewController *editVC = [[EKEventEditViewController alloc] init];
editVC.eventStore = [STCalendarReminderTool shareStoreinstance];
editVC.editViewDelegate = self;
#### editVC.event      = event;
[self presentViewController:editVC animated:YES completion:nil];
其他操作和新加一个事件的一致。
# 八、总结
写的有不好的地方希望大家指出，我会修正的，大家有什么看不明白的也可以在评论里面提问，我会尽力解答。
[Demo地址](https://link.jianshu.com?t=https://github.com/StartHui/todayWidget.git)
