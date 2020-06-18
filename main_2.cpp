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

ofstream file_out; // Arquivo de saída

int count_line = 1;
int count_tokens = 0;
string name_token = "";

int count_erros = 0;
bool flag_erro = false;
string msg_erro = "";

queue<Node> tabela_simbolos;

void token(string);

int main(int argc, char *argv[])
{
    string line, word_aux, word;
    string::iterator it;
    char c;
    bool flag_2s = false; // flag para lidar com tokens que são símbolos de 2 caracteres (eg. <>, >=)
    bool flag_comment = false;

    ifstream file_in(argv[1]); // Arquivo de entrada

    file_out.open("saida.txt"); // Cria o arquivo de saída

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
                            token(word);
                        word = "";
                        word.push_back(c);
                        if (it != (word_aux.end() - 1))
                        {
                            if (word != "")
                                token(word);
                            word = "";
                        }
                    }
                    // Lida com os simbolos que podem começar tokens de 2 caracteres
                    else if ((flag_comment == false) && (flag_2s == false) && ((c == ':') || (c == '<') || (c == '>')))
                    {
                        if (word != "")
                            token(word);
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
                                token(word);
                            word = "";
                        }
                        else
                        {
                            if (word != "")
                                token(word);
                            word = "";
                            word.push_back(c);
                        }
                        flag_2s = false;
                    }
                    // Lida com o final do programa
                    else if ((flag_comment == false) && (c == '.') && (word == "end"))
                    {
                        if (word != "")
                            token(word);
                        word = "";
                        word.push_back(c);
                    }
                    // Lida com comentários
                    else if ((flag_comment == false) && (c == '{'))
                        flag_comment = true;
                    else if ((flag_comment == true) && (c == '}'))
                        flag_comment = false;
                    //Comentário Inserido incorretamente
                   else if ((c != '}') && (flag_comment == true)) {
                        ErroLexico(5, &msg_erro, &flag_erro, count_line, count_erros);
                    }
                    // Caracteres normais
                    else if (flag_comment == false)
                        word.push_back(c);
                }

                if (word != "")
                    token(word);
                word = "";
            }

            count_line++;
        }
        if (flag_comment == true)
            cout << "erro(comentário não finalizado)" << endl;
        file_in.close();

        bool result = ProcedimentoASD(tabela_simbolos, count_erros);

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
    }
    else
    {
        cout << "erro na abertura do arquivo" << endl;
        file_out << "erro na abertura do arquivo" << endl;
    }

    file_out.close();

    return 0;
}

void token(string word)
{
    /* Descobrindo se o numero asc da primeira letra da palavra é um numero
        e caso for, chama o automato que checa se é inteiro ou real*/
    if ((int(*word.begin()) > 48) && (int(*word.begin()) < 57))
    {
        Numbers num(word);
        bool flag_num;

        //cout << "num" << endl;

        flag_num = AutomatoNumInt(num, &name_token);
        if (flag_num == false)
            flag_num = AutomatoNumReal(num, &name_token);
        if (flag_num == false)
        {
            ErroLexico(1, &msg_erro, &flag_erro, count_line, count_erros);
            name_token = "num_real";
        }
    }

    /* Descobrindo se o numero asc da primeira letra da palavra é uma pontuacao
        caso for, são chamados os automatos para reconhecer cada pontuação */
    else if ((int(*word.begin()) >= 58 && int(*word.begin()) <= 62) ||
             (int(*word.begin()) >= 40 && int(*word.begin()) <= 47))
    {
        Punctuation pont(word);
        bool flag_pont;

        //cout << "pont" << endl;

        flag_pont = AutomatoPunct(pont, &name_token);
    }

    else if ((int(*word.begin()) >= 65 && int(*word.begin()) <= 95) ||
             (int(*word.begin()) >= 97 && int(*word.begin()) <= 122) ||
             (int(*word.begin()) >= 48 && int(*word.begin()) <= 57))
    {
        Reserved ident(word);
        bool flag_ident;

        //cout << "id" << endl;

        flag_ident = AutomatoIdent(ident, &name_token);

        if (flag_ident == false)
        {
            ErroLexico(2, &msg_erro, &flag_erro, count_line, count_erros);
            name_token = "id";
        }
    }

    else
        ErroLexico(3, &msg_erro, &flag_erro, count_line, count_erros);

    // Printar no terminal
    /*if (flag_erro)
        cout << "[Token " << count_tokens << "] " << word << ", " << msg_erro << endl;
    else
        cout << "[Token " << count_tokens << "] " << word << ", " << name_token << endl;*/

    tabela_simbolos.push(Node(word, name_token, count_line));

    // Salvar no arquivo
    if (flag_erro)
        file_out << word << ", " << name_token << " (" << msg_erro << ")" << endl;
    else
        file_out << word << ", " << name_token << endl;

    count_tokens++;
    name_token = "";
    flag_erro = false;
}
