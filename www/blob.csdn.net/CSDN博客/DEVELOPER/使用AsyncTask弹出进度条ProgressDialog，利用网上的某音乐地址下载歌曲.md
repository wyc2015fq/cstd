# 使用AsyncTask弹出进度条ProgressDialog，利用网上的某音乐地址下载歌曲 - DEVELOPER - CSDN博客





2014年03月25日 16:18:36[学术袁](https://me.csdn.net/u012827205)阅读数：1406








**有些时候，在你编写程序的时候某种控件不经常使用就会忘记怎么使用。就像AsyncTask、ProgressDialog、网络下载数据等。所以，我就有意的把这些东西记录下来，以免它们从我的生活中淡出！<这个图片不是这个程序运行的效果，但是大概就是这样的>**

![](https://img-blog.csdn.net/20140325181100515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







```java

```
***    /**     * 在使用抽象类"AsyncTask"时候，通过继承它来重写其中<1>onPreExecute、<2>doInBackground、     * <3>onProgressUpdate、<4>onPostExecute四个方法；     * 并且在AsyncTask<Void, Integer, Void> 中的三个参数分别对应方法<2><3><4>中的参数。     *      * 特别注意，使用该种方式也要进行信息的交流，在执行后台耗时操作时 使用mAsynctask.this.publishProgress(count);     * 传递信息。     *     */***



```java
public class MainActivity extends Activity {

	private AsyncTask<Void, Integer, Void> at;
	private ProgressDialog pd;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
	}

	public void onClick(View v) {
		Void params[] = null;
		at = new AsyncTask<Void, Integer, Void>() {
			protected void onPreExecute() {
				pd = new ProgressDialog(MainActivity.this);
				pd.setMax(200);
				pd.setTitle("提示");
				pd.setMessage("下载中，请稍后..");
				pd.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
				pd.show();
			}

			@Override
			protected Void doInBackground(Void... params) {

				String spec = "http://zhangmenshiting.baidu.com/data2/music/90350586/8930817375600128.mp3?xcode=544df7e8700d7942344de01ecf439f0a24b7b177cb0d29e2";
				int total = 0;
				// TODO Auto-generated method stub
				try {
					HttpURLConnection huc = (HttpURLConnection) new URL(spec)
							.openConnection();
					huc.setRequestMethod("GET");
					Long musicLengh = (long) huc.getContentLength();
					InputStream in = huc.getInputStream();
					FileOutputStream fos = new FileOutputStream(new File(
							"/mnt/sdcard/", "huahua"));
					byte[] b = new byte[512];
					int byte_count = 0;
					while (-1 != (byte_count = in.read(b))) {
						fos.write(b, 0, byte_count);
						total += byte_count;
						publishProgress((int) (120 * total / musicLengh));
					}
				} catch (Exception e) {
				}
				;
				return null;
			}

			@Override
			protected void onProgressUpdate(Integer... values) {
				// TODO Auto-generated method stub
				pd.setProgress(values[0]);
				super.onProgressUpdate(values);
			}

			@Override
			protected void onPostExecute(Void result) {
				// TODO Auto-generated method stub
				pd.dismiss();
				Toast.makeText(getApplicationContext(), "下载完毕", 200).show();
				super.onPostExecute(result);
			}

		}.execute(params);

	}

}
```







