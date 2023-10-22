#include <iostream>
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include <time.h>
#include <locale.h>

#pragma warning(disable : 4996) //disable CRT_SECURE_NO_WARNINGS in visual studio

using namespace std;

const char* clientPathname = "clientDatabase.dat";
const char* animalPathname = "animalDatabase.dat";
const char* servicePathname = "serviceDatabase.dat";

FILE* pontClientFile;
FILE* pontAnimalFile;
FILE* pontServiceFile;

struct IClient {
    int id; // primary key, unique
    char document[20]; // unique
    char name[100];
    time_t createdAt;
};

struct IAnimal {
    int id; // primary key, unique
    int idClient; // foreign key 
    char name[100];
    time_t createdAt;
    time_t birthDate;
};

struct IService {
    int id; // primary key, unique
    int idAnimal; // foreign key 
    char serviceType[100];
    char description[100];
    float value;
    time_t createdAt;
};

// menu functions
void menu();

// hook functions
string writeConvertedData(time_t milliseconds);
void verifyIfFileOpened() {};

// page functions
void createClientPage();
void findClientPage();
void listClientsPage();

void createAnimalPage() {};
void findAnimalPage() {};
void listAnimalPage() {};

void createServicePage() {};
void findServicePage() {};
void listServicePage() {};

// find data functions
long int findClient(char document[20]);

string writeConvertedData(time_t milliseconds) {
    struct tm* timeinfo;
    char buffer[80];

    timeinfo = localtime(&milliseconds);

    strftime(buffer, 80, "%d/%m/%y %H:%M:%S", timeinfo);
    return buffer;
}

void verifyIfFileOpened(FILE* arq) {
    if (arq == NULL)
    {
        puts("Arquivo não pode ser aberto");
        system("pause");
        exit(1);
    }
}

void menu() {
    int op;

    cout << "---------------------------------------------------------------------------------\n";
    cout << "                          Sistema de Gestão de Pet Shop                          \n";
    cout << "---------------------------------------------------------------------------------\n\n";

    cout << "Menu:\n";
    cout << "1. Cadastrar Responsável\n";
    cout << "2. Cadastrar Pet\n";
    cout << "3. Gerar Serviço\n";
    cout << "4. Listar Responsáveis\n";
    cout << "5. Listar pets\n";
    cout << "6. Listar serviços\n";
    cout << "0. Sair do programa\n\n";

    cin >> op;

    do {
        switch (op) {
        case 1:
            system("cls");
            createClientPage();

            break;
        case 2:
            system("cls");
            createAnimalPage();

            break;
        case 3:
            system("cls");
            createServicePage();

            break;
        case 4:
            system("cls");
            listClientsPage();

            break;
        case 5:
            system("cls");
            listAnimalPage();

            break;
        case 6:
            system("cls");
            listServicePage();

            break;
        case 0:
            exit(0);

            break;
        }
    } while (op != 0);
}

void createClientPage() {
    char op;
    int navigationOp;
    IClient newClient;

    newClient.id = 0;

    pontClientFile = fopen(clientPathname, "rb");
    verifyIfFileOpened(pontClientFile);
    
    while (fread(&newClient, sizeof(newClient), 1, pontClientFile) != NULL);

    cout << newClient.id;
    
    fclose(pontClientFile);

    cout << "---------------------------------------------------------------------------------\n";
    cout << "                             Cadastro de Responsável                             \n";
    cout << "---------------------------------------------------------------------------------\n\n";

    if (newClient.id == 0) {
        newClient.id = 1;
    }
    else {
        newClient.id = newClient.id + 1;
    }

    cout << setw(50) << "Nome completo do responsável: ";
    cin.ignore(80, '\n');
    cin.getline(newClient.name, sizeof(newClient.name));
    strupr(newClient.name);

    cout << setw(50) << "Documento do responsável (apenas números): ";
    cin.getline(newClient.document, sizeof(newClient.document));

    time(&newClient.createdAt);

    cout << "\nConfirma o cadastro do responsável? Pressione S para confirmar \n";

    cin >> op;

    if (toupper(op) == 'S') {
        if (findClient(newClient.document) != -1) {
            cout << "Já existe um usuário cadastrado com o documento informado. O que deseja fazer? \n\n";
        } else {
            pontClientFile = fopen(clientPathname, "ab");
            verifyIfFileOpened(pontClientFile);
            fwrite(&newClient, sizeof(newClient), 1, pontClientFile);
            fclose(pontClientFile);

            cout << "\nCadastro realizado com sucesso! O que deseja fazer? \n\n";
        }
    } else {
        cout << "\nCadastro não realizado. O que deseja fazer? \n\n";
    }

    cout << "1. Cadastrar novo responsável\n";
    cout << "2. Ver responsável cadastrado\n";
    cout << "3. Ver lista de responsáveis\n";
    cout << "4. Voltar ao menu principal\n";
    cout << "0. Sair do programa\n\n";

    do {
        cin >> navigationOp;

        switch (navigationOp) {
            case 1:
                system("cls");
                createClientPage();

                break;
            case 2:
                system("cls");
                findClientPage();

                break;
            case 3:
                system("cls");
                listClientsPage();

                break;
            case 4:
                system("cls");
                menu();

                break;
            case 0: 
                exit(0);

                break;
        }
    } while (navigationOp != 0);
}

long int findClient(char document[20]) {
    IClient client;
    pontClientFile = fopen(clientPathname, "rb");
    verifyIfFileOpened(pontClientFile);

    while (fread(&client, sizeof(client), 1, pontClientFile) != NULL) {
        if (strcmp(client.document, document) == 0) {
            int posicao = ftell(pontClientFile) - sizeof(client);
            fclose(pontClientFile);
            return posicao;
        }
    }

    fclose(pontClientFile);
    return -1;
}

void findClientPage() {
    int navigationOp;
    IClient client;
    char document[20];
    bool find = false;
    pontClientFile = fopen(clientPathname, "rb");
    verifyIfFileOpened(pontClientFile);

    cout << "---------------------------------------------------------------------------------\n";
    cout << "                              Buscar por Responsável                             \n";
    cout << "---------------------------------------------------------------------------------\n\n";

    cout << "Qual o documento do responsável que deseja buscar?\n";
    cin >> document;

    while (fread(&client, sizeof(client), 1, pontClientFile) != NULL) {
        if (strcmp(document, client.document) == 0) {
            cout << "---------------------------------------------------------------------------------\n";
            cout << setw(8) << "Codigo";
            cout << setw(30) << "Nome";
            cout << setw(20) << "Documento";
            cout << setw(20) << "Data de criação";
            cout << "\n---------------------------------------------------------------------------------\n";
            cout << setw(8) << client.id;
            cout << setw(30) << client.name;
            cout << setw(20) << client.document;
            cout << setw(20) << writeConvertedData(client.createdAt);
            cout << "\n---------------------------------------------------------------------------------\n";
            find = true;
        }
    }

    if (!find) {
        cout << "\nNenhum representante encontrado\n";
    }

    fclose(pontClientFile);

    cout << "\nO que deseja fazer? \n\n";

    cout << "1. Cadastrar novo responsável\n";
    cout << "2. Buscar novo responsáveis\n";
    cout << "3. Listar responsáveis\n";
    cout << "4. Voltar ao menu principal\n";
    cout << "0. Sair do programa\n\n";

    do {
        cin >> navigationOp;

        switch (navigationOp) {
        case 1:
            system("cls");
            createClientPage();

            break;
        case 2:
            system("cls");
            findClientPage();

            break;       
        case 3:
            system("cls");
            listClientsPage();

            break;
        case 4:
            system("cls");
            menu();

            break;
        case 0:
            exit(0);

            break;
        }
    } while (navigationOp != 0);
}

void listClientsPage() {
    int navigationOp;
    IClient client;
    pontClientFile = fopen(clientPathname, "rb");
    verifyIfFileOpened(pontClientFile);

    cout << "---------------------------------------------------------------------------------\n";
    cout << "                            Relatório de Responsáveis                            \n";
    cout << "---------------------------------------------------------------------------------\n";
    cout << setw(8) << "Codigo";
    cout << setw(30) << "Nome";
    cout << setw(20) << "Documento";
    cout << setw(20) << "Data de criação";
    cout << "\n---------------------------------------------------------------------------------\n";

    while (fread(&client, sizeof(client), 1, pontClientFile) != NULL) {
        cout << setw(8) << client.id;
        cout << setw(30) << client.name;
        cout << setw(20) << client.document;
        cout << setw(20) << writeConvertedData(client.createdAt);
        cout << "\n---------------------------------------------------------------------------------\n";
    }

    fclose(pontClientFile);

    cout << "\nO que deseja fazer? \n\n";

    cout << "1. Cadastrar novo responsável\n";
    cout << "2. Buscar por responsável\n";
    cout << "3. Voltar ao menu principal\n";
    cout << "0. Sair do programa\n\n";

    do {
        cin >> navigationOp;

        switch (navigationOp) {
        case 1:
            system("cls");
            createClientPage();

            break;
        case 2:
            system("cls");
            findClientPage();

            break;
        case 3:
            system("cls");
            menu();

            break;
        case 0:
            exit(0);

            break;
        }
    } while (navigationOp != 0);
}

int main()
{
    setlocale(LC_ALL, "Portuguese");

    menu();

    return 0;
}