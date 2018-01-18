
#Arduino Firmware/Sketches for TCH Technology OpenLCB Nodes

Includes latest Arduino .ino sketches for:

  - TCH Technology 32 Output OpenLCB Consumer Node  all Rev

  - TCH Technology 32 Input OpenLCB Producer Node  all Rev

  - TCH Technology 16 in 24 out P/C Node all Rev

*Do not use Arduino 1.5.0 or higher.*

These Sketches were tested on 1.0.5 


--------
##To use this repostitory: 
1. Download a copy of Arduino IDE 1.0.6 from [this page](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous) into a new file, say Arduino106;   
2. Download the [zip file](https://github.com/openlcb/TCH-Technologies/archive/master.zip) from this repositiory into a new directory, say TCHFirmware; 
3. Open Arduino106, and click on 'Arduino' tab and choose Preferences.  In the 'Sketchbook Location' enter the location of the TCHFirmware directory, or browse for it.  When you have found the directory, push OK at the bottom of the window.  
4. Click on 'File' and then 'Sketchbook' and choose one of:
  - tch_tech_16P_24C
  - tch_consumer
  - tch_producer
5. Click on 'Tools' and 'Board' and choose the corresponding board: 
  - TCH Technology Consumer128
  - TCH Technology Consumer64
  - TCH Technology Producer128
  - TCH Technology Producer64
  - TCH Technology Producer Consumer
6. Click on 'Tools' and 'Port' and choose the port attached to teh USB-serial adapter.  
7. Test compile the sketch by pushing the check-circle at the top of the sketch window.  
8. Program the board by pushing the right-arrow-circle.  


