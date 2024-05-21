# Documentação de Projeto

## Sistema de Controle de Acesso com LCD, Teclado e Servo Motor

---

### 1. **Descrição do Projeto**
Este projeto implementa um sistema de controle de acesso utilizando um display LCD, um teclado matricial e um servo motor. O sistema permite que um usuário digite uma senha usando o teclado, e se a senha estiver correta, o servo motor se move para abrir uma porta. Se a senha estiver incorreta, uma mensagem de erro é exibida no LCD.

### 2. **Componentes Utilizados**
- **Arduino Uno**
- **Display LCD I2C 16x2** (LiquidCrystal_I2C)
- **Teclado Matricial 4x4** (Keypad)
- **Servo Motor** (Servo)
- **Fios Jumper e Protoboard**

### 3. **Bibliotecas Utilizadas**
- **LiquidCrystal_I2C**: Para controle do display LCD via interface I2C.
- **Keypad**: Para leitura do teclado matricial.
- **Servo**: Para controle do servo motor.

### 4. **Conexões**
- **Display LCD I2C**:
  - Conectado ao barramento I2C (SDA, SCL) do Arduino.
- **Teclado Matricial**:
  - Linhas conectadas aos pinos digitais 6, 7, 8 e 9 do Arduino.
  - Colunas conectadas aos pinos digitais 10, 11, 12 e 13 do Arduino.
- **Servo Motor**:
  - Sinal conectado ao pino analógico A0 do Arduino.
  - Alimentação conectada aos pinos 5V e GND do Arduino.

### 5. **Código-Fonte**

#### 5.1. **Inclusão de Bibliotecas e Definição de Variáveis**
```cpp
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// Inicialização do display LCD
LiquidCrystal_I2C telaLCD(0x20, 16, 2);

// Configuração do teclado matricial
const byte linhaTeclado = 4;
const byte colunaTeclado = 4;
char senhaDigitada[5];
byte contadorTeclado = 0;

byte pinosLinhaTeclado[linhaTeclado] = {6, 7, 8, 9};
byte pinosColunaTeclado[colunaTeclado] = {10, 11, 12, 13};
char senha[] = "1234";

char teclasTeclado[linhaTeclado][colunaTeclado] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Servo servoMotor;
Keypad teclado = Keypad(makeKeymap(teclasTeclado), pinosLinhaTeclado, pinosColunaTeclado, linhaTeclado, colunaTeclado);
```

#### 5.2. **Função Setup**
```cpp
void setup() {
    telaLCD.init();
    telaLCD.backlight();
    inicial();
    servoMotor.attach(A0, 544, 2400);
    servoMotor.write(0);
}
```

#### 5.3. **Função Loop**
```cpp
void loop() {
    char teclaDigitada = teclado.getKey();
    if ((teclaDigitada != 0) && (contadorTeclado <= 5)) {
        senhaDigitada[contadorTeclado] = teclaDigitada;
        if (contadorTeclado == 0) {
            telaLCD.setCursor((contadorTeclado + 6), 1);
            telaLCD.print(senhaDigitada[contadorTeclado]);
        } else {
            telaLCD.setCursor((contadorTeclado + 5), 1);
            telaLCD.print('*');
            telaLCD.setCursor((contadorTeclado + 6), 1);
            telaLCD.print(senhaDigitada[contadorTeclado]);
        }
        contadorTeclado++;
        if (senhaCorreta()) {
            telaLCD.setCursor(0, 1);
            telaLCD.print("Porta Liberada!");
            servoMotor.write(90);
            limparSenhaDigitada();
            delay(3000);
            contadorTeclado = 0;
            inicial();
        } else if (contadorTeclado == 5) {
            limparSenhaDigitada();
            telaLCD.setCursor(0, 1);
            telaLCD.print("Senha Incorreta!");
            delay(3000);
            contadorTeclado = 0;
            inicial();
        }
    }
}
```

#### 5.4. **Função Auxiliar para Tela Inicial**
```cpp
void inicial() {
    telaLCD.clear();
    telaLCD.setCursor(0, 0);
    telaLCD.print("    Sala CPD ");
    telaLCD.setCursor(0, 1);
    telaLCD.print("Senha: ");
    servoMotor.write(0);
}
```

#### 5.5. **Verificação de Senha**
```cpp
bool senhaCorreta() {
    return (senhaDigitada[0] == senha[0] && senhaDigitada[1] == senha[1] &&
            senhaDigitada[2] == senha[2] && senhaDigitada[3] == senha[3] &&
            senhaDigitada[4] == senha[4]);
}
```

#### 5.6. **Limpeza da Senha Digitada**
```cpp
void limparSenhaDigitada() {
    for (int posicaoDigitada = contadorTeclado; posicaoDigitada >= 0; posicaoDigitada--) {
        senhaDigitada[posicaoDigitada] = {};
    }
}
```

### 6. **Funcionamento**
1. **Inicialização**:
   - O LCD é inicializado e a tela de boas-vindas é exibida.
   - O servo motor é configurado para a posição inicial (0 graus).
2. **Entrada da Senha**:
   - O usuário digita a senha usando o teclado matricial.
   - A primeira tecla digitada é exibida no LCD, as demais são substituídas por asteriscos.
3. **Verificação da Senha**:
   - Se a senha estiver correta, o servo motor se move para 90 graus, liberando a porta e uma mensagem de "Porta Liberada!" é exibida no LCD.
   - Se a senha estiver incorreta, uma mensagem de "Senha Incorreta!" é exibida no LCD.
4. **Reinicialização**:
   - Após 3 segundos, o sistema é reinicializado, voltando à tela de boas-vindas.

### 7. **Link do Projeto**
Você pode acessar o projeto no Tinkercad através do seguinte link: [Controle de Acesso](https://www.tinkercad.com/things/5p0LftADbbh-controle-de-acesso)
