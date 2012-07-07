#!/bin/bash
#
#Author :Tharindra Galahena
#Project:wm_black - a simple window manager in linux 
#Date   :07/07/2012
#

BASE="/usr/local/bin"

echo ""
echo ""
echo "------------------------------wm_black--------------------------------"
echo ""
echo ""
echo "creating desktop file................."

echo "
[Desktop Entry]
Name=wm_black
Icon=
Exec=$BASE/wm_black
Type=XSession
" > /usr/share/xsessions/wm_black.desktop

echo ""
echo "file created................"
echo ""
