#include "GIFencoder.h"
#include "utils.h"
#include "BitFile.h"

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

//Conversao de lista RGB para indexada a um maximo de 256 cores
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
            // printf("%c", image->pixels[y * image->width + x]);

            if (colorIndex == colorNum) 
            {
                image->colors[colorIndex * 3]	  = (char)data[(y * image->width + x)*3];
                image->colors[colorIndex * 3 + 1] = (char)data[(y * image->width + x)*3 + 1];
                image->colors[colorIndex * 3 + 2] = (char)data[(y * image->width + x)*3 + 2];
                colorNum++;
            }
        }
    }

    //Define o n�mero de cores como potencia de 2 (devido aos requistos da Global Color Table)
    image->numColors = nextPower2(colorNum);

    //Refine o array de cores com base no numero final obtido
    copy = (char*)calloc(image->numColors*3, sizeof(char));
    memset(copy, 0, sizeof(char)*image->numColors*3);
    memcpy(copy, image->colors, sizeof(char)*colorNum*3);
    image->colors = copy;

    image->minCodeSize = num_bits(image->numColors - 1);
    if (image->minCodeSize == (char)1)  //Imagens binarias --> caso especial (pag. 26 do RFC)
        image->minCodeSize++;
}

//Determinacao da proxima potencia de 2 de um dado inteiro n
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

//Numero de bits necessario para representar n
char num_bits(int n) {
    char nb = 0;

    if (n == 0)
        return 0;

    while (n != 0) {
        nb++;
        n /= 2;
    }

    return nb;
}

//Fucao para escrever imagem no formato GIF, versao 87a
void GIFEncoderWrite (imageStruct* image, char* outputFile) {
    FILE* file = fopen(outputFile, "wb");
    char trailer;

    //Escrever cabe�alho do GIF
    writeGIFHeader(image, file);

    /* Meta 1 */
    writeImageBlockHeader(image, file);

    /* Meta 2 */
    LZWCompress(file, image->minCodeSize, image->pixels, image->width*image->height);

    fprintf(file, "%c", (char)0);

    /* Trailer */
    trailer = 0x3b;
    fprintf(file, "%c", trailer);

    fclose(file);
}

//Gravar cabecalho do GIF (ate global color table)
void writeGIFHeader (imageStruct* image, FILE* file) {
    int i;
    char toWrite, GCTF, colorRes, SF, sz, bgci, par;
    
    //Assinatura e versao (GIF87a)
    char* s = "GIF87a";
    for (i = 0; i < (int)strlen(s); i++)
    {
        fprintf(file, "%c", s[i]);
    }

    //Ecra logico (igual a da dimensao da imagem) --> primeiro o LSB e depois o MSB
    fprintf(file, "%c", (char)(image->width & 0xFF));
    fprintf(file, "%c", (char)((image->width >> 8) & 0xFF));
    fprintf(file, "%c", (char)(image->height & 0xFF));
    fprintf(file, "%c", (char)((image->height >> 8) & 0xFF));

    //GCTF, Color Res, SF, size of GCT
    GCTF = 1;
    colorRes = 7;  //Numero de bits por cor primaria (-1)
    SF = 0;
    sz = num_bits(image->numColors - 1) - 1; //-1: 0 --> 2^1, 7 --> 2^8
    toWrite = (char) (GCTF << 7 | colorRes << 4 | SF << 3 | sz);
    fprintf(file, "%c", toWrite);

    //Background color index
    bgci = 0;
    fprintf(file, "%c", bgci);

    //Pixel aspect ratio
    par = 0; // 0 --> informacao sobre aspect ratio nao fornecida --> decoder usa valores por omissao
    fprintf(file, "%c",par);

    //Global color table
    for (i = 0; i < image->numColors * 3; i++)
    {
        fprintf(file, "%c", image->colors[i]);
    }
}

/* Fill Dic Function */
void fill_dicionario(List dicionario, int clear_code, int eoi)
{
    int i;
    char temp_char;

    for (i = 0; i < clear_code; i++)
    {
        temp_char = (char)i;
        insere_lista(dicionario, &temp_char, 1);
    }

    temp_char = (char)clear_code;
    insere_lista(dicionario, &temp_char, 1); /* Clear code */
    temp_char = (char)eoi;
    insere_lista(dicionario, &temp_char, 1); /* End of information */
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

    /* LZW Minimum Code Size */
    fprintf(file, "%c", (char)image->minCodeSize);
}

/* Meta 2 */
void LZWCompress (FILE* file, char minCodeSize, char *pixels, int widthHeight) {
    int i, temp_index, clear_code, eoi;
    char *p, *c, *pc, temp_char;
    int p_length = 0, c_length = 0, pc_length = 0;
    List dicionario;
    bitStream *bit_stream;

    /* Create Bit Stream */
    bit_stream = bitFile(file);

    /* Create Dictionary */
    clear_code = (int)pow(2, (int)minCodeSize); /* minCodeSize ^ 2 */
    eoi = clear_code + 1;

    dicionario = cria_lista();
    fill_dicionario(dicionario, clear_code, eoi);

    /* Initially p is empty */
    p = "\0";

    /* Write Start Bits */
	write_bits(bit_stream, clear_code, num_bits(list_last_index(dicionario) - 1)); /* Clear Code */

    /* Algorythm */
    for (i = 0; i < widthHeight; i++)
    {
        /* c = next char in bitstream */
        c = (char*)malloc(sizeof(char));
        c[0] = pixels[i];
        c_length = 1;

        /* p + c */
        if (p_length == 0) /* First case */
        {
            pc = (char*)malloc(sizeof(char));
            pc_length = 1;
            pc[0] = c[0];
        }
        else
        {
            pc_length = 1 + p_length;
            pc = (char*)malloc(pc_length*sizeof(char));

            for (int j = 0; j < p_length; j++)
            {
                pc[j] = p[j];
            }
            pc[pc_length - 1] = c[0];
        }
        
        if (procura_lista(dicionario, pc, pc_length) == 1) /* p + c in dictionary */
        {
            /* p = pc */
            if (pc_length == 1)
            {
                p = (char*)malloc(sizeof(char));
                p_length = 1;
                p[0] = pc[0];
            }
            else
            {
                p = (char*)malloc(pc_length*sizeof(char));
                p_length = pc_length;
                for (int j = 0; j < p_length; j++)
                {
                    p[j] = pc[j];
                }
            }
        }
        else /* p + c not in dictionary */
        {
            /* Insert p + c into the list */
            if (list_last_index(dicionario) < 4096)
            {
                insere_lista(dicionario, pc, pc_length);
            }
            
            /* Get Index */
            temp_index = get_index(dicionario, p, p_length);
            write_bits(bit_stream, temp_index, num_bits(list_last_index(dicionario) - 1)); /* Write Index */

            /* p = c */
            p = (char*)malloc(sizeof(char));
            p_length = 1;
            p[0] = c[0];
        }
    }
	
    write_bits(bit_stream, eoi, num_bits(list_last_index(dicionario) - 1)); /* End of Information */

    flush(bit_stream);
}
