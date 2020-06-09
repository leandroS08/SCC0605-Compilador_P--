#include <iostream>
#include <string>
#include <queue>
#include "elemento.h"
#include "sintatico.h"

using namespace std;

void printQueue(queue<Node> q)
{
    while (!q.empty())
    {
        cout << q.front().getWord() << " ::: " << q.front().getToken() << endl;
        q.pop();
    }
    cout<<endl;
}

bool ProcedimentoASD(queue<Node> tabela)
{   
    cout << "\n\n Fila \n" << endl;
    printQueue(tabela);
    cout << "\n\n" << endl;

    cout << "Entrando Proc ASD" << endl;
    
    Node iter = tabela.front();
    tabela.pop();

    if (iter.getToken().compare("simb_program") == 0)
    {
        iter = tabela.front();
        tabela.pop();

        if (iter.getToken().compare("id") == 0)
        {
            iter = tabela.front();
            tabela.pop();

            if (iter.getToken().compare("simb_pv") == 0)
            {
                ProcedimentoCorpo(tabela);
            }
            //else ErroSintatico();
        }
        //else ErroSintatico();
    }
    //else ErroSintatico();

    iter = tabela.front();
    if (iter.getToken().compare("simb_pont") == 0)
    {
        tabela.pop();
        if (tabela.empty())
        {
            return true;
            cout << "Saindo Proc ASD" << endl;
        } 
        //else ErroSintatico();
    }
    //else ErroSintatico();

    printQueue(tabela);
}

bool ProcedimentoCorpo(queue<Node>& tabela)
{
    cout << "Entrando Proc Corpo" << endl;
    Node iter = tabela.front();

    if (iter.getToken().compare("simb_const") == 0)
    {
        ProcedimentoDConst(tabela);
    }
    else if (iter.getToken().compare("simb_var") == 0)
    {
        ProcedimentoDVar(tabela);
    }
    else if (iter.getToken().compare("simb_procedure") == 0)
    {
        ProcedimentoDProc(tabela);
    }

    iter = tabela.front();
    if (iter.getToken().compare("simb_begin") == 0)
    {
        tabela.pop();
        ProcedimentoComandos(tabela);
    }
    //else ErroSintatico();

    iter = tabela.front();
    if (iter.getToken().compare("simb_end") == 0) 
    {
        tabela.pop();
        cout << "Saindo Proc Corpo" << endl;
        return true;  
    }
    //else ErroSintatico();
}

bool ProcedimentoDConst(queue<Node>& tabela)
{
    Node iter = tabela.front();  
    while (iter.getToken().compare("simb_const") == 0)
    {
        cout << "Entrando Proc Declaracao de Constante" << endl;
        tabela.pop();
    
        iter = tabela.front();
        if (iter.getToken().compare("id") == 0)
        {
            tabela.pop();

            iter = tabela.front();
            if (iter.getToken().compare("simb_igual") == 0)
            {
                tabela.pop();

                iter = tabela.front();
                if (iter.getToken().compare("num_int") == 0 || iter.getToken().compare("num_real") == 0) // Conversar
                {
                    tabela.pop();
                    //cout << "Comi " << iter.getWord() << " ::: " << iter.getToken() << endl;

                    iter = tabela.front();
                    if (iter.getToken().compare("simb_pv") == 0)
                    {
                        tabela.pop();
                        cout << "Saindo Proc Declaracao de Constante" << endl;

                        iter = tabela.front();
                        if (iter.getToken().compare("simb_var") == 0)
                            ProcedimentoDVar(tabela);

                        else if (iter.getToken().compare("simb_procedure") == 0)
                            ProcedimentoDProc(tabela);

                        else if (iter.getToken().compare("simb_begin") == 0)
                            return true;
                        
                        // else ErroSintatico()
                    }
                }
            }
            //else ErroSintatico();
        }
        //else ErroSintatico();
    } 
}

bool ProcedimentoDVar(queue<Node>& tabela)
{
    Node iter = tabela.front();
    while (iter.getToken().compare("simb_var") == 0)
    {
        cout << "Entrando Proc Declaracao de Variavel" << endl;
        tabela.pop();

        ProcedimentoVariaveis(tabela);
        iter = tabela.front();

        if (iter.getToken().compare("simb_dp") == 0)
        {
            tabela.pop();

            iter = tabela.front();
            if (iter.getToken().compare("simb_integer") == 0 || iter.getToken().compare("simb_real") == 0) // Pensei numa situacao que pode dar ruim
            {
                tabela.pop();

                iter = tabela.front();
                if (iter.getToken().compare("simb_pv") == 0)
                {
                    tabela.pop();
                    cout << "Saindo Declaracao de Variavel" << endl;

                    iter = tabela.front();
                    if (iter.getToken().compare("simb_const") == 0) //Nao tenho ctz disso
                        ProcedimentoDConst(tabela);
                    else if (iter.getToken().compare("simb_procedure") == 0)
                        ProcedimentoDProc(tabela);
                    else if (iter.getToken().compare("simb_begin") == 0)
                            return true;
                }
            }
        }
        //else ErroSintatico();
    }
}

bool ProcedimentoDProc(queue<Node>& tabela){
    cout << "Entrando Proc Declaracao de Procedimento" << endl;
    Node iter = tabela.front();

    // Nao precisa necessariamente entrar nesse while
    while (iter.getToken().compare("simb_procedure") == 0){
        tabela.pop();
        iter = tabela.front();

        if (iter.getToken().compare("id") == 0){
            tabela.pop();
            ProcedimentoParametros(tabela);
            iter = tabela.front();

            if (iter.getToken().compare("simb_pv") == 0){
                tabela.pop();
                iter = tabela.front();
                cout << "Saindo Proc Declaracao de Procedimento" << endl;
                if(iter.getToken().compare("simb_var") == 0){
                    ProcedimentoCorpoProc(tabela);
                 }
                 //else erro
            }
            //else erro
        }
        //else erro
    }       
}

bool ProcedimentoVariaveis(queue<Node>& tabela)
{
    Node iter = tabela.front();
    while (iter.getToken().compare("id") == 0)
    {
        cout << "Entrando Proc Variavel" << endl;

        tabela.pop();
        iter = tabela.front();
        
        if (iter.getToken().compare("simb_virg") == 0)
        {
            tabela.pop();
            iter = tabela.front();
        }
        else if (iter.getToken().compare("simb_dp") == 0)
        {
            cout << "Saindo Proc Variavel" << endl;
            return true;
        }
        
        cout << "Saindo Proc Variavel" << endl;
        //Else erro, mas vai ter que ser mais complexo que isso
        
    }
}

bool ProcedimentoTipoVar(queue<Node>& tabela){
    cout << "Entrando Tipo Var" << endl;
    Node iter = tabela.front(); 

    if (iter.getToken().compare("simb_real") == 0){
        tabela.pop();
        iter = tabela.front();
        return true;
    }
    
    else if (iter.getToken().compare("simb_integer") == 0){
        tabela.pop();
        iter = tabela.front();
        return true;
    }
}  

bool ProcedimentoParametros(queue<Node>& tabela){ 
    cout << "Entrando Proc Param" << endl;
    Node iter = tabela.front(); 

    // Nao precisa entrar nesse if
    if (iter.getToken().compare("simb_apar") == 0){
        tabela.pop();
        iter = tabela.front();

        if(iter.getToken().compare("id") != 0);
            //erro aq

        while (iter.getToken().compare("id") == 0){
            ProcedimentoVariaveis(tabela);
            iter = tabela.front();

            if (iter.getToken().compare("simb_dp") == 0){
                tabela.pop();
                ProcedimentoTipoVar(tabela);
                iter = tabela.front();
            } 
            //else erro

        } 

        if(iter.getToken().compare("simb_fpar") == 0){
            tabela.pop();
            iter = tabela.front();
        }
    }        
    cout << "Proc Parametros" << endl;
}

bool ProcedimentoCorpoProc(queue<Node>& tabela){
    Node iter = tabela.front();

    cout << "Entrando Proc Corpo Procedimento" << endl;
    if(iter.getToken().compare("simb_var") == 0){
        ProcedimentoDVar(tabela);

        if(iter.getToken().compare("simb_begin") == 0){
            tabela.pop();
            ProcedimentoComandos(tabela);
            iter = tabela.front();

            if(iter.getToken().compare("simb_end") == 0){
                tabela.pop();
                iter = tabela.front();
                if(iter.getToken().compare("simb_pv") == 0){
                    tabela.pop();
                    iter = tabela.front();
                    cout << "Saindo Proc Corpo Procedimento" << endl;
                }
                //else erro
            }
            //else erro
        }
        //else erro
    } 
    //else erro

}

bool ProcedimentoComandos(queue<Node>& tabela)
{
    Node iter = tabela.front();
    while (iter.getToken().compare("simb_read") == 0 || iter.getToken().compare("simb_write") == 0 || iter.getToken().compare("simb_while") == 0 
    || iter.getToken().compare("simb_for") == 0 || iter.getToken().compare("simb_if") == 0 || iter.getToken().compare("simb_begin") == 0 || iter.getToken().compare("id") == 0){
        tabela.pop();

        bool aux_prov = 0;
        if (iter.getToken().compare("simb_read") == 0){
            ProcedimentoRead(tabela);
            iter = tabela.front();
            if (iter.getToken().compare("simb_pv") == 0){
                tabela.pop();
                iter = tabela.front();
            } //else esqueceu o ponto e virgulo amigo
        }

        if (iter.getToken().compare("simb_write") == 0){
            ProcedimentoWrite(tabela);
            iter = tabela.front();
            if (iter.getToken().compare("simb_pv") == 0){
                tabela.pop();
                iter = tabela.front();
            } //else esqueceu o ponto e virgulo amigo
        }  

        if (iter.getToken().compare("simb_while") == 0){
            aux_prov = ProcedimentoWhile(tabela);
            iter = tabela.front();
            if (iter.getToken().compare("simb_pv") == 0){
                tabela.pop();
                iter = tabela.front();
            } //else esqueceu o ponto e virgulo amigo
        }

        if (iter.getToken().compare("simb_for") == 0){
            aux_prov = ProcedimentoFor(tabela);
            iter = tabela.front();
            if (iter.getToken().compare("simb_pv") == 0){
                tabela.pop();
                iter = tabela.front();
            } //else esqueceu o ponto e virgulo amigo
        }

        else if (iter.getToken().compare("simb_if") == 0){
            aux_prov = ProcedimentoIf(tabela);
            iter = tabela.front();
            if (iter.getToken().compare("simb_pv") == 0){
                tabela.pop();
                iter = tabela.front();
            } //else esqueceu o ponto e virgulo amigo
        }

        if (aux_prov == false){
            break;
        }
            
        if (iter.getToken().compare("simb_begin") == 0){
            iter = tabela.front();
            if (iter.getToken().compare("simb_pv") == 0){
                tabela.pop();
                iter = tabela.front();
            } //else esqueceu o ponto e virgulo amigo
        }

        if (iter.getToken().compare("id") == 0){
            iter = tabela.front();  
            if (iter.getToken().compare("simb_pv") == 0){
                tabela.pop();
                iter = tabela.front();
            } //else esqueceu o ponto e virgulo amigo       
        }
    }
}

bool ProcedimentoRead(queue<Node>& tabela){
    cout << "Entrando Proc Read" << endl;
    Node iter = tabela.front(); 

    if (iter.getToken().compare("simb_apar") == 0)
    {
        tabela.pop();
        ProcedimentoVariaveis(tabela);
        iter = tabela.front();

        if (iter.getToken().compare("simb_fpar") == 0)
        {
            tabela.pop();
            iter = tabela.front();
        }
    }
}

bool ProcedimentoWrite(queue<Node>& tabela){
    cout << "Entrando Proc Write" << endl;
    Node iter = tabela.front(); 

    if (iter.getToken().compare("simb_apar") == 0)
    {
        tabela.pop();
        ProcedimentoVariaveis(tabela);
        iter = tabela.front();

        if (iter.getToken().compare("simb_fpar") == 0)
        {
            tabela.pop();
            iter = tabela.front();
        }
    }
}

bool ProcedimentoWhile(queue<Node>& tabela){
    cout << "Entrando Proc While" << endl;
    Node iter = tabela.front(); 

    if (iter.getToken().compare("simb_apar") == 0){
        tabela.pop();
        ProcedimentoCondicao(tabela);
        iter = tabela.front();

        if (iter.getToken().compare("simb_fpar") == 0){
            tabela.pop();
            iter = tabela.front();

            if (iter.getToken().compare("simb_do") == 0){
                tabela.pop();
                iter = tabela.front();
            } 
            //else erro
        }
        //else erro 
    }
    return false;
}
bool ProcedimentoFor(queue<Node>& tabela){
    cout << "Entrando Proc For" << endl;
    return false;
}

bool ProcedimentoIf(queue<Node>& tabela){
    cout << "Entrando Proc If" << endl;
    return false;
}

bool ProcedimentoCondicao(queue<Node>& tabela){
    cout << "Entrando Proc Cond" << endl;
    Node iter = tabela.front(); 

    if (iter.getToken().compare("simb_soma") == 0 || iter.getToken().compare("simb_sub")){
        ProcedimentoFator(tabela);
        // Vou parar aqui pq daqui pra frente vai ficar bem foda viu
    }

}
bool ProcedimentoFator(queue<Node>& tabela){
    cout << "Entrando Proc Fator" << endl;
    return false;
}


