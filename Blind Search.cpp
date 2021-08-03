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

std::vector<const char *> pilhaPlayer1;
tipo_PosicaoPlano posPlayer2;

void init_Player1() {
	//Desmonta a pilha
	while (pilhaPlayer1.size() > 0) {
		pilhaPlayer1.pop_back();
	}
}
const char *run_Player1() {
	const char *movimento, *origem;
	int confirma = 0;

	if(pilhaPlayer1.size() < 1){
		if(maze_VerCaminho("oeste") == CAMINHO)movimento = "oeste";
		else if (maze_VerCaminho("sul") == CAMINHO)movimento = "sul";
		else if (maze_VerCaminho("leste") == CAMINHO)movimento = "leste";
		else movimento = "norte";
	}
	else{
        //Verifica o caminho de origem
        origem = pilhaPlayer1[pilhaPlayer1.size()];
        if(origem == id_Caminhos[1]) origem = id_Caminhos[2];
        else if(origem == id_Caminhos[2]) origem = id_Caminhos[1];
        else if(origem == id_Caminhos[3]) origem = id_Caminhos[4];
        else if(origem ==id_Caminhos[4]) origem = id_Caminhos[3];

        while(confirma == 0){
            //Escolhe o proximo movimento aleatoriamente
            int move = rand()%4;
            movimento = id_Caminhos[move];

            //Verifica se o movimento vai pra origem, se for, escolhe outro movimento aleatoriamente
            if((movimento == "oeste" && origem == "oeste")||(movimento == "oeste" && origem == "oeste")
               ||(movimento == "oeste" && origem == "oeste")||(movimento == "oeste" && origem == "oeste")){
                confirma = 0;
            }
            else confirma = 1;
        }
	}

    //Salva o movimento no vetor
    if(movimento == "oeste") pilhaPlayer1.push_back(id_Caminhos[3]);
    else if(movimento == "sul") pilhaPlayer1.push_back(id_Caminhos[2]);
    else if(movimento == "leste") pilhaPlayer1.push_back(id_Caminhos[4]);
    else if(movimento == "norte") pilhaPlayer1.push_back(id_Caminhos[1]);

    //Retorna o movimento a ser feito
	return movimento;
}

// *** 	FUNCOES DE INICIALIZACAO E EXECUCAO DO JOGADOR 2 ***
//	Implementacao da segunda estrategia de jogo.
void init_Player2() {
	posPlayer2.x = -1;
	posPlayer2.y = -1;
}
const char *run_Player2() {
	const char *movimento;

	if(posPlayer2.x == -1){	//Se for a posicao original ele verifica se tem caminho livre e anda, comecando pelo oeste
		if(maze_VerCaminho("oeste") == CAMINHO) movimento = "oeste";
		else if (maze_VerCaminho("sul") == CAMINHO) movimento = "sul";
		else if (maze_VerCaminho("leste") == CAMINHO) movimento = "leste";
		else movimento = "norte";
	}
	else{
		if(posPlayer2.x == posAtualP2.x && posPlayer2.y == posAtualP2.y){	//Se a posicao que o player estiver for igual a anterior,
			int move = rand()%4;
			movimento = id_Caminhos[move];
		}
		else{
			if(maze_VerCaminho("oeste") == CAMINHO) movimento = "oeste";
			else if (maze_VerCaminho("sul") == CAMINHO) movimento = "sul";
			else if (maze_VerCaminho("leste") == CAMINHO) movimento = "leste";
			else movimento = "norte";
		}
	}

	posPlayer2.x = posAtualP2.x;
	posPlayer2.y = posAtualP2.y;
	return movimento;
}


