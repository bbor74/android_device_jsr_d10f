build_root=$(pwd) # vendorsetup.sh is sourced by build/envsetup.sh in root of android build tree. Hope that nobody can correctly source it not from root of android tree.

rm -f out/target/product/d10f/system/build.prop out/target/product/d10f/boot/root/default.prop out/target/product/d10f/recovery/root/default.prop

add_lunch_combo lineage_d10f-user
add_lunch_combo lineage_d10f-userdebug
add_lunch_combo lineage_d10f-eng
