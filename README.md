# Regshot advanced
## Description
**Regshot advanced** is a tool to monitor and log filesystem and registry changes for example done by an installer. Its main purpose is to detect things modified or added by tools running on windows systems.

Its use is very simple: do the first shot before e.g. an installer runs, do the second shot directly after installation, and last, compare the shots.

BUT **Regshot advanced** got a plethora of configuration possibilities and produces log files in various formats:
* Plain TXT files
* HTML files
* CMD files (only for undoing the changes)
* Easy parsable text files called "UNL"
* REG files undoing or repeating the registry changes
* NSIS installer and uninstaller files, directly usable by the appropriate compiler (see great open source installer **[nsis](https://nsis.sourceforge.io/Main_Page)**) 
* Innosetup installer and uninstaller files, directly usable by the appropriate compiler (see great open source installer **[innosetup](https://jrsoftware.org/isinfo.php)**) 

## Installation and usage of Regshot advanced 
Installation and usage of **Regshot advanced** is fairly easy: just compile (or use the assets) and start the program by double clicking its exe file. **Regshot advanced** doesn't create any own registry data or file outside its directory (except for the chosen log files of course).

For more details about its configuration, have a look at the wiki.

## Latest news
* **February, the 22nd 2021**: Initial release (source and binary will be uploaded during the next days)

## Current versions
The current version v2.0.1.

## Support this project
If you like this tool please consider supporting it via the PayPal button:
<!---[![Donate with PayPal](https://raw.githubusercontent.com/stefan-niedermann/paypal-donate-button/master/paypal-donate-button.png)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=QT54MSJR6QU7Y)--->
<!---
[![Donate with PayPal](https://www.paypalobjects.com/en_US/AT/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=QT54MSJR6QU7Y)
--->
Donations maybe give me the possibility, to maintain **Regshot advanced** in the future and maybe create new output formats, further do improvements and enhancements. But, of course, you don't have to donate to use this tool. 

## Disclaimer
**Important:**
Use this project at your own risc and handle its output with care!!!! The authors and contributors will not be able to provide any help if you damage your windows installation by using this tool....
