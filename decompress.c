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
     // TREBUIE MODIFICAT
     if(arbore == NULL)
     return;

    pnode aux = arbore->sus_stanga;
    arbore->sus_stanga = arbore->jos_stanga;
    arbore->jos_stanga = aux;
    aux = arbore->sus_dreapta;
    arbore->sus_dreapta = arbore->jos_dreapta;
    arbore->jos_dreapta = aux;

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

void write_file(pixel ** mat, char *file, int size) {

    FILE *output = fopen(file, "wb");
	
	fprintf(output, "P6\n");
	fprintf(output, "%d %d\n", size, size );
	fprintf(output, "255\n");
	
	for(int i = 0; i < size; i++)
		fwrite(mat[i], sizeof(pixel), size, output);
	
	fclose(output);
}

int main(int argc, char **argv)
{   
    FILE *input = fopen("Exemple/compress1_370.out", "rb");
    uint32_t nr_frunze;
    uint32_t nr_noduri;

    for(int i = 1 ; i < argc ; i++) {
        if(strcmp(argv[i], "-d") == 0) {
        
        
        fread(&nr_frunze, sizeof(uint32_t), 1, input);
        fread(&nr_noduri, sizeof(uint32_t), 1, input);

        printf("%"PRIu32" %"PRIu32" \n", nr_frunze, nr_noduri);

        QuadtreeNode *vector = (QuadtreeNode *)malloc(nr_noduri * sizeof(QuadtreeNode));

        fread(vector, sizeof(QuadtreeNode), nr_noduri, input);

        fclose(input);

        pnode root = malloc(sizeof(nod));

        transform_vector(vector,root, 0);
        
        print_vector(vector,nr_noduri);
        print_tree(root);
         
        int size = sqrt(root->area);
        printf("SIZE : %d ", size);

        pixel **matrix = (pixel **)malloc(size * sizeof(pixel *));
          for(int i = 0 ; i < size ; i++) {
             matrix[i] = (pixel *)malloc(size * sizeof(pixel));
        }

        decompression(root, matrix, 0, 0, size);
        
          for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            printf("|%u %u %u| ", matrix[i][j].red , matrix[i][j].green, matrix[i][j].blue);
        
        printf("\n");
       }

       char output[20] = "poza2";

       write_file(matrix, output, size);



        for (int i = 0; i < size; i++)
           free(matrix[i]);
        free(matrix);

 
 

        free_tree(root);
        free(vector);

        }

    }



    return 0;
}