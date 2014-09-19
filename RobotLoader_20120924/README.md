#RobotLoader

You can run the RobotLoader by executing the following command in a terminal:

- 32-bit system: `./robotloader_linux.sh`
- 64-bit system: `./robotloader_linux_x64.sh`

Note: if the /dev/tty* ports are not visible then make the /dev/tty* accessible to the current user.
Alternative is to run the above commands as superuser:

- 32-bit system: `sudo ./robotloader_linux.sh`
- 64-bit system: `sudo ./robotloader_linux_x64.sh`

# RobotFlasher
You can run flash the RP6 directly from terminal by going to this directory and typing `./robotflasher.sh`

To flash type `./robotflasher.sh -f main.hex` <br>
To flash and run the program type: `./robotflasher.sh -fr main.hex`<br>
To erase the memory type: `./robotflasher.sh -e` <br>
To show the information about the robot type: `./robotflasher -id`

NOTE: To be able to flash a file you'd have to put the main.hex inside the robotloader directory before flashing.

## Setting up path
Alternatively you can add the menu.sh to your path:<br>
For Debian based systems:<br>
open a terminal and type: `sudo gedit ~/.bashrc` <br>
To the end of your file add `export PATH=$PATH:<the directory to your robotloader>` <br>
For example: `export PATH=$PATH:/home/student/Desktop/RobotLoader_20120924`<br>
Save the file and run this command: `source ~/.bashrc`<br>
Now you should be able to type `robotflasher.sh` from anywhere on your system.

The commands are now:<br>
To flash type `robotflasher.sh -f main.hex`<br>
To flash and run the program type: `robotflasher.sh -fr main.hex`<br>
To erase the memory type: `robotflasher.sh -e`<br>
To show the information about the robot type: `robotflasher.sh -id`<br>

NOTE: Now you can flash files from any directory so you dont have to go to the RobotLoader directory anymore
