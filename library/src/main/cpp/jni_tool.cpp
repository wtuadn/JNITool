#include <jni.h>
#include <string>
#include "aes.h"
#include "md5.h"

static const char *app_packageName = "com.wtuadn.demo";
static const int app_signature_hash_code = -827662039;
static const uint8_t AES_KEY[] = "xS544RXNm0P4JVLHIEsTqJNzDbZhiLjr";
static const uint8_t AES_IV[] = "KXTUDEdBs9zGlvy7";
static const string PWD_MD5_KEY = "4J9lKuR2c8OuDPBAniEy5USFQdSM0An4";

static bool signature_checked = false;

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_wtuadn_jnitool_JNITool_checkSignature(JNIEnv *env, jclass type, jobject context) {
    //Context的类
    jclass context_clazz = env->GetObjectClass(context);
    // 得到 getPackageManager 方法的 ID
    jmethodID methodID_getPackageManager = env->GetMethodID(context_clazz, "getPackageManager",
                                                            "()Landroid/content/pm/PackageManager;");
    // 获得PackageManager对象
    jobject packageManager = env->CallObjectMethod(context, methodID_getPackageManager);
    // 获得 PackageManager 类
    jclass pm_clazz = env->GetObjectClass(packageManager);
    // 得到 getPackageInfo 方法的 ID
    jmethodID methodID_pm = env->GetMethodID(pm_clazz, "getPackageInfo",
                                             "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    // 得到 getPackageName 方法的 ID
    jmethodID methodID_pack = env->GetMethodID(context_clazz,
                                               "getPackageName", "()Ljava/lang/String;");
    // 获得当前应用的包名
    jstring application_package = (jstring) env->CallObjectMethod(context, methodID_pack);
    const char *package_name = env->GetStringUTFChars(application_package, 0);
    // 获得PackageInfo
    jobject packageInfo = env->CallObjectMethod(packageManager, methodID_pm, application_package, 64);
    jclass packageinfo_clazz = env->GetObjectClass(packageInfo);
    jfieldID fieldID_signatures = env->GetFieldID(packageinfo_clazz,
                                                  "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signature_arr = (jobjectArray) env->GetObjectField(packageInfo, fieldID_signatures);
    //Signature数组中取出第一个元素
    jobject signature = env->GetObjectArrayElement(signature_arr, 0);
    //读signature的hashcode
    jclass signature_clazz = env->GetObjectClass(signature);
    jmethodID methodID_hashcode = env->GetMethodID(signature_clazz, "hashCode", "()I");
    jint hashCode = env->CallIntMethod(signature, methodID_hashcode);

    if (strcmp(package_name, app_packageName) != 0) {
        exit(-1);
    }
    if (hashCode != app_signature_hash_code) {
        exit(-2);
    }
    signature_checked = true;
    return JNI_TRUE;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_wtuadn_jnitool_JNITool_jniencrypt(JNIEnv *env, jclass type, jobject context, jbyteArray jbArr) {

    if (!signature_checked) Java_com_wtuadn_jnitool_JNITool_checkSignature(env, type, context);

    char *str = NULL;
    jsize alen = env->GetArrayLength(jbArr);
    jbyte *ba = env->GetByteArrayElements(jbArr, JNI_FALSE);
    str = (char *) malloc(alen + 1);
    memcpy(str, ba, alen);
    str[alen] = '\0';
    env->ReleaseByteArrayElements(jbArr, ba, 0);

    char *result = AES_ECB_PKCS7_Encrypt(str, AES_KEY);//AES ECB PKCS7Padding加密
//    char *result = AES_CBC_PKCS7_Encrypt(str, AES_KEY, AES_IV);//AES CBC PKCS7Padding加密
    return env->NewStringUTF(result);
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_wtuadn_jnitool_JNITool_jnidecrypt(JNIEnv *env, jclass type, jobject context, jstring out_str) {

    if (!signature_checked) Java_com_wtuadn_jnitool_JNITool_checkSignature(env, type, context);

    const char *str = env->GetStringUTFChars(out_str, 0);
    char *result = AES_ECB_PKCS7_Decrypt(str, AES_KEY);//AES ECB PKCS7Padding解密
//    char *result = AES_CBC_PKCS7_Decrypt(str, AES_KEY, AES_IV);//AES CBC PKCS7Padding解密
    env->ReleaseStringUTFChars(out_str, str);

    jsize len = (jsize) strlen(result);
    jbyteArray jbArr = env->NewByteArray(len);
    env->SetByteArrayRegion(jbArr, 0, len, (jbyte *) result);
    return jbArr;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_wtuadn_jnitool_JNITool_pwdMD5(JNIEnv *env, jclass type, jobject context, jstring out_str) {

    if (!signature_checked) Java_com_wtuadn_jnitool_JNITool_checkSignature(env, type, context);

    const char *str = env->GetStringUTFChars(out_str, 0);
    string result = MD5(MD5(PWD_MD5_KEY + string(str)).toStr()).toStr();//加盐后进行两次md5
    env->ReleaseStringUTFChars(out_str, str);
    return env->NewStringUTF(("###" + result).data());//最后再加三个#
}


