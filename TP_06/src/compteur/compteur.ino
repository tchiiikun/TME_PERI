#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define MAX_WAIT_FOR_TIMER 1

int cpt = 0;

unsigned int waitFor(int timer, unsigned long period){
  static unsigned long waitForTimer[MAX_WAIT_FOR_TIMER];
  unsigned long newTime = micros() / period;              // numéro de la période modulo 2^32 
  int delta = newTime - waitForTimer[timer];              // delta entre la période courante et celle enregistrée
  if ( delta < 0 ) delta += 1 + (0xFFFFFFFF / period);    // en cas de dépassement du nombre de périodes possibles sur 2^32 
  if ( delta ) waitForTimer[timer] = newTime;             // enregistrement du nouveau numéro de période
  return delta;
}

//--------- definition de la tache Compteur

struct Compteur_st {
   int timer;
   unsigned long period;
};

void setup_Compteur(struct Compteur_st * ctx, int timer, unsigned long period) {
  ctx->timer = timer;
  ctx->period = period;

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();
  delay(2000);

  display.clearDisplay(); 
  Serial.begin(9600);
}

void loop_Compteur(struct Compteur_st * ctx) {
  if (!(waitFor(ctx->timer,ctx->period))) return;         // sort s'il y a moins d'une période écoulée
  cpt++;
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(cpt, DEC);
  display.display();
  display.clearDisplay();
}

//--------- Déclaration des tâches

struct Compteur_st Cpt1;

//--------- Setup et Loop

void setup() {
  setup_Compteur(&Cpt1, 0, 1000000);                      // Cpt est exécutée toutes les 500 ms
}

void loop() {
  loop_Compteur(&Cpt1);
}
