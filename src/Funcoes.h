#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Caracteres.h"

class Funcoes
{
private:
    vector<char> adicionaElementos(vector<char> inserirAqui, vector<char> copiarDaqui)
    {
        for (vector<char>::iterator it = copiarDaqui.begin(); it != copiarDaqui.end(); it++)
        {
            inserirAqui.push_back(*it);
        }
        return inserirAqui;
    }

    vector<char> selecionarSimbolos(bool letrasMinusculas, bool letrasMaiusculas, bool numeros, bool especiais)
    {
        Caracteres listaDeLetras;
        vector<char> selecionados = {};

        if (letrasMaiusculas)
        {
            selecionados = adicionaElementos(selecionados, listaDeLetras.maiusculas);
        }

        if (letrasMinusculas)
        {
            selecionados = adicionaElementos(selecionados, listaDeLetras.minusculas);
        }

        if (numeros)
        {
            selecionados = adicionaElementos(selecionados, listaDeLetras.numeros);
        }

        if (especiais)
        {
            selecionados = adicionaElementos(selecionados, listaDeLetras.especiais);
        }

        return selecionados;
    }

    string gerarSenha(int tamanho, bool letrasMinusculas = true, bool letrasMaiusculas = true, bool numeros = true, bool caracteresEspeciais = true)
    {
        vector<char> componentesSenha = selecionarSimbolos(letrasMinusculas, letrasMaiusculas, numeros, caracteresEspeciais);
        if (componentesSenha.size() == 0)
        {
            return "Houve um problema e sua senha nao foi criada";
        }
        string senha;
        srand((unsigned)time(0)); //inicia a geração de númeroa aleatórios.
        for (int i = 0; i < tamanho; i++)
        {
            int posicao = rand() % (componentesSenha.size());
            senha.push_back(componentesSenha[posicao]);
        }
        return senha;
    }

    bool habilitarCampo(string texto)
    {
        int habilitar;
        cout << texto << endl;
        do
        {
            cout << "Digite 1 para habilitar, caso contrário, digite 0." << endl;
            cin >> habilitar;
        } while ((habilitar != 0) && (habilitar != 1));
        return habilitar;
    }

public:
    void imprimirMenu()
    {
        cout << "Digite a opção que você deseja:" << endl;
        cout << "0 - Sair da aplicação" << endl;
        cout << "1 - Gerar senha" << endl;
    }

    void criarSenha()
    {
        int tamanho;
        bool habilitaMaiuscula, habilitaMinuscula, habilitaNumero, habilitaEspecial;
        cout << "Qual o tamanho da sua senha?" << endl;
        cin >> tamanho;
        habilitaMaiuscula = habilitarCampo("Quer habilitar letras maiusculas?");
        habilitaMinuscula = habilitarCampo("Quer habilitar letras minusculas?");
        habilitaNumero = habilitarCampo("Quer habilitar numeros?");
        habilitaEspecial = habilitarCampo("Quer habilitar caracteres especiais?");
        cout << "Sua senha é: " << gerarSenha(tamanho, habilitaMaiuscula, habilitaMinuscula, habilitaNumero, habilitaEspecial) << endl;
    }
};