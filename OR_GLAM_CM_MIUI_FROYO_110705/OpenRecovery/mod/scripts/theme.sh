#/system/bin/sh

case $1 in
	install)
		echo "...install theme"
		mkdir -p /sdcard/OpenRecovery/backup/theme/bootanimation
		cp -r /system/media/bootanimation.zip /sdcard/OpenRecovery/backup/bootanimation/bootanimation.zip 
		cp -r /sdcard/OpenRecovery/theme/eclair/bootanimation/bootanimation.zip /system/media/bootanimation.zip
		chmod 644 /system/media/bootanimation.zip
		chown -R 0.2000 /system/media

		mkdir -p /sdcard/OpenRecovery/backup/theme/framework
		cp -f /system/framework/framework-res.apk /sdcard/OpenRecovery/backup/theme/framework/
		cp -f /sdcard/OpenRecovery/theme/eclair/framework/framework-res.apk /system/framework/framework-res.apk
		chmod 644 /system/framework/framework-res.apk
		cp -f /system/framework/services.jar /sdcard/OpenRecovery/backup/theme/framework/
		cp -f /sdcard/OpenRecovery/theme/eclair/framework/services.jar /system/framework/services.jar
		chmod 644 /system/framework/services.jar
		mkdir -p /sdcard/OpenRecovery/backup/theme/app
		for APK in "/sdcard/OpenRecovery/theme/eclair/apps/"*.apk; do
		BASE_APK="$(basename $APK)"
		cp -f /system/app/$BASE_APK /sdcard/OpenRecovery/backup/theme/app/
		cp -f /sdcard/OpenRecovery/theme/eclair/apps/$BASE_APK /system/app/$BASE_APK
		chmod 644 /system/app/$BASE_APK
		done
		mkdir -p /sdcard/OpenRecovery/backup/theme/fonts
		for FONT in "/sdcard/OpenRecovery/theme/eclair/fonts/"*.ttf; do
		BASE_FONT="$(basename $FONT)"
		cp -f /system/fonts/$BASE_FONT /sdcard/OpenRecovery/backup/theme/fonts/
		cp -f /sdcard/OpenRecovery/theme/eclair/fonts/$BASE_FONT /system/fonts/$BASE_FONT
		chmod 644 /system/fonts/$BASE_FONT
		done
		cp -f /sdcard/OpenRecovery/theme/eclair/wallpaper /data/data/com.android.settings/files/wallpaper
		chmod 777 /data/data/com.android.settings/files/wallpaper
		sleep 1;;


	restore)
		echo "...restore theme"
		rm /system/media/bootanimation.zip
		cp -r /sdcard/OpenRecovery/backup/bootanimation/bootanimation.zip /system/media/bootanimation.zip 
		chmod 644 /system/media/bootanimation.zip
		chown -R 0.2000 /system/media
		cp -f /sdcard/OpenRecovery/backup/theme/framework/framework-res.apk /system/framework/framework-res.apk
		chmod 644 /system/framework/framework-res.apk
		cp -f /sdcard/OpenRecovery/backup/theme/framework/services.jar /system/framework/services.jar
		chmod 644 /system/framework/services.jar
		for APK in "/sdcard/OpenRecovery/backup/theme/app/"*.apk; do
		BASE_APK="$(basename $APK)"
		cp -f /sdcard/OpenRecovery/backup/theme/app/$BASE_APK /system/app/$BASE_APK
		chmod 644 /system/app/$BASE_APK
		done
		for FONT in "/sdcard/OpenRecovery/backup/theme/fonts/"*.ttf; do
		BASE_FONT="$(basename $FONT)"
		cp -f /sdcard/OpenRecovery/backup/theme/fonts/$BASE_FONT /system/fonts/$BASE_FONT
		chmod 644 /system/fonts/$BASE_FONT
		done
		sleep 1;;
esac
