package com.oymotion.gforce;

import android.os.Bundle;
import com.unity3d.player.UnityPlayer;
import com.unity3d.player.UnityPlayerActivity;

public class MainActivity extends UnityPlayerActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    // 第一个参数是unity中的对象名字，记住是对象名字，不是脚本类名
    // 第二个参数是函数名
    // 第三个参数是传给函数的参数，目前只看到一个参数，并且是string的，自己传进去转吧
    public void callUnityFunc(String _objName , String _funcStr, String _content)
    {
        UnityPlayer.UnitySendMessage(_objName, _funcStr, "Come from:" + _content);
    }
}