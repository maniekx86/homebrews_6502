#include <stdlib.h>
#include <string.h>

char *iochip = (char*)0xB000;
char *kbmem = (char*)0x7F00;
char *dispcard = (char*)0x8000;
char *musiccard = (char*)0x9000;
char *drivecard = (char*)0xA000;


static const char ps2ToAscii[128] = {
    0,   // 00
    0,   // 01
    0, // 02
    0, // 03
    0, // 04
    0, // 05
    0, // 06
    0, // 07
    0, // 08
    0, // 09
    0, // 0A
    0, // 0B
    0, // 0C
    '\t', // 0D
    0, // 0E
    0, // 0F 
    0, // 10
    0, // 11
    127, // 12 (SHIFT)
    0, // 13
    126, // 14 (CTRL)
    //'q', // 15
    '+',
    '1', // 16
    0, // 17
    0, // 18
    0, // 19
    'z', // 1A
    's', // 1B
    'a' ,// 1C 
    //'w',   // 1D
    '*',
    '2', // 1E
    0, // 1F
    0, // 20
    'c', // 21
    'x', // 22
    'd', // 23
    'e', // 24
    '4', // 25
    '3', // 26
    0, // 27
    0,   // 28
    ' ',   // 29
    'v',   // 2A
    'f', // 2B
    't', // 2C
    'r', // 2D
    '5', // 2E
    0, // 2F
    0, // 30
    'n', // 31
    'b', // 32
    'h', // 33
    'g', // 34
    'y',   // 35 
    '6', // 36
    0,   // 37
    0, // 38 
    0,   // 39
    'm',   // 3A
    'j',   // 3B
    'u',   // 3C
    '7',   // 3D
    '8',   // 3E
    0,   // 3F
    0,   // 40
    ',',   // 41
    'k',   // 42
    'i',   // 43
    'o',   // 44
    '0',   // 45
    '9',   // 46
    0,   // 47
    0,   // 48
    '.',   // 49
    '/',   // 4A
    'l',   // 4B
    ';',   // 4C
    'p',   // 4D
    '-',   // 4E
    0,   // 4F
    0,   // 50
    0,   // 51
    'i',   // 52
    0,   // 53
    '[',   // 54
    '=',   // 55
    0,   // 56
    0,   // 57
    0,   // 58
    0,   // 59
    '\n',   // 5A
    ']',   // 5B
    0,   // 5C
    '\\',   // 5D
    0,   // 5E
    0,   // 5F
    0,   // 60
    0,   // 61
    0,   // 62
    0,   // 63
    0,   // 64
    0,   // 65
    '\b',   // 66 
    0,   // 67
    0,   // 68
    0,   // 69
    0,   // 6A
    0,   // 6B
    0,   // 6C
    0,   // 6D
    0,   // 6E
    0,   // 6F
    0,   // 70
    0,   // 71
    0,   // 72
    0,   // 73
    0,   // 74
    0,   // 75
    125,   // 76 (ESC)
    0,   // 77
    0,   // 78
    0,   // 79
    0,   // 7A
    0,   // 7B
    0,   // 7C
    0,   // 7D
    0,   // 7E
    0    // 7F
};

char cursorX=0;
char cursorY=0;

void dispChar(char c) {
    dispcard[0] = c;
    cursorX++;
    if(cursorX>79 || c=='\n') {
        cursorX=0;
        if(cursorY<59) {
            cursorY++;
        }
    }
}

static const char text[] = "6502 SBC (V.PCB) maniek86 (c) 2024\nYM Disk Player v1\n";

char ti;

void printText(const char *text) {
    unsigned char i;
	for(i=0;i<127;i++) {
		if(text[i]=='\0') break;
		dispChar((char)text[i]);
	}
}
void setCursor() {
    dispcard[1] = 1; // x cursor pos register
    dispcard[0] = cursorX;
    dispcard[1] = 2; // y cursor pos register
    dispcard[0] = cursorY;
    dispcard[1] = 0; // switch back
}

unsigned char drivebuf[512];
unsigned char drivebuf1[512];

void busyWait() {
    unsigned char rd; 
    while(1) {
        rd = drivecard[7];
        if((rd & 0b10000000) == 0 && (rd & 0b01000000) != 0) {
            break;
        }
    }
}


void drqWait() {
    unsigned char rd;
    while(1) {
        rd = drivecard[7];
        if((rd>>3) & 1) break;
    }
}



void readDiskInfo() {
    int pos=0;
    unsigned char tmp, rd;
    dispChar('\n');
    drivecard[6] = 0b11100000;
    drivecard[7] = 0xEC;
    
    busyWait();
    drqWait();
    
    for(pos=0;pos<512;pos++) {
        tmp = drivecard[0];
        drivebuf[pos] = drivecard[8];
        pos++;
        drivebuf[pos] = tmp;
    }
    dispChar('\n');
    setCursor();
    for(pos=54;pos<92;pos++) {
        dispChar(drivebuf[pos]);
    }
    dispChar('\n');
    setCursor();
    
    while(1) {
        rd = drivecard[7];
        if(((rd>>3) & 1) == 0) break;
        // ???
        tmp = drivecard[0];
        dispChar('.');
    }
    
    //for(pos=0;pos<100;pos++) {
    //    driveinfo[0] = drivecard[0];
    //}
    dispChar('K');
}

void initStartSector(unsigned int sectorNum) {
    drivecard[3] = sectorNum;
    drivecard[4] = (sectorNum >> 8) & 0x00FF;
    drivecard[5] = 0;
    drivecard[6] = 0b11100000;

    drivecard[2] = 1;
    
    drivecard[7] = 0x20;
    
}

void readDiskBuf(unsigned char buf) {
    unsigned int pos;
    unsigned char rd;
    rd = drivecard[7];
    
    // wait till DRQ = 1
    drqWait();
    
    if(buf==0) {
        for(pos=0;pos<512;pos++) {
            drivebuf[pos] = drivecard[0];
            pos++;
            drivebuf[pos] = drivecard[8];
        }
    } else {
        for(pos=0;pos<512;pos++) {
            drivebuf1[pos] = drivecard[0];
            pos++;
            drivebuf1[pos] = drivecard[8];
        }
    }
    
    iochip[1] = iochip[1] >> 1;
    if(iochip[1]==0) iochip[1] = 0x80;
}


void readSector(unsigned int sectorNum, unsigned char buf) {
    unsigned char rd;//, tmp;
    int pos;
    // LBA address
    drivecard[3] = sectorNum;
    drivecard[4] = (sectorNum >> 8) & 0x00FF;
    drivecard[5] = 0;
    drivecard[6] = 0b11100000;

    drivecard[2] = 1;
    
    drivecard[7] = 0x20;
    
    busyWait();
    
    // check error (screw it)
    rd = drivecard[7];
    
    // wait till DRQ = 1
    drqWait();
    
    if(buf==0) {
        for(pos=0;pos<512;pos++) {
            drivebuf[pos] = drivecard[0];
            pos++;
            drivebuf[pos] = drivecard[8];
        }
    } else {
        for(pos=0;pos<512;pos++) {
            //tmp = drivecard[0];
            //pos++;
            //drivebuf1[pos] = drivecard[8];
            //pos--;
            //drivebuf1[pos] = tmp;
            //pos++;
            drivebuf1[pos] = drivecard[0];
            pos++;
            drivebuf1[pos] = drivecard[8];
        }
    }
    
    //while(1) {
    //    rd = drivecard[7];
    //    if(((rd>>3) & 1) == 0) break;
    //    // ???
    //    tmp = drivecard[0];
    //    //dispChar('.');
    //}
    
}

void setMusicReg(unsigned char reg, unsigned char data) {
    // port C: data
    // port B: control
    
    // 1. select addresss
    musiccard[2] = reg;
    
    musiccard[1] = 0b00001111;
    musiccard[1] = 0b00000000;
    
    // 2. set data to register
    musiccard[2] = data;
    
    musiccard[1] = 0b00001010;
    musiccard[1] = 0b00000000;
}


#pragma optimize (push, off)

int delay() { // 5ms
    char a;
    for(a=0;a<100;a++) {
        asm("nop");
    }
}
#pragma optimize (pop)

void playSong() {
    unsigned int bufPos=0;
    unsigned int curSector=0;
    unsigned char curBuf=0;
    char regs[16];
    unsigned char regPos=0;

    readSector(curSector,0);
    //curSector++;
        
    while(1) {
        //dispChar('A');
        for(regPos=0;regPos<14;regPos++) {
            if(curBuf==0) {
                regs[regPos] = drivebuf[bufPos];
                bufPos++;
                if(bufPos==1) {
                    curSector++;
                    initStartSector(curSector);
                }
                if(bufPos>=512) {
                    curBuf = 1;
                    bufPos = 0;
                    //curSector++;
                    readDiskBuf(curBuf);
                    //readSector(curSector,curBuf);
                }
                //dispChar('B');
            } else {
                regs[regPos] = drivebuf1[bufPos];
                bufPos++;
                if(bufPos==1) {
                    curSector++;
                    initStartSector(curSector);
                }
                if(bufPos>=512) {
                    curBuf = 0;
                    bufPos = 0;
                    //curSector++;
                    readDiskBuf(curBuf);
                    //readSector(curSector,curBuf);                    
                }
                //dispChar('C');
            }
        }
        for(regPos=0;regPos<14;regPos++) {
            setMusicReg(regPos,regs[regPos]);
        }
        //dispChar('D');
        if(bufPos!=0) {
            delay(); // 5 ms
            delay(); // 5 ms
            delay();
        }
            
            //delay();
            //delay();
            //delay();
            
            //delay();
            //delay();
            //delay();
            //delay();
        //}
        
    }
}

// screen is 80x60



int main() {
    char tmpVal;
    char myLoc=0;
    char ignoreNext=0;

    kbmem[0] = 0;
    
    iochip[3] = 0b10001001; // port C input, ports A and B output
    iochip[1] = 0b10000000;
    
    musiccard[3] = 0b10000000; // init sound card's 8255
    // disable sound card IRQ
    musiccard[0] = 0x00;
    
    setMusicReg(7, 0b00111111);
    
    
    printText(text);
    
    setCursor();
    dispChar('>');

    while (1) {

        if(myLoc!=kbmem[0]) {
            myLoc++;
            if(myLoc==32) myLoc = 0;
            tmpVal = kbmem[myLoc];
            
            
            if(tmpVal==0xF0) {
                ignoreNext = 1;
                continue;
            }
            if(ignoreNext==1) {
                ignoreNext = 0;
                continue;
            }
            
            tmpVal = ps2ToAscii[tmpVal];
            
            
            if(tmpVal=='i') {
                readDiskInfo();
            }
            if(tmpVal=='m') {
                setMusicReg(0, 80);
                setMusicReg(1, 0);
                setMusicReg(8, 0b00001111);
                setMusicReg(7, 0b00111110);
            }
            if(tmpVal=='r') {
                readSector(0,0);
            }
            
            if(tmpVal=='p') {
                playSong();
            }
            
        }
    }
        
    return (0);
}
