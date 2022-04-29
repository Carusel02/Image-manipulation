#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct QuadtreeNode
{
    unsigned char blue, green, red;
    uint32_t area;
    int32_t top_left, top_right;
    int32_t bottom_left, bottom_right;
} __attribute__((packed)) QuadtreeNode;

typedef struct pixel {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

typedef struct node { 
   unsigned char red, green, blue;
   int type;
   int one, two ,three, four;
   struct node *sus_stanga;
   struct node *sus_dreapta;
   struct node *jos_stanga;
   struct node *jos_dreapta;
} nod, *pnode;

void init(pnode node) {
    node->sus_stanga = malloc(sizeof(node));
    node->sus_dreapta = malloc(sizeof(node));
    node->jos_dreapta = malloc(sizeof(node));
    node->jos_stanga = malloc(sizeof(node));
}

void add(pnode node, unsigned int red, unsigned int green, unsigned int blue) {
    node = (pnode)malloc(sizeof(nod));
    node->red = red ;
    node->green = green ;
    node->blue = blue;
    node->sus_stanga = NULL;
    node->sus_dreapta = NULL;
    node->jos_dreapta = NULL;
    node->jos_stanga = NULL;
}

void free_tree(pnode node) {
    if(node == NULL)
       return;
    
    free_tree(node->sus_dreapta);
    free_tree(node->sus_stanga);
    free_tree(node->jos_dreapta);
    free_tree(node->jos_stanga);
     
    // printf(" ADRESA %p in free\n", node);
    free(node);
}

void print_tree(pnode node) {

    if (node == NULL)
        return;
 
    printf("|%u %u %u| type : %d ", node->red, node->green, node->blue, node->type);
    printf("Indicii : %d %d %d %d\n", node->one, node->two, node->three, node->four);
 
 
    print_tree(node->sus_stanga);
    print_tree(node->sus_dreapta);
    print_tree(node->jos_dreapta);
    print_tree(node->jos_stanga);


}

unsigned int medie(pixel **submatrix, int X, int Y, int size, int type) {
    unsigned int red = 0 ;
    unsigned int green = 0 ;
    unsigned int blue = 0; 
  
    if(type == 0) {
    for (int i = X ; i < X + size ; i++) 
        for(int j = Y ; j < Y + size ; j++)
           red = red + (unsigned int)submatrix[i][j].red;

    red = red / (size * size) ;
    return red;
    }

    if(type == 1) {
    for (int i = X ; i < X + size ; i++) 
        for(int j = Y ; j < Y + size ; j++)
           green = green + (unsigned int)submatrix[i][j].green;

    green = green / (size * size) ;
    return green;
    }

    if(type == 2) {
    for (int i = X ; i < X + size ; i++) 
        for(int j = Y ; j < Y + size ; j++)
           blue = blue + (unsigned int)submatrix[i][j].blue;
    
    blue  = blue / (size * size);
    return blue;
    }

    return 0;
}

int indice = 0;

void process(pixel **matrix, int size, int X, int Y, int prag, pnode root) {
     unsigned int red,green,blue;
     red = medie(matrix,X,Y,size,0);
     green = medie(matrix,X,Y,size,1);
     blue = medie(matrix,X,Y,size,2);
     unsigned int mean = 0;
     

     root->red = red;
     root->blue = blue;
     root->green = green;
     root->type = 2 ;
    //  root->sus_stanga = malloc(sizeof(nod));
    //  printf(" ****ADRESA 1 %p \n", root->sus_stanga);
    //  root->sus_dreapta = malloc(sizeof(nod));
    //  printf(" ****ADRESA 2 %p \n", root->sus_dreapta);
    //  root->jos_dreapta = malloc(sizeof(nod));
    //  printf(" ****ADRESA 3 %p \n", root->jos_dreapta);
    //  root->jos_stanga = malloc(sizeof(nod));
    //  printf(" ****ADRESA 4 %p \n", root->jos_stanga);
     
    //  printf("pixel : ");
    //  printf("|%u %u %u|\n", red, green, blue);

     for(int i = X ; i < X + size ; i++)
      for(int j = Y ; j < Y + size ; j++)
      {
        mean = mean + (red - matrix[i][j].red)*(red - matrix[i][j].red) + (green - matrix[i][j].green)*(green - matrix[i][j].green) + (blue - matrix[i][j].blue)*(blue - matrix[i][j].blue);
      }
     mean = mean / (3 * size * size) ;

    //  printf("mean : %u\n", mean);
     
     if( mean > prag) {
     root->sus_stanga = malloc(sizeof(nod));
    //  printf(" ****ADRESA 1 %p \n", root->sus_stanga);
     root->sus_dreapta = malloc(sizeof(nod));
    //  printf(" ****ADRESA 2 %p \n", root->sus_dreapta);
     root->jos_dreapta = malloc(sizeof(nod));
    //  printf(" ****ADRESA 3 %p \n", root->jos_dreapta);
     root->jos_stanga = malloc(sizeof(nod));
    //  printf(" ****ADRESA 4 %p \n", root->jos_stanga);
     indice++;
     root->one = indice;
     process(matrix, size/2 , X + 0 , Y + size/2 , prag, root->sus_stanga);
    //  indice++;
     root->two = indice;
     process(matrix, size/2 , X + 0 , Y + 0 , prag, root->sus_dreapta);
    //  indice++;
     root->three = indice;
     process(matrix, size/2 , X + size/2 , Y + size/2 , prag, root->jos_dreapta);
    //  indice++;
     root->four = indice;
     process(matrix, size/2 , X + size/2 , Y + 0 , prag, root->jos_stanga);
    //  indice++;
     }
     else
     { root->red = red;
       root->blue = blue;
       root->green = green;
       root->type = -1 ;
       root->one = -1;
       root->two = -1;
       root->three = -1;
       root->four = -1;
    //    printf("!!!!!!! ADRESA %p \n", root);
       root->sus_stanga = NULL;
       root->sus_dreapta = NULL;
       root->jos_dreapta = NULL;
       root->jos_stanga = NULL;
       indice++;
     }

     


}


int main()
{   FILE *input = fopen("Imagini/test0.ppm", "rb");
    printf(input == NULL ? "Fisier negasit\n" : "Fisier gasit\n");
    
    char buffer[256];
    fread(buffer, sizeof(char), 17, input);
    
    buffer[17] = '\0';
    printf("buffer:%s \n", buffer);
    
    // modificare
    int lungime, latime;
    lungime = 4;
    latime = 4;
     
    // alocare dinamica matrice
    pixel **matrix = (pixel **)malloc(lungime * sizeof(pixel *));
    for(int i = 0 ; i < lungime ; i++) {
        matrix[i] = (pixel *)malloc(latime * sizeof(pixel));
    }
    
    // citire matrice de pixeli
     for (int i = 0; i < lungime; i++)
        for (int j = 0; j < latime; j++)
            { fread(&matrix[i][j].red, sizeof(unsigned char), 1, input);
              fread(&matrix[i][j].green, sizeof(unsigned char), 1, input);
              fread(&matrix[i][j].blue, sizeof(unsigned char), 1, input);
            }
    

    pnode root = malloc(sizeof(nod));
    // printf(" ADRESS %p in main \n", root);
    root->sus_stanga = NULL;
    root->sus_dreapta = NULL;
    root->jos_dreapta = NULL;
    root->jos_stanga = NULL;
    
    // cerinta 1
    int prag = 0;
    process(matrix, lungime, 0, 0, prag, root);
    print_tree(root);
    free_tree(root);

     for (int i = 0; i < lungime; i++) {
        for (int j = 0; j < latime; j++)
            printf("|%u %u %u| ", matrix[i][j].red , matrix[i][j].green, matrix[i][j].blue);
        
        printf("\n");
       }

    


    for (int i = 0; i < lungime; i++)
        free(matrix[i]);
    free(matrix);

    fclose(input);
    // QuadtreeNode *vector = (QuadtreeNode *)malloc(sizeof(QuadtreeNode));
    return 0;
}