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

        usuarioSql = getpass("Insira o nome do usuario Mysql:");
        senhaSql = getpass("Insira a senha do usuario Mysql:");

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

    string gerarSenha(int tamanho)
    {
        bool letrasMinusculas = habilitarCampo("Quer habilitar letras maiusculas?");
        bool letrasMaiusculas = habilitarCampo("Quer habilitar letras minusculas?");
        bool numeros = habilitarCampo("Quer habilitar numeros?");
        bool caracteresEspeciais = habilitarCampo("Quer habilitar caracteres especiais?");

        vector<char> componentesSenha = selecionarSimbolos(letrasMinusculas, letrasMaiusculas, numeros, caracteresEspeciais);
        if (componentesSenha.size() == 0)
        {
            cout << "Houve um problema e sua senha nao foi criada";
            return "Houve um problema e sua senha nao foi criada";
        }
        string senha;
        srand((unsigned)time(0)); //inicia a gera????o de n??meroa aleat??rios.
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
            cout << "Digite 1 para habilitar, caso contr??rio, digite 0." << endl;
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
            cout << "Digite s para confirmar, caso contr??rio, digite n." << endl;
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

    void continuar()
    {
        cout << "Digite c e aperte enter para continuar." << endl;
        char caracter;
        cin >> caracter;
        limpaTela();
    }

public:
    int imprimirMenu()
    {
        int leitura;
        cout << "Digite a op????o que voc?? deseja:" << endl;
        cout << "0 - Sair da aplica????o" << endl;
        cout << "1 - Ver informa????es de uso" << endl;
        cout << "2 - Criar o banco de dados" << endl;
        cout << "3 - Gerar senha de uma aplica????o" << endl;
        cout << "4 - Salvar senha externa ?? aplica????o" << endl;
        cout << "5 - Acessar uma senha existente" << endl;
        cout << "6 - Editar uma senha existente" << endl;
        cout << "7 - Excluir uma senha existente" << endl;
        cout << "8 - Fazer backup do banco de dados" << endl;
        cout << "9 - Restaurar o banco de dados atrav??s de um backup" << endl;

        cin >> leitura;
        limpaTela();
        return leitura;
    }

    void criarSenha()
    {
        int tamanho;
        Senha senhaAtual;
        string senhaGerada;

        senhaAtual.setAplicacao();
        cout << "Qual o tamanho da sua senha?" << endl;
        cin >> tamanho;
        limpaTela();
        senhaGerada = gerarSenha(tamanho);
        cout << "Sua senha ??: " << senhaGerada << endl;
        if (confirmarOpcao("Deseja Salvar sua senha?"))
        {
            senhaAtual.setSenha(senhaGerada);
            senhaAtual.salvarData();
            adicionaNoBanco(senhaAtual);
        }
    }

    void instrucoes()
    {
        cout << "Se esta ?? a sua primeira vez utilizando o gerenciador de senhas," << endl;
        cout << "selecione a op????o de criar o banco de dados para iniciar a utiliza????o," << endl;
        cout << "ou selecione a op????o de restaurar o banco de dados atrav??s de um backup." <<endl;
        cout << "Ap??s a cria????o do banco de dados, voc?? pode criar uma senha para uma determinada " << endl;
        cout << "aplica????o, ou adicionar uma senha que voc?? j?? tenha previamente, ou consultar " << endl; 
        cout << "as senhas j?? existentes. Recomenda-se que para a utiliza????o dessa aplica????o" << endl;
        cout << "o usu??rio crie um usu??rio e senhas pr??prios para e n??o utilize o usu??rio mysql padr??o." << endl;
        cout << "Esses usu??rio e senha ser??o utilizados para acessar as informa????es na aplica????o." << endl;
        cout << "ATEN????O: AO REALISAR O BACKUP DO BANCO DE DADOS, TODAS AS SENHAS SALVAS PODEM SER " << endl;
        cout << "ACESSADAS A PARTIR DESSE BACKUP." << endl;
        continuar();
    }

    void configurarBanco()
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        string usuarioSql, senhaSql;

        usuarioSql = getpass("Insira o nome do usuario Mysql:");
        senhaSql = getpass("Insira a senha do usuario Mysql:");

        try
        {
            driver = get_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", usuarioSql, senhaSql);
            stmt = con->createStatement();

            stmt->execute("Create Database GerenciadorSenhas");
        }
        catch (sql::SQLException &e)
        {
            cout << "J?? existe um banco de dados configurado no seu computador. Deseja sobreescrev??-lo? s/n" << endl;
            char sobreescrever;
            cin >> sobreescrever;
            if (tolower(sobreescrever) == 's')
            {
                stmt->execute("drop database GerenciadorSenhas");
                stmt->execute("Create Database GerenciadorSenhas");
            }
            else
            {
                cout << "Banco de dados n??o atualizado" << endl;
            }
        }
        con->setSchema("GerenciadorSenhas");

        try
        {
            stmt->execute("CREATE TABLE senhas (aplicacao varchar(30), senha varchar(20), data varchar(30))");
            cout << "Banco de dados criado com sucesso." << endl;
        }
        catch (sql::SQLException &e)
        {
            cout << "Houve um problema. Erro no programa: ";
            cout << e.what() << endl;
        }
        continuar();
        delete con;
        delete stmt;
    }
    void salvarSenhaExistente()
    {
        Senha s;
        char senha[20];
        s.setAplicacao();
        cout << "Qual ?? a senha que deseja salvar?" << endl;
        cin.getline(senha, 20);
        s.setSenha(senha);

        if (confirmarOpcao("A senha da aplica????o " + s.getAplicacao() + " ?? " + (string)senha + "."))
        {
            s.salvarData();
            adicionaNoBanco(s);
            cout << "Senha salva com sucesso." << endl;
        }
        else
        {
            cout << "A senha n??o foi salva." << endl;
        }
        continuar();
    }

    void pegarSenha()
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;
        string usuarioSql, senhaSql;
        Senha s;

        usuarioSql = getpass("Insira o nome do usuario Mysql:");
        senhaSql = getpass("Insira a senha do usuario Mysql:"); // yes, that's all you need!

        s.setAplicacao();
        try
        {
            driver = get_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", usuarioSql, senhaSql); //Modificar aqui
            stmt = con->createStatement();
            stmt->execute("use GerenciadorSenhas");

            pstmt = con->prepareStatement("select * from senhas where aplicacao = \"" + s.getAplicacao() + "\"");
            res = pstmt->executeQuery();
            if (!res->next())
            { //se o result set estiver fazio
                cout << "A aplica????o n??o foi encotrada no banco de dados. Tente novamente." << endl;
            }
            else
            {
                cout << "A senha da aplicacao " + s.getAplicacao() + " ?? " + res->getString("senha") + " e foi criada em " + res->getString("data") << endl;
            }
        }
        catch (sql::SQLException &e)
        {
            cout << "Houve um problema com a sua aplica????o: ";
            cout << e.what() << endl;
        }
        continuar();
        delete con;
        delete stmt;
    }

    void alterarSenha()
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;
        string usuarioSql, senhaSql;
        Senha s;

        usuarioSql = getpass("Insira o nome do usuario Mysql:");
        senhaSql = getpass("Insira a senha do usuario Mysql:");
        s.setAplicacao();
        try
        {
            driver = get_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", usuarioSql, senhaSql);
            stmt = con->createStatement();
            stmt->execute("use GerenciadorSenhas");

            pstmt = con->prepareStatement("select * from senhas where aplicacao = \"" + s.getAplicacao() + "\"");
            res = pstmt->executeQuery();
            if (!res->next())
            { //se o result set estiver fazio
                cout << "A aplica????o n??o possui nenhuma senha registrada. Tente novamente." << endl;
            }
            else
            {
                cout << "A senha foi encontrada." << endl;
                if (confirmarOpcao("Deseja criar uma senha com o gerador de senhas da aplica????o?"))
                {
                    int tamanho;
                    cout << "Qual o tamanho da sua senha?" << endl;
                    cin >> tamanho;
                    s.setSenha(gerarSenha(tamanho));
                    if (confirmarOpcao("Deseja salvar a senha?"))
                    {
                        s.salvarData();
                        pstmt = con->prepareStatement("update senhas set senha = \"" + s.getSenha() + "\" , data=\"" + s.getData() + "\" where aplicacao = \"" + s.getAplicacao() + "\"");
                        res = pstmt->executeQuery();
                        cout << "A senha da aplica????o " + s.getAplicacao() + " foi atualizada com sucesso." << endl;
                    }
                }
                else if (confirmarOpcao("Deseja digitar uma senha?"))
                {
                    cin.get();
                    char senha[20];
                    cout << "Qual ?? a senha que deseja salvar?" << endl;
                    cin.getline(senha, 20);
                    s.setSenha(senha);

                    if (confirmarOpcao("Deseja salvar a senha?"))
                    {
                        s.salvarData();
                        pstmt = con->prepareStatement("update senhas set senha = \"" + s.getSenha() + "\" , data=\"" + s.getData() + "\" where aplicacao = \"" + s.getAplicacao() + "\"");
                        res = pstmt->executeQuery();
                        cout << "A senha da aplica????o " + s.getAplicacao() + " foi atualizada com sucesso." << endl;
                    }
                }
                else
                {
                    cout << "Encerrando opera????o." << endl;
                }
            }
        }
        catch (sql::SQLException &e)
        {
            cout << "Houve um problema com a sua aplica????o: ";
            cout << e.what() << endl;
        }
        continuar();
        delete con;
        delete stmt;
    }

    void excluirSenha()
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;
        string usuarioSql, senhaSql;
        Senha s;

        usuarioSql = getpass("Insira o nome do usuario Mysql:");
        senhaSql = getpass("Insira a senha do usuario Mysql:");
        s.setAplicacao();
        try
        {
            driver = get_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", usuarioSql, senhaSql);
            stmt = con->createStatement();
            stmt->execute("use GerenciadorSenhas");

            pstmt = con->prepareStatement("select * from senhas where aplicacao = \"" + s.getAplicacao() + "\"");
            res = pstmt->executeQuery();
            if (!res->next())
            { //se o result set estiver fazio
                cout << "A aplica????o n??o foi possui nenhuma senha registrada. Tente novamente." << endl;
            }
            else
            {
                if (confirmarOpcao("A senha foi encontrada, quer mesmo deletar?"))
                {
                    pstmt = con->prepareStatement("delete from senhas where aplicacao = \"" + s.getAplicacao() + "\"");
                    res = pstmt->executeQuery();
                    cout << "A senha da aplica????o " + s.getAplicacao() + " foi deletada com sucesso." << endl;
                }
                else
                {
                    cout << "A senha n??o foi deletada" << endl;
                }
            }
        }
        catch (sql::SQLException &e)
        {
            cout << "Houve um problema com a sua aplica????o: ";
            cout << e.what() << endl;
        }
        continuar();
        delete con;
        delete stmt;
    }

    void fazerBackup()
    {
        string usuarioSql, senhaSql;

        usuarioSql = getpass("Insira o nome do usuario Mysql:");

        senhaSql = getpass("Insira a senha do usuario Mysql:");

        string comando = "mysqldump -u " + usuarioSql + " -p" + senhaSql + " GerenciadorSenhas > BackupSenhas.sql ";
        system((char *)comando.c_str());
        cout << "Backup realizado com sucesso." << endl;
        continuar();
    }
    
    void restaurarBanco()
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        string usuarioSql, senhaSql;

        usuarioSql = getpass("Insira o nome do usuario Mysql:");
        senhaSql = getpass("Insira a senha do usuario Mysql:"); // yes, that's all you need!

        try
        {
            driver = get_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", usuarioSql, senhaSql);
            stmt = con->createStatement();

            stmt->execute("Create Database GerenciadorSenhas");
            string comando = "mysql -u " + usuarioSql + " -p" + senhaSql + " GerenciadorSenhas < BackupSenhas.sql ";
            system((char *)comando.c_str());
            cout << "O banco foi restaurado com sucesso." << endl;
        }
        catch (sql::SQLException &e)
        {
            cout << "Houve um problema com a aplica????o. ";
            cout << e.what();
        }
        continuar();
    }
};