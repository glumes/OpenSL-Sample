package com.glumes.opensl_sample;

import android.content.res.AssetManager;

public class OpenSLInstance {

    public static native void startPlay();

    public static native void stopPlay();

    public static native void startRecord();

    public static native void stopRecord();

    public static native void startPlayFromAssets(AssetManager assetManager, String path);
}
