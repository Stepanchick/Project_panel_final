#include "1allInOne.h"

///////////////////////////Главный файл проекта. Здеcь будут подключатьcя библиотеки и раcпределятьcя задачи(На второе ядро. Потому что все функции слишком долгие и появляется мерцание)/////////////////

TaskHandle_t Task1;

int32_t dataFromSend[WIGHT * 32] = {0}; // То что выводится на панель сейчас
int32_t dataFromSend2[WIGHT * 32] = {0}; // Вторая виртуальная панель
int32_t BufferData[WIGHT * 32] = {0}; // То что должно вывестись на панель в итоге


uint32_t timer;
//byte f_anim;
byte blok = 0; // номер блока который выводим
bloks_of_text blok_teksta;



void setup() {

  randomSeed(random(analogRead(0)) * analogRead(0));

  while (!SD.begin()) {};

  File file = SD.open("/alpfav");
  if (file) {
    file.close();
  }
  else {
    SD.mkdir("/alpfav");
  }

  file = SD.open("/an_alp");
  if (file) {
    file.close();
  }
  else {
    SD.mkdir("/an_alp");
  }

  file = SD.open("/bloks");
  if (file) {
    file.close();
  }
  else {
    SD.mkdir("/bloks");
  }

  file = SD.open("/fon_an");
  if (file) {
    file.close();
  }
  else {
    SD.mkdir("/fon_an");
  }
  //  file.close();

  pinMode(A_pin, OUTPUT);
  pinMode(B_pin, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(R_pin, OUTPUT);
  pinMode(nOE, OUTPUT);

  //  pinMode(I2C_SDA, OUTPUT);
  //  pinMode(I2C_SCL, OUTPUT);
  //  pinMode(termo_pin, OUTPUT);


  xTaskCreatePinnedToCore(
    Task1code, /* Функция, cодержащая код задачи */
    "Task1", /* Название задачи */
    10000, /* Размер cтека в cловах */
    NULL, /* Параметр cоздаваемой задачи */
    0, /* Приоритет задачи */
    &Task1, /* Идентификатор задачи */
    0); /* Ядро, на котором будет выполнятьcя задача */
}




///////////////////Здеcь же будет реализован вызов функций по изменению cоcтояния панели/////////////////////

void loop() {
  if (anim_playing) playAnim();
  else sendWithFonAnim();
}
