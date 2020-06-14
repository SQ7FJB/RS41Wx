# STM32_RTTY
STM32 &amp; SI4032 rtty test

Released under GPL v2

Use:
https://www.wyzbee.com/download/Utilities/Software/CoIDE-1.7.8.exe

And:
https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/gcc-arm-none-eabi-5_4-2016q3-20160926-win32.exe

Using Linux:
cd into main folder
cmake .
make

Have a nice day ;)

#Changelog
 * 14.12.2016 - Reverse engineeded connections, initial hard work, resulting in working RTTY by SQ7FJB
 * 07.01.2017 - GPS now using proprietiary UBLOX protocol, more elastic code to set working frequency by SQ5RWU
 * 23.01.2017 - Test APRS code, small fixes in GPS code by SQ5RWU
 * 06.06.2017 - APRS code fix, some code cleanup
 * 09.06.2019 - Dirty Removing RTTY, moving APRS settings to the config by SQ9MDD


#TODO
 * More APRS config options

# Configuration
All configs in ```config.h```
