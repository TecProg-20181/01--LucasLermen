#include <stdio.h>

#define CORMAXIMAGEM 255
#define DIMENSAOIMAGEM 512

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [largura][altura]
    Pixel pixel[DIMENSAOIMAGEM][DIMENSAOIMAGEM];
    unsigned int largura;
    unsigned int altura;
} Imagem;


int maiorNum(int a, int b) {
    if (a > b){
      return a;
    }
    return b;
}


int menorNum(int a, int b) {
    if (a < b){
      return a;
    }
    return b;
}

Imagem escalaCinza(Imagem img) {
  for (unsigned int i = 0; i < img.altura; ++i) {
    for (unsigned int j = 0; j < img.largura; ++j) {
      int media = img.pixel[i][j].r +
                  img.pixel[i][j].g +
                  img.pixel[i][j].b;
      media /= 3;
      img.pixel[i][j].r = media;
      img.pixel[i][j].g = media;
      img.pixel[i][j].b = media;
    }
  }
    return img;
}

Imagem sepia(Imagem img) {
  for (unsigned int x = 0; x < img.altura; ++x) {
    for (unsigned int j = 0; j < img.largura; ++j) {
      unsigned short int pixel[3];
      pixel[0] = img.pixel[x][j].r;
      pixel[1] = img.pixel[x][j].g;
      pixel[2] = img.pixel[x][j].b;

      int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
      int menor_r = menorNum(p,CORMAXIMAGEM);
      img.pixel[x][j].r = menor_r;

      p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
      menor_r = menorNum(p,CORMAXIMAGEM);
      img.pixel[x][j].g = menor_r;

      p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
      menor_r = menorNum(p,CORMAXIMAGEM);
      img.pixel[x][j].b = menor_r;
    }
  }
  return img;
}

Imagem blur(Imagem img) {
  int tamanhoBorrao = 0;
  scanf("%d", &tamanhoBorrao);

  for (unsigned int i = 0; i < img.altura; ++i) {
    for (unsigned int j = 0; j < img.largura; ++j) {

      Pixel media = {0, 0, 0};

      int menor_altura = menorNum(img.altura - 1, i + tamanhoBorrao/2);
      int min_largura = menorNum(img.largura - 1 , j + tamanhoBorrao/2 );

      for(unsigned int x = maiorNum(0, i - tamanhoBorrao/2); x <= menor_altura; ++x) {
        for(unsigned int y = maiorNum(0 , j - tamanhoBorrao/2); y <= min_largura; ++y) {
          media.r += img.pixel[x][y].r;
          media.g += img.pixel[x][y].g;
          media.b += img.pixel[x][y].b;
        }
      }

      media.r /= tamanhoBorrao * tamanhoBorrao;
      media.g /= tamanhoBorrao * tamanhoBorrao;
      media.b /= tamanhoBorrao * tamanhoBorrao;

      img.pixel[i][j].r = media.r;
      img.pixel[i][j].g = media.g;
      img.pixel[i][j].b = media.b;
    }
  }
  return img;
}

Imagem rotacionaDireita(Imagem img) {
  Imagem rotacionada;

  rotacionada.largura = img.altura;
  rotacionada.altura = img.largura;

  int qteRotacoes = 0;
  scanf("%d", &qteRotacoes);
  qteRotacoes %= 4;
  for (int j = 0; j < qteRotacoes; ++j) {
    for (unsigned int i = 0, y = 0; i < rotacionada.altura; ++i, ++y) {
      for (int j = rotacionada.largura - 1, x = 0; j >= 0; --j, ++x) {
        rotacionada.pixel[i][j].r = img.pixel[x][y].r;
        rotacionada.pixel[i][j].g = img.pixel[x][y].g;
        rotacionada.pixel[i][j].b = img.pixel[x][y].b;
      }
    }
}
  return rotacionada;
}

Imagem espelhamento(Imagem img) {

  int horizontal = 0;
  scanf("%d", &horizontal);

  int largura = img.largura, altura = img.altura;

  if (horizontal == 1){
    largura /= 2;
  } else{
    altura /= 2;
  }

  for (int i = 0; i < altura; ++i) {
    for (int j = 0; j < largura; ++j) {
      int x = i, y = j;
      if (horizontal == 1){
        y = img.largura - 1 - j;
      } else{
        x = img.altura - 1 - i;
      }

      Pixel aux1;
      aux1.r = img.pixel[i][j].r;
      aux1.g = img.pixel[i][j].g;
      aux1.b = img.pixel[i][j].b;

      img.pixel[i][j].r = img.pixel[x][y].r;
      img.pixel[i][j].g = img.pixel[x][y].g;
      img.pixel[i][j].b = img.pixel[x][y].b;

      img.pixel[x][y].r = aux1.r;
      img.pixel[x][y].g = aux1.g;
      img.pixel[x][y].b = aux1.b;
    }
  }
  return img;
}

Imagem inverteCores(Imagem img) {
  for (unsigned int i = 0; i < img.altura; ++i) {
    for (unsigned int j = 0; j < img.largura; ++j) {
      img.pixel[i][j].r = CORMAXIMAGEM - img.pixel[i][j].r;
      img.pixel[i][j].g = CORMAXIMAGEM - img.pixel[i][j].g;
      img.pixel[i][j].b = CORMAXIMAGEM - img.pixel[i][j].b;
    }
  }
  return img;
}

Imagem cortaImagem(Imagem img) {
  Imagem cortada;

  int corteX, corteY;
  scanf("%d %d", &corteX, &corteY);
  int largura, altura;
  scanf("%d %d", &largura, &altura);

  cortada.largura = largura;
  cortada.altura = altura;

  for(int i = 0; i < altura; ++i) {
    for(int j = 0; j < largura; ++j) {
      cortada.pixel[i][j].r = img.pixel[i + corteY][j + corteX].r;
      cortada.pixel[i][j].g = img.pixel[i + corteY][j + corteX].g;
      cortada.pixel[i][j].b = img.pixel[i + corteY][j + corteX].b;
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
            scanf("%hu %hu %hu", &img.pixel[i][j].r,
                                 &img.pixel[i][j].g,
                                 &img.pixel[i][j].b);

        }
    }

    int numOpcoes;
    scanf("%d", &numOpcoes);

    for(int i = 0; i < numOpcoes; ++i) {
      int opcao;
      scanf("%d", &opcao);

      switch(opcao) {
        case 1: {
          img = escalaCinza(img);
          break;
        }
        case 2: {
          img = sepia(img);
          break;
        }
        case 3: {
          img = blur(img);
          break;
        }
        case 4: {
          img = rotacionaDireita(img);
          break;
        }
        case 5: {
          img = espelhamento(img);
          break;
        }
        case 6: {
          img = inverteCores(img);
          break;
        }
        case 7: {
          img = cortaImagem(img);
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
            printf("%hu %hu %hu ", img.pixel[i][j].r,
                                   img.pixel[i][j].g,
                                   img.pixel[i][j].b);

        }
        printf("\n");
    }
    return 0;
}
