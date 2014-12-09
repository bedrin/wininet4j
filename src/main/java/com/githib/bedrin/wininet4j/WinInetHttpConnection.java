package com.githib.bedrin.wininet4j;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class WinInetHttpConnection extends HttpURLConnection {

    static {
        try {
            System.loadLibrary("wininet4j");
            initImpl();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @SuppressWarnings("unused")
    private static long hInternet;

    @SuppressWarnings("unused")
    private long hConnect;

    private long hRequest;

    private static native void initImpl();
    private native void connectImpl() throws IOException;
    private native void disconnectImpl();
    private native long openRequestImpl() throws IOException;
    private native boolean sendRequestImpl(long hRequest) throws IOException;

    public WinInetHttpConnection(URL u) {
        super(u);
    }

    @Override
    public void disconnect() {
        disconnectImpl();
    }

    @Override
    public boolean usingProxy() {
        return false;
    }

    @Override
    public void connect() throws IOException {
        connectImpl();
    }

    @Override
    public InputStream getInputStream() throws IOException {
        openRequest();
        return super.getInputStream();
    }

    protected boolean openRequest() throws IOException {
        hRequest = openRequestImpl();
        return sendRequestImpl(hRequest);
    }

}
