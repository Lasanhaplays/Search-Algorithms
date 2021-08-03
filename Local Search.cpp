/*
	Autor: Luis Otavio Rigo Junior
	Objetivo:
		Implementacao das estrategias definidas pelo programador.
*/
// *** 	FUNCOES DE INICIALIZACAO E EXECUCAO DO JOGADOR 1 ***
// Funcao de inicializacao e execucao da estrategia 1.
tipo_Valor melhorP1;
void init_Player1() {
	// Determina posicao inicial.
	reinicializa_PosicaoAleatoria();
	
	// Implementacao das rotinas de inicializacao.
	melhorP1 = superf_VerAmbiente();
}
tipo_Movimento run_Player1() {
	tipo_Movimento movimento;
	
	// Define a direcao do movimento.
	movimento.direcao = id_Caminhos[rand()%NUMCAMINHOS];
	
	// Define o tamanho do passo.
	movimento.passo = 2;
	
	return movimento;
}
tipo_Valor result_Player1() {
	return melhorP1;
}


// *** 	FUNCOES DE INICIALIZACAO E EXECUCAO DO JOGADOR 2 ***
//	Implementacao da segunda estrategia de jogo.
tipo_Valor melhorP2;
void init_Player2() {
	// Determina posicao inicial.
	reinicializa_PosicaoAleatoria();
	
	// Implementacao das rotinas de inicializacao.
	melhorP2 = superf_VerAmbiente();
}
tipo_Movimento run_Player2() {
	tipo_Movimento movimento;
	
	// Define a direcao do movimento.
	movimento.direcao = id_Caminhos[rand()%NUMCAMINHOS];
	
	// Define o tamanho do passo.
	movimento.passo = 5;
	
	return movimento;
}
tipo_Valor result_Player2() {
	return melhorP2;
}


