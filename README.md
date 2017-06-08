# Summary
This repository is for the elctronic badges of the RVASec 2017 cyber security conference held in Richmond VA.

# Links
**TODO: Links to things like docs and wiki**


# Setup
## Dependencies
* Microchip MPLABX IDE
* XC32 v1.42 with legacy libraries (PLIB)
* A 2017 badge, but a 2016 badge will work for now :P
* These instructions should be generalizable to \*nix OS users. If you are using something else, this may still serve useful as a high-level guide

### Install MPLABX IDE and XC32 compiler
*Don't forget to check if there is a package ready-made for your distro*

- MPLABX IDE can be found at: http://www.microchip.com/mplab/mplab-x-ide
    - Downloads tab, get latest version for your OS

- XC32 Compiler 
    - Up to v1.34 should work fine
    - **Or, to use a newer versions**:  http://www.microchip.com/mplab/compilers
        - Scroll to the bottom and click the 'Downloads' tab
        - You'll need to also install the legacy plib library files: http://www.microchip.com/development-tools/downloads-archive
            - Look for "Language Tool Archives -- MPLAB XC32" and download installer for your OS)

### Install Microchip's Harmony v1.09
- Install to any convenient user directory, the default will be $HOME/microchip/harmony

### Install the MPLAB Harmony Configurator
- Within MPLABX
    -From the menu: Tools/Plugins -> "Available Plugins" tab
    - Find the "MPLAB Harmony Configurator" plugin and click the checkbox
    - Click the Install button, then go through the wizard to install

### Clone this repository and open the project!
- https://github.com/HackRVA/Harmony-Badge-2017.git

## Building the Project
The steps described below, except for the clean & build at the end, should only have to be done once for a fresh copy of the project. *MHC will generate files and alter the projects configuration specifically for your system, don't commit these changes!* Once the project is building - develop your application/changes and request a merge of only these changes. We're still considering the best path forward for the maintenance of some of these configuration files.

### Start the MPLAB IDE and Open the Project
* Open the _project_ located at **Harmony-Badge-2017/firmware/harmony\_badge\_2017.X**

### Set the main Project
* If you forget to do this, Harmony will yell at you until you do. Right-click the project in the projects panel and select **set as main project**

![Set Main Project](http://i.imgur.com/z6A07rk.png)

### Run MHC
* The MHC will read some information from a configuration file and desparately try not to crash in the process

![Running MHC](http://i.imgur.com/t8UXVoA.png)

### Generate Code
* MHC is now ready to generate the FreeRTOS framework and some other boilerplate code, **click the weird gear->[code] button**. Watch magic take place or your computer crash. Only MPLAB decides...

![Generate Code](http://i.imgur.com/YxWV2Ib.png)



### Clean and Build
* The code and make files should be in place now, press the clean and build button. Warnings may occurs, but looks for the 'build successful' output in the last few lines.

![Build](http://i.imgur.com/XPyTwEs.png)


If all has gone well, you'll see a message like the following in the Output/harmony\_badge\_2017 tab in the bottom part of the IDE:

```
BUILD SUCCESSFUL (total time: 3s)
Loading code from […]Harmony-Badge-2017/firmware/harmony_badge_2017.X/dist/pic32mx_usb_badge_int_dyn/production/harmony_badge_2017.X.production.hex...
Loading completed
```

### Flashing using bootloader

You can enter programming mode by holding the main bottom right button whilst powering the device. The RGB led should blink green. The programmer is found in the firmware/src directory

```
/firmware/src/pic32usb ./firmware/harmony_badge_2017.X/dist/pic32mx_usb_badge_int_dyn/production/harmony_badge_2017.X.production.hex
```
