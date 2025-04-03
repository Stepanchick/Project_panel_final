#include "1allInOne.h"

const char addAnimSimv[] = R"raw(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <Title>Добавление нового анимированного cимвола</Title>
</head>
<body>
  <form name="add" action="/add_anim_simv" method="post">
    <p align=center>
        Анимированный символ - это символ с не статичным изображением, а со сменяющимися через равные промежутки времени картинками.<br>
        ВНИМНИЕ! Работает только с фоновыми анимациями. Не предназнаено для обычного текста.<br>
        Введите добавляемый анимированный cимвол: <input tupe="text" id="simv" name="simv" maxlength="1" />
        <br><br>
        Введите последовательность символов: <input tupe="text" name="simv" maxlength="10" id="wight" />
        <br><br>
        Введите время между соседними картинками в мс (1мс = 0.001с): <input tupe="text" name="simvols" id="wight" type="number" min="1" step="1s">
        <br><br>
        <input type="submit" name="send" value="cохранить cимвол" /><br>
        P.S. После сохранения анимированного символа его нельзя будет изменить, только пересоздать. Изменения картинок систавных символов никак не отобразятся на анимированном символе. <br>
    </p>
   </form>
        <form name="back" action="/go_back" method="get">
            <P align=center>
                <input type="submit" name="send" value="Назад"><br>
            </P>
        </form>
    </body>
)raw";


void renderAdderAnimerSimvoler(WiFiClient &client){
  client.print(addAnimSimv);
  File root = SD.open("/an_alp/");
  if (!root) {
    return;
  }
  File file2 = root.openNextFile();
  client.print("Добавленные анимированные символы: ");
  while (file2) {
      client.print('\'');
      String str = file2.name();
      char s1;
      char s2;
      s1 = ((str.charAt(2) - 'A') << 4) +  str.charAt(3) - 'A';
      s2 = ((str.charAt(4) - 'A') << 4) +  str.charAt(5) - 'A';
      if(s1) client.print(s1);
      client.print(s2);
      client.print("\',  ");
      
      file2 = root.openNextFile();
  }
  root.close();
  client.print("</html>");
}

void parserAnimSimv(String &Data){
  unsigned int sim = 0; 
  while(Data.charAt(sim) != '=' && Data.length() > sim){
    sim++;  
  };
  sim++;
  char b = Data.charAt(sim);
  char a = 0;
  if(b == '%'){
    sim++;
    b = Rutext(Data.charAt(sim), Data.charAt(++sim));
    sim++;
  }
  if(byte(b) >= 0x80){
    a = b;
    b = Data.charAt(sim);
    if(b == '%'){
      sim++;
      b = Rutext(Data.charAt(sim), Data.charAt(++sim));
      sim++;
    }
  }

  
  char* nam = new char[]("/an_alp/bk____/");
  nam[10] = (a >> 4) + 'A';
  nam[11] = (a & 0b00001111) + 'A';
  nam[12] = (b >> 4) + 'A';
  nam[13] = (b & 0b00001111) + 'A';

  File root = SD.open(nam);
 if (root) {
    if (!root.isDirectory()) {
      root.close();
      SD.remove(nam);
      SD.mkdir(nam);
    }
    else{
      String namS = "/an_alp/bk____/";
      namS[10] = ((a & 0b11110000) >> 4) + 'A'; // Не по классике, потому что мне лень делать перескок между 9 и А
      namS[11] = (a & 0b00001111) + 'A';
      namS[12] = ((b & 0b11110000) >> 4) + 'A';
      namS[13] = (b & 0b00001111) + 'A';
      
      File file = root.openNextFile();
      while (file) {
        String namS2 = namS;
        namS2 += file.name();
        SD.remove(namS2);
        file = root.openNextFile();
      }
      root.close();
    }
  }
  else{
    SD.mkdir(nam);
  }
  
  delete[] nam;

  nam = new char[]("/an_alp/bk____/bitMp0.txt");
  nam[10] = (a >> 4) + 'A';
  nam[11] = (a & 0b00001111) + 'A';
  nam[12] = (b >> 4) + 'A';
  nam[13] = (b & 0b00001111) + 'A';

  while(Data.charAt(sim) != '=' && Data.length() > sim){
    sim++;  
  };
  sim++;

  char art = '0';
  
  byte collection = 0;
   
  while(Data.charAt(sim) != '&' && Data.length() > sim){
    collection++;
    nam[20] = art;
    File file = SD.open(nam, FILE_WRITE);
    char d = Data.charAt(sim);
    char c = 0;
    if(d == '%'){
      sim++;
      d = Rutext(Data.charAt(sim), Data.charAt(sim + 1));
      sim++;
      if(int(d) >= 0x80){
        c = d;
        sim++;
        d = Data.charAt(sim);
        if(d == '%'){
          sim++;
          d = Rutext(Data.charAt(sim), Data.charAt(sim + 1));
          sim++;
        }
      }
    }


    char* nam2 = new char[] {"/alpfav/bk____.txt"};
    nam2[10] = ((c & 0b11110000) >> 4) + 'A'; // Не по классике, потому что мне лень делать перескок между 9 и А
    nam2[11] = (c & 0b00001111) + 'A';
    nam2[12] = ((d & 0b11110000) >> 4) + 'A';
    nam2[13] = (d & 0b00001111) + 'A';
  
    File file2 = SD.open(nam2, FILE_READ);
    if (!file2) {
      file.println(char(0));
      
    }
    else{
      while(file2.available()){
        file.print(char(file2.read()));
      }
    }
    file2.close();
    file.close();
    sim++;
    art++;
  };
  delete[] nam;
  sim++;

  nam = new char[]("/an_alp/bk____/contr.txt");
  nam[10] = (a >> 4) + 'A';
  nam[11] = (a & 0b00001111) + 'A';
  nam[12] = (b >> 4) + 'A';
  nam[13] = (b & 0b00001111) + 'A';
  File file = SD.open(nam, FILE_WRITE);
  delete[] nam;
  file.println(char(collection));
  while(Data.charAt(sim) != '=' && Data.length() > sim) {
    sim++;
  }
  sim++;
  uint32_t tm = 0;
  while(Data.charAt(sim) != '&' && Data.length() > sim) {
    file.print(Data.charAt(sim));
    tm *= 10;
    tm += byte(Data.charAt(sim));
    sim++;
  }
  file.print('\n');
  file.close();

  nam = new char[]("/alpfav/bk____.txt");
  nam[10] = (a >> 4) + 'A';
  nam[11] = (a & 0b00001111) + 'A';
  nam[12] = (b >> 4) + 'A';
  nam[13] = (b & 0b00001111) + 'A';
  
  SD.remove(nam);

  delete[] nam;
}
