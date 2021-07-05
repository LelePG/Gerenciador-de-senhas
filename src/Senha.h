#include <iostream>
#include <string>
#include <mysql/mysql.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <mysql_driver.h>

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

    string getAplicacao()
    {
        return this->aplicacao;
    }

    string getSenha()
    {
        return this->senha;
    }

    string getData()
    {
        return this->data;
    }
};