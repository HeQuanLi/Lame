package com.hql.lame

/**
 * @Description
 * @Author HeQuanLi
 * @CreateTime 2024年09月03日 11:09:10
 */
object LameUtils {

    init {
        System.loadLibrary("Lames")
    }

    /**
     * 获取lame版本号
     *
     * @return
     */
    external fun getVersion(): String

    /**
     * 默认转换 单声道 16比特 16000采样率 质量5
     *
     * @param inputPath
     * @param mp3path
     * @param sampleRate
     * @param byteRate
     * @param quality
     */
    external fun pcm2mp3(
        inputPath: String,
        mp3path: String,
        channel: Int,
        sampleRate: Int = 1600,
        byteRate: Int = 16,
        quality: Int = 5
    ): Boolean

    /**
     * 默认转换 单声道 16比特 16000采样率 质量5
     *
     * @param inputPath
     * @param mp3path
     * @param sampleRate
     * @param byteRate
     * @param quality
     */
    external fun wav2mp3(
        inputPath: String,
        mp3path: String,
        channel: Int,
        sampleRate: Int = 1600,
        byteRate: Int = 16,
        quality: Int = 5
    ): Boolean
}