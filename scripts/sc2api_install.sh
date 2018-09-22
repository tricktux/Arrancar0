# File:           sc2api_install.sh
# Description:    Instructions to install sc2api to /opt/
# Author:		    Reinaldo Molina
# Email:          rmolin88 at gmail dot com
# Revision:	    0.0.0
# Created:        Mon May 14 2018 05:56
# Last Modified: Sat Sep 22 2018 06:14

# NOTE: Modify here to the path to the built root folder of:
# [s2client-api](https://github.com/Blizzard/s2client-api)
cd ~/Documents/ML_SC2/s2client-api
echo "Downloding submodules"
git submodule update --recursive --init

echo "Building. This may take a minute..."
mkdir build
cd build
cmake .. || exit 1
make -j$(nproc) || exit 2
cd ..

echo "Installing SC2 API headers"
cp -R include/sc2api ~/.local/include
cp -R include/sc2utils ~/.local/include
cp -R include/sc2lib ~/.local/include
cp -R include/sc2renderer ~/.local/include
cp -R build/generated/s2clientprotocol ~/.local/include

echo "Installing protobuf headers"
cp -R contrib/protobuf/src/google ~/.local/include/sc2api

echo "Installing SC2 API libraries"
mkdir -p ~/.local/lib/sc2api
cp build/bin/*.a ~/.local/lib/sc2api

echo "Generating doxygen docs"
doxygen Doxyfile
