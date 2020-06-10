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
    cout << endl;
}

bool ProcedimentoASD(queue<Node> tabela)
{
    cout << "\n********* ANALISE SINTATICA *********" << endl;

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
            cout << "Sucesso! Todas as cadeias processadas" << endl;
            return true;
            
        }
        else 
        {
            cout << "Codigo com erro, cadeias não processadas" << endl;
            printQueue(tabela);
            //ErroSintatico();
        }
    }
    else
    {
        cout << "Codigo com erro, cadeias não processadas" << endl;
        printQueue(tabela);
        /* code */
    }
    
    //else ErroSintatico();
}

bool ProcedimentoCorpo(queue<Node> &tabela)
{
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
        return true;
    }
    //else ErroSintatico();
}

bool ProcedimentoDConst(queue<Node> &tabela)
{
    Node iter = tabela.front();
    while (iter.getToken().compare("simb_const") == 0)
    {
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
                    ProcedimentoTipoVar(tabela);
                    //cout << "Comi " << iter.getWord() << " ::: " << iter.getToken() << endl;

                    iter = tabela.front();
                    if (iter.getToken().compare("simb_pv") == 0)
                    {
                        tabela.pop();

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

bool ProcedimentoDVar(queue<Node> &tabela)
{
    Node iter = tabela.front();
    while (iter.getToken().compare("simb_var") == 0)
    {
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

bool ProcedimentoDProc(queue<Node> &tabela)
{
    Node iter = tabela.front();

    // Nao precisa necessariamente entrar nesse while
    while (iter.getToken().compare("simb_procedure") == 0)
    {
        tabela.pop();
        iter = tabela.front();

        if (iter.getToken().compare("id") == 0)
        {
            tabela.pop();
            ProcedimentoParametros(tabela);
            iter = tabela.front();

            if (iter.getToken().compare("simb_pv") == 0)
            {
                tabela.pop();
                iter = tabela.front();
                if (iter.getToken().compare("simb_var") == 0)
                {
                    ProcedimentoCorpoProc(tabela);
                }
                //else erro
            }
            //else erro
        }
        //else erro
    }
}

bool ProcedimentoVariaveis(queue<Node> &tabela)
{
    Node iter = tabela.front();
    while (iter.getToken().compare("id") == 0)
    {
        tabela.pop();
        iter = tabela.front();

        if (iter.getToken().compare("simb_virg") == 0)
        {
            tabela.pop();
            iter = tabela.front();
        }
        else if (iter.getToken().compare("simb_dp") == 0)
        {
            return true;
        }

        //Else erro, mas vai ter que ser mais complexo que isso
    }
}

bool ProcedimentoTipoVar(queue<Node> &tabela)
{
    Node iter = tabela.front();

    if (iter.getToken().compare("num_real") == 0)
    {
        tabela.pop();
        iter = tabela.front();
        return true;
    }

    else if (iter.getToken().compare("num_int") == 0)
    {
        tabela.pop();
        iter = tabela.front();
        return true;
    }
    else
    {
        return false;
    }
    
}

bool ProcedimentoParametros(queue<Node> &tabela)
{
    Node iter = tabela.front();

    // Nao precisa entrar nesse if
    if (iter.getToken().compare("simb_apar") == 0)
    {
        tabela.pop();
        iter = tabela.front();

        if (iter.getToken().compare("id") != 0)
            ;
        //erro aq

        while (iter.getToken().compare("id") == 0)
        {
            ProcedimentoVariaveis(tabela);
            iter = tabela.front();

            if (iter.getToken().compare("simb_dp") == 0)
            {
                tabela.pop();
                ProcedimentoTipoVar(tabela);
                iter = tabela.front();
            }
            //else erro
        }

        if (iter.getToken().compare("simb_fpar") == 0)
        {
            tabela.pop();
            iter = tabela.front();
        }
    }
    //cout << "Proc Parametros" << endl;
}

bool ProcedimentoCorpoProc(queue<Node> &tabela)
{
    Node iter = tabela.front();

    if (iter.getToken().compare("simb_var") == 0)
    {
        ProcedimentoDVar(tabela);

        if (iter.getToken().compare("simb_begin") == 0)
        {
            tabela.pop();
            ProcedimentoComandos(tabela);
            iter = tabela.front();

            if (iter.getToken().compare("simb_end") == 0)
            {
                tabela.pop();
                iter = tabela.front();
                if (iter.getToken().compare("simb_pv") == 0)
                {
                    tabela.pop();
                    iter = tabela.front();
                }
                //else erro
            }
            //else erro
        }
        //else erro
    }
    //else erro
}

bool ProcedimentoComandos(queue<Node> &tabela)
{
    bool waiting_end = false;

    Node iter = tabela.front();
    while (waiting_end == true || iter.getToken().compare("simb_read") == 0 || iter.getToken().compare("simb_write") == 0 || iter.getToken().compare("simb_while") == 0 || iter.getToken().compare("simb_for") == 0 || iter.getToken().compare("simb_if") == 0 || iter.getToken().compare("simb_begin") == 0 || iter.getToken().compare("id") == 0)
    {
        iter = tabela.front();

        if (iter.getToken().compare("simb_read") == 0)
        {
            ProcedimentoRead(tabela);
        }

        else if (iter.getToken().compare("simb_write") == 0)
        {
            ProcedimentoWrite(tabela);
        }

        else if (iter.getToken().compare("simb_while") == 0)
        {
            waiting_end = ProcedimentoWhile(tabela);
        }

        else if (iter.getToken().compare("simb_for") == 0)
        {
            waiting_end = ProcedimentoFor(tabela);
        }

        else if (iter.getToken().compare("simb_if") == 0)
        {
            waiting_end = ProcedimentoIf(tabela);
        }

        else if (iter.getToken().compare("id") == 0)
        {
            tabela.pop();

            iter = tabela.front();
            if (iter.getToken().compare("simb_atri") == 0)
            {
                tabela.pop();

                ProcedimentoExpressao(tabela);
            }
            else if (iter.getToken().compare("simb_apar") == 0)
            {
                tabela.pop();
                bool aux_mais = true;

                iter = tabela.front();

                while (aux_mais == true)
                {
                    if (iter.getToken().compare("id") == 0)
                    {
                        tabela.pop();

                        iter = tabela.front();
                        if (iter.getToken().compare("simb_pv") == 0)
                            tabela.pop();
                        else if (iter.getToken().compare("simb_fpar") == 0)
                            aux_mais = false;
                        // else erro
                    }
                }

                tabela.pop();
            }
        }

        else if (iter.getToken().compare("simb_begin") == 0)
        {
            tabela.pop();
            ProcedimentoComandos(tabela);
        }

        else if (iter.getToken().compare("simb_end") == 0)
        {
            tabela.pop();
            waiting_end = false;
        }

        else
        {
            waiting_end = false;
            return false;
        }
        

        iter = tabela.front();
        if (iter.getToken().compare("simb_pv") == 0)
        {
            tabela.pop();
            iter = tabela.front();
        } //else esqueceu o ponto e virgulo amigo
    }
}

bool ProcedimentoRead(queue<Node> &tabela)
{
    tabela.pop();
    Node iter = tabela.front();

    if (iter.getToken().compare("simb_apar") == 0)
    {
        tabela.pop();
        ProcedimentoVariaveis(tabela);
        iter = tabela.front();

        if (iter.getToken().compare("simb_fpar") == 0)
        {
            tabela.pop();
            return true;
        }
    }
}

bool ProcedimentoWrite(queue<Node> &tabela)
{
    tabela.pop();
    Node iter = tabela.front();

    if (iter.getToken().compare("simb_apar") == 0)
    {
        tabela.pop();
        ProcedimentoVariaveis(tabela);
        iter = tabela.front();

        if (iter.getToken().compare("simb_fpar") == 0)
        {
            tabela.pop();
            return true;
        }
    }
}

bool ProcedimentoWhile(queue<Node> &tabela)
{
    tabela.pop();
    Node iter = tabela.front();

    if (iter.getToken().compare("simb_apar") == 0)
    {
        tabela.pop();
        ProcedimentoCondicao(tabela);
        iter = tabela.front();

        if (iter.getToken().compare("simb_fpar") == 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (iter.getToken().compare("simb_do") == 0)
            {
                tabela.pop();

                iter = tabela.front();
                if (iter.getToken().compare("simb_begin") == 0)
                {
                    ProcedimentoComandos(tabela);
                    return true;
                }
                else
                    return false;
            }
            //else erro
        }
        //else erro
    }
    // else erro
}

bool ProcedimentoFor(queue<Node> &tabela)
{
    tabela.pop();
    return false;
}

bool ProcedimentoIf(queue<Node> &tabela)
{
    tabela.pop();

    ProcedimentoCondicao(tabela);

    Node iter = tabela.front();
    if (iter.getToken().compare("simb_then") == 0)
    {
        tabela.pop();

        ProcedimentoComandos(tabela);

        iter = tabela.front();
        if (iter.getToken().compare("simb_else") == 0)
        {
            tabela.pop();
            ProcedimentoComandos(tabela);
        }
    }
    // else erro
}

bool ProcedimentoCondicao(queue<Node> &tabela)
{
    Node iter = tabela.front();

    ProcedimentoExpressao(tabela);

    iter = tabela.front();
    if (iter.getToken().compare("simb_igual") == 0 || iter.getToken().compare("simb_dif") || iter.getToken().compare("simb_maig") == 0 || iter.getToken().compare("simb_meig") == 0 || iter.getToken().compare("simb_maior") == 0 || iter.getToken().compare("simb_menor") == 0)
        tabela.pop();
    // else erro

    ProcedimentoExpressao(tabela);
}

bool ProcedimentoExpressao(queue<Node> &tabela)
{
    Node iter = tabela.front();
    if (iter.getToken().compare("simb_soma") == 0 || iter.getToken().compare("simb_sub"))
        tabela.pop();
    
    ProcedimentoFator(tabela);

    ProcedimentoMaisFatores(tabela);

    ProcedimentoOutrosTermos(tabela);
}

bool ProcedimentoTermo(queue<Node> &tabela)
{
    Node iter = tabela.front();
    if (iter.getToken().compare("simb_soma") == 0 || iter.getToken().compare("simb_sub"))
        tabela.pop();
    
    ProcedimentoFator(tabela);

    ProcedimentoMaisFatores(tabela);
}

//////////////////////* Falta tirar recursao
bool ProcedimentoOutrosTermos(queue<Node> &tabela)
{
    Node iter = tabela.front();
    if (iter.getToken().compare("simb_soma") == 0 || iter.getToken().compare("simb_sub") == 0)
    {
        tabela.pop();

        ProcedimentoTermo(tabela);

        ProcedimentoOutrosTermos(tabela);
    } 
}

bool ProcedimentoFator(queue<Node> &tabela)
{
    Node iter = tabela.front();
    if (iter.getToken().compare("id") == 0)
        tabela.pop();
    else if (iter.getToken().compare("num_int") == 0 || iter.getToken().compare("num_real") == 0)
        ProcedimentoTipoVar(tabela);
    else if (iter.getToken().compare("simb_apar") == 0)
    {
        tabela.pop();
        ProcedimentoExpressao(tabela);

        if (iter.getToken().compare("simb_fpar") == 0)
            tabela.pop();
        //else erro
    } 
}

//////////////////////* Falta tirar recursao
bool ProcedimentoMaisFatores(queue<Node>& tabela)
{
    Node iter = tabela.front();
    if (iter.getToken().compare("simb_mult") == 0 || iter.getToken().compare("simb_div") == 0 )
    {
        tabela.pop();

        ProcedimentoFator(tabela);

        ProcedimentoMaisFatores(tabela);
    } 
}
