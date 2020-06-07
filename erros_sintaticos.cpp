//Versão preliminar. 
void  dc_v(string S) {
	if(simb == var){
		obter_simbolo();
	}
	else{
		count<< "Erro: var esperado.";
		erro(primeiro(variaveis,S));//Por hora assim, depois arrumar
		variaveis("{",":","}",S); //Pensar em um modo mais elegante para isso.
	}
	
	if(simb==simb_dp){
		obter_simbolo();
	}
	else{
		count<<"Erro: simbolo : esperado.";
		erro(primeiro(tipo_var,S));
		tipo_var("{",":","}",S);
	}

	if(simb==simb_dp){
		obter_símbolo();
	}

	else{
		count<<"Erro: ':' esperado!";
		erro(primeiro(dc_v,S));
		dc_v(S);
	}
}

