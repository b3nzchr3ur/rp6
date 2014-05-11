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

#Stop script when any of the command fail:
set -e

reportStep "Installing software needed for RP6:"
sudo apt-get update
sudo apt-get install gcc-avr avr-libc git wget openjdk-6-jdk

export GIT_CODE_URL="https://github.com/b3nzchr3ur/rp6.git"
reportStep "Retrieving source code for RP6:" $GIT_CODE_URL
git clone $GIT_CODE_URL

reportStep "Installation done... enjoy!" 
