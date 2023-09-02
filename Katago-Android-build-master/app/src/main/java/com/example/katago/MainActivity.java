package com.example.katago;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private TextView textView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView = findViewById(R.id.textview);
        textView.setText(Native.getKatagoFullInfo());

        Thread thread = new Thread(new Mytask());
        thread.start();


    }

    public class Mytask implements Runnable{
        @Override
        public void run() {
            //子线程运行
            Native.getCfgFromAssets(getAssets());
//            Native.testMain();
            Handler handler = new Handler(Looper.getMainLooper());
            handler.post(new Runnable() {
                @Override
                public void run() {
                    //主线程更新
                }
            });
        }
    }
}