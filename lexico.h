#pragma once

#ifndef LEXICO_H

	#define LEXICO_H

    #include <string>
    #include <stdio.h>
    #include <queue>
    #include "elemento.h"

    void token(queue<Node>&, Node, int&);

    /* Classe para reconhecer numeros */
    class Numbers 
    {
        public:
            std::string n;
            bool frag_dot = true;

            int asc_begin = 48;
            int asc_end = 57;
            int asc_dot = 46;

            Numbers(std::string input) {n = input;}
    };

    bool AutomatoNumInt(Numbers, std::string*); 
    bool AutomatoNumReal(Numbers, std::string*); 

    /* Classe para reconhecer a pontuacao(simbolos) */
    class Punctuation 
    {
        public:
            std:: string p;

            bool frag_pont = true;

            int asc_begin0 = 58;
            int asc_end0 = 62;

            int asc_begin1 = 40;
            int asc_end1 = 47;

            Punctuation (std::string input) {p = input;}    
    };

    bool AutomatoPunct(Punctuation, std::string*); 

    /* Classe para reconhecer palavras reservadas e identificadores */
    class Reserved 
    {
        public:
            std::string w;

            int asc_begin0 = 65;
            int asc_end0 = 90;

            int asc_begin1 = 97;
            int asc_end1 = 122;

            int asc_begin2 = 48;
            int asc_end2 = 57;

            Reserved(std::string input) {w = input;}
    };

    bool AutomatoIdent(Reserved, std::string*); 

    bool ErroLexico(int, std::string*, bool*, int, int&);

#endif