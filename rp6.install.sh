#!/bin/bash
#-------------------------------------------------------------
#
# Script for installing RP6 environment on Linux systems
#
#-------------------------------------------------------------

function reportStep {
	local message="$@"
        local line="---------------------------------------------------------------------"
	echo $line
	echo "" 
	echo $message
	echo "" 
}

#Stop script when any of the command fails:
set -e

reportStep "Installing software needed for RP6:"
sudo apt-get update
#sudo apt-get upgrade
sudo apt-get install gcc-avr avr-libc git wget openjdk-7-jdk

export GIT_CODE_URL="https://github.com/b3nzchr3ur/rp6.git"
reportStep "Retrieving source code for RP6:" $GIT_CODE_URL
git clone $GIT_CODE_URL $HOME/rp6

#makes sure you can access the robotflash.sh from anywhere
echo "export PATH=$PATH:$HOME/rp6/RobotLoader_20120924" >> ~/.bashrc

#Enables user to type "robotload" to start it anywhere
if [[ $arch == "x86_64" ]] ; then 
    echo "alias robotloader='temp_dir=\$(pwd) && cd $HOME/rp6/RobotLoader_20120924 && sudo ./robotloader_linux_x64.sh && cd \"\$temp_dir\"'" >> ~/.bashrc
else
    echo "alias robotloader='temp_dir=\$(pwd) && cd $HOME/rp6/RobotLoader_20120924 && sudo ./robotloader_linux.sh ; cd \"\$temp_dir\"'" >> ~/.bashrc
fi

source ~/.bashrc

reportStep "Installation done... enjoy!"
