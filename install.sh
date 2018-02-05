#!/bin/sh

echo
echo "===== Compiling =====" &&
make &&

echo
echo "===== Installing (need root) =====" &&
sudo make install &&

echo
echo "===== Starting configuration =====" &&
yacm setup &&

echo
echo "===== All done! ====="