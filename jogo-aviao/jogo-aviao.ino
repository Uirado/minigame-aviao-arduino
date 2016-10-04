#define MAX_SHOT_INTERVAL 14
#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);
int bt_L = 8;
int bt_R = 7;
int valorBt_L;
int valorBt_R;
int posicaoAviao;
int posicaoInimigo;

int jogoIniciado = 0;
char tirosL[16];// = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','\0'};
char tirosR[16];// = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','\0'};
char tiroSymbol = '*';
char charAviao;
char charInimigo;

int loopDelay = 100;
int loopWait;
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

void trajetoTiro();
void atirar();
void start();
void gameOver();
void aumentarDificuldade();
void exibirExplosao();
void adicionarTiro(char *tiros);
void tirosShiftLeft(char *tiros);
void limparTiros();

void atualizarTela();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.createChar(0, aviao);
  lcd.createChar(1, inimigo);
  lcd.createChar(2, preto);
  lcd.begin(16,2);
  pinMode(bt_L, INPUT);
  pinMode(bt_R, INPUT);
  charAviao = byte(0);
  charInimigo = byte(1);
  Serial.write(byte(1));
  Serial.print(byte(0));
}

void loop() {
 
  if(jogoIniciado == 0){
    
    while(1){
      //lê botoes
      valorBt_L = digitalRead(bt_L);
      valorBt_R = digitalRead(bt_R);
      lcd.home();
      lcd.print("   Hold L or R   ");
      lcd.setCursor(0,1);
      lcd.print("    to start!!   ");
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

    //lê botoes
    valorBt_L = digitalRead(bt_L);
    valorBt_R = digitalRead(bt_R);
    if(valorBt_L == HIGH && valorBt_R == LOW){
      posicaoAviao = 0;
    } else if(valorBt_L == LOW && valorBt_R == HIGH){
      posicaoAviao = 1;
    }
  
    trajetoTiro();
    
    atualizarTela();
    
    delay(loopDelay);
    //FIM DA EXECUÇÃO
  }
}


void atirar(){
  int i;
  if(posicaoInimigo == 0){
      adicionarTiro(tirosL);
      tirosShiftLeft(tirosR);
  } else{
      adicionarTiro(tirosR);
      tirosShiftLeft(tirosL);
  } 
  if(intervaloMinimo == 2){
    intervaloMinimo += intervaloMinimo;
    atirar();
  }
  
  intervalo = 0;
}

void adicionarTiro(char *tiros){
  tirosShiftLeft(tiros);
  tiros[14] = tiroSymbol;
}

void tirosShiftLeft(char *tiros){
  int i;
  for(i = 0; i < 15; i++){
    tiros[i] = tiros[i+1];
  }
  tiros[14] = ' ';
}

void limparTiros(){
  int i;
  for(i = 0; i < 15; i++){
    tirosL[i] = ' ';
    tirosR[i] = ' ';
  }
  
}

void trajetoTiro(){
  
  if(tirosL[0] == tiroSymbol && posicaoAviao == 0 || tirosR[0] == tiroSymbol && posicaoAviao  == 1){
    //morreu
    gameOver();
    jogoIniciado = 0;
  } else if(intervalo == intervaloMinimo){
    if(loopWait == 0){
      loopWait = MAX_SHOT_INTERVAL;
      if(posicaoInimigo == posicaoAviao){
        atirar();
        aumentarDificuldade();
      } else{
        posicaoInimigo = posicaoAviao;
      }
    } else{
      loopWait--;
    }
  } else{
    intervalo++;
    posicaoInimigo = posicaoAviao;
  }
    tirosShiftLeft(tirosL);
    tirosShiftLeft(tirosR);
}


void start(){
  lcd.clear();
  lcd.home();
  lcd.print("     START      ");
  delay(2000);
  
  jogoIniciado = 1;
  
  posicaoAviao = 0;
  posicaoInimigo = 1;
  limparTiros();
  
  intervalo = MAX_SHOT_INTERVAL;
  intervaloMinimo = MAX_SHOT_INTERVAL;
  loopWait = MAX_SHOT_INTERVAL;

  atualizarTela();
}


void atualizarTela(){
  if(posicaoAviao == 0){
    lcd.setCursor(0,0);
    lcd.write(charAviao);
    lcd.print(tirosL + sizeof(int));
    lcd.setCursor(0,1);
    lcd.print(tirosR);
  } else{
    lcd.setCursor(0,1);
    lcd.write(charAviao);
    lcd.print(tirosR + sizeof(int));
    lcd.setCursor(0,0);
    lcd.print(tirosL);
  }
  if(posicaoInimigo == 0){
    lcd.setCursor(15,0);
    lcd.write(charInimigo);
    lcd.setCursor(15,1);
    lcd.print(" ");
  } else{
    lcd.setCursor(15,1);
    lcd.write(charInimigo);
    lcd.setCursor(15,0);
    lcd.print(" ");
  }
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
    if(intervaloMinimo > 2){
      intervaloMinimo--;
    }
}


