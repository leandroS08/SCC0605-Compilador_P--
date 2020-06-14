#include <iostream>
#include <fstream>
#include <string> 
#include <vector> 
#include <queue>
#include <cstring>
#include "elemento.h"
#include "lexico.h"
#include "sintatico.h"

using namespace std;

ofstream file_out; // Arquivo de saída

#define int tamVnum = 4; //Tamanho por hora, procurei e não achei.
#define int tamVletra= 10; //Tamanho por hora
int count_tokens = 0;
string name_token = "";

bool flag_erro = false;
string msg_erro = "";

queue<Node> tabela_simbolos;

void token(string);

int main(int argc, char *argv[]) 
{ 
    string word, word_aux;
    string::iterator it;
    char c;
    bool flag_2s = false; // flag para lidar com tokens que são símbolos de 2 caracteres (eg. <>, >=)
    bool flag_comment = false;

    ifstream file_in (argv[1]); // Arquivo de entrada

    file_out.open("saida.txt"); // Cria o arquivo de saída

    if (file_in.is_open()) // Testa se o arquivo foi aberto corretamente
    {
        while (! file_in.eof() ) // Verifica se nao esta no final do arquivo
        {
            while(file_in >> word_aux) // Itera sobre o arquivo, palavra por palavra
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
                for(it = word_aux.begin(); it != word_aux.end(); ++it)
                {
                    c = *it;

                    // Lida com os simbolos mais comuns
                    if ( (flag_comment == false) && (flag_2s == false) && ( (c == '=') || (c == ',') || (c == ';') || (c == '(') || (c == ')') || (c == '+') || (c == '-') || (c == '/') || (c == '*') ) )
                    {
                        if(word != "") token(word);
                        word = "";
                        word.push_back(c);
                        if( it != (word_aux.end()-1) )
                        {
                            if(word != "") token(word);
                            word = "";
                        }
                    }
                    // Lida com os simbolos que podem começar tokens de 2 caracteres
                    else if ( (flag_comment == false) && (flag_2s == false) && ( (c == ':') || (c == '<') || (c == '>')) )
                    {
                        if(word != "") token(word); 
                        word = "";
                        word.push_back(c);
                        flag_2s = true;
                    }
                    // Lida com o caso de ser identificado um possível simbolo de 2 caracteres
                    else if ( (flag_comment == false) && (flag_2s == true) )
                    {
                        if( ((word == ":" ) && (c == '='))  || ((word == "<" ) && (c == '>')) || ((word == ">" ) && (c == '=')) || ((word == "<" ) && (c == '=')) )
                        {
                            word.push_back(c);
                            if(word != "") token(word);
                            word = "";
                        }
                        else
                        {
                            if(word != "") token(word);
                            word = "";
                            word.push_back(c);
                        }
                        flag_2s = false;
                    }
                    // Lida com o final do programa
                    else if ( (flag_comment == false) && (c == '.') && (word=="end") )
                    {
                        if(word != "") token(word);
                        word = "";
                        word.push_back(c);
                    }
                    // Lida com comentários
                    else if ( (flag_comment == false) && (c == '{') )
                        flag_comment = true;
                    else if ( (flag_comment == true) && (c == '}') )
                        flag_comment = false;
                    // Caracteres normais
                    else if (flag_comment == false) word.push_back(c);
                }

                if(word != "") token(word);
                word = "";
            }
            if (flag_comment == true) cout << "erro(comentário não finalizado)" << endl;
        }
        file_in.close();

        ProcedimentoASD(tabela_simbolos);
    }
    else {
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
    if( (int (*word.begin()) > 48) &&  (int (*word.begin() ) < 57) ){
        Numbers num(word);
        bool flag_num;
        
        //cout << "num" << endl;

        flag_num = AutomatoNumInt(num, &name_token);
        //tamanho incorreto de numero int
        if( flag_num == true && word.size() > tamVnum){
            ErroLexico(6, &msg_erro, &flag_erro);
        }

        if(flag_num == false) flag_num = AutomatoNumReal(num, &name_token);
        //tamanho incorreto de numero real 
        if( flag_num == true && word.size() > tamVnum){
            ErroLexico(6, &msg_erro, &flag_erro);
        }
        
        if(flag_num == false && flag_erro == false) ErroLexico(1, &msg_erro, &flag_erro);
    }

    /* Descobrindo se o numero asc da primeira letra da palavra é uma pontuacao
        caso for, são chamados os automatos para reconhecer cada pontuação */
    else if( (int (*word.begin()) >= 58 && int (*word.begin()) <= 62) ||
             (int (*word.begin()) >= 40 && int (*word.begin()) <= 47) ){
        Punctuation pont(word);
        bool flag_pont;

        //cout << "pont" << endl;

        flag_pont = AutomatoPunct(pont, &name_token);
    }

    else if( (int (*word.begin()) >= 65 && int (*word.begin()) <= 95) ||
             (int (*word.begin()) >= 97 && int (*word.begin()) <= 122) ||
             (int (*word.begin()) >= 48 && int (*word.begin()) <= 57) ){ 
        Reserved ident(word);
        bool flag_ident;

        //cout << "id" << endl;

        flag_ident = AutomatoIdent(ident, &name_token);

        // verifica irregular
         //tamanho incorreto de variavél alfabética 

        if( word.size() > tamVnum){
            ErroLexico(6, &msg_erro, &flag_erro);
        }

        // verifica caracter irregular
        else if (flag_ident == false) ErroLexico(2, &msg_erro, &flag_erro);

    }
//AQUI COMEÇA AS MODIFICAÇÕES DA CARLA
    else ErroLexico(3, &msg_erro, &flag_erro);

   
//comentario inserido incorretamente
    //Verificar se não esqueci de colocar nada.
     if (c != '}') {
         ErroLexico(5, &msg_erro, &flag_erro);
     }
                       
       
    
    
    //tamanho incorreto de variavél numerica
    //não sei como faz.=[
   // else ErroLexico(4, &msg_erro, &flag_erro);
//AQUI TERMINA AS MODIFICAÇÕES DA  CARLA
    
    
    // Printar no terminal
    if(flag_erro) cout << "[Token " << count_tokens << "] " << word << ", " << msg_erro << endl;
    else cout << "[Token " << count_tokens << "] " << word << ", " << name_token << endl;

    tabela_simbolos.push(Node(word,name_token));

    // Salvar no arquivo
    if(flag_erro) file_out << word << ", " << msg_erro << endl;
    else file_out << word << ", " << name_token << endl;

    count_tokens++;
    name_token = "";
    flag_erro = false;
}

