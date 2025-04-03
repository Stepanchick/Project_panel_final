/////////////////Главный файл анимаций//////////////
#include "1allInOne.h"

void playAnim() { // Заполнение буфера
  uint32_t anim_timer = 0;

  switch (blok_teksta.anim) {
    case 0: { // Выплывание справа
        for (int i = WIGHT * 32; i >= 0; i--) {
          anim_timer = millis() + blok_teksta.anim_time * 1000 / (WIGHT * 32);
          while (millis() < anim_timer) {
            for (int j = 0; j < WIGHT * 32; j++) {
              if (j >= i) BufferData[j] = use_second_array ? dataFromSend2[j - i] : dataFromSend[j - i];
              else BufferData[j] = 0;
            }
            sendWithFonAnim();
          }
        }
      } break;
    case 1: { // Выплывание слева
        for (int i = 0; i < WIGHT * 32; i++) {
          anim_timer = millis() + blok_teksta.anim_time * 1000 / (WIGHT * 32);
          while (millis() < anim_timer) {
            for (int j = 0; j < WIGHT * 32; j++) {
              if (j < i) BufferData[j] = use_second_array ? dataFromSend2[WIGHT * 32 - (i - j)] : dataFromSend[WIGHT * 32 - (i - j)];
              else BufferData[j] = 0;
            }
            sendWithFonAnim();
          }
        }
      } break;
    case 2: { // Падение
        for (int i = HEIGTH * 16 - 1; i >= 0; i--) {
          anim_timer = millis() + blok_teksta.anim_time * 1000 / (HEIGTH * 16);
          while (millis() < anim_timer) {
            for (int j = WIGHT * 32 - 1; j >= 0; j--) {
              BufferData[j] = (use_second_array ? dataFromSend2[j] : dataFromSend[j]) >> i + 1;
              BufferData[j] &= ((1 << 31) ^ (~0)) >> i;
            }
            sendWithFonAnim();
          }
        }
      } break;
    case 3: { // Всплытие
        for (int i = HEIGTH * 16 - 1; i >= 0; i--) {
          anim_timer = millis() + blok_teksta.anim_time * 1000 / (HEIGTH * 16);
          while (millis() < anim_timer) {
            for (int j = 0; j < WIGHT * 32; j++) {
              BufferData[j] = (use_second_array ? dataFromSend2[j] : dataFromSend[j]) << i;
            }
            sendWithFonAnim();
          }
        }
      } break;
    // case 4: Проплывание, реализовано в отдельном файле
    // case 5: Проплывание, реализовано в отдельном файле
    case 6: { // Шторка справа
        for (int i = WIGHT * 32; i >= 0; i--) {
          anim_timer = millis() + blok_teksta.anim_time * 1000 / (WIGHT * 32);
          while (millis() < anim_timer) {
            for (int j = 0; j < WIGHT * 32; j++) {
              if (j >= i) BufferData[j] = use_second_array ? dataFromSend2[j] : dataFromSend[j];
              else BufferData[j] = 0;
            }
            sendWithFonAnim();
          }
        }
      } break;

    case 7: { // Шторка слева
        for (int i = 0; i < WIGHT * 32; i++) {
          anim_timer = millis() + blok_teksta.anim_time * 1000 / (WIGHT * 32);
          while (millis() < anim_timer) {
            for (int j = 0; j < WIGHT * 32; j++) {
              if (j < i) BufferData[j] = use_second_array ? dataFromSend2[j] : dataFromSend[j];
              else BufferData[j] = 0;
            }
            sendWithFonAnim();
          }
        }
      } break;
  }
  anim_playing = 0;
}
