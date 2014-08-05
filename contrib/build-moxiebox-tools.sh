#!/bin/sh

# build-moxiebox-tools.sh
#
# Copyright (c) 2014  Anthony Green
# 
# The above named program is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.
# 
# The above named program is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this work; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301, USA.

# A basic script to build the moxiebox toolchain.  It requires that the 
# GNU src & gcc trees be checked out in the current directory.

MAKEJOBS=4

if ! test -f src/src-release; then
  echo "ERROR: missing GNU src tree (i.e., newlib and libgloss)."
  exit 1
fi

if ! test -f binutils-gdb/src-release; then
  echo "ERROR: missing binutils-gdb tree."
  exit 1
fi

if ! test -f gcc/gcc/version.h; then
  echo "ERROR: missing GNU gcc tree."
  exit 1
fi

for dir in buildmoxiebox/gcc-boot buildmoxiebox/gcc buildmoxiebox/binutils-gdb buildmoxiebox/src root/usr; do
  if ! test -d $dir; then
    mkdir -p $dir;
  fi;
done;

PREFIX=`(cd root/usr; pwd)`

(cd buildmoxiebox/binutils-gdb;
  ../../binutils-gdb/configure --target=moxiebox \
      --disable-gdbtk \
      --prefix=$PREFIX;
  make -j$MAKEJOBS all-binutils all-gas all-ld;
  make install-binutils install-gas install-ld)

(cd buildmoxiebox/gcc-boot; 
  ../../gcc/configure  --target=moxiebox \
                       --prefix=$PREFIX \
                       --enable-languages=c \
                       --disable-libssp \
                       --disable-libquadmath \
                       --without-newlib;
  make -j$MAKEJOBS all;
  make install)

# Put our new tools on the PATH
PATH=$PREFIX/bin:$PATH

(cd buildmoxiebox/src;
  ../../src/configure --target=moxiebox --prefix=$PREFIX;
  make -j$MAKEJOBS all-target-newlib all-target-libgloss \
      CC_FOR_TARGET=moxiebox-gcc;
  make install-target-newlib install-target-libgloss \
      CC_FOR_TARGET=moxiebox-gcc;)

(cd buildmoxiebox/gcc;
  ../../gcc/configure  --target=moxiebox \
                       --prefix=`(cd ../../root/usr; pwd)` \
                       --enable-languages=c,c++ \
                       --disable-libssp \
                       --disable-libquadmath \
                       --with-newlib \
                       --with-headers=$PREFIX/moxiebox/include;
  make -j$MAKEJOBS all;
  make install)

if ! test -f root/usr/moxiebox/lib/crt0.o; then
  (cd root/usr/moxiebox/lib; ln -s ../../../../../runtime/crt0.o .)
fi 

if ! test -f root/usr/moxiebox/lib/libsandboxrt.a; then
  (cd root/usr/moxiebox/lib; ln -s ../../../../../runtime/libsandboxrt.a .)
fi

