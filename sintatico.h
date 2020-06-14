#pragma once

#ifndef SINTATICO_H

	#define SINTATICO_H

    #include <string>
    #include <stdio.h>
    #include <queue>
    #include "elemento.h"

    void printQueue(queue<Node>);

    bool ErroSintatico(int, int, bool*);

    bool toEmPanico(queue<Node> &, int);

    bool ProcedimentoASD(queue<Node>, int&);

    bool ProcedimentoCorpo(queue<Node>&);

    bool ProcedimentoDConst(queue<Node>&);

    bool ProcedimentoDVar(queue<Node>&);

    bool ProcedimentoVariaveis(queue<Node>&);

    bool ProcedimentoTipoVar(queue<Node>&);

    bool ProcedimentoDProc(queue<Node>&);

    bool ProcedimentoCorpoProc(queue<Node>&);

    bool ProcedimentoParametros(queue<Node>&);

    bool ProcedimentoComandos(queue<Node>&);

    bool ProcedimentoRead(queue<Node>&);

    bool ProcedimentoWrite(queue<Node>&);

    bool ProcedimentoWhile(queue<Node>&);

    bool ProcedimentoFor(queue<Node>&);
    
    bool ProcedimentoIf(queue<Node>&);

    bool ProcedimentoCondicao(queue<Node>&);

    bool ProcedimentoExpressao(queue<Node>&);

    bool ProcedimentoTermo(queue<Node>&);

    bool ProcedimentoOutrosTermos(queue<Node>&);

    bool ProcedimentoFator(queue<Node>&);

    bool ProcedimentoMaisFatores(queue<Node>&);
    
    bool ProcedimentoAtribuicao(queue<Node> &);
    
#endif