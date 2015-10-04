#!/bin/sh

# download-tools-sources.sh
#
# Copyright (c) 2012, 2013, 2014  Anthony Green
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

# A basic script to download the upstream GNU toolchain sources.

echo "Downloading GCC sources..."
if [ ! -d gcc ]
then
	svn checkout svn://gcc.gnu.org/svn/gcc/trunk gcc
else
	( cd gcc && svn up )
fi

echo "Downloading binutils sources..."
if [ ! -d binutils-gdb ]
then
	git clone --depth=1 git://sourceware.org/git/binutils-gdb.git
else
	( cd binutils-gdb && git checkout -f && git pull )
fi

echo "Downloading newlib and libgloss..."
cvs -z3 -d:pserver:anoncvs@sourceware.org:/cvs/src co \
    newlib \
    libgloss

echo "Updating binutils and newlib/libgloss with gcc config.sub"
cp gcc/config.sub binutils-gdb
cp gcc/config.sub src
