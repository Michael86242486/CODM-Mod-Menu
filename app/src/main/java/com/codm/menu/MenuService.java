package com.codm.menu;

import android.app.Service;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.IBinder;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;

public class MenuService extends Service {

    private WindowManager windowManager;
    private View overlayView;

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        windowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (overlayView == null) {
            LayoutInflater inflater = LayoutInflater.from(this);
            overlayView = inflater.inflate(R.layout.overlay_menu, null);

            int flags_l;
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                flags_l = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
            } else {
                flags_l = WindowManager.LayoutParams.TYPE_PHONE;
            }

            WindowManager.LayoutParams params = new WindowManager.LayoutParams(
                    WindowManager.LayoutParams.WRAP_CONTENT,
                    WindowManager.LayoutParams.WRAP_CONTENT,
                    flags_l,
                    WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
                    PixelFormat.TRANSLUCENT
            );

            params.gravity = Gravity.TOP | Gravity.LEFT;
            params.x = 0;
            params.y = 100;

            windowManager.addView(overlayView, params);

            overlayView.setOnTouchListener(new View.OnTouchListener() {
                private int initialX, initialY;
                private float initialTouchX, initialTouchY;
                private boolean isDragging = false;
                private long lastClickTime = 0;

                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    switch (event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            initialX = params.x;
                            initialY = params.y;
                            initialTouchX = event.getRawX();
                            initialTouchY = event.getRawY();
                            isDragging = false;
                            return true;

                        case MotionEvent.ACTION_MOVE:
                            int diffX = (int) (event.getRawX() - initialTouchX);
                            int diffY = (int) (event.getRawY() - initialTouchY);
                            if (Math.abs(diffX) > 5 || Math.abs(diffY) > 5) {
                                isDragging = true;
                                params.x = initialX + diffX;
                                params.y = initialY + diffY;
                                windowManager.updateViewLayout(overlayView, params);
                            }
                            return true;

                        case MotionEvent.ACTION_UP:
                            if (!isDragging) {
                                long now = System.currentTimeMillis();
                                if (now - lastClickTime < 300) {
                                    // Double tap – toggle visibility
                                    View menuPanel = overlayView.findViewById(R.id.menuPanel);
                                    menuPanel.setVisibility(
                                            menuPanel.getVisibility() == View.VISIBLE ?
                                                    View.GONE : View.VISIBLE);
                                }
                                lastClickTime = now;
                            }
                            return true;
                    }
                    return false;
                }
            });
        }
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (overlayView != null) {
            windowManager.removeView(overlayView);
            overlayView = null;
        }
    }
}