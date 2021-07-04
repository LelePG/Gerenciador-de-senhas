#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <algorithm>
#include <stdio.h>
#include <cctype>
#include <bits/stdc++.h>
#include "Caracteres.h"

using namespace std;
class Senha
{
private:
    string senha;
    string aplicacao;
    string data;

public:
    void salvarData()
    {
        time_t ttime = time(0);
        char *dt = ctime(&ttime);
        this->data = (string)dt;
    }
    void setSenha(string str)
    {
        this->senha = str;
    }
    void setAplicacao(string str)
    {
        string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
        str.erase(end_pos, str.end());

        for (int i = 0; i < str.length(); i++)
        {
            str[i] = tolower(str[i]);
        }

        this->aplicacao = str;
    }

    void adicionaNoBanco()
    { //TODO
        cout << "a";
    }
};

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
        limpaTela();
        return habilitar;
    }

    void limpaTela()
    {
        #ifdef WINDOWS
                system("cls");
        #else
                system("clear");
        #endif
    }

public:
    int imprimirMenu()
    {
        int leitura;
        cout << "Digite a opção que você deseja:" << endl;
        cout << "0 - Sair da aplicação" << endl;
        cout << "1 - Ver informações de uso" << endl;
        cout << "2 - Configurar o banco" << endl;
        cout << "3 - Gerar senha" << endl;


        cin >> leitura;
        limpaTela();
        return leitura;
    }

    void criarSenha()
    {
        int tamanho;
        bool habilitaMaiuscula, habilitaMinuscula, habilitaNumero, habilitaEspecial;

        Senha senhaAtual;
        char aplicacao[25];
        bool salvar;

        string senhaGerada;
        cout << "Para qual aplicação você quer criar a senha?" << endl;
        cin.get();
        cin.getline(aplicacao, 25);
        senhaAtual.setAplicacao(aplicacao);
        cout << "Qual o tamanho da sua senha?" << endl;
        cin >> tamanho;
        limpaTela();
        habilitaMaiuscula = habilitarCampo("Quer habilitar letras maiusculas?");
        habilitaMinuscula = habilitarCampo("Quer habilitar letras minusculas?");
        habilitaNumero = habilitarCampo("Quer habilitar numeros?");
        habilitaEspecial = habilitarCampo("Quer habilitar caracteres especiais?");
        senhaGerada = gerarSenha(tamanho, habilitaMaiuscula, habilitaMinuscula, habilitaNumero, habilitaEspecial);
        cout << "Sua senha é: " << senhaGerada << endl;
        cout << "Deseja salvar sua senha? 1(sim) ou 0(nao)" << endl;
        cin >> salvar;
        if (salvar)
        {
            senhaAtual.setSenha(senhaGerada);
            senhaAtual.salvarData();
            senhaAtual.adicionaNoBanco();
        }
        limpaTela();
    }

    void instrucoes(){
          cout <<"Mostrar instrucoes" << endl;  
    }

    void configurarBanco(){
cout<< "onfigurar o banco"<<endl;
    }
};