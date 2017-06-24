package com.oymotion.gforce;

import android.os.Bundle;
import com.unity3d.player.UnityPlayer;
import com.unity3d.player.UnityPlayerActivity;

public class MainActivity extends UnityPlayerActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    public void callUnityFunc(String _objName , String _funcStr, String _content)
    {
        UnityPlayer.UnitySendMessage(_objName, _funcStr, "Come from:" + _content);
    }
}
