#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);
int bt_L = 8;
int bt_R = 7;
int valorBt_L;
int valorBt_R;
int posicaoAviao;
int posicaoInimigo;
int posicaoTiroX;
int posicaoTiroY;
int podeAtirar = 1;
int loopDelay = 100;
int loopsWait = 10;
int loopCount = 0;
int jogoIniciado = 0;
<<<<<<< HEAD
=======
unsigned long tempo;
>>>>>>> master

byte preto[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

byte aviao[8] = {
  0b00000,
  0b00010,
  0b10010,
  0b11111,
  0b10010,
  0b00010,
  0b00000,
  0b00000
};

byte inimigo[8] = {
  0b00000,
  0b01110,
  0b00100,
  0b01101,
  0b11111,
  0b01101,
  0b00100,
  0b01110
};

void limparAviao();
void limparInimigo();
void limparCeu();
void trajetoTiro();
void atirar();
void start();
void exibirTiro();
void exibirAviao();
void exibirInimigo();
void gameOver();
void aumentarDificuldade();
void exibirExplosao();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.createChar(0, aviao);
  lcd.createChar(1, inimigo);
  lcd.createChar(2, preto);
  lcd.begin(16,2);
  pinMode(bt_L, INPUT);
  pinMode(bt_R, INPUT);

}

void loop() {
 
  if(jogoIniciado == 0){
    
    while(1){
      //lê botoes
      valorBt_L = digitalRead(bt_L);
      valorBt_R = digitalRead(bt_R);
      lcd.home();
<<<<<<< HEAD
      lcd.print("  Press L or R  ");
      lcd.setCursor(0,1);
      lcd.print("   to start!!   ");
      if(valorBt_L == HIGH || valorBt_R == HIGH){
=======
      lcd.print(" Press L and R  ");
      lcd.setCursor(0,1);
      lcd.print("   to start!!   ");
      if(valorBt_L == HIGH && valorBt_R == HIGH){
>>>>>>> master
        break;
      }
      delay(1000);
      lcd.clear();
      delay(300);
    }
    start();
  } else{
    //INICIO DA EXECUÇÃO
    
    //posicionar inimigo  
    if(loopCount < loopsWait){
      loopCount++;
    } else{
      if(posicaoAviao == 0){
        posicaoInimigo = 0;
      } else{
        posicaoInimigo = 1;
      }
      exibirInimigo();
      
      //Inimigo Atira
      if(posicaoInimigo == posicaoAviao){
        if(podeAtirar){
          atirar();
          aumentarDificuldade();
        }
      }
      loopCount = 0;
    }
    
    //lê botoes
<<<<<<< HEAD

=======
    tempo = millis();
>>>>>>> master
    valorBt_L = digitalRead(bt_L);
    valorBt_R = digitalRead(bt_R);
    if(valorBt_L == HIGH && valorBt_R == LOW){
      posicaoAviao = 0;
    } else if(valorBt_L == LOW && valorBt_R == HIGH){
      posicaoAviao = 1;
    }
    
    exibirAviao();
  
    trajetoTiro();
    
    exibirInimigo();
    
    delay(loopDelay);
    //FIM DA EXECUÇÃO
  }
}


void atirar(){
  podeAtirar = 0;
  posicaoTiroX = posicaoInimigo;
  posicaoTiroY = 15;
  exibirTiro();
}

void trajetoTiro(){
  
  if(posicaoTiroX == posicaoAviao && posicaoTiroY == 0){
    //morreu
    gameOver();
    jogoIniciado = 0;
  } else if(posicaoTiroY < 0){
    posicaoTiroY -1;
    podeAtirar = 1;
  } else{
    posicaoTiroY--;
    podeAtirar = 0;
    exibirTiro();
  }
}


void limparCeu(){
  lcd.setCursor(1,0);
  lcd.print("              ");
  lcd.setCursor(1,1);
  lcd.print("              ");
}

void limparAviao(){
  lcd.setCursor(0,0);
  lcd.print(" ");
  lcd.setCursor(0,1);
  lcd.print(" ");
}

void limparInimigo(){
  lcd.setCursor(15,0);
  lcd.print(" ");
  lcd.setCursor(15,1);
  lcd.print(" ");
}

void start(){
  lcd.clear();
  lcd.home();
  lcd.print("     START");
  delay(2000);

  
  lcd.clear();
   //carrega aviao
  lcd.home();
  lcd.write(byte(0));
  posicaoAviao = 0;
  
  //carrega inimigo
  lcd.setCursor(15,1);
  lcd.write(byte(3));
  posicaoInimigo = 1;

  //carregar tiro
  posicaoTiroX = 0;
  posicaoTiroY = -1;
  podeAtirar = 1;
  jogoIniciado = 1;

  //dificuldade inicial
  loopDelay = 100;
  loopsWait = 10;

  
}

void exibirTiro(){
  limparCeu();
  lcd.setCursor(posicaoTiroY,posicaoTiroX);
  lcd.print("*");
}

void exibirAviao(){
  limparAviao();
  lcd.setCursor(0,posicaoAviao);
  lcd.write(byte(0));
}

void exibirInimigo(){
  limparInimigo();
  lcd.setCursor(15,posicaoInimigo);
  lcd.write(byte(1));
}

void exibirExplosao(){
  int i, j;
  for(i = 0; i < 3;i++){
    
    for(j = 0; j < 16; j++){
      lcd.setCursor(j,0);
      lcd.write(byte(2));
      lcd.setCursor(j,1);
      lcd.write(byte(2));
    }
    delay(100);
    lcd.clear();
    delay(100);
  }
}

void gameOver(){
  exibirExplosao();
  lcd.print("      Game");
  lcd.setCursor(0,1);
  lcd.print("      Over");
  delay(2000);
}

void aumentarDificuldade(){
  if(loopDelay > 20){
    loopDelay = loopDelay - 5;
    loopsWait--;
  }
}

