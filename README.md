# JNITool
NDK实现的工具库，支持AES的ECB和CBC加解密（支持emoji），MD5加盐

AES基于[tiny-AES128-C](https://github.com/kokke/tiny-AES128-C)扩展，根据key长度自动选择AES128、AES192、AES256

默认采用PKCS7Padding填充（和PKCS5Padding一样），加密后进行一次Base64

应用包名和签名的hash code放在native层，使用前会进行签名检验，防二次打包

JNITool.java :
```java
    public static native String pwdMD5(String str);
    public static String encrypt(String str);
    public static String decrypt(String str);
```

aes.c :
```c
    char *AES_ECB_PKCS7_Encrypt(const char *in, const uint8_t *key);
    char *AES_ECB_PKCS7_Decrypt(const char *in, const uint8_t *key);
    char *AES_CBC_PKCS7_Encrypt(const char *in, const uint8_t *key, const uint8_t *iv);
    char *AES_CBC_PKCS7_Decrypt(const char *in, const uint8_t *key, const uint8_t *iv);
```

使用前记得修改jni_tool.cpp的以下内容：
```c++
    static const char *app_packageName = "com.wtuadn.demo";
    static const int app_signature_hash_code = -827662039;
    static const uint8_t AES_KEY[] = "xS544RXNm0P4JVLHIEsTqJNzDbZhiLjr";
    static const uint8_t AES_IV[] = "KXTUDEdBs9zGlvy7";
    static const string PWD_MD5_KEY = "4J9lKuR2c8OuDPBAniEy5USFQdSM0An4";
```
**app_signature_hash_code**获取方法见demo

## Tips
如果只用到ECB或者CBC算法，可以把aes.h头文件里没用到的定义注释掉，减小生成的so文件体积
```c
    #ifndef CBC
      #define CBC 1
    #endif

    #ifndef ECB
      #define ECB 1
    #endif
```

### 鸣谢
> [tiny-AES128-C](https://github.com/kokke/tiny-AES128-C)、[AESJniEncrypt](https://github.com/weizongwei5/AESJniEncrypt)、[MD5](https://github.com/JieweiWei/md5)
