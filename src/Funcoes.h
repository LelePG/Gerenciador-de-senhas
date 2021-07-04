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
#include <mysql/mysql.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <mysql_driver.h>
#include "Caracteres.h"

string SENHA_TESTE = "inserir senha";
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
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        string usuarioSql, senhaSql;

        cout << "Insira o nome do usuario Mysql:" << endl;
        cin >> usuarioSql;
        cout << "Insira a senha do usuario Mysql:" << endl;
        cin >> senhaSql;
        
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", SENHA_TESTE); //Modificar aqui
        stmt = con->createStatement();
        try
        {
            stmt->execute("use GerenciadorSenhas");
                   stmt -> execute("insert into senhas (aplicacao, senha, data) values (\"" + this->aplicacao + "\",\"" + this->senha + "\",\"" + this->data + "\")");

        }
        catch (sql::SQLException &e)
        {
            cout << "Houve um problema com seu banco de dados: ";
            cout << e.what() << endl;
            return;
        }

        delete con;
        delete stmt;
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
        cout << "2 - Criar o banco de dados" << endl;
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
        //limpaTela();
    }

    void instrucoes()
    {
        cout << "Se esta é a sua primeira vez utilizando o gerenciador de senhas," << endl;
        cout << "selecione a opção de criar o banco de dados para iniciar a utilização." << endl;
        cout << "Após a criação do banco de dados, você pode criar uma senha para uma determinada " << endl;
        cout << "aplicação, ou consultar as senhas já existentes. Recomenda-se que para a " <<endl;
        cout << "utilização dessa aplicação, o usuário crie um usuário e senhas próprios para " <<endl;
        cout << "a utilização da aplicação, e não utilize o usuário mysql padrão." << endl;
        cout << "Esses usuário e senha serão utilizados para acessar as informações na aplicação." << endl;
        cout << "Digite c e aperte enter para continuar." << endl;
        string saida;
        cin >> saida;
        limpaTela();
    }

    void configurarBanco()
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        string usuarioSql, senhaSql;

        cout << "Insira o nome do usuario Mysql:" << endl;
        cin >> usuarioSql;
        cout << "Insira a senha do usuario Mysql:" << endl;
        cin >> senhaSql;
        
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", SENHA_TESTE); //Modificar aqui
        stmt = con->createStatement();
        try
        {
            stmt->execute("Create Database GerenciadorSenhas");
        }
        catch (sql::SQLException &e)
        {
            cout << "Já existe um banco de dados configurado no seu computador. Deseja sobreescrevê-lo? s/n" << endl;
            char sobreescrever;
            cin >> sobreescrever;
            if (tolower(sobreescrever) == 's')
            {
                stmt->execute("drop database GerenciadorSenhas");
                stmt->execute("Create Database GerenciadorSenhas");
            }
            else{
                cout << "Banco de dados não atualizado" << endl;
            }
        }
        con->setSchema("GerenciadorSenhas");

        try
        {
            stmt->execute("CREATE TABLE senhas (aplicacao varchar(30), senha varchar(20), data varchar(30))");
        }
        catch (sql::SQLException &e)
        {
            cout << "Houve um problema. Erro no programa: ";
            cout << e.what() <<endl;
            
        }
        limpaTela();
                cout << "Banco de dados criado com sucesso." <<endl;


        delete con;
        delete stmt;
    }
};