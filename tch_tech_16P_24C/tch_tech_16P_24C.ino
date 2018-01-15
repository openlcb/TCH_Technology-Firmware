//==============================================================
// All software, in any form, is made available subject to either
//the GPL version 2.0 license or successor, or the LGPL version 2.0
//license or successor. If you are uncertain about the terms of these
//licenses or how they apply to what you want to do,you must consult
//TCH Technology or the original author before copying, modifying,
//or distributing the software. All documentation and other writings
//are made available subject to the Creative Commons Attribution-Share
//Alike 3.0 United States License. If you are uncertain about the terms
//of these licenses or how they apply to what you want to do, you must
//consult TCH Technology or the original author before copying,
//modifying, or distributing the works.
// Bob Jacobsen 2010, 2012
// based on examples by Alex Shepherd, David Harris.
// Tim Hatch 2010-1016
// 
//==============================================================
// tch_tech_16P_24C
//   A 16 input, 24 output OpenLCB node
//==============================================================

// next line for stand-alone compile
#include <Arduino.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <logging.h>
#include <EEPROM.h>
#include <can.h>
#include <NodeID.h>
#include <p_c_id.h>
#include <default_p_c_configDefInfo.h>

unsigned int datagramCallback(uint8_t *rbuf, unsigned int length, unsigned int from);

const prog_char SNII_const_data[] PROGMEM = "\001TCH Technology\000OpenLCB/LCC 16-Input 24-Output Producer/Consumer Node\0001.0.4\0000.8.7";
#define SNII_var_data 816
#define SNII_var_offset 20
#include <OlcbArduinoCAN.h>

/* *************************************************
 * EEPROM memory layout
 *     See NodeMemory.h for background info
 *
 * Internal data, not to be reset by user:
 *     0 - 3        Memory state flag
 *     4 - 5        Cycle count
 *     6 - 12       Node ID
 *
 * User configuration data:
 *     13 - 13+8*sizeof(Event)  EventID storage
 *     94 - 113     Node name (zero-term string)
 *     114 - 136     User comment (zero-term string)
 *
 *************************************************** */
#define LAST_EEPROM 12+4000+8*sizeof(Event)
extern "C" {
    uint32_t spaceUpperAddr(uint8_t space) {  // return last valid address
      switch (space) {
          case 255: return sizeof(configDefInfo) - 1; // CDI (data starts at zero)
          case 254: return RAMEND; // RAM from Arduino definition
          case 253: return LAST_EEPROM; // Configuration
      }
      return (uint32_t)3;
    }
};

const uint8_t getRead(uint32_t address, int space) {
  if (space == 0xFF) {
    // Configuration definition information
    return pgm_read_byte(configDefInfo+address);
  } else if (space == 0xFE) {
    // All memory
    return *(((uint8_t*)&rxBuffer)+address);
  } else if (space == 0xFD) {
    // Configuration space
    return EEPROM.read(address);
  } else if (space == 0xFC) { // 
    // used by ADCDI for constant data
    return pgm_read_byte(SNII_const_data+address);
  } else if (space == 0xFB) { // 
    // used by ADCDI for variable data
     return EEPROM.read(SNII_var_data+address);
  } else {
    // unknown space
    return 0; 
  }
}
void getWrite(uint32_t address, int space, uint8_t val) {
  if (space == 0xFE) {
    // All memory
    *(((uint8_t*)&rxBuffer)+address) = val;
  } else if (space == 0xFD) {
    // Configuration space
    EEPROM.write(address, val);
  } 
  // all other spaces not written
}

#include <p_c_events.h>
#include <p_c_outputs.h>

#define ShortBlinkOn   0x00010001L
#define ShortBlinkOff  0xFFFEFFFEL

#include <p_c_blink_patterns.h>
#include <p_c_buttons.h>

//assign blue/gold I/O pins
ButtonLed blue(42, LOW);
ButtonLed gold(43, LOW);

void pceCallback(int index){
  // invoked when an event is consumed; drive pins as needed
  // from index
  //
  // sample code uses inverse of low bit of pattern to drive pin all on or all off
  // (pattern is mostly one way, blinking the other, hence inverse)
  //
  buttons[index]->on(patterns[index]&0x1 ? 0x0L : ~0x0L );
}

NodeMemory nm(0);  // allocate from start of EEPROM
void store() { nm.store(&nodeid, events, eventNum); }

PCE pce(events, eventNum, &txBuffer, &nodeid, pceCallback, store, &link);

// Set up Blue/Gold configuration

BG bg(&pce, buttons, patterns, eventNum, &blue, &gold, &txBuffer);

#include <p_c_input_states.h>

boolean first = false;

void produceFromInputs() {
  // called from loop(), this looks at pins and
  // and decides which events to fire.
  // with pce.produce(i);
  // The first event of each pair is sent on button down,
  // and second on button up.
  int start = 0;
  int end = eventNum/2;

  if (first) {
      start = eventNum/4;
      end = eventNum/4;
  }
  first = ! first;
  for (int i = start; i<end; i++) {
    if (states[i] != buttons[i*2]->state) {
      states[i] = buttons[i*2]->state;
      if (states[i]) {
        pce.produce(i*2);
      } else {
        pce.produce(i*2+1);
      }
    }
  }
}


/**
 * Setup does initial configuration
 */
void setup()
{
  // set up serial comm; may not be space for this!
  //delay(250);Serial.begin(BAUD_RATE);logstr("\nTCH Technology \n");
  //Serial.print("free memory in bytes ");
  Serial.println();
  // read OpenLCB from EEPROM
  
  //nm.forceInitAll(); // uncomment if need to go back to initial EEPROM state
  nm.setup(&nodeid, events, eventNum, (uint8_t*) 0, (uint16_t)0, (uint16_t)LAST_EEPROM); 
  
  // set event types, now that IDs have been loaded from configuration
  for (int i=0; i<eventNum/1.6667; i++) {
      pce.newEvent(i,false,true); // produce, consume
  }
  for (int i=eventNum/1.6666; i<eventNum; i++) {
      pce.newEvent(i,true,false); // produce, consume
  }
   Olcb_setup();
}

void loop() {

  bool activity = Olcb_loop();
    if (activity) {
    // blink blue to show that the frame was received
    blue.blink(0x1);
  }
  if (OpenLcb_can_active) {
        gold.blink(0x1);
        OpenLcb_can_active = false;
    }
    //if (OpenLcb_can_active) {
        //gold.blink(0x1);
        //OpenLcb_can_active = false;
    //}
  {
    // link not up, but continue to show indications on blue and gold
    blue.process();
    gold.process();
  }

}

