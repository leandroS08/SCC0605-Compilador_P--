#pragma once

#ifndef SINTATICO_H

	#define SINTATICO_H

    #include <string>
    #include <stdio.h>
    #include <queue>
    #include "elemento.h"

    void printQueue(queue<Node>);

    bool ProcedimentoASD(queue<Node>);

    bool ProcedimentoCorpo(queue<Node>&);

    bool ProcedimentoDConst(queue<Node>&);

    bool ProcedimentoDVar(queue<Node>&);

    bool ProcedimentoVariaveis(queue<Node>&);

    bool ProcedimentoDProc(queue<Node>&);

    bool ProcedimentoCorpoProc(queue<Node>&);

    bool ProcedimentoParametros(queue<Node>&);

    bool ProcedimentoComandos(queue<Node>&);


    
#endif