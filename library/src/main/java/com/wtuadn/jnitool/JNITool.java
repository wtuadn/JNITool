package com.wtuadn.jnitool;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;

/**
 * Created by wtuadn on 2017/3/9.
 */

public class JNITool {
    static {
        System.loadLibrary("jni_tool");
    }

    public static native boolean checkSignature(Context context);

    private static native String jniencrypt(Context context, byte[] bytes);

    private static native byte[] jnidecrypt(Context context, String str);

    public static native String pwdMD5(Context context, String str);

    public static String encrypt(Context context, String str) {
        return jniencrypt(context, str.getBytes());
    }

    public static String decrypt(Context context, String str) {
        return new String(jnidecrypt(context, str));
    }

    //获取签名
    public static int getSignature(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);

            Signature sign = packageInfo.signatures[0];
            return sign.hashCode();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return -1;
    }
}
