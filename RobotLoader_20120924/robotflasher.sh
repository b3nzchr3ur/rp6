#!/bin/bash
# Terminal Flasher RP6
comment=a'
The MIT License (MIT)
Copyright (c) 2014 Wouter Goyen & Bram van Gemert.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
'
dir=$(dirname $0)
file=$(readlink -f "$2" 2>/dev/null )
arch=$(uname -m)
port=/dev/ttyUSB0

if [[ $arch == "x86_64" ]] ; then 
    libpath="./lib/linux_x64"
else
    libpath="./lib"
fi

function validate_file
{
	if [[ -z "$1" ]] ; then
    echo "No hexfile specified."
    exit
fi

if [[ ! -r "$1" ]] ; then
    echo "File does not exist."
    exit
fi

if [[ "$1" != *.hex ]] ; then
    echo "Given file is not a .hex file"
    exit
fi
}

function flash
{

validate_file $file

pushd $dir
sudo java -Xmx256m -Djava.library.path="$libpath" -jar ./RobotLoader_lib.jar -c -port=$port -hex="$file" 2>/dev/null
popd

}

function flashrun
{
validate_file $file

pushd $dir
sudo java -Xmx256m -Djava.library.path="$libpath" -jar ./RobotLoader_lib.jar -c -port=$port -hex="$file" -s 2>/dev/null
popd

}

function erase
{
pushd $dir
sudo java -Xmx256m -Djava.library.path="$libpath" -jar ./RobotLoader_lib.jar -c -port=$port -e 2>/dev/null
popd
}
function Help
{
echo "Help"
echo "A parameter can be given:"
echo "-f directly runs the flash utility"
echo "-fr directly runs the flash utility and runs the program after"
echo "-e erases the memory"
echo "-h shows help"
echo "-id shows information about the rp6"
echo "with -p you can specify the port if it's not ttyUSB0. Use the USB index only."
echo "Example:  ./robotflasher.sh -f test.hex -p 4"
echo "This will be flash the file 'test.hex' to the RP6 that's connected to ttyUSB4"
}
function Id
{
pushd $dir
sudo java -Xmx256m -Djava.library.path="$libpath" -jar ./RobotLoader_lib.jar -c -port=$port -id 2>/dev/null
popd
}

function checkPort 
{
case "$1" in
	-p)
		port=/dev/ttyUSB"$2"
		;;
esac
}

case "$1" in
    	-f)
		checkPort $3 $4
        	flash
		exit    
		;;
	-fr)
		checkPort $3 $4
		flashrun
		exit
		;;
	-e)
		checkPort $2 $3
		erase
		exit
		;;
	-id)
		checkPort $2 $3
		Id
		exit
		;;
	*)
		Help
		exit 1
		;;
esac
