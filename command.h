// WF1  WF0  Desc        OC1  OC0  Desc
// 0    0    Off         0    0    -1 Oct
// 0    1    Saw         0    1    +0 Oct
// 1    0    Square      1    0    +1 Oct
// 1    1    Sine        1    1    +2 Oct
//
// MSB                                       LSB
// 7    6    5    4    3    2    1    0      7    6    5    4    3    2    1    0      Desc
// CMD3 CMD2 CMD1 CMD0 VM3  VM2  VM1  VM0    *    *    *    *    *    *    *    *      Command byte, CMD3:0 = command ID, VM3:0 = voice mask. R = reserved

// 0    0    0    0    VM3  VM2  VM1  VM0    PHRS N6   N5   N4   N3   N2   N1   N0     Change pitch (MIDI note), N7:0 = note number, PHRS = phacc reset 
// 0    0    0    1    VM3  VM2  VM1  VM0    V3   V2   V1   V0   WF1  WF0  OC1  OC0    Config osc A, WF1:0 = waveforfm, OC1:0 = octave select  
// 0    1    0    0    VM3  VM2  VM1  VM0    H7   H6   H5   H4   H3   H2   H1   H0     Change pitch (hz), H8:0 = hz
// 0    1    0    1    VM3  VM2  VM1  VM0    H7   H6   H5   H4   H3   H2   H1   H0     Change detune pitch (hz), H8:0 = hz

#define CMD_CONFIG_OSC      16 // 0b00010000
#define CMD_SET_NOTE        0  // 32 //  0b00100000
#define CMD_SET_HZ          64 // 64 //  0b01000000
#define CMD_SET_DETUNE_HZ   80 // 64 //  0b01010000

#define CMD_SET_NOTE__PHRS          0b10000000
#define CMD_SET_NOTE__NOTE          0b01111111
#define CMD_CONFIG_OSC__VOL         0b11110000
#define CMD_CONFIG_OSC__WF          0b00001100
#define CMD_CONFIG_OSC__OC          0b00000011

uint8_t CUTOFF = 255;

inline void command(uint8_t cmd_byte, uint8_t param) {
   uint8_t cmd   = cmd_byte & 0b11110000;
   uint8_t vmask = cmd_byte & 0b00001111;
 
   for (uint8_t ix = 0; ix < VOICES; ix++) {
    if (vmask & (1 << ix)) {
      switch (cmd) {
        case CMD_CONFIG_OSC:
          oscs[ix].amp = (param & CMD_CONFIG_OSC__VOL) >> 4; 
          oscs[ix].wave = (param & CMD_CONFIG_OSC__WF) >> 2;
          oscs[ix].octave = (param & CMD_CONFIG_OSC__OC);
           break;
        case CMD_SET_NOTE:
           oscs[ix].set_note(param & CMD_SET_NOTE__NOTE);
           if (param & CMD_SET_NOTE__PHRS) oscs[ix].phacc = 0;
           break;
        case CMD_SET_HZ:        
           oscs[ix].set_hz(param, 0);
           break;
        case CMD_SET_DETUNE_HZ:        
           oscs[ix].set_detune_hz(param);
           break;
       }
     }
   }
}

inline void process_commands() {
#ifdef USE_WIRE 
  if (cbuff_count >= 2)
    command(buff_read(cbuff), buff_read(cbuff));
#endif
}

