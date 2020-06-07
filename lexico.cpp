#include <iostream>
#include <string> 
#include "lexico.h"

using namespace std;

bool AutomatoNumInt(Numbers input, std::string* result){
    string::iterator it_num;
    
    for(it_num = input.n.begin(); it_num != input.n.end(); ++it_num){
        if( input.asc_begin > *it_num || input.asc_end < *it_num )
            return false;
    }
    *result = "num_int";
    return true;
}

bool AutomatoNumReal(Numbers input, std::string* result){
    string::iterator it_num;
    input.frag_dot = true;

    for(it_num = input.n.begin(); it_num != input.n.end(); ++it_num){
        if( input.asc_begin > *it_num || input.asc_end < *it_num ){
            if( (input.asc_dot == *it_num) && (input.frag_dot == true) )
                input.frag_dot = false;
            else 
                return false;
        }      
    }
    *result = "num_real";
    return true;
}

bool AutomatoPunct(Punctuation punct, std::string* result){
    if(punct.p.length() == 1)
    {
        switch (*punct.p.begin())
        {
            case (43):
                *result = "simb_soma";
                return true;
            break;

            case (45):
                *result = "simb_sub";
                return true;
            break;

            case (42):
                *result = "simb_mult";
                return true;
            break;

            case (47):
                *result = "simb_div";
                return true;
            break;

            case (62):
                *result = "simb_maior";
                return true;
            break;

            case (60):
                *result = "simb_menor";
                return true;
            break;

            case (40):
                *result = "simb_apar";
                return true;
            break;

            case (41):
                *result = "simb_fpar";
                return true;
            break;

            case (46):
                *result = "simb_pont";
                return true;
            break;

            case (58):
                *result = "simb_dp";
                return true;
            break;

            case (59):
                *result = "simb_pv";
                return true;
            break;

            case (44):
                *result = "simb_virg";
                return true;
            break;

            case (61):
                *result = "simb_igual";
                return true;
            break;
        }
    }
    else if(punct.p.length() == 2)
    {
        if(punct.p.compare(":=") == 0){
            *result = "simb_atri";
            return true;
        }
        else if(punct.p.compare(">=") == 0){
            *result = "simb_maig";
            return true;
        }
        else if(punct.p.compare("<=") == 0){
            *result = "simb_meig";
            return true;
        }
        else if(punct.p.compare("<>") == 0){
            *result = "simb_dif";
            return true;
        }
    }
    else return false;  
}

bool AutomatoIdent(Reserved ident, std::string* result){
    string::iterator it_ident;
    for(it_ident = ident.w.begin(); it_ident != ident.w.end(); ++it_ident){
        if(  ident.asc_begin2 > *it_ident || 
            (ident.asc_end2 < *it_ident && ident.asc_begin0 > *it_ident) || 
             ident.asc_end1 < *it_ident   || 
            (ident.asc_end0 < *it_ident && ident.asc_begin1 > *it_ident) )
            return false;    
    }
    if(ident.w.compare("program") == 0){
        *result = "simb_program";
        return true;
    }
    else if(ident.w.compare("end") == 0){
        *result = "simb_end";
        return true;
    }
    else if(ident.w.compare("begin") == 0){
        *result = "simb_begin";
        return true;
    }
    else if(ident.w.compare("var") == 0){
        *result = "simb_var";
        return true;
    }
    else if(ident.w.compare("else") == 0){
        *result = "simb_else";
        return true;
    }
    else if(ident.w.compare("read") == 0){
        *result = "simb_read";
        return true;
    }
    else if(ident.w.compare("write") == 0){
        *result = "simb_write";
        return true;
    }
    else if(ident.w.compare("if") == 0){
        *result = "simb_if";
        return true;
    }
    else if(ident.w.compare("then") == 0){
        *result = "simb_then";
        return true;
    }
    else if(ident.w.compare("while") == 0){
        *result = "simb_while";
        return true;
    }
    else if(ident.w.compare("do") == 0){
        *result = "simb_do";
        return true;
    }
    else if(ident.w.compare("for") == 0){
        *result = "simb_for";
        return true;
    }
    else if(ident.w.compare("to") == 0){
        *result = "simb_to";
        return true;
    }
    else if(ident.w.compare("procedure") == 0){
        *result = "simb_procedure";
        return true;
    }
    else if(ident.w.compare("integer") == 0){
        *result = "simb_integer";
        return true;
    }
    else if(ident.w.compare("real") == 0){
        *result = "simb_real";
        return true;
    }
    else{
        *result = "id";
        return true;
    }    
}

bool ErroLexico(int erro, std::string* msg_erro, bool* flag){
    *flag = true;
    switch (erro)
    {
        case 1:
            *msg_erro = "erro lexico: número irregular";
        break;

        case 2:
            *msg_erro = "erro lexico: identificador irregular";
        break;

        case 3:
            *msg_erro = "erro lexico: caractere não permitido";
        break;

        *msg_erro = "erro lexico inesperado";
    }
}
        