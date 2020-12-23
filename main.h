// Save Game Extractor - a utility that modules Sega Saturn save game files over audio

/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2017, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once

// program version, keep this length to avoid having to resize strings
#define VERSION "0.97"

// program states
#define STATE_UNINITIALIZED      0
#define STATE_MAIN               1
#define STATE_LIST_SAVES         2
#define STATE_PLAY_SAVES         3
#define STATE_DUMP_BIOS          4
#define STATE_TEST               5
#define STATE_COLLECT            6
#define STATE_CREDITS            7

// option selected on the main screen
#define MAIN_OPTION_INTERNAL     0
#define MAIN_OPTION_CARTRIDGE    1
#define MAIN_OPTION_EXTERNAL     2
#define MAIN_OPTION_BIOS         3
#define MAIN_OPTION_TEST         4
#define MAIN_OPTION_COLLECT      5
#define MAIN_OPTION_CREDITS      6

// position of the heading text
#define HEADING_X                2
#define HEADING_Y                2

#define OPTIONS_X                HEADING_X + 3
#define OPTIONS_Y                HEADING_Y + 4

#define CURSOR_X                 HEADING_X

#define MAIN_NUM_OPTIONS         7
#define BIOS_NUM_OPTIONS         4

#define BIOS_FILENAME           "bios.bin"
#define BIOS_START_ADDR         0x00000000
#define BIOS_SIZE               512 * 1024
#define BIO_NUM_SEGMENTS        4 // how many segments to split the bios into
#define BIOS_SEGMENT_SIZE       BIOS_SIZE / BIO_NUM_SEGMENTS

// BUGBUG: adjust this as necessary
#define MAX_SAVE_SIZE           (256 * 1024)
#define MAX_SAVE_FILENAME       12
#define MAX_SAVES               50
#define MAX_SAVES_PER_PAGE      8 // saves per page to list

#define HEADING_UNDERSCORE     "___________________________________"

// the test audio message
#define TEST_MESSAGE "This\n  is\n  COOL\n"

#define MD5_HASH_SIZE                   16

#define ESTIMATED_TRANSFER_SPEED        70

// records whether or not an input has been pressed that frame
typedef struct _INPUTCACHE
{
    bool pressedUp;
    bool pressedDown;
    bool pressedLeft;
    bool pressedRight;
    bool pressedB;
    bool pressedStartAC;
    bool pressedLT;
    bool pressedRT;
} INPUTCACHE, *PINPUTCACHE;

typedef struct _GAME
{
    // game state variables
    int state; // current state of the program i.e. what screen it is in
    int previousState; // useful when going backwards from certain states

    // number of options in the current state
    int numStateOptions;
    int numSaves; // number of saves on the device

    // the position of the cursor in the 0 position
    // we add cursorOffset to get the correct position
    int cursorPosX;
    int cursorPosY;
    int cursorOffset;

    jo_backup_device backupDevice; // JoInternalMemoryBackup, JoCartridgeMemoryBackup, JoExternalDeviceBackup
    bool listedSaves; // set to true if we already queried the saves from the backup device

    char saveFilename[MAX_SAVE_FILENAME]; // selected save file name
    unsigned int saveFileSize; // selected save file size
    unsigned char* saveFileData; // the raw data, points at transmissonFileData + TRANSMISSION_HEADER_SIZE
    unsigned char* transmissionData; // consists of TRANSMISSION_HEADER + variable length saveFileData
                                     // not encoded or escaped in any form

    unsigned int compressedSize; // size after compression

    unsigned char* encodedTransmissionData; // transmission data encoded with Reed Solomon and later escaped
    unsigned int encodedTransmissionSize;  // number of bytes of encodedTransmissionata



    bool isTransmissionRunning;

    bool md5Calculated; // set to true if we have calculated the md5 MD5_HASH_SIZE
    unsigned char md5Hash[MD5_HASH_SIZE];

    // hack to cache controller inputs
    INPUTCACHE input;

} GAME, *PGAME;

// meta data related to save files
typedef  struct  _SAVES {
    char filename[MAX_SAVE_FILENAME];
    unsigned int datasize;
    unsigned short blocksize;
} SAVES, *PSAVES;

extern GAME g_Game;

// common functions
void jo_main(void);
void transitionToState(int newState);
void abcStartHandler(void);
void clearScreen(void);
int copyBIOS(unsigned int segment);
int copySaveFile(void);
void moveCursor(bool savesPage);

// main screen
void main_draw(void);
void main_input(void);

// list saves screen
void listSaves_draw(void);
void listSaves_input(void);

// playing save screen
void playSaves_draw(void);
void playSaves_input(void);

// dump bios screen
void dumpBios_draw(void);
void dumpBios_input(void);

// test screen
void test_draw(void);
void test_input(void);

// save games collect projects screen
void collect_draw(void);
void collect_input(void);

// credits screen
void credits_draw(void);
void credits_input(void);

// debug output
void debugOutput_draw(void);

// function prototypes to suppress compiler warnings
void *memcpy(void *dest, const void *src, unsigned int n);
char *strncpy(char *dest, const char *src, unsigned int n);
