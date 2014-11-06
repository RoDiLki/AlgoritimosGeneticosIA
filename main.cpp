#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <conio.h>
#include <time.h>
#include <algorithm>
#include <iostream>

int iter;

using namespace std;

vector<vector<int>> matEntrada;
vector<vector<int>> matIndividuos;
vector<float> mediasAptidao;
int qtIndividuos=0;



vector<int> calcAptidao(vector<vector <int>> mat){
	vector<int> vecAptidao;
	
	//printf("*******************************************************\n");
	for (int i = 0; i < mat.size(); i++){
		vecAptidao.push_back(0);
		
		for (int j = 0; j < mat[i].size() - 1; j++) {
			if (j >= 1){
				if (mat[i][j - 1] == mat[i][j + 1]) continue;
			}
			
			vecAptidao[i] += matEntrada[mat[i][j]][mat[i][j+1]];
		}

		//printf("Aptidao do individuo %d : %d", i, vecAptidao[i]);
		for (int w = 0; w < mat[i].size(); w++){
		//	printf(" %d -", mat[i][w]);
		}
		//printf("\n");
	}

	
	return vecAptidao;
}

void calculaMediaAptidoes(vector<int> vecAptidao){
	int total=0;
	for (int i = 0; i < vecAptidao.size(); i++){
		total += vecAptidao[i];
	}

	total /= vecAptidao.size()+0.0;
	mediasAptidao.push_back(total);
}
bool verificaRepeticao(vector<vector<int>> mat, vector<int> indv){

	int ax=0;

	if (mat.size() == 1)return false;

	for (int l = 0; l < mat.size(); l++){
		ax = 0;
		if (mat[l].size() != indv.size())return false;

		for (int c = 0; c < mat[l].size(); c++){
			if (mat[l][c] != indv[c]){
				ax = 1;
				break;
			}
		}
		if (ax == 0) return true;
	}
	
	return false;
}

vector<vector<int>> mutacao(vector<vector<int>> matIndiv){

	int qual = rand() % matIndiv.size();
	//printf(" %d\n", qual);
	do{
		int p1 = rand() % (matIndiv[qual].size() - 1) + 1;
		int p2 = p1;
		while (p2 == p1){
			p2 = rand() % (matIndiv[qual].size() - 1) + 1;
		}
		int aux = matIndiv[qual][p1];
		matIndiv[qual][p1] = matIndiv[qual][p2];
		matIndiv[qual][p2] = aux;

	} while (!verificaRepeticao(matIndiv, matIndiv[qual]));


	return matIndiv;
}


int	main()
{
	int opc =0;
	float percent = 0.8;
	

	string arq;
	printf("\nDigite o nome do arquivo de entrada seguido de .txt  : " );
	//cin >> arq;
	arq = "e.txt";
	ifstream infile(arq);
	string line;
	//Leitura da matriz do arquivo
	while (getline(infile, line)){
		istringstream iss(line);
		int n;
		vector<int> v;

		while (iss >> n){
			v.push_back(n);
		}

		matEntrada.push_back(v);
	}

	infile.close();

	printf("\n\nDigite quantas vezes o software deve buscar pela menor distância : ");
	//cin >> iter;
	iter = 100;
	//printf("\n\nDeseja entrar a quantidade de individuos? Valor default = 40 porcento da entrada\n(1-SIM 0-NAO) .");
	//cin >> opc;

	if (opc == 1){
		printf("Nova porcentagem de individuos : ");
		cin >> percent;
	}

	qtIndividuos = ceil(matEntrada.size() * percent);

	if ((qtIndividuos % 2) == 1)qtIndividuos++;

	printf("Tamanho entrada = %d  Individuos = %d\n", matEntrada.size(), qtIndividuos);


	vector<int> vecAptidao, vecAptidaoFilhos;
	int i, j, u, flag=0, check, op;

	//Iniciando geração de indivíduos
	//qtIndividuos = 5;//															VER DEPOIS
	srand(time(NULL));

	for (i = 0; i < qtIndividuos; ){
		
		vector<int> indiv;
		indiv.push_back(0);
		j = (rand() % (matEntrada.size() - 1)) + 1;
		indiv.push_back(j);
		do{
			u = rand() % 10;
			if (u < 3 && !flag){//vai repetir um nó.4
				flag = 1;
				j = indiv.size() - 2;
				indiv.push_back(j);
				
			}else{
				flag = 0;
				j = (rand() % (matEntrada.size() - 1)) + 1;
				for (int v = 0; v < indiv.size(); v++){
					if (indiv[v] == j){
						j = (rand() % (matEntrada.size() - 1)) + 1;
						v = 0;
					}
				}
				if (indiv[indiv.size() - 1] != j){
					indiv.push_back(j);
					flag = 1;
				}
			}
			check=0;
			for (int v = 0; v < indiv.size(); v++){
				if (indiv[v] == check){
					v = 0;
					check++;
				}
			}
		} while (check != matEntrada.size());
		
		
		int flag2=0;
		for (int w = 0; w < indiv.size() - 1; w++){
			//VERIFICAR NA MATRIZ DE ENTRADA SE NÃO É -1, ENTRE O W E W+1
			if (matEntrada[indiv[w]][indiv[w + 1]] == -1){
				flag2 = 1;
			}
		}
		if (!flag2 && !verificaRepeticao(matIndividuos, indiv)){
			//printf("Individuo %d:  ", matIndividuos.size());
			for (int w = 0; w < indiv.size(); w++){
				//printf("%d - ", indiv[w]);
			}
			//printf("\n");
			matIndividuos.push_back(indiv);
			i++;
		}
		
	}//termina de gerar os indivíduos


	vecAptidao = calcAptidao(matIndividuos); //calcula aptidão dos individuos

	i = 0;
	

	int selec = qtIndividuos / 2;
	int metade = floor(selec / 2);
	
	int flagMuta = 0, flagInterno =0;
	while (i<iter){

		printf("\n\nInicio de um ciclo: numero %d   muta %d \n",i,flagMuta);

		vector<int> indivSelec;
		vector<int> aptidaoCopia;

		aptidaoCopia = vecAptidao;

		sort(aptidaoCopia.begin(), aptidaoCopia.begin()+aptidaoCopia.size()); //ordena o vetor copia de aptidão do individuos

		//seleção dos individuos para crossover
		//escolha pelos menores e os maiores a partir das aptidoes dos indivudous ordenada (aptidaoCopia)
		for (int l = 0; l < metade; l++){ 
			indivSelec.push_back(aptidaoCopia[l]);
			indivSelec.push_back(aptidaoCopia[aptidaoCopia.size() - l - 1]);
		}

		//criação do vetor que armazenará os filhos resultantes do crossover
		vector<vector<int>> filhos; 

		int flag33 = 1;
		
		


		//pega os indices dos selecionados e passa para indivSelec que antes tinha as aptidões, agora tem os indices
		for (int l = 0; l < indivSelec.size() ; l++){
			for (int x = 0; x < vecAptidao.size(); x++){
				if (indivSelec[l] == vecAptidao[x]){
					vecAptidao[x] = 0;
					indivSelec[l] = x;
					break;
				}
			}
		}

		
		//processo de crossover
			
		do{
			vector<int> mascara;  //criação da máscara de crossover utilizando 80 porcento do tamanho minimo de cada individuo 
			int tmascara = matEntrada.size();
			//printf("masc =");
			for (int l = 0; l < tmascara ; l++){
				mascara.push_back(rand() % 2);
				//printf(" %d", mascara[l]);
			}
			//int inicioMascara = ceil(0.1*matEntrada.size());

			int inicioMascara = 0;

			for (int indv = 0; indv < indivSelec.size()-1; indv++){//pega os individuos para crossover
				for (int f = indv +1; f < indivSelec.size(); f++){

					filhos.push_back(matIndividuos[indivSelec[indv]]);

					for (int l = inicioMascara; l < tmascara; l++){//crossover mudando quando a máscara é 1 seguindo o tamanho dela
						//printf("\n%d - %d", filhos[filhos.size()-1][l], matIndividuos[indivSelec[f]][l]);
						if (mascara[l]==1){
							filhos[filhos.size() - 1][l] = matIndividuos[indivSelec[f]][l];
							//printf(" trocou if %d - %d", filhos[filhos.size() - 1][l], matIndividuos[indivSelec[f]][l]);
						}
					}
					//printf("\n\n");
					int check = 0; //verifica se o filho é válido
					for (int v = 0; v < filhos[filhos.size() - 1].size(); v++){

						if (filhos[filhos.size() - 1][v] == check){
							v = 0;
							check++;
						}
					}
					
					if (check != matEntrada.size() && filhos.size() > 0){
						filhos.pop_back();//filho inválido remove
					} else{
						//pobrema is here
						if (filhos.size() > 0){
							for (int w = 0; w < filhos[filhos.size() - 1].size() - 1; w++){
								//VERIFICAR NA MATRIZ DE ENTRADA SE NÃO É -1, ENTRE O W E W+1
								if (matEntrada[filhos[filhos.size() - 1][w]][filhos[filhos.size() - 1][w + 1]] == -1){
									filhos.pop_back(); //filho inválido por ter valor -1 em alguma ligação
									break;
								}
							}
						}
					}
				}
			}
			

		} while (filhos.size() == 0 );
		
		//trocar os filhos pelos pais


		//printf("Filhos\n");
		vecAptidaoFilhos = calcAptidao(filhos); //calcula aptidão dos filhos gerados
		vecAptidao = calcAptidao(matIndividuos);
		//realiza a troca "vezes" vezes.
		
		for (int vezes = 0; vezes < vecAptidaoFilhos.size(); vezes++){
			int maiorP = 0, menorF = 999, indiceP, indiceF;
			//procura o menor filho.
			for (int r = 0; r < vecAptidaoFilhos.size(); r++){
				if (vecAptidaoFilhos[r] < menorF){
					menorF = vecAptidaoFilhos[r];
					indiceF = r;
				}
			}

			vecAptidaoFilhos[indiceF] = 999;

			//procura o maior pai da matriz original de individuos selecionados
			for (int r = 0; r < vecAptidao.size(); r++){
				if (vecAptidao[r] > maiorP){
					maiorP = vecAptidao[r];
					indiceP = r;
				}
			}

			vecAptidao[indiceP] = 0;

			if (menorF < maiorP && !verificaRepeticao(matIndividuos,filhos[indiceF])){ //troca o filho "melhor" pelo pai "pior"
				printf("\nTROCOU indice %d\n",indiceP);
				matIndividuos[indiceP] = filhos[indiceF];
				flagInterno ++; //não será necessário mutação para os próximos 10 ciclos
			}
			

		}
		
		if (flagInterno != 0){
			flagMuta = 0;
			flagInterno = 0;
		}
		else{
			flagMuta++;
			flagInterno = 0;
		}
		if (flagMuta >= 10){
			printf("\nFez Mutacao\n");
			matIndividuos = mutacao(matIndividuos);
			flagMuta = 0; //não será necessário mutação para os próximos 10 ciclos
		}

		//verificar se só não faz o print

		vecAptidao = calcAptidao(matIndividuos); //recalcula a aptidão

		
		calculaMediaAptidoes(vecAptidao);
		filhos.clear();
		i++;
	}

	float taxaAprend;
	taxaAprend = mediasAptidao[0] - mediasAptidao[mediasAptidao.size() - 1];
	taxaAprend = taxaAprend / iter;
	printf("\n\n Media Final : %.04f \n\nTaixa de aprendizado por ciclo : %.04f \n\n",(mediasAptidao[mediasAptidao.size()- 1]),taxaAprend);




	system("pause");
	return 0;
}

