package an.qt.useJar;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.media.MediaCodecInfo;
import android.os.Bundle;
import android.os.Environment;
//import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;

/**
 * A placeholder fragment containing a simple view.
 */
public class ExtendsQtWithJava extends org.qtproject.qt5.android.bindings.QtActivity{

    private final static int BUFFER_SIZE = 1024*1024*2;
    private static ByteBuffer mDirectBuffer;
    private static int dataLen;
    private static int iSize = 0;
    static byte[] data = new byte[1024*1024*2];
    static int len = 0;
    public ExtendsQtWithJava(){
        mDirectBuffer = ByteBuffer.allocateDirect(BUFFER_SIZE);
    }
    private final static String TAG = "~~~extendsQt";
    public static ExtendsQtNative m_nativeNotify = null;
    private static void notifyQt(int len, byte[] data){
        if(m_nativeNotify == null){
            m_nativeNotify = new ExtendsQtNative();
        }
        m_nativeNotify.sendByte(len,data);
    }
//    private static byte[] getByteArrayFromQt(){
//        if(m_nativeNotify == null){
//            m_nativeNotify = new ExtendsQtNative();
//        }
//        return m_nativeNotify.getByteArray();
//    }
    private static void setDirectBuffer()
    {
        if(m_nativeNotify == null){
            m_nativeNotify = new ExtendsQtNative();
        }
        m_nativeNotify.setDirectBuffer(mDirectBuffer,BUFFER_SIZE);
    }
    public static void start() {

        Log.e(TAG, "start");
        setDirectBuffer();

//        mSurfaceView = new SurfaceView(null);
        initDecoder();
        startDecoder();
    }

    public static void stop() {
        Log.e(TAG, "stop test");
        Log.e(TAG, "buf0==p=" +mDirectBuffer.array()[0]);
        Log.e(TAG, "buf1==p=" +mDirectBuffer.array()[1]);
        Log.e(TAG, "buf2==p=" +mDirectBuffer.array()[2]);

    }
    public static void flsh(int len) {
        dataLen = len;
        Log.e(TAG, "buf0==p=" +mDirectBuffer.array()[0]);
        Log.e(TAG, "buf1==p=" +mDirectBuffer.array()[1]);
        Log.e(TAG, "buf2==p=" +mDirectBuffer.array()[2]);
        Log.e(TAG, "len==========" +dataLen);
    }
    public static void setSize(int size) {
        iSize = size;
//        Log.e(TAG, "size==========" + iSize);
    }
    private static final int[] Table_fv1 = { -180, -179, -177, -176, -174, -173, -172, -170, -169, -167, -166, -165, -163, -162, -160, -159, -158, -156, -155, -153, -152, -151, -149, -148, -146, -145, -144, -142, -141, -139, -138, -137,  -135, -134, -132, -131, -130, -128, -127, -125, -124, -123, -121, -120, -118, -117, -115, -114, -113, -111, -110, -108, -107, -106, -104, -103, -101, -100, -99, -97, -96, -94, -93, -92, -90,  -89, -87, -86, -85, -83, -82, -80, -79, -78, -76, -75, -73, -72, -71, -69, -68, -66, -65, -64,-62, -61, -59, -58, -57, -55, -54, -52, -51, -50, -48, -47, -45, -44, -43, -41, -40, -38, -37,  -36, -34, -33, -31, -30, -29, -27, -26, -24, -23, -22, -20, -19, -17, -16, -15, -13, -12, -10, -9, -8, -6, -5, -3, -2, 0, 1, 2, 4, 5, 7, 8, 9, 11, 12, 14, 15, 16, 18, 19, 21, 22, 23, 25, 26, 28, 29, 30, 32, 33, 35, 36, 37, 39, 40, 42, 43, 44, 46, 47, 49, 50, 51, 53, 54, 56, 57, 58, 60, 61, 63, 64, 65, 67, 68, 70, 71, 72, 74, 75, 77, 78, 79, 81, 82, 84, 85, 86, 88, 89, 91, 92, 93, 95, 96, 98, 99, 100, 102, 103, 105, 106, 107, 109, 110, 112, 113, 114, 116, 117, 119, 120, 122, 123, 124, 126, 127, 129, 130, 131, 133, 134, 136, 137, 138, 140, 141, 143, 144, 145, 147, 148,  150, 151, 152, 154, 155, 157, 158, 159, 161, 162, 164, 165, 166, 168, 169, 171, 172, 173, 175, 176, 178 };
    private static final int[] Table_fv2 = { -92, -91, -91, -90, -89, -88, -88, -87, -86, -86, -85, -84, -83, -83, -82, -81, -81, -80, -79, -78, -78, -77, -76, -76, -75, -74, -73, -73, -72, -71, -71, -70, -69, -68, -68, -67, -66, -66, -65, -64, -63, -63, -62, -61, -61, -60, -59, -58, -58, -57, -56, -56, -55, -54, -53, -53, -52, -51, -51, -50, -49, -48, -48, -47, -46, -46, -45, -44, -43, -43, -42, -41, -41, -40, -39, -38, -38, -37, -36, -36, -35, -34, -33, -33, -32, -31, -31, -30, -29, -28, -28, -27, -26, -26, -25, -24, -23, -23, -22, -21, -21, -20, -19, -18, -18, -17, -16, -16, -15, -14, -13, -13, -12, -11, -11, -10, -9, -8, -8, -7, -6, -6, -5, -4, -3, -3, -2, -1, 0, 0, 1, 2, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 17, 18, 19, 20, 20, 21, 22, 22, 23, 24, 25, 25, 26, 27, 27, 28, 29, 30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 37, 38, 39, 40, 40, 41, 42, 42, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50, 50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 62, 63, 64, 65, 65, 66, 67, 67, 68, 69, 70, 70, 71, 72, 72, 73, 74, 75, 75, 76, 77, 77, 78, 79, 80, 80, 81, 82, 82, 83, 84, 85, 85, 86, 87, 87, 88, 89, 90, 90 };
    private static final int[] Table_fu1 = { -44, -44, -44, -43, -43, -43, -42, -42, -42, -41, -41, -41, -40, -40, -40, -39, -39, -39, -38, -38, -38, -37, -37, -37, -36, -36, -36, -35, -35, -35, -34, -34, -33, -33, -33, -32, -32, -32, -31, -31, -31, -30, -30, -30, -29, -29, -29, -28, -28, -28, -27, -27, -27, -26, -26, -26, -25, -25, -25, -24, -24, -24, -23, -23, -22, -22, -22, -21, -21, -21, -20, -20, -20, -19, -19, -19, -18, -18, -18, -17, -17, -17, -16, -16, -16, -15, -15, -15, -14, -14, -14, -13, -13, -13, -12, -12, -11, -11, -11, -10, -10, -10, -9, -9, -9, -8, -8, -8, -7, -7, -7, -6, -6, -6, -5, -5, -5, -4, -4, -4, -3, -3, -3, -2, -2, -2, -1, -1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 31, 32, 32, 33, 33, 33, 34, 34, 34, 35, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 42, 42, 42, 43, 43 };
    private static final int[] Table_fu2 = { -227, -226, -224, -222, -220, -219, -217, -215, -213, -212, -210, -208, -206, -204, -203, -201, -199, -197, -196, -194, -192, -190, -188, -187, -185, -183, -181, -180, -178, -176, -174, -173, -171, -169, -167, -165, -164, -162, -160, -158, -157, -155, -153, -151, -149, -148, -146, -144, -142, -141, -139, -137, -135, -134, -132, -130, -128, -126, -125, -123, -121, -119, -118, -116, -114, -112, -110, -109, -107, -105, -103, -102, -100, -98, -96, -94, -93, -91, -89, -87, -86, -84, -82, -80, -79, -77, -75, -73, -71, -70, -68, -66, -64, -63, -61, -59, -57, -55, -54, -52, -50, -48, -47, -45, -43, -41, -40, -38, -36, -34, -32, -31, -29, -27, -25, -24, -22, -20, -18, -16, -15, -13, -11, -9, -8, -6, -4, -2, 0, 1, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 21, 23, 24, 26, 28, 30, 31, 33, 35, 37, 39, 40, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 60, 62, 63, 65, 67, 69, 70, 72, 74, 76, 78, 79, 81, 83, 85, 86, 88, 90, 92, 93, 95, 97, 99, 101, 102, 104, 106, 108, 109, 111, 113, 115, 117, 118, 120, 122, 124, 125, 127, 129, 131, 133, 134, 136, 138, 140, 141, 143, 145, 147, 148, 150, 152, 154, 156, 157, 159, 161, 163, 164, 166, 168, 170, 172, 173, 175, 177, 179, 180, 182, 184, 186, 187, 189, 191, 193, 195, 196, 198, 200, 202, 203, 205, 207, 209, 211, 212, 214, 216, 218, 219, 221, 223, 225 };
    //YUV420 Planar
    public static byte[] YV12ToRGB_Table(byte[] pYV12, int width, int height){
        final int nYLen =  width * height ;
        final int nHfWidth = (width>>1);
        byte[] pRGB24 = new byte[nYLen*3];

        // Y data
        //unsigned char* yData = pYV12;
        byte[] yData = new byte[pYV12.length];
        for(int i=0;i<pYV12.length;i++)
            yData[i] = pYV12[i];
        // v data
        //unsigned char* vData = &yData[nYLen];
        byte[] vData = new byte[yData.length - nYLen];
        for(int i=0;i<vData.length;i++)
            vData[i] = yData[nYLen + i];
        // u data
        //unsigned char* uData = &vData[nYLen>>2];
        byte[] uData = new byte[vData.length - (nYLen>>2)];
        for(int i=0;i<uData.length;i++)
        {
            uData[i] = vData[(nYLen>>2) + i];
        }
    //        if( uData.length == 0  ||  vData.length == 0)
    //            return false;
        //int rgb[3];
        int[] rgb = new int[3];
        int i, j, m, n, x, y, pu, pv, py, rdif, invgdif, bdif;
        int k, idx;
        m = -width;
        n = -nHfWidth;
        //--------------------------------------
        boolean addhalf = true;
        for(y=0; y<height;y++) {
            m += width;
            if( addhalf ){
                n+=nHfWidth;
                addhalf = false;
            } else {
                addhalf = true;
            }
            for(x=0; x<width;x++)  {
                i = m + x;
                j = n + (x>>1);

                py = (int)((yData[i] + 256)%256);

                // search tables to get rdif invgdif and bidif
                rdif = Table_fv1[(int)((vData[j] + 256)%256)];    // fv1
                invgdif = Table_fu1[(int)((uData[j]+ 256)%256)] + Table_fv2[(int)((vData[j] + 256)%256)]; // fu1+fv2
                bdif = Table_fu2[(int)((uData[j] + 256)%256)]; // fu2

                rgb[2] = py+rdif;    // R
                rgb[1] = py-invgdif; // G
                rgb[0] = py+bdif;    // B

                // copy this pixel to rgb data
                idx = (y * width + x) * 3;
                for(k=0; k<3; k++)
                    pRGB24[idx + k] = (byte)(rgb[k]<0? 0: rgb[k]>255? 255 : rgb[k]);
            }
        }
        return pRGB24;
    }
    //YUV420 Semi-Planar
    public static byte[] YV12ToRGB_Table_2(byte[] pYV12, int width, int height){
        final int nYLen =  width * height ;
        final int nHfWidth = (width>>1);
        byte[] pRGB24 = new byte[nYLen*3];

        // Y data
        //unsigned char* yData = pYV12;
        byte[] yData = new byte[pYV12.length];
        for(int i=0;i<pYV12.length;i++)
            yData[i] = pYV12[i];

        byte[] vData = new byte[nYLen>>2];
        byte[] uData = new byte[nYLen>>2];
        for(int i=0;i<(nYLen>>1);i++)
        {
            if(i%2 == 0)
                vData[i/2] = yData[nYLen + i];
            else
                uData[i/2] = yData[nYLen + i];
        }

        //int rgb[3];
        int[] rgb = new int[3];
        int i, j, m, n, x, y, pu, pv, py, rdif, invgdif, bdif;
        int k, idx;
        m = -width;
        n = -nHfWidth;
        //--------------------------------------
        boolean addhalf = true;
        for(y=0; y<height;y++) {
            m += width;
            if( addhalf ){
                n+=nHfWidth;
                addhalf = false;
            } else {
                addhalf = true;
            }
            for(x=0; x<width;x++)  {
                i = m + x;
                j = n + (x>>1);

                py = (int)((yData[i] + 256)%256);

                // search tables to get rdif invgdif and bidif
                rdif = Table_fv1[(int)((vData[j] + 256)%256)];    // fv1
                invgdif = Table_fu1[(int)((uData[j]+ 256)%256)] + Table_fv2[(int)((vData[j] + 256)%256)]; // fu1+fv2
                bdif = Table_fu2[(int)((uData[j] + 256)%256)]; // fu2

                rgb[2] = py+rdif;    // R
                rgb[1] = py-invgdif; // G
                rgb[0] = py+bdif;    // B

                // copy this pixel to rgb data
                idx = (y * width + x) * 3;
                for(k=0; k<3; k++)
                    pRGB24[idx + k] = (byte)(rgb[k]<0? 0: rgb[k]>255? 255 : rgb[k]);
            }
        }
        return pRGB24;
    }


    private static final String SAMPLE = "sdcard/gaode.mp4";
    private static final int WIDTH_DECODER = 800;
    private static final int HEIGHT_DECODER = 480;
    private static PlayerThread mPlayer = null;

    private static RandomAccessFile mFileInput;
    private static MediaCodec mMediaCodec;
//    private static SurfaceView mSurfaceView = null;
    private static Surface mSurface = null;

//        @Override
//        public void onDestroy() {
//                // TODO Auto-generated method stub
//                mMediaCodec.stop();
//                mMediaCodec.release();
//                super.onDestroy();
//        }

//        @Override
//        public void surfaceCreated(SurfaceHolder holder) {
//        }

//        @Override
//        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
//                mSurface = holder.getSurface();
//                initDecoder();
//                startDecoder();
//        }

//        @Override
//        public void surfaceDestroyed(SurfaceHolder holder) {
//                if (mPlayer != null) {
//                        mPlayer.interrupt();
//                }
//        }

    public static void startDecoder() {
        if (mPlayer == null) {
            mPlayer = new PlayerThread();
            mPlayer.start();
        }
    }

    private static class PlayerThread extends Thread {

        @Override
        public void run() {
            readFile_1();
        }
    }

    private static void readFile_1() {
        if (mFileInput == null) {
            try {
                mFileInput = new RandomAccessFile(new File(SAMPLE), "r");
            } catch (FileNotFoundException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

        int nPos = 0;
        int nLength = 0;
        byte [] buffer = null;

        try {
            nLength = (int)mFileInput.length();
            buffer = new byte[nLength];
            Log.i("yflog", "read file length:" + nLength);

            mFileInput.read(buffer);
        } catch (IOException e1) {
            // TODO Auto-generated catch block
            e1.printStackTrace();
        }

        int count = 26;
        int flag = 0;
        int index = 0;
        byte[] frameDate = new byte[WIDTH_DECODER * HEIGHT_DECODER * 3 / 2];
        int byteCount = count;
        try {
            // mFileInput.read(frameDate, 0, 25);
            System.arraycopy(buffer, 0, frameDate, 0, count);
            nPos += count;

            flag = 0;
            while (nPos < nLength) {
                if (byteCount != count)
                {
                    byteCount = 4;
                }
                while (nPos < nLength)
                {
//                    Log.e("aaaa", "nPos: " + nPos + "; nLength:" + nLength);
                    // frameDate[byteCount] = (byte) flag;
                    flag = buffer[nPos++];
                    frameDate[byteCount++] = (byte)flag;
                    if (flag == 0)
                    {
                        index = 1;
                        while (flag == 0)
                        {
                            if (nPos < nLength)
                            {

                                flag = buffer[nPos++];

                                frameDate[byteCount++] = (byte) flag;
                                index++;
                            }
                        }
                        if (flag == 1 && index >= 4)
                        {
                            byteCount = byteCount - 4;
//                            Log.e("Media", "flag = " + flag);
                            break;
                        }
                    }
                }// 读文件结�?
                onFrame(frameDate, 0, byteCount);
//                Log.e("Media", "nPos: " + nPos + "; nLength:" + nLength +";byteCount:"+ byteCount);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            Log.e("Media", "~~~~~~~finally");
            try {
                mFileInput.close();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            mFileInput = null;
            mPlayer = null;
        }
        Log.e("Media", "~~~~~~~continue ");
    }

    private static void initDecoder() {
        mMediaCodec = MediaCodec.createDecoderByType("video/avc");
        // 瀹介珮鍙傛暟鍏跺疄鏃犵敤,浣哸ndroid4.4涔嬪悗鐗堟湰,璁剧疆涓�?鍙兘瀵艰嚧鍒濆鍖栧け璐ョ殑闂�?
        MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc",
                        WIDTH_DECODER, HEIGHT_DECODER);
        mMediaCodec.configure(mediaFormat, mSurface, null, 0);
        mMediaCodec.setVideoScalingMode(MediaCodec.VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING);
        mMediaCodec.start();
    }

    public static void onFrame(byte[] buf, int offset, int length) {
        long time = System.currentTimeMillis();
        Log.e("Media", "0");
        ByteBuffer[] inputBuffers = mMediaCodec.getInputBuffers();
         Log.e("Media", "1");
        int inputBufferIndex = mMediaCodec.dequeueInputBuffer(-1);
        Log.e("Media", "2");
        if (inputBufferIndex >= 0) {
            Log.e("Media", "3");
            ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
            inputBuffer.clear();
            inputBuffer.put(buf, offset, length);
            mMediaCodec.queueInputBuffer(inputBufferIndex, 0, length, 1, 0);
            Log.e("Media", "4");
        }

        Log.e("Media", "5");
        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
        int outputBufferIndex = mMediaCodec.dequeueOutputBuffer(bufferInfo, 0);
        Log.e("Media", "6");

//                MediaFormat bufferFormat = mMediaCodec.getOutputFormat();
//                int iFormat = bufferFormat.getInteger(MediaFormat.KEY_COLOR_FORMAT);

        ByteBuffer[] outputBuffers = mMediaCodec.getOutputBuffers();
        Log.e("Media", "outputBufferIndex: " + outputBufferIndex);
        while (outputBufferIndex >= 0) {
            ByteBuffer outBuf = outputBuffers[outputBufferIndex];
            byte[] chunk = new byte[bufferInfo.size];
            outBuf.get(chunk);
            outBuf.clear();
            if (chunk.length > 0)
            {
                // chunk解码后的数据
                len = bufferInfo.size;
                Log.e("Media", "~~~~~~~chunk.length: " + chunk.length);
//                Log.e("Media", "iFormat: " + iFormat);
//                if(iFormat == MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar)
//                    data = YV12ToRGB_Table_2(chunk, WIDTH_DECODER,HEIGHT_DECODER);
//                else if(iFormat == MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar)
                    data = YV12ToRGB_Table(chunk, WIDTH_DECODER,HEIGHT_DECODER);

                new Runnable()
                {
                    public void run(){

                        notifyQt(len*2, data);
//                        notifyQt(len, chunk);
                    }
                }.run();
            }
            mMediaCodec.releaseOutputBuffer(outputBufferIndex, true);
            outputBufferIndex = mMediaCodec.dequeueOutputBuffer(bufferInfo, 0);
        }
        Log.i("TAG", "------------onFrame---------------time:" + (System.currentTimeMillis() - time));
    }
}

