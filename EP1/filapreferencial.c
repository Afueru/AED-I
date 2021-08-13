#include "filapreferencial.h"
#include <windows.h>

PFILA criarFila(){
	PFILA res = (PFILA) malloc(sizeof(FILAPREFERENCIAL));
	res->inicio = NULL;
	res->fimPref = NULL;
	res->inicioNaoPref = NULL;
	res->fim = NULL;
	return res;
}

int tamanho(PFILA f){
	PONT atual = f->inicio;
	int tam = 0;
	while (atual) {
		atual = atual->prox;
		tam++;
	}
	return tam;
}

PONT buscarID(PFILA f, int id){
	PONT atual = f->inicio;
	 while (atual) {
		if (atual->id == id) return atual;
		atual = atual->prox;
	}
	return NULL;
}

void exibirLog(PFILA f){
	int numElementos = tamanho(f);
	printf("\nLog fila [elementos: %i] - Inicio:", numElementos);
	PONT atual = f->inicio;
	while (atual){
		printf(" [%i;%i]", atual->id, atual->idade);
		atual = atual->prox;
	}
	printf("\n\n");
}


int consultarIdade(PFILA f, int id){
	PONT atual = f->inicio;
	 while (atual) {
		if (atual->id == id) return atual->idade;
		atual = atual->prox;
	}
	return -1;
}



bool inserirPessoaNaFila(PFILA f, int id, int idade){
	if (idade >= 0 && id >= 0) {       //Checa se o id e a idade são válidos
		PONT atual = f->inicio;
		while (atual != NULL) {     //Checa se a pessoa de mesmo id ja está na fila
			if (atual->id == id) {
				return false;   //Poderia ter usado o buscarID....
			}
			else {
			atual = atual->prox;
			}
		}
		PONT p = (PONT) malloc(sizeof(ELEMENTO)); //Aloca memória para o novo elemento, já constatado que é valido
		p->id = id;
		p->idade = idade;
		if (p->idade >= IDADEPREFERENCIAL) {   //Insere preferencial
			if (f->inicio == NULL) {
				f->inicio = p;
				f->fimPref = p;
				f->fim = p;
				return true;
			}
			if (f->fimPref == NULL) {
				f->inicio = p;
			}
			else {
				f->fimPref->prox = p;
			}
			p->prox = f->inicioNaoPref;
			f->fimPref = p;
			return true;
		}
		else { //Insere não preferencial
			if (f->inicioNaoPref == NULL) {
				f->inicioNaoPref = p;
				if (f->fimPref != NULL) f->fimPref->prox = p;
				if (f->inicio == NULL) {
					f->inicio = p;
					f->fim = p;
					p->prox = NULL;
					return true;
				}
			}
			else {
				f->fim->prox = p;
			}
			f->fim = p;
			p->prox = NULL;
			return true;
		}
	}
	else {
	    return false;
	}
}

bool atenderPrimeiraDaFila(PFILA f, int* id){
	if (f->inicio == NULL) return false; //Caso a fila esteja vazia
	PONT p = f->inicio;
		if (f->inicio == f->fim) {     //Caso tenha apenas 1 elemento na fila
		*id = f->inicio->id;
		p->prox = NULL;
		free(p);
		f->fimPref = NULL;
		f->inicioNaoPref = NULL;
		f->inicio = NULL;
		f->fim = NULL;
		return true;
		}
		if (f->inicio == f->inicioNaoPref) {  //Caso haja apenas pessoas sem direito ao atendimento preferencial
			*id = f->inicio->id;
			f->inicio = p->prox;
			f->inicioNaoPref = p->prox;
			p->prox = NULL;
			free(p);
			return true;
		}
		if (f->fim == f->fimPref) {      //Caso haja apenas pessoas com direito ao atendimento preferencial
			*id = p->id;
			f->inicio = p->prox;
			p->prox = NULL;
			free(p);
			return true;
		}
		else {            //Caso haja pessoas com direito aos dois tipos de atendimento 
			*id = p->id;
			if (p == f->fimPref) {
				f->inicio = f->inicioNaoPref;
				f->fimPref = NULL;
				p->prox = NULL;
				free(p);
				return true;
			}                                          //Último erro por aqui!
			f->inicio = p->prox;
			p->prox = NULL;
			free(p);			
			return true;
			f->fimPref->prox = f->inicioNaoPref;
		}

	}


bool desistirDaFila(PFILA f, int id){
	PONT p = f->inicio;
	PONT ant;
	while (p->id != id) { //Checa se a pessoa com este ID está na fila
		ant = p;
		if (ant == f->fim) return false;                          //Erro aqui
		p = p->prox;
	}
	if (p == f->inicio) {   //Caso p seja o primeiro da fila
		return atenderPrimeiraDaFila(f,&id);
	}
	if (p == f->fimPref) {     //Caso p seja o último preferencial 
		f->fimPref = ant;
		ant->prox = f->inicioNaoPref;
		free(p);
		return true;	
	}
	if (p == f->fim) {          //Caso p seja o último
		if (p == f->inicioNaoPref) f->inicioNaoPref = NULL;
		f->fim = ant;
		ant->prox = NULL;
		free(p);
		return true;
	}
	if (p == f->inicioNaoPref) {
		f->inicioNaoPref = p->prox;
		ant->prox = p->prox;     //ant->prox = p->prox
		free(p);
		return true;
	}
	else {             //A pessoa está na fila, não é o ultimo nem o último preferencial, também não é o primeiro da fila e não é o primeiro não preferencial!
		ant->prox = p->prox;
		free(p);
		return true;		
	}

}

