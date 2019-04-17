# Android 使用 ImageView、SurfaceView、自定义 View 绘制图片（附源码简析） - 我的学习笔记 - CSDN博客





2018年07月15日 01:15:02[zouzhiheng](https://me.csdn.net/u011330638)阅读数：978








打算根据 @卢俊 写的 [音视频开发入门指南](https://zhuanlan.zhihu.com/p/28518637) 写一个系列博客，这是第一篇，也是文章 [音视频开发入门指南](https://zhuanlan.zhihu.com/p/28518637) 任务列表中的第一个。

## ImageView

ImageView 绘制图片就很简单了，无论是 xml 还是 Java 代码，都只需要设置一个属性即可。但我们可以看看 ImageView 的源码实现，来学习一下 ImageView 绘制图片的流程。

直接看 onDraw() 即可：

```
@Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        if (mDrawable == null) {
            return; // couldn't resolve the URI
        }

        if (mDrawableWidth == 0 || mDrawableHeight == 0) {
            return;     // nothing to draw (empty bounds)
        }

		// 不需要变换坐标，直接绘制即可
        if (mDrawMatrix == null && mPaddingTop == 0 && mPaddingLeft == 0) {
            mDrawable.draw(canvas);
        } else {
            final int saveCount = canvas.getSaveCount();
            canvas.save(); // 保存 canvas

			// 坐标变换
            if (mCropToPadding) {
                final int scrollX = mScrollX;
                final int scrollY = mScrollY;
                canvas.clipRect(scrollX + mPaddingLeft, scrollY + mPaddingTop,
                        scrollX + mRight - mLeft - mPaddingRight,
                        scrollY + mBottom - mTop - mPaddingBottom);
            }

            canvas.translate(mPaddingLeft, mPaddingTop);

            if (mDrawMatrix != null) {
                canvas.concat(mDrawMatrix);
            }
            // 绘制图片
            mDrawable.draw(canvas);
            // 恢复 canvas
            canvas.restoreToCount(saveCount);
        }
    }
```

ImageView 是绘制图片用的，因此 mDrawable 应该为 BitmapDrawable 类型：

```
// BitmapDrawable.java
	@Override
    public void draw(Canvas canvas) {
        final Bitmap bitmap = mBitmapState.mBitmap;
        if (bitmap == null) {
            return;
        }

        ... 
        
        if (shader == null) {
            if (needMirroring) {
                canvas.save();
                // Mirror the bitmap
                canvas.translate(mDstRect.right - mDstRect.left, 0);
                canvas.scale(-1.0f, 1.0f);
            }

            canvas.drawBitmap(bitmap, null, mDstRect, paint);

            if (needMirroring) {
                canvas.restore();
            }
        } else {
            updateShaderMatrix(bitmap, paint, shader, needMirroring);
            canvas.drawRect(mDstRect, paint);
        }

	    ...
    }
```

可以看到，其实 ImageView 和我们平时自定义 View 的写法没什么不同，最终还是通过 canvas.drawXxx 来完成绘制的。另外 onMeasure() 方法就不分析了，有兴趣的可以自己看看。

## SurfaceView

这里的目标是写一个能够指定 drawable 类型的属性来显示图片的代码，因此首先要在 attrs.xml 自定义一个属性：

```
<?xml version="1.0" encoding="utf-8"?>
<resources>

    <declare-styleable name="DrawableSurfaceView">
	    <attr name="src" format="reference"/>
    </declare-styleable>

</resources>
```

然后写一个继承 SurfaceView 的自定义 View 即可：

```
public class DrawableSurfaceView extends SurfaceView implements SurfaceHolder.Callback {

    private Bitmap mBitmap;

    public DrawableSurfaceView(Context context) {
        super(context);
        init(context, null);
    }

    public DrawableSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context, attrs);
    }

    private void init(Context context, AttributeSet attrs) {
        Drawable drawable;
        if (attrs != null) {
            TypedArray array = context.obtainStyledAttributes(attrs, R.styleable.DrawableSurfaceView);
            drawable = array.getDrawable(R.styleable.DrawableSurfaceView_src);
            array.recycle();
        } else {
            drawable = context.getResources().getDrawable(R.mipmap.draw_image_surface);
        }
        if (drawable == null) {
            throw new RuntimeException("DrawableSurfaceView get null drawable");
        }
        getHolder().addCallback(this);
        mBitmap = ((BitmapDrawable) drawable).getBitmap();
        drawable.setCallback(this);
        drawable.setLevel(0);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.drawColor(Color.BLACK);
        canvas.drawBitmap(mBitmap, 0, 0, null);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Canvas canvas = null;
        try {
            canvas = holder.lockCanvas();
            onDraw(canvas);
        } finally {
            if (canvas != null) {
                holder.unlockCanvasAndPost(canvas);
            }
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

}
```

关于 SurfaceView，有一个知识点是，渲染到 Surface 上的内容将会由 SurfaceFlinger（而非应用）进行合成，因此获得的 **Surface 可以由单独的线程或单独的进程进行渲染**，并与应用界面执行的任何渲染隔离开。

## 自定义 View

自定义 View 和 SurfaceView 也没什么不同，不过如果看过《Android 开发艺术探索》，或者对 View 的绘制流程有所了解的话，应该知道，直接继承 View 的时候，需要注意 padding、wrap_content 等属性的处理。

大体流程则和 SurfaceView 差不多：

```
<?xml version="1.0" encoding="utf-8"?>
<resources>

    <attr name="src" format="reference"/>

    <declare-styleable name="DrawableView">
        <attr name="src"/>
    </declare-styleable>

    <declare-styleable name="DrawableSurfaceView">
        <attr name="src"/>
    </declare-styleable>

</resources>
```

```
public class DrawableView extends View {

    private static final String TAG = "DrawableView";

    private Bitmap mBitmap;
    private Rect mRectSrc;
    private Rect mRectDst;

    public DrawableView(Context context) {
        super(context);
        init(context, null);
    }

    public DrawableView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(context, attrs);
    }

    private void init(Context context, AttributeSet attrs) {
        Drawable drawable;
        if (attrs != null) {
            TypedArray array = context.obtainStyledAttributes(attrs, R.styleable.DrawableView);
            drawable = array.getDrawable(R.styleable.DrawableView_src);
            array.recycle();
        } else {
            drawable = context.getResources().getDrawable(R.mipmap.draw_image_custom);
        }
        if (drawable == null) {
            throw new RuntimeException("DrawableSurfaceView get null drawable");
        }
        mBitmap = ((BitmapDrawable) drawable).getBitmap();
        drawable.setCallback(this);
        drawable.setLevel(0);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        int widthMode = MeasureSpec.getMode(widthMeasureSpec);
        int heightMode = MeasureSpec.getMode(heightMeasureSpec);
        int widthSize = MeasureSpec.getSize(widthMeasureSpec);
        int heightSize = MeasureSpec.getSize(heightMeasureSpec);

        int width;
        int height;

		// 处理 wrap_content 
        if (widthMode == MeasureSpec.EXACTLY) {
            width = widthSize;
        } else if (widthMode == MeasureSpec.AT_MOST) {
            width = Math.min(mBitmap.getWidth(), widthSize);
        } else {
            width = mBitmap.getWidth();
        }

        if (heightMode == MeasureSpec.EXACTLY) {
            height = heightSize;
        } else if (heightMode == MeasureSpec.AT_MOST) {
            height = Math.min(mBitmap.getHeight(), heightSize);
        } else {
            height = mBitmap.getHeight();
        }

        setMeasuredDimension(width, height);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        final int paddingLeft = getPaddingLeft();
        final int paddingTop = getPaddingTop();
        final int paddingRight = getPaddingRight();
        final int paddingBottom = getPaddingBottom();
        // 支持 padding
        int width = getWidth() - paddingRight;
        int height = getHeight() - paddingBottom;
        canvas.drawColor(Color.BLACK);
        // 保存 Rect，以免在 onDraw 方法里过多创建对象
        if (mRectSrc == null) {
            mRectSrc = new Rect(0, 0, mBitmap.getWidth(), mBitmap.getHeight());
        }
        if (mRectDst == null || mRectDst.left != paddingLeft || mRectDst.top != paddingTop
                || mRectDst.right != width || mRectDst.bottom != height) {
            mRectDst = new Rect(paddingLeft, paddingTop, width, height);
        }
        canvas.drawBitmap(mBitmap, mRectSrc, mRectDst, null);
    }

}
```

## 自定义 View 和 SurfaceView 的区别

这里有一个值得注意的地方，即在 DrawableSurfaceView 中绘制图片时，首先需要 lockSurface 返回一个 canvas，然后调用 canvas.drawBitmap()，最后再 unlockCanvasAndPost。而直接继承 View 的 DrawableView 则不需要，这是为什么呢？

其实 DrawableView 也执行了 lockSurface 和 unlockCanvasAndPost，只不过是 ViewRootImpl 在遍历 View Tree 的时候执行的，而上面有提到， SurfaceView 的内容由 SurfaceFlinger 直接合成，因此它不归 ViewRootImpl 管，所以要自己手动调用这两个方法。

ViewRootImpl  遍历 View Tree 的对应的方法为 performTraversals，其中有三大流程：performMeasure、performLayout、performDraw，View 的 lockSurface 和 unlockCanvasAndPost 这两个方法就是在 performDraw 里调用的：

```
public final class ViewRootImpl implements ViewParent,
        View.AttachInfo.Callbacks, ThreadedRenderer.DrawCallbacks {

    private void performDraw() {
        try {
            draw(fullRedrawNeeded);
        } finally {
            ...
        }
        ...
    }
    
    private void draw(boolean fullRedrawNeeded) {
        ...
        if (!dirty.isEmpty() || mIsAnimating || accessibilityFocusDirty) {
            if (mAttachInfo.mThreadedRenderer != null && mAttachInfo.mThreadedRenderer.isEnabled()) {
                // 硬件
                mAttachInfo.mThreadedRenderer.draw(mView, mAttachInfo, this);
                ...
            } else {
                ...
                // 软件
                if (!drawSoftware(surface, mAttachInfo, xOffset, yOffset, scalingRequired, dirty)) {
                    return;
                }
            }
        }
        ...
    }

    private boolean drawSoftware(Surface surface, AttachInfo attachInfo, int xoff, int yoff,
            boolean scalingRequired, Rect dirty) {
        
        ...
        
        // Draw with software renderer.
        final Canvas canvas;
        try {
            // 获取一个 Canvas，在此基础上作图
            canvas = mSurface.lockCanvas(dirty);
            ...
        } catch (Surface.OutOfResourcesException e) {
            ...
        } catch (IllegalArgumentException e) {
            ...
        }

        try {
            ...
            try {
                // 坐标转换
                canvas.translate(-xoff, -yoff);
                if (mTranslator != null) {
                    mTranslator.translateCanvas(canvas);
                }
                ...
                // draw 会调用 onDraw 等方法
                mView.draw(canvas);
            } finally {
                ...
            }
        } finally {
            try {
                surface.unlockCanvasAndPost(canvas);
            } catch (IllegalArgumentException e) {
                ...
            }
        }
        return true;
    }

}
```

可见，SurfaceView 和普通 View 的绘制流程大致还是相同的，只不过 View 的封装度更高，有些工作 Android 已经帮自己做好了。

源码已上传到 [GitHub](https://github.com/zouzhiheng/AVGraphics)



