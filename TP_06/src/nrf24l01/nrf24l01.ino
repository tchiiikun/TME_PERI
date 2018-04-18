#include <SPI.h>
#include "RF24.h"
#include "printf.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

RF24 radio(9,10);

byte addresses[][6] = {"0XXXX"};

#define MAX_WAIT_FOR_TIMER 1
unsigned int waitFor(int timer, unsigned long period){
  static unsigned long waitForTimer[MAX_WAIT_FOR_TIMER];
  unsigned long newTime = micros() / period;              // numéro de la période modulo 2^32 
  int delta = newTime - waitForTimer[timer];              // delta entre la période courante et celle enregistrée
  if ( delta < 0 ) delta += 1 + (0xFFFFFFFF / period);    // en cas de dépassement du nombre de périodes possibles sur 2^32 
  if ( delta ) waitForTimer[timer] = newTime;             // enregistrement du nouveau numéro de période
  return delta;
}

//--------- definition de la tache Mess

struct Mess_st {
  int timer;                                              // numéro de timer utilisé par WaitFor
  unsigned long period;                                             // periode d'affichage
  char mess[32];
} Mess_t ; 

void setup_Mess(struct Mess_st * ctx, int timer, unsigned long period, const char * mess) {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();
  delay(2000);
  display.clearDisplay();
  
  ctx->timer = timer;
  ctx->period = period;
  strcpy(ctx->mess, mess);
  Serial.begin(9600);                                     // initialisation du débit de la liaison série
}

void loop_Mess(struct Mess_st *ctx) {
  if (!(waitFor(ctx->timer,ctx->period))) return;         // sort s'il y a moins d'une période écoulée

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(ctx->mess);
  display.display();
  display.clearDisplay();
}

//--------- Déclaration des tâches

struct Mess_st msg;

//--------- Setup et Loop

void setup() {
  radio.begin();
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1,addresses[0]);
  radio.printDetails();
  radio.startListening();
  setup_Mess(&msg, 0, 1000000,"");
  Serial.begin(9600);
}

void loop() {
  char buffer[32];

  if( radio.available()){
    radio.read( buffer, sizeof(buffer) );             // Get the payload
    Serial.println(buffer);
    strcpy(ctx->mess, buffer);
    loop_Mess(&msg);
   }
}
