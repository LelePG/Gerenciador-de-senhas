#include <iostream>
#include "Funcoes.h"
using namespace std;

int main()
{
    Funcoes f;
    cout << "Boas vindas ao gerenciador de senhas!" << endl;
    int leitura;
    


    do
    {
        Senha s;
           switch (f.imprimirMenu())
        {
        case 0:
            break;
        case 1:
            f.criarSenha();
            break;
        case 2:
            
            s.salvarData();
        break;

        default:
            cout << "Opção inválida" << endl;
        }

    } while (leitura != 0);
    return 0;
}