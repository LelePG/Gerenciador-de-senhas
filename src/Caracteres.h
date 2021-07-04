#include <iostream>
#include <string>		
#include <vector>
using namespace std;

//enuns consistem em constantes numéricas nomeadas, o que não é a melhor ideia aqui.

class Caracteres{
    public:
    const vector<char> minusculas = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    const vector<char> maiusculas ={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
    'Q','R','S','T','U','V','X','Y','Z'};
    const vector<char> numeros = {'0','1','2','3','4','5','6','7','8','9'};
    const vector<char> especiais = {'!','?','@','#','%','$','&','-','+','=','_','*'};

};

