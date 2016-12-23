package com.cloudups.thread;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initCreateThread();

    }


    private void initCreateThread() {
        findViewById(R.id.btn_create_thread).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                new Thread(new Runnable() {
                    @Override
                    public void run() {

                        sem();
                    }
                }).start();
            }
        });
    }

    private native void createThread();

    private native void mutex();

    private native void cond();

    private native void sem();
}
