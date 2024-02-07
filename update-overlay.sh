#!/bin/sh

MYABSPATH=$(readlink -f "$0")
PATCHBASE=$(dirname "$MYABSPATH")

cd "$PATCHBASE"

mkdir -p overlay/packages/apps/Settings/res/xml/

xmlstarlet ed --pf --ps \
-u '/PreferenceScreen/Preference[@android:key="additional_system_update_settings"]/@android:title' -v "@string/wifi_show_advanced" \
../../../packages/apps/Settings/res/xml/device_info_settings.xml > overlay/packages/apps/Settings/res/xml/device_info_settings.xml

xmlstarlet ed --pf --ps \
-u '/PreferenceScreen/Preference[@android:key="additional_system_update_settings"]/@android:title' -v "@string/wifi_show_advanced" \
../../../packages/apps/Settings/res/xml/system_dashboard_fragment.xml > overlay/packages/apps/Settings/res/xml/system_dashboard_fragment.xml
