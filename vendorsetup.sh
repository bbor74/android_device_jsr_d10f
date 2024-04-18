build_root=$(pwd) # vendorsetup.sh is sourced by build/envsetup.sh in root of android build tree. Hope that nobody can correctly source it not from root of android tree.

echo "Updating overlay"
sh device/jsr/d10f/update-overlay.sh

rm -f out/target/product/d10f/system/build.prop out/target/product/d10f/boot/root/default.prop out/target/product/d10f/recovery/root/default.prop

