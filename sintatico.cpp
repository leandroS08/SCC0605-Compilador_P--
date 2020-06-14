#include <iostream>
#include <string>
#include <queue>
#include "elemento.h"
#include "sintatico.h"

using namespace std;

int count_erros_sintaticos = 0;

void printQueue(queue<Node> q)
{
    while (!q.empty())
    {
        cout << q.front().getLine() << " ::: " << q.front().getWord() << " ::: " << q.front().getToken() << endl;
        q.pop();
    }
    cout << endl;
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
        msg_erro = "faltando -> .";
        break;

    case 4:
        msg_erro = "faltando -> begin";
        break;

    case 5:
        msg_erro = "faltando -> end";
        break;

    case 6:
        msg_erro = "declaração de constante inválida";
        break;

    case 7:
        msg_erro = "operação inválida em área de declaração";
        break;
    }

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
        else
        {
            bool aux;
            //ErroSintatico(0, iter.getLine(), aux);
            return false;
        }
    }
}

bool ProcedimentoASD(queue<Node> tabela, int &count_erros)
{
    cout << "\n********* ANALISE SINTATICA *********" << endl;

    bool flag_erro = false;
    bool flag_panico = false;
    int id_panico = 0;

    Node iter = tabela.front();

    //********************************* mudei os lugares dos pop da vida
    if (iter.getToken().compare("simb_program") == 0)
    {
        tabela.pop();
        iter = tabela.front();

        if (iter.getToken().compare("id") == 0)
        {
            tabela.pop();
            iter = tabela.front();

            if (iter.getToken().compare("simb_pv") == 0)
                tabela.pop();
            else
            {
                ErroSintatico(1, iter.getLine(), flag_erro);
                flag_panico = toEmPanico(tabela, id_panico);
            }
        }
        else
        {
            ErroSintatico(1, iter.getLine(), flag_erro);
            flag_panico = toEmPanico(tabela, id_panico);
        }
    }
    else
    {
        ErroSintatico(1, iter.getLine(), flag_erro);
        flag_panico = toEmPanico(tabela, id_panico);
    }

    if (flag_panico == false)
    {
        ProcedimentoCorpo(tabela);

        iter = tabela.front();
        if (iter.getToken().compare("simb_pont") == 0)
            tabela.pop();
        else if (tabela.empty())
        {
            ErroSintatico(2, iter.getLine(), flag_erro);
            count_erros = count_erros + count_erros_sintaticos;
            return true;
        }
        else
        {
            ErroSintatico(3, iter.getLine(), flag_erro);
            cout << endl;
            printQueue(tabela);
            count_erros = count_erros + count_erros_sintaticos;
            return false;
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

        iter = tabela.front();
        if (iter.getToken().compare("simb_begin") == 0)
        {
            tabela.pop();
            wait_begin = true;
        }
        else
        {
            flag_panico = toEmPanico(tabela, id_panico);
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
        }

        if (tabela.empty())
        {
            ErroSintatico(4, iter.getLine(), flag_erro);
            wait_begin = true;
            return false;
        }

        iter = tabela.front();
    }

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

        else
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

        else
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

/* Explicar pro Leandro que tem que ser sim_real e simb_integer******************************/
bool ProcedimentoTipoVar(queue<Node> &tabela)
{
    Node iter = tabela.front();

    if (iter.getToken().compare("simb_real") == 0)
    {
        tabela.pop();
        iter = tabela.front();
        return true;
    }

    else if (iter.getToken().compare("simb_integer") == 0)
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

            iter = tabela.front();
            if (iter.getToken().compare("simb_end") == 0)
            {
                tabela.pop();
                return true;
            }
        }

        else
        {
            return false;
        }

        iter = tabela.front();
        if (iter.getToken().compare("simb_pv") == 0)
        {
            waiting_end = false;
            tabela.pop();
            iter = tabela.front();
        }
        // vai entrar so em procedimentos
        else if (iter.getToken().compare("simb_end") == 0)
        {
            return true;
        }

        else
        {
            cout << "esqueceu o ponto e virgula" << endl;
        }
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

bool ProcedimentoFor(queue<Node> &tabela)
{
    tabela.pop();

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
            {
                tabela.pop();
                ProcedimentoComandos(tabela);
            }
            // else erro
        }
        //else fez errado
    }
    // else erro
}

bool ProcedimentoAtribuicao(queue<Node> &tabela)
{
    Node iter = tabela.front();

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
        }
    }
    return false;
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
    if (iter.getToken().compare("id") == 0)
        tabela.pop();
    else if (iter.getToken().compare("num_int") == 0 || iter.getToken().compare("num_real") == 0)
        tabela.pop();
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
bool ProcedimentoMaisFatores(queue<Node> &tabela)
{
    Node iter = tabela.front();
    while (iter.getToken().compare("simb_mult") == 0 || iter.getToken().compare("simb_div") == 0)
    {
        tabela.pop();

        ProcedimentoFator(tabela);
        iter = tabela.front();
    }
}
