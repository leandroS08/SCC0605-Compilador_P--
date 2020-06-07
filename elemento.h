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

        public:
            Node(string w, string t) // construtor
            {
                this->word = w;
                this->token = t;
            }

            string getWord() {return word;}

            string getToken() {return token;}
    };

#endif