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
    char phone[20];
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
string writeConvertedDataTime(time_t milliseconds);
void verifyIfFileOpened() {};

// page functions
void createClientPage();
void findClientPage();
void listClientsPage();

void createAnimalPage();
void findAnimalPage();
void listAnimalPage();

void createServicePage();
void findServicePage();
void listServicePage();

// find data functions
long int findClient(char document[20]);
long int findAnimal(char name[100], char document[20]);

string writeConvertedDataTime(time_t milliseconds) {
    struct tm* timeinfo;
    char buffer[80];

    timeinfo = localtime(&milliseconds);

    strftime(buffer, 80, "%d/%m/%y %H:%M:%S", timeinfo);
    return buffer;
}

string writeConvertedData(time_t milliseconds) {
    struct tm* timeinfo;
    char buffer[80];

    timeinfo = localtime(&milliseconds);

    strftime(buffer, 80, "%d/%m/%y", timeinfo);
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

    cout << setw(50) << "Documento do responsável (XXX.XXX.XXX-XX): ";
    cin.getline(newClient.document, sizeof(newClient.document));    
    
    cout << setw(50) << "Telefone do responsável ((XX) XXXXX-XXXX): ";
    cin.getline(newClient.phone, sizeof(newClient.phone));

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
            cout << setw(20) << "Telefone";
            cout << setw(20) << "Data de criação";
            cout << "\n---------------------------------------------------------------------------------\n";
            cout << setw(8) << client.id;
            cout << setw(30) << client.name;
            cout << setw(20) << client.document;
            cout << setw(20) << client.phone;
            cout << setw(20) << writeConvertedDataTime(client.createdAt);
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
    cout << "2. Buscar novo responsável\n";
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
    cout << setw(20) << "Telefone";
    cout << setw(20) << "Data de criação";
    cout << "\n---------------------------------------------------------------------------------\n";

    while (fread(&client, sizeof(client), 1, pontClientFile) != NULL) {
        cout << setw(8) << client.id;
        cout << setw(30) << client.name;
        cout << setw(20) << client.document;
        cout << setw(20) << client.phone;
        cout << setw(20) << writeConvertedDataTime(client.createdAt);
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

void createAnimalPage() {
    char op;
    int navigationOp;
    IClient client;
    IAnimal newAnimal;
    char document[20];
    int day, month, year;
    struct tm* timeinfo;

    newAnimal.id = 0;

    pontAnimalFile = fopen(animalPathname, "rb");
    verifyIfFileOpened(pontAnimalFile);

    while (fread(&newAnimal, sizeof(newAnimal), 1, pontAnimalFile) != NULL);

    fclose(pontAnimalFile);

    cout << "---------------------------------------------------------------------------------\n";
    cout << "                                 Cadastro de Pet                                 \n";
    cout << "---------------------------------------------------------------------------------\n\n";

    if (newAnimal.id == 0) {
        newAnimal.id = 1;
    }
    else {
        newAnimal.id = newAnimal.id + 1;
    }

    cout << setw(55) << "Digite o documento do responsável (XXX.XXX.XXX-XX): ";
    cin.ignore(80, '\n');
    cin.getline(document, sizeof(document));

    pontClientFile = fopen(clientPathname, "rb");
    verifyIfFileOpened(pontClientFile);

    while (fread(&client, sizeof(client), 1, pontClientFile) != NULL) {
        if (strcmp(client.document, document) == 0) {
            newAnimal.idClient = client.id;
            fclose(pontClientFile);
            break;
        }
    }

    fclose(pontClientFile);

    if (client.id > 0 && strcmp(client.document, document) == 0) {
        cout << setw(55) << "Nome do pet: ";
        cin.getline(newAnimal.name, sizeof(newAnimal.name));
        strupr(newAnimal.name);

        cout << setw(55) << "Dia de nascimento do pet (apenas numeros): ";
        cin >> day;

        cout << setw(55) << "Mês de nascimento do pet (apenas numeros): ";
        cin >> month;

        cout << setw(55) << "Ano de nascimento do pet (apenas numeros): ";
        cin >> year;

        time(&newAnimal.birthDate);
        timeinfo = localtime(&newAnimal.birthDate);
        timeinfo->tm_year = year - 1900;
        timeinfo->tm_mon = month - 1;
        timeinfo->tm_mday = day;

        newAnimal.birthDate = mktime(timeinfo);

        time(&newAnimal.createdAt);

        cout << "\nConfirma o cadastro do pet? Pressione S para confirmar \n";

        cin >> op;

        if (toupper(op) == 'S') {
            if (findAnimal(newAnimal.name, document) != -1) {
                cout << "Já existe um pet cadastrado com o nome informado para o responsável informado.\n";
            }
            else {
                pontAnimalFile = fopen(animalPathname, "ab");
                verifyIfFileOpened(pontAnimalFile);
                fwrite(&newAnimal, sizeof(newAnimal), 1, pontAnimalFile);
                fclose(pontAnimalFile);

                cout << "\nCadastro realizado com sucesso!\n";
            }
        }
        else {
            cout << "\nCadastro não realizado.\n";
        }
    }
    else {
        cout << "\nResponsável não encontrado!\n";
    }

    cout << "\nO que deseja fazer? \n\n";

    cout << "1. Cadastrar novo pet\n";
    cout << "2. Ver pet cadastrado\n";
    cout << "3. Ver lista de pets\n";
    cout << "4. Voltar ao menu principal\n";
    cout << "0. Sair do programa\n\n";

    do {
        cin >> navigationOp;

        switch (navigationOp) {
        case 1:
            system("cls");
            createAnimalPage();

            break;
        case 2:
            system("cls");
            findAnimalPage();

            break;
        case 3:
            system("cls");
            listAnimalPage();

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

void findAnimalPage() {
    int navigationOp;
    IAnimal animal;
    char name[100];
    bool find = false;
    pontAnimalFile = fopen(animalPathname, "rb");
    verifyIfFileOpened(pontAnimalFile);

    cout << "---------------------------------------------------------------------------------\n";
    cout << "                                  Buscar por Pet                                 \n";
    cout << "---------------------------------------------------------------------------------\n\n";

    cout << "Qual o nome do pet que deseja buscar?\n";
    cin.ignore(80, '\n');
    cin.getline(name, sizeof(name));
    strupr(name);

    while (fread(&animal, sizeof(animal), 1, pontAnimalFile) != NULL) {
        if (strcmp(name, animal.name) == 0) {
            if (!find) {
                cout << "\n---------------------------------------------------------------------------------\n";
                cout << setw(8) << "Codigo";
                cout << setw(24) << "Código do responsável";
                cout << setw(10) << "Nome";
                cout << setw(20) << "Data de nascimento";
                cout << setw(20) << "Data de criação";
                cout << "\n---------------------------------------------------------------------------------\n";
            }

            cout << setw(8) << animal.id;
            cout << setw(24) << animal.idClient;
            cout << setw(10) << animal.name;
            cout << setw(20) << writeConvertedData(animal.birthDate);
            cout << setw(20) << writeConvertedDataTime(animal.createdAt);
            cout << "\n---------------------------------------------------------------------------------\n";
            find = true;
        }
    }

    if (!find) {
        cout << "\nNenhum pet encontrado\n";
    }

    fclose(pontAnimalFile);

    cout << "\nO que deseja fazer? \n\n";

    cout << "1. Cadastrar novo pet\n";
    cout << "2. Buscar novo pet\n";
    cout << "3. Listar pets\n";
    cout << "4. Voltar ao menu principal\n";
    cout << "0. Sair do programa\n\n";

    do {
        cin >> navigationOp;

        switch (navigationOp) {
        case 1:
            system("cls");
            createAnimalPage();

            break;
        case 2:
            system("cls");
            findAnimalPage();

            break;
        case 3:
            system("cls");
            listAnimalPage();

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

void listAnimalPage() {
    int navigationOp;
    IAnimal animal;
    pontAnimalFile = fopen(animalPathname, "rb");
    verifyIfFileOpened(pontAnimalFile);

    cout << "---------------------------------------------------------------------------------\n";
    cout << "                                Relatório de Pets                                \n";
    cout << "---------------------------------------------------------------------------------\n";
    cout << setw(8) << "Codigo";
    cout << setw(24) << "Código do responsável";
    cout << setw(10) << "Nome";
    cout << setw(20) << "Data de nascimento";
    cout << setw(20) << "Data de criação";
    cout << "\n---------------------------------------------------------------------------------\n";

    while (fread(&animal, sizeof(animal), 1, pontAnimalFile) != NULL) {
        cout << setw(8) << animal.id;
        cout << setw(24) << animal.idClient;
        cout << setw(10) << animal.name;
        cout << setw(20) << writeConvertedData(animal.birthDate);
        cout << setw(20) << writeConvertedDataTime(animal.createdAt);
        cout << "\n---------------------------------------------------------------------------------\n";
    }

    fclose(pontAnimalFile);

    cout << "\nO que deseja fazer? \n\n";

    cout << "1. Cadastrar novo pet\n";
    cout << "2. Buscar por pet\n";
    cout << "3. Voltar ao menu principal\n";
    cout << "0. Sair do programa\n\n";

    do {
        cin >> navigationOp;

        switch (navigationOp) {
        case 1:
            system("cls");
            createAnimalPage();

            break;
        case 2:
            system("cls");
            findAnimalPage();

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

long int findAnimal(char name[100], char document[20]) {
    IClient client;
    IAnimal animal;
    pontClientFile = fopen(clientPathname, "rb");
    verifyIfFileOpened(pontClientFile);
    pontAnimalFile = fopen(animalPathname, "rb");
    verifyIfFileOpened(pontAnimalFile);
    bool findClient = false;

    while (fread(&client, sizeof(client), 1, pontClientFile) != NULL) {
        if (strcmp(client.document, document) == 0) {
            findClient = true;
            break;
        }
    }

    fclose(pontClientFile);

    if (!findClient) {
        fclose(pontAnimalFile);
        return -1;
    }

    while (fread(&animal, sizeof(animal), 1, pontAnimalFile) != NULL) {
        if (strcmp(animal.name, name) == 0 && client.id == animal.idClient) {
            int posicao = ftell(pontAnimalFile) - sizeof(animal);
            fclose(pontAnimalFile);
            return posicao;
        }
    }

    fclose(pontAnimalFile);
    return -1;
}

void createServicePage() {
    char op;
    int navigationOp;
    IClient client;
    IAnimal animal;
    IService newService;
    char clientDocument[20];
    char petName[100];
    bool findClient = false;

    newService.id = 0;

    pontServiceFile = fopen(servicePathname, "rb");
    verifyIfFileOpened(pontServiceFile);

    while (fread(&newService, sizeof(newService), 1, pontServiceFile) != NULL);

    fclose(pontServiceFile);

    cout << "---------------------------------------------------------------------------------\n";
    cout << "                               Cadastro de Serviço                               \n";
    cout << "---------------------------------------------------------------------------------\n\n";

    if (newService.id == 0) {
        newService.id = 1;
    }
    else {
        newService.id = newService.id + 1;
    }

    cout << setw(55) << "Digite o documento do responsável (XXX.XXX.XXX-XX): ";
    cin.ignore(80, '\n');
    cin.getline(clientDocument, sizeof(clientDocument));
    
    cout << setw(55) << "Digite o nome do pet: ";
    cin.getline(petName, sizeof(petName));
    strupr(petName);

    pontClientFile = fopen(clientPathname, "rb");
    verifyIfFileOpened(pontClientFile);

    while (fread(&client, sizeof(client), 1, pontClientFile) != NULL) {
        if (strcmp(client.document, clientDocument) == 0) {
            findClient = true;
            fclose(pontClientFile);
            break;
        }
    }    

    fclose(pontClientFile);

    pontAnimalFile = fopen(animalPathname, "rb");
    verifyIfFileOpened(pontAnimalFile);
    
    while (fread(&animal, sizeof(animal), 1, pontAnimalFile) != NULL) {
        if (strcmp(animal.name, petName) == 0 && strcmp(client.document, clientDocument) == 0) {
            newService.idAnimal = animal.id;
            fclose(pontAnimalFile);
            break;
        }
    }

    fclose(pontAnimalFile);

    if (animal.id > 0 && findClient == true) {
        cout << setw(55) << "Tipo do serviço: ";
        cin.getline(newService.serviceType, sizeof(newService.serviceType));
        strupr(newService.serviceType);
        
        cout << setw(55) << "Descrição do serviço: ";
        cin.getline(newService.description, sizeof(newService.description));
        strupr(newService.description);
        
        cout << setw(55) << "Valor do serviço: ";
        cin >> newService.value;

        time(&newService.createdAt);

        cout << "\nConfirma o cadastro do serviço? Pressione S para confirmar \n";

        cin >> op;

        if (toupper(op) == 'S') {
            pontServiceFile = fopen(servicePathname, "ab");
            verifyIfFileOpened(pontServiceFile);
            fwrite(&newService, sizeof(newService), 1, pontServiceFile);
            fclose(pontServiceFile);

            cout << "\nCadastro realizado com sucesso!\n";
        }
        else {
            cout << "\nCadastro não realizado.\n";
        }
    }
    else {
        cout << "\nResponsável ou pet não encontrado!\n";
    }

    cout << "\nO que deseja fazer? \n\n";

    cout << "1. Cadastrar novo serviço\n";
    cout << "2. Ver serviço cadastrado\n";
    cout << "3. Ver lista de serviços\n";
    cout << "4. Voltar ao menu principal\n";
    cout << "0. Sair do programa\n\n";

    do {
        cin >> navigationOp;

        switch (navigationOp) {
            case 1:
                system("cls");
                createServicePage();

                break;
            case 2:
                system("cls");
                findServicePage();

                break;
            case 3:
                system("cls");
                listServicePage();

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

void findServicePage() {
    int navigationOp;
    IService service;
    char serviceType[100];
    bool find = false;
    pontServiceFile = fopen(servicePathname, "rb");
    verifyIfFileOpened(pontServiceFile);

    cout << "---------------------------------------------------------------------------------\n";
    cout << "                                Buscar por Serviço                               \n";
    cout << "---------------------------------------------------------------------------------\n\n";

    cout << "Qual o tipo de serviço que deseja buscar?\n";
    cin.ignore(80, '\n');
    cin.getline(serviceType, sizeof(serviceType));
    strupr(serviceType);

    while (fread(&service, sizeof(service), 1, pontServiceFile) != NULL) {
        if (strcmp(serviceType, service.serviceType) == 0) {
            if (!find) {
                cout << "\n---------------------------------------------------------------------------------\n";
                cout << setw(8) << "Codigo";
                cout << setw(24) << "Código do pet";
                cout << setw(20) << "Tipo do serviço";
                cout << setw(40) << "Descrição do serviço";
                cout << setw(20) << "Valor do serviço";
                cout << setw(20) << "Data de criação";
                cout << "\n---------------------------------------------------------------------------------\n";
            }

            cout << setw(8) << service.id;
            cout << setw(24) << service.idAnimal;
            cout << setw(20) << service.serviceType;
            cout << setw(40) << service.description;
            cout << setw(20) << service.value;
            cout << setw(20) << writeConvertedDataTime(service.createdAt);
            cout << "\n---------------------------------------------------------------------------------\n";
            find = true;
        }
    }

    if (!find) {
        cout << "\nNenhum serviço encontrado\n";
    }

    fclose(pontServiceFile);

    cout << "\nO que deseja fazer? \n\n";

    cout << "1. Cadastrar novo serviço\n";
    cout << "2. Buscar novo serviço\n";
    cout << "3. Listar serviços\n";
    cout << "4. Voltar ao menu principal\n";
    cout << "0. Sair do programa\n\n";

    do {
        cin >> navigationOp;

        switch (navigationOp) {
        case 1:
            system("cls");
            createServicePage();

            break;
        case 2:
            system("cls");
            findServicePage();

            break;
        case 3:
            system("cls");
            listServicePage();

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

void listServicePage() {
    int navigationOp;
    IService service;
    pontServiceFile = fopen(servicePathname, "rb");
    verifyIfFileOpened(pontServiceFile);

    cout << "---------------------------------------------------------------------------------\n";
    cout << "                              Relatório de Serviços                              \n";
    cout << "---------------------------------------------------------------------------------\n";
    cout << setw(8) << "Codigo";
    cout << setw(24) << "Código do pet";
    cout << setw(20) << "Tipo do serviço";
    cout << setw(40) << "Descrição do serviço";
    cout << setw(20) << "Valor do serviço";
    cout << setw(20) << "Data de criação";
    cout << "\n---------------------------------------------------------------------------------\n";

    while (fread(&service, sizeof(service), 1, pontServiceFile) != NULL) {
        cout << setw(8) << service.id;
        cout << setw(24) << service.idAnimal;
        cout << setw(20) << service.serviceType;
        cout << setw(40) << service.description;
        cout << setw(20) << service.value;
        cout << setw(20) << writeConvertedDataTime(service.createdAt);
        cout << "\n---------------------------------------------------------------------------------\n";
    }

    fclose(pontServiceFile);

    cout << "\nO que deseja fazer? \n\n";

    cout << "1. Cadastrar novo serviço\n";
    cout << "2. Buscar por serviço\n";
    cout << "3. Voltar ao menu principal\n";
    cout << "0. Sair do programa\n\n";

    do {
        cin >> navigationOp;

        switch (navigationOp) {
        case 1:
            system("cls");
            createServicePage();

            break;
        case 2:
            system("cls");
            findServicePage();

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