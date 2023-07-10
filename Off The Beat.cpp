#include <graphics.h>
#include <stdio.h>
#include <windows.h>

// Define as teclas virtuais
#define VK_A 0x41
#define VK_D 0x44
#define VK_ESCAPE 0x1B

// Void usado para carregar imagens no jogo 
void* CarregarImagem(const char* Sprite, int x, int y, int altura, int largura)
{
    void* imagem;
    int aux = imagesize(x, y, largura, altura);
    imagem = malloc(aux);
    readimagefile(Sprite, x, y, largura, altura);
    getimage(x, y, largura, altura, imagem);
    return imagem;
}
int main()
{

    unsigned long contaTick; // Conta os Ticks da placa mãe
    unsigned long iniciaTick; // Sincroniza os ticks do jogo

    iniciaTick = GetTickCount();

    initwindow(450, 800, "off the beat");

    void* imagem = CarregarImagem(".\\Assets\\Off The Beat Title Screen.BMP", 0, 0, 800, 450); // carrega background
    void* botaoM = CarregarImagem(".\\Assets\\mascara.bmp", 250, 250, 100, 100); // mascara
    void* botao1 = CarregarImagem(".\\Assets\\jogar.bmp", 250, 250, 100, 100); // jogar
    void* botao2 = CarregarImagem(".\\Assets\\sair.bmp", 250, 250, 100, 100); // sair
    

    int x = getmaxx() / 2; // cordenada x do personagem
    int y = 600; // cordenada y do personagem
    int lane = 2; // lane inicial

    int enemy1x = 50; // posição X dos inimigos na lane 1
    int enemy1y = -60; // posição Y dos inimigos na lane 1

    int enemy2x = 200; // posição X dos inimigos da lane 2
    int enemy2y = -200; // posição Y dos inimigos da lane 2

    int enemy3x = 350; // posição X dos inimigos da lane 3
    int enemy3y = -190; // posição Y dos inimigos da lane 3

    int pg = 1;
    mciSendString("open .\\sons\\Menu.mp3 type MPEGVideo alias Menu", NULL, 0, 0); // Carrega musica do menu
	
    while (true) {

        contaTick = GetTickCount(); // começa a contagem de ticks
        if (iniciaTick - contaTick > 1000 / 60) //Contagem total de ticks da placa mãe 
            putimage(0, 0, imagem, COPY_PUT); // Desenha background
            putimage(150, 350, botaoM, AND_PUT); // MASCARA2
            putimage(150, 350, botao2, OR_PUT); // SAIR.BMP

            putimage(150, 250, botaoM, AND_PUT); // MASCARA
            putimage(150, 250, botao1, OR_PUT); // JOGAR.BMP

            mciSendString("seek Menu to start", NULL, 0, 0); // procura arquivo
            mciSendString("play Menu repeat", NULL, 0, 0); // Play

            // Input de usuario
            while (1) {
                int x, y;
                if (ismouseclick(WM_LBUTTONDOWN)) {

                    getmouseclick(WM_LBUTTONDOWN, x, y);
                    if (x >= 150 && x <= 250 && y >= 250 && y <= 350) {

                        // Começa o jogo
                        mciSendString("stop Menu", NULL, 0, 0); // Para a musica de menu
                        mciSendString("open .\\sons\\Jogo.mp3 type MPEGVideo alias Jogo", NULL, 0, 0); // carrega a musica do jogo
                        mciSendString("seek Jogo to start", NULL, 0, 0); // busca arquivo
                        mciSendString("play Jogo repeat", NULL, 0, 0); // Toca
                    	
                        break;
                    }
                    else if (x >= 150 && x <= 250 && y >= 350 && y <= 450) {
                        // Sair
                        closegraph();
                        return 0;
                    }
                }
            }
        }

        // Atualiza sprites
        int spriteCount = 5; // Calcula o numero de sprites
        char spriteNames[5][25] = {
            ".\\Assets\\sprite 0.bmp",
            ".\\Assets\\sprite 2.bmp",
            ".\\Assets\\sprite 4.bmp",
            ".\\Assets\\sprite 6.bmp",
            ".\\Assets\\sprite 8.bmp",
        };

        int currentSprite = 0; // Sprite atual

        bool gameRunning = true; // Variavel checa a musica
		
		//carrega imagem do gameover
		void* gameover = CarregarImagem(".\\Assets\\gameover.bmp", 0, 0, 800,450);
        // Carrega imagem do inimigo
        void* enemyImage = CarregarImagem(".\\Assets\\enemy.bmp", 0, 0, 50, 50);
        void* enemyMascara = CarregarImagem(".\\Assets\\enemy_Mascara.bmp", 0, 0, 50, 50);
        // Carrega imagem do Bit
        void* bitImage = CarregarImagem(".\\Assets\\Bit.bmp", 0, 0, 100, 100);
        void* bitMascara = CarregarImagem(".\\Assets\\Bit_Mascara.bmp", 0, 0, 100, 100);

        int score = 0; // Contador de hits

        while (gameRunning) {


			/////////////Paginação/////////////////

            if (pg == 1)
                pg = 2;
            else
                pg = 1;
            setactivepage(pg);
            putimage(0, 0, CarregarImagem(spriteNames[currentSprite], 0, 0, 800, 450), COPY_PUT); // muda sprite do background

            //////////////////////// Enemy ////////////////////////////////////////////////////
			
            putimage(enemy1x - 10, enemy1y - 10, enemyMascara, AND_PUT); // Limpa posição anterior
            putimage(enemy1x - 10, enemy1y - 10, enemyImage, OR_PUT);
            enemy1y += 15; // atualiza nova posição
            
        
            putimage(enemy2x - 10, enemy2y - 10, enemyMascara, AND_PUT); // Limpa posição anterior
            putimage(enemy2x - 10, enemy2y - 10, enemyImage, OR_PUT);
            enemy2y += 20; // atualiza nova posição


            putimage(enemy3x - 10, enemy3y - 10, enemyMascara, AND_PUT); // Limpa posição anterior
            putimage(enemy3x - 10, enemy3y - 10, enemyImage, OR_PUT);
            enemy3y += 10; // atualiza nova posição
			
            
            if (enemy1y >= getmaxy())  // Verificar se o primeiro objeto atingiu a parte inferior da tela
        	enemy1y = -130;  // Reiniciar a posição do primeiro objeto para a parte superior da tela

    		if (enemy2y >= getmaxy())  // Verificar se o segundo objeto atingiu a parte inferior da tela
        	enemy2y = -150;  // Reiniciar a posição do segundo objeto para a parte superior da tela
         
    		if (enemy3y >= getmaxy())  // Verificar se o terceiro objeto atingiu a parte inferior da tela
        	enemy3y = -70;  // Reiniciar a posição do terceiro objeto para a parte superior da tela
            

            /////////////////////// Main Character ////////////////////////////////////////

            putimage(x - 100 / 2, y - 100 / 2, bitMascara, AND_PUT);
            putimage(x - 100 / 2, y - 100 / 2, bitImage, OR_PUT);

            if (GetAsyncKeyState(VK_A) & 0x8000) { // tecla A
                if (lane > 0) {
                    lane--;
                    x = (lane + 1) * getmaxx() / 3 - getmaxx() / 6;
                    sndPlaySound(".\\sons\\Pulo.wav", SND_ASYNC);
                }
            }
            else if (GetAsyncKeyState(VK_D) & 0x8000) { // tecla D
                if (lane < 2) {
                    lane++;
                    x = (lane + 1) * getmaxx() / 3 - getmaxx() / 6;
                    sndPlaySound(".\\sons\\Pulo.wav", SND_ASYNC);
                }
            }
            else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { //Esc
                mciSendString("stop Jogo", NULL, 0, 0); // para musica
                gameRunning = false; // Checa musica
            }
            setvisualpage(pg); // Buffer Duplo

            //////////////////////////////////Detecta colisão/////////////////////////////////////////////////////

            // Checa colisão
            if (x >= enemy1x - 10 && x <= enemy1x + 40 && y >= enemy1y - 10 && y <= enemy1y + 40) {
                score++; // score de dano
                enemy1y = -60; // Reseta posição inimiga
                sndPlaySound(".\\sons\\Danomorte.wav", SND_ASYNC);
            }

           // Checa colisão
            if (x >= enemy2x - 10 && x <= enemy2x + 40 && y >= enemy2y - 10 && y <= enemy2y + 40) {
                score++; // score de dano
                enemy2y = -200;// Reseta posição inimiga
                sndPlaySound(".\\sons\\Danomorte.wav", SND_ASYNC);
            }

            // Checa colisão
            if (x >= enemy3x - 10 && x <= enemy3x + 40 && y >= enemy3y - 10 && y <= enemy3y + 40) {
                score++; // score de dano
                enemy3y = -190; // Reseta posição inimiga
                sndPlaySound(".\\sons\\Danomorte.wav", SND_ASYNC);
            }

            if (score >= 3) {
                mciSendString("stop Jogo", NULL, 0, 0); //para musica
                sndPlaySound(".\\sons\\fim.wav", SND_ASYNC);
            	putimage(0, 0, gameover, COPY_PUT); // Desenha gameover
 				delay(5000);   
                gameRunning = false; // Checa estado do jogo
            }

            //////////////////////////////////Delay De background///////////////////////////////////////////////////
            
            // atualiza background
            currentSprite = (currentSprite + 1) % spriteCount;
            
            delay(50); // Delay
            
            
        }	
        
		free(enemyImage);
		free(enemyMascara);
		free(bitImage);
		free(bitMascara);
			
    }
    
	free(imagem);
	free(botaoM);
	free(botao1);
	free(botao2);


    closegraph();
    return 0;
}
