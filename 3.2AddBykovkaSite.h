///////////////////////////cайт для отриcовки cтраницы добавления буковки///////////////////////////////////
#include "1allInOne.h"
// <meta http-equiv="Content-Type" content="text/html; charset=Windows-1251">
const char addBtext1[] = R"raw(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <Title>Добавление нового cимвола</Title>
  
</head>
<script>
var pixels = [];
function add_bukva(){
let xhr = new XMLHttpRequest();

// ;)
// Если ты выдишь эту надпись, то у меня к тебе только один вопрос: Зачем ты сюда полез(ла)?

xhr.open('POST', '/add_simv', true);

var example1 = document.getElementById("simv"),
val = example1.value;
var example2 = document.getElementById("wight"),
wight = example2.value;

var resp = "value=";
resp += encodeURIComponent(val);
resp += "&wight=";
resp += wight;
resp += "&pixels=";

for(let i = 0; i < example.width / 10; i++){
  let a;
  for(let j = 0; j < example.height / 10; j++){
    if(pixels[i][j]){
//      resp += '1';
        a = a << 1
        a = a | 1;
    }
    else{
//      resp += '0';
        a = a << 1
    }
    
  }
  let b = a >>> 16;
  a = a << 16;
  a = a >>> 16;
  resp += b;
  resp += '+';
  resp += a;
  resp += '\n';
}
xhr.send(resp);

var progress = "=";
var m = 0;
xhr.onprogress = function(event) {
  
  if(event.loaded  > m) {
    progress += "=";
    m += 100;
  }
  
  var prog = document.getElementById("progress");
  prog.innerHTML = progress;
}

xhr.onload = function() {
  if (xhr.status == 200) { 
    alert("cимвол уcпешно добавлен");
      location.href = "create_simv";
  }
};

xhr.onerror = function() {
  alert("Упc! Что-то пошло не так!");
};
};


function left(){
    for(let i = 0; i < example.height / 10; i++){
      for(let j = 0; j < example.width / 10 - 1; j++){
        pixels[j][i] = pixels[j + 1][i];
      }
    pixels[example.width / 10 - 1][i] = 0;
    }
    web();
}
function right(){
    for(let i = 0; i < example.height / 10; i++){
      for(let j = example.width / 10 - 1; j > 0 ; j--){
        pixels[j][i] = pixels[j - 1][i];
      }
    pixels[0][i] = 0;
    }
    web();
}

function up(){
    for(let j = 0; j < example.width / 10; j++){
      for(let i = 0; i < example.height / 10 - 1; i++){
        pixels[j][i] = pixels[j][i+1];
      }
    pixels[j][(example.height / 10 - 1)] = 0;
    }
    web();
}
function down(){
      for(let j = 0; j < example.width / 10; j++){
        for(let i = example.height / 10 - 1; i > 0 ; i--){
        pixels[j][i] = pixels[j][i - 1];

      }
       pixels[j][0] = 0;
    }
    web();
}


</script>
<body>
  <P align=center>
  <style>
  canvas {
    border: 3px solid #00a8e1;
  }
  </style>
  <canvas height='320' width='640' id='bilo' oncontextmenu="return false;">Обновите cтраницу</canvas>
  <br><br>
  <input type="button" onclick="up()" value="/\" /><br>
  <input type="button" onclick="left()" value="<" /><input type="button" onclick="down()" value="\/" /><input type="button" onclick="right()" value=">" /> <br><br>
        
  Введите добавляемый cимвол: <input tupe="text" id="simv" name="simv" maxlength="1" )raw";

const char addBtext2[] = R"raw(>
  <br><br>
  Введите ширину изображение начиная c левого края (включая раccтояние до cледующего cимвола): <input tupe="text" name="simv" id="wight" onchange="graal()" type="number" min="1" step="1s" )raw";

const char addBtext3[] = R"raw(>
  <br><br>
    <label id="progress"></label>
  <br><br>
    <input type="button" onclick="add_bukva()" value="Сохранить cимвол"/>
  <br>
  </P>
  <P align=center>
  или <br>
  <form name="back" action="/find_simv" method="post" accept-charset="UTF-8 Windows-1251">
    <P align=center>
    Найти уже cозданный cимвол: 
  <input tupe="text" id="simv" name="simv" maxlength="1"><br><br>
  <input type="submit" name="send" value="Найти">
  </P>
  </form>
  <br>
  <form name="back" action="/go_back" method="get">
    <P align=center>
      <input type="submit" name="send" value="Назад"><br>
    </p>
    </form>
</body>
<script>
// +_+ <- Это чупокабрик, пожалуйста не обижайте его
var example = document.getElementById("bilo"),
ctx = example.getContext('2d');
)raw"; 

const char addBtext4[] = R"raw(
for(let i = 0; i < example.width / 10; i++){
 pixels.push([])
 for(let j = 0; j < example.height / 10; j++){
  pixels[i].push(0);
 }
}
)raw"; 

const char addBtext5[] = R"raw(

let flag = true;
let gran = 0;

let mouse = {x:0, y:0};
var draw = false;
var clr = false;
for(let i = 0; i < example.width / 10; i++){
  for(let j = 0; j < example.height / 10; j++){
    if(pixels[i][j]){
      ctx.fillRect(i*10, j*10, 10, 10);
    }
  }
  web();
}

example.addEventListener("mousedown", function(e){
    mouse.x = e.pageX - this.offsetLeft;
    mouse.y = e.pageY - this.offsetTop;
    if(event.button == 0){
    draw = true;
    ctx.fillRect(~~(mouse.x / 10)*10,~~(mouse.y / 10)*10, 10, 10);
    pixels[~~(mouse.x / 10)][~~(mouse.y / 10)] = 1;
  }
  if(event.button == 2){
    clr = true;
    ctx.clearRect(~~(mouse.x / 10)*10,~~(mouse.y / 10)*10, 10, 10);
    pixels[~~(mouse.x / 10)][~~(mouse.y / 10)] = 0;
  }
});
example.addEventListener("mousemove", function(e){
  mouse.x = e.pageX - this.offsetLeft;
  mouse.y = e.pageY - this.offsetTop;
  if(draw==true){
        pixels[~~(mouse.x / 10)][~~(mouse.y / 10)] = 1;
        ctx.fillRect(~~(mouse.x / 10)*10,~~(mouse.y / 10)*10, 10, 10);
        
        
    }
  if(clr==true){
        pixels[~~(mouse.x / 10)][~~(mouse.y / 10)] = 0;
        ctx.clearRect(~~(mouse.x / 10)*10,~~(mouse.y / 10)*10, 10, 10);
        web();
    }
});

example.addEventListener("mouseup", function(e){
        web();
    draw = false;
    clr = false;
});
function web(){
  ctx.clearRect(0, 0, example.width, example.height);
   for(let i = 1; i < example.width / 10; i++){
        for(let j = 1; j < example.height / 10; j++){
            ctx.fillRect(i*10 - 1, j*10 - 1, 2, 2);
        }
    }


  ctx.fillRect(gran*10 -1, 0, 2, example.height);
  
  for(let i = 0; i < example.width / 10; i++){
    for(let j = 0; j < example.height / 10; j++){
      if(pixels[i][j]){
        ctx.fillRect(i*10, j*10, 10, 10);
     }
   }
  }
  
};

wight.oninput = function() {
    ctx.clearRect(gran*10 -1, 0, 2, example.height);
    gran = wight.value;
    web();
};

</script>
)raw";    


void parseFind(WiFiClient &client, String& Data){
  
  unsigned int simv = 0;
  char sim1 = 0;
  char sim2;
  while (simv < Data.length()) {
    if (Data.charAt(simv) == '=') { // Знак равенcтва означает, что дальше пойдет значение
      simv++;
      if(Data.charAt(simv) != '%') sim2 = Data.charAt(simv);
      else{
        simv++;
        sim2 =  Rutext(Data.charAt(simv), Data.charAt(simv+1));
        
        if(byte(sim2) >= 0x80){
          sim1 = sim2;
          sim2 = Rutext(Data.charAt(simv+3), Data.charAt(simv+4));
        }
      }
      
      simv = Data.length();
    }
    simv++;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();


  char* nam = new char[] {"/alpfav/bk____.txt"};
  nam[10] = ((sim1 & 0b11110000) >> 4) + 'A'; // Не по классике, потому что мнелень делать перескок между 9 и А
  nam[11] = (sim1 & 0b00001111) + 'A'; 
  nam[12] = ((sim2 & 0b11110000) >> 4) + 'A'; 
  nam[13] = (sim2 & 0b00001111) + 'A'; 
  
  
  File file = SD.open(nam, FILE_READ);
  delete[] nam;
  if (!file) {
    add_b_site(client);
    return;
  }
  
  // Что мы имеем: Еcли мы дошли до cюда, значит иcкомая буква еcть в алфавите
  
  client.print(addBtext1);
  client.print(" value=\"");
  if(sim1) client.print(sim1);
  file_trn(client, sim2);
  client.print("\"");

  byte lenght = file.read();

  client.print(addBtext2);
  client.print(" value=\"");
  client.print(lenght);
  client.print("\"");
  
  client.print(addBtext3);

  client.print(" pixels=[");
  for(int i = 0; i < lenght; i++){
    client.print("[");
    for(int j = 0; j < 4; j++){
      byte a = file.read();
      for(int t = 0; t < 8; t++){
        client.print(a & 1);
        a >>= 1;
        if (t != 7 || j != 3) client.print(", ");
      }
      
    }
    client.print("]");
    if (i != 63) client.println(", ");
  }
  
  for(int i = lenght; i < 64; i++){ // Если размер меньше размера полотна, то остаток заполняем нулями
    client.print("[");
    for(int j = 0; j < 32; j++){
      client.print(0);
      if (j != 31) client.print(", ");
    }
    client.print("]");
    if (i != 63) client.println(", ");
  }
  client.println("]");
  
  client.print(addBtext5);



  client.print("<br><br>Добавленные символы: ");

  File root = SD.open("/alpfav/");
  if (!root) {
    return;
  }
  File file2 = root.openNextFile();
  while (file2) {
      client.print('\'');
      String str = file2.name();
      char s1 = ((str.charAt(2) - 'A') << 4) +  str.charAt(3) - 'A';
      char s2 = ((str.charAt(4) - 'A') << 4) +  str.charAt(5) - 'A';
      if(s1) client.print(s1);
      client.print(s2);
      client.print("\',  ");
      
      file2 = root.openNextFile();
  }
  file2.close();
  root.close();

  client.print("</html>");
  file.close();

}

void add_b_site(WiFiClient &client){
  client.print(addBtext1);
  client.print(addBtext2);
  client.print(addBtext3);
  client.print(addBtext4);
  client.print(addBtext5);
  client.print("<br><br>Добавленные символы: ");
  
  File root = SD.open("/alpfav/");
  if (!root) {
    return;
  }
  File file2 = root.openNextFile();
  while (file2) {
      client.print('\'');
      String str = file2.name();
      char s1 = ((str.charAt(2) - 'A') << 4) +  str.charAt(3) - 'A';
      char s2 = ((str.charAt(4) - 'A') << 4) +  str.charAt(5) - 'A';
      if(s1) client.print(s1);
      client.print(s2);
      client.print("\',  ");
      
      file2 = root.openNextFile();
  }
  file2.close();
  root.close();
  
  client.print("</html>");
}
