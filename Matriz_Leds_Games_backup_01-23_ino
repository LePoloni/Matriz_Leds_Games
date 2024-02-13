/*
Desenvolvida por: Leandro Poloni Dantas
Data: Janeiro/2023

Referências:
Neopixel
https://docs.micropython.org/en/latest/library/neopixel.html
https://docs.micropython.org/en/latest/esp8266/tutorial/neopixel.html
Exemplo do uso de canvas: https://wokwi.com/projects/330751728413573715
Outro exemplo: https://github.com/wokwi/wokwi-features/issues/237
Exemplos da bilbioteca: https://wokwi.com/arduino/libraries/Adafruit_NeoPixel
Tetris online: https://tetris.com/play-tetris
Projeto em Micropython: https://wokwi.com/projects/353230342736706561

Atenção: Evitar cores muito intesas quando usar apenas a fonte do Arduino!
*/
#include <Adafruit_NeoPixel.h>
//As classes são excludentes
//#define Like_Tetris
#ifdef Like_Tetris
  #include "Like_Tetris.h"
#else
  #include "Like_Snake.h"
#endif

#define PIN 6
#define COLUNAS 10
#define LINHAS  15

#define botaoEsq  3
#define botaoDir  4
#define botaoRot  5
#define botaoDes  7
#define botaoSel  A2
#define joyVer    A0
#define joyHor    A1

#define STOP      0
#define ESQUERDA  1
#define DIREITA   2
#define CIMA      3
#define BAIXO     4

int move = STOP;
#ifdef Like_Tetris
  like_tetris tt;
#else
  like_snake snake;
#endif
Adafruit_NeoPixel nP;

void setup() 
{
  pinMode(botaoEsq, INPUT_PULLUP);
  pinMode(botaoDir, INPUT_PULLUP);
  pinMode(botaoRot, INPUT_PULLUP);
  pinMode(botaoRot, INPUT_PULLUP);
  pinMode(botaoDes, INPUT_PULLUP);
  pinMode(botaoSel, INPUT_PULLUP);
  randomSeed(1);
  //randomSeed(analogRead(0));
  
  #ifdef Like_Tetris
    //tt = like_tetris(PIN, COLUNAS, LINHAS, T_LINHA);  //Ideal para simulação no Wokwi
    tt = like_tetris(PIN, COLUNAS, LINHAS, T_LINHA_ZIG_ZAG);
    //Métodos da classe herdada
    tt.begin();
    tt.fill(0x00000F, 0, 150); //Cor, Primeiro, Quantidade
    tt.show();
    delay(1000);
    tt.limpa_matriz(0x0F0F00);
    //Método de teste do pino 13
    tt.pisca();
  #else
    //snake = like_snake(); //Padrão
    snake = like_snake(PIN, COLUNAS, LINHAS, 0xFF0000, 0x00FF00, 0x0000FF, T_LINHA_ZIG_ZAG);  //Customizado
    snake.begin();
    snake.fill(0x00000F, 0, 50);
    snake.show();
    delay(1000);
    snake.limpa_matriz(0x0F0000);
    delay(1000);
    snake.limpa_matriz(0x000000);
  #endif //Like_Tetris
}

void loop() 
{
  #ifdef Like_Tetris
    //tt.pisca();
    //teste1();
    //teste2();
    //teste3();
    //teste4();
    //teste5();
    //teste6();
    //teste7();
    //teste8();
    //teste9();
    //teste10();
    //teste11();
    teste12();  //Tetris completo
    //teste13();  
    //teste14(); 
  #else
    //teste15(); 
    teste16();    //Snake completo
  #endif
}

//Faz a leitura do joystick e atualiza o estado
void ler_joystick()
{
  uint16_t x = analogRead(joyHor);
  uint16_t y = analogRead(joyVer);
  
  if (x > 800)
      //move = ESQUERDA;  //Wokwi
      move = DIREITA;   //Joystick
  else if (x < 200)
      //move = DIREITA;   //Wokwi
      move = ESQUERDA;  //Joystick
  else if (y > 800)
      move = CIMA;
  else if (y < 200)
      move = BAIXO;
  //else
  //    move = STOP;
  
  Serial.print("V = "); Serial.println(y);
  Serial.print("H = "); Serial.println(x);
  Serial.print("M = "); Serial.println(move);
}


#ifdef Like_Tetris  //Testes da biblioteca Like_Tetris

//Teste 1 - Desloca um pixel dentre os pontos de origem e destino por tempo
void teste1()
{
    tt.limpa_matriz(0x0F0F0F);
    delay(1000);
    tt.desloca_od({2,0}, {2,10}, 0x00FF00, 0xFF0000, 500);
    delay(1000);
    tt.desloca_od({0,0}, {9,14}, 0x00FF00, 0xFF0000, 100);
    delay(1000);
    tt.desloca_od({0,10}, {9,14}, 0X00FF00, 0xFF0000, 100);
    delay(1000);
}

//Teste 2 - Desloca um ponto a partir da última posição (rodar com o teste 5)
void teste2()
{
  tt.limpa_matriz(0x0F0F0F);
  delay(1000);
  tt.desloca_delta({0,2}, 0x00FF00, 0xFF0000, 100);
  delay(1000);
  tt.desloca_delta({1,3}, 0x00FF00, 0xFF0000, 100);
  delay(1000);
  tt.desloca_delta({1,4}, 0x00FF00, 0xFF0000, 100);
  delay(1000);
}

//Teste 3 - Desenho de linhas usando origem e destino e delta
void teste3()
{
  tt.limpa_matriz(0x0F0F0F);
  delay(100);
  tt.linha_od({0,0}, {9,9}, 0xFF0000, 100);
  tt.linha_delta({-9,9}, 0xFF0000, 100); //A classe salva a posição final da ultima linha
  delay(100);
}

//Teste 4 - Desenha todos os modelos de blocos
void teste4()
{
  t_bloco bloco;
  tt.limpa_matriz(0x0F0F0F);
  delay(500);
  bloco = tt.cria_bloco({2,0},0,0xFF0000);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({1,2},1,0xFF0000);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({5,2},2,0xFF0000);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({2,5},3,0xFF0000);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({2,8},4,0xFF0000);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({1,11},5,0xFF0000);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({5,11},6,0xFF0000);
  tt.desenha_bloco(bloco);
  delay(1000);
}

//Teste 5 - Deslocamento de bloco e verificação de fim da matriz
void teste5()
{
  t_bloco bloco = tt.cria_bloco({0,0},2,0xFF0000);
  tt.limpa_matriz(0x0F0F0F);
  while(true)
  {
    if (tt.desenha_bloco(bloco) == false)
    {
      Serial.println("Acabou a matriz");
      bloco = tt.cria_bloco({0,0},2,0xFF0000);
    }
    Serial.print("bloco P[0].y="); Serial.println(bloco.P[0].y); 
    delay(300);
    tt.apaga_bloco(bloco, 0x0F0F0F);
    bloco = tt.desloca_bloco(bloco, {1,1});
  }
}

//Teste 6 - Testa deslocamento e ocupação de posição manual
void teste6()
{
  t_bloco bloco = tt.cria_bloco({0,0},2,0xFFFF00);
  t_bloco bloco2;

  tt.limpa_matriz(0x000000);
  
  if (tt.testa_ocupacao(bloco, 0x000000) == false)
  {
    tt.desenha_bloco(bloco);
    delay(1000);
  }
  else
  {
    Serial.print('FIM');
    return;
  }
  while(true)
  {
    //Desloca
    bloco2 = tt.desloca_bloco(bloco, {0,1});
    
    Serial.print("bloco P[0].y="); Serial.println(bloco.P[0].y);
    Serial.print("bloco2 P[0].y="); Serial.println(bloco2.P[0].y); 
    
    //Apaga bloco1
    tt.apaga_bloco(bloco, 0x000000);
    
    //Testa
    if (tt.testa_ocupacao(bloco2, 0x000000) == false)
    {
      //Desenha bloco2
      tt.desenha_bloco(bloco2);
      //Atualiza bloco
      //TODO: checar essa passagem por referência
      bloco = bloco2;
      //Atraso
      delay(200);
    }
    else
    {
      //Desenha bloco1 novamente
      tt.desenha_bloco(bloco);
      break;
    }
  }
}

//Teste 7 - Testa deslocamento e ocupação de posição por método
void teste7()
{
  bool estado;
  t_bloco bloco = tt.cria_bloco({0,0},1,0xFF0000);
  t_bloco bloco2;

  tt.limpa_matriz(0x000000);
  delay(500);

  if (tt.testa_ocupacao(bloco, 0x000000) == false)
  {
    tt.desenha_bloco(bloco);
    delay(500);
    estado = true;
    while(estado == true)
    {
      Serial.print("blocoR P[0].y="); Serial.println(bloco.P[0].y);
      //Atraso
      delay(200);
      //Novo movimento
      estado = tt.movimento_completo(bloco, 0x000000, &bloco);  //Atualiza o próprio bloco
      Serial.print("estado="); Serial.println(estado);
      Serial.print("blocoR P[0].y="); Serial.println(bloco.P[0].y);
    }
  }
}

//Teste 8 - Referência de estruturas
void teste8()
{
    t_bloco bloco = {{{0,0}, {1,1}, {2,2}, {3,3}}, 4, 5, 6};
    t_bloco bloco2 = bloco;
    
    Serial.print("bloco.P[1].x ="); Serial.println(bloco.P[1].x);
    Serial.print("bloco2.P[1].x ="); Serial.println(bloco2.P[1].x);
    
    bloco2.P[1].x = 3;
    //São estruturas independentes
    Serial.print("bloco.P[1].x ="); Serial.println(bloco.P[1].x);
    Serial.print("bloco2.P[1].x ="); Serial.println(bloco2.P[1].x);

    //Atualizei a referência (faz uma cópia completa)
    bloco = bloco2;
    Serial.print("bloco.P[1].x ="); Serial.println(bloco.P[1].x);
    Serial.print("bloco2.P[1].x ="); Serial.println(bloco2.P[1].x);

    bloco2.P[1].x = 5;
    //São estruturas independentes
    Serial.print("bloco.P[1].x ="); Serial.println(bloco.P[1].x);
    Serial.print("bloco2.P[1].x ="); Serial.println(bloco2.P[1].x);

    delay(2000);
}

//Teste 9 - Teste rotação dos blocos e choque com os limites da matriz
void teste9()
{
  //t_bloco bloco = tt.cria_bloco({0,0},0,0xFF0000);
  t_bloco bloco = tt.cria_bloco({5,5},0,0xFF0000);
  t_bloco bloco_aux = bloco;

  tt.limpa_matriz(0x000000);
  delay(500);

  if (tt.testa_ocupacao(bloco, 0x000000) == false)
  {
      tt.desenha_bloco(bloco);
  }
  delay(500);
  //Apaga o bloco
  tt.apaga_bloco(bloco, 0x000000);

  bool estado = true;
  while (estado == true)
  {
    Serial.println("Antes");
    tt.print_bloco(bloco);
    //Rotaciona o bloco
    bloco = tt.rotaciona_bloco(bloco);
    Serial.println("Depois");
    tt.print_bloco(bloco);
    //Faz ajuste de posição (testa se o bloco ficou partido em duas partes após
    //rotacionar ou deslocar)
    bloco_aux = tt.ajusta_posicao_bloco(bloco);
    Serial.println("Ajustado");
    tt.print_bloco(bloco_aux);
    //Desenha o bloco
    //tt.desenha_bloco(bloco) //Bloco original
    tt.desenha_bloco(bloco_aux);//Bloco ajustado
    //Atraso
    delay(500);
    //Apaga o bloco auxiliar (importante quando tem ajuste de posição)
    tt.apaga_bloco(bloco_aux, 0x000000);
  }
}

//Teste 10 - Mover um bloco por botões
void teste10()
{
  uint32_t cor_fundo = 0x000000;
  t_bloco bloco, bloco_aux;

  tt.limpa_matriz(cor_fundo);
    
  while (true)
  {
    bloco = tt.cria_bloco({5,0},1,0xFFFF00);
    if (tt.testa_ocupacao(bloco, 0x000000) == false)
    {
      tt.desenha_bloco(bloco);
      delay(1000);
    }
    bool estado = true;
    while (estado == true)
    {
      //Novo movimento
      estado = tt.movimento_completo(bloco, cor_fundo, &bloco);
            
      if (digitalRead(botaoEsq) == 0)
      {
        Serial.println("botaoEsq = 0");
        //Apaga o bloco
        tt.apaga_bloco(bloco, 0x000000);
        //Desloca
        bloco = tt.desloca_bloco(bloco, {-1,0});
        //Faz ajuste de posição (testa se o bloco ficou partido em duas partes após
        //rotacionar ou deslocar)
        bloco = tt.ajusta_posicao_bloco(bloco);
        //Desenha o bloco
        tt.desenha_bloco(bloco);
      }     
      if (digitalRead(botaoDir) == 0)
      {
        Serial.println("botaoDir = 0");
        //Apaga o bloco
        tt.apaga_bloco(bloco, 0x000000);
        //Desloca
        bloco = tt.desloca_bloco(bloco, {1,0});
        //Faz ajuste de posição (testa se o bloco ficou partido em duas partes após
        //rotacionar ou deslocar)
        bloco = tt.ajusta_posicao_bloco(bloco);
        //Desenha o bloco
        tt.desenha_bloco(bloco);
      }     
      if (digitalRead(botaoRot) == 0)
      {
        Serial.println("botaoRot = 0");
        //Apaga o bloco
        tt.apaga_bloco(bloco, 0x000000);
        //Rotaciona o bloco
        bloco = tt.rotaciona_bloco(bloco);
        //Faz ajuste de posição (testa se o bloco ficou partido em duas partes após
        //rotacionar ou deslocar)
        bloco_aux = tt.ajusta_posicao_bloco(bloco);
        //Desenha o bloco
        //tt.desenha_bloco(bloco); //Bloco original
        tt.desenha_bloco(bloco_aux); //Bloco ajustado
      }    
      delay(500);
    }
  }
  Serial.println("Fim");
}

//Teste 11 - Mover um bloco por botões, novo ajuste
void teste11()
{  
  uint32_t cor_fundo = 0x000000;
  t_bloco bloco, bloco2;

  tt.limpa_matriz(cor_fundo);
  
  //Pré-ocupa algumas linhas parcialmente
  bloco = tt.cria_bloco({0,14},0,0xFFFF00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({0,13},0,0xFFFF00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({0,12},0,0xFFFF00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({6,14},0,0xFFFF00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({6,13},0,0xFFFF00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({6,12},0,0xFFFF00);
  tt.desenha_bloco(bloco);

  while(true)
  {
    //1-Cria um novo bloco
    bloco = tt.cria_bloco({5,0},0,0xFFFF00);
    
    //2-Teste se a posição já está ocupada
    if (tt.testa_ocupacao(bloco, 0x000000) == false)
    {
      tt.desenha_bloco(bloco);
      delay(1000);
    }
    else
    {
      Serial.println("FIM DO JOGO");
      return;          
    }
    //3-Inicia um loop de descida do bloco
    bool estado = true;
    while (estado == true)
    {
      //Debug - mostra o bloca a ser movido
      tt.print_bloco(bloco);
        
      //Novo movimento
      estado = tt.movimento_completo(bloco, cor_fundo, &bloco);
        
      //Atraso entre um novo movimento
      delay(500);
        
      //Testa se o movimento foi possível
      if (estado == false)
      {
        break; //Interrompe o laço
      } 
      //Testa os botões            
        
      //Botão esquerdo
      if (digitalRead(botaoEsq) == 0)
      {
        Serial.println("botaoEsq = 0");
        //Apaga o bloco
        tt.apaga_bloco(bloco, 0x000000);
        //Desloca
        bloco2 = tt.desloca_bloco(bloco, {-1,0});
        //Faz ajuste de posição (testa se o bloco ficou partido em duas partes após
        //rotacionar ou deslocar)
        bloco2 = tt.ajusta_posicao_bloco(bloco2);
        //Testa
        if (tt.testa_ocupacao(bloco2, cor_fundo) == false)
        {
          //Desenha bloco2
          tt.desenha_bloco(bloco2);
          //Atualiza o bloco1
          bloco = bloco2;
        }
        else
        {
          //Desenha bloco1
          tt.desenha_bloco(bloco);
        }
      }     
      
      //Botão direito
      if (digitalRead(botaoDir) == 0)
      {
        Serial.println("botaoDir = 0");
        //Apaga o bloco
        tt.apaga_bloco(bloco, 0x000000);
        //Desloca
        bloco2 = tt.desloca_bloco(bloco, {+1,0});
        //Faz ajuste de posição (testa se o bloco ficou partido em duas partes após
        //rotacionar ou deslocar)
        bloco2 = tt.ajusta_posicao_bloco(bloco2);
        //Testa
        if (tt.testa_ocupacao(bloco2, cor_fundo) == false)
        {
          //Desenha bloco2
          tt.desenha_bloco(bloco2);
          //Atualiza o bloco1
          bloco = bloco2;
        }
        else
        {
          //Desenha bloco1
          tt.desenha_bloco(bloco);
        }
      }         
              
      //Botão de rotação
      if (digitalRead(botaoRot) == 0)
      {
        Serial.println("botaoRot = 0");
        //Apaga o bloco
        tt.apaga_bloco(bloco, 0x000000);
        //Rotaciona o bloco
        bloco2 = tt.rotaciona_bloco(bloco);
        //Faz ajuste de posição (testa se o bloco ficou partido em duas partes após
        //rotacionar ou deslocar)
        bloco2 = tt.ajusta_posicao_bloco(bloco2);
        //Testa
        if (tt.testa_ocupacao(bloco2, cor_fundo) == false)
        {
          //Desenha bloco2
          tt.desenha_bloco(bloco2);
          //Atualiza o bloco1
          bloco = bloco2;
        }
        else
        {
          //Desenha bloco1
          tt.desenha_bloco(bloco);
        }
      }     
    }
    //4-Verifica o preenchimento de linhas
    tt.testa_preenchimento_linha(cor_fundo);
  }
}

//Teste 12 - Tetris completo - move bloco por botões com sorteio de tipo e cor
void teste12()
{
  uint32_t cor_fundo = 0x000000;
  t_bloco bloco, bloco2;

  tt.limpa_matriz(cor_fundo);
  
  //Com bateria externa (cores intensas)
  /*
  //Pré-ocupa algumas linhas parcialmente
  bloco = tt.cria_bloco({0,14},0,0xFFFF00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({0,13},0,0xFFFF00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({0,12},0,0xFFFF00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({6,14},0,0xFFFF00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({6,13},0,0xFFFF00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({6,12},0,0xFFFF00);
  tt.desenha_bloco(bloco);
  //*/

  //Sem bateria externa (cores suaves)
  //*
  //Pré-ocupa algumas linhas parcialmente
  bloco = tt.cria_bloco({0,14},0,0x0F0F00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({0,13},0,0x0F0F00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({0,12},0,0x0F0F00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({6,14},0,0x0F0F00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({6,13},0,0x0F0F00);
  tt.desenha_bloco(bloco);
  bloco = tt.cria_bloco({6,12},0,0x0F0F00);
  tt.desenha_bloco(bloco);
  //*/

  while (true)
  {
    //0-Sorteia o tipo de bloco e cor
    int tipo = random(0,7); //0..<7

    //Com bateria externa (cores intensas)
    /*
    uint32_t cvm = random(0,2) * 255; //0..<2
    uint32_t cvd = random(0,2) * 255; //0..<2
    uint32_t caz = random(0,2) * 255; //0..<2    
    if (cvm==0 && cvd==0 && caz==0)
    {
        cvm = 255;
        cvd = 255;
        caz = 255;
    }
    //*/
    //Sem bateria externa (cores suaves)
    //*
    uint32_t cvm = random(0,2) * 15; //0..<2
    uint32_t cvd = random(0,2) * 15; //0..<2
    uint32_t caz = random(0,2) * 15; //0..<2    
    if (cvm==0 && cvd==0 && caz==0)
    {
        cvm = 15;
        cvd = 15;
        caz = 15;
    }
    //*/

    uint32_t cor = (cvm<<16) + (cvd<<8) + (caz<<0);
    //1-Cria um novo bloco
    bloco = tt.cria_bloco({5,0},tipo,cor);
    //Serial.print("cvm="); Serial.println(cvm);
    //Serial.print("cvd="); Serial.println(cvd);
    //Serial.print("caz="); Serial.println(caz);
    //Serial.print("cor="); Serial.println(cor);
    //while(1);
    
    //2-Teste se a posição já está ocupada
    if (tt.testa_ocupacao(bloco, 0x000000) == false)
    {
      tt.desenha_bloco(bloco);
      delay(1000);
    }
    else
    {
      Serial.println("FIM DO JOGO");
      return;            
    }

    //3-Inicia um loop de descida do bloco
    bool estado = true;
    while (estado == true)
    {
      //Debug - mostra o bloca a ser movido
      Serial.print("bloco a ser movido");
      tt.print_bloco(bloco);
      
      //Novo movimento
      estado = tt.movimento_completo(bloco, cor_fundo, &bloco);
      
      //Atraso entre um novo movimento
      delay(500);
      
      //Testa se o movimento foi possível
      if (estado == false)
      {
          break; //Interrompe o laço
      }

      //Testa os botões            
      
      //Botão esquerdo
      if (digitalRead(botaoEsq) == 0)
      {
        Serial.println("botaoEsq = 0");
        //Apaga o bloco
        tt.apaga_bloco(bloco, 0x000000);
        //Desloca
        bloco2 = tt.desloca_bloco(bloco, {-1,0});
        //Faz ajuste de posição (testa se o bloco ficou partido em duas partes após
        //rotacionar ou deslocar)
        bloco2 = tt.ajusta_posicao_bloco(bloco2);
        //Testa
        if (tt.testa_ocupacao(bloco2, cor_fundo) == false)
        {
          //Desenha bloco2
          tt.desenha_bloco(bloco2);
          //Atualiza o bloco1
          bloco = bloco2;
        }
        else
        {
          //Desenha bloco1
          tt.desenha_bloco(bloco);
        }
      }     
      
      //Botão direito
      if (digitalRead(botaoDir) == 0)
      {
        Serial.println("botaoDir = 0");
        //Apaga o bloco
        tt.apaga_bloco(bloco, 0x000000);
        //Desloca
        bloco2 = tt.desloca_bloco(bloco, {+1,0});
        //Faz ajuste de posição (testa se o bloco ficou partido em duas partes após
        //rotacionar ou deslocar)
        bloco2 = tt.ajusta_posicao_bloco(bloco2);
        //Testa
        if (tt.testa_ocupacao(bloco2, cor_fundo) == false)
        {
          //Desenha bloco2
          tt.desenha_bloco(bloco2);
          //Atualiza o bloco1
          bloco = bloco2;
        }
        else
        {
          //Desenha bloco1
          tt.desenha_bloco(bloco);
        }
      }         
              
      //Botão de rotação
      if (digitalRead(botaoRot) == 0)
      {
        Serial.println("botaoRot = 0");
        //Apaga o bloco
        tt.apaga_bloco(bloco, 0x000000);
        //Rotaciona o bloco
        bloco2 = tt.rotaciona_bloco(bloco);
        //Faz ajuste de posição (testa se o bloco ficou partido em duas partes após
        //rotacionar ou deslocar)
        bloco2 = tt.ajusta_posicao_bloco(bloco2);
        //Testa
        if (tt.testa_ocupacao(bloco2, cor_fundo) == false)
        {
          //Desenha bloco2
          tt.desenha_bloco(bloco2);
          //Atualiza o bloco1
          bloco = bloco2;
        }
        else
        {
          //Desenha bloco1
          tt.desenha_bloco(bloco);
        }   
      }     
    }
    //4-Verifica o preenchimento de linhas
    tt.testa_preenchimento_linha(cor_fundo);
  }
}

//Teste 13 - Caracteres
void teste13()
{
  uint32_t cor_fundo = 0x000000;
  uint32_t cores[] = {0x0F0000, 0x000F00, 0x00000F, 0x0F0F0F};
        
  tt.limpa_matriz(cor_fundo);

  uint8_t tam = TAM_P;
  uint8_t cor = 0;
  while (true)
  {
    uint8_t carac = 0;
    
    while (carac<16)
    {
      tt.limpa_matriz(cor_fundo);
      delay(100);
      //desenha_caractere(&tt, carac, tam, cores[cor]);
      tt.desenha_numero(carac, tam, cores[cor], {0,0});
      carac = carac+1;
      delay(200);
      if (digitalRead(botaoRot) == 0)
          tam = (tam+1)%3;
      if (digitalRead(botaoEsq) == 0)
          cor = (cor+1)%4;
    }
    cor = (cor+1)%4;
  }
}

#else //Testes da biblioteca Like_Snake

//Teste 14 - Leitura do joystick
void teste14()
{
  while (true)
  {
    Serial.print("Vertical = "); Serial.println(analogRead(joyVer));
    Serial.print("Horizonntal = "); Serial.println(analogRead(joyHor));
    delay(300);
  }
}

//Teste 15 - Teste Snake
void teste15()
{
  uint32_t cor_cobra = 0x00FF00;
  uint32_t cor_cabeca = 0xFF0000;
  uint32_t cor_fruta = 0x0000FF;
                             //(pino,col,     lin,    cor_cobra, cor_cabeca, cor_fruta, tipo_mat="t_linha");
  //like_snake snake = like_snake(6,   colunas, linhas, cor_cobra, cor_cabeca, cor_fruta, T_LINHA_ZIG_ZAG);
  snake.fill(0x000000);
  snake.show();
  //while(1);

  while (true)
  {
    snake.sorteia_pos_fruta();
    snake.sorteia_pos_cobra();
    delay(1000);
  }
}

//Teste 16 - Snake completo
void teste16()
{
  uint32_t cor_cobra = 0x00FF00;
  uint32_t cor_cabeca = 0xFF0000;
  uint32_t cor_fruta = 0x0000FF;
  uint32_t cor_fundo = 0x000000;
                             //(pino,col,     lin,    cor_cobra, cor_cabeca, cor_fruta, tipo_mat="t_linha");
  //like_snake snake = like_snake(6,   colunas, linhas, cor_cobra, cor_cabeca, cor_fruta, "t_linha_zig_zag");
  snake.fill(0x000000);
  snake.show();
  //while(1);

  Serial.println("**** Início do Jogo *****");
    
  snake.sorteia_pos_fruta();
  snake.sorteia_pos_cobra();
  delay(1000);
  //while(1);
  while (true)
  {
    ler_joystick();
    Serial.print("Move loop = "); Serial.println(move);

    //while(1);
    bool dc = snake.desloca_cobra(move);
    //Serial.println("Desloca = "); Serial.println(dc);
    if (dc == true)
    {
        bool cf = snake.comeu_fruta();
        //Serial.print("Comeu = "); Serial.println(cf);
        if (cf == true)
          snake.sorteia_pos_fruta();
    }
    else
    {
      delay(1000);
      snake.limpa_matriz(cor_fundo);
      snake.desenha_pontuacao();          //Cor padrão
      //snake.desenha_pontuacao(0x070000);  //Cor customizada
      delay(2000);
      move = STOP; //Garante que o jogo começa com a cobra parada
      snake.reinicia_jogo();
    } 
    delay(300);
  } 
}

#endif //Fim dos testes
