package an.qt.useJar;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.widget.ImageView.ScaleType;
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
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.view.MotionEvent;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.FileOutputStream;
import java.io.BufferedInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import android.os.Handler;

import android.widget.RelativeLayout;

import java.text.SimpleDateFormat;
import android.os.Bundle;
import android.app.Activity;
import android.view.Window;
import android.view.WindowManager;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import android.content.res.AssetManager;
import java.util.LinkedList;
import java.util.Queue;
import java.net.Socket;
import java.io.DataInputStream;

class FrameStruct {
    byte[] buf;
    int len;
};

/**
 * A placeholder fragment containing a simple view.
 */
public class ExtendsQtSurface extends Activity{
    public static ExtendsQtSurface instance = null;

    private static Queue<FrameStruct> frameQueue = new LinkedList<FrameStruct>();
    private static boolean bCanFlush = false;
    private static Lock lock = new ReentrantLock();

    private static final int MSG_NO_CAN_FLUSH = 101;
    private static final int MSG_NO_ZOOM_IN = 102;
    private static final int MSG_NO_ZOOM_OUT = 103;
    private static final int MSG_NO_MENU = 104;
    private static final int MSG_NO_PRESS = 105;
    private static final int MSG_NO_RELEASE = 106;
    private static final int MSG_NO_MOVE = 107;
    private static final int MSG_NO_CMD_1 = 111;
    private static final int WIDTH_DECODER = 800;
    private static final int HEIGHT_DECODER = 480;

    private static final String SAMPLE = "sdcard/lk.mp4";

    private final static String TAG = "JAVA";
    private PlayerThread mPlayer = null;

//    private View rootView;
    private TextView mTimeTv;
    private Button mButton;
    private Button mButtonZoomIn;
    private Button mButtonZoomOut;
    private Button mButtonMenu;
    private ImageView imageView;
    private ImageView MenuimageView;
    private Button mButtonBack;

    private Button mButtonCmd1;
    private Button mButtonCmd2;
    private Button mButtonCmd3;
    private Button mButtonCmd4;
    private Button mButtonCmd5;
    private Button mButtonCmd6;
    private LinearLayout buttonCmdLl;
    private TextView mLabel;

    private static SurfaceView mSurfaceView;
    private static Surface mSurface;
    private static RandomAccessFile mFileInput;
    private static MediaCodec mMediaCodec;

    private AssetManager asm;

    protected void onCreate(Bundle savedInstanceState) {
        Log.e(TAG, "===================onCreate");
//        if(mDirectBuffer.array() == NULL)
//        {
//            mDirectBuffer = ByteBuffer.allocateDirect(BUFFER_SIZE);
//        }
//        setDirectBuffer();

        instance = this;
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        RelativeLayout mRl = new RelativeLayout(this);
        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT,
                RelativeLayout.LayoutParams.MATCH_PARENT);
        mRl.setLayoutParams(lp);
        mSurfaceView = new SurfaceView(this);
        RelativeLayout.LayoutParams sfvLp = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT,
                RelativeLayout.LayoutParams.WRAP_CONTENT);
        sfvLp.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
        sfvLp.addRule(RelativeLayout.ALIGN_PARENT_TOP);
        mSurfaceView.setLayoutParams(sfvLp);
        mRl.addView(mSurfaceView);

        mTimeTv = new TextView(this);
        SimpleDateFormat df = new SimpleDateFormat("HH:mm:ss");
        String date = df.format(new java.util.Date());
        mTimeTv.setText(date);
        mTimeTv.setTextSize(35);
        mTimeTv.setTextColor(Color.BLACK);
        RelativeLayout.LayoutParams timeLp = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT,
                RelativeLayout.LayoutParams.WRAP_CONTENT);
        timeLp.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
        timeLp.rightMargin = 10;
        timeLp.topMargin = 10;

        LinearLayout buttonLl = new LinearLayout(this);
        buttonLl.setOrientation(LinearLayout.VERTICAL);
        RelativeLayout.LayoutParams buttonLp = new RelativeLayout.LayoutParams
                (RelativeLayout.LayoutParams.WRAP_CONTENT,
                        RelativeLayout.LayoutParams.WRAP_CONTENT);
        buttonLp.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
        buttonLp.alignWithParent = true;
        buttonLp.leftMargin = 40;

        LinearLayout.LayoutParams btnLp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT);
        btnLp.bottomMargin = 5;

        asm=getAssets();
        mButtonZoomIn = new Button(this);

        InputStream isIn = null;
        try {
            isIn = asm.open("ZoomInBtnNormal.png");
        } catch (IOException e) {
                // TODO Auto-generated catch block
            e.printStackTrace();
        }
        mButtonZoomIn.setBackground(Drawable.createFromStream(isIn, null));

        mButtonZoomOut = new Button(this);
        InputStream isOut = null;
        try {
            isOut = asm.open("ZoomOutBtnNormal.png");
        } catch (IOException e) {
                // TODO Auto-generated catch block
            e.printStackTrace();
        }
        mButtonZoomOut.setBackground(Drawable.createFromStream(isOut, null));

        mButtonMenu = new Button(this);
        InputStream isMenu = null;
        try {
            isMenu = asm.open("BtnNormal.png");
        } catch (IOException e) {
                // TODO Auto-generated catch block
            e.printStackTrace();
        }
        mButtonMenu.setBackground(Drawable.createFromStream(isMenu, null));
        mButtonMenu.setText("菜单");
        mButtonMenu.setTextColor(Color.BLACK);

        imageView = new ImageView(this);
        InputStream isImage = null;
        try {
            isImage = asm.open("kedu.png");
        } catch (IOException e) {
                // TODO Auto-generated catch block
            e.printStackTrace();
        }
        Bitmap bitmap = BitmapFactory.decodeStream(isImage);
        imageView.setImageBitmap(bitmap);
//        imageView.setImageResource(R.mipmap.ic_launcher);
        imageView.setLayoutParams(btnLp);
        mButtonZoomIn.setLayoutParams(btnLp);
        mButtonZoomOut.setLayoutParams(btnLp);
        mButtonMenu.setLayoutParams(btnLp);
        buttonLl.addView(mButtonZoomIn);
        buttonLl.addView(mButtonZoomOut);
        buttonLl.addView(imageView);
        buttonLl.addView(mButtonMenu);

        mRl.addView(buttonLl, buttonLp);
        mRl.addView(mTimeTv, timeLp);


        ///////////////////////////////////////////////////////////

        MenuimageView = new ImageView(this);
        InputStream isMenuImage = null;
        try {
            isMenuImage = asm.open("Backgroud.png");
        } catch (IOException e) {
                // TODO Auto-generated catch block
            e.printStackTrace();
        }
        Bitmap bitmapMenuBack = BitmapFactory.decodeStream(isMenuImage);
        MenuimageView.setImageBitmap(bitmapMenuBack);

        RelativeLayout.LayoutParams menuImageLp = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT,
            RelativeLayout.LayoutParams.MATCH_PARENT);
        mRl.addView(MenuimageView, menuImageLp);
        menuImageLp.width = RelativeLayout.LayoutParams.MATCH_PARENT;
        MenuimageView.setScaleType(ScaleType.FIT_XY);
        MenuimageView.setVisibility(View.INVISIBLE);


        mButtonBack = new Button(this);
        InputStream isBtnBack = null;
        try {
            isBtnBack = asm.open("ReturnBtnNormal.png");
        } catch (IOException e) {
                // TODO Auto-generated catch block
            e.printStackTrace();
        }
        mButtonBack.setBackground(Drawable.createFromStream(isBtnBack, null));
        RelativeLayout.LayoutParams menuBackLp = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT,
            RelativeLayout.LayoutParams.WRAP_CONTENT);
        menuBackLp.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
        menuBackLp.addRule(RelativeLayout.ALIGN_PARENT_TOP);
        menuBackLp.topMargin = 20;
        menuBackLp.leftMargin = 30;
        mRl.addView(mButtonBack, menuBackLp);
        mButtonBack.setWidth(40);
        mButtonBack.setHeight(50);
        mButtonBack.setVisibility(View.INVISIBLE);

        mButtonBack.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View arg0) {
                Log.e("================", "MenuBackBtnListen");
                MenuimageView.setVisibility(View.INVISIBLE);
                mButtonBack.setVisibility(View.INVISIBLE);
                buttonCmdLl.setVisibility(View.INVISIBLE);
                mLabel.setVisibility(View.INVISIBLE);
    //            notifyQt(MSG_NO_MENU, 0, 0);
            }
        });
        mButtonBack.setOnTouchListener(new View.OnTouchListener() {
            InputStream is = null;
            @Override
            public boolean onTouch(View arg0, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN){
                    try {
                        is = asm.open("ReturnBtnPress.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }else if(event.getAction() == MotionEvent.ACTION_UP){
                    try {
                        is = asm.open("ReturnBtnNormal.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mButtonBack.setBackground(Drawable.createFromStream(is, null));
                return false;
            }
        });

        mLabel = new TextView(this);
        RelativeLayout.LayoutParams labelLp = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT,
            RelativeLayout.LayoutParams.WRAP_CONTENT);
        labelLp.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
        labelLp.addRule(RelativeLayout.ALIGN_PARENT_TOP);
        labelLp.topMargin = 20;
        labelLp.leftMargin = 150;
        mLabel.setText("高德导航");
        mLabel.setTextSize(25);
        mLabel.setTextColor(Color.BLACK);
        mRl.addView(mLabel, labelLp);
        mLabel.setVisibility(View.INVISIBLE);


        buttonCmdLl = new LinearLayout(this);
        buttonCmdLl.setOrientation(LinearLayout.VERTICAL);
        RelativeLayout.LayoutParams buttonCmdLp = new RelativeLayout.LayoutParams
                (RelativeLayout.LayoutParams.WRAP_CONTENT,
                        RelativeLayout.LayoutParams.WRAP_CONTENT);
        buttonCmdLp.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
        buttonCmdLp.alignWithParent = true;
        buttonCmdLp.leftMargin = 150;
        buttonCmdLp.bottomMargin = 40;

        LinearLayout.LayoutParams btnCmdLp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT);

        mButtonCmd1 = new Button(this);
        mButtonCmd2 = new Button(this);
        mButtonCmd3 = new Button(this);
        mButtonCmd4 = new Button(this);
        mButtonCmd5 = new Button(this);
        mButtonCmd6 = new Button(this);

        InputStream isBtnCmd = null;
        try {
            isBtnCmd = asm.open("CommandItemNormal.png");
        } catch (IOException e) {
                // TODO Auto-generated catch block
            e.printStackTrace();
        }
        mButtonCmd1.setBackground(Drawable.createFromStream(isBtnCmd, null));
        mButtonCmd2.setBackground(Drawable.createFromStream(isBtnCmd, null));
        mButtonCmd3.setBackground(Drawable.createFromStream(isBtnCmd, null));
        mButtonCmd4.setBackground(Drawable.createFromStream(isBtnCmd, null));
        mButtonCmd5.setBackground(Drawable.createFromStream(isBtnCmd, null));
        mButtonCmd6.setBackground(Drawable.createFromStream(isBtnCmd, null));

        mButtonCmd1.setLayoutParams(btnCmdLp);
        mButtonCmd2.setLayoutParams(btnCmdLp);
        mButtonCmd3.setLayoutParams(btnCmdLp);
        mButtonCmd4.setLayoutParams(btnCmdLp);
        mButtonCmd5.setLayoutParams(btnCmdLp);
        mButtonCmd6.setLayoutParams(btnCmdLp);
        buttonCmdLl.addView(mButtonCmd1);
        buttonCmdLl.addView(mButtonCmd2);
        buttonCmdLl.addView(mButtonCmd3);
        buttonCmdLl.addView(mButtonCmd4);
        buttonCmdLl.addView(mButtonCmd5);
        buttonCmdLl.addView(mButtonCmd6);

        int iBtnCmdWidth = 750;
        mButtonCmd1.setWidth(iBtnCmdWidth);
        mButtonCmd1.setHeight(60);
        mButtonCmd2.setWidth(iBtnCmdWidth);
        mButtonCmd2.setHeight(60);
        mButtonCmd3.setWidth(iBtnCmdWidth);
        mButtonCmd3.setHeight(60);
        mButtonCmd4.setWidth(iBtnCmdWidth);
        mButtonCmd4.setHeight(60);
        mButtonCmd5.setWidth(iBtnCmdWidth);
        mButtonCmd5.setHeight(60);
        mButtonCmd6.setWidth(iBtnCmdWidth);
        mButtonCmd6.setHeight(60);

        mButtonCmd1.setText("Exit 高德导航");
        mButtonCmd1.setTextSize(25);
        mButtonCmd1.setTextColor(Color.BLACK);

        mButtonCmd2.setText("开始导航");
        mButtonCmd2.setTextSize(25);
        mButtonCmd2.setTextColor(Color.BLACK);

        mButtonCmd3.setText("回车位");
        mButtonCmd3.setTextSize(25);
        mButtonCmd3.setTextColor(Color.BLACK);

        mButtonCmd4.setText("开启/关闭路况");
        mButtonCmd4.setTextSize(25);
        mButtonCmd4.setTextColor(Color.BLACK);

        mButtonCmd5.setText("回家");
        mButtonCmd5.setTextSize(25);
        mButtonCmd5.setTextColor(Color.BLACK);

        mButtonCmd6.setText("去公司");
        mButtonCmd6.setTextSize(25);
        mButtonCmd6.setTextColor(Color.BLACK);


        mRl.addView(buttonCmdLl, buttonCmdLp);
        buttonCmdLl.setVisibility(View.INVISIBLE);

        mButtonCmd1.setOnTouchListener(new View.OnTouchListener() {
            InputStream is = null;
            @Override
            public boolean onTouch(View arg0, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN){
                    try {
                        is = asm.open("CommandItemPress.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }else if(event.getAction() == MotionEvent.ACTION_UP){
                    try {
                        is = asm.open("CommandItemNormal.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mButtonCmd1.setBackground(Drawable.createFromStream(is, null));
                return false;
            }
        });
        mButtonCmd2.setOnTouchListener(new View.OnTouchListener() {
            InputStream is = null;
            @Override
            public boolean onTouch(View arg0, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN){
                    try {
                        is = asm.open("CommandItemPress.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }else if(event.getAction() == MotionEvent.ACTION_UP){
                    try {
                        is = asm.open("CommandItemNormal.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mButtonCmd2.setBackground(Drawable.createFromStream(is, null));
                return false;
            }
        });
        mButtonCmd3.setOnTouchListener(new View.OnTouchListener() {
            InputStream is = null;
            @Override
            public boolean onTouch(View arg0, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN){
                    try {
                        is = asm.open("CommandItemPress.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }else if(event.getAction() == MotionEvent.ACTION_UP){
                    try {
                        is = asm.open("CommandItemNormal.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mButtonCmd3.setBackground(Drawable.createFromStream(is, null));
                return false;
            }
        });
        mButtonCmd4.setOnTouchListener(new View.OnTouchListener() {
            InputStream is = null;
            @Override
            public boolean onTouch(View arg0, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN){
                    try {
                        is = asm.open("CommandItemPress.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }else if(event.getAction() == MotionEvent.ACTION_UP){
                    try {
                        is = asm.open("CommandItemNormal.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mButtonCmd4.setBackground(Drawable.createFromStream(is, null));
                return false;
            }
        });
        mButtonCmd5.setOnTouchListener(new View.OnTouchListener() {
            InputStream is = null;
            @Override
            public boolean onTouch(View arg0, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN){
                    try {
                        is = asm.open("CommandItemPress.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }else if(event.getAction() == MotionEvent.ACTION_UP){
                    try {
                        is = asm.open("CommandItemNormal.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mButtonCmd5.setBackground(Drawable.createFromStream(is, null));
                return false;
            }
        });
        mButtonCmd6.setOnTouchListener(new View.OnTouchListener() {
            InputStream is = null;
            @Override
            public boolean onTouch(View arg0, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN){
                    try {
                        is = asm.open("CommandItemPress.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }else if(event.getAction() == MotionEvent.ACTION_UP){
                    try {
                        is = asm.open("CommandItemNormal.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mButtonCmd6.setBackground(Drawable.createFromStream(is, null));
                return false;
            }
        });

        mButtonCmd1.setOnClickListener(new OnClickListener(){
            @Override
            public void onClick(View arg0) {
                Log.e("================", "mButtonCmd1");
                notifyQt(MSG_NO_CMD_1, 0, 0);
            }
        });
        ////////////////////////////////////////////////////
        setContentView(mRl);

        mButtonZoomIn.setWidth(80);
        mButtonZoomIn.setHeight(70);
        mButtonZoomOut.setWidth(80);
        mButtonZoomOut.setHeight(70);
        mButtonMenu.setWidth(80);
        mButtonMenu.setHeight(70);
        imageView.setMinimumWidth(90);
        imageView.setMinimumHeight(70);

        mButtonZoomIn.setOnClickListener(new OnClickListener(){
            @Override
            public void onClick(View arg0) {
                Log.e("================", "ZoomInBtnListen");
                notifyQt(MSG_NO_ZOOM_IN, 0, 0);
            }
        });
        mButtonZoomOut.setOnClickListener(new OnClickListener(){
            @Override
            public void onClick(View arg0) {
                Log.e("================", "ZoomOutBtnListen");
                notifyQt(MSG_NO_ZOOM_OUT, 0, 0);
            }
        });
        mButtonMenu.setOnClickListener(new OnClickListener(){
            @Override
            public void onClick(View arg0) {
                notifyQt(MSG_NO_MENU, 0, 0);
                Log.e("================", "MenuBtnListen");
                MenuimageView.setVisibility(View.VISIBLE);
                mButtonBack.setVisibility(View.VISIBLE);
                buttonCmdLl.setVisibility(View.VISIBLE);
                mLabel.setVisibility(View.VISIBLE);

            }
        });

        mButtonZoomIn.setOnTouchListener(new View.OnTouchListener() {

            InputStream is = null;
            @Override
            public boolean onTouch(View arg0, MotionEvent event) {
                // TODO Auto-generated method stub
                if(event.getAction() == MotionEvent.ACTION_DOWN){
                    try {
                        is = asm.open("ZoomInBtnPress.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                }else if(event.getAction() == MotionEvent.ACTION_UP){
                    try {
                        is = asm.open("ZoomInBtnNormal.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mButtonZoomIn.setBackground(Drawable.createFromStream(is, null));

                return false;
            }
        });
        mButtonZoomOut.setOnTouchListener(new View.OnTouchListener() {

            InputStream is = null;
            @Override
            public boolean onTouch(View arg0, MotionEvent event) {
                // TODO Auto-generated method stub
                if(event.getAction() == MotionEvent.ACTION_DOWN){
                    try {
                        is = asm.open("ZoomOutBtnPress.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                }else if(event.getAction() == MotionEvent.ACTION_UP){
                    try {
                        is = asm.open("ZoomOutBtnNormal.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mButtonZoomOut.setBackground(Drawable.createFromStream(is, null));

                return false;
            }
        });
        mButtonMenu.setOnTouchListener(new View.OnTouchListener() {

            InputStream is = null;
            @Override
            public boolean onTouch(View arg0, MotionEvent event) {
                // TODO Auto-generated method stub
                if(event.getAction() == MotionEvent.ACTION_DOWN){
                    try {
                        is = asm.open("BtnPress.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                }else if(event.getAction() == MotionEvent.ACTION_UP){
                    try {
                        is = asm.open("BtnNormal.png");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mButtonMenu.setBackground(Drawable.createFromStream(is, null));

                return false;
            }
        });


        handler.postDelayed(updateThread, 50);
        timeHandler.postDelayed(updateTimeThread, 1000);

        PlayerThread p = new PlayerThread();
        p.start();
    }
    Handler handler = new Handler();
    Runnable updateThread = new Runnable() {

        public void run() {
            initDecoder();
        }
    };

    Handler timeHandler = new Handler();
    Runnable updateTimeThread = new Runnable() {

        public void run() {
            // TODO Auto-generated method stub
            SimpleDateFormat df = new SimpleDateFormat("HH:mm:ss");
            String date = df.format(new java.util.Date());
            mTimeTv.setText(date);
            timeHandler.postDelayed(updateTimeThread, 1000);
        }
    };
    private class PlayerThread extends Thread {

        @Override
        public void run() {
            FrameStruct frame;
            while(true)
            {
                if(bCanFlush)
                {
                    lock.lock();
                    while((frame=frameQueue.poll())!=null)
                    {
//                        Log.e(TAG, "frame.len = " +  frame.len + "; frameQueue.size() = " + frameQueue.size());
                        onFrame(frame.buf, 0, frame.len);
                    }
                    lock.unlock();
                }
            }
        }
    }
    static int findFirstFrame = 0;
    static int byteCount = 4;
    static byte[] frameData = new byte[WIDTH_DECODER * HEIGHT_DECODER * 3 / 2];
    static int lastIndex = 0;
    private static class socketThread extends Thread {

        @Override
        public void run() {

            ////////////file
//            int fileLength = 0;
//            int offset = 0;
//            int readLen = 50;
//            byte [] fileBuffer = null;
//            if (mFileInput == null) {
//                try {
//                    mFileInput = new RandomAccessFile(new File(SAMPLE), "r");
//                } catch (FileNotFoundException e) {
//                    // TODO Auto-generated catch block
//                    e.printStackTrace();
//                }
//            }
//            try {
//                fileLength = (int)mFileInput.length();
//                fileBuffer = new byte[fileLength];
//                Log.i("yflog", "read file length:" + fileLength);
//                mFileInput.read(fileBuffer);
//            } catch (IOException e1) {
//                // TODO Auto-generated catch block
//                e1.printStackTrace();
//            }
            ////////////////

            String host = "127.0.0.1";
            int port = 5050;

            Socket client;
            DataInputStream readIn;
            byte[] recvBuf = new byte[WIDTH_DECODER * HEIGHT_DECODER * 3 / 2];
            int dataLen;
            try {
                client = new Socket(host, port);
                readIn = new DataInputStream(client.getInputStream());

//                while(offset < fileLength)
//                {
//                    dataLen = readLen;
//                    System.arraycopy(fileBuffer, offset, recvBuf, 0, readLen);
//                    offset += readLen;
//                    Log.e(TAG, "dataLen = " + dataLen);
                while((dataLen = readIn.read(recvBuf)) > 0)
                {
                    frameData[0] = 0;
                    frameData[1] = 0;
                    frameData[2] = 0;
                    frameData[3] = 1;

                    int nPos = 0;
                    int index = 0;
                    int flag = 0;

                    int canOnFrame = 0;
                    while (nPos < dataLen) {

                        canOnFrame = 0;
                        while (nPos < dataLen) {
                            flag = recvBuf[nPos++];
                            if(findFirstFrame == 1)
                            {
                                frameData[byteCount++] = (byte)flag;
                            }
                            if (flag == 0)
                            {
                                index = lastIndex + 1;
                                while (flag == 0)
                                {
                                    if (nPos < dataLen)
                                    {
                                        lastIndex = 0;
                                        flag = recvBuf[nPos++];
                                        if(findFirstFrame == 1)
                                        {
                                            frameData[byteCount++] = (byte)flag;
                                        }
                                        index++;
                                    }
                                    else
                                    {
                                        lastIndex = index;
                                        break;
                                    }
                                }
                                if (flag == 1 && index >= 4)
                                {
                                    if(findFirstFrame == 0)
                                        findFirstFrame = 1;
                                    else
                                    {
                                        byteCount = byteCount - 4;
                                        canOnFrame = 1;
                                    }

                                    break;
                                }
                            }
                        }
                        if(canOnFrame == 1)
                        {
                            FrameStruct frame = new FrameStruct();
                            frame.buf = new byte[byteCount];
                            frame.len = byteCount;
                            System.arraycopy(frameData, 0, frame.buf, 0, byteCount);
                            lock.lock();
                            frameQueue.offer(frame);
                            lock.unlock();
//                            onFrame(frameData, 0, byteCount);
                            byteCount = 4;
                        }
                    }

                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {

                    }
                }

                readIn.close();
                client.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


    private static void initDecoder() {
        Log.e("================", "initDecoder");
        mMediaCodec = MediaCodec.createDecoderByType("video/avc");
        MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc",
                        WIDTH_DECODER, HEIGHT_DECODER);
        mMediaCodec.configure(mediaFormat, mSurfaceView.getHolder().getSurface(), null, 0);
        mMediaCodec.setVideoScalingMode(MediaCodec.VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING);
        mMediaCodec.start();
        bCanFlush = true;
//        notifyQt(MSG_NO_CAN_FLUSH, 0, 0);
    }  

    public static boolean onFrame(byte[] buf, int offset, int length) {
        long time = System.currentTimeMillis();
        ByteBuffer[] inputBuffers = mMediaCodec.getInputBuffers();
        int inputBufferIndex = mMediaCodec.dequeueInputBuffer(-1);
        if (inputBufferIndex >= 0) {
//            Log.e("Media", "inputBufferIndex:"+inputBufferIndex);
            ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
            inputBuffer.clear();
            inputBuffer.put(buf, offset, length);
            mMediaCodec.queueInputBuffer(inputBufferIndex, 0, length, 0, MediaCodec.BUFFER_FLAG_SYNC_FRAME);
        }
        else
        {
            return false;
        }

        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
        int outputBufferIndex = mMediaCodec.dequeueOutputBuffer(bufferInfo, 0);

        while (outputBufferIndex >= 0) {
//            Log.e("Media", "outputBufferIndex:"+outputBufferIndex);
            mMediaCodec.releaseOutputBuffer(outputBufferIndex, true);
            outputBufferIndex = mMediaCodec.dequeueOutputBuffer(bufferInfo, 0);
        }

        return true;
    }

    //Qt=============
    public static ExtendsQtNative m_nativeNotify = null;
    private static void notifyQt(int msgNo, int x, int y){
        if(m_nativeNotify == null){
            m_nativeNotify = new ExtendsQtNative();
        }
        m_nativeNotify.notifyMsg(msgNo, x, y);
    }

    public static void start() {
        Log.e("Media", "==start");
        socketThread s = new socketThread();
        s.start();
    }

    public static void stop() {
        Log.e("Media", "==stop");

        if(instance != null)
        {
            instance.finish();
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        // TODO Auto-generated method stub

        int pCount = event.getPointerCount();
        if(event.getPointerCount() == 1)
        {
            float x = event.getX();
            float y = event.getY();
            int i = event.getAction();
            switch (event.getAction() & MotionEvent.ACTION_MASK)
            {
            case MotionEvent.ACTION_MOVE:
                Log.e(TAG, "MotionEvent.ACTION_MOVE:x=" + x + ";y=" + y);
                notifyQt(MSG_NO_MOVE, (int)x, (int)y);
                break;
            case MotionEvent.ACTION_UP:
                Log.e(TAG, "MotionEvent.ACTION_POINTER_UP:x=" + x + ";y=" + y);
                notifyQt(MSG_NO_RELEASE, (int)x, (int)y);
                break;
            case MotionEvent.ACTION_DOWN:
                Log.e(TAG, "MotionEvent.ACTION_POINTER_DOWN:x=" + x + ";y=" + y);
                notifyQt(MSG_NO_PRESS, (int)x, (int)y);
                break;
            default:
                break;
           }
        }
        return super.onTouchEvent(event);
    }

   @Override
   protected void onPause() {
       // TODO Auto-generated method stub
       Log.e(TAG, "===================onPause");
       super.onPause();
   }

   @Override
   protected void onRestart() {
       // TODO Auto-generated method stub
       Log.e(TAG, "===================onRestart");
       super.onRestart();
   }

   @Override
   protected void onResume() {
       // TODO Auto-generated method stub
       Log.e(TAG, "===================onResume");
       super.onResume();
   }

   @Override
   protected void onStart() {
       // TODO Auto-generated method stub
       Log.e(TAG, "===================onStart");
       super.onStart();
   }

   @Override
   protected void onStop() {
       // TODO Auto-generated method stub
       Log.e(TAG, "===================onStop");
       mMediaCodec.stop();

       super.onStop();
   }
   @Override
   protected void onDestroy() {
      // TODO Auto-generated method stub
      Log.e(TAG, "===================onDestroy");
      mMediaCodec.release();
      super.onDestroy();
   }
}

