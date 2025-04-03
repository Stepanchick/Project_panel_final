#pragma once

#define HEIGTH 2
#define WIGHT 3

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <Arduino.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <GyverDS3231.h>
GyverDS3231 dsdt;

#include <GyverDS18.h>
#define termo_pin 27
GyverDS18Single dst(termo_pin);

#include <Preferences.h>
Preferences preferences;

#define nOE 32
#define A_pin 1
#define B_pin 17
#define CLK 26
#define SCLK 16
#define R_pin 33

//#define I2C_SDA 21
//#define I2C_SCL 22
//#define termo_pin 27

void Task1code( void * parameter);
char* password;
bool smisl = 1;

void load_data_blok();
void up_bloks();

void SendData();
void setText();
void setTextFrom(int nach);
void setBufferWhithoutAnim();
void playAnim();

//int myGetTemp(char* &text, byte &txt_l, int t);

void sendWithFonAnim(byte blok);

void add_simv(WiFiClient &client);

void mainSait(WiFiClient &client);
void add_b_site(WiFiClient &client);

bool use_second_array = 0;
bool on_off_tochki;
bool anim_playing = 0;
bool chanch_sim = 0;

///////////////////////Фоновые анимации///////////////////

class fon_animations {
  public:
    bool anim_simv;
    int x_vect;
    int y_vect;
    unsigned int time_an;
    
    int x_col[2];
    int y_col[2];
    bool invers;
    
    int x_nach[2];
    int y_nach[2];
    uint32_t plotn[2];
};

fon_animations fon_anim;


class partikle{
public:
  int32_t x; // координата х
  int32_t y; // координата у
  
  int Vx; // скорость по оси х
  int Vy; // скорость по оси у

//  int x_col[2]; // колебания частицы по x
//  int y_col[2]; // колебания частицы по y

  bool anim_simv;
  bool inv;

  uint32_t interv; // Интервал, не ну а что?
  uint32_t timer; // персональный таймер

  partikle* next = nullptr; // будет очередь
};

partikle pt1;
byte part_length = 0; // Переменная, котрую я был вынужден вытащить сюды
uint32_t timer_fanim;

///////////////////////Блоки текcта/////////////////////

class bloks_of_text {
  public:
    char* text;
    byte leng = 0;
    
    float text_time;
    int anim;
    int anim_time;
    int fon_anim = -1;
    
    int day1;
    int month1;
    int day2;
    int month2;
    byte dayWeek;
};


class simvol{
public:
  byte lengt = 0;
  uint32_t* bitMap;
};

class ten_in_one{
public:
  byte colich = 0;
  unsigned int tim = 0;
  simvol bitMaps[10];
};


simvol fon_smv;
ten_in_one fon_anim_smv;

#include "3.1MainSite.h"
#include "3.2AddBykovkaSite.h"
#include "3.3AddFonAnimationSite.h"
#include "3.4Data_Time.h"
#include "3.5AddAnimBykovaSite.h"
