package com.glumes.opensl_sample;

import android.Manifest;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.tbruyelle.rxpermissions2.RxPermissions;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private RxPermissions rxPermissions = new RxPermissions(this);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.startPlay).setOnClickListener(this);
        findViewById(R.id.stopPlay).setOnClickListener(this);
        findViewById(R.id.startRecord).setOnClickListener(this);
        findViewById(R.id.stopRecord).setOnClickListener(this);

        rxPermissions.request(
                Manifest.permission.INTERNET,
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.READ_EXTERNAL_STORAGE)
                .subscribe();
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.startPlay:
//                OpenSLInstance.startPlay();
                OpenSLInstance.startPlayFromAssets(getAssets(), "mydream.m4a");
                break;
            case R.id.stopPlay:
                OpenSLInstance.stopPlay();
                break;
            case R.id.startRecord:
                break;
            case R.id.stopRecord:
                break;
        }
    }
}
