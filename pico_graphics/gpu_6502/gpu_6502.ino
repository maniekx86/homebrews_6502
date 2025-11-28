#include <PicoDVI.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"

//overclock to 200 mhz

// Define the video chip interface pins
#define CLK_PIN 20
#define CS_PIN 22
#define RW_PIN 11
#define RESET_PIN 21
#define A0_PIN 10

#define DATA_PIN_START 2
#define DATA_PIN_END 9
/*
 * pico
D0  GP 2
...
D7  GP 9

RW  GP 11
A0  GP 10


CLK   GP 20
RESET GP 21
CS    GP 22 // TO DO FLIP A0 AND CS done
 */

static const uint16_t program_instructions[] = {
            //     .wrap_target
    0x2094, //  0: wait   1 gpio, 20                 
    0x2014, //  1: wait   0 gpio, 20                 
    0x00c0, //  2: jmp    pin, 0                     
    0x400a, //  3: in     pins, 10                   
    0x0000, //  4: jmp    0                           
            //     .wrap
};
/*
 * 
 * start_read:
    wait 0 GPIO 20
    wait 1 GPIO 20
    in pins 8 
    push
jmp start_read
    
 */

static const pio_program_t pio_program = {
    .instructions = program_instructions,
    .length = 5,
    .origin = -1
};
uint8_t read_data_bus() {
    uint8_t data = 0;
    for (int i = DATA_PIN_START; i <= DATA_PIN_END; i++) {
        if (gpio_get(i)) {
            data |= 1 << (i - DATA_PIN_START);
        }
    }
    return data;
}

PIO pio = pio1;
uint sm;

DVItext1 display(DVI_RES_640x480p60, pico_sock_cfg);

void setup() {
  pinMode(CLK_PIN, INPUT);
  pinMode(CS_PIN, INPUT);
  pinMode(RW_PIN, INPUT);
  pinMode(RESET_PIN, INPUT);
  pinMode(A0_PIN, INPUT);
  //Serial.begin(115200);

  //setInput();

  

  const int PIN_MASK = 0xFF;
  uint pin = DATA_PIN_START;
  
  sm = pio_claim_unused_sm(pio, true);
  uint offset = pio_add_program(pio, &pio_program);
  pio_sm_config c = pio_get_default_sm_config();

  for (int i = DATA_PIN_START; i <= DATA_PIN_START + 8; i++) {
    gpio_set_dir(i, GPIO_IN);
    pio_gpio_init(pio, i);
  }
  pio_gpio_init(pio, CLK_PIN);
  gpio_set_dir(CS_PIN, GPIO_IN);
  pio_gpio_init(pio, CS_PIN);
  
  pio_sm_set_consecutive_pindirs(pio, sm, DATA_PIN_START, 10, false);
  pio_sm_set_consecutive_pindirs(pio, sm, CLK_PIN, 1, false);
  
  sm_config_set_in_pins(&c, DATA_PIN_START);

  sm_config_set_jmp_pin (&c, CS_PIN); // CS skip
  
  sm_config_set_in_shift(&c, false, true, 8);
  sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);
  
  pio_sm_init(pio, sm, offset, &c);
  pio_sm_set_enabled(pio, sm, true);



  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }

  //attachInterrupt(CS_PIN, isr_clk, FALLING);
  attachInterrupt(RESET_PIN, isr_reset, FALLING);

  //display.print("DISP OK. ");
  
}


void setInput() {
  for (int i = DATA_PIN_START; i <= DATA_PIN_END; i++) {
    //pinMode(i, INPUT);
    gpio_set_dir(i, GPIO_IN);
  }
}
void setOutput() {
  for (int i = DATA_PIN_START; i <= DATA_PIN_END; i++) {
    gpio_set_dir(i, GPIO_OUT);
    //pinMode(i, OUTPUT);
  }
}


uint8_t dataBuf[64][2];
int mainCounter=0;
int isrCounter=0;

uint8_t vidRegister = 0; // 0 - data, 1 - cursor posX, 2 - cursor posY

void putData(uint8_t data, uint8_t type) {
  dataBuf[isrCounter][0] = data;
  dataBuf[isrCounter][1] = type;
  isrCounter++;
  
  if(isrCounter>63) {
    isrCounter = 0;
  }
}


void loop() {
  if(mainCounter!=isrCounter) {

    if(dataBuf[mainCounter][1]==1) {
      vidRegister = dataBuf[mainCounter][0];
    } else if(dataBuf[mainCounter][1]==255) {
      // reset
      display.fillScreen(0);
      display.setCursor(0, 0);
    } else {
      uint8_t data = dataBuf[mainCounter][0];
    
      switch(vidRegister) {
        case 0: {
          display.write(data);    

          break;
        }
        case 1: {
          display.setCursor(data,display.getCursorY());
          break;
        }
        case 2: {
          display.setCursor(display.getCursorX(),data);
          break;
        }
      }
    }

    mainCounter++;
        
    if(mainCounter>63) {
      mainCounter = 0;
    }
    
  }


  //pio_sm_exec_wait_blocking(pio, sm, pio_encode_in(pio_x, 1));
  while(!pio_sm_is_rx_fifo_empty(pio, sm)) {
    //if (!gpio_get(CS_PIN) && !gpio_get(RW_PIN)) {
        uint32_t data = pio_sm_get(pio, sm);
        if( (!(data >> 9)) & 1) { // if RW is write (LOW) 
          if((!(data >> 8)) & 1) { // data register
            putData(data,0);  
          } else { // control register
            putData(data,1);
          }
          
        }
        
    //}
  }
    

}
int delayy=0;

uint8_t readData() {
  uint8_t data = 0;
  for (uint8_t i = DATA_PIN_START; i <= DATA_PIN_END; i++) {
    data |= (gpio_get(i) << (i - DATA_PIN_START));
  }
  return data;
}





void isr_reset() { // faling
  putData(0,255);
  vidRegister = 0;
}
