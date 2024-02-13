/*
Desenvolvida por: Leandro Poloni Dantas
Data: Janeiro/2023
Sobre: Classe com métodos customizados para desenvolver um jogo como snake
usando fitas de led do tipo WS2812B e herda a classe NeoPixel.
Essa classe é uma adapatação da classe criada originalmente em MicroPython.

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

#include "Like_Snake.h"
//#define USAR_SERIAL

//Construtor
like_snake::like_snake(int16_t pino, uint16_t col, uint16_t lin,
			                 uint32_t corcobra, uint32_t corcabeca, uint32_t corfruta,
			                 uint8_t tipo_mat)
			     :Adafruit_NeoPixel(col*lin, pino, NEO_GRB + NEO_KHZ800)	//Construtor da classe herdada
{
	colunas = col;
	linhas = lin;
	pixels = col*lin;
	posicao = {0,0};	//x e y
	tipo_matriz = tipo_mat;
	
	cor_cobra = corcobra;
  cor_cabeca = corcabeca;
  cor_fruta = corfruta;
  cor_fundo = 0x000000;

  tipo_matriz = tipo_mat;

  tamanho = 5;		//Tamanho padrão da cobra
  pontos = 0;			//Pontuação inicial do jogo
  movimento = STOP;	//Estado do jogo
	pos_fruta = {0,0};	//Posição padrão da fruta
  pos_cobra.len = 0;	//posição padrão da cobra
	cobra_append({0,1});
	cobra_append({1,1});
	cobra_append({2,1});
	cobra_append({3,1});
	cobra_append({4,1});
	pos_ponta_rabo = {4,1};
	
	//Nenhum método da classe herdada deve ser executado aqui!
	//Não usar delay
	
	//Inicializa GPIO de debug
	//pinMode(13, OUTPUT);
	//digitalWrite(13, LOW);	

	//Inicializa a serial para debug
#ifdef USAR_SERIAL
	Serial.begin(9600);
	Serial.println("Like Snake!");
#endif
}
    
//Retorna o número do pixel de um ponto
//Isso depende da forma construtiva da matriz
int like_snake::num_pixel(t_ponto xy)
{
	if (tipo_matriz == T_LINHA)
		return xy.x + xy.y*colunas;
	else if (tipo_matriz == T_LINHA_ZIG_ZAG)
	{
    //Se é linha par (avança da esq. p/ dir.)
		if (xy.y%2 == 0)
			return xy.x + xy.y*colunas;
		//Se é linha ímpar (avança da dir. p/ esq.)
		else
			return colunas-1-xy.x + xy.y*colunas;
	}
	else if (tipo_matriz == T_COLUNA)
		return xy.x*linhas + xy.y;        
	else if (tipo_matriz == T_COLUNA_ZIG_ZAG)
	{
		//Se é coluna par (avança da cima p/ baixo)
		if (xy.x%2 == 0)
			return xy.x*linhas + xy.y;
		//Se é coluna ímpar (avança da vaixo p/ cima)
		else
			return xy.x*linhas + linhas-1-xy.y;
	}
	else //Tipo t_linha é o padrão
		return xy.x + xy.y*colunas;
}

//Limpa a matriz todo com a cor escolhida
void like_snake::limpa_matriz(uint32_t cor)
{
	fill(cor, 0, pixels);
	show();
	posicao = {0, 0};
}    

//Desenha uma linha a parir de um ponto de origem até um ponto de destino
//Reporta a posição final
t_ponto like_snake::desenha_linha(t_ponto origem, t_ponto destino, uint32_t cor_pixel)
{
	int8_t ox, oy, dx, dy, deltax, deltay, mdeltax, mdeltay;
	uint16_t pixel;
	ox = origem.x;
	oy = origem.y;
	dx = destino.x;
	dy = destino.y;
	deltax = dx-ox;
	deltay = dy-oy;
	
	if (deltax < 0)
		mdeltax = -deltax;
	else
		mdeltax = deltax;
	
	if  (deltay < 0)
		mdeltay = -deltay;
	else
		mdeltay = deltay;

	if (mdeltax > mdeltay)
	{
		if (ox < dx)
		{
			while (ox <= dx)
			{
				pixel = num_pixel({ox, oy});
				
				setPixelColor(pixel, cor_pixel);
				show();
				ox = ox + 1;
				if (oy < dy)
					oy = oy+1;
				else if (oy > dy)
					oy = oy-1;
			}
			ox = ox - 1;
		}
		else
		{
			while (ox >= dx)
			{
				pixel = num_pixel({ox, oy});
				
				setPixelColor(pixel, cor_pixel);
				show();
				ox = ox - 1;
				if (oy < dy)
					oy = oy+1;
				else if (oy > dy)
					oy = oy-1;
			}
			ox = ox + 1;
		}
	}
	else
	{		
		if (oy < dy)
		{
			while (oy <= dy)
			{
				pixel = num_pixel({ox, oy});
				
				setPixelColor(pixel, cor_pixel);
				show();
				oy = oy + 1;
				if (ox < dx)
					ox = ox+1;
				else if (ox > dx)
					ox = ox-1;
			}
			oy = oy - 1;
		}
		else
		{
			while (oy >= dy)
			{
				pixel = num_pixel({ox, oy});
				
				setPixelColor(pixel, cor_pixel);
				show();
				oy = oy - 1;
				if (ox < dx)
					ox = ox+1;
				else if (ox > dx)
					ox = ox-1;
			}
			oy = oy + 1;
		}
	}
	posicao.x = ox;
	posicao.y = oy;
	//Serial.println(posicao);
  return posicao;
}

//Desenha um número na tela
void like_snake::desenha_numero(uint8_t numero, uint8_t tam, uint32_t cor, t_ponto ponto)
{
	//Caracteres do display de 7 segmentos
	uint8_t vetor[] = {
			0x3F,
			0x06,
			0x5B,
			0x4F,
			0x66,
			0x6D,
			0x7D,
			0x07,
			0x7F,
			0x6F,
			0x77,
			0x7C,
			0x39,
			0x5E,
			0x79,
			0x71
		  };

	//Segmentos 7x13
	t_ponto ag[2] = {{1,0},{5,0}};
	t_ponto bg[2] = {{6,1},{6,5}};
	t_ponto cg[2] = {{6,7},{6,11}};
	t_ponto dg[2] = {{1,12},{5,12}};
	t_ponto eg[2] = {{0,7},{0,11}};
	t_ponto fg[2] = {{0,1},{0,5}};
	t_ponto gg[2] = {{1,6},{5,6}};
	//Segmentos 6x11
	t_ponto am[2] = {{1,0},{4,0}};
	t_ponto bm[2] = {{5,1},{5,4}};
	t_ponto cm[2] = {{5,6},{5,9}};
	t_ponto dm[2] = {{1,10},{4,10}};
	t_ponto em[2] = {{0,6},{0,9}};
	t_ponto fm[2] = {{0,1},{0,4}};
	t_ponto gm[2] = {{1,5},{4,5}};
	//Segmentos 5x9
	t_ponto ap[2] = {{1,0},{3,0}};
	t_ponto bp[2] = {{4,1},{4,3}};
	t_ponto cp[2] = {{4,5},{4,7}};
	t_ponto dp[2] = {{1,8},{3,8}};
	t_ponto ep[2] = {{0,5},{0,7}};
	t_ponto fp[2] = {{0,1},{0,3}};
	t_ponto gp[2] = {{1,4},{3,4}};
	//Lista com os 3 tamanhos (G, M e P)
	t_ponto seg[3][7][2];
	//Grande
	seg[0][0][0] = ag[0];    seg[0][0][1] = ag[1];
	seg[0][1][0] = bg[0];    seg[0][1][1] = bg[1];
	seg[0][2][0] = cg[0];    seg[0][2][1] = cg[1];
	seg[0][3][0] = dg[0];    seg[0][3][1] = dg[1];
	seg[0][4][0] = eg[0];    seg[0][4][1] = eg[1];
	seg[0][5][0] = fg[0];    seg[0][5][1] = fg[1];
	seg[0][6][0] = gg[0];    seg[0][6][1] = gg[1];
	//Médio
	seg[1][0][0] = am[0];    seg[1][0][1] = am[1];
	seg[1][1][0] = bm[0];    seg[1][1][1] = bm[1];
	seg[1][2][0] = cm[0];    seg[1][2][1] = cm[1];
	seg[1][3][0] = dm[0];    seg[1][3][1] = dm[1];
	seg[1][4][0] = em[0];    seg[1][4][1] = em[1];
	seg[1][5][0] = fm[0];    seg[1][5][1] = fm[1];
	seg[1][6][0] = gm[0];    seg[1][6][1] = gm[1];
	//Pequeno
	seg[2][0][0] = ap[0];    seg[2][0][1] = ap[1];
	seg[2][1][0] = bp[0];    seg[2][1][1] = bp[1];
	seg[2][2][0] = cp[0];    seg[2][2][1] = cp[1];
	seg[2][3][0] = dp[0];    seg[2][3][1] = dp[1];
	seg[2][4][0] = ep[0];    seg[2][4][1] = ep[1];
	seg[2][5][0] = fp[0];    seg[2][5][1] = fp[1];
	seg[2][6][0] = gp[0];    seg[2][6][1] = gp[1];
        
	uint8_t x = vetor[numero];
	uint8_t offset_x = ponto.x;
	uint8_t offset_y = ponto.y;
        
	uint8_t s = 0;	//Segmento (0..6 --> a..g)
	while (x != 0)
	{
		if (x & 0x01)	//Testa a bit/segmento
		{
			t_ponto xy_o = {seg[tam][s][0].x+offset_x, seg[tam][s][0].y+offset_y};
			t_ponto xy_d = {seg[tam][s][1].x+offset_x, seg[tam][s][1].y+offset_y};
			//print_ponto(xy_o);
			//print_ponto(xy_d);
			desenha_linha(xy_o, xy_d, cor);
			//delay(500);
		}
		x = x >> 1;
		s += 1;		
	}
}

//Desenha a pontuação atual na tela
void like_snake::desenha_pontuacao(uint32_t cor)
{
  int dezena = pontos/10;
	int unidade = pontos%10;
	desenha_numero(dezena, TAM_P, cor, {0,3});
	desenha_numero(unidade, TAM_P, cor, {5,3});
}

//Desenha a fruta
void like_snake::desenha_fruta(void)
{
	uint16_t pixel = num_pixel(pos_fruta);
	
	setPixelColor(pixel, cor_fruta);
	//Atuaiza a matriz
	show();
#ifdef USAR_SERIAL
	Serial.print("Fruta: "); print_ponto(pos_fruta);
#endif
}   

//Desenha a cobra
void like_snake::desenha_cobra(void)
{
	uint16_t pixel = num_pixel(pos_cobra.P[0]);
	setPixelColor(pixel, cor_cabeca);
		
	uint8_t tam_cobra = pos_cobra.len;
	uint8_t i = 1;
	
	while (i < tam_cobra)
	{
		pixel = num_pixel(pos_cobra.P[i]);
		setPixelColor(pixel, cor_cobra);
		i += 1;
	}
	
	//Atuaiza a matriz
	show();
#ifdef USAR_SERIAL
	Serial.print("Cobra: "); print_ponto(pos_cobra.P[0]); print_ponto(pos_cobra.P[tam_cobra-1]);
#endif
}

//Sorteia a posição da fruta e desenha
void like_snake::sorteia_pos_fruta(void)
{
	//Apaga a fruta antiga (não precisa, a cobra já ocupou a posição)
	//t_ponto pixel = num_pixel(pos_fruta);
	//setPixelColor(pixel, cor_cor_fundo);
   
	//Sorteia a nova posição da fruta sem conflito com a cobra
	uint8_t tamanho = pos_cobra.len;
	t_ponto xy = {0,0};
	bool conflito = true;
	while (conflito == true)
	{
		//Sorteia uma cooredenada
		xy.x = random(0,colunas); //0..<colunas
		xy.y = random(0,linhas); //0..<linhas
		uint8_t i = 0;
		//Compara com toda a cobra
		while (i < tamanho)
		{
			conflito = false;
			//Se for igual a uma posição da cobra, retorna e sorteia outra posição
			if ((pos_cobra.P[i].x == xy.x) && (pos_cobra.P[i].y == xy.y))
			{
				conflito = true;
				break;
			}
			i += 1;
		}
	}
	pos_fruta = xy;
	
	//Desenha a fruta na nova posição
	desenha_fruta();
}

//Sorteia a posição da cobra e desenha
void like_snake::sorteia_pos_cobra(void)
{
	//Apaga a cobra antiga
	uint8_t tam_cobra = tamanho;
	uint8_t i = 0;
	while (i < tam_cobra)
	{
		int pixel = num_pixel(pos_cobra.P[i]);
		setPixelColor(pixel, cor_fundo);
		i += 1;
	}
	//Sorteia a nova posição da cobra sem conflito com a fruta
	uint8_t x = 0;
	uint8_t y = 0;
	t_ponto xy = {0,0};
	bool conflito = true;
	
	while (conflito == true)
	{
		//Sorteia uma cooredenada
		xy.x = random(tamanho-1, colunas); //tamanho-1..<colunas-Cabeça para direita
		xy.y = random(0,linhas); //0..<linhas
		//Assim que as linhas forem diferentes encerra o laço
		if (xy.y != pos_fruta.y)
		{
			conflito = false;
		}
	}
	//Monta a nova cobra
	pos_cobra.len = 0;
	i = 0;
	
	while (i < tamanho)
	{		
		cobra_append({xy.x-i, xy.y});
		i += 1;
	}		
	//Desenha a cobra  na nova posição
	desenha_cobra();
}

//Desloca a cobra
bool like_snake::desloca_cobra(uint8_t move)
{
	//Serial.print("Move class = "); Serial.println(move);
	if (move != STOP)
	{
		//Testa a mudança de direção do movimento
		if (movimento == STOP || movimento == ESQUERDA || movimento == DIREITA)
		{
			if (move == CIMA)
			{
				movimento = CIMA;
			}
			else if (move == BAIXO)
			{
				movimento = BAIXO;
			}
		}
		else if (movimento == STOP || movimento == CIMA || movimento == BAIXO)
		{
			if (move == ESQUERDA)
			{
				movimento = ESQUERDA;
			}
			else if (move == DIREITA)
			{
				movimento = DIREITA;
			}
		}
		//Serial.print("Movimento = "); Serial.println(movimento);
	}
  //*
	//Atualiza a posição do corpo
	//Serial.print("Movimento class = "); Serial.println(movimento);
	if (movimento != STOP)
	{
		//Salva a ponta do rabo para poder crescer a cobra
		pos_ponta_rabo = pos_cobra.P[pos_cobra.len-1];
		//Apaga a ponta do rabo
		uint16_t pixel = num_pixel(pos_ponta_rabo);
		setPixelColor(pixel, cor_fundo);
		uint8_t i = pos_cobra.len - 1;
		//Move a cobra da cauda para cabeça
		while (i > 0)
		{
			pos_cobra.P[i] = pos_cobra.P[i-1];
			i -= 1;
		}
	}
	//Atualiza a posição da cabeça e determina sucesso ou erro ao tocar nos limites da tela
	//Coordenadas da cabeça
	uint8_t y = pos_cobra.P[0].y;
	uint8_t x = pos_cobra.P[0].x;
	
	if (movimento == CIMA)
	{
		if (pos_cobra.P[0].y > 0)
		{
			y -=  1;
			pos_cobra.P[0] = {x, y};
		}
		else
		{
#ifdef USAR_SERIAL
			Serial.println("Erro, saiu da tela");
#endif
			return false;
		}
	}
	else if (movimento == BAIXO)
	{
		if (pos_cobra.P[0].y < (linhas-1))
		{
			y += 1;
			pos_cobra.P[0] = {x, y};
		}
		else
		{
#ifdef USAR_SERIAL
			Serial.println("Erro, saiu da tela");
#endif
			return false;
		}
	}
	else if (movimento == ESQUERDA)
	{
		if (pos_cobra.P[0].x > 0)
		{
			x -= 1;
			pos_cobra.P[0] = {x, y};
		}
		else
		{
#ifdef USAR_SERIAL
			Serial.println("Erro, saiu da tela");
#endif
			return false;
		}
	}
	else if (movimento == DIREITA)
	{
		if (pos_cobra.P[0].x < (colunas-1))
		{
			x += 1;
			pos_cobra.P[0] = {x, y};
		}
		else
		{
#ifdef USAR_SERIAL
			Serial.println("Erro, saiu da tela");
#endif
			return false;
		}
	}
	
	//Redesenha a cobra
	desenha_cobra();
	
	//Testa se a cobra não se mordeu
	uint16_t pixel = num_pixel(pos_cobra.P[0]);
	//Serial.print("Cor = "); Serial.println(getPixelColor(pixel));
	if (getPixelColor(pixel) == cor_cobra)
	{
#ifdef USAR_SERIAL
		Serial.println("Erro, a cobra se mordeu");
#endif
		return false;
	}
  //*/
	return true;
}       
    
//Verifica se a cobra comeu a fruta
bool like_snake::comeu_fruta(void)
{
	if ((pos_cobra.P[0].x == pos_fruta.x) && (pos_cobra.P[0].y == pos_fruta.y))
	{
		//Incrementa a pontuação
		pontos += 1;
		//Aumenta o tamanho da cobra
		tamanho += 1;
		//Cresce a cobra
		cobra_append(pos_ponta_rabo);
		//Redesenha a cobra
		desenha_cobra();
		return true;
	}
	else
	{
		return false;
	}
}

//Reinia o jogo
void like_snake::reinicia_jogo(void)
{
	tamanho = 5;
	pontos = 0;
	movimento = STOP;
	pos_fruta = {0,0};
	pos_cobra.len = 0;	//posição padrão da cobra
	cobra_append({0,1});
	cobra_append({1,1});
	cobra_append({2,1});
	cobra_append({3,1});
	cobra_append({4,1});
	
	limpa_matriz(cor_fundo);

	sorteia_pos_fruta();
	sorteia_pos_cobra();
}

//Acrescenta um ponto na cobra
void like_snake::cobra_append(t_ponto ponto)
{
	if(pos_cobra.len < pixels)
	{
		pos_cobra.P[pos_cobra.len] = ponto;
		pos_cobra.len++;
	}	
}

//Debug: imprime um ponto na serial
void like_snake::print_ponto(t_ponto ponto)
{
#ifdef USAR_SERIAL
  Serial.print("P={"); Serial.print(ponto.x); Serial.print(","); Serial.print(ponto.y);Serial.println("}");
#endif
}
