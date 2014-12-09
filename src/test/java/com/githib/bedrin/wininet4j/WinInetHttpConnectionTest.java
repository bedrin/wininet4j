package com.githib.bedrin.wininet4j;

import org.junit.Test;

import java.net.URL;

import static org.junit.Assert.*;

public class WinInetHttpConnectionTest {

    @Test
    public void testConnect() throws Exception {
        URL url = new URL("https://www.rsdn.bugogaru/");
        WinInetHttpConnection urlc = new WinInetHttpConnection(url);
        urlc.connect();
        urlc.getInputStream();
    }

}