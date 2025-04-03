#include "1allInOne.h"

const char addFanim1[] = R"raw(
<!DOCTYPE html>
<html>

<head>
  <meta charset="utf-8">
  <Title>Добавление фоновой анимации</Title>
</head>
<body>
  <br><br>
  <form name="create" method="post" action="/create_fon_anim">
    <p align=center>
      <label>
        Введите название новой фоновой анимации:
        <input type="text" value="#ЗНАЧ!" maxlength="10" name="name">
      </label><br><br>

      <label>
        Введите символ, который будет болтаться на фоне:
        <input type="text" value="#" name="simv" maxlength="1">
      </label><br><br>

      <label>
        Размеры матрицы 32 * 96. Начало координат - левый нижний угол.<br>
        Введите вектор движения(скорости) частицы(символа) в пикселях в секунду(целое число).<br>
        x:
        <input type="text" value="0" name="x1">
        y:
        <input type="text" value="0" name="y1">
      </label><br><br>
      <label>
        Время перемещения частицы:
        <input type="text" value="10" name="time">
      </label><br><br>
      <label>
        Введите возможные колебания символа в формате "x1; x2" и "y1; y2" (Возможны отрицательные значения).<br>
        Колебания - это случайные отклонения от заданного движения.<br>
        Выбирается случайное число из указанного промежутка и вектор скорости частицы сдвигается влево(-) или вправо(+) на указанное число.(пустое поле = 0;0)<br>

        По x:
        <input type="text" value="0" name="x3">
        По y:
        <input type="text" value="0" name="y3">
      </label><br><br>
      <label>
        Инверсия пикселей: <input type="checkbox" name="invers"><br>
        (Если симол пересекается с текстом или с другой частицей то пиксель пересечения тухнет.)<br><br>
        
      </label>
      <label>
        Координаты начала движения частиц(Число или промежуток в формате "a1; a2"):<br><br>
        x:
        <input type="text" value="0" name="x4">
        y:
        <input type="text" value="0" name="y4">
      </label><br><br>
      <label>
        Время между соседними частицами(одномоментно могут существовать 100 частиц) - число или интервал(t1; t2) допустимы дробные значения:
        <input type="text" value="1" name="p1">
      </label><br><br>
      <button type="submit">Сохранить</button>
    </p>
  </form>
  
  <form name="back" action="/go_back" method="get">
    <P align=center>
      <input type="submit" name="send" value="Назад"><br>
    </P>
  </form><br><br>
  <P align=center>
  ----------------------------ИЛИ----------------------------<br><br>
  </P>
  <form name="delete" action="/delete_anim" method="post">
    <P align=center>
     <select name="del_anims" id="anims_select">
     <option value="-1">Выберите анимацию.</option>
)raw";
const char addFanim2[] = R"raw(
     </select><br><br>
      <input type="submit" name="send" value="Удалить"><br>
    </P>
  </form>
  
</body>
</html>
)raw";



void rederAddFonAnim(WiFiClient &client) {
  client.print(addFanim1);

  File file;

  char* nam = new char[]("/fon_an/anim__.txt"); 
  for(int i = 0;; i++){
    nam[12] = i / 10 + '0';
    nam[13] = i % 10 + '0';
    file = SD.open(nam, FILE_READ);
    if(!file) break;
    
    client.print("<option value=\"");
    client.print(i);
    client.print("\">");

    char c = file.read();
    while(c != '\n'){
      file_trn(client, c);
      c = file.read();
    }

    
    client.print("</option>");
  }
  delete[] nam;
  
  client.print(addFanim2);
}

void deleteFonAnim(String& Data){
  int simv = 0;
  while(simv < Data.length()){
    if (Data.charAt(simv) == '='){
      simv++;
      simv = Data.substring(simv, Data.indexOf(';')).toInt();
      if (simv < 0) return;
      else{
        char* nam = new char[]("/fon_an/anim__.txt"); // ищем пустой файлъ
        nam[12] = simv / 10 + '0';
        nam[13] = simv % 10 + '0';
        SD.remove(nam);
        simv++;
        char* nam2 = new char[]("/fon_an/anim__.txt"); // ищем пустой файлъ
        nam2[12] = simv / 10 + '0';
        nam2[13] = simv % 10 + '0';
        
        for(; SD.rename(nam2, nam);){
          nam[12] = simv / 10 + '0';
          nam[13] = simv % 10 + '0';
          simv++;
          nam2[12] = simv / 10 + '0';
          nam2[13] = simv % 10 + '0';
        }
    
        delete[] nam;
        delete[] nam2;
      }
      return;
    }
    simv++;
  }
}


void parseAnims(String& Data){
    unsigned int simv = 0;
    byte num = 0;
    String dannie = "";
  
    byte blok = 0;
    fon_animations anims;
    File file;

    char* nam = new char[]("/fon_an/anim00.txt"); // ищем пустой файлъ
    for(; SD.open(nam, FILE_READ); num++){
      nam[12] = num / 10 + '0';
      nam[13] = num % 10 + '0';
    }

    file = SD.open(nam, FILE_WRITE);
    delete[] nam;
    
    while (simv < Data.length()) {
      if (Data.charAt(simv) == '=') { // Знак равенства означает, что дальше пойдет значение
        simv++;
        while (Data.charAt(simv) != '&' && simv < Data.length()) { 
          if (Data.charAt(simv) != '%') {   // Если нет знака % значит символ передался за раз
            dannie += Data.charAt(simv) != '+' ? Data.charAt(simv): ' ';
            simv++;
          }
          else { //Если стоит знак % значит вместо символа отправлен его код в 16-ричной системе
            simv++;
            dannie += Rutext(Data.charAt(simv), Data.charAt(simv + 1));
            simv += 2;
          }
          
        }
        
        
        
        switch (blok){
          case 0: { // Имя
            file.print(dannie);
            file.print('\n');
            blok++; 
          } 
            break;
          case 1: { //  Символ
            file.print(dannie);
            file.print('\n');
            blok++; 
          } 
            break;
          case 2: { // Вектор X
            file.print(dannie);
            file.print('\n');
            blok++;
          }break;
          case 3:{ // Вектор Y
            file.print(dannie);
            file.print('\n');
            blok++; 
          }break;
          case 4: { // Время перемещения частицы
            file.print(dannie);
            file.print('\n');
            blok++;
          }break;
          case 5: { // Смещение X
            if (dannie.indexOf(';') == -1){
              file.print(dannie);
              file.print(' ');
              file.print(dannie);
              file.print('\n');
            }
            else {
              file.print(dannie.substring(0, dannie.indexOf(';')));
              file.print(' ');
              file.print(dannie.substring(dannie.indexOf(';') + 1));
              file.print('\n');
            }            
            blok++;
          }break;
          case 6: { // Смещение Y
            if (dannie.indexOf(';') == -1){
              file.print(dannie);
              file.print(' ');
              file.print(dannie);
              file.print('\n');
            }
            else {
              file.print(dannie.substring(0, dannie.indexOf(';')));
              file.print(' ');
              file.print(dannie.substring(dannie.indexOf(';') + 1));
              file.print('\n');
            } 
            blok++;
          }break;
          case 7: { // Инверсия + Координаты начала X
            if(dannie.indexOf("on") != -1) {
              file.print('+');
              break;
            }
            else{
              file.print('\n');
            }
            if (dannie.indexOf(';') == -1){
              file.print(dannie);
              file.print(' ');
              file.print(dannie);
              file.print('\n');
            }
            else {
              file.print(dannie.substring(0, dannie.indexOf(';')));
              file.print(' ');
              file.print(dannie.substring(dannie.indexOf(';') + 1));
              file.print('\n');
            } 
            blok++;
          }break;
          case 8: { // Координаты начала Y
            if (dannie.indexOf(';') == -1){
              file.print(dannie);
              file.print(' ');
              file.print(dannie);
              file.print('\n');
            }
            else {
              file.print(dannie.substring(0, dannie.indexOf(';')));
              file.print(' ');
              file.print(dannie.substring(dannie.indexOf(';') + 1));
              file.print('\n');
            } 
            blok++;
          }break;
          
          case 9: { // Интервал между
            if(dannie.indexOf(';') == -1){
              if(dannie.indexOf('.') != -1){
                file.print(dannie.substring(0, dannie.indexOf('.')));
                String dannie2 = dannie.substring(dannie.indexOf('.') + 1);
                if(dannie2.length() >= 3){
                  file.print(dannie2.substring(0, 3));
                  file.print(" ");
                  file.print(dannie.substring(0, dannie.indexOf('.')));
                  file.print(dannie2.substring(0, 3));
                }
                else{
                  file.print(dannie2);
                  for(int i = dannie2.length(); i < 3; i++){
                    file.print('0');
                  }
                  file.print(" ");
                  file.print(dannie.substring(0, dannie.indexOf('.')));
                  file.print(dannie2);
                  for(int i = dannie2.length(); i < 3; i++){
                    file.print('0');
                  }
                }
              }
              else {
                file.print(dannie);
                file.print("000");
                file.print(" ");
                file.print(dannie);
                file.print("000");
              }
            }
            
            else{
              if(dannie.substring(0, dannie.indexOf(';')).indexOf('.') != -1){
                file.print(dannie.substring(0, dannie.indexOf('.')));
                dannie = dannie.substring(dannie.indexOf('.') + 1);
                if(dannie.substring(0, dannie.indexOf(';')).length() >= 3){
                  file.print(dannie.substring(0, 3));
                }
                else{
                  file.print(dannie.substring(0, dannie.indexOf(';')));
                  for(int i = dannie.substring(0, dannie.indexOf(';')).length(); i < 3; i++){
                    file.print('0');
                  }
                }
              }
              else {
                file.print(dannie.substring(0, dannie.indexOf(';')));
                file.print("000");
              }

              dannie = dannie.substring(dannie.indexOf(';')+1);
              file.print(' ');

              if(dannie.indexOf('.') != -1){
                file.print(dannie.substring(0, dannie.indexOf('.')));
                dannie = dannie.substring(dannie.indexOf('.') + 1);
                if(dannie.length() >= 3){
                  file.print(dannie.substring(0, 3));
                }
                else{
                  file.print(dannie);
                  for(int i = dannie.length(); i < 3; i++){
                    file.print('0');
                  }
                }
              }
              else {
                file.print(dannie);
                file.print("000");
              }

              file.print('\n');
              file.close();
            }
                     
            partikle* t = &pt1;
            partikle* t2;
            byte pl = part_length;
            part_length = 0;
            
            
            if(pl > 0) t = t->next;
            for(int i = 0; i < pl - 2; i++){
              t2 = t->next->next;
              delete t->next;
              t->next = t2;
            }
            if(pl > 1) delete t->next;
        }break;
      }
      dannie = ""; 
    }
    simv++;
  }
}
