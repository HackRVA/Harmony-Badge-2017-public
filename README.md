# Summary
This repository is for the elctronic badges of the RVASec 2017 cyber security conference held in Richmond VA.

# Links
**TODO: Links to things like docs and wiki**


# Setup
## Dependencies
* MPLAB
* XC32 v1.42 with legacy libraries (PLIB)
* A 2017 badge, but a 2016 badge will work for now :P
* \*nix OS recommended

## Building the Project
The steps described below, except for the clean & build at the end, should only have to be done once for a fresh copy of the project. *MHC will generate files and alter the projects configuration specifically for your system, don't commit these changes!* Once the project is building - develop your application/changes and request a merge of only these changes. We're still considering the best path forward for the maintenance of some of these configuration files.

### Start the MPLAB IDE and Open the Project
* Open the _project_ located at **Harmony-Badge-2017/firmware/harmony\_badge\_2017.X**

### Set the main Project
* If you forget to do this, Harmony will yell at you until you do. Right-click the project in the projects panel and select **set as main project*

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



