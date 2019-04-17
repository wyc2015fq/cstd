# [zt] Android中使用List列表 - Grandyang - 博客园







# [[zt] Android中使用List列表](https://www.cnblogs.com/grandyang/p/3990047.html)







### 原文地址：[http://www.vogella.com/tutorials/AndroidListView/article.html](http://www.vogella.com/tutorials/AndroidListView/article.html)



## 1. Android and Lists




### 1.1. Using lists in Android


The display of elements in a list is a very common pattern in mobile applications. The user sees a list of items and can scroll through them. Such an activity is depicted in the following picture.



![Listview example](http://www.vogella.com/tutorials/AndroidListView/images/xlistview_scheme10.png.pagespeed.ic.oP5lBQ-2EL.png)



Typically the user interacts with the list via the action bar, for example, via a refresh button. Individual list items can be selected. This selection can update the action bar or can trigger a detailed screen for the selection. The following graphic sketches that. On the selection of a list item another activity is started.



![Listview example](http://www.vogella.com/tutorials/AndroidListView/images/xlistview_scheme20.png.pagespeed.ic.6PGDNIEZ-T.png)






### 1.2. Views for handling lists


Android provides the `ListView` and the `ExpandableListView` classes which is capable of displaying a scrollable list of items.

The `ExpandableListView` class supports a grouping of items.




### 1.3. Possible input types for lists


The input to the list (items in the list) can be arbitrary Java objects. The adapter extracts the correct data from the data object and assigns this data to the *views* in the row of the `ListView`.

These items are typically called the *data model* of the list. An adapter can receive data as input.




### 1.4. Adapters


An *adapter* manages the data model and adapts it to the individual rows in the list view. An adapter extends the`BaseAdapter` class.

Every line in the list view consists of a layout which can be as complex as you want. A typical line in a list view has an image on the left side and two text lines in the middle as depicted in the following graphic.



![Listview example layout](http://www.vogella.com/tutorials/AndroidListView/images/xlistview_line10.png.pagespeed.ic.V-pIawT-_J.png)



A layout file for a such a line might look like the following.


<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="?android:attr/listPreferredItemHeight"
    android:padding="6dip" >

    <ImageView
        android:id="@+id/icon"
        android:layout_width="wrap_content"
        android:layout_height="fill_parent"
        android:layout_alignParentBottom="true"
        android:layout_alignParentTop="true"
        android:layout_marginRight="6dip"
        android:contentDescription="TODO"
        android:src="@drawable/ic_launcher" />

    <TextView
        android:id="@+id/secondLine"
        android:layout_width="fill_parent"
        android:layout_height="26dip"
        android:layout_alignParentBottom="true"
        android:layout_alignParentRight="true"
        android:layout_toRightOf="@id/icon"
        android:ellipsize="marquee"
        android:singleLine="true"
        android:text="Description"
        android:textSize="12sp" />

    <TextView
        android:layout_width="fill_parent"
        android:layout_height="wrap_content"
        android:layout_above="@id/secondLine"
        android:layout_alignParentRight="true"
        android:layout_alignParentTop="true"
        android:layout_alignWithParentIfMissing="true"
        android:layout_toRightOf="@id/icon"
        android:gravity="center_vertical"
        android:text="Example application"
        android:textSize="16sp" />

</RelativeLayout> 


The adapter would inflate the layout for each row in its `getView()` method and assign the data to the individual views in the row.

The adapter is assigned to the `ListView` via the `setAdapter` method on the `ListView` object.


### Tip

Adapters are not only used by `ListView`, but also by other views which extend`AdapterView` as, for example, `Spinner`, `GridView`, `Gallery` and `StackView`.





### 1.5. Filtering and sorting


Filtering and sorting of the data is handled by the adapter. You need to implement the logic in your custom adapter implementation.




### 1.6. Data updates in the adapter


The `notifyDataSetChanged()` method on the adapter is called if the data has changed or if new data is available.

The `notifyDataSetInvalidated()` method is called if the data is not available anymore.




### 1.7. Listener


To react to selections in the list, set an `OnItemClickListener` to your `ListView`.


listView.setOnItemClickListener(new OnItemClickListener() {
  *@Override*public void onItemClick(AdapterView<?> parent, View view,
    int position, long id) {
    Toast.makeText(getApplicationContext(),
      "Click ListItem Number " + position, Toast.LENGTH_LONG)
      .show();
  }
}); 






## 2. Default adapter




### 2.1. Default platform adapter


Android provides default adapter implementations; the most important are `ArrayAdapter` and `CursorAdapter`.

`ArrayAdapter` can handle data based on `Arrays` or `java.util.List`.

`SimpleCursorAdapter` can handle database related data.




### 2.2. Using ArrayAdapter


The `ArrayAdapter` class can handle a list or array of Java objects as input. Every Java object is mapped to one row. By default, it maps the `toString()` method of the object to a view in the row layout.

You can define the ID of the view in the constructor of the `ArrayAdapter` otherwise the `android.R.id.text1` ID is used as default.

The `ArrayAdapter` class allows to remove all elements in its underlying data structure with the `clear()` method call. You can then add new elements via the `add()` method or a `Collection` via the `addAll()` method.

You can also directly modify the underlying data structure and call the `notifyDataSetChanged()` method on the adapter to notify it about the changes in data.


### Warning

If you want to change the data in your adapter, the underlying data structure must support this operation. This is, for example, the case for the `ArrayList` class, but not for arrays.





### 2.3. ListView example with ArrayAdapter


The following listing shows a layout file called `activity_listviewexampleactivity.xml` which includes a`ListView`.


<ListView xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@+id/listview"
    android:layout_width="wrap_content"
    android:layout_height="wrap_content" /> 


The following example shows the usage of the `ListView` view in an activity. It uses a default layout from the Android platform for the row layout. It also demonstrates the removal of list items and uses animations for the removal.


package com.vogella.android.listview.withanimation;

public class ListViewExampleActivity extends Activity {

  *@Override*protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_listviewexampleactivity);

    final ListView listview = (ListView) findViewById(R.id.listview);
    String[] values = new String[] { "Android", "iPhone", "WindowsMobile",
        "Blackberry", "WebOS", "Ubuntu", "Windows7", "Max OS X",
        "Linux", "OS/2", "Ubuntu", "Windows7", "Max OS X", "Linux",
        "OS/2", "Ubuntu", "Windows7", "Max OS X", "Linux", "OS/2",
        "Android", "iPhone", "WindowsMobile" };

    final ArrayList<String> list = new ArrayList<String>();
    for (int i = 0; i < values.length; ++i) {
      list.add(values[i]);
    }
    final StableArrayAdapter adapter = new StableArrayAdapter(this,
        android.R.layout.simple_list_item_1, list);
    listview.setAdapter(adapter);

    listview.setOnItemClickListener(new AdapterView.OnItemClickListener() {

      *@Override*public void onItemClick(AdapterView<?> parent, final View view,
          int position, long id) {
        final String item = (String) parent.getItemAtPosition(position);
        view.animate().setDuration(2000).alpha(0)
            .withEndAction(new Runnable() {
              *@Override*public void run() {
                list.remove(item);
                adapter.notifyDataSetChanged();
                view.setAlpha(1);
              }
            });
      }

    });
  }

  private class StableArrayAdapter extends ArrayAdapter<String> {

    HashMap<String, Integer> mIdMap = new HashMap<String, Integer>();

    public StableArrayAdapter(Context context, int textViewResourceId,
        List<String> objects) {
      super(context, textViewResourceId, objects);
      for (int i = 0; i < objects.size(); ++i) {
        mIdMap.put(objects.get(i), i);
      }
    }

    *@Override*public long getItemId(int position) {
      String item = getItem(position);
      return mIdMap.get(item);
    }

    *@Override*public boolean hasStableIds() {
      return true;
    }

  }

} 






## 3. Custom adapter implementations




### 3.1. Developing a custom adapter


The `ArrayAdapter` is limited as it supports only the mapping of `toString()` to one view in the row layout. To control the data assignment and to support several views, you have to create your custom adapter implementation.

For this you would extend an existing adapter implementation or subclass the `BaseAdapter` class directly.


### Tip

Frequently you extend `ArrayAdapter` to write a custom adapter, as this is simpler than extending `BaseAdapter` directly.





### 3.2. Preparing a row for the list


The adapter needs to create a layout for each row of the list. The `ListView` instance calls the `getView()` method on the adapter for each data element. In this method the adapter creates the row layout and maps the data to the views in the layout.

This root of the layout is typically a `ViewGroup` (layout manager) and contains several other views , e.g., an `ImageView`and a `TextView`. The following graphic shows a list with different layouts for odd and even rows.



![Adapter provides data and defines the layout per row](http://www.vogella.com/tutorials/AndroidListView/images/xadapter10.png.pagespeed.ic.Wb9PqhtB3Q.png)



Within the `getView()` method you would inflate an XML based layout and then set the content of the individual views based on the Java object for this row. To inflate the XML layout file, you can use the `LayoutInflator` system service.


### Note

This layout inflator service can get accessed via the `getLayoutInflator()` method of the activity or via the `context.getSystemService(Context.LAYOUT_INFLATER_SERVICE)`method call.


After the adapter inflated the layout, it searches for the relevant views in the layout and fills them with the data. The individual elements in the layout can be found via the `findViewById()` method call on the top level view.




### 3.3. Example for a custom adapter


The following code shows an implementation of a custom adapter. This adapter assumes that you have two png files (no.png and yes.png) in one of your `res/drawable` folders. The coding inflates an XML layout file, finds the relevant views in the layout and sets their content based on the input data.


package de.vogella.android.listactivity;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class MySimpleArrayAdapter extends ArrayAdapter<String> {
  private final Context context;
  private final String[] values;

  public MySimpleArrayAdapter(Context context, String[] values) {
    super(context, R.layout.rowlayout, values);
    this.context = context;
    this.values = values;
  }

  *@Override*public View getView(int position, View convertView, ViewGroup parent) {
    LayoutInflater inflater = (LayoutInflater) context
        .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    View rowView = inflater.inflate(R.layout.rowlayout, parent, false);
    TextView textView = (TextView) rowView.findViewById(R.id.label);
    ImageView imageView = (ImageView) rowView.findViewById(R.id.icon);
    textView.setText(values[position]);
    *// change the icon for Windows and iPhone*
    String s = values[position];
    if (s.startsWith("iPhone")) {
      imageView.setImageResource(R.drawable.no);
    } else {
      imageView.setImageResource(R.drawable.ok);
    }

    return rowView;
  }
} 





### 3.4. Updating the data model from the adapter


The row can also contain *views* which interact with the underlying data model via the adapter. For example, you can have a `Checkbox` in your row layout and if the `Checkbox` is selected, the underlying data is changed.





## 4. ListActivity and ListFragment




### 4.1. Default container for using ListView


Android provides specialized fragment and activity classes to simplify list handling.

The classes are the `ListActivity` class if you want to use lists in activities and the the `ListFragment` class if you want to use lists in fragments.

You do not have to assign a layout to these elements. If you do not define a layout, the activity or fragment contains a single `ListView` by default. `ListActivity` and `ListFragment` also allow you to override a `onListItemClick()` method for handling selection of list items.

Both classes allow you to set the adapter to the default `ListView` via the `setListAdapter()` method.

The following example code shows a simple `ListFragment` implementation.


package de.vogella.android.fragments;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.app.ListFragment;

public class MyListFragment extends ListFragment {

  *@Override*public void onActivityCreated(Bundle savedInstanceState) {
    super.onActivityCreated(savedInstanceState);
    String[] values = new String[] { "Android", "iPhone", "WindowsMobile",
        "Blackberry", "WebOS", "Ubuntu", "Windows7", "Max OS X",
        "Linux", "OS/2" };
    ArrayAdapter<String> adapter = new ArrayAdapter<String>(getActivity(),
        android.R.layout.simple_list_item_1, values);
    setListAdapter(adapter);
  }

  *@Override*public void onListItemClick(ListView l, View v, int position, long id) {
    *// do something with the data*
  }
} 


The next example code demonstrates the usage of a `ListActivity`.


package de.vogella.android.listactivity;

import android.app.ListActivity;
import android.os.Bundle;
import android.widget.ArrayAdapter;

public class MyListActivity extends ListActivity {
  public void onCreate(Bundle icicle) {
    super.onCreate(icicle);
    String[] values = new String[] { "Android", "iPhone", "WindowsMobile",
        "Blackberry", "WebOS", "Ubuntu", "Windows7", "Max OS X",
        "Linux", "OS/2" };
    ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
        android.R.layout.simple_list_item_1, values);
    setListAdapter(adapter);
  }
} 





### 4.2. ListActivity and custom layout


You can use a custom layout with `ListActivity` or `ListFragment`. In this case the fragment or activity searches in the provided layout for a `ListView` with the pre-defined `android:id` attribute set to `@android:id/list`. This usage is demonstrated by the following code snippet.


<ListView
  android:id="@android:id/list"
  android:layout_width="match_parent"
  android:layout_height="wrap_content" >
</ListView> 



### Warning

If you do not use this ID or do not include a `ListView` into your layout, the application crashes once you try to display the activity or the fragment.





### 4.3. Placeholder for an empty list


You can also use a view with the `@android:id/empty` ID in your layout. The corresponding activity and fragment shows this view automatically if the `ListView` is empty and hides it otherwise. For example, you could display an error message in such a view.







## 5. Exercise: Using ListView and ListActivity


The following exercise demonstrates how to use a `ListView` in an `ListActivity`. You use the predefined`ArrayAdapter` class and an existing Android layout for the rows.

Create a new Android project called *de.vogella.android.listactivity* with the activity called `MyListActivity`.

Change `MyListActivity` class based on the the following code example. Note that the `setContentView()` method is not used.


package de.vogella.android.listactivity;

import android.app.ListActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

public class MyListActivity extends ListActivity {
  public void onCreate(Bundle icicle) {
    super.onCreate(icicle);
    String[] values = new String[] { "Android", "iPhone", "WindowsMobile",
        "Blackberry", "WebOS", "Ubuntu", "Windows7", "Max OS X",
        "Linux", "OS/2" };
    ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
        android.R.layout.simple_list_item_1, values);
    setListAdapter(adapter);
  }

  *@Override*protected void onListItemClick(ListView l, View v, int position, long id) {
    String item = (String) getListAdapter().getItem(position);
    Toast.makeText(this, item + " selected", Toast.LENGTH_LONG).show();
  }
} 




![ListActivity shows the items](http://www.vogella.com/tutorials/AndroidListView/images/xlistactivity10.png.pagespeed.ic.hJc-76OIyv.png)






## 6. Exercise: ListActivity with own layout


In our example your will define your layout for the rows and use it in your adapter.

Create the `rowlayout.xml` layout file in the `res/layout` folder of the `de.vogella.android.listactivity` project.


<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="wrap_content"
    android:layout_height="wrap_content" >

    <ImageView
        android:id="@+id/icon"
        android:layout_width="22px"
        android:layout_height="22px"
        android:layout_marginLeft="4px"
        android:layout_marginRight="10px"
        android:layout_marginTop="4px"
        android:src="@drawable/ic_launcher" >
    </ImageView>

    <TextView
        android:id="@+id/label"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@+id/label"
        android:textSize="20px" >
    </TextView>

</LinearLayout> 


Change your activity so that is using the new layout.


package de.vogella.android.listactivity;

import android.app.ListActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

public class MyListActivity extends ListActivity {
  public void onCreate(Bundle icicle) {
    super.onCreate(icicle);
    String[] values = new String[] { "Android", "iPhone", "WindowsMobile",
        "Blackberry", "WebOS", "Ubuntu", "Windows7", "Max OS X",
        "Linux", "OS/2" };
    *// use your custom layout*
    ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
        R.layout.rowlayout, R.id.label, values);
    setListAdapter(adapter);
  }

  *@Override*protected void onListItemClick(ListView l, View v, int position, long id) {
    String item = (String) getListAdapter().getItem(position);
    Toast.makeText(this, item + " selected", Toast.LENGTH_LONG).show();
  }
} 




![Displaying a ListView with a own layout](http://www.vogella.com/tutorials/AndroidListView/images/xlistlayout10.png.pagespeed.ic.AySLgw_ZrH.png)






## 7. Tutorial: Implementing your own adapter


The following uses two images "no.png" and "ok.png". I placed it in the "res/drawable-mdpi" folder. You must create your own icons. In case you do not find any icons just copy "icon.png" and use a drawing program to change it a little bit.

Create the class `MySimpleArrayAdapter` which will serve as our adapter.


package de.vogella.android.listactivity;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class MySimpleArrayAdapter extends ArrayAdapter<String> {
  private final Context context;
  private final String[] values;

  public MySimpleArrayAdapter(Context context, String[] values) {
    super(context, R.layout.rowlayout, values);
    this.context = context;
    this.values = values;
  }

  *@Override*public View getView(int position, View convertView, ViewGroup parent) {
    LayoutInflater inflater = (LayoutInflater) context
        .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    View rowView = inflater.inflate(R.layout.rowlayout, parent, false);
    TextView textView = (TextView) rowView.findViewById(R.id.label);
    ImageView imageView = (ImageView) rowView.findViewById(R.id.icon);
    textView.setText(values[position]);
    *// Change the icon for Windows and iPhone*
    String s = values[position];
    if (s.startsWith("Windows7") || s.startsWith("iPhone")
        || s.startsWith("Solaris")) {
      imageView.setImageResource(R.drawable.no);
    } else {
      imageView.setImageResource(R.drawable.ok);
    }

    return rowView;
  }
} 


To use this adapter, change the *activity* to the following.


package de.vogella.android.listactivity;

import android.app.ListActivity;
import android.os.Bundle;

public class MyListActivity extends ListActivity {
  public void onCreate(Bundle icicle) {
    super.onCreate(icicle);
    String[] values = new String[] { "Android", "iPhone", "WindowsMobile",
        "Blackberry", "WebOS", "Ubuntu", "Windows7", "Max OS X",
        "Linux", "OS/2" };
    MySimpleArrayAdapter adapter = new MySimpleArrayAdapter(this, values);
    setListAdapter(adapter);
  }

} 


If you run this example you should get a list with different icons for the certain elements.



![ListView with different elements for each row](http://www.vogella.com/tutorials/AndroidListView/images/xlistlayoutIndividual10.png.pagespeed.ic.E3l_nPH4Ba.png)






## 8. ListViews and performance




### 8.1. Motivation


Performance is especially important on Android as users expect fast reaction times. Compared to desktop computers an Android device is relatively slow from the hardware perspective.

This part describes how to reduce these operations to implement your custom list adapter efficiently. The default Android adapters like `ArrayAdapter` are already performance optimized.




### 8.2. Time consuming operations


Every view which get inflated from an XML layout file will result in a Java object. Inflating layouts and creating Java objects is expensive with regards to time and memory consumption.

In addition using the `findViewById()` method is relatively time consuming, even though it is not as bad as XML inflating.




### 8.3. Avoiding layout inflation and object creation


A `ListView` typically contains more data than the number of displayed rows. If the user scrolls the list, then rows and their associated views are being scrolled out of the visible area. The Java objects which represents the rows can be reused for newly visible rows.

If Android determines that a row is not visible anymore, it allows the `getView()` of the adapter method to reuse the associated view via the `convertView` parameter.

The adapter can assign new data to the views contained in the view hierarchy of the `convertView`. This avoids inflating an XML file and creating new Java objects.

In case Android cannot reuse a row, the Android system passes `null` to the `convertView` parameter. Therefore the adapter implementation needs to check for this.




### 8.4. Holder Pattern


The *View Holder* pattern allows to avoid the `findViewById()` method in the adapter.

A `ViewHolder` class is a static inner class in your adapter which holds references to the relevant views. in your layout. This reference is assigned to the row view as a tag via the `setTag()` method.

If we receive a `convertView` object, we can get the instance of the `ViewHolder` via the `getTag()` method and assign the new attributes to the views via the `ViewHolder` reference.

While this sounds complex this is approximately 15 % faster then using the `findViewById()` method.




### 8.5. Example


The following code shows a performance optimized adapter implementation which reuses existing views and implements the holder pattern.


package de.vogella.android.listactivity;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class MyPerformanceArrayAdapter extends ArrayAdapter<String> {
  private final Activity context;
  private final String[] names;

  static class ViewHolder {
    public TextView text;
    public ImageView image;
  }

  public MyPerformanceArrayAdapter(Activity context, String[] names) {
    super(context, R.layout.rowlayout, names);
    this.context = context;
    this.names = names;
  }

  *@Override*public View getView(int position, View convertView, ViewGroup parent) {
    View rowView = convertView;
    *// reuse views*if (rowView == null) {
      LayoutInflater inflater = context.getLayoutInflater();
      rowView = inflater.inflate(R.layout.rowlayout, null);
      *// configure view holder*
      ViewHolder viewHolder = new ViewHolder();
      viewHolder.text = (TextView) rowView.findViewById(R.id.TextView01);
      viewHolder.image = (ImageView) rowView
          .findViewById(R.id.ImageView01);
      rowView.setTag(viewHolder);
    }

    *// fill data*
    ViewHolder holder = (ViewHolder) rowView.getTag();
    String s = names[position];
    holder.text.setText(s);
    if (s.startsWith("Windows7") || s.startsWith("iPhone")
        || s.startsWith("Solaris")) {
      holder.image.setImageResource(R.drawable.no);
    } else {
      holder.image.setImageResource(R.drawable.ok);
    }

    return rowView;
  }
} 








## 9. Storing the selection of a view


By default a `ListView` has no selection mode active. You can activate it via the `setChoiceMode()` method call. Pass`ListView.CHOICE_MODE_MULTIPLE` for multiple selections or `ListView.CHOICE_MODE_SINGLE` for single selections to this method.

To get the selected items of a `ListView`, use the `getCheckedItemPosition()` for a single selection method or`listView.getCheckedItemPositions()` for multiple selections. . If you have stable ID, you could also use the`getCheckedItemIds()` method to get the selected IDs.

Android already provides a default layout for this: the `android.R.layout.simple_list_item_multiple_choice` layout which contains a configured `CheckedTextView` view.

The following activities demonstrate how to use these selection modes. If you use these modes, the `ListView` stores the selected values. It is not persisted in your data model.


package com.vogella.android.listview.selection.multi;

import android.app.ListActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.vogella.android.listview.selection.R;

public class MainActivity extends ListActivity {

  *@Override*protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    String[] values = new String[] { "a", "b", "c", "d", "e", "f", "g",
        "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s",
        "t", "u", "w", "x", "y", "z" };
    
    ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
        android.R.layout.simple_list_item_multiple_choice, values);
    setListAdapter(adapter);
    getListView().setChoiceMode(ListView.CHOICE_MODE_MULTIPLE);
  }

  *@Override*public boolean onCreateOptionsMenu(Menu menu) {
    getMenuInflater().inflate(R.menu.main, menu);
    return true;
  }

  *@Override*public boolean onOptionsItemSelected(MenuItem item) {
    Toast.makeText(this,
        String.valueOf(getListView().getCheckedItemCount()),
        Toast.LENGTH_LONG).show();
    return true;
  }
} 



package com.vogella.android.listview.selection.single;

import android.app.ListActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;


public class MainActivity extends ListActivity {

  *@Override*protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    String[] values = new String[] { "a", "b", "c", "d", "e", "f", "g",
        "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s",
        "t", "u", "w", "x", "y", "z" };
    
    ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
        android.R.layout.simple_list_item_single_choice, values);
    setListAdapter(adapter);
    getListView().setChoiceMode(ListView.CHOICE_MODE_SINGLE);
  }

  *@Override*public boolean onCreateOptionsMenu(Menu menu) {
    getMenuInflater().inflate(R.menu.main, menu);
    return true;
  }

  *@Override*public boolean onOptionsItemSelected(MenuItem item) {
    Toast.makeText(this,
        String.valueOf(getListView().getCheckedItemCount()),
        Toast.LENGTH_LONG).show();
    return true;
  }
} 





## 10. Contextual action mode for ListViews


The following assumes that you already familiar with the concept of the *ActionBar* and contextual action mode in general. This part will explain how to use contextual action mode for a `ListView` selection.

To assign a contextual action mode to a long click on an individual item, use the method`setOnItemLongClickListener()` on `ListView`. This methods includes information about the selected item. In this method you can start the `ActionMode`.

The following examples demonstrate that. It assumes that you have a menu XML file defined called`rowselection.xml` and that this menu contains one entry with the *`@+id/menuitem1_show`* ID.


package de.vogella.android.listactivity;

import android.app.ListActivity;
import android.os.Bundle;
import android.view.ActionMode;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.Toast;

public class MyListActivityActionbar extends ListActivity {

  protected Object mActionMode;
  public int selectedItem = -1;

  public void onCreate(Bundle icicle) {
    super.onCreate(icicle);
    setContentView(R.layout.main);
    String[] values = new String[] { "Android", "iPhone", "WindowsMobile",
        "Blackberry", "WebOS", "Ubuntu", "Windows7", "Max OS X",
        "Linux", "OS/2", "Ubuntu", "Windows7", "Max OS X", "Linux",
        "OS/2", "Ubuntu", "Windows7", "Max OS X", "Linux", "OS/2" };

    MySimpleArrayAdapter adapter = new MySimpleArrayAdapter(this, values);
    setListAdapter(adapter);

    getListView().setOnItemLongClickListener(new OnItemLongClickListener() {

      *@Override*public boolean onItemLongClick(AdapterView<?> parent, View view,
          int position, long id) {

        if (mActionMode != null) {
          return false;
        }
        selectedItem = position;

        *// start the CAB using the ActionMode.Callback defined above*
        mActionMode = MyListActivityActionbar.this
            .startActionMode(mActionModeCallback);
        view.setSelected(true);
        return true;
      }
    });
  }

  private ActionMode.Callback mActionModeCallback = new ActionMode.Callback() {

    *// called when the action mode is created; startActionMode() was called*public boolean onCreateActionMode(ActionMode mode, Menu menu) {
      *// Inflate a menu resource providing context menu items*
      MenuInflater inflater = mode.getMenuInflater();
      *// assumes that you have "contexual.xml" menu resources*
      inflater.inflate(R.menu.rowselection, menu);
      return true;
    }

    *// the following method is called each time **// the action mode is shown. Always called after**// onCreateActionMode, but**// may be called multiple times if the mode is invalidated.*public boolean onPrepareActionMode(ActionMode mode, Menu menu) {
      return false; *// Return false if nothing is done*
    }

    *// called when the user selects a contextual menu item*public boolean onActionItemClicked(ActionMode mode, MenuItem item) {
      switch (item.getItemId()) {
      case R.id.menuitem1_show:
        show();
        *// the Action was executed, close the CAB*
        mode.finish();
        return true;
      default:
        return false;
      }
    }

    *// called when the user exits the action mode*public void onDestroyActionMode(ActionMode mode) {
      mActionMode = null;
      selectedItem = -1;
    }
  };

  private void show() {
    Toast.makeText(MyListActivityActionbar.this,
        String.valueOf(selectedItem), Toast.LENGTH_LONG).show();
  }

} 


If you start your application and long press on an item in the list, you get your contextual action bar.



![Using the contextual ActionBar on ListView](http://www.vogella.com/tutorials/AndroidListView/images/xlistview_contextual10.png.pagespeed.ic.8T1wsGMXAi.png)






## 11. Implementing undo for an action




### 11.1. When should you offer an undo action?


It is good practice to allow the user to undo critical actions. Such a critical action is, for example, the deletion of list items.

A proven pattern to handle this undo option is to offer a selection at the end of the screen. This selection vanishes after a predefined time or once the user continues to interact with the application.

For example, the Gmail application implements such a behavior.



![Undo button](http://www.vogella.com/tutorials/AndroidListView/images/xundobutton02.png.pagespeed.ic.seQU5MSPPd.jpg)






### 11.2. Example


The following description contains an example for implementing an undo action. It uses an animation to phase out the undo button automatically out after a while.

For this example create a new project called *com.vogella.android.userinterface.undo* based on the *BlankTemplate*template.

Create the following layout for your *activity*. It uses a `FrameLayout` to show two different parts of the user interface. The button bar is initially hidden. The button uses a drawable. Either add such a drawable to your project or remove the reference.


<FrameLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent" >

    <RelativeLayout
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        tools:context=".MainActivity" >

        <ListView
            android:id="@+id/listview"
            android:layout_width="match_parent"
            android:layout_height="match_parent" >
        </ListView>
    </RelativeLayout>

    <LinearLayout
        android:id="@+id/undobar"
        android:visibility="gone"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_gravity="bottom|center_horizontal"
        android:layout_margin="20dp"
        android:alpha="100"
        android:background="#808080"
        android:dividerPadding="11dp"
        android:padding="4dp" >

        <TextView
            android:id="@+id/undobar_message"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="Deleted"
            android:textAppearance="?android:attr/textAppearanceMedium"
            android:textColor="#fff" />

        <Button
            android:id="@+id/undobar_button"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_marginLeft="30dp"
            android:onClick="onClick"
            android:background="#808080"
            android:drawableLeft="@drawable/ic_undobar_undo"
            android:text="Undo"
            android:textAppearance="?android:attr/textAppearanceMedium"
            android:textColor="#fff" />
    </LinearLayout>

</FrameLayout> 


Change your *activity* so that it is similar to the following code. The Android project wizard in Eclipse already generated an `ActionBar` entry. This entry is used in the following code. If in doubt, create your own `ActionBar` entry.


package com.vogella.android.userinterface.undo;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.vogella.android.actionbar.undo.R;

public class MainActivity extends Activity {

  private View viewContainer;

  *@Override*protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    ListView l = (ListView) findViewById(R.id.listview);
    String[] values = new String[] { "Ubuntu", "Android", "iPhone",
        "Windows", "Ubuntu", "Android", "iPhone", "Windows" };
    ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
        android.R.layout.simple_list_item_1, values);
    viewContainer = findViewById(R.id.undobar);
    l.setAdapter(adapter);
  }

  *@Override*public boolean onCreateOptionsMenu(Menu menu) {
    *// Inflate the menu; this adds items to the action bar if it is present.*
    getMenuInflater().inflate(R.menu.activity_main, menu);
    return true;
  }

  *@Override*public boolean onOptionsItemSelected(MenuItem item) {
    showUndo(viewContainer);
    return true;
  }

  public void onClick(View view) {
    Toast.makeText(this, "Deletion undone", Toast.LENGTH_LONG).show();
    viewContainer.setVisibility(View.GONE);
  }

  public static void showUndo(final View viewContainer) {
    viewContainer.setVisibility(View.VISIBLE);
    viewContainer.setAlpha(1);
    viewContainer.animate().alpha(0.4f).setDuration(5000)
        .withEndAction(new Runnable() {

          *@Override*public void run() {
            viewContainer.setVisibility(View.GONE);
          }
        });

  }
} 


If you select the entry in the *ActionBar*, the button bar becomes visible for 5 seconds.



![Undo button](http://www.vogella.com/tutorials/AndroidListView/images/xundobutton10.png.pagespeed.ic.smlx0edmY6.png)







## 12. Performance Optimization


The following will implement a performance optimized version of the adapter from the previous example.

Create the following `MyPerformanceArrayAdapter` class.


package de.vogella.android.listactivity;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class MyPerformanceArrayAdapter extends ArrayAdapter<String> {
  private final Activity context;
  private final String[] names;

  static class ViewHolder {
    public TextView text;
    public ImageView image;
  }

  public MyPerformanceArrayAdapter(Activity context, String[] names) {
    super(context, R.layout.rowlayout, names);
    this.context = context;
    this.names = names;
  }

  *@Override*public View getView(int position, View convertView, ViewGroup parent) {
    View rowView = convertView;
    *// reuse views*if (rowView == null) {
      LayoutInflater inflater = context.getLayoutInflater();
      rowView = inflater.inflate(R.layout.rowlayout, null);
      *// configure view holder*
      ViewHolder viewHolder = new ViewHolder();
      viewHolder.text = (TextView) rowView.findViewById(R.id.TextView01);
      viewHolder.image = (ImageView) rowView
          .findViewById(R.id.ImageView01);
      rowView.setTag(viewHolder);
    }

    *// fill data*
    ViewHolder holder = (ViewHolder) rowView.getTag();
    String s = names[position];
    holder.text.setText(s);
    if (s.startsWith("Windows7") || s.startsWith("iPhone")
        || s.startsWith("Solaris")) {
      holder.image.setImageResource(R.drawable.no);
    } else {
      holder.image.setImageResource(R.drawable.ok);
    }

    return rowView;
  }
} 


Use your new adapter in your *activity*. If you run the application it should look the same but it will be much faster, especially for large datasets.


package de.vogella.android.listactivity;

import android.app.ListActivity;
import android.os.Bundle;

public class MyListActivity extends ListActivity {
  public void onCreate(Bundle icicle) {
    super.onCreate(icicle);
    String[] values = new String[] { "Android", "iPhone", "WindowsMobile",
        "Blackberry", "WebOS", "Ubuntu", "Windows7", "Max OS X",
        "Linux", "OS/2" };
    setListAdapter(new MyPerformanceArrayAdapter(this, values));
  }

} 





## 13. Tutorial: How to display two items in a ListView


You can use the `SimpleAdapter` class to show the data of two elements. This class expects a Array of Strings (`from`data) in which the fields of the input data are defined. It also requires a Array of ints which defines the IDs of the widgets in the layout for the row to which these fields are mapped.

The actual data is then a list of Maps. The Map defines for each field in the from data a value.

The following shows an example which reuses an predefined layout from Android for the row.


package de.vogella.android.listactivity;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import android.app.ListActivity;
import android.os.Bundle;
import android.widget.SimpleAdapter;

public class MyTwoListItemsActivity extends ListActivity {
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    ArrayList<Map<String, String>> list = buildData();
    String[] from = { "name", "purpose" };
    int[] to = { android.R.id.text1, android.R.id.text2 };

    SimpleAdapter adapter = new SimpleAdapter(this, list,
        android.R.layout.simple_list_item_2, from, to);
    setListAdapter(adapter);
  }

  private ArrayList<Map<String, String>> buildData() {
    ArrayList<Map<String, String>> list = new ArrayList<Map<String, String>>();
    list.add(putData("Android", "Mobile"));
    list.add(putData("Windows7", "Windows7"));
    list.add(putData("iPhone", "iPhone"));
    return list;
  }

  private HashMap<String, String> putData(String name, String purpose) {
    HashMap<String, String> item = new HashMap<String, String>();
    item.put("name", name);
    item.put("purpose", purpose);
    return item;
  }
  
} 







## 14. Selecting multiple items in the ListView




### 14.1. Interaction between the model and Listview


Frequently you need to select items in your `ListView`. As the row of the `ListView` are getting recycled you cannot store the selection on the `View` level.



![ListView with Interaction and full model](http://www.vogella.com/tutorials/AndroidListView/images/xinteractive10.png.pagespeed.ic.pC6EMcFIfe.jpg)



Selection is just one possible example but you can imange other interaction between your row and model.

To persist the selection you have to update your data model with the selected state.

To update the data model in your `ListView` you define your own `Adapter` class. In this adapter class you attach a listener to the `View` which is responsible for selecting the model element. If selected you update the state in the model which you can add as a tag to the View to have access to it.

The following example demonstrates how to use standard Java object and how to interact from the `Views` with the model.




### 14.2. Tutorial: Domain Model and Rows interaction


Continue to use the `de.vogella.android.listactivity` project.

Create the following `Model` which hold the name and the information if this element is currently selected.


package de.vogella.android.listactivity;

public class Model {

  private String name;
  private boolean selected;

  public Model(String name) {
    this.name = name;
    selected = false;
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public boolean isSelected() {
    return selected;
  }

  public void setSelected(boolean selected) {
    this.selected = selected;
  }

} 


Create the following new layout file called `rowbuttonlayout.xml`.


<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="wrap_content" >

    <TextView
        android:id="@+id/label"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@+id/label"
        android:textSize="30px" >
    </TextView>

    <CheckBox
        android:id="@+id/check"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentRight="true"
        android:layout_marginLeft="4px"
        android:layout_marginRight="10px" >
    </CheckBox>

</RelativeLayout> 


Create the following `Adapter`. This adapter adds a listener on the `Checkbox` view . If the checkbox is selected the underlying data of the model is changed. `Checkbox` gets the corresponding model element assigned via the `getTag()`method.


package de.vogella.android.listactivity;

import java.util.List;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.TextView;

public class InteractiveArrayAdapter extends ArrayAdapter<Model> {

  private final List<Model> list;
  private final Activity context;

  public InteractiveArrayAdapter(Activity context, List<Model> list) {
    super(context, R.layout.rowbuttonlayout, list);
    this.context = context;
    this.list = list;
  }

  static class ViewHolder {
    protected TextView text;
    protected CheckBox checkbox;
  }

  *@Override*public View getView(int position, View convertView, ViewGroup parent) {
    View view = null;
    if (convertView == null) {
      LayoutInflater inflator = context.getLayoutInflater();
      view = inflator.inflate(R.layout.rowbuttonlayout, null);
      final ViewHolder viewHolder = new ViewHolder();
      viewHolder.text = (TextView) view.findViewById(R.id.label);
      viewHolder.checkbox = (CheckBox) view.findViewById(R.id.check);
      viewHolder.checkbox
          .setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            *@Override*public void onCheckedChanged(CompoundButton buttonView,
                boolean isChecked) {
              Model element = (Model) viewHolder.checkbox
                  .getTag();
              element.setSelected(buttonView.isChecked());

            }
          });
      view.setTag(viewHolder);
      viewHolder.checkbox.setTag(list.get(position));
    } else {
      view = convertView;
      ((ViewHolder) view.getTag()).checkbox.setTag(list.get(position));
    }
    ViewHolder holder = (ViewHolder) view.getTag();
    holder.text.setText(list.get(position).getName());
    holder.checkbox.setChecked(list.get(position).isSelected());
    return view;
  }
} 


Finally change your *activity* to the following.


package de.vogella.android.listactivity;

import java.util.ArrayList;
import java.util.List;

import android.app.ListActivity;
import android.os.Bundle;
import android.widget.ArrayAdapter;

public class MyList extends ListActivity {
/** Called when the activity is first created. */
public void onCreate(Bundle icicle) {
    super.onCreate(icicle);
    *// create an array of Strings, that will be put to our ListActivity*
    ArrayAdapter<Model> adapter = new InteractiveArrayAdapter(this,
        getModel());
    setListAdapter(adapter);
  }

  private List<Model> getModel() {
    List<Model> list = new ArrayList<Model>();
    list.add(get("Linux"));
    list.add(get("Windows7"));
    list.add(get("Suse"));
    list.add(get("Eclipse"));
    list.add(get("Ubuntu"));
    list.add(get("Solaris"));
    list.add(get("Android"));
    list.add(get("iPhone"));
    *// Initially select one of the items*
    list.get(1).setSelected(true);
    return list;
  }

  private Model get(String s) {
    return new Model(s);
  }

} 


If you start your app you should be able to flag items. These changes will be reflected in your model.





## 15. Implementing an expandable ListView




### 15.1. ExpandableListView


The `ExpandableListView` is similar to `ListView` but allow you to define groups and details for this group.`ExpandableListView` expects and adapter of type `BaseExpandableListAdapter`.

In this case you have to define two layouts, one for the group and another one for the details row.




### 15.2. ExpandableListView example


In the following example you create an expandable listview similar to the following screenshot.



![Screenshot of ExpandableListView](http://www.vogella.com/tutorials/AndroidListView/images/xexpandablelistview10.png.pagespeed.ic.MQEgKpTNd6.jpg)



Create a project called *com.vogella.android.listview.expandable* with the activity called `MainActivity`.

Create or adjust the following layout files. First `layout/activity_main.xml`.


<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:id="@+id/LinearLayout1"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context=".MainActivity" >

    <ExpandableListView
        android:id="@+id/listView"
        android:layout_width="match_parent"
        android:layout_height="wrap_content" >
    </ExpandableListView>

</LinearLayout> 


Afterwards create `layout/listrow_group.xml`.


<CheckedTextView xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:id="@+id/textView1"
    android:layout_width="wrap_content"
    android:layout_height="?android:attr/listPreferredItemHeight"
    android:layout_marginLeft="8dp"
    android:drawableRight="@drawable/ic_launcher"
    android:gravity="left"
    android:paddingLeft="32dp"
    android:paddingTop="8dp"
    android:text="Test"
    android:textSize="14sp"
    android:textAlignment="textEnd"
    android:textStyle="bold" /> 


The last required layout is `layout/listrow_details.xml`.


<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="40dp"
    android:clickable="true"
    android:orientation="vertical"
    android:paddingLeft="40dp"
    tools:context=".MainActivity" >

    <TextView
        android:id="@+id/textView1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:drawableLeft="@drawable/ic_launcher"
        android:drawablePadding="5dp"
        android:gravity="center_vertical"
        android:text="@string/hello_world"
        android:textSize="14sp"
        android:textStyle="bold" >
    </TextView>

    <View
        android:layout_width="match_parent"
        android:layout_height="1dp"
        android:background="@android:color/black" />

</LinearLayout> 


Create the following class which hold your domain model for the `ExpandableListView`.


package com.vogella.android.listview.expandable;

import java.util.ArrayList;
import java.util.List;

public class Group {

  public String string;
  public final List<String> children = new ArrayList<String>();

  public Group(String string) {
    this.string = string;
  }

} 


Finally create the adapter as described by the following listing and change the activity to the code provided below.


package com.vogella.android.listview.expandable;

import android.app.Activity;
import android.util.SparseArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.CheckedTextView;
import android.widget.TextView;
import android.widget.Toast;

public class MyExpandableListAdapter extends BaseExpandableListAdapter {

  private final SparseArray<Group> groups;
  public LayoutInflater inflater;
  public Activity activity;

  public MyExpandableListAdapter(Activity act, SparseArray<Group> groups) {
    activity = act;
    this.groups = groups;
    inflater = act.getLayoutInflater();
  }

  *@Override*public Object getChild(int groupPosition, int childPosition) {
    return groups.get(groupPosition).children.get(childPosition);
  }

  *@Override*public long getChildId(int groupPosition, int childPosition) {
    return 0;
  }

  *@Override*public View getChildView(int groupPosition, final int childPosition,
      boolean isLastChild, View convertView, ViewGroup parent) {
    final String children = (String) getChild(groupPosition, childPosition);
    TextView text = null;
    if (convertView == null) {
      convertView = inflater.inflate(R.layout.listrow_details, null);
    }
    text = (TextView) convertView.findViewById(R.id.textView1);
    text.setText(children);
    convertView.setOnClickListener(new OnClickListener() {
      *@Override*public void onClick(View v) {
        Toast.makeText(activity, children,
            Toast.LENGTH_SHORT).show();
      }
    });
    return convertView;
  }

  *@Override*public int getChildrenCount(int groupPosition) {
    return groups.get(groupPosition).children.size();
  }

  *@Override*public Object getGroup(int groupPosition) {
    return groups.get(groupPosition);
  }

  *@Override*public int getGroupCount() {
    return groups.size();
  }

  *@Override*public void onGroupCollapsed(int groupPosition) {
    super.onGroupCollapsed(groupPosition);
  }

  *@Override*public void onGroupExpanded(int groupPosition) {
    super.onGroupExpanded(groupPosition);
  }

  *@Override*public long getGroupId(int groupPosition) {
    return 0;
  }

  *@Override*public View getGroupView(int groupPosition, boolean isExpanded,
      View convertView, ViewGroup parent) {
    if (convertView == null) {
      convertView = inflater.inflate(R.layout.listrow_group, null);
    }
    Group group = (Group) getGroup(groupPosition);
    ((CheckedTextView) convertView).setText(group.string);
    ((CheckedTextView) convertView).setChecked(isExpanded);
    return convertView;
  }

  *@Override*public boolean hasStableIds() {
    return false;
  }

  *@Override*public boolean isChildSelectable(int groupPosition, int childPosition) {
    return false;
  }
} 



package com.vogella.android.listview.expandable;

import java.util.ArrayList;

import android.app.Activity;
import android.os.Bundle;
import android.util.SparseArray;
import android.view.Menu;
import android.widget.ExpandableListView;

public class MainActivity extends Activity {
  *// more efficient than HashMap for mapping integers to objects*
  SparseArray<Group> groups = new SparseArray<Group>();

  *@Override*protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    createData();
    ExpandableListView listView = (ExpandableListView) findViewById(R.id.listView);
    MyExpandableListAdapter adapter = new MyExpandableListAdapter(this,
        groups);
    listView.setAdapter(adapter);
  }

  public void createData() {
    for (int j = 0; j < 5; j++) {
      Group group = new Group("Test " + j);
      for (int i = 0; i < 5; i++) {
        group.children.add("Sub Item" + i);
      }
      groups.append(j, group);
    }
  }

} 






## 16. Tutorial: Miscellaneous




### 16.1. Adding a longclick listener to the list items


You can also add a `LongItemClickListener` to the `View`. For this receive the `ListView` via the `getListVIew()` method and set the `LongItemClickListener` via the setOnItemLongClickListener() method.


package de.vogella.android.listactivity;

import android.app.ListActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

public class MyList extends ListActivity {
/** Called when the activity is first created. */
public void onCreate(Bundle icicle) {
    super.onCreate(icicle);
    *// create an array of Strings, that will be put to our ListActivity*
    String[] names = new String[] { "Linux", "Windows7", "Eclipse", "Suse",
        "Ubuntu", "Solaris", "Android", "iPhone" };
    ArrayAdapter<String> adapter = new MyPerformanceArrayAdapter(this, names);
    setListAdapter(adapter);
    ListView list = getListView();
    list.setOnItemLongClickListener(new OnItemLongClickListener() {

      *@Override*public boolean onItemLongClick(AdapterView<?> parent, View view,
          int position, long id) {
        Toast.makeText(MyList.this,
            "Item in position " + position + " clicked",
            Toast.LENGTH_LONG).show();
        *// Return true to consume the click event. In this case the**// onListItemClick listener is not called anymore.*return true;
      }
    });
  }

  *@Override*protected void onListItemClick(ListView l, View v, int position, long id) {
    super.onListItemClick(l, v, position, id);
    *// Get the item that was clicked*
    Object o = this.getListAdapter().getItem(position);
    String keyword = o.toString();
    Toast.makeText(this, "You selected: " + keyword, Toast.LENGTH_SHORT)
        .show();

  }

} 





### 16.2. Header and Footer


You can of course put arbitrary `Views` elements around your ListView. For example you can define a layout with two`TextViews` and a `ListView` between them. In this case the two TextViews will always be visible above the List (header) and the other will be visible below the ListView. If you want to display a list header or list footer only at the see the beginning or end of the list you can use the `addHeaderView()` method or `addFooterView()` method on the`ListView` class.


package de.vogella.android.listactivity;

import android.app.ListActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class MyList extends ListActivity {
/** Called when the activity is first created. */
public void onCreate(Bundle icicle) {
    super.onCreate(icicle);
    *// create an array of Strings, that will be put to our ListActivity*
    String[] names = new String[] { "Linux", "Windows7", "Eclipse", "Suse",
        "Ubuntu", "Solaris", "Android", "iPhone", "Linux", "Windows7",
        "Eclipse", "Suse", "Ubuntu", "Solaris", "Android", "iPhone" };
    View header = getLayoutInflater().inflate(R.layout.header, null);
    View footer = getLayoutInflater().inflate(R.layout.footer, null);
    ListView listView = getListView();
    listView.addHeaderView(header);
    listView.addFooterView(footer);
    setListAdapter(new ArrayAdapter<String>(this,
        android.R.layout.simple_list_item_single_choice,
        android.R.id.text1, names));

  }
} 






## 17. SimpleCursorAdapter


In case you work with a content provider or directly with the database you can use the `SimpleCursorAdapter` to define the data for your `ListView`. The following will demonstrates how to access the Contacts ContentProvider.

Create a new Android project called "de.vogella.android.listactivity.cursor" with the *activity* called *MyListActivity*. Change `MyListActivity` to the following.


package de.vogella.android.listactivity.cursor;

import android.app.ListActivity;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.widget.ListAdapter;
import android.widget.SimpleCursorAdapter;

public class MyListActivity extends ListActivity {
/** Called when the activity is first created. */
*@Override*public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    Cursor mCursor = getContacts();
    startManagingCursor(mCursor);
    *// now create a new list adapter bound to the cursor.**// SimpleListAdapter is designed for binding to a Cursor.*
    ListAdapter adapter = new SimpleCursorAdapter(this, *// Context.*
        android.R.layout.two_line_list_item, *// Specify the row template**// to use (here, two**// columns bound to the**// two retrieved cursor**// rows).*
        mCursor, *// Pass in the cursor to bind to.**// Array of cursor columns to bind to.*new String[] { ContactsContract.Contacts._ID,
            ContactsContract.Contacts.DISPLAY_NAME },
        *// Parallel array of which template objects to bind to those**// columns.*new int[] { android.R.id.text1, android.R.id.text2 });

    *// Bind to our new adapter.*
    setListAdapter(adapter);
  }

  private Cursor getContacts() {
    *// Run query*
    Uri uri = ContactsContract.Contacts.CONTENT_URI;
    String[] projection = new String[] { ContactsContract.Contacts._ID,
        ContactsContract.Contacts.DISPLAY_NAME };
    String selection = ContactsContract.Contacts.IN_VISIBLE_GROUP + " = '"
        + ("1") + "'";
    String[] selectionArgs = null;
    String sortOrder = ContactsContract.Contacts.DISPLAY_NAME
        + " COLLATE LOCALIZED ASC";

    return managedQuery(uri, projection, selection, selectionArgs,
        sortOrder);
  }

} 


Make sure you give your application the permission to read the contacts. (Uses Permissions "android.permission.READ_CONTACTS" in AndroidManifest.xml)




## 18. Additional Open Source libraries


Sometimes having to press a refresh button on the ActionBar to refresh data can be annoying for the user. Chris Banes has implemented an Open Source library to implement the pull to refresh pattern for a `Listview`. https://github.com/chrisbanes/Android-PullToRefresh.

Also you may want to use the swipe to dismiss gesture to delete items from a ListView. Roman Nurik provides an example for this at [Android swipe-to-dismiss library](https://github.com/romannurik/android-swipetodismiss) which Jake Wharton backported to earlier Android releases at[SwipeToDismissNOA](https://github.com/JakeWharton/SwipeToDismissNOA).



















