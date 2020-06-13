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
            int line;

        public:
            Node(string w, string t, int l) // construtor
            {
                this->word = w;
                this->token = t;
                this->line = l;
            }

            string getWord() {return word;}

            string getToken() {return token;}

            int getLine() {return line;}
    };

#endif