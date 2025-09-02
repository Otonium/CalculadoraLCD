#include <Keypad.h>             // facilita teclados 
#include <LiquidCrystal_I2C.h>     

LiquidCrystal_I2C lcd(0x27, 20, 4); // 20 colunas, 4 linhas

// Variáveis para guardar os números que o usuário digita:
long primeiroN = 0;       // Guarda o primeiro número antes do operador
long segundoN = 0;      // Guarda o segundo número depois do operador
double total = 0;     // Guarda o resultado da operação (pode ter parte decimal)

// Variável que armazena a tecla pressionada:
char teclaPress;

// Dimensões do teclado:
const byte LIN = 4;  
const byte COL = 4;

// Mapa das teclas: cada caractere corresponde a uma tecla do teclado 4×4
char keys[LIN][COL] = {
  {'1','2','3','+'},   
  {'4','5','6','-'},  
  {'7','8','9','*'},   
  {'C','0','=','/'}    
};

// Define pinos que cada linha do teclado está conectada:
byte rowPins[LIN] = {14, 27, 26, 25};  
// Define pinos que cada coluna do teclado está conectada:
byte colPins[COL] = {33, 32, 18, 19};  

// Cria o objeto que gerencia o teclado, passando o mapa, os pinos e as dimensões:
Keypad _teclado = Keypad(makeKeymap(keys), rowPins, colPins, LIN, COL);

long funcaoSegNum(); //Declara a função que vai ser usada

void setup() {
  lcd.init();                
  lcd.backlight();           
  //Mensagem de boas-vindas:
  lcd.clear();
  lcd.setCursor(0, 0);       // Posiciona o “cursor” no canto superior esquerdo
  lcd.print("Calculadora");  
  lcd.setCursor(0, 1);       // Vai para a segunda linha
  lcd.print("Antonio Carlos");
  delay(4000);               // Mostra a mensagem por 4 segundos
  lcd.clear();               // Apaga a tela para começar limpo
  lcd.setCursor(0, 0);       
  lcd.print("* = Limpar tela");  
  lcd.setCursor(0, 1);       // Vai para a segunda linha
  lcd.print("# = Sinal de Igual");
  lcd.setCursor(0, 2);       
  lcd.print("A = Soma ");  
  delay(2000);               
  lcd.clear();
  lcd.setCursor(0, 0);       
  lcd.print("B = Subtracao");
  lcd.setCursor(0, 1);       
  lcd.print("C = Multiplicacao");  
  lcd.setCursor(0, 2);       
  lcd.print("D = Divisao");
  delay(3000); 
  lcd.clear();
}


void loop() {
  // Lê qual tecla foi pressionada (se nenhuma, teclaPress fica nulo):
  teclaPress = _teclado.getKey();

  // Decide o que fazer com base na tecla lida:
  switch (teclaPress) {

    // Se for um dígito de '0' a '9', o primeiro número esta sendo escrito:
    case '0' ... '9':              
      lcd.setCursor(0, 0); 
      //#### Trecho que cronstroe casas decimais do primeiro numero #######          
      // primeiroN = primeiroN * 10 + valor da tecla
      // Ex.: se digitou '2' depois '3', primeiroN vira 2*10 + 3 = 23
      primeiroN = primeiroN * 10 + (teclaPress - '0');
      lcd.print(primeiroN);            // Mostra o número que está sendo digitado
      break;

    // Se apertou '+', somar:
    case '+':
      // Se já tinha um resultado em "total", usa como ponto de partida
      if (total != 0) {
        primeiroN = total;
      }
      lcd.setCursor(0, 1);         // Vai para a linha de baixo
      lcd.print("+");              // Exibe o símbolo de adição
      segundoN = funcaoSegNum();     // Chama a função que lê o segundo número até '='
      total = primeiroN + segundoN;      // Faz a conta
      lcd.clear();
      lcd.setCursor(0, 0);         // Posição para exibir o resultado
      lcd.print(total);            // Mostra o resultado
      primeiroN = 0;                   // Reseta para a próxima operação
      segundoN = 0;
                        
      break;

    // Mesma lógica para subtração, multiplicação e divisão:
    case '-':
      if (total != 0) {
        primeiroN = total;
      }
      lcd.setCursor(0, 1);
      lcd.print("-");
      segundoN = funcaoSegNum();
      total = primeiroN - segundoN;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(total);
      primeiroN = 0; segundoN = 0;
      break;

    case '*':
      if (total != 0) {
        primeiroN = total;
      }
      lcd.setCursor(0, 1);
      lcd.print("*");
      segundoN = funcaoSegNum();
      total = primeiroN * segundoN;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(total);
      primeiroN = 0; segundoN = 0;
      break;

    case '/':
      if (total != 0) {
        primeiroN = total;
      }
      lcd.setCursor(0, 1);
      lcd.print("/");
      segundoN = funcaoSegNum();
      lcd.clear();
      lcd.setCursor(0, 0);
      // Se tentar dividir por zero, mostra “Invalid”:
      if (segundoN == 0) {
        lcd.print("Impossivel divir por zero");
      } else {
        total = (double)primeiroN / (double)segundoN;
        lcd.print(total);
      }
      primeiroN = 0; segundoN = 0;
      break;

    // 'C' limpa tudo:
    case 'C':
      total = 0;       // Zera o resultado
      primeiroN = 0;       // Zera o primeiro número
      segundoN = 0;      // Zera o segundo número
      lcd.clear();     // Limpa a tela
      break;

    // Se apertar '=', poderia repetir a lógica de segundo número,
    // mas aqui o '=' só é tratado dentro de funcaoSegNum().
  }
}



// --------------------------------------------------
//  Função que lê o “segundo número” até encontrar '='
// --------------------------------------------------
long funcaoSegNum() {
  segundoN = 0;              // Começa do zero sempre
  while (true) {           // Fica rodando até encontrar '='
    teclaPress = _teclado.getKey();  // Lê tecla
    // Se for dígito, adiciona ao segundo número:
    if (teclaPress >= '0' && teclaPress <= '9') {
      segundoN = segundoN * 10 + (teclaPress - '0');
      lcd.setCursor(0, 2);  // Vai para a 3ª linha (índice 2)
      lcd.print(segundoN);    // Mostra o número à medida que digita
    }
    // Se for '=', sai do loop:
    if (teclaPress == '=') 
    break;
  }
  return segundoN;           // Devolve o valor digitado
}
