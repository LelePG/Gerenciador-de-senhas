#include <iostream>
#include "Funcoes.h"
using namespace std;

int main()
{
    Funcoes f;
    cout << "Boas vindas ao gerenciador de senhas!" << endl;
    int leitura;
    while(true)
    {
        Senha s;
        int leitura = f.imprimirMenu();
           switch (leitura)
        {
        case 0:
        cout << "Obrigada por usar o gerenciador de senhas!" << endl;
        return 0;
            break;
        case 1:
            f.instrucoes();
            break;
        case 2:
            f.configurarBanco();
        break;
        case 3:
            f.criarSenha();
            break;
        case 4:
            f.salvarSenhaExistente();
            break;
        case 5:
            f.pegarSenha();
            break;
        case 6:
            f.fazerBackup();
            break;
        case 7:
            f.restaurarBanco();

        default:
            cout << "Opção inválida" << endl;
        }
        

    } 
    return 0;
}