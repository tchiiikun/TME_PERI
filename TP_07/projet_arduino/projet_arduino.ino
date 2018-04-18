#include <SPI.h>
#include "RF24.h"
#include "printf.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

RF24 radio(9,10);

byte addresses[][6] = {"0XXXX"};

#define MAX_WAIT_FOR_TIMER 2
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
  char mess[20];
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

//--------- definition de la tache Boite au letttre

struct mailbox {  enum {EMPTY, FULL} state;
  int timer;
  unsigned long period;
  unsigned long val;
} mb0 = {.state = mailbox::EMPTY};

void setup_T(struct mailbox *ctx, int timer, unsigned long period) {
  ctx->timer = timer;
  ctx->period = period;
  Serial.begin(9600); 
}

// Ecriture
void loop_T1(struct mailbox *ctx, int pin) {  
  if (ctx->state != mailbox::EMPTY) return;         // attend que la mailbox soit vide
  ctx->val = analogRead(pin);
  ctx->state = mailbox::FULL;
}

// Lecture 
void loop_T2(struct mailbox* ctx, struct Mess_st* msg) {
  if (!waitFor(ctx->timer, ctx->period)) return;    // sort s'il y a moins d'une période écoulée
  sprintf(msg->mess, "%lu", ctx->val);
  if (ctx->state != mailbox::FULL) return;          // attend que la mailbox soit pleine
  ctx->state = mailbox::EMPTY;
}

//--------- definition de la tache NRF écriture

void setupWrite() {
  radio.begin();
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.printDetails();
  radio.stopListening();
  Serial.begin(9600);
}

void loopWrite(struct Mess_st* ctx) {
  radio.write( ctx->mess, sizeof(ctx->mess) );             // Get the payload
  Serial.println(ctx->mess);
}

//--------- Déclaration des tâches

struct mailbox Mb1;
struct Mess_st Mess1;

//--------- Setup et Loop

void setup() {
  setup_T(&Mb1, 0, 500000);                       // Mb est exécutée toutes les 100ms 
  setup_Mess(&Mess1, 1, 500000, "");              // Led est exécutée toutes les 100ms 
  setupWrite();
}

void loop() {
  loop_T1(&Mb1, 15);
  loop_T2(&Mb1, &Mess1);
  loop_Mess(&Mess1);
  loopWrite(&Mess1);
}
