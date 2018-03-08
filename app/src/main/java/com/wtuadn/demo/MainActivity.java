package com.wtuadn.demo;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;

import com.wtuadn.jnitool.JNITool;


public class MainActivity extends AppCompatActivity {
    private static final String TAG = "jnidemo";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Log.e(TAG, "signature：" + JNITool.getSignature(this));

        String originalStr = "123";
        String encrypt = JNITool.encrypt(originalStr);
        Log.e(TAG, "原字符串：" + originalStr);
        Log.e(TAG, "AES ECB PKCS7Padding 加密：" + encrypt);
        Log.e(TAG, "AES ECB PKCS7Padding 解密：" + JNITool.decrypt(encrypt));

        String pwdStr = "pwd123456";
        Log.e(TAG, "password: " + pwdStr);
        Log.e(TAG, "md5再加salt: " + JNITool.pwdMD5(pwdStr));
    }
}
