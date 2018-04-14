#define MAX_WAIT_FOR_TIMER 2

unsigned int waitFor(int timer, unsigned long period){
  static unsigned long waitForTimer[MAX_WAIT_FOR_TIMER];
  unsigned long newTime = micros() / period;              // numéro de la période modulo 2^32 
  int delta = newTime - waitForTimer[timer];              // delta entre la période courante et celle enregistrée
  if ( delta < 0 ) delta += 1 + (0xFFFFFFFF / period);    // en cas de dépassement du nombre de périodes possibles sur 2^32 
  if ( delta ) waitForTimer[timer] = newTime;             // enregistrement du nouveau numéro de période
  return delta;
}

//--------- définition de la tache Led

struct Led_st {
  int timer;                                              // numéro de timer utilisé par WaitFor
  unsigned long period;                                   // periode de clignotement
  int pin;                                                // numéro de la broche sur laquelle est la LED
  int etat;                                               // etat interne de la led
}; 

void setup_Led(struct Led_st* ctx, int timer, unsigned long period, byte pin) {
  ctx->timer = timer;
  ctx->period = period;
  ctx->pin = pin;
  ctx->etat = 0;
  pinMode(pin,OUTPUT);
  digitalWrite(pin, ctx->etat);
}

void loop_Led(struct Led_st* ctx) {
  if (!waitFor(ctx->timer, ctx->period)) return;          // sort s'il y a moins d'une période écoulée
  digitalWrite(ctx->pin,ctx->etat);                       // ecriture
  ctx->etat = 1 - ctx->etat;                              // changement d'état
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
void loop_T2(struct mailbox* ctx, struct Led_st* led) {
  if (!waitFor(ctx->timer, ctx->period)) return;    // sort s'il y a moins d'une période écoulée
  led->period = (ctx->val)*1000;
  if (ctx->state != mailbox::FULL) return;          // attend que la mailbox soit pleine
  ctx->state = mailbox::EMPTY;
}

//--------- Déclaration des tâches

struct mailbox Mb1;
struct Led_st Led1;

//--------- Setup et Loop

void setup() {
  setup_T(&Mb1, 0, 500000);                          // Mb est exécutée toutes les 500ms 
  setup_Led(&Led1, 1, 500000, 13);                  // Led est exécutée toutes les 500ms 
}

void loop() {
  loop_T1(&Mb1, 15);
  loop_T2(&Mb1, &Led1);
  loop_Led(&Led1);  
}

