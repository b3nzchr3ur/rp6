# RP6
Modifications of code repository for RP6-code from http://www.arexx.com/rp6

![RP6](http://www.arexx.com/rp6/html/images/rp6/rp6.jpg "picture")

## Updates from original source
- Makefile of examples projects are simplied. See for instance this [makefile](RP6Examples_20120725f/RP6CONTROL_EXAMPLES/Example_01_LCD/makefile) that includes [makefile.control](RP6Examples_20120725f/RP6Lib/RP6control/makefile.control).
- Support for unit testing using unity. Located at [unity](RP6Examples_20120725f/unity) directory.
- Split up [RP6ControlLib.h](RP6Examples_20120725f/RP6Lib/RP6control/RP6ControlLib.h) into a [set](RP6Examples_20120725f/RP6Lib/RP6control) of smaller header files. Same for RP6BaseLib.h.
- Include [RP6control_I2CMasterLib](RP6Examples_20120725f/RP6Lib/RP6control_I2CMasterLib) into RP6Lib directory. 
- Added [unit testing examples](RP6Examples_20120725f/assignments.control_board/c_coding_examples_with_unit_test_on_rp6).

## Tooling
- Added [install script](rp6.install.sh) for Linux systems.
- Added [command line script](RobotLoader_20120924/README.md) to RobotLoader for programming RP6. (Kudos to [@Xesxen](http://github.com/Xesxen) and [@Bramvangemert](http://github.com/Bramvangemert)).
