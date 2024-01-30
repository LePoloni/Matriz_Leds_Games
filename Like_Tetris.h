/*
Desenvolvida por: Leandro Poloni Dantas
Data: Janeiro/2023
Sobre: Classe com métodos customizados para desenvolver um jogo como tetris
usando fitas de led do tipo WS2812B e herda a classe NeoPixel
Essa classe é uma adapatação da classe criada originalmente em MicroPython

Material de referência:
https://docs.micropython.org/en/latest/library/neopixel.html
https://docs.micropython.org/en/latest/esp8266/tutorial/neopixel.html
Exemplo do uso de canvas: https://wokwi.com/projects/330751728413573715
Outro exemplo: https://github.com/wokwi/wokwi-features/issues/237
Exemplos da bilbioteca: https://wokwi.com/arduino/libraries/Adafruit_NeoPixel
Tetris online: https://tetris.com/play-tetris
*/

#ifndef _LIKE_TETRIS_H_
#define _LIKE_TETRIS_H_

#include <Arduino.h>
//#include <Adafruit_NeoMatrix.h> 
//Documentação: https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
#include <Adafruit_NeoPixel.h>
//Documentação: https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.h

#define T_LINHA				    0
#define T_LINHA_ZIG_ZAG		1
#define T_COLUNA			    2
#define T_COLUNA_ZIG_ZAG	3

#define X					0
#define Y					1

#define B_TRACO			0
#define B_Z					1
#define B_S					2
#define B_QUADRADO	3
#define B_T					4
#define B_L					5
#define B_J					6

#define R					0
#define G					1
#define B					2

#define TAM_G     0
#define TAM_M     1
#define TAM_P     2

typedef struct { uint8_t x, y; } t_ponto;
typedef struct { 
				t_ponto P[4];	      //P1..4
				uint32_t cor;	      //Cor
				uint8_t tipo;		    //Tipo
				uint8_t ponto_fixo;	//Ponto Fixo				
			   } t_bloco;

class like_tetris : public Adafruit_NeoPixel//, public Adafruit_NeoMatrix
{
  public:
	uint8_t colunas, linhas, tipo_matriz;
  uint16_t pixels;
  uint8_t pontos;
	t_ponto posicao;	//x e y
	
	public:	
	//Construtor
	like_tetris(int16_t pino = 6, uint8_t col = 10, uint8_t lin = 15, uint8_t tipo_mat = T_LINHA_ZIG_ZAG);
    
  //Teste
  void pisca();

  //Retorna o número do pixel de um ponto
  //Isso depende da forma construtiva da matriz
  int num_pixel(t_ponto xy);

  //Limpa a matriz todo com a cor escolhida
  void limpa_matriz(uint32_t cor);
  
  //Desenha uma linha a parir de um ponto de origem até um ponto de destino
  //Reporta a posição final
  t_ponto linha_od(t_ponto origem, t_ponto destino, uint32_t cor_pixel, uint32_t tempo);
  
  //Desenha uma linha a parir do último ponto desenhado até de deslocamento desejado
  //Reporta a posição final
  t_ponto linha_delta(t_ponto offset, uint32_t cor_pixel, uint32_t tempo);

  //Desenha um número na matriz
  void desenha_numero(uint8_t numero, uint8_t tam, uint32_t cor, t_ponto ponto);

  //Desenha a pontuação atual na matriz
  void desenha_pontuacao(uint32_t cor);
  
  //Desloca um pixel do ponto de origem ao ponto de destino respeitando o tempo
  //entre cada movimento
  //Reporta a posição final
  t_ponto desloca_od(t_ponto origem, t_ponto destino, uint32_t cor_fundo, uint32_t cor_pixel, uint32_t tempo);
  
  //Desloca um pixel a parir do último ponto desenhado até de deslocamento desejado
  //Reporta a posição final
  t_ponto desloca_delta(t_ponto offset, uint32_t cor_fundo, uint32_t cor_pixel, uint32_t tempo);

  //Cria um bloco de tetris a partir de um ponto de origem, tipo e cor desejada
  //Não desenha o bloco
  //Retorna um bloco
  t_bloco cria_bloco(t_ponto origem, uint8_t tipo, uint32_t cor);
  
  //Desenha um bloco
  //Retorna True para sucesso e False para falha
  bool desenha_bloco(t_bloco bloco);
  
  //Apaga um bloco
  void apaga_bloco(t_bloco bloco, uint32_t cor_fundo);

  //Desloca um bloco de acordo com o offset (x,y) desejado
  //Não desenha o bloco, nem verifa os limites da matriz
  //Retorna um bloco
  t_bloco desloca_bloco(t_bloco bloco, t_ponto offset);

  //Verifica se as coordenadas do bloco passado então na área da matriz e se
  //todos o pixel tem cor igual a cor de fundo passada
  //Esse método serve para verificar se um bloco pode ocupar uma posição nova
  //Retorna True para sucesso e False para falha
  bool testa_ocupacao(t_bloco bloco, uint32_t cor_fundo);
      
  //Tenta deslocar um bloco uma linha para baixo
  //Retorna True para sucesso e False para falha
  //Retorna um bloco reposicionado
  bool movimento_completo(t_bloco bloco, uint32_t cor_fundo, t_bloco *bloco_ret);
      
  //Rotaciona um bloco 90 graus em sentido horário
  //Retorna um bloco
  t_bloco rotaciona_bloco(t_bloco bloco);

  //Ajusta o bloco para não ficar partido em duas partes após rotacionar
  //Retorna um bloco
  t_bloco ajusta_posicao_bloco(t_bloco bloco);
  
  //Verifica se uma linha foi preenchida. Em caso positivo apaga seu
  //conteúdo de desloca as demais para baixo
  void testa_preenchimento_linha(uint32_t cor_fundo);

  //Debug: imprime um bloco na serial
  void print_bloco(t_bloco bloco);

  //Debug: imprime um ponto na serial
  void print_ponto(t_ponto ponto);
};

#endif // _LIKE_TETRIS_H_
