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
        ProcedimentoVariaveis(tabela);
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
    cout << "Entrando Proc Declaracao de Constante" << endl;

    Node iter = tabela.front();
    
    if (iter.getToken().compare("simb_const") == 0)
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
                if (iter.getToken().compare("num_int") == 0 || iter.getToken().compare("num_real") == 0)
                {
                    tabela.pop();
                    //cout << "Comi " << iter.getWord() << " ::: " << iter.getToken() << endl;

                    iter = tabela.front();
                    if (iter.getToken().compare("simb_pv") == 0)
                    {
                        tabela.pop();
                        cout << "Saindo Proc Declaracao de Constante" << endl;

                        iter = tabela.front();
                        if (iter.getToken().compare("simb_const") == 0)
                            ProcedimentoDConst(tabela);
                        else if (iter.getToken().compare("simb_var") == 0)
                            ProcedimentoDVar(tabela);
                        else if (iter.getToken().compare("simb_procedure") == 0)
                            ProcedimentoDProc(tabela);
                        else
                            return true;
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
    cout << "Entrando Proc Declaracao de Variavel" << endl;
    Node iter = tabela.front();

    if (iter.getToken().compare("simb_var") == 0)
    {
        tabela.pop();

        ProcedimentoVariaveis(tabela);

        iter = tabela.front();
        if (iter.getToken().compare("simb_dp") == 0)
        {
            tabela.pop();

            iter = tabela.front();
            if (iter.getToken().compare("simb_integer") == 0 || iter.getToken().compare("simb_real") == 0)
            {
                tabela.pop();

                iter = tabela.front();
                if (iter.getToken().compare("simb_pv") == 0)
                {
                    tabela.pop();
                    cout << "Saindo Declaracao de Variavel" << endl;

                    iter = tabela.front();
                    if (iter.getToken().compare("simb_const") == 0)
                        ProcedimentoDConst(tabela);
                    else if (iter.getToken().compare("simb_var") == 0)
                        ProcedimentoDVar(tabela);
                    else if (iter.getToken().compare("simb_procedure") == 0)
                        ProcedimentoDProc(tabela);
                    else
                        return true;
                }
            }
        }
        //else ErroSintatico();
    }
}

bool ProcedimentoDProc(queue<Node>& tabela)
{
    cout << "Entrando Proc Declaracao de Procedimento" << endl;
    Node iter = tabela.front();

    if (iter.getToken().compare("simb_procedure") == 0)
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
                cout << "Saindo Proc Declaracao de Procedimento" << endl;

                ProcedimentoCorpoProc(tabela);
            }
        }
    }
}

bool ProcedimentoVariaveis(queue<Node>& tabela)
{
    cout << "Entrando Proc Variavel" << endl;
    Node iter = tabela.front();

    if (iter.getToken().compare("id") == 0)
    {
        tabela.pop();

        iter = tabela.front();
        if (iter.getToken().compare("simb_virg") == 0)
        {
            tabela.pop();
            ProcedimentoVariaveis(tabela);
        }
        else
        {
            cout << "Saindo Proc Variavel" << endl;
        }
        
    }
}

bool ProcedimentoParametros(queue<Node>& tabela)
{
    cout << "Proc Parametros" << endl;
}

bool ProcedimentoCorpoProc(queue<Node>& tabela)
{
    cout << "Proc Corpo Procedimento" << endl;
}


bool ProcedimentoComandos(queue<Node>& tabela)
{
    cout << "Entrando Proc Comandos" << endl;
    Node iter = tabela.front();

    if (iter.getToken().compare("simb_read") == 0 || iter.getToken().compare("simb_write") == 0)
    {
        tabela.pop();

        iter = tabela.front();
        if (iter.getToken().compare("simb_apar") == 0)
        {
            tabela.pop();

            ProcedimentoVariaveis(tabela);

            iter = tabela.front();
            if (iter.getToken().compare("simb_fpar") == 0)
            {
                tabela.pop();

                iter = tabela.front();

                if (iter.getToken().compare("simb_pv") == 0)
                {
                    tabela.pop();

                    ProcedimentoComandos(tabela);
                }
            }
        }
    }
    else if (iter.getToken().compare("simb_while") == 0);
    else if (iter.getToken().compare("simb_for") == 0);
    else if (iter.getToken().compare("simb_if") == 0);
    else if (iter.getToken().compare("simb_begin") == 0);
    else if (iter.getToken().compare("id") == 0);
    else return false;
}

