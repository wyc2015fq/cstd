# 安卓开发37：自定义的HorizontalScrollView类，使其pageScroll的时候焦点不选中 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年09月12日 17:31:00[initphp](https://me.csdn.net/initphp)阅读数：2131
所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









自定义一个HorizontalScrollView类，主要为了让这个HorizontalScrollView不能鼠标点击，不能左右按键，并且没有焦点。





```java
public class ImageMoveHorizontalScrollView extends HorizontalScrollView {

    private boolean    mSmoothScrollingEnabled = true;

    private final Rect mTempRect               = new Rect();

    public ImageMoveHorizontalScrollView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public ImageMoveHorizontalScrollView(Context context) {
        super(context);
    }

    /**
     * 关闭鼠标点击的效果，重写该方法
     */
    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        return false;
    }

    /**
     * 关闭左右按键效果，重写该方法
     */
    @Override
    public boolean executeKeyEvent(KeyEvent event) {
        return false;
    }

    /**
     * 去除焦点选中
     */
    @Override
    public boolean pageScroll(int direction) {
        boolean right = direction == View.FOCUS_RIGHT;
        int width = getWidth();

        if (right) {
            mTempRect.left = getScrollX() + width;
            int count = getChildCount();
            if (count > 0) {
                View view = getChildAt(0);
                if (mTempRect.left + width > view.getRight()) {
                    mTempRect.left = view.getRight() - width;
                }
            }
        } else {
            mTempRect.left = getScrollX() - width;
            if (mTempRect.left < 0) {
                mTempRect.left = 0;
            }
        }
        mTempRect.right = mTempRect.left + width;

        return scrollAndFocus(direction, mTempRect.left, mTempRect.right);
    }

    private boolean scrollAndFocus(int direction, int left, int right) {
        boolean handled = true;

        int width = getWidth();
        int containerLeft = getScrollX();
        int containerRight = containerLeft + width;
        boolean goLeft = direction == View.FOCUS_LEFT;
//主要在这边，注释掉下面的代码
        //        View newFocused = findFocusableViewInBounds(goLeft, left, right);
        //        if (newFocused == null) {
        //            newFocused = this;
        //        }

        if (left >= containerLeft && right <= containerRight) {
            handled = false;
        } else {
            int delta = goLeft ? (left - containerLeft) : (right - containerRight);
            doScrollX(delta);
        }
        //去除 滚动后 foucus的
        //        if (newFocused != findFocus())
        //            newFocused.requestFocus(direction);

        return handled;
    }

    /**
     * Smooth scroll by a X delta
     * @param delta the number of pixels to scroll by on the X axis
     */
    private void doScrollX(int delta) {
        if (delta != 0) {
            if (mSmoothScrollingEnabled) {
                smoothScrollBy(delta, 0);
            } else {
                scrollBy(delta, 0);
            }
        }
    }

}
```




使用：



```java
sc.pageScroll(View.FOCUS_RIGHT);  //向右翻一页
```







