////////////////////////Здесь будут загружаться новые блоки. Задача долгая, поэтому переложена на второе ядро и вынесена в отдельный файл.///////////////////////////////////
#include "1allInOne.h"

void up_bloks() {
  if (!smisl) return;


  if (millis() - timer >= blok_teksta.text_time && endAnim >= WIGHT * 32) {  // ищем разницу
    dsdt.updateNow();
    byte nachalo = blok;
    byte fan = blok_teksta.fon_anim;
    load_data_blok();
    Datime dt(dsdt);


    while (blok != nachalo) {
      if (blok_teksta.month1 != 0) {
        if (blok_teksta.month2 != 0) {
          if (blok_teksta.month1 < blok_teksta.month2) {
            if (blok_teksta.month1 == dt.month && blok_teksta.day1 <= dt.day) goto tyda;
            if (blok_teksta.month2 == dt.month && blok_teksta.day2 >= dt.day) goto tyda;
            if (blok_teksta.month1 < dt.month && blok_teksta.month2 > dt.month) goto tyda;
          }
          else if (blok_teksta.month1 > blok_teksta.month2) {
            if (blok_teksta.month1 == dt.month && blok_teksta.day1 <= dt.day) goto tyda;
            if (blok_teksta.month2 == dt.month && blok_teksta.day2 >= dt.day) goto tyda;
            if (!(blok_teksta.month2 <= dt.month && blok_teksta.month1 >= dt.month)) goto tyda;
          }
          else {
            if (blok_teksta.month1 == dt.month && blok_teksta.day1 <= dt.day && blok_teksta.day2 >= dt.day) goto tyda;
          }
        }
        else {
          if (blok_teksta.month1 == dt.month && blok_teksta.day1 == dt.day) goto tyda;
        }
      }
      else {
        if (blok_teksta.dayWeek == dt.weekDay || blok_teksta.dayWeek == 0) goto tyda;
      }
      load_data_blok();
    }

    if (!smisl) return;

tyda:

    if (blok_teksta.fon_anim != -1 && blok_teksta.fon_anim != fan) setFonSimvols();

    if (blok_teksta.anim != -1) anim_playing = 1;
    timer = millis();

    if (blok_teksta.anim == 4 || blok_teksta.anim == 5){
      if(!set_anim_interv()){
        anim_playing = 0;
        up_bloks();
      }
    }
  }

  if (dsdt.tick()) {
    on_off_tochki = !on_off_tochki;
  }

  if (anim_playing) timer = millis();

  if (blok_teksta.anim != 4 && blok_teksta.anim != 5) setText();
  else if (millis() > svim_anim + svim_anim_timer) setTextFrom();

}


void setFonSimvols() {

  char* nam = new char[]("/fon_an/anim__.txt"); // ищем пустой файлъ
  nam[12] = blok_teksta.fon_anim / 10 + '0';
  nam[13] = blok_teksta.fon_anim % 10 + '0';

  char a = 0;
  char b = 0;

  File Fanim = SD.open(nam, FILE_READ);
  delete[] nam;
  if (!Fanim) {
    return;
  }

  fon_anim.anim_simv = 0;
  fon_anim.x_vect = 0;
  fon_anim.y_vect = 0;
  fon_anim.time_an = 0;
  fon_anim.x_col[0] = 0; fon_anim.x_col[1] = 0;
  fon_anim.y_col[0] = 0; fon_anim.y_col[1] = 0;
  fon_anim.invers = 0;
  fon_anim.x_nach[0] = 0; fon_anim.x_nach[1] = 0;
  fon_anim.y_nach[0] = 0; fon_anim.y_nach[1] = 0;
  fon_anim.plotn[0] = 0; fon_anim.plotn[1] = 0;




  while (Fanim.read() != '\n');

  bool minus = 0;
  char c = Fanim.read();
  b = c;
  c = Fanim.read();
  if (c != '\n') {
    a = b;
    a = c;
    c = Fanim.read();
  } // Прочитали символ, который будем показывать
  c = Fanim.read();

  if (c == '-') {
    minus = 1;
    c = Fanim.read();
  }
  while (c >= '0' && c <= '9') { // Записали икись
    fon_anim.x_vect *= 10;
    fon_anim.x_vect += c - '0';
    c = Fanim.read();
  }
  if (minus) fon_anim.x_vect *= -1;
  minus = 0;

  while (c != '\n') {
    c = Fanim.read();
  }; // Скипнули строку
  c = Fanim.read();

  if (c == '-') {
    minus = 1;
    c = Fanim.read();
  }
  while (c >= '0' && c <= '9') { // Записали игорька
    fon_anim.y_vect *= 10;
    fon_anim.y_vect += c - '0';
    c = Fanim.read();
  }
  if (minus) fon_anim.y_vect *= -1;
  minus = 0;

  while (c != '\n') {
    c = Fanim.read();
  }; // Скипнули строку
  c = Fanim.read();

  while (c >= '0' && c <= '9') { // Записали времечко
    fon_anim.time_an *= 10;
    fon_anim.time_an += c - '0';
    c = Fanim.read();
  }
  fon_anim.time_an *= 1000;
  while (c != '\n') {
    c = Fanim.read();
  }; // Скипнули строку
  c = Fanim.read();

  if (c == '-') {
    minus = 1;
    c = Fanim.read();
  }
  while (c >= '0' && c <= '9') { // Записали икися Первого
    fon_anim.x_col[0] *= 10;
    fon_anim.x_col[0] += c - '0';
    c = Fanim.read();
  }
  if (minus) fon_anim.x_col[0] *= -1;
  minus = 0;

  while (!(c >= '0' && c <= '9')) {
    c = Fanim.read();
    if (c == '-') {
      minus = 1;
      c = Fanim.read();
    }
  }; // Скипнули пробел(ы)
  while (c >= '0' && c <= '9') { // Записали икися Второго
    fon_anim.x_col[1] *= 10;
    fon_anim.x_col[1] += c - '0';
    c = Fanim.read();
  }
  if (minus) fon_anim.x_col[1] *= -1;
  minus = 0;

  while (c != '\n') {
    c = Fanim.read();
  }; // Скипнули строку
  c = Fanim.read();

  if (c == '-') {
    minus = 1;
    c = Fanim.read();
  }
  while (c >= '0' && c <= '9') { // Записали игорька Первого
    fon_anim.y_col[0] *= 10;
    fon_anim.y_col[0] += c - '0';
    c = Fanim.read();
  }
  if (minus) fon_anim.y_col[0] *= -1;
  minus = 0;

  while (!(c >= '0' && c <= '9')) {
    c = Fanim.read();
    if (c == '-') {
      minus = 1;
      c = Fanim.read();
    }
  }; // Скипнули пробел(ы)
  while (c >= '0' && c <= '9') { // Записали игорька Второго
    fon_anim.y_col[1] *= 10;
    fon_anim.y_col[1] += c - '0';
    c = Fanim.read();
  }
  if (minus) fon_anim.y_col[1] *= -1;
  minus = 0;

  while (c != '\n') {
    c = Fanim.read();
  }; // Скипнули строку
  c = Fanim.read();

  if (c == '+') { // Есть ли жизнь на марсе?
    fon_anim.invers = 1;
    c = Fanim.read();
  }
  else {
    fon_anim.invers = 0;
  }
  c = Fanim.read();


  if (c == '-') {
    minus = 1;
    c = Fanim.read();
  }
  while (c >= '0' && c <= '9') { // Записали икися Первого Прародителя
    fon_anim.x_nach[0] *= 10;
    fon_anim.x_nach[0] += c - '0';
    c = Fanim.read();
  }
  if (minus) fon_anim.x_nach[0] *= -1;
  minus = 0;

  while (!(c >= '0' && c <= '9')) {
    c = Fanim.read();
    if (c == '-') {
      minus = 1;
      c = Fanim.read();
    }
  }; // Скипнули пробел(ы)
  while (c >= '0' && c <= '9') { // Записали икися Второго Прародителя
    fon_anim.x_nach[1] *= 10;
    fon_anim.x_nach[1] += c - '0';
    c = Fanim.read();
  }
  if (minus) fon_anim.x_nach[1] *= -1;
  minus = 0;

  while (c != '\n') {
    c = Fanim.read();
  }; // Скипнули строку
  c = Fanim.read();

  if (c == '-') {
    minus = 1;
    c = Fanim.read();
  }
  while (c >= '0' && c <= '9') { // Записали игорька Первого Прародителя
    fon_anim.y_nach[0] *= 10;
    fon_anim.y_nach[0] += c - '0';
    c = Fanim.read();
  }
  if (minus) fon_anim.y_nach[0] *= -1;
  minus = 0;

  while (!(c >= '0' && c <= '9')) {
    c = Fanim.read();
    if (c == '-') {
      minus = 1;
      c = Fanim.read();
    }
  }; // Скипнули пробел(ы)
  while (c >= '0' && c <= '9') { // Записали игорька Второго Прародителя
    fon_anim.y_nach[1] *= 10;
    fon_anim.y_nach[1] += c - '0';
    c = Fanim.read();
  }
  if (minus) fon_anim.y_nach[1] *= -1;
  minus = 0;

  while (c != '\n') {
    c = Fanim.read();
  }; // Скипнули строку
  c = Fanim.read();


  while (c >= '0' && c <= '9') { // Записали времечко
    fon_anim.plotn[0] *= 10;
    fon_anim.plotn[0] += c - '0';
    c = Fanim.read();
  }
  while (c <= '0' || c >= '9') {
    c = Fanim.read();
  }; // Скипнули пробел(ы)
  while (c >= '0' && c <= '9') { // Записали времечко еще раз
    fon_anim.plotn[1] *= 10;
    fon_anim.plotn[1] += c - '0';
    if (Fanim.available()) c = Fanim.read();
    else break;
  }
  Fanim.close();


  nam = new char[] {"/alpfav/bkAA__.txt"};
  nam[10] = ((a & 0b11110000) >> 4) + 'A';
  nam[11] = (a & 0b00001111) + 'A';
  nam[12] = ((b & 0b11110000) >> 4) + 'A';
  nam[13] = (b & 0b00001111) + 'A';


  File file = SD.open(nam, FILE_READ);
  delete[] nam;

  fon_anim.anim_simv = 0;

  if (!file) {
    if (fon_smv.lengt) delete[] fon_smv.bitMap;
    fon_smv.lengt = 0;

    nam = new char[]("/an_alp/bk____/contr.txt");
    nam[10] = ((a & 0b11110000) >> 4) + 'A';
    nam[11] = (a & 0b00001111) + 'A';
    nam[12] = ((b & 0b11110000) >> 4) + 'A';
    nam[13] = (b & 0b00001111) + 'A';

    file = SD.open(nam, FILE_READ);
    delete[] nam;
    if (!file) {
      return;
    }

    fon_anim_smv.colich = file.read();

    fon_anim_smv.tim = 0;
    char c = file.read();

    while (c < '0' || c > '9') {
      c = file.read();
    };

    while (c >= '0' && c <= '9') {
      fon_anim_smv.tim *= 10;
      fon_anim_smv.tim += c - '0';
      c = file.read();
    }

    if (!fon_anim_smv.tim) fon_anim_smv.tim = 1;

    file.close();

    for (int i = 0; i < fon_anim_smv.colich; i++) {
      nam = new char[]("/an_alp/bk____/bitMp0.txt");
      nam[10] = (a >> 4) + 'A';
      nam[11] = (a & 0b00001111) + 'A';
      nam[12] = (b >> 4) + 'A';
      nam[13] = (b & 0b00001111) + 'A';
      nam[20] = i + '0';
      File art = SD.open(nam, FILE_READ);
      delete[] nam;

      byte lengt = art.read();
      uint32_t*  bitMap = new uint32_t[lengt];

      for (int i = 0; i < lengt; i++) {
        bitMap[i] = 0;
        for (int j = 0; j < 4; j++) {
          bitMap[i] += (uint32_t)art.read() << 8 * j;
        }
      }

      uint32_t* delMap = fon_anim_smv.bitMaps[i].bitMap;
      fon_anim_smv.bitMaps[i].bitMap = bitMap;
      if (fon_anim_smv.bitMaps[i].lengt) delete[] delMap;
      fon_anim_smv.bitMaps[i].lengt = lengt;

      art.close();
    }
    for (int i = fon_anim_smv.colich; i < 10; i++) {
      if (fon_anim_smv.bitMaps[i].lengt) delete[] fon_anim_smv.bitMaps[i].bitMap;
      fon_anim_smv.bitMaps[i].lengt = 0;
    }
    fon_anim.anim_simv = 1;
    return;
  }


  chanch_sim = 1;
  byte lengt = file.read();
  uint32_t*  bitMap = new uint32_t[lengt];

  for (int i = 0; i < lengt; i++) {
    bitMap[i] = 0;
    for (int j = 0; j < 4; j++) {
      bitMap[i] += (uint32_t)file.read() << 8 * j;
    }
  }

  uint32_t* delMap = fon_smv.bitMap;
  fon_smv.bitMap = bitMap;
  if (fon_smv.lengt) delete[] delMap;
  fon_smv.lengt = lengt;

  chanch_sim = 0;
  file.close();
  return;
}

/////////////////////////////////////////Загрузка блоков текста/////////////////////////////


void load_data_blok() {

  char* nam = new char[] {"/bloks/blok__.txt"};
  nam[11] = blok / 10 + '0';
  nam[12] = blok % 10 + '0';
  File file = SD.open(nam, FILE_READ);
  delete[] nam;

  if (!file) {
    if (blok != 0) {
      blok = 0;
      load_data_blok();
    }
    else {
      smisl = 0;
    }
    return;
  }
  blok++;



  if (blok_teksta.leng) delete[] blok_teksta.text;

  blok_teksta.day1 = 0;
  blok_teksta.month1 = 0;
  blok_teksta.day2 = 0;
  blok_teksta.month2 = 0;
  blok_teksta.dayWeek = 0;

  String str = "";
  char c = file.read();
  while (c != '\n') { // Получили текст
    str += c;
    c = file.read();
  }
  c = file.read();

  blok_teksta.text = new char[str.length() + 2];
  str.toCharArray(blok_teksta.text, str.length() + 1);
  blok_teksta.leng = str.length(); // И вычислили его длинну

  blok_teksta.text_time = 0;
  while (c != '\n' && c != ' ') { // Получили время, отведенное этому тексту
    blok_teksta.text_time *= 10;
    blok_teksta.text_time += c - '0';
    c = file.read();
  }


  if (c == '\n') goto wihtout_data;
  c = file.read();

  while (!(c >= '0' && c <= '9')) {
    if (c == '\n') goto wihtout_data;
    switch (c) {
      case 'M': {
          c = file.read();
          if (c == 'O') blok_teksta.dayWeek = 1;
          while (c != '\n') c = file.read();
        }
        goto wihtout_data;
      case 'T': {
          c = file.read();
          if (c == 'U') blok_teksta.dayWeek = 2;
          if (c == 'H') blok_teksta.dayWeek = 4;
          while (c != '\n') c = file.read();
        }
        goto wihtout_data;
      case 'W': {
          c = file.read();
          if (c == 'E') blok_teksta.dayWeek = 3;
          while (c != '\n') c = file.read();
        }
        goto wihtout_data;
      case 'F': {
          c = file.read();
          if (c == 'R') blok_teksta.dayWeek = 5;
          while (c != '\n') c = file.read();
        }
        goto wihtout_data;
      case 'S': {
          c = file.read();
          if (c == 'A') blok_teksta.dayWeek = 6;
          if (c == 'U') blok_teksta.dayWeek = 7;
          while (c != '\n') c = file.read();
        }
        goto wihtout_data;
      default: {
          c = file.read();
        }
    }
  }



  while (c >= '0' && c <= '9') { // Получаем первый день
    blok_teksta.day1 *= 10;
    blok_teksta.day1 += c - '0';
    c = file.read();
  }

  if (c == '\n') goto wihtout_data;
  c = file.read();

  while (c >= '0' && c <= '9') { // Получаем первый месяц
    blok_teksta.month1 *= 10;
    blok_teksta.month1 += c - '0';
    c = file.read();
  }

  if (c == '\n') goto wihtout_data;
  c = file.read();

  while (c >= '0' && c <= '9') { // Получаем второй день
    blok_teksta.day2 *= 10;
    blok_teksta.day2 += c - '0';
    c = file.read();
  }

  if (c == '\n') goto wihtout_data;
  c = file.read();

  while (c >= '0' && c <= '9') { // Получаем второй месяц
    blok_teksta.month2 *= 10;
    blok_teksta.month2 += c - '0';
    c = file.read();
  }

  while (c != '\n') c = file.read();

wihtout_data: // Досрочное выполнение ввиду отсутсвия даты


  c = file.read();

  blok_teksta.anim = 0;
  if (c == '-') {
    blok_teksta.anim = -1;
    while (c != '\n') c = file.read();
  }

  while (c != '\n' && c != ' ') { // Получили анимацию
    blok_teksta.anim *= 10;
    blok_teksta.anim += c - '0';
    c = file.read();
  }
  c = file.read();

  blok_teksta.anim_time = 0;
  while (c != '\n' && c != ' ') { // Получили время на анимацию
    blok_teksta.anim_time *= 10;
    blok_teksta.anim_time += c - '0';
    c = file.read();
  }
  c = file.read();

  blok_teksta.fon_anim = 0;

  if (c == '-') {
    blok_teksta.fon_anim = -1;
    while (c != '\n') c = file.read();
  }

  while (c != '\n' && c != ' ') { // Получили анимацию
    blok_teksta.fon_anim *= 10;
    blok_teksta.fon_anim += c - '0';
    c = file.read();
  }

  file.close();
}
