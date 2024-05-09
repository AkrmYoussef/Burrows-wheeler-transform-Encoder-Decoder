#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// get the string input from the user
void getStringInput(char* input, size_t size) {
  printf("Enter a string: ");
  fgets(input, size, stdin);
}
// get the choice for encoding or decoding
int getEncodingDecodingChoice() {
  int choice;
  printf("Enter 1 for encoding or 0 for decoding: ");
  scanf("%d", &choice);
  getchar();  
  return choice;
}
// get the end of file character
char getEOF() {
  char eof[2];
  printf("Enter EOF character: ");
  fgets(eof, sizeof(eof), stdin);
  return eof[0];
}
// sort the strings in lexicographical order
void sort_lexico(char** arr, int n) {

   for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
         if (strcmp(arr[i], arr[j]) > 0) {
            char* temp =  arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
         }
      }
   }
}

// append the input matrix to the beginning of the sorted matrix
void append_beginning(char** input_matrix, char** sort, int len) {
 for(int i = 0; i < len; i++){
    for(int j = len-1 ; j > 0  ; j--){
      sort[i][j] = sort[i][j-1];
    }
    sort[i][0] = input_matrix[i][0];
  }
}

void burrowsWheelerEncode(char* input, char eof, char* encoded_string) {
  int len = strlen(input);
  input[len - 1] = eof;

  //Construct bwt_matrix
  char **bwt_matrix = (char**)malloc(len *sizeof(char*));
  
  int i;
  for(i = 0; i < len; i++){
        bwt_matrix[i] = (char*)malloc( 32 * sizeof(char));
  }
  
  // storing the rotations of the input string
  strcpy(bwt_matrix[0], input);
  for(i = 1; i < len; i++){
    char temp = input[len-1];
    for(int j = len-1 ; j > 0 ; j--){
      input[j] = input[j-1];
    }
    input[0] = temp;
    strcpy(bwt_matrix[i], input);
    printf("Matrix[%d]: %s\n",i,bwt_matrix[i]);
  }

  
   // storing strings in the lexicographical order

    sort_lexico(bwt_matrix, len);

    printf("\nStrings in lexicographical order: \n");
    for (int i = 0; i < len; ++i) {
        printf("matrix[%d] %s\n",i, bwt_matrix[i]);
    }

    // p the last column of the matrix
    printf("length of input: %d\n", len);
    for(i = 0; i < len; i++){
      encoded_string[i] = bwt_matrix[i][len-1];
    }
    
    encoded_string[len] = '\0';
    printf("Encoded string: %s\n", encoded_string);
   
    for (i = 0; i < len; i++) {
        free(bwt_matrix[i]);
    }
    free(bwt_matrix);
}

void burrowsWheelerDecode(char* input, char eof, char* decoded_string){
 int len = strlen(input)-1;
 printf("Length of input: %d\n", len);

 char** sort = malloc(len * sizeof(char*));
 char** input_matrix = malloc(len * sizeof(char*));

  // construct the input matrix from the input string
  for (int i = 0; i < len; i++) {
      input_matrix[i] = malloc(len * sizeof(char));
      strcpy(input_matrix[i], (char[2]) { (char) input[i], '\0' });
  }

  // construct the sort matrix from the input matrix
  for (int i = 0; i < len; i++) {
      sort[i] = malloc((len+2) * sizeof(char));
      strcpy(sort[i], input_matrix[i]);
  }


   for(int i = 0; i < len-1; i++){ 
    // sort lexicographically   
    sort_lexico(sort, len);
 

    printf("\nStrings in lexicographical order: \n");
    for (int i = 0; i < len; ++i) {
        printf("sort[%d]: %s\n",i, sort[i]);
    }

    //append the input matrix to the beginning of the sorted matrix
    append_beginning(input_matrix, sort,len);
   }

  // find the string in sort matrix that end with the EOF character
  for(int i = 0; i < len; i++){
    if(sort[i][len-1] == eof){
      strcpy(decoded_string, sort[i]);
      break;
    }
  }

  decoded_string[len-1] = '\0';

  // free the input matrix
  for (int i = 0; i < len; i++) {
        free(input_matrix[i]);
  }
  free(input_matrix);

  //free the sorted matrix
  for (int i = 0; i < len; i++) {
        free(sort[i]);
  }
  free(sort);

}

int main() {
  // input string
  char input[100];  // Array declared in main
  getStringInput(input, sizeof(input));
  printf("You entered: %s\n", input);

  // end of file character
  char eof = getEOF();
  printf("You entered EOF character: %c\n", eof);

  // choice for encoding or decoding
  int encodingChoice;
  encodingChoice = getEncodingDecodingChoice();  // Get user's encoding choice
  if (encodingChoice != 0 && encodingChoice != 1) {
    printf("Invalid choice. Please choose between 0 and 1.\n");
    return 1;
  }
  printf("Encoding/Decoding choice: %d\n", encodingChoice);

  // encode or decode the input string
  if (encodingChoice == 1) {
    char* encoded_string = malloc(100 * sizeof(char)); 
    burrowsWheelerEncode(input, eof, encoded_string);
    
    free(encoded_string);
  }
  else{
    char decoded_string[100];
    burrowsWheelerDecode(input, eof, decoded_string);
    printf("Decoded string: %s\n", decoded_string);
  }

  return 0;
}
