package com.righere.opencvdemo;

import android.graphics.Bitmap;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Display;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private final static String TAG = "OpencvDemo_MainActivity";
    private ImageView imageView;
    private Button mButton_split;
    private boolean splitStatus = false;
    private int w;
    private int h;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("opencv_java3");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        hideSystemNavigationBar();

        Display display = getWindowManager().getDefaultDisplay();
        Point size = new Point();
        display.getRealSize(size);
        w = size.x;
        h = size.y;

        imageView = (ImageView) findViewById(R.id.image);
        mButton_split = (Button) findViewById(R.id.Button_click);
        splitPic();
        mButton_split.setOnClickListener(this);
    }
    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case R.id.Button_click:
                splitPic();
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native int[] splitter(int w, int h);

    private void splitPic(){
        if(splitStatus){
            int[] resultInt = splitter(w, h);
            Bitmap resultImg = Bitmap.createBitmap(resultInt, w, h, Bitmap.Config.ARGB_8888);
            imageView.setImageBitmap(resultImg);
            mButton_split.setText("Unsplit");
            splitStatus = false;
            Toast.makeText(this,"OpenCV_native_split_picture_success",Toast.LENGTH_SHORT).show();
        }
        else{
            imageView.setImageResource(R.drawable.convexd);
            mButton_split.setText("Split");
            splitStatus = true;
            Toast.makeText(this,"Original",Toast.LENGTH_SHORT).show();
        }
    }

    private void hideSystemNavigationBar() {
        if (Build.VERSION.SDK_INT > 11 && Build.VERSION.SDK_INT < 19) {
            View view = this.getWindow().getDecorView();
            view.setSystemUiVisibility(View.GONE);
        } else if (Build.VERSION.SDK_INT >= 19) {
            View decorView = getWindow().getDecorView();
            int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_FULLSCREEN;
            decorView.setSystemUiVisibility(uiOptions);
        }
    }
}
