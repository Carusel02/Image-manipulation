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
   int area;
   struct node *sus_stanga;
   struct node *sus_dreapta;
   struct node *jos_stanga;
   struct node *jos_dreapta;
} nod, *pnode;

// void init(pnode node) {
//     node->sus_stanga = malloc(sizeof(node));
//     node->sus_dreapta = malloc(sizeof(node));
//     node->jos_dreapta = malloc(sizeof(node));
//     node->jos_stanga = malloc(sizeof(node));
// }

// void add(pnode node, unsigned int red, unsigned int green, unsigned int blue) {
//     node = (pnode)malloc(sizeof(nod));
//     node->red = red ;
//     node->green = green ;
//     node->blue = blue;
//     node->sus_stanga = NULL;
//     node->sus_dreapta = NULL;
//     node->jos_dreapta = NULL;
//     node->jos_stanga = NULL;
// }

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
    printf("Indicii : %d %d %d %d ", node->one, node->two, node->three, node->four);
    printf("Area %d \n", node->area);
 
 
    print_tree(node->sus_stanga);
    print_tree(node->sus_dreapta);
    print_tree(node->jos_dreapta);
    print_tree(node->jos_stanga);


}

uint64_t medie(pixel **submatrix, int X, int Y, unsigned int size, int type) {
    uint64_t red = 0 ;
    uint64_t green = 0 ;
    uint64_t blue = 0; 
  
    if(type == 0) {
    for (int i = X ; i < X + size ; i++) 
        for(int j = Y ; j < Y + size ; j++)
           red = red + submatrix[i][j].red;

    red =  red / (size * size) ;
    return red;
    }

    if(type == 1) {
    for (int i = X ; i < X + size ; i++) 
        for(int j = Y ; j < Y + size ; j++)
           green = green + submatrix[i][j].green;

    green =  green / (size * size) ;
    return green;
    }

    if(type == 2) {
    for (int i = X ; i < X + size ; i++) 
        for(int j = Y ; j < Y + size ; j++)
           blue = blue + submatrix[i][j].blue;
    
    blue  = blue / (size * size);
    return blue;
    }

    return 0;
}

int indice = 0;

void process(pixel **matrix, unsigned int size, int X, int Y, int prag, pnode root) {
     uint64_t red,green,blue;
     red = medie(matrix,X,Y,size,0);
     green = medie(matrix,X,Y,size,1);
     blue = medie(matrix,X,Y,size,2);
     uint64_t mean = 0;

    //  printf("%"PRIu64" %"PRIu64" %"PRIu64"\n", red, green, blue);
     

     root->red = red;
     root->blue = blue;
     root->green = green;
     root->type = 2 ;
     root->area = size * size;
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
     mean =  mean / (3 * size * size) ;

     
     if( mean > prag ) {
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
     process(matrix, size/2 , X + 0 , Y + 0  , prag, root->sus_stanga);
    //  indice++;
     root->two = indice;
     process(matrix, size/2 , X + 0 , Y + size/2 , prag, root->sus_dreapta);
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

void node_number(pnode arbore, uint32_t *numar) {
    if(arbore == NULL)
       return;
    
    node_number(arbore->sus_stanga,numar);
    node_number(arbore->sus_dreapta,numar);
    node_number(arbore->jos_dreapta,numar);
    node_number(arbore->jos_stanga,numar);
    
    (*numar)++;
    
}

int counter = 0 ;

void save_data(pnode arbore, QuadtreeNode *vector) {
     if(arbore == NULL)
       return;
     
     vector[counter].red = arbore->red;
     vector[counter].green = arbore->green;
     vector[counter].blue = arbore->blue;
     vector[counter].area = arbore->area;
     vector[counter].top_left = arbore->one;
     vector[counter].top_right = arbore->two;
     vector[counter].bottom_right = arbore->three;
     vector[counter].bottom_left = arbore->four;



     counter++;
     
     save_data(arbore->sus_stanga,vector);
     save_data(arbore->sus_dreapta,vector);
     save_data(arbore->jos_dreapta,vector);
     save_data(arbore->jos_stanga,vector);

}

void print_vector(QuadtreeNode *v, int dim) {

     for(int i = 0 ; i < dim ; i++) {
        printf("|%u %u %u| size %d\n", v[i].red, v[i].green, v[i].blue, v[i].area);
        printf("|%d %d %d %d|\n", v[i].top_left, v[i].top_right, v[i].bottom_right, v[i].bottom_left);
     }

}

void number_leaf(pnode arbore, uint32_t *leaf) {
    
    if(arbore == NULL)
       return;
    
    number_leaf(arbore->sus_stanga,leaf);
    number_leaf(arbore->sus_dreapta,leaf);
    number_leaf(arbore->jos_dreapta,leaf);
    number_leaf(arbore->jos_stanga,leaf);
    
    if(arbore->type == -1)
    (*leaf)++;


}

int main()
{   FILE *input = fopen("Exemple/test0.ppm", "rb");
    printf(input == NULL ? "Fisier negasit\n" : "Fisier gasit\n");
    
    unsigned dim1,dim2,max;
    char type[10];
    fscanf(input, "%s%d%d%d", type, &dim1, &dim2, &max);
    printf("%s%d%d%d", type, dim1, dim2, max);

    char newline;
	fread(&newline,sizeof(char),1,input);


    // char buffer[256];
    // fread(buffer, sizeof(char), 15, input);
    
    // buffer[15] = '\0';
    // printf("buffer:%s \n", buffer);
    
    // modificare
    unsigned int lungime, latime;
    lungime = 256;
    latime = 256;
     
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
    // print_tree(root);
    
    uint32_t numar_noduri = 0 ;
    uint32_t leaf = 0 ;
    node_number(root,&numar_noduri);
    number_leaf(root,&leaf);
    printf(" NUMAR NODURI = %d\n", numar_noduri);
    QuadtreeNode *vector = (QuadtreeNode *)malloc(numar_noduri * sizeof(QuadtreeNode));
    save_data(root,vector);
    print_vector(vector,numar_noduri);

    FILE *output = fopen("output", "wb");

    fwrite(&leaf, sizeof(uint32_t), 1, output);
    fwrite(&numar_noduri, sizeof(uint32_t), 1, output);
    fwrite(vector, sizeof(QuadtreeNode), numar_noduri, output);

    
    printf("LEAF : %d \n", leaf);


    
    
    
    fclose(output);
    free_tree(root);

    //  for (int i = 0; i < lungime; i++) {
    //     for (int j = 0; j < latime; j++)
    //         printf("|%u %u %u| ", matrix[i][j].red , matrix[i][j].green, matrix[i][j].blue);
        
    //     printf("\n");
    //    }

    


    for (int i = 0; i < lungime; i++)
        free(matrix[i]);
    free(matrix);
    
    free(vector);
    fclose(input);
    return 0;
}