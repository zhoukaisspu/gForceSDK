package com.oym.libble;

import android.util.Log;

import android.content.Context;

public class GlobalContext {
    private final static String Tag = "GlobalContext";
    private static Context mContext = null;

    public static void setApplicationContext(Context context) {
        Log.d(Tag, "setApplicationContext");
        mContext = context;
    }

    public static Context getApplicationContext() {
        Log.d(Tag, "getApplicationContext");
        return mContext;
    }
}
