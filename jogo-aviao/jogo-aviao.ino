#define MAX_SHOT_INTERVAL 14
#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);
int bt_L = 8;
int bt_R = 7;
int valorBt_L;
int valorBt_R;
int posicaoAviao;
int posicaoInimigo;
int podeAtirar;
int loopDelay;
int loopsWait;
int loopCount;
int jogoIniciado = 0;
char tirosL[16];
char tirosR[16];
char tiroSymbol = '*';
int intervalo;
int intervaloMinimo;

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
void exibirTiros();
void exibirAviao();
void exibirInimigo();
void gameOver();
void aumentarDificuldade();
void exibirExplosao();
void adicionarTiro(char *tiros);
void tirosShiftLeft(char *tiros);

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
      lcd.print("  Press L or R  ");
      lcd.setCursor(0,1);
      lcd.print("   to start!!   ");
      if(valorBt_L == HIGH || valorBt_R == HIGH){
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
    } else if(loopCount == loopsWait){
      loopCount = 0;
      posicaoInimigo = random(1,2) -1;
    }
    exibirInimigo();
    //Inimigo Atira

    
    
    //lê botoes

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
  int i;
  int tiros = random(2,4) -1;
  podeAtirar = 0;
  intervalo = 0;
  if(posicaoInimigo == 0){
    for(i = 0; i < tiros; i++){
      adicionarTiro(tirosL);
      tirosShiftLeft(tirosR);
      exibirTiros();
    }
  } else{
    for(i = 0; i < tiros; i++){
      adicionarTiro(tirosR);
      tirosShiftLeft(tirosL);
      exibirTiros();
    }
  }
}

void adicionarTiro(char *tiros){
  tirosShiftLeft(tiros);
  tiros[14] = tiroSymbol;
}
void tirosShiftLeft(char *tiros){
  int i;
  for(i = 0; i < 14; i++){
    tiros[i] = tiros[i+1];
  }
  tiros[14] = ' ';
}

void trajetoTiro(){
  
  if(tirosL[0] == tiroSymbol && posicaoAviao == 0 || tirosR[0] == tiroSymbol && posicaoAviao  == 1 ){
    //morreu
    gameOver();
    jogoIniciado = 0;
  } else if(intervalo == intervaloMinimo){
    podeAtirar = 1;
    atirar();
    aumentarDificuldade();
  } else{
    intervalo++;
    podeAtirar = 0;
    exibirTiros();
  }
    tirosShiftLeft(tirosL);
    tirosShiftLeft(tirosR);
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
  int i;
  lcd.clear();
  lcd.home();
  lcd.print("     START");
  delay(2000);
  
  jogoIniciado = 1;
  
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
  podeAtirar = 1;
  intervalo = 0;
  intervaloMinimo = MAX_SHOT_INTERVAL;
  
  //resetar tiros
  for(i = 0; i < 15; i++){
    tirosL[i] = ' ';
    tirosR[i] = ' ';
  }
  tirosL[15] = '\0';
  tirosR[15] = '\0';

  //dificuldade inicial
  loopDelay = 150;
  loopsWait = 10;

}

void exibirTiros(){
  
  limparCeu();
  lcd.setCursor(1,0);
  lcd.print(tirosL);
  lcd.setCursor(1,1);
  lcd.print(tirosR);
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
  if(loopDelay > 50){
    loopDelay = loopDelay - 1;
    if(loopsWait > 5){
      loopsWait--;
    }
    if(intervaloMinimo > 2){
      intervaloMinimo--;
    }
  }  else{
     intervaloMinimo = random(1,5);
  }
}

