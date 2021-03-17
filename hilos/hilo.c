#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
	char word[101];
	int frequency;
} WordArray ;

int compareWords(const void *f1, const void *f2){
	WordArray *a = (WordArray *)f1;
	WordArray *b = (WordArray *)f2;
	return (b->frequency - a->frequency);
}

void countFrequency(void *arg){
	
	char *fileName = (char*)malloc(sizeof(char));
	fileName = (char*)arg;
	WordArray *words = (WordArray*)calloc((1000),sizeof(WordArray));
	char *result = (char*)malloc(sizeof(result)*100);
	int counter = 0;
	int isUnique;
	int i = 0;
	FILE *file;
	char buff[1000];

	//Limpiar el array, para que los hilos no mofiquen el contenido 
	for (i = 0; i < sizeof(WordArray); i++){
		words[i].word[0] = '\0';
		words[i].frequency = 0;
	}

	file = fopen(fileName, "r");
	if (file == NULL){
		printf("No se puede abrir el fichero: ");
	}
			
	else {
		while ( (fscanf(file, "%s", buff)) != EOF)
		{
			isUnique = -1;

			// Comparacion de cadenas para verificar si la palabra existe en el arreglo
			int k;
			for (k = 0; k < counter; k++){
				if (strcmp(words[k].word, buff) == 0){
					isUnique = k;
				}
			}
			//Si la cadena no esta en el array, agregarla al array
			if (isUnique == -1){
				strcpy(words[counter].word, buff);
				words[counter].frequency = 1;
				counter++;
			}
			//Incrementar la frecuencia
			else {
				words[isUnique].frequency++;
			}
			//Reasignacion de memoria del arreglo
			words = realloc(words, (sizeof(*words) + counter) * sizeof(WordArray));	
		}
	}

	//Ordenar las palabras desde la mas frecuente
	qsort(words, counter, sizeof(WordArray), compareWords);
	//Mostrar los tres primeros resultados
	snprintf(result, 10000, "%s %d %s %s %s", fileName, counter, words[0].word, words[1].word, words[2].word);
	fclose(file);
	printf("%s\n", result);

}

int main(int argc, char *argv[]){
	
	//Declara los hilos y atributos predeterminados
	pthread_t threads[argc-1];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	//Ejecutar los hilos de forma concurrente
	int i;
	for (i = 1; i < argc; i++){
		char* arguments = (char*)malloc(sizeof(argv[i])+1);
		arguments = argv[i];
		//Crear un nuevo hilo por cada argumento pasado
		pthread_create(&threads[i], &attr, (void*) countFrequency, (void*) arguments);
	}
	for (i = 1; i < argc; i++){
		//Unir los hilos para evitar perdidas de memoria
		pthread_join(threads[i], NULL);
	}

	return 0;
}