Intoduction
---------------

Table of Contents
---------------

[TOC]

Prerequisites
---------------

Tools used for the crosscompilation can be found under the following URLs:

* [brew](https://brew.sh/)
* [Cross Compilation for windows](https://words.filippo.io/easy-windows-and-linux-cross-compilers-for-macos/)
* [Cross Compilation for Raspberry]
* [Cross Compilation for Linux]
* [Cross Compilation for Board]

# Install Homebrew

/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

Run these two commands in your terminal to add Homebrew to your PATH:

(echo; echo 'eval "$(/opt/homebrew/bin/brew shellenv)"') >> /Users/sgabre/.zprofile
eval "$(/opt/homebrew/bin/brew shellenv)"


# Step 1: Compiler and binutils

In order to compile for the Raspberry Pi, we need binutils to assemble and link binaries and a compiler that can produce code for the ARM CPU. In this step, we install clang as compiler and the GNU binutils compiled for the hard float GNU EABI, which is what most Linux distributions run. In addition, we also install rsync from homebrew, which we need later:

brew install arm-linux-gnueabihf-binutils llvm rsync

Step 2: Sysroot

In order to compile anything meaningful, we also need the libraries that come with our Raspberry Pi Linux distribution. There are multiple ways to obtain them, like using remote file systems. This is however quite slow and requires to have the Rasbperry Pi online and running all the time while compiling. I found it more convenient to simply use rsync to sync the relevant directories directly to my Mac.

Run the following command in your favourite directory (e.g. your home directory):

rsync -rzLR --safe-links \
pi@raspberrypi:/usr/lib/arm-linux-gnueabihf \
pi@raspberrypi:/usr/lib/gcc/arm-linux-gnueabihf \
pi@raspberrypi:/usr/include \
pi@raspberrypi:/lib/arm-linux-gnueabihf \
sysroot/ 

Bringing it all together
We now have all the pieces together, let’s compile our first C++ program. Save the C++ code below to a file called hello.cpp:
int main() { return 42; }
In order to compile it, we launch the clang compiler that we installed with homebrew in Step 1, letting it know where to find the binutils from Step 1 and the sysroot from Step 2:
`brew --prefix llvm`/bin/clang++ \
--target=arm-linux-gnueabihf \
--sysroot=$HOME/sysroot \
-isysroot=$HOME/sysroot \
-isystem=$HOME/sysroot/usr/include/c++/6.3.0 \
-isystem=$HOME/sysroot/usr/include/carm-linux-gnueabihf/c++/6.3.0 \
-L$HOME/sysroot/usr/lib/gcc/arm-linux-gnueabihf/6.3.0 \
-B$HOME/sysroot/usr/lib/gcc/arm-linux-gnueabihf/6.3.0 \
--gcc-toolchain=`brew --prefix arm-linux-gnueabihf-binutils` \
-o hello \
hello.cpp
Note: Adapt the paths above if your Linux doesn’t use gcc 6.3.0 as default compiler, or if you chose a different path than your home directory in Step 2.
This command creates the “hello” executable which will run on any Raspberry Pi. Happy compiling!