#include <jni.h>
#include <stdlib.h>
#include <inttypes.h>

JNIEXPORT jlong JNICALL
Java_com_parrot_arsdk_arsal_ARNativeData_allocateData (JNIEnv *env, jobject thizz, jint capacity)
{
    void *ptr = 0;
    /* Alloc only if capacity is positive */
    if (capacity > 0)
    {
        ptr = malloc (capacity);
    }
    return (jlong)(intptr_t)ptr;
}

JNIEXPORT void JNICALL
Java_com_parrot_arsdk_arsal_ARNativeData_freeData (JNIEnv *env, jobject thizz, jlong data)
{
    void *ptr = (void *)(intptr_t)data;
    free (ptr);
}

JNIEXPORT jbyteArray JNICALL
Java_com_parrot_arsdk_arsal_ARNativeData_generateByteArray (JNIEnv *env, jobject thizz, jlong data, jint capacity, jint used)
{
    jbyteArray retArray = NULL;
    if (used <= capacity)
    {
        retArray = (*env)->NewByteArray (env, used);
        if (retArray != NULL)
        {
            (*env)->SetByteArrayRegion (env, retArray, 0, used, (jbyte *) (intptr_t)data);
        }
    }
    return retArray;
}

JNIEXPORT jboolean JNICALL
Java_com_parrot_arsdk_arsal_ARNativeData_copyData (JNIEnv *env, jobject thizz, jlong dst, jint dstCapacity, jlong src, jint srcLen)
{
    jboolean retVal = JNI_FALSE;
    if (srcLen <= dstCapacity)
    {
        void *srcPtr = (void *)(intptr_t)src;
        void *dstPtr = (void *)(intptr_t)dst;
        memcpy (dstPtr, srcPtr, srcLen);
        retVal = JNI_TRUE;
    }
    return retVal;
}
