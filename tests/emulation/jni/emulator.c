#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <gfx/gfx_types_resource.h>
#include "gfx/gfx_primitive.h"
#include "jni/com_microchip_grc_Emulator.h"
#include "system_config.h"
#include <assert.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

jobject makeGfxResourceHeader(JNIEnv *env);

jint callGfxResourceHeaderGetHeight(JNIEnv *env, jobject gfxResourceHdr);
void callGfxResourceHeaderSetHeight(JNIEnv *env, jobject gfxResourceHeader, uint16_t i);
jint callGfxResourceHeaderGetBpp(JNIEnv *env, jobject gfxResourceHdr);
void callGfxResourceHeaderSetBpp(JNIEnv *env, jobject gfxResourceHeader, uint16_t i);
jint callGfxResourceHeaderGetLastChar(JNIEnv *env, jobject gfxResourceHdr);
void callGfxResourceHeaderSetLastChar(JNIEnv *env, jobject gfxResourceHeader, uint16_t i);
jint callGfxResourceHeaderGetFirstChar(JNIEnv *env, jobject gfxResourceHdr);
void callGfxResourceHeaderSetFirstChar(JNIEnv *env, jobject gfxResourceHeader, uint16_t i);

jobject
callGfxExternalResourceCallbackGetBuffer(JNIEnv *env, GFX_RESOURCE_HDR param, jint i, jint i1, jobject pJobject);
jboolean callIteratorHasNext(JNIEnv *env, jobject iterator);
jobject callIteratorNext(JNIEnv *env, jobject iterator);


char *getString(JNIEnv *env, jobject string);

static JavaVM *vm;
static GFX_RESOURCE_HDR pHdr;

JNIEXPORT void JNICALL Java_com_microchip_grc_Emulator_gfxColorSet
        (JNIEnv * env, jobject thisObj, jint color) {
    GFX_ColorSet(color);
}

JNIEXPORT jint JNICALL Java_com_microchip_grc_Emulator_gfxColorGet
        (JNIEnv * env, jobject thisObj) {
    return GFX_ColorGet();
}

JNIEXPORT void JNICALL Java_com_microchip_grc_Emulator_gfxLineDraw
        (JNIEnv * env, jobject thisObj, jint x1, jint y1, jint x2, jint y2) {
    GFX_LineDraw(x1, y1, x2, y2);
}

JNIEXPORT jint JNICALL Java_com_microchip_grc_Emulator_gfxLineStyleSet
        (JNIEnv * env, jobject thisObj, jint lineStyle) {
    return GFX_LineStyleSet(lineStyle);
}

JNIEXPORT jint JNICALL Java_com_microchip_grc_Emulator_gfxLineStyleGet
        (JNIEnv * env, jobject thisObj) {
    return GFX_LineStyleGet();
}

JNIEXPORT jint JNICALL Java_com_microchip_grc_Emulator_gfxFontSet
        (JNIEnv * env, jobject thisObj, jobject gfxResourceHdr) {

    jint height = callGfxResourceHeaderGetHeight(env, gfxResourceHdr);
    jint bpp = callGfxResourceHeaderGetBpp(env, gfxResourceHdr);
    jint lastChar = callGfxResourceHeaderGetLastChar(env, gfxResourceHdr);
    jint firstChar = callGfxResourceHeaderGetFirstChar(env, gfxResourceHdr);
    
    pHdr.type = GFX_RESOURCE_FONT_EXTERNAL_NONE;
    pHdr.resource.font.location.extAddress = 0;
    pHdr.resource.font.header.height = height;
    pHdr.resource.font.header.firstChar = firstChar;
    pHdr.resource.font.header.lastChar = lastChar;
    pHdr.resource.font.header.bpp = bpp;

    return GFX_FontSet(&pHdr);
}

JNIEXPORT jobject JNICALL Java_com_microchip_grc_Emulator_gfxFontGet
        (JNIEnv * env, jobject thisObj) {
    jobject gfxResourceHeader = makeGfxResourceHeader(env);

    callGfxResourceHeaderSetHeight(env, gfxResourceHeader, GFX_FontGet()->resource.font.header.height);
    callGfxResourceHeaderSetLastChar(env, gfxResourceHeader, GFX_FontGet()->resource.font.header.lastChar);
    callGfxResourceHeaderSetFirstChar(env, gfxResourceHeader, GFX_FontGet()->resource.font.header.firstChar);
    callGfxResourceHeaderSetBpp(env, gfxResourceHeader, GFX_FontGet()->resource.font.header.bpp);

    return gfxResourceHeader;
}


JNIEXPORT jint JNICALL Java_com_microchip_grc_Emulator_gfxScreenClear
        (JNIEnv *env, jobject thisObj) {
    return GFX_ScreenClear();
}

JNIEXPORT void JNICALL Java_com_microchip_grc_Emulator_gfxTextAreaBottomSet
        (JNIEnv * env, jobject thisObj, jint bottom) {
    GFX_TextAreaBottomSet(bottom);
}

JNIEXPORT void JNICALL Java_com_microchip_grc_Emulator_gfxTextAreaLeftSet
        (JNIEnv * env, jobject thisObj, jint left) {
    GFX_TextAreaLeftSet(left);
}

JNIEXPORT void JNICALL Java_com_microchip_grc_Emulator_gfxTextAreaRightSet
        (JNIEnv * env, jobject thisObj, jint right) {
    GFX_TextAreaRightSet(right);
}

JNIEXPORT void JNICALL Java_com_microchip_grc_Emulator_gfxTextAreaTopSet
        (JNIEnv * env, jobject thisObj, jint top) {
    GFX_TextAreaTopSet(top);
}

JNIEXPORT jint JNICALL Java_com_microchip_grc_Emulator_gfxTextCursorPositionSet
        (JNIEnv * env, jobject thisObj, jint x, jint y) {
    return GFX_TextCursorPositionSet(x, y);
}

JNIEXPORT jint JNICALL Java_com_microchip_grc_Emulator_gfxTextStringDraw
        (JNIEnv * env, jobject thisObj, jint x, jint y, jstring string, jint length) {

    char *chars = getString(env, string);

    GFX_STATUS status = GFX_TextStringDraw(x, y, chars, length);

    free(chars);
    return status;
}

JNIEXPORT jboolean JNICALL Java_com_microchip_grc_Emulator_validateScreen
        (JNIEnv * env, jobject thisObj, jstring string) {

    char *chars = getString(env, string);
    bool result = VirtualScreen_ValidateScreen(chars);
    free(chars);
    return result;
}
static char *fileName = NULL;
JNIEXPORT void JNICALL Java_com_microchip_grc_Emulator_setFontFileName
        (JNIEnv * env, jobject thisObj, jstring string) {
    if(fileName != NULL) {
        free(fileName);
    }
    fileName = getString(env, string);
}

JNIEXPORT void JNICALL Java_com_microchip_grc_Emulator_init
        (JNIEnv * env, jobject thisObj) {
    (*env)->GetJavaVM(env, &vm);
}

JNIEXPORT jint JNICALL Java_com_microchip_grc_Emulator_getDisplayHorizontalResolution
        (JNIEnv * env, jobject thisObj) {
    return DISP_HOR_RESOLUTION;
}

JNIEXPORT jint JNICALL Java_com_microchip_grc_Emulator_getDisplayVerticalResolution
        (JNIEnv * env, jobject thisObj) {
    return DISP_VER_RESOLUTION;
}

char* getString(JNIEnv *env, jobject string) {
    const jchar *_chars = (*env)->GetStringChars(env, string, NULL);
    jsize size = (*env)->GetStringLength(env, string);
    char *chars = calloc(size + 1, sizeof(char));

    for(int i = 0; i < size; i ++) {
        chars[i] = _chars[i] & 0xFF;
    }
    chars[size] = 0;

    (*env)->ReleaseStringChars(env, string, _chars);
    return chars;
}

jclass findGfxResourceHeaderClass(JNIEnv *env) {
    return (*env)->FindClass(env, "com/microchip/grc/GFXResourceHeader");
}

jmethodID getGfxResourceHeaderGetHeightMethodID(JNIEnv *env) {
    return (*env)->GetMethodID(env, findGfxResourceHeaderClass(env), "getHeight", "()I");
}

jmethodID getGfxResourceHeaderSetHeightMethodID(JNIEnv *env) {
    return (*env)->GetMethodID(env, findGfxResourceHeaderClass(env), "setHeight", "(I)V");
}

jmethodID getGfxResourceHeaderGetBppMethodID(JNIEnv *env) {
    return (*env)->GetMethodID(env, findGfxResourceHeaderClass(env), "getBpp", "()I");
}

jmethodID getGfxResourceHeaderSetBppMethodID(JNIEnv *env) {
    return (*env)->GetMethodID(env, findGfxResourceHeaderClass(env), "setBpp", "(I)V");
}

jmethodID getGfxResourceHeaderGetLastCharMethodID(JNIEnv *env) {
    return (*env)->GetMethodID(env, findGfxResourceHeaderClass(env), "getLastChar", "()I");
}

jmethodID getGfxResourceHeaderSetLastCharMethodID(JNIEnv *env) {
    return (*env)->GetMethodID(env, findGfxResourceHeaderClass(env), "setLastChar", "(I)V");
}

jmethodID getGfxResourceHeaderGetFirstCharMethodID(JNIEnv *env) {
    return (*env)->GetMethodID(env, findGfxResourceHeaderClass(env), "getFirstChar", "()I");
}

jmethodID getGfxResourceHeaderSetFirstCharMethodID(JNIEnv *env) {
    return (*env)->GetMethodID(env, findGfxResourceHeaderClass(env), "setFirstChar", "(I)V");
}

jclass findByteClass(JNIEnv *env) {
    return (*env)->FindClass(env, "java/lang/Byte");
}

jmethodID getByteValueMethodID(JNIEnv *env) {
    return (*env)->GetMethodID(env, findByteClass(env), "byteValue", "()B");
}

jbyte callByteValue(JNIEnv *env, jobject b) {
    jmethodID byteValueMethodId = getByteValueMethodID(env);
    return (*env)->CallByteMethod(env, b, byteValueMethodId);
}

jclass findGfxExternalResourceCallbackClass(JNIEnv *env) {
    return (*env)->FindClass(env, "com/microchip/grc/GFXExternalResourceCallback");
}

jclass findGfxExternalResourceCallbackImplClass(JNIEnv *env) {
    return (*env)->FindClass(env, "com/microchip/grc/GFXExternalResourceCallbackImpl");
}

jclass findIteratorClass(JNIEnv *env) {
    return (*env)->FindClass(env, "com/microchip/grc/ByteIterator");
}

jmethodID getHasNextMethodId(JNIEnv *env) {
    return (*env)->GetMethodID(env, findIteratorClass(env), "hasNext", "()Z");
}

jmethodID getNextMethodId(JNIEnv *env) {
    return (*env)->GetMethodID(env, findIteratorClass(env), "next", "()Ljava/lang/Byte;");
}

jmethodID getGfxExternalResourceCallbackGetBufferMethodID(JNIEnv *env) {
    jclass pJobject = findGfxExternalResourceCallbackClass(env);
    return (*env)->GetMethodID(env, pJobject, "getBuffer", "(Lcom/microchip/grc/GFXResourceHeader;II)Ljava/util/Iterator;");
}

jint callGfxResourceHeaderGetHeight(JNIEnv *env, jobject gfxResourceHdr) {
    return (*env)->CallIntMethod(env, gfxResourceHdr, getGfxResourceHeaderGetHeightMethodID(env));
}

void callGfxResourceHeaderSetHeight(JNIEnv *env, jobject gfxResourceHeader, uint16_t i) {
    (*env)->CallVoidMethod(env, gfxResourceHeader, getGfxResourceHeaderSetHeightMethodID(env), i);
}

jint callGfxResourceHeaderGetBpp(JNIEnv *env, jobject gfxResourceHdr) {
    return (*env)->CallIntMethod(env, gfxResourceHdr, getGfxResourceHeaderGetBppMethodID(env));
}

void callGfxResourceHeaderSetBpp(JNIEnv *env, jobject gfxResourceHeader, uint16_t i) {
    (*env)->CallVoidMethod(env, gfxResourceHeader, getGfxResourceHeaderSetBppMethodID(env), i);
}

jint callGfxResourceHeaderGetLastChar(JNIEnv *env, jobject gfxResourceHdr) {
    return (*env)->CallIntMethod(env, gfxResourceHdr, getGfxResourceHeaderGetLastCharMethodID(env));
}

void callGfxResourceHeaderSetLastChar(JNIEnv *env, jobject gfxResourceHeader, uint16_t i) {
    (*env)->CallVoidMethod(env, gfxResourceHeader, getGfxResourceHeaderSetLastCharMethodID(env), i);
}

jint callGfxResourceHeaderGetFirstChar(JNIEnv *env, jobject gfxResourceHdr) {
    return (*env)->CallIntMethod(env, gfxResourceHdr, getGfxResourceHeaderGetFirstCharMethodID(env));
}

void callGfxResourceHeaderSetFirstChar(JNIEnv *env, jobject gfxResourceHeader, uint16_t i) {
    (*env)->CallVoidMethod(env, gfxResourceHeader, getGfxResourceHeaderSetFirstCharMethodID(env), i);
}

jclass findGfxResourceHeaderImplClass(JNIEnv *env) {
    return (*env)->FindClass(env, "com/microchip/grc/GFXResourceHeaderImpl");
}

jobject makeGfxResourceHeader(JNIEnv *env) {
    return (*env)->AllocObject(env, findGfxResourceHeaderImplClass(env));
}

jobject makeGfxExternalResourceCallback(JNIEnv *env) {
    return (*env)->AllocObject(env, findGfxExternalResourceCallbackImplClass(env));
}

jobject
callGfxExternalResourceCallbackGetBuffer(JNIEnv *env, GFX_RESOURCE_HDR param, jint i, jint i1, jobject pJobject) {
    jmethodID getBufferMethodId = getGfxExternalResourceCallbackGetBufferMethodID(env);
    return (*env)->CallObjectMethod(env, pJobject, getBufferMethodId, param, i, i1);
}

jboolean callIteratorHasNext(JNIEnv *env, jobject iterator) {
    jmethodID nextMethodId = getHasNextMethodId(env);
    return (*env)->CallBooleanMethod(env, iterator, nextMethodId);
}

jobject callIteratorNext(JNIEnv *env, jobject iterator) {
    jmethodID nextMethodId = getNextMethodId(env);
    return (*env)->CallObjectMethod(env, iterator, nextMethodId);
}

GFX_STATUS GFX_ExternalResourceCallback(
        GFX_RESOURCE_HDR *pResource,
        uint32_t offset,
        uint16_t nCount,
        void *pBuffer) {
    FILE *resource_file;

    resource_file = fopen(fileName, "rb");

    if(resource_file == NULL){ return GFX_STATUS_FAILURE; }
    if(fseek(resource_file, offset, SEEK_SET) != 0) { return GFX_STATUS_FAILURE; }
    if(fread(pBuffer, 1, nCount, resource_file) != nCount ) { return GFX_STATUS_FAILURE; }

    fclose(resource_file);
    return GFX_STATUS_SUCCESS;
}




#pragma clang diagnostic pop