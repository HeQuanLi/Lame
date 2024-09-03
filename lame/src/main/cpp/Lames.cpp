#include <jni.h>
#include "lame.h"  // 引入你的头文件

const int BUFF_SIZE = 8192;
extern "C"
JNIEXPORT jstring

JNICALL
Java_com_hql_lame_LameUtils_getVersion(JNIEnv *env, jobject jo) {
    return env->NewStringUTF(get_lame_version());
}

// 公共函数：处理PCM或WAV转MP3
jboolean process_to_mp3(JNIEnv *env, jstring input_path, jstring mp3path, jint channel,
                        jint sample_rate, jint byte_rate, jint quality, jboolean is_wav) {
    jboolean isCopy;
    const char *cinput = env->GetStringUTFChars(input_path, &isCopy);
    const char *cmp3 = env->GetStringUTFChars(mp3path, &isCopy);

    FILE *finput = fopen(cinput, "rb");
    if (!finput) {
// 处理文件打开失败
        env->ReleaseStringUTFChars(input_path, cinput);
        env->ReleaseStringUTFChars(mp3path, cmp3);
        return false;
    }

    if (is_wav) {
        fseek(finput, 4 * 1024, SEEK_CUR);  // 跳过WAV文件头
    }

    FILE *fmp3 = fopen(cmp3, "wb+");
    if (!fmp3) {
// 处理MP3文件打开失败
        fclose(finput);
        env->ReleaseStringUTFChars(input_path, cinput);
        env->ReleaseStringUTFChars(mp3path, cmp3);
        return false;
    }

    short int wav_buffer[BUFF_SIZE * channel];
    unsigned char mp3_buffer[BUFF_SIZE];

// 初始化lame
    lame_global_flags *lameConvert = lame_init();
    if (!lameConvert) {
// 处理lame初始化失败
        fclose(finput);
        fclose(fmp3);
        env->ReleaseStringUTFChars(input_path, cinput);
        env->ReleaseStringUTFChars(mp3path, cmp3);
        return false;
    }

    lame_set_in_samplerate(lameConvert, sample_rate);
    lame_set_num_channels(lameConvert, channel);
    lame_set_out_samplerate(lameConvert, sample_rate);
    lame_set_brate(lameConvert, byte_rate);
    lame_set_mode(lameConvert, MONO);
    lame_set_quality(lameConvert, quality);
    lame_set_VBR(lameConvert, vbr_default);

    if (lame_init_params(lameConvert) < 0) {
// 处理lame参数初始化失败
        lame_close(lameConvert);
        fclose(finput);
        fclose(fmp3);
        env->ReleaseStringUTFChars(input_path, cinput);
        env->ReleaseStringUTFChars(mp3path, cmp3);
        return false;
    }

// 开始转换
    int read, write;
    do {
        read = fread(wav_buffer, sizeof(short int) * channel, BUFF_SIZE, finput);
        if (read > 0) {
            write = lame_encode_buffer(lameConvert, wav_buffer, nullptr, read, mp3_buffer,
                                       BUFF_SIZE);
        } else {
            write = lame_encode_flush(lameConvert, mp3_buffer, BUFF_SIZE);
        }
        if (write > 0) {
            fwrite(mp3_buffer, 1, write, fmp3);
        }
    } while (read > 0);

// 添加Xing/Info标签
    lame_mp3_tags_fid(lameConvert, fmp3);

// 释放资源
    lame_close(lameConvert);
    fclose(finput);
    fclose(fmp3);
    env->ReleaseStringUTFChars(input_path, cinput);
    env->ReleaseStringUTFChars(mp3path, cmp3);
    return true;
}

extern "C"
JNIEXPORT jboolean

JNICALL
Java_com_hql_lame_LameUtils_pcm2mp3(JNIEnv *env,
                                    jobject jo,
                                    jstring input_path,
                                    jstring mp3path,
                                    jint channel,
                                    jint sample_rate,
                                    jint byte_rate,
                                    jint quality) {
    return process_to_mp3(env, input_path, mp3path, channel, sample_rate, byte_rate, quality,
                          false);
}

extern "C"
JNIEXPORT jboolean

JNICALL
Java_com_hql_lame_LameUtils_wav2mp3(JNIEnv *env,
                                    jobject jo,
                                    jstring input_path,
                                    jstring mp3path,
                                    jint channel,
                                    jint sample_rate,
                                    jint byte_rate,
                                    jint quality) {
    return process_to_mp3(env, input_path, mp3path, channel, sample_rate, byte_rate, quality, true);
}