#include <vector>
#include <algorithm>
#include <mysql/mysql.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <mysql_driver.h>
#include <stdlib.h> 
#include "Senha.h"
#include "Caracteres.h"

class Funcoes
{
private:
    void adicionaNoBanco(Senha s)
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
        con = driver->connect("tcp://127.0.0.1:3306", usuarioSql, senhaSql);
        stmt = con->createStatement();

        limpaTela();

        try
        {
            stmt->execute("use GerenciadorSenhas");
            stmt->execute("insert into senhas (aplicacao, senha, data) values (\"" + s.getAplicacao() + "\",\"" + s.getSenha() + "\",\"" + s.getData() + "\")");
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

    vector<char> concatenaVetoresDeChar(vector<char> inserirAqui, vector<char> copiarDaqui)
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

        selecionados = letrasMaiusculas ? concatenaVetoresDeChar(selecionados, listaDeLetras.maiusculas) : selecionados;
        selecionados = letrasMinusculas ? concatenaVetoresDeChar(selecionados, listaDeLetras.minusculas) : selecionados;
        selecionados = numeros ? concatenaVetoresDeChar(selecionados, listaDeLetras.numeros) : selecionados;
        selecionados = especiais ? concatenaVetoresDeChar(selecionados, listaDeLetras.especiais) : selecionados;

        return selecionados;
    }

    string gerarSenha(int tamanho, bool letrasMinusculas = true, bool letrasMaiusculas = true, bool numeros = true, bool caracteresEspeciais = true)
    {
        vector<char> componentesSenha = selecionarSimbolos(letrasMinusculas, letrasMaiusculas, numeros, caracteresEspeciais);
        if (componentesSenha.size() == 0)
        {
            cout << "Houve um problema e sua senha nao foi criada";
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

    bool confirmarOpcao(string texto)
    {
        char confirmar;
        cout << texto << endl;
        do
        {
            cout << "Digite s para confirmar, caso contrário, digite n." << endl;
            cin >> confirmar;
        } while ((tolower(confirmar) != 's') && (tolower(confirmar) != 'n'));
        int retorno = tolower(confirmar) == 's' ? 1 : 0;
        limpaTela();
        return retorno;
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
        cout << "4 - Salvar senha existente" << endl;
        cout << "5 - Verificar senha" << endl;
        cout << "6 - Fazer backup do banco de dados" <<endl;
        cout << "7 - Restaurar o banco de dados através de um backup" << endl;

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
        if (confirmarOpcao("Deseja Salvar sua senha?"))
        {
            senhaAtual.setSenha(senhaGerada);
            senhaAtual.salvarData();
            adicionaNoBanco(senhaAtual);
        }
    }

    void instrucoes()
    {
        cout << "Se esta é a sua primeira vez utilizando o gerenciador de senhas," << endl;
        cout << "selecione a opção de criar o banco de dados para iniciar a utilização." << endl;
        cout << "Após a criação do banco de dados, você pode criar uma senha para uma determinada " << endl;
        cout << "aplicação, ou consultar as senhas já existentes. Recomenda-se que para a " << endl;
        cout << "utilização dessa aplicação, o usuário crie um usuário e senhas próprios para " << endl;
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
try
        {
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", usuarioSql, senhaSql);
        stmt = con->createStatement();
        
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
            else
            {
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
            cout << e.what() << endl;
        }
        limpaTela();
        cout << "Banco de dados criado com sucesso." << endl;

        delete con;
        delete stmt;
    }
    void salvarSenhaExistente()
    {
        Senha s;
        char aplicacao[25], senha[20];
        cout << "Qual é a aplicação desta senha?" << endl;
        cin.get();
        cin.getline(aplicacao, 25);
        s.setAplicacao(aplicacao);
        cout << "Qual é a senha que deseja salvar?" << endl;
        cin.getline(senha, 20);
        s.setSenha(senha);

        if (confirmarOpcao("A senha da aplicação " + (string)aplicacao + " é " + (string)senha + "."))
        {
            s.salvarData();
            adicionaNoBanco(s);
        }
        limpaTela();
        cout << "Senha salva com sucesso." << endl;
    }

    void pegarSenha()
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;
        string usuarioSql, senhaSql, aplicacao;

        cout << "Insira o nome do usuario Mysql:" << endl;
        cin >> usuarioSql;
        cout << "Insira a senha do usuario Mysql:" << endl;
        cin >> senhaSql;

        cout << "Para qual aplicação você que recuperar a senha?" <<endl;
        cin >> aplicacao;

        try
        {
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", usuarioSql, senhaSql); //Modificar aqui
        stmt = con->createStatement();
        stmt->execute("use GerenciadorSenhas");
        
            pstmt = con->prepareStatement("select * from senhas where aplicacao = \"" + aplicacao + "\"");
            res = pstmt->executeQuery();
            if (!res->next()){//se o result set estiver fazio
                   cout<< "A aplicação não foi encotrada no banco de dados. Tente novamente." <<endl; 
            }
            while (res->next())
            {
                cout << "A senha da aplicacao " + aplicacao + " é " + res->getString("senha") + " e foi criada em " + res->getString("data") << endl;
                cout << "Digite c para continuar." << endl;
                char caracter;
                cin >> caracter;
                limpaTela();
            }
        }
        catch (sql::SQLException &e)
        {
            cout<< "Houve um problema com a sua aplicação: ";
            cout << e.what() << endl;
        }
        delete con;
        delete stmt;
    }
    void fazerBackup()
    {
        
        string usuarioSql, senhaSql;

        cout << "Insira o nome do usuario Mysql:" << endl;
        cin >> usuarioSql;
        cout << "Insira a senha do usuario Mysql:" << endl;
        cin >> senhaSql;

        string comando = "mysqldump -u "+usuarioSql+" -p"+senhaSql+" GerenciadorSenhas > BackupSenhas.sql ";            
            system((char *)comando.c_str());
         
        
    }
    void restaurarBanco()
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        string usuarioSql, senhaSql;

        cout << "Insira o nome do usuario Mysql:" << endl;
        cin >> usuarioSql;
        cout << "Insira a senha do usuario Mysql:" << endl;
        cin >> senhaSql;
       
try
        {
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", usuarioSql, senhaSql);
        stmt = con->createStatement();
        
            stmt->execute("Create Database GerenciadorSenhas");
        }
        catch (sql::SQLException &e)
        {
            cout << "Houve um problema com a aplicação. ";
            cout<< e.what();
        }
        string comando = "mysql -u "+usuarioSql+" -p"+senhaSql+" GerenciadorSenhas < BackupSenhas.sql ";            
            system((char *)comando.c_str());
    }
};