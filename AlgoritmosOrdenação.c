#include <stdio.h>
#include <locale.h>

int vet[21] = {36,24,25,26,2,45,45,47,7,26,26,10,34,33,28,50,28,41,27,34};  //Inicializar o vetor como vari�vel global
void imprimirVetor(int tamanho);											//para poder us�-lo em fun��es
void bubbleSort (int tamanho);
void selectionSort (int tamanho);
void insertionSort (int tamanho);

int main()
{
	int tamanho = sizeof(vet) / sizeof(vet[0]);
	int valor, i, escolha;
	setlocale(LC_ALL, "Portuguese");
	
	printf("Que tipo de ordena��o voc� quer fazer? ");
	printf("\n1 - Selection Sort\n2 - Bubble Sort\n3 - Insertion Sort\n0 - Sair\n");
	scanf("%d", &escolha);
	
	switch (escolha)
	{
		case 1:
			selectionSort(tamanho);
			break;
		case 2:
			bubbleSort(tamanho);
			break;
		case 3:
			insertionSort(tamanho);
			break;
		case 0:
			break;
		default: 
			printf("Escolha inv�lida! ");
			break;
	}
	
	if (escolha == 1 || escolha == 2 || escolha == 3 || escolha == 0)imprimirVetor(tamanho);
	
	return 0;
}

void imprimirVetor(int tamanho)
{
	int i;
	printf("\n\nValores do Vetor: \n");
	for (i = 0;i < tamanho; i++)
	{
		printf("%d  ", vet[i]);  // Pular linha ap�s 10 caracteres para ser poss�vel visualizar no console.
		if (i % 10 == 0 && i != 0) printf("\n");
	}
}

void bubbleSort (int tamanho)
{
	int i, j, tmp;
	for (i = 0;i < tamanho - 1; i++)
	{
		for (j = 0;j < tamanho - i; j++)
		{
			if (vet[j] > vet[j+1])	//
			{						//	Se a posi��o atual for maior que a pr�xima posi��o
				tmp = vet[j];		//	vamos troc�-las antes de continuar a leitura das
				vet[j] = vet[j+1];	//	outras posi��es.
				vet[j+1] = tmp;		//
			}	
		}
	}
}

void selectionSort (int tamanho)
{
	int i, j, tmp, menor;
	
	for (i = 0; i < tamanho - 1; i++)
	{
		menor = i; //Atribue � vari�vel menor a primeira posi��o que ser� comparada com as outras para n�o termos erros
		for (j = i + 1; j < tamanho; j++)
		{
			if (vet[j] < vet[menor]) //Se acharmos algo menor que o vetor na posi��o "menor", substitu�mos a vari�vel por j
			{
				menor = j;
			}
		}
		tmp = vet[i];		//
		vet[i] = vet[menor];// Trocando a "posi��o base" com a menor encontrada
		vet[menor] = tmp;	//
	}
}

void insertionSort (int tamanho)
{
	int i, j, tmp;
	
	for (i = 1; i < tamanho; i++)
	{
		j = i;
		while (vet[j] < vet[j-1] && j > 0)	//
		{									//
			tmp = vet[j];					//Looping que verifica se a posi��o atual � menor que a posi��o anterior.
			vet[j] = vet[j-1];				//Caso seja menor e j seja maior que 0, vamos troc�-las. 
			vet[j-1] = tmp;					//
			j--;							//
		}
	}
}
