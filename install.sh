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
echo "starting compilation............"

gcc -o wm_black wm_black_main.c wm_black.c config_read.c share.c grapics.c -lX11
gcc -o base_win base_win_main.c base_win.c  config_read.c share.c grapics.c -lX11
gcc -o clock clock_main.c config_read.c share.c grapics.c -lX11
gcc -o launcher launcher_main.c launcher.c config_read.c share.c grapics.c -lX11
gcc -o minimize minimize_main.c minimize.c share.c grapics.c -lX11

echo ""
echo "compilaion finished............."
echo "start copying..............."

cp background.bmp $HOME
cp icon.bmp $HOME
cp apps $HOME
cp apps_head $HOME

sudo cp wm_black $BASE
sudo cp base_win $BASE
sudo cp clock $BASE
sudo cp launcher $BASE
sudo cp minimize $BASE

echo ""
echo "copying finished..........."
echo "installation finished.............."


