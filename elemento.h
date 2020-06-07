#pragma once

#ifndef ELEMENTO_H

	#define ELEMENTO_H

    #include <string>
    #include <stdio.h>

    using namespace std;

    class Node
    {
        private:
            string word;
            string token;
            Node* next;

        public:
            Node(string w, string t) // construtor
            {
                this->word = w;
                this->token = t;
                this->next = NULL;
            }

            string getWord() {return word;}

            string getToken() {return token;}

            Node* getNext() {return next;}

            void setNext(Node* p) {next = p;}
    };

#endif