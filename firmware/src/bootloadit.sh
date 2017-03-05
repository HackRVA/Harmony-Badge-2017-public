PROGRAMMER_BIN=./pic32usb
HEX_PATH='../harmony_badge_2017.X/dist/pic32mx_usb_badge_int_dyn/production/harmony_badge_2017.X.production.hex'

if [ ! -f "$HEX_PATH" ]; then
    echo "Hex not found!"
    echo "Check that compilation was successful and that this script is in src/"
fi

md5sum $HEX_PATH

#ls -lt ../freertosBadge.X/dist/pic32mx_usb_badge_int_dyn/production/freertosBadge.X.production.hex

$PROGRAMMER_BIN $HEX_PATH
