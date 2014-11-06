#include "GIFencoder.h"
#include "list.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

//Conversao de um objecto do tipo Image numa imagem indexada
imageStruct* GIFEncoder(unsigned char *data, int width, int height) {
    imageStruct* image = (imageStruct*)malloc(sizeof(imageStruct));
    image->width = width;
    image->height = height;

    //Converter para imagem indexada
    RGB2Indexed(data, image);

    return image;
}

//Conversao de lista RGB para indexada: máximo de 256 cores
void RGB2Indexed(unsigned char *data, imageStruct* image) {
    int x, y, colorIndex, colorNum = 0;
    char *copy;

    image->pixels = (char*)calloc(image->width*image->height, sizeof(char));
    image->colors = (char*)calloc(MAX_COLORS * 3, sizeof(char));


    for (x = 0; x < image->width; x++) {
        for (y = 0; y < image->height; y++) {
            for (colorIndex = 0; colorIndex < colorNum; colorIndex++) {
                if (image->colors[colorIndex * 3] == (char)data[(y * image->width + x)*3] && 
                        image->colors[colorIndex * 3 + 1] == (char)data[(y * image->width + x)*3 + 1] &&
                        image->colors[colorIndex * 3 + 2] == (char)data[(y * image->width + x)*3 + 2])
                    break;
            }

            if (colorIndex >= MAX_COLORS) {
                printf("Demasiadas cores...\n");
                exit(1);
            }

            image->pixels[y * image->width + x] = (char)colorIndex;

            if (colorIndex == colorNum) 
            {
                image->colors[colorIndex * 3]	  = (char)data[(y * image->width + x)*3];
                image->colors[colorIndex * 3 + 1] = (char)data[(y * image->width + x)*3 + 1];
                image->colors[colorIndex * 3 + 2] = (char)data[(y * image->width + x)*3 + 2];
                colorNum++;
            }
        }
    }

    //Define o número de cores como potência de 2 (devido aos requistos da Global Color Table)
    image->numColors = nextPower2(colorNum);

    //Refine o array de cores com base no número final obtido
    copy = (char*)calloc(image->numColors*3, sizeof(char));
    memset(copy, 0, sizeof(char)*image->numColors*3);
    memcpy(copy, image->colors, sizeof(char)*colorNum*3);
    image->colors = copy;

    image->minCodeSize = numBits(image->numColors - 1);
    if (image->minCodeSize == (char)1)  //Imagens binárias --> caso especial (pág. 26 do RFC)
        image->minCodeSize++;
}

//Determinação da próxima potência de 2 de um dado inteiro n
int nextPower2(int n) {
    int ret = 1, nIni = n;

    if (n == 0)
        return 0;

    while (n != 0) {
        ret *= 2;
        n /= 2;
    }

    if (ret % nIni == 0)
        ret = nIni;

    return ret;
}

//Número de bits necessário para representar n
char numBits(int n) {
    char nb = 0;

    if (n == 0)
        return 0;

    while (n != 0) {
        nb++;
        n /= 2;
    }

    return nb;
}

//Função para escrever imagem no formato GIF, versão 87a
void GIFEncoderWrite (imageStruct* image, char* outputFile) {
    FILE* file = fopen(outputFile, "wb");
    char trailer;

    //Escrever cabeçalho do GIF
    writeGIFHeader(image, file);

    //Escrever cabeçalho do Image Block
    // CRIAR FUN‚ÌO para ESCRITA do IMAGE BLOCK HEADER!!!
    //Sugest‹o da assinatura do mŽtodo a chamar:
    writeImageBlockHeader(image, file);

    /////////////////////////////////////////
    //Escrever blocos com 256 bytes no m‡ximo
    /////////////////////////////////////////
    //CODIFICADOR LZW AQUI !!!! 
    //Sugest‹o de assinatura do mŽtodo a chamar:
    LZWCompress(file, image->minCodeSize, *(image->pixels), image->width*image->height);

    fprintf(file, "%c", (char)0);

    //Trailer
    trailer = 0x3b;
    fprintf(file, "%c", trailer);

    fclose(file);
}

//Gravar cabeçalho do GIF (até global color table)
void writeGIFHeader (imageStruct* image, FILE* file) {
    int i;
    char toWrite, GCTF, colorRes, SF, sz, bgci, par;

    //Assinatura e versão (GIF87a)
    char* s = "GIF87a";
    for (i = 0; i < (int)strlen(s); i++)
        fprintf(file, "%c", s[i]);	

    //Ecrã lógico (igual à da dimensão da imagem) --> primeiro o LSB e depois o MSB
    fprintf(file, "%c", (char)(image->width & 0xFF));
    fprintf(file, "%c", (char)((image->width >> 8) & 0xFF));
    fprintf(file, "%c", (char)(image->height & 0xFF));
    fprintf(file, "%c", (char)((image->height >> 8) & 0xFF));

    //GCTF, Color Res, SF, size of GCT
    GCTF = 1;
    colorRes = 7;  //número de bits por cor primária (-1)
    SF = 0;
    sz = numBits(image->numColors - 1) - 1; //-1: 0 --> 2^1, 7 --> 2^8
    toWrite = (char) (GCTF << 7 | colorRes << 4 | SF << 3 | sz);
    fprintf(file, "%c", toWrite);

    //Background color index
    bgci = 0;
    fprintf(file, "%c", bgci);

    //Pixel aspect ratio
    par = 0; // 0 --> informação sobre aspect ratio não fornecida --> decoder usa valores por omissão
    fprintf(file, "%c",par);

    //Global color table
    for (i = 0; i < image->numColors * 3; i++)
    {
        fprintf(file, "%c", image->colors[i]);
    }
}

/* Meta 1 */
void writeImageBlockHeader (imageStruct* image, FILE* file) {
    char img_separator;

    /* Image Separator */
    img_separator = 0x2c;
    fprintf(file, "%c", img_separator);

    /* Left Position */
    fprintf(file, "%c", (char)0);
    fprintf(file, "%c", (char)0);

    /* Top Position */
    fprintf(file, "%c", (char)0);
    fprintf(file, "%c", (char)0);
    
    /* Image Width and Height */
    fprintf(file, "%c", (char)(image->width & 0xFF));
    fprintf(file, "%c", (char)((image->width >> 8) & 0xFF));
    fprintf(file, "%c", (char)(image->height & 0xFF));
    fprintf(file, "%c", (char)((image->height >> 8) & 0xFF));

    /* Byte Offset 8 */
    fprintf(file, "%c", (char)0);
    
    /* Local Color Tale */
    fprintf(file, "%c", (char)0); 
   
    /* LZW Minimum Code Size */
    fprintf(file, "%c", (char)2); /* Numero de cores */
}

/* Meta 2 */
void LZWCompress (FILE* file, char minCodeSize, char pixels, int widthHeight) {
    List lista = cria_lista();
    /* Code */
}
