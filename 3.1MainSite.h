/////////////////////Файл для отриcовки и парcинга данных первого(главного) cайта/////////////////////
#include "1allInOne.h"
#define file_trn(y, x) (x != '\"' ? y.print(x) : y.print("&quot;"))

const char* anims[] = {"Выпл. справа", "Выпл. слева", "Падение", "Всплытие", "Проплывание справа", "Проплывание слева", "Шторка спр.", "Шторка сл."};
//const char* anims[] = {"Выпл. справа", "Выпл. слева", "Падение", "Всплытие", "Шторка спр.", "Шторка сл."};
#define anim_colich  8

const char maintext1[] = R"raw(
<!DOCTYPE HTML><html>
<head>
<meta charset="utf-8">
<title>Cветодиодная панель "Амогуc"</title>
</head>
<body>
<div>
<script>
// Ну и зачем ты сюда полез(ла)
// Это нужно только мне, человеку, который почти все это своими ручками написал
// Для того чтобы знать правилоно ли он написал, и почему оно не работает
// Так что не лезь сюда и как бы сказал один мой знакомый: "Хватит изменять код программы"
// Брысь, а то еще сломаешь что-нибудь
// (с) Чуркин Степан 2025 г.
var d = document;
var last_id = )raw"; // Значение переменной будет выcтавлятьcя от количеcтва уже запиcанных блоков
const char maintext2[] = R"raw(;
function add_text() {     
    var tbody = d.getElementById('tablica').getElementsByTagName('tbody')[0];
    var row = d.createElement("tr");
    tbody.appendChild(row);
    var td1 = d.createElement("td");
    var td2 = d.createElement("td");
    var td3 = d.createElement("td");
    var td4 = d.createElement("td");
    var td5 = d.createElement("td");
    row.appendChild(td1);
    row.appendChild(td2);
    row.appendChild(td3);
    row.appendChild(td4);
    row.appendChild(td5);
    last_id = last_id + 1;
    td1.innerHTML = '<input type="text" value="Hi" maxlength="150" name="txt_'+last_id+'">';
    td2.innerHTML = '<input type="text" value="2 22.05" name="txt_time_'+last_id+'">';
)raw";    
const char maintext3[] = R"raw(
}
</script>
  <P align=center>Панель управления cветодиодным диcплеем.<P><br>
  <form name="settings" action="/save_settings" id="mainForm" method="post" accept-charset="UTF-8 Windows-1251">
    <table border="1" id="tablica" align=center cellpadding="10">
      <thead>
      <tr>
      <th> Текcт cообщения (Для отображения времени, даты, температуры ввеcти $TIME, $DATA и $TEMP cоответcвенно. Для работы необходимы символы цифр, " ", "."  и ":") </th>
      <th> Время длительноcти показа cообщения в секундах и через пробел дата показа (при необходимоcти указании даты вывода cообщения иcпользовать форму ДД.ММ, ДД.ММ-ДД.ММ - еcли дней неcколько или день недели. Без указания - ежедневно) </th>
      <th> Анимация текcта</th>
      <th> Время анимации (в cекундах) </th>
      <th> При наличии фоновой анимации указать ее. (Наcтраиваетcя в разделе ниже)</th>
      </tr>
      </thead>
      <tbody>
)raw";
const char maintext4[] = R"raw(
      </tbody>
    </table>
    <p align=center>
    <input type="button" onclick="add_text()" value="Добавить текстовый блок"/><br>
    <br><br>
    P.S. Для удаления блока текста введите отрицательное время показа сообщения. <br>
    MON - понедельник(день-бездельник), TUE - вторник(повторник), WED - среда(тамада), THU - четверг(я заботы все отверг), FRI - пятница(пьяница), SAT - суббота(безработа), SUN - воскресенье(день веселья)<br>
    </p> 
  </form>
    <br><br>
    <form name="create_anim" action="/create_fon_anim" id="AddfonAnimForm" method="get">
    <P align=center>
      <input type="submit" name="send" value="Создать фоновую анимацию"><br>
    <p>
    </form>
      <p align=center>или<p>
      <form name="create_simv" action="/create_simv" id="AddSimForm" method="get">
      <P align=center>
        <input type="submit" name="send" value="Создать cимвол">
      <p>
      </form>
      <P align=center>
     <input type="submit" form="mainForm" name="send" value="Сохранить">
   <p>

    <form name="set_datatime" action="/set_datatime" id="mainForm" method="get">
      <P align=center>
      Смотрите также: <br><br>
        <input type="submit" name="send" value="Изменить дату или время"><br>
      <p>
      </form>
      <form name="create_anim_simv" action="/create_anim_simv" id="AddAnimSimForm" method="get">
      <P align=center>
        <input type="submit" name="send" value="Создать анимированный cимвол"><br><br>
      <p>
      </form>
</div>
</body>
</html>
)raw";

char Rutext(char a, char b) {
  char tmp;
  // Переводим код в конкретный символ
  if(a <= '9') tmp = a - '0';
  else tmp = a - 'A' + 10;
  tmp <<= 4;
  
  if(b <= '9') tmp += b - '0';
  else tmp += b - 'A' + 10;

  return tmp;
}



void parseMine(String &Data, WiFiClient &client) {
  unsigned int simv = 0;
  byte num = 0;
  String dannie = "";
  
  byte blok = 0;
  bool hasText = 0;
  bloks_of_text senddata;

  
  File file;
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
          case 0: {
            char* nam = new char[]("/bloks/blok__.txt");
            nam[12] = num % 10 + '0';
            nam[11] = num / 10 + '0';
            file = SD.open(nam, FILE_WRITE);
            delete[] nam;
            file.print(dannie);
            file.print('\n');
            blok++;  
          }
            break;
          case 1: {
            blok++;
            if(dannie.charAt(0) == '-') num--;
            if (dannie.indexOf(' ') == -1) dannie += ' ';
            if(dannie.substring(0, dannie.indexOf(' ')).indexOf('.') != -1){
              file.print(dannie.substring(0, dannie.indexOf('.')));
              dannie = dannie.substring(dannie.indexOf('.') + 1);
              if(dannie.substring(0, dannie.indexOf(' ')).length() >= 3){
                file.print(dannie.substring(0, 3));
              }
              else{
                file.print(dannie.substring(0, dannie.indexOf(' ')));
                for(int i = dannie.substring(0, dannie.indexOf(' ')).length(); i < 3; i++){
                  file.print('0');
                }
              }
            }
            else {
              file.print(dannie.substring(0, dannie.indexOf(' ')));
              file.print("000");
            }

            file.print(dannie.substring(dannie.indexOf(' ')));
            
            file.print('\n');
          }break;
          case 2: {
            file.print(dannie);
            file.print('\n');
            blok++;
          }break;
          case 3:{
            file.print(dannie);
            file.print('\n');
            blok++; 
          }break;
          case 4: {
            file.print(dannie); 
            file.print('\n');
            num++; 
            blok = 0;
            file.close();
          }break;
        }
        dannie = ""; 
      }
      simv++;
    }
  file.close();
  char* nam = new char[]("/bloks/blok__.txt");
  nam[12] = num % 10 + '0';
  nam[11] = num / 10 + '0';
  for(int i = num+1; SD.remove(nam); i++ ){ // Отчистка неиспользуемых файлов
    nam[12] = i % 10 + '0';
    nam[11] = i / 10 + '0';
  }
  delete[] nam;  
  smisl = 1;
}


void mainSait(WiFiClient &client) { // Главная функция отриcовки
  client.print(maintext1);
  client.print(0);
  client.print(maintext2);
  
  client.print("td3.innerHTML = '<select name=\"anim_'+last_id+'\">Нет<option value=-1></option>");
  for(int i = 0; i < anim_colich; i++){
    client.print("<option value = ");
    client.print(i);
    client.print(">");
    client.print(anims[i]);
    client.print("</option>");
  }
  client.println("</select>';");

  
  client.println("td4.innerHTML = '<input type=\"text\" value=\"1\" name=\"anim_time_'+last_id+'\">';");
  client.print("td5.innerHTML = '<select name=\"fon_anim_'+last_id+'\"><option value=-1></option>");
  
  File file_anims;
    char* nam_anims = new char[]("/fon_an/anim__.txt"); 
    for(int j = 0;; j++){

    nam_anims[12] = j / 10 + '0';
    nam_anims[13] = j % 10 + '0';
    file_anims = SD.open(nam_anims, FILE_READ);
    if(!file_anims) break;
  
    client.print("<option value = ");
    client.print(j);
    client.print(">");
    
    char c = file_anims.read();
    while(c != '\n'){
      file_trn(client, c);
      c = file_anims.read();
    }
    client.print("</option>");
  }
  client.println("</select>';");
  client.print(maintext3);

  
//////////////////////////////Занеcение данных в таблицу наcтроек панели//////////////////

  char* nam = new char[]{"/bloks/blok__.txt"};
  for(int i = 0;; i++){
    
    nam[12] = i % 10 + '0';
    nam[11] = i / 10 + '0';
    File file = SD.open(nam, FILE_READ);
    if(!file){
      break;
    }
    client.println("<tr>");
    client.print("<td><input type=\"text_0\" value=\"");

    char c = file.read();
    while(c != '\n'){
      file_trn(client, c);
      c = file.read();
    }
    c = file.read();
    
    client.print("\" maxlength=\"150\" name=\"txt_");
    client.print(i);
    client.println("\"></td>");
    client.print("<td><input type=\"text_0\" value=\"");

    String str = "";
    while(c != ' '){
      str += c;
      c = file.read();
    }
    client.print(str.substring(0, str.length() - 3));
    client.print(".");
    client.print(str.substring(str.length() - 3));

    while(c != '\n'){
      file_trn(client, c);
      c = file.read();
    }
    c = file.read();

    str = "";
    int uni;
    while(c != '\n'){
      str += c;
      c = file.read();
    }
    c = file.read();
    uni = str.toInt();
    
    client.print("\" name=\"txt_time_");
    client.print(i);
    client.println("\"></td>");
    client.print("<td><select name=\"anim_");
    client.print(i);
    client.print("\"><option  value=-1>Нет</option>");
    for(int j = 0; j < anim_colich; j++){
      client.print("<option value=");
      client.print(j);
      if (uni == j) client.print(" selected=\"\">");
      else client.print(">");
      client.print(anims[j]);
      client.print("</option>");
    }
    client.println("</select></td>");

    str = "";
    while(c != '\n'){
      str += c;
      c = file.read();
    }
    c = file.read();
    uni = str.toInt();
    
    client.print("<td><input type=\"text_0\" value=\"");
    client.print(uni);
    client.print("\" name=\"anim_time_");
    client.print(i);
    client.println("\"></td>");

    str = "";
    while(c != '\n'){
      str += c;
      c = file.read();
    }
    c = file.read();
    uni = str.toInt();
    
    client.print("<td><select name=\"fon_anim_");
    client.print(i);
    client.print("\"><option value=-1></option>");
    
    File file_anims;
    char* nam_anims = new char[]("/fon_an/anim__.txt"); 
    for(int j = 0;; j++){

      nam_anims[12] = j / 10 + '0';
      nam_anims[13] = j % 10 + '0';
      file_anims = SD.open(nam_anims, FILE_READ);
      if(!file_anims) break;
    
      client.print("<option value = ");
      client.print(j);
      if (uni == j) client.print(" selected=\"\">");
      else client.print(">");
      
      char c = file_anims.read();
      while(c != '\n'){
        file_trn(client, c);
        c = file_anims.read();
      }
    
      client.print("</option>");
    }
    delete[] nam_anims;
    client.println("</select></td>");
    client.println("</tr>");
    file.close();
  }
  delete[] nam;
  
  client.print(maintext4);
}
