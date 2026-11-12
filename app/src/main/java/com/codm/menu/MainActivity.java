package com.codm.menu;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.provider.Settings;
import android.widget.Toast;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("Menu");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (Settings.canDrawOverlays(this)) {
            startService(new Intent(this, MenuService.class));
            finish();
        } else {
            Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION);
            startActivity(intent);
            Toast.makeText(this, "Please grant overlay permission", Toast.LENGTH_LONG).show();
        }
    }
}