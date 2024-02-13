/*
Desenvolvida por: Leandro Poloni Dantas
Data: Janeiro/2023
Sobre: Classe com métodos customizados para desenvolver um jogo como snake
usando fitas de led do tipo WS2812B e herda a classe NeoPixel
Essa classe é uma adapatação da classe criada originalmente em MicroPython

Data: Fevereiro/2024
Foi introduzido do define USAR_SERIAL que habilita o uso da porta serial
para envio de mensagens de depuração. Essa modificação foi necessária
por permitir a simulação no SimulIDE 1.0.0 que não é compatível com o 
uso da porta serial.

Material de referência:
https://docs.micropython.org/en/latest/library/neopixel.html
https://docs.micropython.org/en/latest/esp8266/tutorial/neopixel.html
Exemplo do uso de canvas: https://wokwi.com/projects/330751728413573715
Outro exemplo: https://github.com/wokwi/wokwi-features/issues/237
Exemplos da bilbioteca: https://wokwi.com/arduino/libraries/Adafruit_NeoPixel
Snake online: https://playsnake.org/
*/

#ifndef _LIKE_SNAKE_H_
#define _LIKE_SNAKE_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
//Documentação: https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.h

#define T_LINHA				    0
#define T_LINHA_ZIG_ZAG		1
#define T_COLUNA			    2
#define T_COLUNA_ZIG_ZAG	3

#define STOP      0
#define ESQUERDA  1
#define DIREITA   2
#define CIMA      3
#define BAIXO     4

#define TAM_G     0
#define TAM_M     1
#define TAM_P     2

typedef struct { uint8_t x, y; } t_ponto;
typedef struct { t_ponto P[50]; uint8_t len; } t_cobra; //Testar o limite de memória do Arduino ao compilar (ideal 150)


class like_snake : public Adafruit_NeoPixel
{
	public:  
	uint8_t colunas, linhas, tipo_matriz;
	uint16_t pixels;
	t_ponto posicao;	//x e y
	uint32_t cor_cobra, cor_cabeca, cor_fruta, cor_fundo;
	uint8_t tamanho, pontos, movimento;
	t_ponto pos_fruta, pos_ponta_rabo;
  t_cobra pos_cobra;
    
	public:
	//Construtor
	like_snake(int16_t pino = 6, uint16_t col = 10, uint16_t lin = 15,
			       uint32_t corcobra = 0x00FF00, uint32_t corcabeca = 0xFF0000, uint32_t corfruta = 0x0000FF,
			       uint8_t tipo_mat = T_LINHA_ZIG_ZAG);
			   
	//Retorna o número do pixel de um ponto
  //Isso depende da forma construtiva da matriz
  int num_pixel(t_ponto xy);
    
    //Limpa a matriz todo com a cor escolhida
	void limpa_matriz(uint32_t cor);
    
	//Desenha uma linha a parir de um ponto de origem até um ponto de destino
	//Reporta a posição final
	t_ponto desenha_linha(t_ponto origem, t_ponto destino, uint32_t cor_pixel);
	
	//Desenha um número na tela
	void desenha_numero(uint8_t numero, uint8_t tam, uint32_t cor, t_ponto ponto);
    
	//Desenha a pontuação atual na tela
	void desenha_pontuacao(uint32_t cor = 0xFF0000);

    //Desenha a fruta
	void desenha_fruta(void);

    //Desenha a cobra
	void desenha_cobra(void);
    
	//Sorteia a posição da fruta e desenha
	void sorteia_pos_fruta(void);
	
	//Sorteia a posição da cobra e desenha
	void sorteia_pos_cobra(void);
	
    //Desloca a cobra
	bool desloca_cobra(uint8_t move);
    
	//Verifica se a cobra comeu a fruta
	bool comeu_fruta(void);
        
    //Reinia o jogo
	void reinicia_jogo(void);

	//Acrescenta um ponto na cobra
	void cobra_append(t_ponto ponto);
    
	//Debug: imprime um ponto na serial
	void print_ponto(t_ponto ponto);
};

#endif // _LIKE_SNAKE_H_
