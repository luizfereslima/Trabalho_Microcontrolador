#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

/*
 * Definição de variáveis e objetos:
 * - lcd: objeto do LCD com endereço I2C 0x20, 16 colunas e 2 linhas
 * - numLinhasTeclado e numColunasTeclado: dimensões do teclado matricial
 * - senhaCorreta: senha inicial correta
 * - entradaUsuario: vetor para armazenar a senha digitada pelo usuário
 * - contadorEntradas: contador para rastrear o número de teclas pressionadas
 * - pinosLinhasTeclado e pinosColunasTeclado: mapeamento dos pinos do teclado
 * - senhaAdm: senha de administrador
 * - teclasTeclado: mapeamento das teclas do teclado
 * - servoMotor: objeto servo motor
 * - teclado: objeto teclado com o mapeamento das teclas e pinos definidos
 */
LiquidCrystal_I2C lcd(0x20, 16, 2);
const byte numLinhasTeclado = 4;
const byte numColunasTeclado = 4;
char senhaCorreta[5] = "1234";
char entradaUsuario[5];
byte contadorEntradas = 0;

byte pinosLinhasTeclado[numLinhasTeclado] = {6, 7, 8, 9};
byte pinosColunasTeclado[numColunasTeclado] = {10, 11, 12, 13};
char senhaAdm[] = "0800";

char teclasTeclado[numLinhasTeclado][numColunasTeclado] = { 
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Servo servoMotor;
Keypad teclado = Keypad(makeKeymap(teclasTeclado), pinosLinhasTeclado, pinosColunasTeclado, numLinhasTeclado, numColunasTeclado);

void setup() {
    /*
     * Função setup:
     * - Inicializa o LCD e liga o backlight
     * - Configura a tela inicial do LCD
     * - Conecta o servo motor ao pino A0 e define os pulsos mínimos e máximos
     * - Define a posição inicial do servo motor para 0 graus
     */
    lcd.init();
    lcd.backlight();
    inicial();
    servoMotor.attach(A0, 544, 2400);
    servoMotor.write(0);
}

void loop() {
    /*
     * Função loop:
     * - Lê as teclas pressionadas no teclado
     * - Armazena as teclas pressionadas no vetor entradaUsuario
     * - Exibe as teclas pressionadas no LCD
     * - Verifica se 4 teclas foram pressionadas e compara a senha digitada com as senhas corretas
     * - Toma ações apropriadas (abrir a porta, entrar no modo admin, exibir erro)
     * - Limpa a senha digitada e retorna para a tela inicial do LCD
     */
    char teclaDigitada = teclado.getKey();
    
    // Verifica se uma tecla foi pressionada e se o contador de entradas é menor que 4
    if ((teclaDigitada != 0) && (contadorEntradas < 4)) {
        entradaUsuario[contadorEntradas] = teclaDigitada;

        if (contadorEntradas == 0) {
            /*
            Verifica se a tecla pressionada é a primeira digitada. 
            A variável contadorEntradas é usada para contar quantas teclas foram pressionadas até o momento.
            */

            lcd.setCursor((contadorEntradas + 6), 1);
            lcd.print(entradaUsuario[contadorEntradas]);
            /*
            Se for a primeira tecla (contadorEntradas == 0), o cursor do LCD é posicionado na coluna 6 da segunda linha ((contadorEntradas + 6), 1). 
            O valor 6 parece ser uma posição de base escolhida para exibir a senha. 
            Em seguida, a tecla pressionada (entradaUsuario[contadorEntradas]) é exibida no LCD.
            */

        } else {
            lcd.setCursor((contadorEntradas + 5), 1);
            lcd.print('*');
            lcd.setCursor((contadorEntradas + 6), 1);
            lcd.print(entradaUsuario[contadorEntradas]);
            /*
            Se não for a primeira tecla (contadorEntradas > 0), o código faz o seguinte:

            Posiciona o cursor uma coluna à esquerda da posição atual (contadorEntradas + 5, 1) e exibe um asterisco ('*'). 
            Isso é feito para mascarar a tecla anterior, transformando-a em um asterisco.
            Move o cursor para a próxima posição (contadorEntradas + 6, 1) e exibe a tecla pressionada (entradaUsuario[contadorEntradas]).
            */
        }
        contadorEntradas++;

        // Verifica se foram digitadas 4 teclas
        if (contadorEntradas == 4) {
            entradaUsuario[4] = '\0'; // Adiciona o caractere de fim de string
            
            // Verifica se a senha digitada é a senha de administrador
            if (strcmp(entradaUsuario, senhaAdm) == 0) {
                lcd.setCursor(0, 1);
                lcd.print("Modo admin");
                delay(2000);
                alterarSenha();
            
            // Verifica se a senha digitada é a senha correta
            } else if (strcmp(entradaUsuario, senhaCorreta) == 0) {
                lcd.setCursor(0, 1);
                lcd.print("Porta Liberada!");
                servoMotor.write(90); // Abre a porta (servo a 90 graus)
                delay(3000);
                servoMotor.write(0); // Fecha a porta após 3 segundos
            
            // Caso a senha esteja incorreta
            } else {
                lcd.setCursor(0, 1);
                lcd.print("Senha Incorreta!");
                delay(3000);
            }
            limparSenha();
            inicial();
        }
    }
}

void inicial() {
    /*
     * Função auxiliar para configurar a tela inicial do LCD:
     * - Limpa o LCD
     * - Exibe uma mensagem de boas-vindas
     * - Exibe um prompt para digitar a senha
     */
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Sala CPD");
    lcd.setCursor(0, 1);
    lcd.print("Senha: ");
}

void limparSenha() {
    /*
     * Função para limpar a senha digitada:
     * - Limpa o vetor entradaUsuario
     * - Reseta o contador de entradas
     */
    for (int i = 0; i < 5; i++) {
        entradaUsuario[i] = '\0';
    }
    contadorEntradas = 0;
}

void alterarSenha() {
    /*
     * Função para alterar a senha:
     * - Limpa o LCD e exibe uma mensagem para digitar a nova senha
     * - Lê a nova senha digitada
     * - Armazena a nova senha em senhaCorreta
     * - Exibe uma mensagem de confirmação
     */
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nova Senha: ");
    delay(1000);

    char novaSenhaUsuario[5];
    byte contadorNovaSenha = 0;

    while (contadorNovaSenha < 4) { //Inicia um loop que continuará até que 4 teclas sejam pressionadas.
        char teclaNovaSenha = teclado.getKey(); //Lê a tecla pressionada no teclado e armazena em teclaNovaSenha.
        
        if (teclaNovaSenha) { //Verifica se uma tecla foi pressionada (se teclaNovaSenha não é nula).
            novaSenhaUsuario[contadorNovaSenha] = teclaNovaSenha; //Armazena a tecla pressionada na posição atual do array novaSenhaUsuario.
            lcd.setCursor((contadorNovaSenha + 6), 1);
            /*
            Posiciona o cursor na coluna contadorNovaSenha + 6 da segunda linha do LCD.
            Esse cálculo (contadorNovaSenha + 6) é usado para deixar um espaço após a mensagem inicial "Nova Senha: ". 
            */

            lcd.print(teclaNovaSenha); //Exibe a tecla pressionada no LCD.
            contadorNovaSenha++; //Incrementa o contador, rastreando o número de teclas pressionadas.
        }
    }

    novaSenhaUsuario[4] = '\0';
    /*
    Adiciona o caractere nulo ('\0') no final do array novaSenhaUsuario para marcar o fim da string. 
    Isso garante que a string seja manipulada corretamente por funções que trabalham com strings.
    */

    strcpy(senhaCorreta, novaSenhaUsuario); 
    /*
    Usei a função strcpy para copiar a nova senha armazenada em novaSenhaUsuario para a variável senhaCorreta. 
    strcpy (stringcopy) é uma função da biblioteca padrão do C que copia o conteúdo de uma string de origem para uma string de destino.
    */

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Senha Alterada!");
    delay(2000);
}