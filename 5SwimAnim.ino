///////////////////////////Анимации проплывания текста///////////////////////////////////
#include "1allInOne.h"

simvol bykovka; // Буфер с символом
int num_byk = 0; // Номер символа, который в буфере
int num_bits;
unsigned int svim_anim = 0;
unsigned int svim_anim_timer = 0;
unsigned int endAnim = WIGHT * 32;


void setTextFrom() {
  svim_anim_timer = millis();

  if (blok_teksta.anim == 5) {
    for (int i = 1; i < WIGHT * 32; i++) {
      if (use_second_array) {
        dataFromSend[i] = dataFromSend2[i - 1];
      }
      else {
        dataFromSend2[i] = dataFromSend[i - 1];
      }
    }
    if (num_byk < -1) { // Так как после загрузки нулевого символа индекс буквы будет -1, а символ вывести надо
      if (use_second_array) {
        dataFromSend[0] = 0;
      }
      else {
        dataFromSend2[0] = 0;
      }
      use_second_array = !use_second_array;
      endAnim++;
      return;
    }

    if (use_second_array) {
      dataFromSend[0] = bykovka.bitMap[num_bits];
    }
    else {
      dataFromSend2[0] = bykovka.bitMap[num_bits];
    }

    num_bits--;
    if (num_bits < 0) {
      while (num_byk >= -1 && !load_next_simvol()) {};
    }

  }

  else if (blok_teksta.anim == 4) {
    for (int i = 0; i < WIGHT * 32 - 1; i++) {
      if (use_second_array) {
        dataFromSend[i] = dataFromSend2[i + 1];
      }
      else {
        dataFromSend2[i] = dataFromSend[i + 1];
      }
    }

    if (num_byk > blok_teksta.leng || num_byk < 0) {
      if (use_second_array) {
        dataFromSend[WIGHT * 32 - 1] = 0;
      }
      else {
        dataFromSend2[WIGHT * 32 - 1] = 0;
      }
      use_second_array = !use_second_array;
      endAnim++;
      return;
    }

    if (use_second_array) {
      dataFromSend[WIGHT * 32 - 1] = bykovka.bitMap[num_bits];
    }
    else {
      dataFromSend2[WIGHT * 32 - 1] = bykovka.bitMap[num_bits];
    }


    num_bits++;
    if (num_bits >= bykovka.lengt) {
      while (!load_next_simvol() && num_byk <= blok_teksta.leng && num_byk >= 0) {};
    }
  }


  use_second_array = !use_second_array;
}


bool set_anim_interv() {
  endAnim = 0;

  byte txt_l = blok_teksta.leng;
  char* text = new char[txt_l];
  for (int i = 0; i < txt_l; i++) text[i] = blok_teksta.text[i];

    if (!use_second_array) {
      for (int i = 0; i < WIGHT * 32; i++) dataFromSend[i] = 0;
    }
    else {
      for (int i = 0; i < WIGHT * 32; i++) dataFromSend2[i] = 0;
    }

  for (int i = 0; i < txt_l; i++) { // Замена переменных на их числовые значения
    if (text[i] == '$') { // Обработчик служебных блоков
      if (text[i + 1] == 'D' && text[i + 2] == 'A' && text[i + 3] == 'T' && text[i + 4] == 'A') {
        i += myGetData(text, txt_l, i);
      }
      if (text[i + 1] == 'T' && text[i + 2] == 'I' && text[i + 3] == 'M' && text[i + 4] == 'E') {
        i += myGetTime(text, txt_l, i);
      }
      if (text[i + 1] == 'T' && text[i + 2] == 'E' && text[i + 3] == 'M' && text[i + 4] == 'P') {
        i += myGetTemp(text, txt_l, i);
      }
      if (text[i + 1] == 'D' && text[i + 2] == 'A' && text[i + 3] == 'Y') {
        i += myGetDay(text, txt_l, i);
      }
    }
  }

  int dlinna_texta_pikseli = 0;


  for (int i = 0; i < txt_l; i++) {
    char* nam = new char[] {"/alpfav/bkAA__.txt"};
    char a = 0;
    char b = text[i];
    if (b >= 0x80) {
      a = b;
      i++;
      b = text[i];
    }

    nam[10] = ((a & 0b11110000) >> 4) + 'A';
    nam[11] = (a & 0b00001111) + 'A';
    nam[12] = ((b & 0b11110000) >> 4) + 'A';
    nam[13] = (b & 0b00001111) + 'A';


    File file = SD.open(nam, FILE_READ);
    delete[] nam;
    if (!file) {
      continue;
    }
    dlinna_texta_pikseli += file.read();
    file.close();
  }


  svim_anim = blok_teksta.anim_time * 1000 / (dlinna_texta_pikseli + WIGHT * 32);

  if (blok_teksta.leng) delete[] blok_teksta.text;
  blok_teksta.text = text;
  blok_teksta.leng = txt_l;

  blok_teksta.text_time =  blok_teksta.anim_time * 1000;
  if (blok_teksta.anim == 4) num_byk = 0;
  else num_byk = blok_teksta.leng - 1;

  while (num_byk >= 0 && num_byk < txt_l && !load_next_simvol()) {

  };

  if (num_byk < -1 || num_byk > txt_l) {
    endAnim = WIGHT * 32;
    blok_teksta.text_time = 0;
    return 0;
  }
  return 1;
}


bool load_next_simvol() {
  char a = 0;
  char b = blok_teksta.text[num_byk];

  if (blok_teksta.anim == 4 && b >= 0x80) {
    a = b;
    num_byk++;
    b = blok_teksta.text[num_byk];
  }

  if (blok_teksta.anim == 5 && (b & 0b11000000) == 0b10000000 ) {
    a = blok_teksta.text[num_byk - 1];
    num_byk--;
  }

  if (blok_teksta.anim == 4) num_byk++;
  else num_byk--;

  char* nam = new char[] {"/alpfav/bkAA__.txt"};
  nam[10] = ((a & 0b11110000) >> 4) + 'A';
  nam[11] = (a & 0b00001111) + 'A';
  nam[12] = ((b & 0b11110000) >> 4) + 'A';
  nam[13] = (b & 0b00001111) + 'A';


  File file = SD.open(nam, FILE_READ);
  delete[] nam;
  if (!file) {
    return 0;
  }

  if (bykovka.lengt) delete[] bykovka.bitMap;

  bykovka.lengt = file.read();
  bykovka.bitMap = new uint32_t[bykovka.lengt];

  for (int i = 0; i < bykovka.lengt; i++) {
    bykovka.bitMap[i] = 0;
    for (int j = 0; j < 4; j++) {
      bykovka.bitMap[i] += (uint32_t)file.read() << 8 * j;
    }
  }

  if (blok_teksta.anim == 4) num_bits = 0;
  else num_bits = bykovka.lengt - 1;

  file.close();
  return 1;
}
