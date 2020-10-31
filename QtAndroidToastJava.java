package com.amin.QtAndroidToastJava;

import org.qtproject.qt5.android.bindings.QtApplication;
import org.qtproject.qt5.android.bindings.QtActivity;

import android.content.Context;
import android.content.Intent;

import android.os.Bundle;
import android.os.Looper;
import android.os.Handler;

import android.widget.Toast;


public class QtAndroidToastJava extends QtActivity
{
    private static QtAndroidToastJava thisInstance;

    public QtAndroidToastJava()
    {
        thisInstance = this;
    }

    public static void sMakeToast(String msg)
    {
        thisInstance.makeToast(msg);
    }

    private void makeToast(String msg)
    {
        final String tmpStr = msg;
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                Toast.makeText(QtAndroidToastJava.this, tmpStr, Toast.LENGTH_LONG).show();
            }
        });
    }
}
