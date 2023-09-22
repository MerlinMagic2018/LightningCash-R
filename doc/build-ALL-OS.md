

LightningCashr Build Instructions for any OS
--------------------------------


---> Compile from Ubuntu 18.04 ---> Necessary steps for ANY OS :


sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils curl git wget python3

sudo apt-get install libboost-all-dev
^^^
If this does not work, try this instead :

sudo apt-get install -y libboost-system-dev libboost-filesystem-dev libboost-chrono1.65-dev libboost-program-options-dev libboost-thread-dev

sudo apt-get install software-properties-common

sudo add-apt-repository -y ppa:luke-jr/bitcoincore

sudo apt-get update

sudo apt-get install libdb4.8-dev libdb4.8++-dev

sudo apt-get install libminiupnpc-dev

sudo apt-get install libqrencode-dev

GUI build : To build with Qt 5 (recommended) you need the following:

    sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler

Alternatively, to build with Qt 4 (NOT recommended) you need the following:

    sudo apt-get install libqt4-dev libprotobuf-dev protobuf-compiler

sudo apt install git

git clone https://github.com/diabaths/LightningCashr-Coin

cd LightningCashr-Coin


-------------------------------------------------------------------------

## Building for 64-bit Windows
------------

sudo apt install g++-mingw-w64-x86-64

sudo add-apt-repository "deb http://archive.ubuntu.com/ubuntu zesty universe"

sudo apt update

sudo apt upgrade

sudo update-alternatives --config x86_64-w64-mingw32-g++ # Set the default mingw32 g++ compiler option to posix.

PATH=$(echo "$PATH" | sed -e 's/:\/mnt.*//g') # strip out problematic Windows %PATH% imported var

cd depends

make HOST=x86_64-w64-mingw32 # You can add -jx where x is the number of core/threads to speed up compilation

cd ..

./autogen.sh

./configure --disable-bench --disable-tests --disable-gui-tests --disable-zmq --prefix=$PWD/depends/x86_64-w64-mingw32


make    # You can add -jx where x is the number of core/threads to speed up compilation

Done.

daemon, cli and tx in src folder, GUI ( qt ) in src/qt folder.
---------------------------------------------------------------------------




## Building for 32-bit Windows
----------

sudo apt-get install g++-mingw-w64-i686 mingw-w64-i686-dev

sudo apt-get update

sudo update-alternatives --config i686-w64-mingw32-g++  # Set the default mingw32 g++ compiler option to posix.

sudo apt-get install git

sudo apt-get install curl

git clone https://github.com/diabaths/LightningCashr-Coin

sudo chmod -R a+rw LightningCashr-Coin

PATH=$(echo "$PATH" | sed -e 's/:\/mnt.*//g') # strip out problematic Windows %PATH% imported var

cd LightningCashr-Coin

cd depends

make HOST=i686-w64-mingw32   # You can add -jx where x is the number of core/threads to speed up compilation

cd ..

./autogen.sh

./configure --disable-bench --disable-tests --disable-gui-tests --disable-zmq --prefix=$PWD/depends/i686-w64-mingw32

# If the windows GUI wallet seems to have errors in it, compile with this instead :

CONFIG_SITE=$PWD/depends/i686-w64-mingw32/share/config.site ./configure --disable-bench --disable-tests --disable-gui-tests --diable-zmq --prefix=/

make      # You can add -jx where x is the number of core/threads to speed up compilation

Done.

daemon, cli and tx in src folder, GUI ( qt ) in src/qt folder.

---------------------------------------------------------------------------

OSX ( Apple )
----------

sudo apt-get update

sudo apt-get install librsvg2-bin libtiff-tools bsdmainutils cmake imagemagick libcap-dev libz-dev libbz2-dev python-setuptools


# Not 100 % sure which one to use...

sudo apt install python-pip
sudo apt install python3-pip python-pip-whl python3-setuptools python3-wheel


pip install setuptools

sudo apt install cmake

sudo apt install libcap-dev

sudo apt install libbz2-dev



git clone https://github.com/diabaths/LightningCashr-Coin

cd LightningCashr-Coin

cd depends

mkdir SDKs

cd SDKs

sudo apt install wget

wget https://github.com/phracker/MacOSX-SDKs/releases/download/10.13/MacOSX10.11.sdk.tar.xz

# If ^^^ is not available anymore, search for MacOSX10.11.sdk.tar.xz on google.

tar -xf MacOSX10.11.sdk.tar.xz

cd ../..

make -C depends HOST=x86_64-apple-darwin11     # You can add -jx where x is the number of core/threads to speed up compilation

./autogen.sh

./configure --prefix=$PWD/depends/x86_64-apple-darwin11 --disable-tests --disable-bench --disable-gui-tests --disable-zmq

make      # You can add -jx where x is the number of core/threads to speed up compilation

Done.

lightningcashrd, lightningcashr-cli and lightningcashr-tx are in src folder, GUI ( qt ) in src/qt folder.

# You can also create a .dmg that contains the .app bundle (optional):

make deploy

# Installation into user directories (optional):

sudo make install

    or

sudo cd ~/LightningCashr-Coin/src
sudo cp lightningcashrd /usr/local/bin/
sudo cp lightningcashr-cli /usr/local/bin/

--------------------------------------------------------------------------

Linux ( linux-gnu, usable on ANY linux ):
------

sudo apt-get update

git clone https://github.com/diabaths/LightningCashr-Coin

cd LightningCashr-Coin

cd depends

make HOST=x86_64-pc-linux-gnu

cd ..

./autogen.sh

./configure --disable-tests --disable-bench --disable-gui-tests --disable-zmq --prefix=$PWD/depends/x86_64-pc-linux-gnu

make     # You can add -jx where x is the number of core/threads to speed up compilation

sudo make install

Done.

Binaries in depends/x86_64-pc-linux-gnu/bin/

You need both the "bin" and the "lib" folders, together in a folder, for the binaries to work.

----------------------------------------------------------------------------
