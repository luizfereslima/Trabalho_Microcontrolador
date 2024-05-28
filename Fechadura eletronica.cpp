#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x20, 16, 2); // Parametros LCD I2C

const byte numLinhasTeclado = 4; // Definicao linhas teclado
const byte numColunasTeclado = 4; // Definicao colunas teclado
char senhaUsuario[5]; // Vetor base para senha
byte contadorDeTeclas = 0;

byte pinosLinhasTeclado[numLinhasTeclado] = {6, 7, 8, 9}; // Definicao dos pinos das linhas
byte pinosColunasTeclado[numColunasTeclado] = {10, 11, 12, 13}; // Definicao dos pinos das colunas
char senhaCorreta[] = "1234";

char teclasTeclado[numLinhasTeclado][numColunasTeclado] = { // Criacao matriz (teclas)
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Servo servoMotor; // Definicao do servo motor
Keypad teclado = Keypad(makeKeymap(teclasTeclado), pinosLinhasTeclado, pinosColunasTeclado, numLinhasTeclado, numColunasTeclado);

/*  
Criação do objeto 'teclado' do tipo 'Keypad'
Este objeto será usado para ler as teclas pressionadas no teclado matricial

makeKeymap(teclasTeclado): Cria um mapeamento das teclas do teclado matricial
- teclasTeclado: Matriz contendo as teclas organizadas em linhas e colunas

pinosLinhasTeclado: Array com os pinos conectados às linhas do teclado matricial
pinosColunasTeclado: Array com os pinos conectados às colunas do teclado matricial
numLinhasTeclado: Número de linhas do teclado matricial
numColunasTeclado: Número de colunas do teclado matricial 
*/

void setup() {
    lcd.init(); // Inicializa o LCD
    lcd.backlight(); // Liga o backlight do LCD
    inicial(); // Configuração inicial do LCD
    servoMotor.attach(A0, 544, 2400);

    /*
    Attach do servo motor ao pino A0 do Arduino
    - A0: Pino ao qual o servo motor está conectado
    - 544: Valor mínimo do pulso de controle para a posição 0 grau
    - 2400: Valor máximo do pulso de controle para a posição 180 graus
    */

    servoMotor.write(0); // Define a posição inicial do servo (0 graus)
}

void loop() {
    char teclaDigitada = teclado.getKey(); // Leitura do teclado
    if ((teclaDigitada != 0) && (contadorDeTeclas <= 5)) { // Verifica se uma tecla foi pressionada e se o contador não ultrapassou 5
        senhaUsuario[contadorDeTeclas] = teclaDigitada; // Armazena a tecla pressionada na senha digitada
        // Verifica se é a primeira tecla digitada
        if (contadorDeTeclas == 0) {
            // Se for a primeira tecla, define a posição do cursor no LCD e exibe a tecla pressionada
            lcd.setCursor((contadorDeTeclas + 6), 1);

            /*
            Define a posição do cursor no LCD para exibir a tecla pressionada 
            - (contadorDeTeclas + 6): Calcula a coluna onde o caractere será exibido no LCD
            - contadorDeTeclas: Posição atual da tecla digitada
            - 6: Adiciona um deslocamento para a direita para a exibição da senha
            - 1: Define a linha onde o caractere será exibido (linha inferior do LCD)
            */

            lcd.print(senhaUsuario[contadorDeTeclas]); // Exibe a tecla pressionada
        } else {
            // Se não for a primeira tecla, exibe um asterisco no lugar da tecla pressionada e move o cursor para a próxima posição
            lcd.setCursor((contadorDeTeclas + 5), 1);

            /*
            Define a posição do cursor no LCD para exibir um asterisco no lugar da tecla pressionada
            - (contadorDeTeclas + 5): Calcula a coluna onde o asterisco será exibido no LCD
            - contadorDeTeclas: Posição atual da tecla digitada
            - 5: Adiciona um deslocamento para a direita para a exibição do asterisco
            - 1: Define a linha onde o asterisco será exibido (linha inferior do LCD)
            */

            lcd.print('*'); // Exibe um asterisco no lugar da tecla pressionada
            lcd.setCursor((contadorDeTeclas + 6), 1);

            /*
            Define a posição do cursor no LCD para exibir a tecla pressionada 
            - (contadorDeTeclas + 6): Calcula a coluna onde o caractere será exibido no LCD
            - contadorDeTeclas: Posição atual da tecla digitada
            - 6: Adiciona um deslocamento para a direita para a exibição da senha
            - 1: Define a linha onde o caractere será exibido (linha inferior do LCD)
            */

            lcd.print(senhaUsuario[contadorDeTeclas]); // Exibe a tecla pressionada
        }
        contadorDeTeclas++; // Incrementa o contador de teclas digitadas
        if ((senhaUsuario[0] == senhaCorreta[0]) && (senhaUsuario[1] == senhaCorreta[1]) && (senhaUsuario[2] == senhaCorreta[2]) && (senhaUsuario[3] == senhaCorreta[3]) && (senhaUsuario[4] == senhaCorreta[4])) {

            /*
            Verifica se a senha digitada é igual à senha definida
            - senhaUsuario[i]: representa a i-ésima tecla digitada pelo usuário
            - senhaCorreta[i]: representa o i-ésimo dígito da senha definida
            - (senhaUsuario[0] == senhaCorreta[0]): verifica se o primeiro dígito da senha digitada é igual ao primeiro dígito da senha definida
            - (senhaUsuario[1] == senhaCorreta[1]): verifica se o segundo dígito da senha digitada é igual ao segundo dígito da senha definida
            - (senhaUsuario[2] == senhaCorreta[2]): verifica se o terceiro dígito da senha digitada é igual ao terceiro dígito da senha definida
            - (senhaUsuario[3] == senhaCorreta[3]): verifica se o quarto dígito da senha digitada é igual ao quarto dígito da senha definida
            - (senhaUsuario[4] == senhaCorreta[4]): verifica se o quinto dígito da senha digitada é igual ao quinto dígito da senha definida
            */

            lcd.setCursor(0, 1); // Define a posição do cursor no LCD
            lcd.print("Porta Liberada!"); // Exibe mensagem de porta liberada
            servoMotor.write(90); // Move o servo para a posição de porta aberta (90 graus)
            for (int posicaoDigitada = contadorDeTeclas; posicaoDigitada >= 0; posicaoDigitada--) { // Limpa o vetor senhaUsuario
                senhaUsuario[posicaoDigitada] = {};

                /*
                Limpa o vetor senhaUsuario
                - for (int posicaoDigitada = contadorDeTeclas; posicaoDigitada >= 0; posicaoDigitada--): loop que percorre o vetor senhaUsuario a partir da última posição digitada até a primeira
                - posicaoDigitada: variável de controle do loop, indica a posição atual no vetor senhaUsuario
                - contadorDeTeclas: quantidade de dígitos digitados na senha
                - posicaoDigitada >= 0: condição de parada do loop, garante que todas as posições do vetor serão percorridas
                - posicaoDigitada--: decrementa a posição atual para acessar a próxima posição do vetor
                - senhaUsuario[posicaoDigitada] = {}: atribui um valor vazio à posição atual do vetor senhaUsuario, limpando-a
                */

            }
            delay(3000); // Aguarda 3 segundos
            contadorDeTeclas = 0; // Reseta o contador de teclas digitadas
            inicial(); // Retorna para a tela inicial do LCD
        } else if (contadorDeTeclas == 5) { // Se o usuário digitou a senha incorretamente
            for (int posicaoDigitada = contadorDeTeclas; posicaoDigitada >= 0; posicaoDigitada--) { // Limpa o vetor senhaUsuario
                senhaUsuario[posicaoDigitada] = {};
            }
            lcd.setCursor(0, 1); // Define a posição do cursor no LCD
            lcd.print("Senha Incorreta!"); // Exibe mensagem de senha incorreta
            delay(3000);
            contadorDeTeclas = 0; // Reseta o contador de teclas digitadas
            inicial(); // Retorna para a tela inicial do LCD
        }
    }
}

void inicial() { // Func auxiliar para configurar a tela inicial do LCD
    lcd.clear(); // Limpa o LCD
    lcd.setCursor(0, 0); // Define a posição do cursor no LCD
    lcd.print("    Sala CPD "); // Exibe mensagem de boas-vindas
    lcd.setCursor(0, 1); // Define a posição do cursor no LCD
    lcd.print("Senha: "); // Exibe mensagem para digitar a senha
    servoMotor.write(0); // Fecha a porta (0 graus)
}
