////////////////Главный файл c функциями для обновления панели////////////
#include "1allInOne.h"

/*  Гайд по управлению матрицей:

   1. по SPI Выдаем данные, которые необходимо загрузить в матрицу (сначало это, чтобы матрица поменялась моментально независимо от размеров и количестава загружаемых данных)
   2. nOE Переводим в 0 тем самым получаем доступ к изменению состояния светодиодов, по факту перейдя в режим редактирования панели
   3. ножками A и B Выбираем обновляемую группу светодиодов (ряд)
   4. Подтверждаем выбор подачей короткого импульса на SCLK
   5. Устанавливаем логическую единице на nOE чтобы сохранить настройки и выйти из режима редактирования
   ВАЖНО: одномоментно может гореть только один ряд светодиодов

   P.S. там стоит инвентор(логическое не) поэтому так
*/


void SendData() {

  // 1  2  3  4  5  6  7  8
  // 16 15 14 13 12 11 10 9 <- Пока перевернутые панели

  // 0b0001000100010001 0001000100010001
  // верх                            низ


  for (byte i = 0; i < 4; i++) { // выбор ряда
    for (int t = HEIGTH; t > 0; t--) {
      for (int j = 0; j < WIGHT; j++) { // выбираем пару ПАНЕЛЕЙ по оси х.
        for (int a = 0; a < 4; a++) { //выбор сегмента в пределах одной пнаели по ширине
          for (int b = 0; b < 4; b++) { //выбор сегмента в пределах одной пнаели по высоте
            for (int c = a * 8; c < a * 8 + 8; c ++) {
              if (t <= HEIGTH / 2) {
                digitalWrite(R_pin, !(BufferData[j * 32 + c] & (1 << t * 16 - b * 4 - i - 1)));
              }
              else {
                digitalWrite(R_pin, !(BufferData[WIGHT * 32 - j * 32 - 1 - c] & (1 << t * 16 - 16 + b * 4 + i)));
              }
              digitalWrite(CLK, 1);
              digitalWrite(CLK, 0);

            }
          }
        }
      }
    }

    digitalWrite(nOE, 0);
    digitalWrite(A_pin, !(i & 1));
    digitalWrite(B_pin, !(i > 1));
    digitalWrite(SCLK, 1);
    digitalWrite(SCLK, 0);
    digitalWrite(nOE, 1);
  }

}

float temp_f;

void setText() { // Заполнение нового текстового блока
  byte txt_l = blok_teksta.leng;
  char* text = new char[txt_l];
  for (int i = 0; i < txt_l; i++) text[i] = blok_teksta.text[i];

  if (use_second_array) {
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

  int dlinna_texta_pikseli_and_nachalo_texta = 0;


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
    dlinna_texta_pikseli_and_nachalo_texta += file.read();
    file.close();
  }


  // Отлично, здесь у нас будет информация о том, сколько пикселей занимает запись
  // и массив символов где необходимые переменные заменены на цифры


  dlinna_texta_pikseli_and_nachalo_texta = WIGHT * 16 - dlinna_texta_pikseli_and_nachalo_texta / 2; // Теперь у нас есть координаты начала теста



  for (int i = 0; i < txt_l; i++) {
    if (dlinna_texta_pikseli_and_nachalo_texta >= WIGHT * 32) break; // дальше смысла нет

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
    int lenght = file.read();

    // Далее - цикл занесения необходимой буквы в буфер
    if (dlinna_texta_pikseli_and_nachalo_texta + lenght < 0) {
      dlinna_texta_pikseli_and_nachalo_texta += lenght;
      file.close();
      continue;
    }

    for (int a = 0; a < lenght; a++) {


      if (dlinna_texta_pikseli_and_nachalo_texta < 0) {
        dlinna_texta_pikseli_and_nachalo_texta++;
        file.seek(file.position() + 4);
        continue;
      }
      if (dlinna_texta_pikseli_and_nachalo_texta >= WIGHT * 32) {
        file.close();
        break;

      }

      for (int y = 0; y < 4; y++) {
        if (use_second_array) {
          dataFromSend[dlinna_texta_pikseli_and_nachalo_texta] += uint32_t(file.read()) << y * 8;
        }
        else {
          dataFromSend2[dlinna_texta_pikseli_and_nachalo_texta] += uint32_t(file.read()) << y * 8;
        }
      }

      dlinna_texta_pikseli_and_nachalo_texta++;
    }
    file.close();

  }
  use_second_array = !use_second_array;
  delete[] text;
}


void setBufferWhithoutAnim() { // Заполнение буфера
  if (!use_second_array) {
    for (int i = 0; i < WIGHT * 32; i++)  BufferData[i] = dataFromSend[i];
  }
  else {
    for (int i = 0; i < WIGHT * 32; i++)  BufferData[i] = dataFromSend2[i];
  }
}

int myGetTemp(char* &text, byte &txt_l, int t) {

  if (dst.ready()) {         // измерения готовы по таймеру
    if (dst.readTemp()) {  // если чтение успешно
      temp_f = dst.getTemp();
      dst.requestTemp();  // запрос следующего измерения
    }
  }

  String temp;
  char* ttext = text;
  temp = String(temp_f);

  text = new char[txt_l - 5 + temp.length() - 1];

  for (int i = 0; i < t; i++) {
    text[i] = ttext[i];
  }
  for (int i = t; i < t + temp.length() - 1; i++) {
    text[i] = temp.charAt(i - t);
  }
  for (int i = t + temp.length() - 1; i < txt_l - 5 + temp.length() - 1; i++) {
    text[i] = ttext[i + 5 - temp.length() + 1];
  }

  txt_l = txt_l - 5 + temp.length() - 1;
  delete[] ttext;

  return temp.length() - 1;
}


int myGetData(char* &text, byte &txt_l, int t) {
  Datime dt(dsdt);

  char* bufer = new char[11];
  dt.dateToChar(bufer);

  char* ttext = text;

  text = new char[txt_l - 5 + 8];

  for (int i = 0; i < t; i++) {
    text[i] = ttext[i];
  }
  for (int i = t; i < t + 8; i++) {
    if (i < t + 6) text[i] = bufer[i - t];
    else text[i] = bufer[i - t + 2];
  }
  for (int i = t + 8; i < txt_l - 5 + 8; i++) {
    text[i] = ttext[i + 5 - 8];
  }

  txt_l = txt_l - 5 + 8;
  delete[] ttext;
  delete[] bufer;

  return 8;
}

int myGetTime(char* &text, byte &txt_l, int t) {
  Datime dt(dsdt);
  char* bufer = new char[9];
  dt.timeToChar(bufer);

  char* ttext = text;

  text = new char[txt_l - 5 + 5];

  for (int i = 0; i < t; i++) {
    text[i] = ttext[i];
  }

  for (int i = t; i < t + 2; i++) {
    text[i] = bufer[i - t];
  }
  if (on_off_tochki) {
    text[t + 2] = ':';
  }
  else {
    text[t + 2] = ' ';
  }

  for (int i = t + 3; i < t + 3 + 2; i++) {
    text[i] = bufer[i - t];
  }

  for (int i = t + 5; i < txt_l - 5 + 5; i++) {
    text[i] = ttext[i + 5 - 5];
  }

  txt_l = txt_l - 5 + 5;
  delete[] ttext;
  delete[] bufer;

  return 5;
}

int myGetDay(char* &text, byte &txt_l, int t) {
  Datime dt(dsdt);

  char* bufer = new char[11];
  dt.dateToChar(bufer);

  byte sdvig = bufer[0] == '0' ? 1 : 2;

  char* ttext = text;

  text = new char[txt_l - 4 + sdvig];

  for (int i = 0; i < t; i++) {
    text[i] = ttext[i];
  }

  if (sdvig == 1) {
    text[t] = bufer[1];
  }
  else {
    text[t] = bufer[0];
    text[t + 1] = bufer[1];
  }

  for (int i = t + sdvig; i < txt_l - 4 + sdvig; i++) {
    text[i] = ttext[i + 4 - sdvig];
  }

  txt_l = txt_l - 4 + sdvig;
  delete[] ttext;
  delete[] bufer;

  return  1;
}


// ------------------------------Обработка фоновых анимаций--------------------------------

void sendWithFonAnim() {
  if (!anim_playing) setBufferWhithoutAnim();

  partikle* pt = &pt1;

  if (part_length != 0) {

    // Удаление первой частицы, если надо
    if (millis() > pt->next->timer + pt->next->interv) {
      if (part_length == 1) { // Коли всего одна частица
        delete pt->next;
        part_length--;
        SendData(); // выводим содержимое буффера
        return; // и валим от греха подальше
      }
      else {
        partikle* pt2;
        pt2 = pt->next->next;
        delete pt->next;
        pt->next = pt2;
        part_length--;
      }
    }

    pt = pt->next;



    for (byte i = 0; i < part_length; i++) {

      pt->x += pt->Vx; // перво-наперво надо посчитать физику частицы
      pt->y += pt->Vy;




      // Дальше прописываем отрисовку
      int x = pt->x >> 10;
      int y = pt->y >> 10;


      if (y <= - HEIGTH * 16 && pt->Vy < 0) pt->interv = 0;
      if (y >= HEIGTH * 16 && pt->Vy > 0) pt->interv = 0;
      if (x <= -64 && pt->Vx < 0) pt->interv = 0;
      if (x >= WIGHT * 32 + 64 && pt->Vx > 0) pt->interv = 0;

      byte lengt = 0;
      uint32_t* bitMap;

      if (pt->anim_simv) {
        byte number_art = ((millis() - pt->timer) / fon_anim_smv.tim) % fon_anim_smv.colich;
        lengt = fon_anim_smv.bitMaps[number_art].lengt;
        bitMap = fon_anim_smv.bitMaps[number_art].bitMap;
      }
      else {
        while (chanch_sim);
        lengt = fon_smv.lengt;
        bitMap = fon_smv.bitMap;
      }

      if (pt->interv) {
        if (y > - HEIGTH * 16 && y < 64) {
          for (byte a = 0; a < lengt; a++) {
            if (a + x >= 0 && a + x < WIGHT * 32) {
              if (y < 0) {
                if (a + x >= 0 && a + x < WIGHT * 32) BufferData[a + x] = pt->inv ? BufferData[a + x] ^ (bitMap[a] << y * (-1)) : BufferData[a + x] | (bitMap[a] << y * (-1));
              }
              else {
                if (a + x >= 0 && a + x < WIGHT * 32) BufferData[a + x]  = pt->inv ?  BufferData[a + x] ^ (bitMap[a] >> y) : BufferData[a + x] | (bitMap[a] >> y);
              }
            }
          }
        }
      }


      // Здеся проверка и удаление уже отработавших частиц
      if (i < part_length - 1) {

        if (millis() > pt->next->timer + pt->next->interv) {
          if (i < part_length - 2) {
            partikle* pt2;
            pt2 = pt->next->next;
            delete pt->next;
            pt->next = pt2;
          }
          else {
            delete pt->next;
            part_length--;
            SendData(); // выводим содержимое буффера
            return; // и валим от греха подальше
          }
          part_length--;

        }

        pt = pt->next;
      }

    }
  }

  // Тута добавление новых частиц по таймеру

  if (blok_teksta.fon_anim == -1) {
    SendData();
    return;
  }

  if (part_length < 150 && millis() > timer_fanim) {
    timer_fanim = millis();

    timer_fanim += random((fon_anim.plotn[0]), (fon_anim.plotn[1]));


    pt->next = new partikle;
    pt = pt->next;


    pt->Vx = fon_anim.x_vect + random(fon_anim.x_col[0], fon_anim.x_col[1] + 1);
    pt->Vy = fon_anim.y_vect + random(fon_anim.y_col[0], fon_anim.y_col[1] + 1);

    pt->Vx *= 5;
    pt->Vy *= 5;

    int x = random(fon_anim.x_nach[0], fon_anim.x_nach[1]);
    int y = random(fon_anim.y_nach[0], fon_anim.y_nach[1]);

    pt->x = x << 10;
    pt->y = y << 10;

    pt->inv = fon_anim.invers;
    pt->anim_simv = fon_anim.anim_simv;
    pt->interv = fon_anim.time_an;
    pt->timer = millis();

    part_length++;
  }

  SendData(); // выводим содержимое буффера
}
