#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [largura][altura][rgb]
    // 0 -> r - Vermelho
    // 1 -> g - Verde
    // 2 -> b - Azul
    unsigned short int pixel[512][512][3];
    unsigned int largura;
    unsigned int altura;
} Imagem;

Imagem escalaCinza(Imagem img) {
    for (unsigned int i = 0; i < img.altura; ++i) {
        for (unsigned int j = 0; j < img.largura; ++j) {
            int media = img.pixel[i][j][0] +
                        img.pixel[i][j][1] +
                        img.pixel[i][j][2];
            media /= 3;
            img.pixel[i][j][0] = media;
            img.pixel[i][j][1] = media;
            img.pixel[i][j][2] = media;
        }
    }

    return img;
}

void blur(unsigned int altura, unsigned short int pixel[512][512][3], int T, unsigned int largura) {
    for (unsigned int i = 0; i < altura; ++i) {
        for (unsigned int j = 0; j < largura; ++j) {
            Pixel media = {0, 0, 0};

            int menor_altura = (altura - 1 > i + T/2) ? i + T/2 : altura - 1;
            int min_largura = (largura - 1 > j + T/2) ? j + T/2 : largura - 1;
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_altura; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_largura; ++y) {
                    media.r += pixel[x][y][0];
                    media.g += pixel[x][y][1];
                    media.b += pixel[x][y][2];
                }
            }

            // printf("%u", media.r)
            media.r /= T * T;
            media.g /= T * T;
            media.b /= T * T;

            pixel[i][j][0] = media.r;
            pixel[i][j][1] = media.g;
            pixel[i][j][2] = media.b;
        }
    }
}

Imagem rotacionarDireita(Imagem img) {
    Imagem rotacionada;

    rotacionada.largura = img.altura;
    rotacionada.altura = img.largura;

    for (unsigned int i = 0, y = 0; i < rotacionada.altura; ++i, ++y) {
        for (int j = rotacionada.largura - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
            rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
            rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }

    return rotacionada;
}

void inverterCores(unsigned short int pixel[512][512][3],
                    unsigned int largura, unsigned int altura) {
    for (unsigned int i = 0; i < altura; ++i) {
        for (unsigned int j = 0; j < largura; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

Imagem cortarImagem(Imagem img, int corteX, int corteY, int largura, int altura) {
    Imagem cortada;

    cortada.largura = largura;
    cortada.altura = altura;

    for(int i = 0; i < altura; ++i) {
        for(int j = 0; j < largura; ++j) {
            cortada.pixel[i][j][0] = img.pixel[i + corteY][j + corteX][0];
            cortada.pixel[i][j][1] = img.pixel[i + corteY][j + corteX][1];
            cortada.pixel[i][j][2] = img.pixel[i + corteY][j + corteX][2];
        }
    }

    return cortada;
}


int main() {
    Imagem img;

    // le o tipo da imagem
    char p3[4];
    scanf("%s", p3);

    // le a largura, altura e cor maxima da imagem
    int maxCor;
    scanf("%u %u %d", &img.largura, &img.altura, &maxCor);

    // le todos os pixels da imagem
    for (unsigned int i = 0; i < img.altura; ++i) {
        for (unsigned int j = 0; j < img.largura; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][0],
                                 &img.pixel[i][j][1],
                                 &img.pixel[i][j][2]);

        }
    }

    int numOpcoes;
    scanf("%d", &numOpcoes);

    for(int i = 0; i < numOpcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escalaCinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                for (unsigned int x = 0; x < img.altura; ++x) {
                    for (unsigned int j = 0; j < img.largura; ++j) {
                        unsigned short int pixel[3];
                        pixel[0] = img.pixel[x][j][0];
                        pixel[1] = img.pixel[x][j][1];
                        pixel[2] = img.pixel[x][j][2];

                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                        int menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][0] = menor_r;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][1] = menor_r;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][2] = menor_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.altura, img.pixel, tamanho, img.largura);
                break;
            }
            case 4: { // Rotacao
                int qteRotacoes = 0;
                scanf("%d", &qteRotacoes);
                qteRotacoes %= 4;
                for (int j = 0; j < qteRotacoes; ++j) {
                    img = rotacionarDireita(img);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int largura = img.largura, altura = img.altura;

                if (horizontal == 1) largura /= 2;
                else altura /= 2;

                for (int i = 0; i < altura; ++i) {
                    for (int j = 0; j < largura; ++j) {
                        int x = i, y = j;

                        if (horizontal == 1) y = img.largura - 1 - j;
                        else x = img.altura - 1 - i;

                        Pixel aux1;
                        aux1.r = img.pixel[i][j][0];
                        aux1.g = img.pixel[i][j][1];
                        aux1.b = img.pixel[i][j][2];

                        img.pixel[i][j][0] = img.pixel[x][y][0];
                        img.pixel[i][j][1] = img.pixel[x][y][1];
                        img.pixel[i][j][2] = img.pixel[x][y][2];

                        img.pixel[x][y][0] = aux1.r;
                        img.pixel[x][y][1] = aux1.g;
                        img.pixel[x][y][2] = aux1.b;
                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                inverterCores(img.pixel, img.largura, img.altura);
                break;
            }
            case 7: { // Cortar Imagem
                int corteX, corteY;
                scanf("%d %d", &corteX, &corteY);
                int largura, altura;
                scanf("%d %d", &largura, &altura);

                img = cortarImagem(img, corteX, corteY, largura, altura);
                break;
            }
        }

    }

    // printa o tipo de imagem
    printf("P3\n");
    // printa largura altura e cor da imagem
    printf("%u %u\n255\n", img.largura, img.altura);

    // print pixels of image
    for (unsigned int i = 0; i < img.altura; ++i) {
        for (unsigned int j = 0; j < img.largura; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][0],
                                   img.pixel[i][j][1],
                                   img.pixel[i][j][2]);

        }
        printf("\n");
    }
    return 0;
}
