#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

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

void free_tree(pnode node) {
    if(node == NULL)
       return;
    
    free_tree(node->sus_dreapta);
    free_tree(node->sus_stanga);
    free_tree(node->jos_dreapta);
    free_tree(node->jos_stanga);
     
    free(node);
}

void print_tree(pnode node) {

    if (node == NULL)
        return;
 
    printf("|%u %u %u|", node->red, node->green, node->blue);
    printf("Indicii : %d %d %d %d ", node->one, node->two, node->three, node->four);
    printf("Area %d \n", node->area);
    printf("Type : %d\n", node->type);
 
 
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

     root->red = red;
     root->blue = blue;
     root->green = green;
     root->type = 2 ;
     root->area = size * size;

     for(int i = X ; i < X + size ; i++)
      for(int j = Y ; j < Y + size ; j++)
      {
        mean = mean + (red - matrix[i][j].red)*(red - matrix[i][j].red) + (green - matrix[i][j].green)*(green - matrix[i][j].green) + (blue - matrix[i][j].blue)*(blue - matrix[i][j].blue);
      }
     mean =  mean / (3 * size * size) ;

     
     if( mean > prag ) {
     root->sus_stanga = malloc(sizeof(nod));
     root->sus_dreapta = malloc(sizeof(nod));
     root->jos_dreapta = malloc(sizeof(nod));
     root->jos_stanga = malloc(sizeof(nod));
     
     indice++;

     root->one = indice;
     process(matrix, size/2 , X + 0 , Y + 0  , prag, root->sus_stanga);

     root->two = indice;
     process(matrix, size/2 , X + 0 , Y + size/2 , prag, root->sus_dreapta);

     root->three = indice;
     process(matrix, size/2 , X + size/2 , Y + size/2 , prag, root->jos_dreapta);

     root->four = indice;
     process(matrix, size/2 , X + size/2 , Y + 0 , prag, root->jos_stanga);

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

void vertical(pnode arbore) {
    if(arbore == NULL)
    return;

    pnode aux = arbore->sus_stanga;
    arbore->sus_stanga = arbore->jos_stanga;
    arbore->jos_stanga = aux;
    aux = arbore->sus_dreapta;
    arbore->sus_dreapta = arbore->jos_dreapta;
    arbore->jos_dreapta = aux;

    vertical(arbore->sus_stanga);
    vertical(arbore->sus_dreapta);
    vertical(arbore->jos_dreapta);
    vertical(arbore->jos_stanga);

}

void orizontal(pnode arbore) {

     if(arbore == NULL)
     return;

    pnode aux = arbore->sus_stanga;
    arbore->sus_stanga = arbore->sus_dreapta;
    arbore->sus_dreapta = aux;
    aux = arbore->jos_dreapta;
    arbore->jos_dreapta = arbore->jos_stanga;
    arbore->jos_stanga = aux;

    orizontal(arbore->sus_stanga);
    orizontal(arbore->sus_dreapta);
    orizontal(arbore->jos_dreapta);
    orizontal(arbore->jos_stanga);



}

void transform_vector(QuadtreeNode *v, pnode arbore, int i) {
    
	arbore->red   = v[i].red;
	arbore->blue  = v[i].blue;
	arbore->green = v[i].green;
	arbore->area  = v[i].area;
    arbore->one = v[i].top_left;
    arbore->two = v[i].top_right;
    arbore->three = v[i].bottom_right;
    arbore->four = v[i].bottom_left;



	if(v[i].top_left != -1)
	 {  arbore->sus_stanga = malloc(sizeof(nod));
        arbore->sus_dreapta = malloc(sizeof(nod));
        arbore->jos_dreapta = malloc(sizeof(nod));
        arbore->jos_stanga = malloc(sizeof(nod));

        arbore->type = 2;
		transform_vector(v, arbore->sus_stanga, v[i].top_left);
		transform_vector(v, arbore->sus_dreapta, v[i].top_right);
		transform_vector(v, arbore->jos_dreapta, v[i].bottom_right);
		transform_vector(v, arbore->jos_stanga, v[i].bottom_left);

	}
	else
	{   arbore->type = -1;
		arbore->sus_stanga = NULL;
		arbore->sus_dreapta = NULL;
		arbore->jos_dreapta = NULL;
		arbore->jos_stanga = NULL;
	}


}

void decompression(pnode arbore, pixel **matrix, int X, int Y, int size) {

	if(arbore->type != -1)
	{
		decompression(arbore->sus_stanga, matrix, X + 0 , Y + 0, size/2);
		decompression(arbore->sus_dreapta, matrix, X + 0, Y + size/2, size/2);
		decompression(arbore->jos_dreapta, matrix, X + size/2, Y + size/2, size/2);
		decompression(arbore->jos_stanga,  matrix, X + size/2, Y + 0, size/2);
	}
	else
	{   for(int i = X; i < X + size; i++)
			for(int j = Y; j < Y + size; j++)
			{
				matrix[i][j].red   = arbore->red;
				matrix[i][j].green = arbore->green;
				matrix[i][j].blue  = arbore->blue;
			}

	}

}

void write_file(pixel ** matrix, char *file, int size) {

    FILE *output = fopen(file, "wb");
	
	fprintf(output, "P6\n");
	fprintf(output, "%d %d\n", size, size );
	fprintf(output, "255\n");
	
	for(int i = 0; i < size; i++)
		fwrite(matrix[i], sizeof(pixel), size, output);
	
	fclose(output);
}

void write_file_compress(QuadtreeNode *v, char *file, uint32_t nr_noduri, uint32_t nr_frunze){
    
    FILE *output = fopen(file, "wb");

    fwrite(&nr_frunze, sizeof(uint32_t), 1, output);
    fwrite(&nr_noduri, sizeof(uint32_t), 1, output);
    fwrite(v, sizeof(QuadtreeNode), nr_noduri, output);

    fclose(output);
}

int main(int argc, char **argv)
{   // verificare argumente
    if(argc == 1)
    return 0;

    if(strcmp(argv[1], "-d") == 0) {
        // declarare nr frunze si nr noduri
        uint32_t nr_frunze;
        uint32_t nr_noduri;
        FILE *input = fopen(argv[2], "rb");
        
        // citire nr frunze, nr noduri, si elemente vector
        fread(&nr_frunze, sizeof(uint32_t), 1, input);
        fread(&nr_noduri, sizeof(uint32_t), 1, input);
        QuadtreeNode *vector = (QuadtreeNode *)malloc(nr_noduri * sizeof(QuadtreeNode));
        fread(vector, sizeof(QuadtreeNode), nr_noduri, input);
        fclose(input);
        
        // declarare arbore si copiere elemente din vector
        pnode root = malloc(sizeof(nod));
        transform_vector(vector,root, 0);    
        
        // declarare matrice pixeli
        int size = sqrt(root->area);
        pixel **matrix = (pixel **)malloc(size * sizeof(pixel *));
        for(int i = 0 ; i < size ; i++) {
             matrix[i] = (pixel *)malloc(size * sizeof(pixel));
        }
        // transpunere elemente din arbore in matrice
        decompression(root, matrix, 0, 0, size);
        // scriere in fisier
        write_file(matrix, argv[3], size);
        
        // eliberare memorie matrice, memorie arbore, memorie vector
        for (int i = 0; i < size; i++)
           free(matrix[i]);
        free(matrix);
        free_tree(root);
        free(vector);
    }

    if(strcmp(argv[1], "-c") == 0) {
        // declarare factor si convertire in tip int
        int32_t factor;
        sscanf (argv[2],"%"SCNd32"",&factor);
        FILE *input = fopen(argv[3], "rb");
        
        // citire date
        unsigned int dim1,dim2,max;
        char type[10];
        fscanf(input, "%s%d%d%d", type, &dim1, &dim2, &max);
        // citire spatiu gol din fisier binar
        char newline;
	    fread(&newline,sizeof(char),1,input);
        
        //  alocare dinamica matrice
        pixel **matrix = (pixel **)malloc(dim1 * sizeof(pixel *));
        for(int i = 0 ; i < dim2 ; i++) {
            matrix[i] = (pixel *)malloc(dim2 * sizeof(pixel));
        }

        // citire matrice de pixeli
        for (int i = 0; i < dim1; i++)
            for (int j = 0; j < dim2; j++) {
                fread(&matrix[i][j].red, sizeof(unsigned char), 1, input);
                fread(&matrix[i][j].green, sizeof(unsigned char), 1, input);
                fread(&matrix[i][j].blue, sizeof(unsigned char), 1, input);
            }

        fclose(input);
        
        // declarare arbore
        pnode root = malloc(sizeof(nod));
        root->sus_stanga = NULL;
        root->sus_dreapta = NULL;
        root->jos_dreapta = NULL;
        root->jos_stanga = NULL;
        // verificare poza patrata
        int size;
        if(dim1 == dim2) {
            size = dim1;
        }
        else {
            return -1;
        }
        // procesare imagine (impartire in submatrici de pixeli)
        process(matrix, size, 0, 0, factor, root);
        // extragere nr noduri si nr frunze
        uint32_t nr_noduri = 0;
        uint32_t nr_frunze = 0;
        node_number(root,&nr_noduri);
        number_leaf(root,&nr_frunze);
        // declarare vector si stocare elemente din arbore
        QuadtreeNode *vector = (QuadtreeNode *)malloc(nr_noduri * sizeof(QuadtreeNode));
        save_data(root,vector);
        // scriere in fisier
        write_file_compress(vector,argv[4],nr_noduri,nr_frunze);
         
        // eliberare memorie
         for (int i = 0; i < size; i++)
           free(matrix[i]);
        free(matrix);
        free_tree(root);
        free(vector);
    }

    if(strcmp(argv[1], "-m") == 0) {
        // declarare factor si convertire in tip int
        int32_t factor;
        sscanf (argv[3],"%"SCNd32"",&factor);
        FILE *input = fopen(argv[4], "rb");
        
        // citire date
        unsigned int dim1,dim2,max;
        char type[10];
        fscanf(input, "%s%d%d%d", type, &dim1, &dim2, &max);
        // citire spatiu gol din fisier binar
        char newline;
	    fread(&newline,sizeof(char),1,input);
        
        //  alocare dinamica matrice
        pixel **matrix = (pixel **)malloc(dim1 * sizeof(pixel *));
        for(int i = 0 ; i < dim2 ; i++) {
            matrix[i] = (pixel *)malloc(dim2 * sizeof(pixel));
        }

        // citire matrice de pixeli
        for (int i = 0; i < dim1; i++)
            for (int j = 0; j < dim2; j++) {
                fread(&matrix[i][j].red, sizeof(unsigned char), 1, input);
                fread(&matrix[i][j].green, sizeof(unsigned char), 1, input);
                fread(&matrix[i][j].blue, sizeof(unsigned char), 1, input);
            }

        fclose(input);
        
        // declarare arbore
        pnode root = malloc(sizeof(nod));
        root->sus_stanga = NULL;
        root->sus_dreapta = NULL;
        root->jos_dreapta = NULL;
        root->jos_stanga = NULL;
        // verificare poza patrata
        int size;
        if(dim1 == dim2) {
            size = dim1;
        }
        else {
            return -1;
        }
        
        if(strcmp(argv[2],"v") == 0) {
            // procesare imagine (impartire in submatrici de pixeli)
            process(matrix, size, 0, 0, factor, root);
            vertical(root);
            decompression(root,matrix,0,0,size);
            // scriere in fisier
             write_file(matrix, argv[5], size);
        }
        if(strcmp(argv[2],"h") == 0) {
            // procesare imagine (impartire in submatrici de pixeli)
            process(matrix, size, 0, 0, factor, root);
            orizontal(root);
            decompression(root,matrix,0,0,size);
            // scriere in fisier
             write_file(matrix, argv[5], size);
        }

         
        // eliberare memorie
         for (int i = 0; i < size; i++)
           free(matrix[i]);
        free(matrix);
        free_tree(root);
    }

    return 0;
}
