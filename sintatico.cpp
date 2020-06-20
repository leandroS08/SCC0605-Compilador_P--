#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include "elemento.h"
#include "sintatico.h"

using namespace std;

int count_erros_sintaticos = 0;

bool ProcedimentoASD(queue<Node> &tabela, int &count_erros)
{
    bool flag_erro = false;
    bool flag_panico = false;
    int id_panico = 0;

    Node iter = tabela.front();

    if (iter.getToken().compare("simb_program") == 0)
    {
        tabela.pop();
        iter = tabela.front();

        if (iter.getToken().compare("id") == 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (iter.getToken().compare("simb_pv") != 0)
            {
                ErroSintatico(1, iter.getLine(), flag_erro);
                flag_panico = newToEmPanico(tabela, 1);
            }
        }
        else
        {
            ErroSintatico(1, iter.getLine(), flag_erro);
            flag_panico = newToEmPanico(tabela, 1);
        }
    }
    else
    {
        ErroSintatico(1, iter.getLine(), flag_erro);
        flag_panico = newToEmPanico(tabela, 1);
    }

    iter = tabela.front();
    if (iter.getToken().compare("simb_pv") == 0)
        tabela.pop();
    else
        ErroSintatico(0, iter.getLine(), flag_erro);

    iter = tabela.front();
    if (flag_panico == false)
    {
        ProcedimentoCorpo(tabela);

        iter = tabela.front();
        if (iter.getToken().compare("simb_pont") == 0)
        {
            tabela.pop();
            count_erros = count_erros + count_erros_sintaticos;
            return true;
        }
        else if (tabela.empty())
        {
            ErroSintatico(2, -1, flag_erro);
            count_erros = count_erros + count_erros_sintaticos;
            return true;
        }
    }
    else
    {
        ErroSintatico(-1, iter.getLine(), flag_erro); // não conseguiu recuperar a compilação do código
        count_erros = count_erros + count_erros_sintaticos;
        return false;
    }
}

bool ProcedimentoCorpo(queue<Node> &tabela)
{
    bool flag_erro = false;
    bool flag_panico = false;
    int id_panico = 1;

    Node iter = tabela.front();

    bool wait_begin = false;
    while (!wait_begin)
    {
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
        
        if (tabela.empty())
        {
            ErroSintatico(4, iter.getLine(), flag_erro);
            wait_begin = true;
            return true;
        }

        iter = tabela.front();
        if (iter.getToken().compare("simb_begin") == 0)
        {
            tabela.pop();
            wait_begin = true;
        }
        else
        {
            flag_panico = newToEmPanico(tabela, 4);
            iter = tabela.front();
            if (iter.getToken().compare("simb_begin") == 0)
            {
                ErroSintatico(7, iter.getLine(), flag_erro);
                wait_begin = true;
            }
            else if (iter.getToken().compare("simb_end") == 0)
            {
                ErroSintatico(4, iter.getLine(), flag_erro);
                wait_begin = true;
            }
            else
            {
                ErroSintatico(7, iter.getLine(), flag_erro);
                wait_begin = true;
            }
        }

        if (tabela.empty())
        {
            ErroSintatico(4, iter.getLine(), flag_erro);
            wait_begin = true;
            return true;
        }

        iter = tabela.front();
    }

    if (flag_panico == false)
    {
        ProcedimentoComandos(tabela);

        iter = tabela.front();
        if (iter.getToken().compare("simb_end") == 0)
        {
            tabela.pop();
            return true;
        }
        else
        {
            ErroSintatico(5, iter.getLine(), flag_erro);
            return true;
        }
    }
    else
    {
        ErroSintatico(-1, iter.getLine(), flag_erro); // não conseguiu recuperar a compilação do código
        return false;
    }
}

bool ProcedimentoDConst(queue<Node> &tabela)
{
    bool flag_erro = false;
    bool flag_panico = false;
    int id_panico = 2;

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
                    tabela.pop();
                    //cout << "Comi " << iter.getWord() << " ::: " << iter.getToken() << endl;

                    iter = tabela.front();
                    if (iter.getToken().compare("simb_pv") == 0)
                        tabela.pop();
                    else
                        ErroSintatico(0, iter.getLine(), flag_erro);
                }
                else
                {
                    ErroSintatico(6, iter.getLine(), flag_erro);
                    flag_panico = toEmPanico(tabela, id_panico);
                }
            }
            else
            {
                ErroSintatico(6, iter.getLine(), flag_erro);
                flag_panico = toEmPanico(tabela, id_panico);
            }
        }
        else
        {
            ErroSintatico(6, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
        }

        iter = tabela.front();

        if (iter.getToken().compare("simb_const") == 0)
            continue;

        if (iter.getToken().compare("simb_var") == 0)
            ProcedimentoDVar(tabela);

        else if (iter.getToken().compare("simb_procedure") == 0)
            ProcedimentoDProc(tabela);

        else if (iter.getToken().compare("simb_begin") == 0)
            continue;

        else if(!tabela.empty())
        {
            ErroSintatico(7, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
            return false;
        }
    }

    return true;
}

bool ProcedimentoDVar(queue<Node> &tabela)
{
    bool flag_erro = false;
    bool flag_panico = false;
    int id_panico = 3;

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
                    tabela.pop();
                else
                    ErroSintatico(0, iter.getLine(), flag_erro);
            }
            else
            {
                ErroSintatico(8, iter.getLine(), flag_erro);
                flag_panico = toEmPanico(tabela, id_panico);
            }
        }
        else
        {
            ErroSintatico(8, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
        }

        iter = tabela.front();
        if (iter.getToken().compare("simb_const") == 0)
            ProcedimentoDConst(tabela);

        if (iter.getToken().compare("simb_var") == 0)
            continue;

        else if (iter.getToken().compare("simb_procedure") == 0)
            ProcedimentoDProc(tabela);

        else if (iter.getToken().compare("simb_begin") == 0)
            continue;

        else if(!tabela.empty())
        {
            ErroSintatico(7, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
            return false;
        }
    }

    return true;
}

bool ProcedimentoDProc(queue<Node> &tabela)
{
    Node iter = tabela.front();
    bool flag_panico = false;
    bool flag_erro = false;
    int id_panico = 5;

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
                if (iter.getToken().compare("simb_var") == 0 || iter.getToken().compare("simb_begin") == 0)
                {
                    ProcedimentoCorpoProc(tabela);
                }
                else
                {
                    ErroSintatico(9, iter.getLine(), flag_erro);
                    flag_panico = toEmPanico(tabela, id_panico);
                    ProcedimentoCorpoProc(tabela);
                }
            }
            else
                ErroSintatico(0, iter.getLine(), flag_erro);
        }
        else if(!tabela.empty())
        {
            ErroSintatico(9, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
            ProcedimentoCorpoProc(tabela);
        }
        iter = tabela.front();
    }
}

bool ProcedimentoVariaveis(queue<Node> &tabela)
{
    Node iter = tabela.front();
    bool flag_panico = false;
    bool flag_erro = false;
    int id_panico = 6;

    if (iter.getToken().compare("id") == 0)
    {
        tabela.pop();
        iter = tabela.front();
    }
    else
    {
        ErroSintatico(10, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, id_panico);
        iter = tabela.front();
    }

    while (iter.getToken().compare("simb_virg") == 0)
    {
        tabela.pop();
        iter = tabela.front();

        if (iter.getToken().compare("id") == 0)
        {
            tabela.pop();
            iter = tabela.front();
        }
        else if(!tabela.empty())
        {
            ErroSintatico(10, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
            iter = tabela.front();
        }
    }
    return true;
}

bool ProcedimentoTipoVar(queue<Node> &tabela)
{
    Node iter = tabela.front();
    bool flag_panico = false;
    bool flag_erro = false;
    int id_panico = 6;

    if (iter.getToken().compare("simb_real") == 0)
    {
        tabela.pop();
        iter = tabela.front();
        return false;
    }

    else if (iter.getToken().compare("simb_integer") == 0)
    {
        tabela.pop();
        iter = tabela.front();
        return false;
    }
    else
    {
        ErroSintatico(9, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, id_panico);
        return true;
    }
}

bool ProcedimentoParametros(queue<Node> &tabela)
{
    Node iter = tabela.front();
    bool flag_panico = false;
    bool flag_erro = false;
    int id_panico = 6;

    if (iter.getToken().compare("simb_apar") == 0)
    {
        tabela.pop();
        iter = tabela.front();

        if (iter.getToken().compare("id") != 0)
        {
            ErroSintatico(9, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
            iter = tabela.front();
            flag_erro = true;
        }

        while (iter.getToken().compare("id") == 0)
        {
            ProcedimentoVariaveis(tabela);
            iter = tabela.front();
        }

        if (iter.getToken().compare("simb_dp") == 0)
        {
            tabela.pop();
            flag_erro = ProcedimentoTipoVar(tabela);
            iter = tabela.front();

            if (iter.getToken().compare("simb_fpar") == 0)
            {
                tabela.pop();
                iter = tabela.front();
            }
            else
            {
                ErroSintatico(9, iter.getLine(), flag_erro);
                flag_panico = toEmPanico(tabela, id_panico);
                iter = tabela.front();
            }
        }
        else if (iter.getToken().compare("simb_dp") != 0 and flag_erro == false)
        {
            ErroSintatico(9, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
            iter = tabela.front();
        }
    }

    if (iter.getToken().compare("simb_fpar") == 0)
    {
        tabela.pop();
        iter = tabela.front();
        if (iter.getToken().compare("simb_pv") == 0)
        {
            return true;
        }
    }

    else if (iter.getToken().compare("simb_pv") == 0)
    {
        return true;
    }

    else
    {
        ErroSintatico(0, iter.getLine(), flag_erro);
        return false;
    }
}

bool ProcedimentoCorpoProc(queue<Node> &tabela)
{
    Node iter = tabela.front();
    bool flag_panico = false;
    bool flag_erro = false;
    int id_panico = 8;

    if (iter.getToken().compare("simb_var") == 0)
    {
        ProcedimentoDVar(tabela);
        iter = tabela.front();

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
                else
                {
                    ErroSintatico(0, iter.getLine(), flag_erro);
                    return true;
                }
            }
            else
            {
                ErroSintatico(12, iter.getLine(), flag_erro);
                flag_panico = toEmPanico(tabela, id_panico);
                return true;
            }
        }
        else
        {
            ErroSintatico(12, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
            return true;
        }
    }

    else if (iter.getToken().compare("simb_begin") == 0)
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
            else
            {
                ErroSintatico(0, iter.getLine(), flag_erro);
                return true;
            }
        }
        else
        {
            ErroSintatico(12, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
            return true;
        }
    }
    else
    {
        ErroSintatico(12, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, id_panico);
        return true;
    }
}

bool ProcedimentoComandos(queue<Node> &tabela)
{
    bool waiting_end = false;
    bool flag_panico = false;
    bool flag_erro = false;
    int id_panico = 8;

    Node iter = tabela.front();
    while (iter.getToken().compare("simb_end") != 0 && tabela.empty() == false)
    {
        if (iter.getToken().compare("simb_read") == 0)
            ProcedimentoRead(tabela);

        else if (iter.getToken().compare("simb_write") == 0)
            ProcedimentoWrite(tabela);

        else if (iter.getToken().compare("simb_while") == 0)
            waiting_end = ProcedimentoWhile(tabela);

        else if (iter.getToken().compare("simb_for") == 0)
            waiting_end = ProcedimentoFor(tabela);

        else if (iter.getToken().compare("simb_if") == 0)
            waiting_end = ProcedimentoIf(tabela);

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
                    iter = tabela.front();
                    if (iter.getToken().compare("id") == 0)
                    {
                        tabela.pop();
                        iter = tabela.front();
                        if (iter.getToken().compare("simb_pv") == 0){
                            tabela.pop();
                            iter = tabela.front();
                        }
                        else if (iter.getToken().compare("simb_fpar") == 0)
                        {
                            tabela.pop();
                            iter = tabela.front();
                            aux_mais = false;
                        }
                        else
                        {
                            ErroSintatico(19, iter.getLine(), flag_erro);
                            flag_panico = newToEmPanico(tabela, 5);
                            iter = tabela.front();
                            if (iter.getToken().compare("simb_fpar") == 0)
                            {
                                tabela.pop();
                                aux_mais = false;
                            }
                            else
                            {
                                ErroSintatico(-1, iter.getLine(), flag_erro);
                                aux_mais = false;
                            }
                        }
                    }
                    else
                    {
                        ErroSintatico(19, iter.getLine(), flag_erro);
                        flag_panico = newToEmPanico(tabela, 5);
                        aux_mais = false;
                    }
                }
                tabela.pop();
            }
            else
            {
                ErroSintatico(11, iter.getLine(), flag_erro);
                flag_panico = toEmPanico(tabela, 4);
            }
        }

        else if (iter.getToken().compare("simb_begin") == 0)
        {
            waiting_end = true;
            tabela.pop();
            ProcedimentoComandos(tabela);

            iter = tabela.front();
            if (iter.getToken().compare("simb_end") == 0)
            {
                tabela.pop();
                return true;
            }
            else
            {
                ErroSintatico(5, iter.getLine(), flag_erro);
                flag_panico = toEmPanico(tabela, id_panico);
            }
        }
        else
        {
            ErroSintatico(11, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
        }

        iter = tabela.front();
        if (iter.getToken().compare("simb_pv") == 0)
            tabela.pop();
        if (iter.getToken().compare("simb_end") == 0)
        {
            waiting_end = false;
            return true;
        }
        //else
        //{
        //    ErroSintatico(0, iter.getLine(), flag_erro);
        //}

        iter = tabela.front();
    }
}

bool ProcedimentoRead(queue<Node> &tabela)
{
    tabela.pop();
    Node iter = tabela.front();
    bool flag_erro = false;
    bool flag_panico = false;
    int id_panico = 8;

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
        else
        {
            ErroSintatico(11, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
            return false;
        }
    }
    else
    {
        ErroSintatico(11, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, id_panico);
        return false;
    }
}

bool ProcedimentoWrite(queue<Node> &tabela)
{
    tabela.pop();
    Node iter = tabela.front();
    bool flag_erro = false;
    bool flag_panico = false;
    int id_panico = 8;

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
        else
        {
            ErroSintatico(11, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
            return false;
        }
    }
    else
    {
        ErroSintatico(11, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, id_panico);
        return false;
    }
}

bool ProcedimentoWhile(queue<Node> &tabela)
{
    tabela.pop();
    Node iter = tabela.front();
    bool flag_erro = false;
    bool flag_panico = false;
    int id_panico = 9;

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
            }
            else
            {
                ErroSintatico(13, iter.getLine(), flag_erro);
                flag_panico = toEmPanico(tabela, id_panico);
            }
        }
        else
        {
            ErroSintatico(13, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
        }
    }
    else
    {
        ErroSintatico(13, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, id_panico);
    }

    ProcedimentoComandos(tabela);
}

bool ProcedimentoIf(queue<Node> &tabela)
{
    tabela.pop();
    bool flag_erro = false;
    bool flag_panico = false;
    int id_panico = 10;
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
        }
        else
        {
            ErroSintatico(14, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
            ProcedimentoComandos(tabela);
        }
    }

    else
    {
        ProcedimentoCondicao(tabela);
        iter = tabela.front();
    }
    

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
    else
    {
        ErroSintatico(14, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, id_panico);
        ProcedimentoComandos(tabela);
    }
}

bool ProcedimentoFor(queue<Node> &tabela)
{
    tabela.pop();
    bool flag_erro = false;
    bool flag_panico = false;

    ProcedimentoAtribuicao(tabela);

    Node iter = tabela.front();
    if (iter.getToken().compare("simb_to") == 0)
    {
        tabela.pop();
        iter = tabela.front();

        if (iter.getToken().compare("num_int") == 0 || iter.getToken().compare("num_real") == 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (iter.getToken().compare("simb_do") == 0)
                tabela.pop();
            else
            {
                ErroSintatico(13, iter.getLine(), flag_erro);
                flag_panico = toEmPanico(tabela, 10);
            }
        }
        else
        {
            ErroSintatico(13, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, 10);
        }
    }
    else
    {
        ErroSintatico(13, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, 10);
    }

    if (flag_panico == false)
    {
        ProcedimentoComandos(tabela);
        return true;
    }
    else
    {
        ErroSintatico(-1, iter.getLine(), flag_erro); // não conseguiu recuperar a compilação do código
        return false;
    }
}

bool ProcedimentoAtribuicao(queue<Node> &tabela)
{
    Node iter = tabela.front();
    bool flag_erro = false;
    bool flag_panico = false;

    if (iter.getToken().compare("id") == 0)
    {
        tabela.pop();
        iter = tabela.front();
        if (iter.getToken().compare("simb_atri") == 0)
        {
            tabela.pop();
            iter = tabela.front();
            if (iter.getToken().compare("num_int") == 0 || iter.getToken().compare("num_real") == 0)
            {
                tabela.pop();
                return true;
            }
            else
            {
                ErroSintatico(16, iter.getLine(), flag_erro);
                flag_panico = toEmPanico(tabela, 11);
            }
        }
        else
        {
            ErroSintatico(16, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, 11);
        }
    }
    else
    {
        ErroSintatico(16, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, 11);
    }
}

bool ProcedimentoCondicao(queue<Node> &tabela)
{
    bool flag_erro = false;
    bool flag_panico = false;

    ProcedimentoExpressao(tabela);

    Node iter = tabela.front();
    if (iter.getToken().compare("simb_igual") == 0 || iter.getToken().compare("simb_dif") == 0|| iter.getToken().compare("simb_maig") == 0 || iter.getToken().compare("simb_meig") == 0 || iter.getToken().compare("simb_maior") == 0 || iter.getToken().compare("simb_menor") == 0)
        tabela.pop();
    else
    {
        ErroSintatico(15, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, 9);
    }

    ProcedimentoExpressao(tabela);
}

bool ProcedimentoExpressao(queue<Node> &tabela)
{
    Node iter = tabela.front();
    bool flag_erro = false;
    bool flag_panico = false;

    if (iter.getToken().compare("simb_soma") == 0 || iter.getToken().compare("simb_sub") == 0)
        tabela.pop();

    ProcedimentoFator(tabela);

    ProcedimentoMaisFatores(tabela);

    ProcedimentoOutrosTermos(tabela);
}

bool ProcedimentoTermo(queue<Node> &tabela)
{
    Node iter = tabela.front();
    bool flag_erro = false;
    bool flag_panico = false;

    if (iter.getToken().compare("simb_soma") == 0 || iter.getToken().compare("simb_sub") == 0)
        tabela.pop();

    ProcedimentoFator(tabela);

    ProcedimentoMaisFatores(tabela);
}

bool ProcedimentoOutrosTermos(queue<Node> &tabela)
{
    Node iter = tabela.front();
    bool flag_erro = false;
    bool flag_panico = false;

    while (iter.getToken().compare("simb_soma") == 0 || iter.getToken().compare("simb_sub") == 0)
    {
        tabela.pop();

        ProcedimentoTermo(tabela);
        iter = tabela.front();
    }
}

bool ProcedimentoFator(queue<Node> &tabela)
{
    Node iter = tabela.front();
    bool flag_erro = false;
    bool flag_panico = false;

    if (iter.getToken().compare("id") == 0)
    {
        tabela.pop();
        return true;
    }
    else if (iter.getToken().compare("num_int") == 0 || iter.getToken().compare("num_real") == 0)
    {
        tabela.pop();
        return true;
    }
    else if (iter.getToken().compare("simb_apar") == 0)
    {
        tabela.pop();

        ProcedimentoExpressao(tabela);

        iter = tabela.front();
        if (iter.getToken().compare("simb_fpar") == 0)
            tabela.pop();
        else
        {
            ErroSintatico(17, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, 9);
        }
    }
    else
    {
        ErroSintatico(18, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, 9);
    }
}

bool ProcedimentoMaisFatores(queue<Node> &tabela)
{
    Node iter = tabela.front();
    bool flag_erro = false;
    bool flag_panico = false;

    while (iter.getToken().compare("simb_mult") == 0 || iter.getToken().compare("simb_div") == 0)
    {
        tabela.pop();

        ProcedimentoFator(tabela);
        iter = tabela.front();
    }
}

bool ErroSintatico(int erro, int line, bool &flag)
{
    string msg_erro;
    flag = true;

    switch (erro)
    {
    case -1:
        msg_erro = "modo pânico não conseguiu recuperar a compilação";
        break;

    case 0:
        msg_erro = "faltando -> ; (aluno usp não faz isso...)";
        line = line - 1;
        break;

    case 1:
        msg_erro = "estrutura de inicialização do programa com erros"; // quando falta e quando ta errado
        break;

    case 2:
        msg_erro = "faltando . na finalização do programa";
        break;

    case 4:
        msg_erro = "faltando -> begin";
        break;

    case 5:
        msg_erro = "faltando -> end";
        break;

    case 6:
        msg_erro = "estrutura de declaração de constante inválida";
        break;

    case 7:
        msg_erro = "operação inválida em área de declaração";
        break;

    case 8:
        msg_erro = "estrutura de declaração de variável inválida";
        break;

    case 9:
        msg_erro = "estrutura declaração de procedimento inválida";
        break;

    case 10:
        msg_erro = "variáveis inseridas de forma inválida nesse comando";
        break;

    case 11:
        msg_erro = "comando inválido";
        break;

    case 12:
        msg_erro = "corpo de procedimento inválido";
        break;

    case 13:
        msg_erro = "estrutura de repetição inválida";
        break;

    case 14:
        msg_erro = "estrutura condicional inválida";
        break;

    case 15:
        msg_erro = "condicao inválida";
        break;

    case 16:
        msg_erro = "estrutura de atribuição inválida";
        break;

    case 17:
        msg_erro = "faltando -> )";
        break;

    case 18:
        msg_erro = "fator inválido";
        break;

    case 19:
        msg_erro = "lista de argumentos inválida";
        break;
    }

    ofstream file_out;                    // Arquivo de saída
    file_out.open("saida.txt", ios::app); // Cria o arquivo de saída

    file_out << "Erro sintatico na linha " << line << ": " << msg_erro << endl;

    file_out.close();

    cout << "Erro sintatico na linha " << line << ": " << msg_erro << endl;
    count_erros_sintaticos++;
}

bool toEmPanico(queue<Node> &tabela, int id_panico)
{
    Node iter = tabela.front();

    if (id_panico == 0 || id_panico == 2 || id_panico == 3 || id_panico == 4)
    {
        while (iter.getToken().compare("simb_pv") != 0 && iter.getToken().compare("simb_const") != 0 && iter.getToken().compare("simb_var") != 0 && iter.getToken().compare("simb_procedure") != 0 && iter.getToken().compare("simb_begin") != 0 && iter.getToken().compare("simb_end") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        if (iter.getToken().compare("simb_pv") == 0)
        {
            tabela.pop();
            return false;
        }
        return false;
    }

    else if (id_panico == 5)
    {
        iter = tabela.front();
        while (iter.getToken().compare("simb_var") != 0 && iter.getToken().compare("simb_begin") != 0 && iter.getToken().compare("simb_pv") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }

        if (iter.getToken().compare("simb_pv") == 0)
        {
            tabela.pop();
            return false;
        }
        return false;
    }

    else if (id_panico == 6)
    {
        iter = tabela.front();
        while (iter.getToken().compare("simb_fpar") != 0 && iter.getToken().compare("simb_pv") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
    }

    else if (id_panico == 7)
    {
        iter = tabela.front();
        while (iter.getToken().compare("simb_fpar") != 0 && iter.getToken().compare("simb_pv") != 0 && iter.getToken().compare("simb_dp") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
    }

    else if (id_panico == 8)
    {
        iter = tabela.front();
        while (iter.getToken().compare("simb_pv") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
    }

    else if (id_panico == 9)
    {
        iter = tabela.front();
        while (iter.getToken().compare("simb_apar") != 0 && iter.getToken().compare("id") != 0 && iter.getToken().compare("num_real") != 0 && iter.getToken().compare("num_int") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
    }

    else if (id_panico == 10)
    {
        while (iter.getToken().compare("simb_read") != 0 && iter.getToken().compare("simb_write") != 0 && iter.getToken().compare("simb_while") != 0 && iter.getToken().compare("simb_for") != 0 && iter.getToken().compare("simb_if") != 0 && iter.getToken().compare("simb_begin") != 0 && iter.getToken().compare("id") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
    }

    else if (id_panico == 11)
    {
        while (iter.getToken().compare("simb_to") != 0 && iter.getToken().compare("num_int") != 0 && iter.getToken().compare("num_real") != 0 && iter.getToken().compare("num_do") != 0 && iter.getToken().compare("simb_read") != 0 && iter.getToken().compare("simb_write") != 0 && iter.getToken().compare("simb_while") != 0 && iter.getToken().compare("simb_for") != 0 && iter.getToken().compare("simb_if") != 0 && iter.getToken().compare("simb_begin") != 0 && iter.getToken().compare("id") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
    }

    else if (id_panico == 12)
    {
        iter = tabela.front();
        while (iter.getToken().compare("simb_fpar") != 0 && iter.getToken().compare("simb_mult") != 0 && iter.getToken().compare("simb_div") != 0 && iter.getToken().compare("id") != 0 && iter.getToken().compare("simb_apar") != 0 && iter.getToken().compare("id") != 0 && iter.getToken().compare("num_real") != 0 && iter.getToken().compare("num_int") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
    }
}

bool newToEmPanico(queue<Node> &tabela, int id_panico)
{
    Node iter = tabela.front();

    // Se a função pânico conseguir achar o ponto de restauração, ela retorna falso
    // Senão, se ela consumir tudo e não achar o ponto, ela retorna true
    // O resultado é sempre armazenado em uma flag_panico na função de chamada, que indica se houve erro na função pânico
    // A função não consome o termo de restauração!

    switch (id_panico)
    {
    case 0: // ponto e virgula
        while (iter.getToken().compare("simb_pv") != 0 && iter.getToken().compare("simb_end") != 0 && iter.getToken().compare("simb_pont") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
        break;

    case 2: // operações
        while (iter.getToken().compare("simb_soma") != 0 && iter.getToken().compare("simb_sub") != 0 && iter.getToken().compare("simb_mult") != 0 && iter.getToken().compare("simb_div") != 0 && iter.getToken().compare("simb_maior") != 0 && iter.getToken().compare("simb_menor") != 0 && iter.getToken().compare("simb_meig") != 0 && iter.getToken().compare("simb_maig") != 0 && iter.getToken().compare("simb_dif") != 0 && iter.getToken().compare("simb_igual") != 0 && iter.getToken().compare("simb_end") != 0 && iter.getToken().compare("simb_pont") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
        break;

    case 1: // palavras reservadas de declaração, pv ou begin
        while (iter.getToken().compare("simb_pv") != 0 && iter.getToken().compare("simb_const") != 0 && iter.getToken().compare("simb_procedure") != 0 && iter.getToken().compare("simb_var") != 0 && iter.getToken().compare("simb_begin") != 0 && iter.getToken().compare("simb_end") != 0 && iter.getToken().compare("simb_pont") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
        break;

    case 3: // identificador ou pv
        while (iter.getToken().compare("id") != 0 && iter.getToken().compare("simb_pv") != 0 && iter.getToken().compare("simb_end") != 0 && iter.getToken().compare("simb_pont") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
        break;

    case 4: // comando ou pv
        while (iter.getToken().compare("simb_pv") != 0 && iter.getToken().compare("simb_read") != 0 && iter.getToken().compare("simb_write") != 0 && iter.getToken().compare("simb_if") != 0 && iter.getToken().compare("simb_for") != 0 && iter.getToken().compare("simb_while") != 0 && iter.getToken().compare("simb_begin") != 0 && iter.getToken().compare("simb_end") != 0 && iter.getToken().compare("simb_pont") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
        break;

    case 5: // fecha parenteses
        while (iter.getToken().compare("simb_fpar") != 0 && iter.getToken().compare("simb_end") != 0 && iter.getToken().compare("simb_pont") != 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (tabela.empty())
                return true;
        }
        return false;
        break;
    }
}