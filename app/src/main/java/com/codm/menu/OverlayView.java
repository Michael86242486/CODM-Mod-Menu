package com.codm.menu;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.Switch;
import android.widget.TextView;

public class OverlayView {

    private View rootView;
    private Context context;

    // Native methods
    public static native void NativeToggle(int feature, boolean enabled);

    public OverlayView(Context context, View parent) {
        this.context = context;
        this.rootView = parent;
        initControls();
    }

    private void initControls() {
        // Wire up switches to native toggles
        int[] switchIds = {
            R.id.switch_aimbot,
            R.id.switch_esp,
            R.id.switch_wallhack,
            R.id.switch_no_recoil,
            R.id.switch_no_spread,
            R.id.switch_magic_bullet,
            R.id.switch_radar,
            R.id.switch_speedhack,
            R.id.switch_god_mode,
            R.id.switch_unlimited_ammo
        };

        for (int i = 0; i < switchIds.length; i++) {
            Switch sw = rootView.findViewById(switchIds[i]);
            if (sw != null) {
                final int featureIndex = i;
                sw.setOnCheckedChangeListener((buttonView, isChecked) -> {
                    NativeToggle(featureIndex, isChecked);
                });
            }
        }
    }
}