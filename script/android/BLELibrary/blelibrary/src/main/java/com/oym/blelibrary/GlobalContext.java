package com.oym.blelibrary;

import android.util.Log;
import android.app.Application;
import android.content.Context;
/**
 * Created by shimengz on 2017/4/20.
 */

public class GlobalContext extends Application{
    private final static String Tag = "GlobalContext";
    private static Context mContext;

    public void onCreate(){
        super.onCreate();
        Log.d(Tag, "onCreate to get context");
        mContext = getApplicationContext();
    }

    public static Context getglobalContext() {
        Log.d(Tag, "getglobalContext");
        return mContext;
    }
}
