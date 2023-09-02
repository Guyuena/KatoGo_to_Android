package com.example.katago;

import android.content.res.AssetManager;

public class Native {

    static {
        System.loadLibrary("katago");
    }



    public static native String getKatagoVersion();

    public static native String getKatagoFullInfo();

    public static native String test();

    public static native void testMain();

    public static native void getCfgFromAssets(AssetManager assetManager);
}
