package com.hounjini;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;

public class bootstrap extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
    
        Button recoveryBtn = (Button)findViewById(R.id.reboot_recovery);
        recoveryBtn.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
        		String rebootToRecovery = "/system/persistent/orbootstrap/openrecovery.sh";
        		AlertDialog.Builder builder = new Builder(bootstrap.this);
        		builder.setPositiveButton(android.R.string.ok, null);
        		try {
                    Helper.runSuCommand(bootstrap.this, rebootToRecovery);
                    builder.setTitle("Reboot to Recovery Failed!");
        		}
                catch (Exception e) {
                	builder.setTitle("Unexpected Error Occurred!");
                	builder.setMessage(e.getMessage());
                	e.printStackTrace();
                }
                builder.create().show();
        	}
        });
        
        Button installBtn = (Button)findViewById(R.id.install);
        installBtn.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
        		StringBuilder installScript = new StringBuilder();
        		installScript.append("cp /sdcard/openrecovery/orbootstrap/install_script.sh /tmp;");
        		installScript.append("chmod 755 /tmp/install_script.sh;");
        		installScript.append("/tmp/install_script.sh;");
        		AlertDialog.Builder builder = new Builder(bootstrap.this);
        		builder.setPositiveButton(android.R.string.ok, null);
        		try {
                    Helper.runSuCommand(bootstrap.this, installScript.toString());
                    builder.setMessage("Installation Success!");
                }
                catch (Exception e) {
                	builder.setTitle("Installation Failed!");
                	builder.setMessage(e.getMessage());
                	e.printStackTrace();
                }
                builder.create().show();
        	}
        });

        Button uninstallBtn = (Button)findViewById(R.id.uninstall);
        uninstallBtn.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
        		StringBuilder uninstallScript = new StringBuilder();
        		uninstallScript.append("mount -o rw,remount system /system;");
        		uninstallScript.append("mv /system/bin/sh.bin /system/bin/sh;");
        		uninstallScript.append("rm -r /system/persistent;");
        		uninstallScript.append("mount -o ro,remount system /system;");
        		AlertDialog.Builder builder = new Builder(bootstrap.this);
        		builder.setPositiveButton(android.R.string.ok, null);
        		try {
                    Helper.runSuCommand(bootstrap.this, uninstallScript.toString());
                    builder.setMessage("Remove Success!");
                }
                catch (Exception e) {
                	builder.setTitle("Remove Failed!");
                	builder.setMessage(e.getMessage());
                	e.printStackTrace();
                }
                builder.create().show();
        	}
        });
    }
}