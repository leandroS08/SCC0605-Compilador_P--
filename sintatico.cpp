#include <iostream>
#include <string> 
#include <queue>
#include "elemento.h"
#include "sintatico.h"

using namespace std;

bool ProcedimentoASD(queue<Node> tabela){
    Node iter = tabela.front();
    tabela.pop();

    if(iter.getToken().compare("simb_program") == 0)
    {
        cout << "achou!" << endl;
        iter = tabela.front();
        tabela.pop();

        if(iter.getToken().compare("id") == 0){
            cout << "achou!" << endl;
            iter = tabela.front();
            tabela.pop();

            if(iter.getToken().compare("simb_pv") == 0){
                cout << "achou!" << endl;
                ProcedimentoCorpo(&tabela);
            }
            //else ErroSintatico();
        }
        //else ErroSintatico();
    }
    //else ErroSintatico();

    iter = tabela.front();
    tabela.pop();

    if(iter.getToken().compare("simb_pont") == 0){
        if(tabela.empty()) return true;
        //else ErroSintatico();
    }
    //else ErroSintatico();
}

bool ProcedimentoCorpo(queue<Node>* tabela)
{
    // A gramática a partir desse ponto não é LL1, não da pra avançar sem mudar ela
}

