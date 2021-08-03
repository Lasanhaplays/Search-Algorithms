/*
	Autor: Luis Otavio Rigo Junior
	Objetivo: 
		Este arquivo destina-se a implementacao das estrategias de jogo dos agentes.
	
	Devem ser implementados os 4 prototipos:
		init_Player1 - executada uma unica vez e contem as funcoes de inicializacao da estrategia do jogador 1;
		run_Player1 - executado a cada passo do jogo e retorna uma string com a direcao de movimento do jogador 1;
		init_Player2 - executada uma unica vez e contem as funcoes de inicializacao da estrategia do jogador 2;
		run_Player2 - executado a cada passo do jogo e retorna uma string com a direcao de movimento do jogador 2.
	
	Funcoes que podem ser chamadas pelo jogador (agente):
		char *maze_VerAmbiente(char tipo[MAXLINE]);
			- Utilizada para verificar alguma informacao da celula. Ex.: id;
		bool maze_VerCaminho(const char *direcao);
			- Retorna se existe um caminho naquela direcao (verdadeiro ou falso).
		bool maze_VerMinotauro(const char *direcao);
			- Retorna se o minotauro estah naquela direcao (verdadeiro ou falso).
		float maze_CustoDoCaminho(const char *direcao);
			- Retorna o custo de executar um passo naquela direcao.
		float maze_HeuristicaDistEuclidiana(const char *direcao);
			- Retorna o a distancia heuclidiana da celula que estah naquela direcao ateh a saida.
	
	Constantes que podem ser usadas pelo jogador (agente):
		#define NUMCAMINHOS 4
		const char *id_Caminhos[NUMCAMINHOS] = {"norte", "sul", "oeste", "leste"};
		const char *id_Retornos[NUMCAMINHOS] = {"sul", "norte", "leste", "oeste"};
		typedef struct str_PosicaoPlano {
			int x,y;
		} tipo_PosicaoPlano;
*/

// *** 	FUNCOES DE INICIALIZACAO E EXECUCAO DO JOGADOR 1 ***
//	Implementacao da primeira estrategia de jogo.

#include "globais.h"
#include "maze.h"
#include <queue>
#include <stack>
#include <string>
#include <iostream>
#include <ostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

using namespace std; 
/** 
 * Estrategias de busca guiada
 * 
 * @author Arlon Scheidegger
*/

//Auxiliares globais
//Player 1
stack<string> pilha_caminho_player1;
stack<string> pilha_caminho_retorno_player1;
int sem_fim[60][35][4]; //Armazena os dados dos caminhos que sao sem fim para cada celula
int visitado_player1[60][35];
tipo_PosicaoPlano posicao_atual_player_1;
string direcao_origem_player_1 = "";

//Funcoes auxiliares

/** 
 * Retorna a direcao oposta da passada como parametro
*/
string get_opposite_direction(const char *direction){
	
	string str(direction);
	string opposite_direction;

	if (str == "leste") {
		opposite_direction = "oeste";
	}else if(str == "sul") {
		opposite_direction = "norte";
	}else if(str == "oeste") {
		opposite_direction = "leste";
	}else if(str == "norte") {
		opposite_direction = "sul";
	}

	return opposite_direction;
}

void PrintStack(stack<string> s) 
{ 
    // If stack is empty then return 
    if (s.empty())  
        return; 
      
  
    string x = s.top(); 
  
    // Pop the top element of the stack 
    s.pop(); 
  
    // Recursively call the function PrintStack 
    PrintStack(s); 
  
    // Print the stack element starting 
    // from the bottom 
    cout << x << "\n "; 
  
    // Push the same element onto the stack 
    // to preserve the order 
    s.push(x); 
}

tipo_PosicaoPlano update_position(const char* movimento, tipo_PosicaoPlano posicao_atual){
	string str(movimento);

	if (str == "leste") {
		posicao_atual.x++;
	}else if(str == "sul") {
		posicao_atual.y++;
	}else if(str == "oeste") {
		posicao_atual.x--;
	}else if(str == "norte") {
		posicao_atual.y--;
	}

	return posicao_atual;
}

//Dado uma string de movimento retorna o numero correspondente a posicao no enum de caminhos
int caminho_to_number(const char* movimento){
	string str(movimento);
	int caminho_num;

	if (str == "leste") {
		caminho_num = 3;
	}else if(str == "sul") {
		caminho_num = 1;
	}else if(str == "oeste") {
		caminho_num = 2;
	}else if(str == "norte") {
		caminho_num = 0;
	}

	return caminho_num;
}

bool is_all_caminhos_explorados(int visitados[60][35], tipo_PosicaoPlano posicao){
	bool resposta = true;

	if (maze_VerCaminho("leste") == CAMINHO && posicao.x < 64) {
		if(visitados[posicao.x+1][posicao.y] != 1){
			resposta = false;
		}
	}
	if (maze_VerCaminho("oeste") == CAMINHO && posicao.x > 0) {
		if(visitados[posicao.x-1][posicao.y] != 1){
			resposta = false;
		}
	}
	if (maze_VerCaminho("sul") == CAMINHO && posicao.x < 64) {
		if(visitados[posicao.x][posicao.y+1] != 1){
			resposta = false;
		}
	}
	if (maze_VerCaminho("norte") == CAMINHO && posicao.x < 64) {
		if(visitados[posicao.x][posicao.y-1] != 1){
			resposta = false;
		}
	}

	return resposta;
}

int quantidade_caminhos(){
	int qtd_caminhos = 0;
	if (maze_VerCaminho("leste") == CAMINHO) {
		qtd_caminhos++;
	}
	if (maze_VerCaminho("oeste") == CAMINHO) {
		qtd_caminhos++;
	}
	if (maze_VerCaminho("sul") == CAMINHO) {
		qtd_caminhos++;
	}
	if (maze_VerCaminho("norte") == CAMINHO) {
		qtd_caminhos++;
	}

	return qtd_caminhos;
}

string my_number_to_caminho(int number){
	switch(number){
		case 0:
		return "leste";
		break;
		case 1:
		return "sul";
		break;
		case 2:
		return "oeste";
		break;
		case 3:
		return "norte";
		break;
		default:
		return "";
		break;
	}
}

bool check_minotauro(){
	return maze_VerMinotauro("leste") || maze_VerMinotauro("oeste") || maze_VerMinotauro("sul") || maze_VerMinotauro("norte");
}
//Estrategias

int isRunningP1 = 0;
const char *movimento_anteriorP1;
//	Esta estrategia e a de busca gulosa, o agente caminha sempre em direcao ao caminho de menor distancia ate a saida, voltando apenas quando encontra um beco sem saida
void init_Player1() {
	for(int i = 0; i < 60; i++){
		for(int j = 0; j < 35; j++){
			visitado_player1[i][j] = 0;
			for(int k = 0; k < 4; k++){
				sem_fim[i][j][k] = 0;
			}
		}
	}

	posicao_atual_player_1.x = 0;
	posicao_atual_player_1.y = 0;
}


const char *run_Player1() {
	string movimento = "";
	float custo;
	bool sem_movimentos = false;
	float distancias[4] = {-1, -1, -1, -1};
	bool all_checked = false;
	int index_min = -1;
	bool minotauro = check_minotauro();
	int count = 0;
	
	// if(minotauro){
	// 	cout << "True \n";
	// }else{
	// 	cout << "False \n";
	// }

	//Caso celula ainda nao foi visitada adiciona seus possiveis caminhos a pilha
	if((visitado_player1[posicao_atual_player_1.x][posicao_atual_player_1.y] == 0 && !minotauro) || isRunningP1 == 1){
		if (isRunningP1 != 1)	visitado_player1[posicao_atual_player_1.x][posicao_atual_player_1.y] = 1;

		if (maze_VerCaminho("leste") == CAMINHO && direcao_origem_player_1 != "leste" && sem_fim[posicao_atual_player_1.x][posicao_atual_player_1.y][3] != 1) {
			distancias[0] = maze_HeuristicaDistEuclidiana("leste");
		}
		if (maze_VerCaminho("sul") == CAMINHO && direcao_origem_player_1 != "sul" && sem_fim[posicao_atual_player_1.x][posicao_atual_player_1.y][1] != 1) {
			distancias[1] = maze_HeuristicaDistEuclidiana("sul");
		}
		if (maze_VerCaminho("oeste") == CAMINHO && direcao_origem_player_1 != "oeste" && sem_fim[posicao_atual_player_1.x][posicao_atual_player_1.y][2] != 1) {
			distancias[2] = maze_HeuristicaDistEuclidiana("oeste");
		}
		if (maze_VerCaminho("norte") == CAMINHO && direcao_origem_player_1 != "norte" && sem_fim[posicao_atual_player_1.x][posicao_atual_player_1.y][0] != 1) {	
			distancias[3] = maze_HeuristicaDistEuclidiana("norte");	
		}

		all_checked = true;
		
		//cout << posicao_atual_player_1.x << " , " << posicao_atual_player_1.y << " : ";

		for (int i = 0; i < 4; i++)
		{
			if(distancias[i] != 0){
				all_checked = false;
			}

			//cout << distancias[i] << " / ";
		}
		
		//cout << "\n";

		while(!all_checked){
			// Checa a existencia de um caminho em todas as direcoes e escolhe o de menor distancia a saida, as adiciona a pilha de caminhos a serem visitados e marca como checkado
			custo = -1;
			movimento = "";

			//cout << "TO aqui \n";

			//Insere os maiores primeiros na pilha, de forma que o menor seja o proximo passo
			for(int i = 0; i < 4; i++){
				if(distancias[i] != -1){
					if(distancias[i] > custo){
						custo = distancias[i];
						movimento = my_number_to_caminho(i);
						index_min = i;
					}
				}
			}

			if(movimento != ""){
				count++;
				pilha_caminho_player1.push(movimento);
				distancias[index_min] = -1;
			} else {
				all_checked = true;
			}


		}

	}

	/* DEBUG

	cout << "Pos: " << posicao_atual_player_1.x << "," << posicao_atual_player_1.y  << "\n----------------\n";
	cout << "Todos Caminhos Visitados?: " << is_all_caminhos_explorados(visitado_player1, posicao_atual_player_1) << "\n----------------\n";
	PrintStack(pilha_caminho_player1);
	cout << "-------------------- \n";
	PrintStack(pilha_caminho_retorno_player1);
	cout << "==================== \n";
	*/

	const char *proximo_movimento;
		printf("entrou p1 - %d", count);
	if(isRunningP1 == 1 && count > 1) {
		for(int c = 0; c < count; c++){
			if(pilha_caminho_player1.top().c_str() != get_opposite_direction(movimento_anteriorP1)){
				isRunningP1 = 0;
				break;
			} else {
				pilha_caminho_player1.pop();
			}
		}
		
		
	}
	
	//Caso nao encontre nenhum caminho retorne para o anterior e o marca como sem fim
	if(index_min == -1 && is_all_caminhos_explorados(visitado_player1, posicao_atual_player_1) || isRunningP1 == 1){
		proximo_movimento = pilha_caminho_retorno_player1.top().c_str();
		direcao_origem_player_1 = get_opposite_direction(proximo_movimento);
		pilha_caminho_retorno_player1.pop();
		//atualiza posicao atual dado o proximo movimento
		posicao_atual_player_1 = update_position(proximo_movimento, posicao_atual_player_1);

		if(!minotauro && isRunningP1 == 0){
			//Marca que o caminho para esta celula a partir da celula anterior eh sem fim caso o retorno nao seja devido ao minotauro
			sem_fim[posicao_atual_player_1.x][posicao_atual_player_1.y][caminho_to_number(direcao_origem_player_1.c_str())] = 1; 
		}else{

			//Caso seja devido ao minotauro marca a celula para a qual o agente ira voltar como nao visitada novamente
			sem_fim[posicao_atual_player_1.x][posicao_atual_player_1.y][caminho_to_number(direcao_origem_player_1.c_str())] = 0; 
			visitado_player1[posicao_atual_player_1.x][posicao_atual_player_1.y] = 0;
			isRunningP1 = 1;
		}
	} else{ 
		//Retira o proximo movimento da pilha e colocar qual movimento deve ser realizado para retonar no topo da pilha de retorno
		proximo_movimento = pilha_caminho_player1.top().c_str();
		pilha_caminho_retorno_player1.push(get_opposite_direction(proximo_movimento));
		direcao_origem_player_1 = get_opposite_direction(proximo_movimento);
		pilha_caminho_player1.pop();
		//atualiza posicao atual dado o proximo movimento
		posicao_atual_player_1 = update_position(proximo_movimento, posicao_atual_player_1);
	}

		
	movimento_anteriorP1 = proximo_movimento;
	return proximo_movimento;
}

// *** 	FUNCOES DE INICIALIZACAO E EXECUCAO DO JOGADOR 2 ***
//	Implementacao da segunda estrategia de jogo.
//	Esta estrategia consiste em A*

// Classe que armazena movimento e 
 
typedef struct MovimentoAstar
{
	string movimento;
   	float custo;
	tipo_PosicaoPlano posicao;
} MovimentoAStar;


// To compare two points
class myComparator
{
public:
    int operator() (const MovimentoAStar& p1, const MovimentoAStar& p2)
    {
        return p1.custo >= p2.custo;
    }
};

priority_queue <MovimentoAStar, vector<MovimentoAStar>, myComparator > queue_movimentos;
priority_queue <MovimentoAStar, vector<MovimentoAStar>, myComparator > queue_movimentos_retorno;

void print_queue(priority_queue<MovimentoAStar, vector<MovimentoAStar>, myComparator > pq){
	while (! pq.empty() ) {
		cout << pq.top().movimento << " - " << pq.top().custo << " - " << pq.top().posicao.x << " , " << pq.top().posicao.y << "\n";
		pq.pop();
	}
}

//Player 2
int visitado_player2[60][35];
stack<string> pilha_caminho_player2;
stack<string> pilha_caminho_retorno_player2;
stack<string> pilha_caminho_retorno_player2_2;
tipo_PosicaoPlano posicao_atual_player_2;
string direcao_origem_player_2 = "";
int sem_fim_2[60][35][4]; //Armazena os dados dos caminhos que sao sem fim para cada celula
bool usei_queue = false;
bool voltando = false;
bool voltar = false;
int isRunningP2 = 0;
const char *movimento_anterior;

void init_Player2() {
	for(int i = 0; i < 60; i++){
		for(int j = 0; j < 35; j++){
			visitado_player2[i][j] = 0;
		}
	}

	posicao_atual_player_2.x = 0;
	posicao_atual_player_2.y = 0;
}

const char *run_Player2() {

	string movimento = "";
	float custo;
	bool sem_movimentos = false;
	float distancias[4] = {-1, -1, -1, -1};
	bool all_checked = false;
	int index_min = -1;
	bool minotauro = check_minotauro();
	int count = 0;
	
	// if(minotauro){
	// 	cout << "True \n";
	// }else{
	// 	cout << "False \n";
	// }

	//Caso celula ainda nao foi visitada adiciona seus possiveis caminhos a pilha
	if((visitado_player2[posicao_atual_player_2.x][posicao_atual_player_2.y] == 0 && !minotauro) || isRunningP2 == 1){
		
		if (isRunningP2 != 1)	visitado_player2[posicao_atual_player_2.x][posicao_atual_player_2.y] = 1;

		if (maze_VerCaminho("leste") == CAMINHO && direcao_origem_player_2 != "leste" && sem_fim_2[posicao_atual_player_2.x][posicao_atual_player_2.y][3] != 1) {
			distancias[0] = maze_CustoDoCaminho("leste");
		}
		if (maze_VerCaminho("sul") == CAMINHO && direcao_origem_player_2 != "sul" && sem_fim_2[posicao_atual_player_2.x][posicao_atual_player_2.y][1] != 1) {
			distancias[1] = maze_CustoDoCaminho("sul");
		}
		if (maze_VerCaminho("oeste") == CAMINHO && direcao_origem_player_2 != "oeste" && sem_fim_2[posicao_atual_player_2.x][posicao_atual_player_2.y][2] != 1) {
			distancias[2] = maze_CustoDoCaminho("oeste");
		}
		if (maze_VerCaminho("norte") == CAMINHO && direcao_origem_player_2 != "norte" && sem_fim_2[posicao_atual_player_2.x][posicao_atual_player_2.y][0] != 1) {	
			distancias[3] = maze_CustoDoCaminho("norte");	
		}

		all_checked = true;
		
		//cout << posicao_atual_player_2.x << " , " << posicao_atual_player_2.y << " : ";

		for (int i = 0; i < 4; i++)
		{
			if(distancias[i] != 0){
				all_checked = false;
			}

			//cout << distancias[i] << " / ";
		}
		
		//cout << "\n";

		while(!all_checked){
			// Checa a existencia de um caminho em todas as direcoes e escolhe o de menor distancia a saida, as adiciona a pilha de caminhos a serem visitados e marca como checkado
			custo = -1;
			movimento = "";

			//cout << "TO aqui \n";

			//Insere os maiores primeiros na pilha, de forma que o menor seja o proximo passo
			for(int i = 0; i < 4; i++){
				if(distancias[i] != -1){
					if(distancias[i] > custo){
						custo = distancias[i];
						movimento = my_number_to_caminho(i);
						index_min = i;
					}
				}
			}

			if(movimento != ""){
				count++;
				pilha_caminho_player2.push(movimento);
				distancias[index_min] = -1;
			} else {
				all_checked = true;
			}


		}

	}

	/* DEBUG

	cout << "Pos: " << posicao_atual_player_2.x << "," << posicao_atual_player_2.y  << "\n----------------\n";
	cout << "Todos Caminhos Visitados?: " << is_all_caminhos_explorados(visitado_player2, posicao_atual_player_2) << "\n----------------\n";
	PrintStack(pilha_caminho_player1);
	cout << "-------------------- \n";
	PrintStack(pilha_caminho_retorno_player1);
	cout << "==================== \n";
	*/

	const char *proximo_movimento;
	printf("entrou p2 - %d\n", count);
	if(isRunningP2 == 1 && count > 1) {
		for(int c = 0; c < count; c++){
			if(pilha_caminho_player2.top().c_str() != get_opposite_direction(movimento_anterior)){
				isRunningP1 = 0;
				break;
			} else {
				pilha_caminho_player2.pop();
			}
		}
		
		
	}

	//Caso nao encontre nenhum caminho retorne para o anterior e o marca como sem fim
	if(index_min == -1 && is_all_caminhos_explorados(visitado_player2, posicao_atual_player_2) ){
		proximo_movimento = pilha_caminho_retorno_player2.top().c_str();
		direcao_origem_player_2 = get_opposite_direction(proximo_movimento);
		pilha_caminho_retorno_player2.pop();
		//atualiza posicao atual dado o proximo movimento
		posicao_atual_player_2 = update_position(proximo_movimento, posicao_atual_player_2);
		if(!minotauro && isRunningP2 == 0){
			//Marca que o caminho para esta celula a partir da celula anterior eh sem fim caso o retorno nao seja devido ao minotauro
			sem_fim_2[posicao_atual_player_2.x][posicao_atual_player_2.y][caminho_to_number(direcao_origem_player_2.c_str())] = 1; 
		}else{

			//Caso seja devido ao minotauro marca a celula para a qual o agente ira voltar como nao visitada novamente
			sem_fim_2[posicao_atual_player_2.x][posicao_atual_player_2.y][caminho_to_number(direcao_origem_player_2.c_str())] = 0; 
			visitado_player2[posicao_atual_player_2.x][posicao_atual_player_2.y] = 0;
			isRunningP2 = 1;
		}
	} else{ 
		//Retira o proximo movimento da pilha e colocar qual movimento deve ser realizado para retonar no topo da pilha de retorno
		proximo_movimento = pilha_caminho_player2.top().c_str();
		pilha_caminho_retorno_player2.push(get_opposite_direction(proximo_movimento));
		direcao_origem_player_2 = get_opposite_direction(proximo_movimento);
		pilha_caminho_player2.pop();
		//atualiza posicao atual dado o proximo movimento
		posicao_atual_player_2 = update_position(proximo_movimento, posicao_atual_player_2);
	}

		
	movimento_anterior = proximo_movimento;

	return proximo_movimento;
}



// const char *run_Player2() {

// 	// if(maze_VerCaminho("leste") == CAMINHO){
// 	// 	cout << "leste: " << tLab_CustoDoCaminho("leste") << " / ";
// 	// }
// 	// if(maze_VerCaminho("oeste") == CAMINHO){
// 	// 	cout << "oeste: " << tLab_CustoDoCaminho("oeste") << " / ";
// 	// }
// 	// if(maze_VerCaminho("norte") == CAMINHO){
// 	// 	cout << "norte: " << tLab_CustoDoCaminho("norte") << " / ";
// 	// }
// 	// if(maze_VerCaminho("sul") == CAMINHO){
// 	// 	cout << "sul: " << tLab_CustoDoCaminho("sul") << " / ";
// 	// }

// 	// cout << "\n";


// 	//A Fazer:
// 	// Se mover apenas se proxima posicao ter menor custo que a posicao no topo da pilha
// 	// Se nao houver deve-se retornar ate onde tem

// 	string movimento = "";
// 	float custo;
// 	float distancia;
// 	bool minotauro = check_minotauro();
// 	MovimentoAStar new_move;

// 	if(usei_queue){
// 		queue_movimentos.pop();
// 		usei_queue = false;
// 	}

	
// 	// if(minotauro){
// 	// 	cout << "True \n";
// 	// }else{
// 	// 	cout << "False \n";
// 	// }

// 	//Caso celula ainda nao foi visitada adiciona seus possiveis caminhos a pilha

// 	cout << (visitado_player2[posicao_atual_player_2.x][posicao_atual_player_2.y] == 0 ? "Nao visitado" : "Visitado") << "\n";
// 	cout << "Direcao ORIGEM: " << direcao_origem_player_2 <<"\n";
	
// 	if(visitado_player2[posicao_atual_player_2.x][posicao_atual_player_2.y] == 0){
// 		visitado_player2[posicao_atual_player_2.x][posicao_atual_player_2.y] = 1;

// 		if (maze_VerCaminho("leste") == CAMINHO && direcao_origem_player_2 != "leste") {
// 			distancia = maze_HeuristicaDistEuclidiana("leste");
// 			movimento = "leste";
// 			new_move.movimento = movimento;
// 			new_move.custo = distancia;
// 			new_move.posicao.x = posicao_atual_player_2.x;
// 			new_move.posicao.y = posicao_atual_player_2.y;
// 			queue_movimentos.push(new_move);
// 		}
// 		if (maze_VerCaminho("sul") == CAMINHO && direcao_origem_player_2 != "sul") {
// 			distancia = maze_HeuristicaDistEuclidiana("sul");
// 			movimento = "sul";
// 			new_move.movimento = movimento;
// 			new_move.custo = distancia;
// 			new_move.posicao.x = posicao_atual_player_2.x;
// 			new_move.posicao.y = posicao_atual_player_2.y;
// 			queue_movimentos.push(new_move);
// 		}
// 		if (maze_VerCaminho("oeste") == CAMINHO && direcao_origem_player_2 != "oeste") {
// 			distancia = maze_HeuristicaDistEuclidiana("oeste");
// 			movimento = "oeste";
// 			new_move.movimento = movimento;
// 			new_move.custo = distancia;
// 			new_move.posicao.x = posicao_atual_player_2.x;
// 			new_move.posicao.y = posicao_atual_player_2.y;
// 			queue_movimentos.push(new_move);
// 		}
// 		if (maze_VerCaminho("norte") == CAMINHO && direcao_origem_player_2 != "norte") {	
// 			distancia = maze_HeuristicaDistEuclidiana("norte");
// 			movimento = "norte";
// 			new_move.movimento = movimento;
// 			new_move.custo = distancia;
// 			new_move.posicao.x = posicao_atual_player_2.x;
// 			new_move.posicao.y = posicao_atual_player_2.y;
// 			queue_movimentos.push(new_move);
// 		}

// 	}

// 	//DEBUG

// 	cout << "Pos: " << posicao_atual_player_2.x << "," << posicao_atual_player_2.y  << "\n----------------\n";
// 	//cout << "Todos Caminhos Visitados?: " << is_all_caminhos_explorados(visitado_player2, posicao_atual_player_2) << "\n----------------\n";
// 	print_queue(queue_movimentos);
// 	cout << "-------------------- \n";
// 	//PrintStack(pilha_caminho_retorno_player2);
// 	//cout << "==================== \n";
	

// 	int a;
// 	// if(posicao_atual_player_2.x == 3 && posicao_atual_player_2.y == 4)
// 	// 	cin >> a;

// 	const char *proximo_movimento_2;
// 	MovimentoAStar current_move;
	
// 	//Caso nao encontre nenhum caminho retorne para o anterior e o marca como sem fim
// 	if(queue_movimentos.top().posicao.x == posicao_atual_player_2.x && queue_movimentos.top().posicao.y == posicao_atual_player_2.y){
// 		if(posicao_atual_player_2.x != 0 && posicao_atual_player_2.y != 0 && voltando){
// 			voltar = true;
// 			voltando = false;
// 		}
// 		//Retira o proximo movimento da pilha e colocar qual movimento deve ser realizado para retonar no topo da pilha de retorno
// 		//proximo_movimento_2 = queue_movimentos.top().getMovimento().c_str();
// 		current_move = queue_movimentos.top();
// 		proximo_movimento_2 =   (queue_movimentos.top()).movimento.c_str();
// 		direcao_origem_player_2 = get_opposite_direction(proximo_movimento_2);
// 		//cout << "Direcao ORIGEM: " << direcao_origem_player_2 << " PROXIMO: " << proximo_movimento_2 <<"\n";
// 		pilha_caminho_retorno_player2.push(direcao_origem_player_2);
		
// 		//cout << "Direcao ORIGEM: " << direcao_origem_player_2 << " PROXIMO: " << proximo_movimento_2 <<"\n";
// 		usei_queue = true;
// 		//atualiza posicao atual dado o proximo movimento
// 		posicao_atual_player_2 = update_position(proximo_movimento_2, posicao_atual_player_2);
// 		cout <<"Fui pra frente \n";

// 	} else{
// 		if(voltar){
// 			proximo_movimento_2 = pilha_caminho_retorno_player2_2.top().c_str();
// 			pilha_caminho_retorno_player2_2.pop();
// 			if(pilha_caminho_retorno_player2_2.empty()){
// 				voltar = false;
// 			}
// 		}else{
// 			proximo_movimento_2 = pilha_caminho_retorno_player2.top().c_str();
// 			pilha_caminho_retorno_player2_2.push(get_opposite_direction(proximo_movimento_2));
// 			pilha_caminho_retorno_player2.pop();
// 			voltando = true;
// 		}


// 		direcao_origem_player_2 = get_opposite_direction(proximo_movimento_2);

// 		//atualiza posicao atual dado o proximo movimento
// 		posicao_atual_player_2 = update_position(proximo_movimento_2, posicao_atual_player_2);
// 		cout <<"Voltei \n";
// 		//sem_fim_2[posicao_atual_player_2.x][posicao_atual_player_2.y][caminho_to_number(direcao_origem_player_2.c_str())] = 1; 
// 		// if(!minotauro){
// 		// 	//Marca que o caminho para esta celula a partir da celula anterior eh sem fim caso o retorno nao seja devido ao minotauro
// 		// }else{

// 		// 	//Caso seja devido ao minotauro marca a celula para a qual o agente ira voltar como nao visitada novamente
// 		// 	visitado_player2[posicao_atual_player_2.x][posicao_atual_player_2.y] == 0;
// 		// } 
		
// 	}

// 	cout << "Proximo Movimento: " << proximo_movimento_2 << "\n";
	
// 	// if(posicao_atual_player_2.x == 3 && posicao_atual_player_2.y == 4)
// 	// 	cin >> a;

// 	return proximo_movimento_2;
// }


