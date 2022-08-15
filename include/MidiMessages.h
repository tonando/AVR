#ifndef MidiMessages_h
#define MidiMessages_h

#define BAUD_MIDI 31250
#define MIDI_BAUD BAUD_MIDI

#define MSG_NOTE_OFF                      0x80
#define MSG_NOTE_ON                       0x90
#define MSG_POLYPHONIC_KEY_PRESSURE       0xA0      //KEY NUMBER          AMOUNT OF PRESSURE
#define MSG_CONTROL_CHANGE                0xB0      //CONTROLLER NUMBER   CONTROLLER VALUE

// ctrl szstem	0xBx
#define RESET	0x79
#define ALL_NOTES_OFF	0x7B
#define OMNI_MODE_OFF	0x7C
#define OMNI_MODE_ON		0x7D
//     0xB07E        Mono mode on (Poly mode off)      
//     0xB07F        Poly mode on (Mono mode off)      
     
     
#define MSG_PROGRAM_CHANGE                0xC      //PROGRAM NUMBER
#define MSG_CHANNEL_PRESSURE              0xD      //PRESSURE VALUE
#define MSG_PITCH_BEND 					0xE

//System Real-Time Message         Status Byte 
//------------------------         -----------
#define MSG_TIMING_CLOCK                         0xF8
#define MSG_START_SEQUENCE                       0xFA
#define MSG_CONTINUE_SEQUENCE                    0xFB
#define MSG_STOP_SEQUENCE                        0xFC
#define MSG_ACTIVE_SENSING                       0xFE
#define MSG_SYSTEM_RESET                         0xFF


//System Common Message   Status Byte      Number of Data Bytes
//---------------------   -----------      --------------------
#define MSG_MIDI_TIMING_CODE            0xF1       //            1
#define MSG_SONG_POSITION_POINTER       0xF2        //           2
#define MSG_SONG_SELECT                 0xF3         //          1
#define MSG_TUNE_REQUEST                0xF6          //        NONE

#endif