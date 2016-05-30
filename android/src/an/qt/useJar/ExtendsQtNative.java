package an.qt.useJar;
import java.lang.String;

public class ExtendsQtNative
{
    public native void sendByte(int len, byte[] date);
    public native void notifyMsg(int msgNo, int x, int y);
    public native void setDirectBuffer(Object buffer, int len);
}
