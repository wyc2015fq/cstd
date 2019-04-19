# startActivityForResult的用法 - walkabc - 博客园
# [startActivityForResult的用法](https://www.cnblogs.com/hitfire/articles/3382490.html)
### Starting Activities and Getting Results
The `[startActivity(Intent)](http://developer.android.com/reference/android/app/Activity.html#startActivity(android.content.Intent))` method is used to start a new activity, which will be placed at the top of the activity stack. It takes a single argument, an`[Intent](http://developer.android.com/reference/android/content/Intent.html)`, which describes the activity to be executed.
Sometimes you want to get a result back from an activity when it ends. For example, you may start an activity that lets the user pick a person in a list of contacts; when it ends, it returns the person that was selected. To do this, you call the `[startActivityForResult(Intent, int)](http://developer.android.com/reference/android/app/Activity.html#startActivityForResult(android.content.Intent, int))` version with a second integer parameter identifying the call. The result will come back through your `[onActivityResult(int, int, Intent)](http://developer.android.com/reference/android/app/Activity.html#onActivityResult(int, int, android.content.Intent))` method.
When an activity exits, it can call `[setResult(int)](http://developer.android.com/reference/android/app/Activity.html#setResult(int))` to return data back to its parent. It must always supply a result code, which can be the standard results RESULT_CANCELED, RESULT_OK, or any custom values starting at RESULT_FIRST_USER. In addition, it can optionally return back an Intent containing any additional data it wants. All of this information appears back on the parent's `Activity.onActivityResult()`, along with the integer identifier it originally supplied.
If a child activity fails for any reason (such as crashing), the parent activity will receive a result with the code RESULT_CANCELED.
```
public class MyActivity extends Activity {
     ...
     static final int PICK_CONTACT_REQUEST = 0;
     protected boolean onKeyDown(int keyCode, KeyEvent event) {
         if (keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
             // When the user center presses, let them pick a contact.
             startActivityForResult(
                 new Intent(Intent.ACTION_PICK,
                 new Uri("content://contacts")),
                 PICK_CONTACT_REQUEST);
            return true;
         }
         return false;
     }
     protected void onActivityResult(int requestCode, int resultCode,
             Intent data) {
         if (requestCode == PICK_CONTACT_REQUEST) {
             if (resultCode == RESULT_OK) {
                 // A contact was picked.  Here we will just display it
                 // to the user.
                 startActivity(new Intent(Intent.ACTION_VIEW, data));
             }
         }
     }
 }
```

