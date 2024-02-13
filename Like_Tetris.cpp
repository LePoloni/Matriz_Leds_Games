/*
Desenvolvida por: Leandro Poloni Dantas
Data: Janeiro/2023
Sobre: Classe com métodos customizados para desenvolver um jogo como tetris
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
Tetris online: https://tetris.com/play-tetris
*/

#include "Like_Tetris.h"
//#define USAR_SERIAL

//Construtor
like_tetris::like_tetris(int16_t pino, uint8_t col, uint8_t lin, uint8_t tipo_mat)
						:Adafruit_NeoPixel(col*lin, pino, NEO_GRB + NEO_KHZ800)	//Construtor da classe herdada
{
  colunas = col;
  linhas = lin;
  pixels = col*lin;
  tipo_matriz = tipo_mat;
  posicao.x = 0;
  posicao.y = 0;
  pontos = 0;			//Pontuação inicial do jogo
	
	//Nenhum método da classe herdada deve ser executado aqui!
	//Não usar delay
	
	//Inicializa GPIO de debug
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);	

	//Inicializa a serial para debug
#ifdef USAR_SERIAL
  Serial.begin(9600);
	Serial.println("Like Tetris!");
#endif
}

//Teste
void like_tetris::pisca()
{
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
	delay(500);
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
	delay(500);
#ifdef USAR_SERIAL
  Serial.print("col="); Serial.println(colunas);
  Serial.print("lin="); Serial.println(linhas);
  Serial.print("tipo_mat=");

  switch(tipo_matriz)
  {
    case T_LINHA:           Serial.println("T_LINHA");          break;
    case T_LINHA_ZIG_ZAG:   Serial.println("T_LINHA_ZIG_ZAG");  break;
    case T_COLUNA:          Serial.println("T_COLUNA");         break;
    case T_COLUNA_ZIG_ZAG:  Serial.println("T_COLUNA_ZIG_ZAG"); break;
  }
#endif
}

//Retorna o número do pixel de um ponto
//Isso depende da forma construtiva da matriz
int like_tetris::num_pixel(t_ponto xy)
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
void like_tetris::limpa_matriz(uint32_t cor)
{
	fill(cor, 0, pixels);
	show();
	posicao.x = 0;
	posicao.y = 0;
}

//Desenha uma linha a parir de um ponto de origem até um ponto de destino
//Reporta a posição final
t_ponto like_tetris::linha_od(t_ponto origem, t_ponto destino, uint32_t cor_pixel, uint32_t tempo)
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
				delay(tempo);
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
				delay(tempo);
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
				delay(tempo);
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
				delay(tempo);
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
	//print(posicao);
  return posicao;
}

//Desenha uma linha a parir do último ponto desenhado até de deslocamento desejado
//Reporta a posição final
t_ponto like_tetris::linha_delta(t_ponto offset, uint32_t cor_pixel, uint32_t tempo)
{
    uint8_t destinox = posicao.x + offset.x;
    uint8_t destinoy = posicao.y + offset.y;
    t_ponto destino = {destinox, destinoy};
    //print_ponto(destino);
    return linha_od(posicao, destino, cor_pixel, tempo);
}

//Desenha um número na matriz
void like_tetris::desenha_numero(uint8_t numero, uint8_t tam, uint32_t cor, t_ponto ponto)
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
			linha_od(xy_o, xy_d, cor, 0);
			//delay(500);
		}
		x = x >> 1;
		s += 1;		
	}
}

//Desenha a pontuação atual na matriz
void like_tetris::desenha_pontuacao(uint32_t cor)
{
  int dezena = pontos/10;
	int unidade = pontos%10;
	desenha_numero(dezena, TAM_P, cor, {0,3});
	desenha_numero(unidade, TAM_P, cor, {5,3});
}

//Desloca um pixel do ponto de origem ao ponto de destino respeitando o tempo
//entre cada movimento
//Reporta a posição final
t_ponto like_tetris::desloca_od(t_ponto origem, t_ponto destino, uint32_t cor_fundo, uint32_t cor_pixel, uint32_t tempo)
{
	uint8_t ox, oy, dx, dy;
  int8_t deltax, deltay, mdeltax, mdeltay;
	ox = origem.x;
	oy = origem.y;
	dx = destino.x;
	dy = destino.y;
	
	//Calcula o delta de deslocamento em x e y
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
	
	//Desenha o pixel na origem
	//int pixel = ox + oy*colunas;
	uint16_t pixel = num_pixel(origem);
				
	setPixelColor(pixel, cor_pixel);
	show();
	delay(tempo);

	//Se o delocamento em x é maior que em y
	if (mdeltax > mdeltay)
	{
		//Se o ponto de origem em x é menor que o destino
		if (ox < dx)
		{
			//Enquanto não chegar no destino
			while (ox < dx)
			{
				//Apaga o pixel anterior
				setPixelColor(pixel, cor_fundo);
				show();
				//Desloca o ponto em x e y                                        
				ox = ox + 1;
				if (oy < dy)
					oy = oy + 1;
				else if (oy > dy)
					oy = oy - 1;
				//Calcula o número do pixel
				//pixel = ox + oy*colunas;
        pixel = num_pixel({ox,oy});
				//Desenha o pixel na nova posição
				setPixelColor(pixel, cor_pixel);
				show();
				delay(tempo);
			}
		}
		else
		{
			//Enquanto não chegar no destino
			while (ox > dx)
			{
				//Apaga o pixel anterior 
				setPixelColor(pixel, cor_fundo);
				show();
				//Desloca o ponto em x e y 
				ox = ox - 1;
				if (oy < dy)
					oy = oy+1;
				else if (oy > dy)
					oy = oy-1;
				//Calcula o número do pixel
				//pixel = ox + oy*colunas;
        pixel = num_pixel({ox,oy});
				//Desenha o pixel na nova posição
				setPixelColor(pixel, cor_pixel);
				show();
				delay(tempo);
			}
		}
	}
	//Se o delocamento em y é maior que em x
	else
	{		
		if (oy < dy)
		{			
			while (oy < dy)
			{
				setPixelColor(pixel, cor_fundo);
				show();

				oy = oy + 1;
				if (ox < dx)
					ox = ox+1;
				else if (ox > dx)
					ox = ox-1;
				
				//ixel = ox + oy*colunas;
        pixel = num_pixel({ox,oy});

				setPixelColor(pixel, cor_pixel);
				show();
				delay(tempo);
			}
		}
		else
		{
			while (oy > dy)
			{
				setPixelColor(pixel, cor_fundo);
				show();
				
				oy = oy - 1;
				if (ox < dx)
					ox = ox+1;
				else if (ox > dx)
					ox = ox-1;

				//pixel = ox + oy*colunas;
				pixel = num_pixel({ox,oy});

				setPixelColor(pixel, cor_pixel);
				show();
				delay(tempo);
			}
		}
	}
	//Atualiza a última posição desenhada
	posicao.x = ox;
	posicao.y = oy;

	//print_ponto(posicao);
	//Retorna a posição do último pixel desenhado
	return posicao;
}

//Desloca um pixel a parir do último ponto desenhado até de deslocamento desejado
//Reporta a posição final
t_ponto like_tetris::desloca_delta(t_ponto offset, uint32_t cor_fundo, uint32_t cor_pixel, uint32_t tempo)
{
	t_ponto destino = {0,0};
	uint8_t destinox = posicao.x+offset.x;
	uint8_t destinoy = posicao.y+offset.y;
	destino.x = destinox;
	destino.y = destinoy;
	//print_ponto(destino);
	return desloca_od(posicao, destino, cor_fundo, cor_pixel, tempo);
}

//Cria um bloco de tetris a partir de um ponto de origem, tipo e cor desejada
//Não desenha o bloco
//Retorna um bloco
t_bloco like_tetris::cria_bloco(t_ponto origem, uint8_t tipo, uint32_t cor)
{
	uint8_t ox = origem.x;
	uint8_t oy = origem.y;
					 				 //P1     P2     P3     P4     Cor Tipo Ponto Fixo
	t_bloco bloco = {{{0,0}, {0,0}, {0,0}, {0,0}}, 0,  0,   0};
	
	switch (tipo)
	{
		case B_TRACO:     //Traço
			bloco.P[0].x = 0+ox;
			bloco.P[0].y = 0+oy;
			bloco.P[1].x = 1+ox;
			bloco.P[1].y = 0+oy;
			bloco.P[2].x = 2+ox;
			bloco.P[2].y = 0+oy;
			bloco.P[3].x = 3+ox;
			bloco.P[3].y = 0+oy;
			bloco.cor = cor;
			bloco.tipo = B_TRACO;
			bloco.ponto_fixo = 1;
			break;
		case B_Z:     //Z
			bloco.P[0].x = 0+ox;
			bloco.P[0].y = 0+oy;
			bloco.P[1].x = 1+ox;
			bloco.P[1].y = 0+oy;
			bloco.P[2].x = 1+ox;
			bloco.P[2].y = 1+oy;
			bloco.P[3].x = 2+ox;
			bloco.P[3].y = 1+oy;
			bloco.cor = cor;
			bloco.tipo = B_Z;
			bloco.ponto_fixo = 1;
			break;
		case B_S:     //S
			bloco.P[0].x = 1+ox;
			bloco.P[0].y = 0+oy;
			bloco.P[1].x = 2+ox;
			bloco.P[1].y = 0+oy;
			bloco.P[2].x = 0+ox;
			bloco.P[2].y = 1+oy;
			bloco.P[3].x = 1+ox;
			bloco.P[3].y = 1+oy;
			bloco.cor = cor;
			bloco.tipo = B_S;
			bloco.ponto_fixo = 0;
			break;				
		case B_QUADRADO:     //Quadrado
			bloco.P[0].x = 0+ox;
			bloco.P[0].y = 0+oy;
			bloco.P[1].x = 1+ox;
			bloco.P[1].y = 0+oy;
			bloco.P[2].x = 0+ox;
			bloco.P[2].y = 1+oy;
			bloco.P[3].x = 1+ox;
			bloco.P[3].y = 1+oy;
			bloco.cor = cor;
			bloco.tipo = B_S;
			bloco.ponto_fixo = -1;	//Fixo (não gira)
			break;		
		case B_T:     //T
			bloco.P[0].x = 0+ox;
			bloco.P[0].y = 0+oy;
			bloco.P[1].x = 1+ox;
			bloco.P[1].y = 0+oy;
			bloco.P[2].x = 2+ox;
			bloco.P[2].y = 0+oy;
			bloco.P[3].x = 1+ox;
			bloco.P[3].y = 1+oy;
			bloco.cor = cor;
			bloco.tipo = B_T;
			bloco.ponto_fixo = 1;
			break;	
		case B_L:     //L
			bloco.P[0].x = 0+ox;
			bloco.P[0].y = 0+oy;
			bloco.P[1].x = 0+ox;
			bloco.P[1].y = 1+oy;
			bloco.P[2].x = 0+ox;
			bloco.P[2].y = 2+oy;
			bloco.P[3].x = 1+ox;
			bloco.P[3].y = 2+oy;
			bloco.cor = cor;
			bloco.tipo = B_T;
			bloco.ponto_fixo = 1;
			break;	
		case B_J:     //J
			bloco.P[0].x = 1+ox;
			bloco.P[0].y = 0+oy;
			bloco.P[1].x = 1+ox;
			bloco.P[1].y = 1+oy;
			bloco.P[2].x = 0+ox;
			bloco.P[2].y = 2+oy;
			bloco.P[3].x = 1+ox;
			bloco.P[3].y = 2+oy;
			bloco.cor = cor;
			bloco.tipo = B_T;
			bloco.ponto_fixo = 1;
			break;	
	}
	///Retorna uma lista que representa o bloco
	return bloco;
}

//Desenha um bloco
//Retorna True para sucesso e False para falha
bool like_tetris::desenha_bloco(t_bloco bloco)
{
	//Verifica se está fora dos limites da matriz
	if ((num_pixel(bloco.P[0]) >= pixels) ||
	    (num_pixel(bloco.P[1]) >= pixels) ||
	    (num_pixel(bloco.P[2]) >= pixels) ||
	    (num_pixel(bloco.P[3]) >= pixels))
		return false;
					   //posição     			cor
	setPixelColor(num_pixel(bloco.P[0]), bloco.cor);
	setPixelColor(num_pixel(bloco.P[1]), bloco.cor);
	setPixelColor(num_pixel(bloco.P[2]), bloco.cor);
	setPixelColor(num_pixel(bloco.P[3]), bloco.cor);
	show();
	
	return true;
}
    
//Apaga um bloco
void like_tetris::apaga_bloco(t_bloco bloco, uint32_t cor_fundo)
{
	//novo_bloco = bloco #Não pode ser usado, funciona como ponteiro
					      //P1     P2     P3     P4     Cor Tipo Ponto Fixo
	t_bloco novo_bloco = {{{0,0}, {0,0}, {0,0}, {0,0}}, 0,  0,   0};
	
	//Atualiza todos os pixels do novo_bloco com o bloco original, menos a cor
	novo_bloco = bloco;
	novo_bloco.cor = cor_fundo;
	
	desenha_bloco(novo_bloco);
}

//Desloca um bloco de acordo com o offset (x,y) desejado
//Não desenha o bloco, nem verifa os limites da matriz
//Retorna um bloco
t_bloco like_tetris::desloca_bloco(t_bloco bloco, t_ponto offset)
{
	uint8_t ox = offset.x;
	uint8_t oy = offset.y;
					  //P1     P2     P3     P4     Cor Tipo Ponto Fixo
	t_bloco bloco2 = {{{0,0}, {0,0}, {0,0}, {0,0}}, 0,  0,   0};

	//Atualiza todos os pixels do bloco com o offset
	bloco2.P[0].x = bloco.P[0].x + offset.x;
	bloco2.P[0].y = bloco.P[0].y + offset.y;
	bloco2.P[1].x = bloco.P[1].x + offset.x;
	bloco2.P[1].y = bloco.P[1].y + offset.y;
	bloco2.P[2].x = bloco.P[2].x + offset.x;
	bloco2.P[2].y = bloco.P[2].y + offset.y;
	bloco2.P[3].x = bloco.P[3].x + offset.x;
	bloco2.P[3].y = bloco.P[3].y + offset.y;	
	bloco2.cor = bloco.cor;
	bloco2.tipo = bloco.tipo;
	bloco2.ponto_fixo = bloco.ponto_fixo;
		
	//Retorna o bloco deslocado
	return bloco2;
}
    
//Verifica se as coordenadas do bloco passado então na área da matriz e se
//todos o pixel tem cor igual a cor de fundo passada
//Esse método serve para verificar se um bloco pode ocupar uma posição nova
//Retorna true para sucesso e false para falha
bool like_tetris::testa_ocupacao(t_bloco bloco, uint32_t cor_fundo)
{
	//print('bloco =', bloco)
	//print(cor_fundo)
	
	//Verifica se está fora dos limites da matriz
	if ((num_pixel(bloco.P[0]) >= pixels) ||
	    (num_pixel(bloco.P[1]) >= pixels) ||
	    (num_pixel(bloco.P[2]) >= pixels) ||
	    (num_pixel(bloco.P[3]) >= pixels))
		//Saiu dos limites
		//print('Ocupação=True')  
		return true;

	//Se na posição de todos os pixels do bloco a cor é igual a cor de fundo
	if ((getPixelColor(num_pixel(bloco.P[0])) == cor_fundo) &&
	    (getPixelColor(num_pixel(bloco.P[1])) == cor_fundo) &&
		(getPixelColor(num_pixel(bloco.P[2])) == cor_fundo) &&
		(getPixelColor(num_pixel(bloco.P[3])) == cor_fundo))
		//A posição não está ocupara
		//print('Ocupação=False')  
		return false;
	else
		//A posição está ocupara
		//print('True')            
		return true;
}

//Tenta deslocar um bloco uma linha para baixo
//Retorna True para sucesso e false para falha
//Retorna um bloco reposicionado
bool like_tetris::movimento_completo(t_bloco bloco, uint32_t cor_fundo, t_bloco *bloco_ret)
{
					             //P1     P2     P3     P4     Cor Tipo Ponto Fixo
	t_bloco bloco2    = {{{0,0}, {0,0}, {0,0}, {0,0}}, 0,  0,   0};
	t_ponto desloca = {0,1};
	
	//Atualiza todos os pixels do bloco_ret com o bloco original
	*bloco_ret = bloco;
	
	//Serial.println('bloco');
  //print_bloco(bloco);

	//Se ultrapassou a última linha
	if ((bloco.P[0].y >= linhas) ||
	    (bloco.P[1].y >= linhas) ||
	    (bloco.P[2].y >= linhas) ||
	    (bloco.P[3].y >= linhas))
	{
		//Retorna o bloco original
#ifdef USAR_SERIAL
		Serial.println("Ultrapassou a última linha");
#endif
		return false;
	}
	//Desloca y+1
	bloco2 = desloca_bloco(bloco, desloca);
	
	//Serial.println('bloco2');
  //print_bloco(bloco2);
	
	//Apaga bloco1
	apaga_bloco(bloco, cor_fundo);
	
	//Testa
	if (testa_ocupacao(bloco2, cor_fundo) == false)
	{
		//Desenha bloco2
		desenha_bloco(bloco2);
		//Retorna o estado e o bloco atualizado
		//Atualiza todos os pixels do bloco_ret com o bloco original
		*bloco_ret = bloco2;
#ifdef USAR_SERIAL
		Serial.println("return true");
    Serial.print("bloco_ret.P[0].y="); Serial.println(bloco_ret->P[0].y);
#endif		
    return true;
	}			
	else
	{
		//Desenha bloco1 novamente
		desenha_bloco(bloco);
		//Retorna o estado e o bloco original
#ifdef USAR_SERIAL
    Serial.println("return false");
#endif
		return false;
	}
}

//Rotaciona um bloco 90 graus em sentido horário
//Retorna um bloco
t_bloco like_tetris::rotaciona_bloco(t_bloco bloco)
{
					             //P1     P2     P3     P4     Cor Tipo Ponto Fixo
	t_bloco bloco_ret = {{{0,0}, {0,0}, {0,0}, {0,0}}, 0,  0,   0};
	t_bloco bloco_rot = {{{0,0}, {0,0}, {0,0}, {0,0}}, 0,  0,   0};
	
	//Atualiza todos os pixels do bloco_ret com o bloco original
	bloco_ret = bloco;

  bloco_rot.cor = bloco.cor;
	bloco_rot.tipo = bloco.tipo;
	bloco_rot.ponto_fixo = bloco.ponto_fixo;
	//print_bloco(bloco);

	//Se for um quadrado, não precisa rotacionar
	if (bloco.tipo == B_QUADRADO)
		return bloco_ret;

	//1 - Ponto-Referência (Ponto Fixo)
	uint8_t ref = bloco.ponto_fixo;
	bloco_rot.P[0].x = bloco.P[0].x - bloco.P[ref].x;
	bloco_rot.P[0].y = bloco.P[0].y - bloco.P[ref].y;
	bloco_rot.P[1].x = bloco.P[1].x - bloco.P[ref].x;
	bloco_rot.P[1].y = bloco.P[1].y - bloco.P[ref].y;
	bloco_rot.P[2].x = bloco.P[2].x - bloco.P[ref].x;
	bloco_rot.P[2].y = bloco.P[2].y - bloco.P[ref].y;
	bloco_rot.P[3].x = bloco.P[3].x - bloco.P[ref].x;
	bloco_rot.P[3].y = bloco.P[3].y - bloco.P[ref].y;
	//Serial.println("bloco_rot1");
	//print_bloco(bloco_rot);
	
	//2 - Inverte x e y
	uint8_t aux = bloco_rot.P[0].x;
	bloco_rot.P[0].x = bloco_rot.P[0].y;
	bloco_rot.P[0].y = aux;
	
	aux = bloco_rot.P[1].x;
	bloco_rot.P[1].x = bloco_rot.P[1].y;
	bloco_rot.P[1].y = aux;
	
	aux = bloco_rot.P[2].x;
	bloco_rot.P[2].x = bloco_rot.P[2].y;
	bloco_rot.P[2].y = aux;
	
	aux = bloco_rot.P[3].x;
	bloco_rot.P[3].x = bloco_rot.P[3].y;
	bloco_rot.P[3].y = aux;
	//Serial.println("bloco_rot2");
	//print_bloco(bloco_rot);
	
	//3 - Multiplica x por -1
	bloco_rot.P[0].x = -bloco_rot.P[0].x;
	bloco_rot.P[1].x = -bloco_rot.P[1].x;
	bloco_rot.P[2].x = -bloco_rot.P[2].x;
	bloco_rot.P[3].x = -bloco_rot.P[3].x;
	//Serial.println("bloco_rot3");
	//print_bloco(bloco_rot);
	
	//4 - Ponto+Referência
	bloco_rot.P[0].x = bloco_rot.P[0].x + bloco.P[ref].x;
	bloco_rot.P[0].y = bloco_rot.P[0].y + bloco.P[ref].y;
	bloco_rot.P[1].x = bloco_rot.P[1].x + bloco.P[ref].x;
	bloco_rot.P[1].y = bloco_rot.P[1].y + bloco.P[ref].y;
	bloco_rot.P[2].x = bloco_rot.P[2].x + bloco.P[ref].x;
	bloco_rot.P[2].y = bloco_rot.P[2].y + bloco.P[ref].y;
	bloco_rot.P[3].x = bloco_rot.P[3].x + bloco.P[ref].x;
	bloco_rot.P[3].y = bloco_rot.P[3].y + bloco.P[ref].y;
	//Serial.println("bloco_rot4");
	
	//5 - Retorna o novo bloco
	return bloco_rot;
}

//Ajusta o bloco para não ficar partido em duas partes após rotacionar
//Retorna um bloco
t_bloco like_tetris::ajusta_posicao_bloco(t_bloco bloco)
{
	t_ponto desloca = {0,0};
	
	//Ajusta o bloco para não ficar partido em duas partes após rotacionar
					           //P1     P2     P3     P4     Cor Tipo Ponto Fixo
	t_bloco bloco_aj = {{{0,0}, {0,0}, {0,0}, {0,0}}, 0,  0,   0};
	
	//Atualiza todos os pixels do bloco_aj com o bloco original
	bloco_aj = bloco;
	//Ponto-Referência (Ponto Fixo)
	uint8_t ref = bloco.ponto_fixo;
	//Ajuste feito
	bool ok = false;
	//Loop de ajuste
	while (ok == false)
	{
		ok = true;
		//Se a posição de um pixel é maior que o número de colunas ou menor que 0,
		//é sinal que houve partição do bloco
		//Deslocamento negativo em x
		if ((bloco_aj.P[0].x >= colunas) ||
		    (bloco_aj.P[1].x >= colunas) ||
		    (bloco_aj.P[2].x >= colunas) ||
		    (bloco_aj.P[3].x >= colunas))
		{
			desloca.x = -1;
			desloca.y = 0;
			bloco_aj = desloca_bloco(bloco_aj, desloca);
			ok = false;
			//Serial.println("FalseX+");
			//Serial.println("bloco_aj");
			//print_bloco(bloco_aj);
			//delay(1000)
		}
		//Deslocamento positivo em x
		else if ((bloco_aj.P[0].x < 0) ||
		         (bloco_aj.P[1].x < 0) ||
				 		 (bloco_aj.P[2].x < 0) ||
				 		 (bloco_aj.P[3].x < 0))
		{
			desloca.x = 1;
			desloca.y = 0;
			bloco_aj = desloca_bloco(bloco_aj, desloca);
			ok = false;
			//Serial.println("FalseX-");
			//Serial.println("bloco_aj");
			//print_bloco(bloco_aj);
			//delay(1000);
		}
		//Se a posição de um pixel é maior que o número de linhas ou menor que 0,
		//é sinal que houve partição do bloco
		//Deslocamento negativo em y
		if ((bloco_aj.P[0].y >= linhas) ||
		    (bloco_aj.P[1].y >= linhas) ||
		    (bloco_aj.P[2].y >= linhas) ||
		    (bloco_aj.P[3].y >= linhas))
		{
			desloca.x = 0;
			desloca.y = -1;
			bloco_aj = desloca_bloco(bloco_aj, desloca);
			ok = false;
			//Serial.println("FalseX+");
			//Serial.println("bloco_aj");
			//print_bloco(bloco_aj);
			//delay(1000);
		}
		//Deslocamento positivo em y
		else if ((bloco_aj.P[0].y < 0) ||
		         (bloco_aj.P[1].y < 0) ||
				 (bloco_aj.P[2].y < 0) ||
				 (bloco_aj.P[3].y < 0))
		{
			desloca.x = 0;
			desloca.y = 1;
			bloco_aj = desloca_bloco(bloco_aj, desloca);
			ok = false;
			//Serial.println("FalseX-");
			//Serial.println("bloco_aj");
			//print_bloco(bloco_aj);
			//delay(1000);
		}
	}
	//Retorna o bloco ajustado
	return bloco_aj;
}

//Verifica se uma linha foi preenchida. Em caso positivo apaga seu
//conteúdo de desloca as demais para baixo
void like_tetris::testa_preenchimento_linha(uint32_t cor_fundo)
{
	t_ponto pixel = {0,0};
	
	//Varre as linhas da última para primeira
	int8_t l = linhas - 1;
	while (l >= 0)
	{
#ifdef USAR_SERIAL
		Serial.print(">>>> l = "); Serial.println(l);
#endif			

		//1-Verifica se a linha está preenchida
		bool igual = true;
		for (int c = 0; c < colunas; c++)
		{
			//Se uma dos pixel for igual a cor do fundo
			pixel.x = c; pixel.y = l;
			if (getPixelColor(num_pixel(pixel)) == cor_fundo)
			{
				//A linha não está preenchida
				igual = false;
			}
		}
		//2-Se a linha está preenchida
		if (igual == true)
		{
			//3-Apaga a linha
			for (int c = 0; c < colunas; c++)
			{
				pixel.x = c; pixel.y = l;
				setPixelColor(num_pixel(pixel), cor_fundo);
			}
			//Atualiza a matriz
			show();
			delay(300);
			
			//4-Desloca o conteúdo das linhas anteriores um linha para baixo
			//Salva o valor da linha atual
			uint8_t l_igual = l;               
			//Ajusta todas as linhas anteriores
			while (l_igual > 0)
			{
#ifdef USAR_SERIAL			
				Serial.print(">>>> l_igual = "); Serial.println(l_igual);
#endif					
				for (int c = 0; c < colunas; c++)
				{
					//Lê a cor da linha anterior
					pixel.x = c; pixel.y = l_igual-1;					
					uint32_t cor_pixel_linha_anterior = getPixelColor(num_pixel(pixel));
					//Salva a cor na linha atual
					pixel.x = c; pixel.y = l_igual;
					setPixelColor(num_pixel(pixel), cor_pixel_linha_anterior);
				}
				l_igual -= 1;
			}
			//Preenche a linha 0 com a cor de fundo
			for (int c = 0; c < colunas; c++)
			{
				pixel.x = c; pixel.y = 0;
				setPixelColor(num_pixel(pixel), cor_fundo);
			}
			//Atualiza a matriz
			show();
			delay(300);
			
			//5-Após deslocar todas é linhas é preciso voltar e testar a linha atual novamente,
			//porque a linha anterior que foi deslocada também pode ter sido preenchida
			l += 1;
		}
		//6-Decrementa uma linha (vai para próxima linha)
		l -= 1;
	}
}

//Debug: imprime um bloco na serial
void like_tetris::print_bloco(t_bloco bloco)
{
#ifdef USAR_SERIAL
  Serial.print("P[0]="); Serial.print(bloco.P[0].x); Serial.print(","); Serial.println(bloco.P[0].y);
  Serial.print("P[1]="); Serial.print(bloco.P[1].x); Serial.print(","); Serial.println(bloco.P[1].y);
  Serial.print("P[2]="); Serial.print(bloco.P[2].x); Serial.print(","); Serial.println(bloco.P[2].y);
  Serial.print("P[3]="); Serial.print(bloco.P[3].x); Serial.print(","); Serial.println(bloco.P[3].y);
  Serial.print("cor="); Serial.println(bloco.cor);
  Serial.print("tipo="); Serial.println(bloco.tipo);
  Serial.print("ponto_fixo="); Serial.println(bloco.ponto_fixo);
#endif
}

//Debug: imprime um ponto na serial
void like_tetris::print_ponto(t_ponto ponto)
{
#ifdef USAR_SERIAL
  Serial.print("P={"); Serial.print(ponto.x); Serial.print(","); Serial.print(ponto.y);Serial.println("}");
#endif
}
