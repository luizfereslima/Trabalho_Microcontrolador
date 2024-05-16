#include <LiquidCrystal_I2C.h> 
#include <Keypad.h> 
#include <Servo.h> 

LiquidCrystal_I2C telaLCD(0x20,16,2); // Parametros LCD I2C

const byte linhaTeclado = 4; // Definicao linhas teclado
const byte colunaTeclado = 4; // Definicao colunas teclado
char senhaDigitada[5]; // Vetor base para senha
byte contadorTeclado = 0;

byte pinosLinhaTeclado[linhaTeclado] = {6, 7, 8, 9}; // Definicao dos pinos das linhas
byte pinosColunaTeclado[colunaTeclado] = {10, 11, 12, 13}; // Definicao dos pinos das colunas
char senha[] = "1234";

char teclasTeclado[linhaTeclado][colunaTeclado] = { // Criacao matriz (teclas)
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Servo servoMotor; // Definicao do servo motor
Keypad teclado = Keypad(makeKeymap(teclasTeclado), pinosLinhaTeclado, pinosColunaTeclado, linhaTeclado, colunaTeclado );

    /*  
    Criação do objeto 'teclado' do tipo 'Keypad'
    Este objeto será usado para ler as teclas pressionadas no teclado matricial

    makeKeymap(teclasTeclado): Cria um mapeamento das teclas do teclado matricial
   - teclasTeclado: Matriz contendo as teclas organizadas em linhas e colunas

    pinosLinhaTeclado: Array com os pinos conectados às linhas do teclado matricial
    pinosColunaTeclado: Array com os pinos conectados às colunas do teclado matricial
    linhaTeclado: Número de linhas do teclado matricial
    colunaTeclado: Número de colunas do teclado matricial 
    */

void setup() {
    telaLCD.init(); // Inicializa o LCD
    telaLCD.backlight(); // Liga o backlight do LCD
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
    if ((teclaDigitada != 0) && (contadorTeclado <= 5)) { // Verifica se uma tecla foi pressionada e se o contador não ultrapassou 5
        senhaDigitada[contadorTeclado] = teclaDigitada; // Armazena a tecla pressionada na senha digitada
        // Verifica se é a primeira tecla digitada
        if (contadorTeclado == 0) {
            // Se for a primeira tecla, define a posição do cursor no LCD e exibe a tecla pressionada
            telaLCD.setCursor((contadorTeclado + 6), 1);

                /*
                Define a posição do cursor no LCD para exibir a tecla pressionada 
                - (contadorTeclado + 6): Calcula a coluna onde o caractere será exibido no LCD
                - contadorTeclado: Posição atual da tecla digitada
                - 6: Adiciona um deslocamento para a direita para a exibição da senha
                - 1: Define a linha onde o caractere será exibido (linha inferior do LCD)
                */

            telaLCD.print(senhaDigitada[contadorTeclado]); // Exibe a tecla pressionada
        } else {
            // Se não for a primeira tecla, exibe um asterisco no lugar da tecla pressionada e move o cursor para a próxima posição
            telaLCD.setCursor((contadorTeclado + 5), 1);
            
                /*
                Define a posição do cursor no LCD para exibir um asterisco no lugar da tecla pressionada
                - (contadorTeclado + 5): Calcula a coluna onde o asterisco será exibido no LCD
                - contadorTeclado: Posição atual da tecla digitada
                - 5: Adiciona um deslocamento para a direita para a exibição do asterisco
                - 1: Define a linha onde o asterisco será exibido (linha inferior do LCD)
                */

            telaLCD.print('*'); // Exibe um asterisco no lugar da tecla pressionada
            telaLCD.setCursor((contadorTeclado + 6), 1); 
                
                /*
                Define a posição do cursor no LCD para exibir a tecla pressionada 
                - (contadorTeclado + 6): Calcula a coluna onde o caractere será exibido no LCD
                - contadorTeclado: Posição atual da tecla digitada
                - 6: Adiciona um deslocamento para a direita para a exibição da senha
                - 1: Define a linha onde o caractere será exibido (linha inferior do LCD)
                */

            telaLCD.print(senhaDigitada[contadorTeclado]); // Exibe a tecla pressionada
        }
        contadorTeclado++; // Incrementa o contador de teclas digitadas
        if ((senhaDigitada[0] == senha[0]) && (senhaDigitada[1] == senha[1]) && (senhaDigitada[2] == senha[2]) && (senhaDigitada[3] == senha[3]) && (senhaDigitada[4] == senha[4])) { 
            
            /*
            Verifica se a senha digitada é igual à senha definida
            - senhaDigitada[i]: representa a i-ésima tecla digitada pelo usuário
            - senha[i]: representa o i-ésimo dígito da senha definida
            - (senhaDigitada[0] == senha[0]): verifica se o primeiro dígito da senha digitada é igual ao primeiro dígito da senha definida
            - (senhaDigitada[1] == senha[1]): verifica se o segundo dígito da senha digitada é igual ao segundo dígito da senha definida
            - (senhaDigitada[2] == senha[2]): verifica se o terceiro dígito da senha digitada é igual ao terceiro dígito da senha definida
            - (senhaDigitada[3] == senha[3]): verifica se o quarto dígito da senha digitada é igual ao quarto dígito da senha definida
            - (senhaDigitada[4] == senha[4]): verifica se o quinto dígito da senha digitada é igual ao quinto dígito da senha definida
            */

            telaLCD.setCursor(0, 1); // Define a posição do cursor no LCD
            telaLCD.print("Porta Liberada!"); // Exibe mensagem de porta liberada
            servoMotor.write(90); // Move o servo para a posição de porta aberta (90 graus)
            for (int posicaoDigitada = contadorTeclado; posicaoDigitada >= 0; posicaoDigitada--) { // Limpa o vetor senhaDigitada
                senhaDigitada[posicaoDigitada] = {};

                    /*
                    Limpa o vetor senhaDigitada
                    - for (int posicaoDigitada = contadorTeclado; posicaoDigitada >= 0; posicaoDigitada--): loop que percorre o vetor senhaDigitada a partir da última posição digitada até a primeira
                    - posicaoDigitada: variável de controle do loop, indica a posição atual no vetor senhaDigitada
                    - contadorTeclado: quantidade de dígitos digitados na senha
                    - posicaoDigitada >= 0: condição de parada do loop, garante que todas as posições do vetor serão percorridas
                    - posicaoDigitada--: decrementa a posição atual para acessar a próxima posição do vetor
                    - senhaDigitada[posicaoDigitada] = {}: atribui um valor vazio à posição atual do vetor senhaDigitada, limpando-a
                    */

            }
            delay(3000); // Aguarda 3 segundos
            contadorTeclado = 0; // Reseta o contador de teclas digitadas
            inicial(); // Retorna para a tela inicial do LCD
        } else if (contadorTeclado == 5) { // Se o usuário digitou a senha incorretamente
            for (int posicaoDigitada = contadorTeclado; posicaoDigitada >= 0; posicaoDigitada--) { // Limpa o vetor senhaDigitada
                senhaDigitada[posicaoDigitada] = {};
            }
            telaLCD.setCursor(0, 1); // Define a posição do cursor no LCD
            telaLCD.print("Senha Incorreta!"); // Exibe mensagem de senha incorreta
            delay(3000);
            contadorTeclado = 0; // Reseta o contador de teclas digitadas
            inicial(); // Retorna para a tela inicial do LCD
        }
    }
}

void inicial() { // Func auxiliar para configurar a tela inicial do LCD
    telaLCD.clear(); // Limpa o LCD
    telaLCD.setCursor(0, 0); // Define a posição do cursor no LCD
    telaLCD.print("    Sala CPD "); // Exibe mensagem de boas-vindas
    telaLCD.setCursor(0, 1); // Define a posição do cursor no LCD
    telaLCD.print("Senha: "); // Exibe mensagem para digitar a senha
    servoMotor.write(0); // Fecha a porta (0 graus)
}
