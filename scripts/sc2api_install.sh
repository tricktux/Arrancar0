# File:           sc2api_install.sh
# Description:    Instructions to install sc2api to /opt/
# Author:		    Reinaldo Molina
# Email:          rmolin88 at gmail dot com
# Revision:	    0.0.0
# Created:        Mon May 14 2018 05:56
# Last Modified:  Mon May 14 2018 05:56

# NOTE: Modify here to the path to the built root folder of:
# [s2client-api](https://github.com/Blizzard/s2client-api)
cd ~/Documents/ML_SC2/s2client-api
git submodule update --recursive --init
mkdir build
cd build
cmake .. || exit 1
make -j8 || exit 2
cd ..

# Install SC2 API headers
cp -R include/sc2api ~/.local/include
cp -R include/sc2utils ~/.local/include
cp -R build/generated/s2clientprotocol ~/.local/include

# Install protobuf headers
cp -R contrib/protobuf/src/google ~/.local/include/sc2api

# Install SC2 API libraries
mkdir -p ~/.local/lib/sc2api
cp build/bin/libcivetweb.a ~/.local/lib/sc2api
cp build/bin/libprotobuf.a ~/.local/lib/sc2api
cp build/bin/libsc2api.a ~/.local/lib/sc2api
cp build/bin/libsc2lib.a ~/.local/lib/sc2api
cp build/bin/libsc2protocol.a ~/.local/lib/sc2api
cp build/bin/libsc2utils.a ~/.local/lib/sc2api
