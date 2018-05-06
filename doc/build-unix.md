Copyright (c) 2009-2012 Bitcoin Developers
Distributed under the MIT/X11 software license, see the accompanying
file license.txt or http://www.opensource.org/licenses/mit-license.php.
This product includes software developed by the OpenSSL Project for use in
the OpenSSL Toolkit (http://www.openssl.org/).  This product includes
cryptographic software written by Eric Young (eay@cryptsoft.com) and UPnP
software written by Thomas Bernard.


UNIX BUILD NOTES
================

Dependencies
---------------------

These dependencies are required:

 Library     | Purpose          | Description
 ------------|------------------|----------------------
 libssl      | Crypto           | Random Number Generation
 libgmp      | Secp256k1        | Secp256k1 Dependency
 libboost    | Utility          | Library for threading, data structures, etc
 libevent    | Networking       | OS independent asynchronous networking
 libdb4.8    | Berkeley DB      | Wallet storage (only needed when wallet enabled)
 libsecp256k1| Secp256k1        | Elliptic Curve Cryptography

Optional dependencies:

 Library     | Purpose          | Description
 ------------|------------------|----------------------
 miniupnpc   | UPnP Support     | Firewall-jumping support
 qt          | GUI              | GUI toolkit (only needed when GUI enabled)
 protobuf    | Payments in GUI  | Data interchange format used for payment protocol (only needed when GUI enabled)
 libqrencode | QR codes in GUI  | Optional for generating QR codes (only needed when GUI enabled)

For the versions used in the release, see [release-process.md](release-process.md) under *Fetch and build inputs*.

System requirements
--------------------

C++ compilers are memory-hungry. It is recommended to have at least 1 GB of
memory available when compiling LindaCoin Core. With 512MB of memory or less
compilation will take much longer due to swap thrashing.

Dependency Build Instructions: Ubuntu & Debian
----------------------------------------------
Build requirements:

    sudo apt-get install build-essential libtool automake autotools-dev autoconf pkg-config libssl-dev libgmp3-dev libevent-dev bsdmainutils

On at least Ubuntu 14.04+ and Debian 7+ there are generic names for the
individual boost development packages, so the following can be used to only
install necessary parts of boost:

    sudo apt-get install libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev

If that doesn't work, you can install all boost development packages with:

    sudo apt-get install libboost-all-dev

BerkeleyDB is required for the wallet. db4.8 packages are available [here](https://launchpad.net/~bitcoin/+archive/bitcoin).
You can add the repository and install using the following commands:

    sudo add-apt-repository ppa:bitcoin/bitcoin
    sudo apt-get update
    sudo apt-get install libdb4.8-dev libdb4.8++-dev
    
If that does not work alternatively download and compile: 
    
    cd ~
    mkdir bitcoin/db4/
    
    wget 'http://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz'
    tar -xzvf db-4.8.30.NC.tar.gz
    cd db-4.8.30.NC/build_unix/
    ../dist/configure --enable-cxx
    make
    sudo make install

 
Ubuntu and Debian have their own libdb-dev and libdb++-dev packages, but these will install
BerkeleyDB 5.1 or later, which break binary wallet compatibility with the distributed executables which
are based on BerkeleyDB 4.8. If you do not care about wallet compatibility,
pass `--with-incompatible-bdb` to configure.

To build Secp256k1:

    cd src/secp256k1/ && ./configure && make
    sudo make install
    sudo ldconfig

Optional:

    sudo apt-get install libminiupnpc-dev (see --with-miniupnpc and --enable-upnp-default)

Dependencies for the GUI: Ubuntu & Debian
-----------------------------------------

If you want to build Linda-Qt, make sure that the required packages for Qt development
are installed. Qt 5 is necessary to build the GUI.
If both Qt 4 and Qt 5 are installed, Qt 5 will be used.

To build with Qt 5 (recommended) you need the following:

    sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler

libqrencode (optional) can be installed with:

    sudo apt-get install libqrencode-dev

Once these are installed, they will be found by configure and a Linda-qt executable will be
built by default.

```

Notes
-----
1) You only need Berkeley DB if the wallet is enabled (see the section *Disable-Wallet mode* below).

2) The release is built with GCC and then "strip transferd" to strip the debug
symbols, which reduces the executable size by about 90%.

If you get an error about secp256k1
cd secp256k1/
./autogen.sh
./configure.sh
make
cd ..
make -f makefile.unix
strip Lindad

To Build Lindad
--------

With UPNP:

    cd src && \
    make -f makefile.unix && \
    strip Lindad

(Recommended) Without UPNP:

    cd src && \
    make -f makefile.unix USE_UPNP= && \
    strip Lindad

To Build Linda-QT
--------

With UPNP:
    qmake -qt=qt5 && \
    make \

(Recommended) Without UPNP:

    qmake -qt=qt5 USE_UPNP=- && \
    make \
```

# STATIC BUILD INSTRUCTIONS
The following instructions have been tested on the following distributions:

1. Ubuntu 16.04.4
2. Linux Mint 18.3

The advantage of building the wallet this way is that it will be able to be executed even on a fresh ubuntu installation without adding additional libraries. There will be no dependencies error messages at startup in case some shared libs are missing. The current release was build that way.

## Get the building environment ready (same as above)

Open a terminal window. If git is not installed in your system, install it by issuing the following command
```
sudo apt-get install git
```
Install Linux development tools 
```
sudo apt-get install build-essential libtool automake autotools-dev autoconf pkg-config libgmp3-dev libevent-dev bsdmainutils
```
Additionnal dependencies needed to compile QT
```
sudo apt-get install libxcb1-dev libgtk3-dev libgtk2-dev
```
## Compile all dependencies manually and use their static libs
### Download and build BerkeleyDB 5.0.32.NC
```
cd ~/
wget 'http://download.oracle.com/berkeley-db/db-5.0.32.NC.tar.gz'
tar -xzvf db-5.0.32.NC.tar.gz
cd db-5.0.32.NC/build_unix/
../dist/configure --enable-cxx --disable-shared 
make

```

### Compiling Boost 1.58

Download Boost 1.58 here : 
https://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz/download<br>
Put the archive in ~/deps

```
cd ~/deps
tar xvfz boost_1_58_0.tar.gz
cd ~/deps/boost_1_58_0
./bootstrap.sh

./b2 --build-type=complete --layout=versioned --with-chrono --with-filesystem --with-program_options --with-system --with-thread toolset=gcc variant=release link=static threading=multi runtime-link=static stage

```

### Compiling miniupnpc

Install Miniupnpc. Download it from here http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.9.tar.gz<br>
and place it in your deps folder, then :
```
cd ~/deps
tar xvfz miniupnpc-1.9.tar.gz

cd miniupnpc-1.9
make upnpc-static
```
==> Important : don't forget to rename "miniupnpc-1.9" directory to "miniupnpc"

### Compiling OpenSSL

download 1.0.2g version here : https://www.openssl.org/source/old/1.0.2/openssl-1.0.2g.tar.gz<br>
place archive in deps folders then :
```
tar xvfz openssl-1.0.2g.tar.gz
cd openssl-1.0.2g 
./config no-shared no-dso
make depend
make
```

### Compiling QT 5.5.0 statically
Download QT 5.5.0 sources
https://download.qt.io/archive/qt/5.5/5.5.0/single/qt-everywhere-opensource-src-5.5.0.tar.gz<br>
Extract in deps folder
```
tar xvfz qt-everywhere-opensource-src-5.5.0.tar.gz
```
after everything is extracted, create another directory where static libs will be installed. 
For example, i created ~/deps/Qt/5.5.0_static and used that directory in configure command below (it may take a while) :
```
cd ~/deps/qt-everywhere-opensource-src-5.5.0

./configure -static -opensource -release -confirm-license -no-compile-examples -nomake tests -prefix ~/deps/Qt/5.5.0_static -qt-zlib -qt-libpng -no-libjpeg -qt-xcb -qt-freetype -qt-pcre -qt-harfbuzz -largefile -no-opengl -no-openssl -gtkstyle -skip wayland -skip qtserialport -skip script -skip qtdeclarative -skip qtwebchannel -alsa -c++11 -nomake tools -no-icu

make -j 4
```
After it successfuly ends :
```
make install
```
### Compiling Linda QT wallet

Clone the Lindacoin repository
```
git clone https://github.com/Lindacoin/Linda.git
```
if required, fix the leveldb files permissions
```
cd ~/Linda/src/leveldb
chmod +x build_detect_platform
chmod 775 *
```
you may also be required to build leveldb prior to start the wallet build
```
make clean
make libleveldb.a libmemenv.a
```
build libsecp256k1
```
cd ~/Linda/src/secp256k1
./autogen.sh
./configure --enable-static --disable-shared
make
```
Just by precaution, go to secp256k1/.libs dir and delete all non static libs (all except *.a files) if present, to make sure only static libs will be used during linking

go back to Lindacoin dir to modify Linda-qt.pro if needed :
```
cd ~/Linda
nano Linda-qt.pro
```
All dependencies dir variables to set according to what have been done above in linux {} section :
```
linux {
	DEPS_PATH = $(HOME)/deps
	SECP256K1_LIB_PATH = src/secp256k1/.libs
	SECP256K1_INCLUDE_PATH = src/secp256k1/include
## comment below dependencies if u don't need to compile a static binary on linux
	MINIUPNPC_LIB_PATH = $$DEPS_PATH/miniupnpc
	MINIUPNPC_INCLUDE_PATH = $$DEPS_PATH
	BOOST_LIB_PATH = $$DEPS_PATH/boost_1_58_0/stage/lib
	BOOST_INCLUDE_PATH = $$DEPS_PATH/boost_1_58_0
	BDB_LIB_PATH = $$DEPS_PATH/db-5.0.32.NC/build_unix
	BDB_INCLUDE_PATH = $$DEPS_PATH/db-5.0.32.NC/build_unix
	OPENSSL_LIB_PATH = $$DEPS_PATH/openssl-1.0.2g
	OPENSSL_INCLUDE_PATH = $$DEPS_PATH/openssl-1.0.2g/include
}
```
After saving the .pro file :
```
export PATH=$HOME/deps/Qt/5.4.2_static/bin:$PATH
qmake
make
```

### Compiling Lindad
With dependencies (except QT cause not needed here) compliled as above, put these lines in ~/Linda/src/makefile.unix (first lines)
```
DEPS_PATH = $(HOME)/deps
SECP256K1_LIB_PATH = src/secp256k1/.libs
SECP256K1_INCLUDE_PATH = src/secp256k1/include
MINIUPNPC_LIB_PATH = $(DEPS_PATH)/miniupnpc
MINIUPNPC_INCLUDE_PATH = $(DEPS_PATH)
BOOST_LIB_PATH = $(DEPS_PATH)/boost_1_58_0/stage/lib
BOOST_INCLUDE_PATH = $(DEPS_PATH)/boost_1_58_0
BDB_LIB_PATH = $(DEPS_PATH)/db-5.0.32.NC/build_unix
BDB_INCLUDE_PATH = $(DEPS_PATH)/db-5.0.32.NC/build_unix
OPENSSL_LIB_PATH = $(DEPS_PATH)/openssl-1.0.2g
OPENSSL_INCLUDE_PATH = $(DEPS_PATH)/openssl-1.0.2g/include
```
After that in ~/Linda/src
```
make -f makefile.unix
strip Lindad
```
