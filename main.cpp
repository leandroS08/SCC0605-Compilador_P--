#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include "elemento.h"
#include "lexico.h"
#include "sintatico.h"

using namespace std;

int count_line = 1;

int count_erros = 0;

queue<Node> tabela_simbolos;

void printQueue(queue<Node>);

int main(int argc, char *argv[])
{
    string line, word_aux, word;
    string::iterator it;
    char c;
    bool flag_2s = false; // flag para lidar com tokens que são símbolos de 2 caracteres (eg. <>, >=)
    bool flag_comment = false;

    ifstream file_in(argv[1]); // Arquivo de entrada

    ofstream file_out; // Arquivo de saída
    file_out.open("saida.txt", ios::trunc); // Cria o arquivo de saída

    if (file_in.is_open()) // Testa se o arquivo foi aberto corretamente
    {
        cout << "\n********* ANALISE LEXICA *********" << endl;

        while (getline(file_in, line))
        {
            istringstream ss(line);
            while (ss >> word_aux)
            {
                //cout << "Original: " << word_aux << endl;
                /* 
                    As palavras retornadas podem conter mais de um 1 token, 
                    pois a separação é feita com base em espaços em branco e quebras de linha
                    e nem sempre os tokens do programa tem espaços em branco entre si
                    Exemplo: var x:integer; 
                            - Retornará: "var" e "x:integer;"
                    O próximo for itera sobre a palavra retornada, caractere por
                    caractere, para conferir se ela não contém mais tokens
                    */
                for (it = word_aux.begin(); it != word_aux.end(); ++it)
                {
                    c = *it;

                    // Lida com os simbolos mais comuns
                    if ((flag_comment == false) && (flag_2s == false) && ((c == '=') || (c == ',') || (c == ';') || (c == '(') || (c == ')') || (c == '+') || (c == '-') || (c == '/') || (c == '*')))
                    {
                        if (word != "")
                            token(tabela_simbolos, Node(word, count_line), count_erros);
                        word = "";
                        word.push_back(c);
                        if (it != (word_aux.end() - 1))
                        {
                            if (word != "")
                                token(tabela_simbolos, Node(word, count_line), count_erros);
                            word = "";
                        }
                    }
                    // Lida com os simbolos que podem começar tokens de 2 caracteres
                    else if ((flag_comment == false) && (flag_2s == false) && ((c == ':') || (c == '<') || (c == '>')))
                    {
                        if (word != "")
                            token(tabela_simbolos, Node(word, count_line), count_erros);
                        word = "";
                        word.push_back(c);
                        flag_2s = true;
                    }
                    // Lida com o caso de ser identificado um possível simbolo de 2 caracteres
                    else if ((flag_comment == false) && (flag_2s == true))
                    {
                        if (((word == ":") && (c == '=')) || ((word == "<") && (c == '>')) || ((word == ">") && (c == '=')) || ((word == "<") && (c == '=')))
                        {
                            word.push_back(c);
                            if (word != "")
                                token(tabela_simbolos, Node(word, count_line), count_erros);
                            word = "";
                        }
                        else
                        {
                            if (word != "")
                                token(tabela_simbolos, Node(word, count_line), count_erros);
                            word = "";
                            word.push_back(c);
                        }
                        flag_2s = false;
                    }
                    // Lida com o final do programa
                    else if ((flag_comment == false) && (c == '.') && (word == "end"))
                    {
                        if (word != "")
                            token(tabela_simbolos, Node(word, count_line), count_erros);
                        word = "";
                        word.push_back(c);
                    }
                    // Lida com comentários
                    else if ((flag_comment == false) && (c == '{'))
                        flag_comment = true;
                    else if ((flag_comment == true) && (c == '}'))
                        flag_comment = false;
                    // Caracteres normais
                    else if (flag_comment == false)
                        word.push_back(c);
                }

                if (word != "")
                    token(tabela_simbolos, Node(word, count_line), count_erros);
                word = "";
            }

            count_line++;
        }
        if (flag_comment == true)
            cout << "erro(comentário não finalizado)" << endl;
        file_in.close();
    }
    else
    {
        cout << "erro na abertura do arquivo" << endl;
        //file_out << "erro na abertura do arquivo" << endl;
    }

    printQueue(tabela_simbolos);


    cout << "\n********* ANALISE SINTATICA *********" << endl;

    bool result = ProcedimentoASD(tabela_simbolos, count_erros);
    
    printQueue(tabela_simbolos);


    cout << "\n\n********* COMPILAÇÃO FINALIZADA *********" << endl;
    if (result)
        cout << "- Todas as cadeias processadas" << endl;
    else
        cout << "- Sobraram cadeias não processadas" << endl;

    if (count_erros == 0)
    {
        cout << "- Sem erros" << endl;
        return true;
    }
    else
    {
        cout << "- Número de erros mapeados: " << count_erros << endl;
    }

    file_out.close();

    return 0;
}

void printQueue(queue<Node> q)
{
    while (!q.empty())
    {
        cout << q.front().getLine() << ") " << q.front().getWord() << " <=> " << q.front().getToken() << endl;
        q.pop();
    }
    cout << endl;
}
