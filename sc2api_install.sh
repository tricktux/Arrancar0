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

# Install SC2 API headers
sudo mkdir -p /opt/local/include
sudo cp -R include/sc2api /opt/local/include
sudo cp -R include/sc2utils /opt/local/include
sudo cp -R build/generated/s2clientprotocol /opt/local/include

# Install protobuf headers
sudo cp -R contrib/protobuf/src/google /opt/local/include/sc2api

# Install SC2 API libraries
sudo mkdir -p /opt/local/lib/sc2api
sudo cp build/bin/libcivetweb.a /opt/local/lib/sc2api
sudo cp build/bin/libprotobuf.a /opt/local/lib/sc2api
sudo cp build/bin/libsc2api.a /opt/local/lib/sc2api
sudo cp build/bin/libsc2lib.a /opt/local/lib/sc2api
sudo cp build/bin/libsc2protocol.a /opt/local/lib/sc2api
sudo cp build/bin/libsc2utils.a /opt/local/lib/sc2api
