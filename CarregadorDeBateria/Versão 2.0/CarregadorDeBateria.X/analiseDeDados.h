int indexDoMenorValor(float dados[], int quantidadeDeValoresParaAnalisar);

int indexDoMenorValor(float dados[], int quantidadeDeValoresParaAnalisar) {
    int index = 0;
    int contador = 0;
    float menorValor = 0;
    menorValor = dados[contador];
    index = contador;
    
    for(contador = 0; contador < quantidadeDeValoresParaAnalisar; contador++) {
        if(dados[contador] < menorValor) {
            index = contador;
            menorValor = dados[contador];
        }
    }
    
    return index;
}