# TP6 : Programmation Arduino

&nbsp;

## 1. Démarrage

La led clignote sur la led `L`, la led clignote toutes les 2 secondes jusqu'à l'infini.

Exemple du **blink** : 
	
	// the setup function runs once when you press reset or power the board
	void setup() {
	  // initialize digital pin 13 as an output.
	  pinMode(13, OUTPUT);
	}
	
	// the loop function runs over and over again forever
	void loop() {
	  digitalWrite(13, HIGH);   
	  delay(100);              // wait for a second
	  digitalWrite(13, LOW);    
	  delay(100);              // wait for a second
	}

Si on change la fréquence comme par example on remplace `delay(1000)` par `delay(100)`, on observe que la LED est ON / OFF pendant 100 ms. 

## 2. Exécution multi-tâches

> Que contient le tableau waitForTimer[] ?

Le tableau waitForTimer[] contient le nombre maximum de timers utilisés.
Chaque tâche aura son propre timer avec une identifiant `int timer`. 
On pourra avec ce timer, définir une période pour chaque tâche.

> Dans quel cas la fonction waitFor() peut rendre 2 ?

Si le nombre de périodes écoulés depuis le dernier appel est de 2 périodes.
c'est possible si cette fonction n'a pas été appelé pendant longtemps.

> Modifier le programme initial pour afficher "Salut" en plus de "bonjour" toutes les 1.5 secondes sans changer le comportement existant.
	
	//--------- Déclaration des tâches
	
	struct Led_st Led1;
	struct Mess_st Mess1;
	struct Mess_st Mess2;
	
	//--------- Setup et Loop
	
	void setup() {
	  setup_Led(&Led1, 0, 100000, 13);                        // Led est exécutée toutes les 100ms 
	  setup_Mess(&Mess1, 1, 1500000, "bonjour");              // Mess est exécutée toutes les 1.5 secondes 
	  setup_Mess(&Mess2, 2, 1500000, "salut");                // Mess est exécutée toutes les 1.5 secondes 
	}
	
	void loop() {
	  loop_Led(&Led1);                                        
	  loop_Mess(&Mess1); 
	  loop_Mess(&Mess2); 
	}

Pour ajouter cette nouvelle tâche, nous avons ajouté un `setup_Mess(&Mess2, 2, 1500000, "salut");` dans la fonction `setup` pour instancier l'identifiant du timer, ainsi que la période de 1.5 secondes.

Cette troisième tâche va être exécuté dans la fonction `loop()` ayant comme paramètre une `struct Mess_st Mess2;` pour écrire ce nouveau message en parallèle des autres tâches.

## 3. Utilisation de l'écran
	
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
	  setup_Compteur(&Cpt1, 0, 1000000);                      // Cpt est exécutée toutes les secondes
	}
	
	void loop() {
	  loop_Compteur(&Cpt1);
	}

On déclare et initialise le compteur en variable globale. 

La fonction `void setup_Compteur()` permet d'afficher le logo d'Adafruit puis d'effacer son contenu pour passer la fonction `void loop_Compteur()`.

La fonction `void loop_Compteur()` permet d'afficher le compteur sur l'OLED au fur et à mesure qu'on l'incrémente.

Pour observer son comportement, on l'affiche sur l'écran OLED toutes les 1 seconde.

## 4. Communication inter-tâches

> Ajouter une tâche qui lit toutes les 0,5 secondes le port analogique 15 (par analogRead()) sur lequel se trouve la photo-résistance et qui sort sa valeur dans une boite à lettre.

> Mofifier la tâche Led pour que la fréquence de clignotement soit inversement proportionnel à la lumière reçue (moins il y a de lumière plus elle clignote vite). La tâche Led devra donc se brancher sur la boite à lettre.

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

On a ajouté les deux paramètres : `int timer` et `unsigned long period` liés au timer dans le `struct` du mailbox pour pourvoir lire toutes les 500 ms une valeur sur le port 15.

Pour récupérer le paramtre `mb->val` du mailbox, on a mis en plus du paramètre `struct mailbox* mb`, `struct Led_st* ctx` pour que la période de la Led soit modifiable en fonction de la valeur de la photo-résistance.

### Conclusion :

Nous avons regroupé les 3 fonctions dans un seul fichier :

- Ecriture des messages "Bonjour" et "Salut" tous les 1.5 secondes
- Affichage sur le OLED du compteur toutes les secondes
- Clignotement des LEDS en fonction de la valeur de la photo-résistance

## 5. Utilisation du module NRF24L01

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
	
	void loop_Mess(struct Mess_st *ctx, const char * mess) {
	  if (!(waitFor(ctx->timer,ctx->period))) return;         // sort s'il y a moins d'une période écoulée
	
	  display.setTextSize(1);
	  display.setTextColor(WHITE);
	  display.setCursor(0,0);
	  display.println(mess);
	  display.display();
	  delay(2000);
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
	    loop_Mess(&msg, buffer);
	   }
	}
 
Pour ce module, nous avons les fonctions `setup_Mess()` et `loop_Mess` pour afficher un message sur le OLED.

Si la radio est disponible, la lecture sur le radio s'effectue et le message est écrit sur le OLED.