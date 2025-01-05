

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;

void Login();
void ShowManageUsersScreen();
void ShowTransactionsScreen();
void ShowMainMenueScreen();
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";
enum enUserPermissions
{
    pAll = -1,pShowListClients=1,pAddNewClient=2,pDeleteClient=4,pUpdateClient=8,pFindClient=16,pTransactions=32,pManageUsers=64
};
enum enManageUsersMenueOption
{
    eListUsers=1,eAddNewUser=2,eDeleteUser=3,eUpdateUser=4,eFindUser=5,eMainMenue=6
};
enum enMainMenueOption
{
    eShowClientList = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eTransactions = 6,eManageUsers=7, eLogout = 8
};
enum enTransactionsMenueOption
{
    Deposit = 1, Withdraw = 2, TotalBalance = 3, MainMenue = 4
};
struct sClient
{
    string AccNum, Pincode, Name, Phone;
    double Balance;
    bool MarkedForDelete = false;
};
struct sUser
{
    string UserName;
    string Password;
    int Permissions;
    bool MarkForDelete = false;
};
sUser CurrentUser;
vector<string>SplitString(string Text, string delim = "#//#")
{
    short Pos = 0;
    vector<string>vWords;
    string Word = "";
    while ((Pos = Text.find(delim)) != string::npos)
    {
        Word = Text.substr(0, Pos);
        if (!Word.empty())
        {
            vWords.push_back(Word);
        }
        Text = Text.erase(0, Pos + delim.length());
    }
    if (!Text.empty())
    {
        vWords.push_back(Text);
    }
    return vWords;
}
sUser ConvertLineToUser(string Line)
{
    sUser User;
    vector<string>vData = SplitString(Line);
    User.UserName = vData[0];
    User.Password = vData[1];
    User.Permissions = stoi(vData[2]);
    return User;
}
string ConvertUserToLine(sUser User, string Separator = "#//#")
{
    string Line = "";
    Line += User.UserName + Separator;
    Line += User.Password + Separator;
    Line += to_string(User.Permissions);
    return Line;
}
vector<sUser>LoadUsersDataFromFile(string FileName)
{
    vector<sUser>vUsers;
    fstream File;
    File.open(FileName, ios::in);
    if (File.is_open())
    {
        sUser User;
        string Line;
        while (getline(File, Line))
        {
            User = ConvertLineToUser(Line);
            vUsers.push_back(User);
        }
        File.close();
    }
    return vUsers;
}
bool CheckAccessNotDenied(enUserPermissions Permission)
{
    if (CurrentUser.Permissions == enUserPermissions::pAll)
        return false;
    if ((CurrentUser.Permissions & Permission) == Permission)
        return false;
    return true;
}
void ShowAccessDeniedMessage()
{
    cout << "==============================================================================================================\n";
    cout << "\t\tAccess Denied!\n\t\t You do not have the permission to open this .Please contact your Admin.\n";
    cout << "==============================================================================================================\n";
}
void SaveUsersVectorToFile(vector<sUser> vUsers, string UsersFileName)
{
    fstream File;
    File.open(UsersFileName, ios::out);
    if (File.is_open())
    {
        string Line;
        for (sUser& U : vUsers)
        {
            if (U.MarkForDelete == false)
            {
                Line = ConvertUserToLine(U);
                File << Line << endl;
            }
        }
        File.close();
    }
}

void PrintUserCard(sUser User)
{
    cout << right << setw(25) << "UserName:" << User.UserName << endl;
    cout << right << setw(25) << "Password:" << User.Password << endl;
    cout << right << setw(25) << "Permissions:" << User.Permissions << endl;
}
void PrintUserLine(sUser User)
{
    cout << "|" << left << setw(20) << User.UserName;
    cout << "|" << left << setw(15) << User.Password;
    cout << "|" << left << setw(15) << User.Permissions;
}
void ShowListUsers()
{
    vector<sUser>vUsers = LoadUsersDataFromFile(UsersFileName);
    cout << "\t\tUser List (" << vUsers.size() << ") User(s)\n";
    cout << "-------------------------------------------------------------------------------\n";
    if (vUsers.size() == 0)
    {
        cout << "\t\tNo Users are found !\n";
    }
    else
    {
        cout << "|User Name           |Password       |Permissions\n";
        for (sUser& U : vUsers)
        {
            PrintUserLine(U);
            cout << endl;
        }
    }
    cout << "-------------------------------------------------------------------------------\n\n\n";

}
string ConvertClientToLine(sClient Client, string Seperator = "#//#")
{
    string Line = "";
    Line += Client.AccNum + Seperator;
    Line += Client.Pincode + Seperator;
    Line += Client.Name + Seperator;
    Line += Client.Phone + Seperator;
    Line += to_string(Client.Balance);
    return Line;
}
string ReadClientAccountNumber()
{
    string AccNum;
    cout << "\nEnter Account Number?";
    getline(cin >> ws, AccNum);
    return AccNum;
}
void SaveClientsVectorToFile(vector<sClient>vClients, string FileName)
{
    fstream File;
    File.open(FileName, ios::out);//only out to renew records
    if (File.is_open())
    {
        string Line = "";
        for (sClient C : vClients)
        {
            if (C.MarkedForDelete == false)
            {
                Line = ConvertClientToLine(C);
                File << Line << endl;
            }
        }
        File.close();
    }
}
bool FindClientByAccountNumber(string AccNum, sClient& Client, vector<sClient>vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccNum == AccNum)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
sClient ConvertLineToClient(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string>vData = SplitString(Line, Seperator);
    Client.AccNum = vData[0];
    Client.Pincode = vData[1];
    Client.Name = vData[2];
    Client.Phone = vData[3];
    Client.Balance = stod(vData[4]);
    return Client;
}
vector<sClient>LoadClientsDataFromFile(string FileName)
{
    vector<sClient>vClients;
    fstream File;
    File.open(FileName, ios::in);
    if (File.is_open())
    {
        string Line = "";
        sClient Client;
        while (getline(File, Line))
        {
            Client = ConvertLineToClient(Line);
            vClients.push_back(Client);
        }
        File.close();
    }
    return vClients;
}
void PrintClientCard(sClient Client)
{
    cout << right << setw(20) << "Account Number:" << Client.AccNum << endl;
    cout << right << setw(20) << "Pincode:" << Client.Pincode << endl;
    cout << right << setw(20) << "Name:" << Client.Name << endl;
    cout << right << setw(20) << "Phone:" << Client.Phone << endl;
    cout << right << setw(20) << "Balance:" << Client.Balance << endl;
}
short ReadTransactionsMenueOption()
{
    short option;
    do
    {
        cout << "Choose what do you want to do? [1:4] ?";
        cin >> option;
    } while (option < 1 || option>4);
    return option;
}
void GoBackToTransactionsMenueScreen()
{
    cout << "Press any key to go back to Transactions Menue Screen....\n";
    system("pause>0");
    ShowTransactionsScreen();
}
bool DepositToClientAndSaveToFile(double Amount, sClient Client, vector<sClient> vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccNum == Client.AccNum)
        {
            C.Balance += Amount;
            SaveClientsVectorToFile(vClients, ClientsFileName);
            cout << "Transaction Succeded .You have Now :" << C.Balance << endl;
            return true;
        }
    }
    return false;
}
void ShowWithrawScreen()
{
    string AccNum;
    sClient Client;
    double Amount;
    vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "-------------------------------------------------------------------\n";
    cout << "\t\tWithdraw Screen\n";
    cout << "-------------------------------------------------------------------\n";
    cout << "\nEnter Account Number?";
    getline(cin >> ws, AccNum);
    while (!FindClientByAccountNumber(AccNum, Client, vClients))
    {
        cout << "\nNo Client found with Account Number :" << AccNum << "! Enter another one:";
        getline(cin >> ws, AccNum);
    }
    PrintClientCard(Client);
    cout << endl << endl;
    cout << "\nEnter Amount you want to withdraw:";
    cin >> Amount;
    while (Amount > Client.Balance)
    {
        cout << "You only have " << Client.Balance << "! Try less amount:";
        cin >> Amount;
    }
    DepositToClientAndSaveToFile(-1 * Amount, Client, vClients);
}
void ShowDepositScreen()
{
    string AccNum;
    sClient Client;
    double Amount;
    vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "-------------------------------------------------------------------\n";
    cout << "\t\tDeposit Screen\n";
    cout << "-------------------------------------------------------------------\n";
    cout << "\nEnter Account Number?";
    getline(cin >> ws, AccNum);
    while (!FindClientByAccountNumber(AccNum, Client, vClients))
    {
        cout << "\nNo Client found with Account Number :" << AccNum << "! Enter another one:";
        getline(cin >> ws, AccNum);
    }
    PrintClientCard(Client);
    cout << endl << endl;
    cout << "\nEnter Amount you want to deposit:";
    cin >> Amount;
    DepositToClientAndSaveToFile(Amount, Client, vClients);
}
void PrintClientBalanceLine(sClient Client)
{
    cout << "|" << left << setw(17) << Client.AccNum;
    cout << "|" << left << setw(18) << Client.Name;
    cout << "|" << left << setw(13) << Client.Balance;

}
void ShowTotalBalancesScreen()
{
    vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
    double Total = 0;
    cout << "----------------------------------------------------------------------\n";
    cout << "\t\tTotal Balances Screen\n";
    cout << "----------------------------------------------------------------------\n";
    if (vClients.size() == 0)
    {
        cout << "No Balance is found!\n";
    }
    else
    {
        cout << "|Account Number   |Name              |Total Balance    \n";
        cout << "----------------------------------------------------------------------\n";
        for (sClient& Client : vClients)
        {
            Total += Client.Balance;
            PrintClientBalanceLine(Client);
            cout << endl;
        }
        cout << "----------------------------------------------------------------------\n";
        cout << "\t\tTotal Balances=" << Total;
    }


}
void PerformTransactionsMenueOption(enTransactionsMenueOption option)
{
    switch (option)
    {
    case Deposit:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenueScreen();
        break;
    case Withdraw:
        system("cls");
        ShowWithrawScreen();
        GoBackToTransactionsMenueScreen();
        break;
    case TotalBalance:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenueScreen();
        break;
    case MainMenue:
        ShowMainMenueScreen();
    }
}
void ShowTransactionsScreen()
{
    if (CheckAccessNotDenied(enUserPermissions::pTransactions))
    {
        ShowAccessDeniedMessage();
        return;
    }
    system("cls");
    cout << "===============================================================================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===============================================================================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===============================================================================================\n";
    cout << "Choose what do you want to do ? [1:6]?";
    PerformTransactionsMenueOption((enTransactionsMenueOption)ReadTransactionsMenueOption());

}
void ShowFindClientScreen()
{
    if (CheckAccessNotDenied(enUserPermissions::pFindClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    string AccNum;
    sClient Client;
    vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "---------------------------------------------------------------------\n";
    cout << "\t\tFind Client Screen\n";
    cout << "---------------------------------------------------------------------\n";
    cout << "\nEnter Account Number?\n\n";
    cin >> AccNum;
    if (FindClientByAccountNumber(AccNum, Client, vClients))
    {
        PrintClientCard(Client);
    }
    else
    {
        cout << "\t\tNo Client found with Account Number:" << AccNum << endl;
    }



}
sClient UpdateClientRecord(string AccNum)
{
    sClient Client;
    Client.AccNum = AccNum;
    cout << "\nEnter Pincode?";
    getline(cin >> ws, Client.Pincode);
    cout << "\nEnter Name?";
    getline(cin, Client.Name);
    cout << "\nEnter Phone?";
    getline(cin, Client.Phone);
    cout << "\nEnter Balance?";
    cin >> Client.Balance;
    return Client;
}
bool UpdateClientByAccountNumberFromFile(string AccNum, string FileName, vector<sClient>vClients)
{
    sClient Client;
    char Sure;
    if (!FindClientByAccountNumber(AccNum, Client, vClients))
    {
        cout << "\nClient with Account number:" << AccNum << " does not exist!\n";
    }
    else
    {
        //MarkClientForDelete(Client, vClients);
        cout << "\nAre you sure you want to delete this Client?";
        cin >> Sure;
        if (Sure == 'y' || Sure == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccNum == AccNum)
                {
                    C = UpdateClientRecord(AccNum);
                }
            }
            SaveClientsVectorToFile(vClients, ClientsFileName);
            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\nClient Updated Successfully.\n";
            return true;
        }

    }
    return false;
}
void ShowUpdateClientScreen()
{
    if (CheckAccessNotDenied(enUserPermissions::pUpdateClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "---------------------------------------------------------------------\n";
    cout << "\t\tUpdate Client Screen\n";
    cout << "---------------------------------------------------------------------\n";
    vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumberFromFile(AccountNumber, ClientsFileName, vClients);
}
string ReadUserName()
{
    string UserName;
    cout << "\nEnter User Name?";
    getline(cin >> ws, UserName);
    return UserName;
}
bool MarkClientForDelete(sClient Client, vector<sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (Client.AccNum == C.AccNum)
        {
            C.MarkedForDelete = true;
            return true;
        }
    }
    return false;
}
bool DeleteClientByAccountNumberFromFile(string AccNum, string FileName, vector<sClient>vClients)
{
    sClient Client;
    char Sure;
    if (AccNum == "Admin")
    {
        cout << "You can not delete Administrator Account!\n";
        return false;
    }
    if (!FindClientByAccountNumber(AccNum, Client, vClients))
    {
        cout << "\nClient with Account number:" << AccNum << " does not exist!\n";
    }
    else
    {
        cout << "\nAre you sure you want to delete this Client?";
        cin >> Sure;
        if (Sure == 'y' || Sure == 'Y')
        {
            MarkClientForDelete(Client, vClients);
            SaveClientsVectorToFile(vClients, ClientsFileName);
            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\nClient Deleted Successfully.\n";
            return true;
        }
    }
    return false;
}
void ShowDeleteClientScreen()
{
    if (CheckAccessNotDenied(enUserPermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "---------------------------------------------------------------------\n";
    cout << "\t\tDelete Client Screen\n";
    cout << "---------------------------------------------------------------------\n";
    vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumberFromFile(AccountNumber, ClientsFileName, vClients);
}
void AddNewClientLineToFile(string FileName, string Line)
{
    fstream File;
    File.open(FileName, ios::out | ios::app);
    if (File.is_open())
    {
        File << Line << endl;
        File.close();
    }
}
bool ClientExistsByAccountNumber(string AccNum)
{
    vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
    for (sClient& Client : vClients)
    {
        if (AccNum == Client.AccNum)
        {
            return true;
        }
    }
    return false;
}
sClient ReadNewClient()
{
    sClient Client;
    cout << "\nEnter Account Number ? ";
    getline(cin >> ws, Client.AccNum);
    while (ClientExistsByAccountNumber(Client.AccNum))
    {
        cout << "\nClient with Account Number" << Client.AccNum << " already exists.Enter another Account Number ? ";
        getline(cin >> ws, Client.AccNum);
    }
    cout << "\nEnter Pincode?";
    getline(cin, Client.Pincode);
    cout << "\nEnter Name?";
    getline(cin, Client.Name);
    cout << "\nEnter Phone?";
    getline(cin, Client.Phone);
    cout << "\nEnter Balance?";
    cin >> Client.Balance;
    return Client;
}
int ReadUserPermissions()
{
    char Give;
    int Permissions = 0;
    cout << "\nDo you want to give full access? Y/N?";
    cin >> Give;
    if (Give == 'Y' || Give == 'y')
    {
        return -1;
    }
    cout << "\nDo you want to give access to show client list? Y/N?";
    cin >> Give;
    if (Give == 'y' || Give == 'Y')
    {
        Permissions += enUserPermissions::pShowListClients;
    }
    cout << "\nDo you want to give access to add new client? Y/N?";
    cin >> Give;
    if (Give == 'y' || Give == 'Y')
    {
        Permissions += enUserPermissions::pAddNewClient;
    }
    cout << "\nDo you want to give access to delete client? Y/N?";
    cin >> Give;
    if (Give == 'y' || Give == 'Y')
    {
        Permissions += enUserPermissions::pDeleteClient;
    }
    cout << "\nDo you want to give access to update client? Y/N?";
    cin >> Give;
    if (Give == 'y' || Give == 'Y')
    {
        Permissions += enUserPermissions::pUpdateClient;
    }
    cout << "\nDo you want to give access to find client? Y/N?";
    cin >> Give;
    if (Give == 'y' || Give == 'Y')
    {
        Permissions += enUserPermissions::pFindClient;
    }
    cout << "\nDo you want to give access to transactions? Y/N?";
    cin >> Give;
    if (Give == 'y' || Give == 'Y')
    {
        Permissions += enUserPermissions::pTransactions;
    }
    cout << "\nDo you want to give access to manage users? Y/N?";
    cin >> Give;
    if (Give == 'y' || Give == 'Y')
    {
        Permissions += enUserPermissions::pManageUsers;
    }
    return Permissions;
}
bool FindUserByUserName(string UserName, vector<sUser>vUsers, sUser& User)
{
    for (sUser& U : vUsers)
    {
        if (U.UserName == UserName)
        {
            User = U;
            return true;
        }
    }
    return false;
}
void AddNewClient()
{
    sClient Client = ReadNewClient();
    AddNewClientLineToFile(ClientsFileName, ConvertClientToLine(Client));
}
void AddNewClients()
{
    char AddMore;
    do
    {
        system("cls");
        cout << "\nAdding new client:\n";
        AddNewClient();
        cout << "\nClient added successfully.Do you want to add more clients?Y/N?";
        cin >> AddMore;
    } while (AddMore == 'y' || AddMore == 'Y');
}
void ShowAddNewClientScreen()
{
    if (CheckAccessNotDenied(enUserPermissions::pAddNewClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "---------------------------------------------------------------------\n";
    cout << "\t\tAdd New Client Screen";
    cout << "---------------------------------------------------------------------\n";
    AddNewClients();
}
void PrintClientLine(sClient Client)
{
    cout << "|" << left << setw(15) << Client.AccNum;
    cout << "|" << left << setw(10) << Client.Pincode;
    cout << "|" << left << setw(25) << Client.Name;
    cout << "|" << left << setw(15) << Client.Phone;
    cout << "|" << left << setw(9) << Client.Balance;
}
void ShowClientListScreen()
{
    if (CheckAccessNotDenied(enUserPermissions::pShowListClients))
    {
        ShowAccessDeniedMessage();
        return;
    }
    vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "\t\tClient List " << vClients.size() << " Client(s).\n\n";
    if (vClients.size() == 0)
    {
        cout << "\t\tNo Clients Found In The List!\n";
    }
    else
    {
        cout << "-------------------------------------------------------------------------------\n";
        cout << "|Account Number |Pincode   |ClientName               |Phone          |Balance  \n";
        cout << "-------------------------------------------------------------------------------\n";
        for (sClient& Client : vClients)
        {
            PrintClientLine(Client);
            cout << endl;
        }

    }
    cout << "\n\n-------------------------------------------------------------------------------\n";
    cout << "-------------------------------------------------------------------------------\n";
}
void GoBackToMainMenueScreen()
{
    cout << "Press any key to go back to main menue screen....\n";
    system("pause>0");
    ShowMainMenueScreen();
}
void ShowEndScreen()
{
    cout << "====================================================================\n";
    cout << "\t\tProgram Ends....!\n";
    cout << "====================================================================\n";
}
short ReadManageUsersMenueOption()
{
    short option;
    do
    {
        cout << "\nChoose what do you want to do? [1:6]?";
        cin >> option;
    } while (option<1||option>6);
    return option;
}
void GoBackToManageUsersScreen()
{
    cout << "Press any key to go back to manage users screen....\n";
    system("pause>0");
    ShowManageUsersScreen();
}
void ShowFindUserScreen()
{
    cout << "-------------------------------------------------------------------------\n";
    cout << "\t\tFind User Screen\n";
    cout << "-------------------------------------------------------------------------\n";
    vector<sUser>vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    sUser User;
    if (FindUserByUserName(UserName,vUsers,User))
    {
        PrintUserCard(User);
        cout << endl << endl;
    }
    else
    {
        cout << "No user found with user name:" << UserName << endl;
    }
}
sUser ChangeUserData(string UserName)
{
    sUser User;
    User.UserName = UserName;
    cout << "\nEnter Password?";
    getline(cin >> ws, User.Password);
    cout << "\nEnter Permissions?";
    User.Permissions = ReadUserPermissions();
    return User;
}
bool UpdateUserByUserName(string UserName,vector<sUser> vUsers,string UsersFileName)
{
    sUser User;
    char Sure;
    if (!FindUserByUserName(UserName, vUsers, User))
    {
        cout << "User with user name :" << UserName << " is not found!\n";
        return false;
    }
    cout << endl;
    PrintUserCard(User);
    cout << endl << endl;
    cout << "\n Are you sure you want to update this user?";
    cin >> Sure;
    if (Sure == 'y' || Sure == 'Y')
    {
        for (sUser& U : vUsers)
        {
            if (U.UserName == UserName)
            {
                U = ChangeUserData(UserName);
                break;
            }
        }
        SaveUsersVectorToFile(vUsers, UsersFileName);
        vUsers = LoadUsersDataFromFile(UsersFileName);
        cout << "\nUser updated successfully\n";
        return true;
    }
    return false;
}
void ShowUpdateUserScreen()
{
    cout << "-------------------------------------------------------------------------\n";
    cout << "\t\tUpdate User Screen\n";
    cout << "-------------------------------------------------------------------------\n";
    vector<sUser>vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    UpdateUserByUserName(UserName, vUsers, UsersFileName);
}
bool MarkUserForDelete(sUser User,vector<sUser>& vUsers)
{
    for (sUser& U : vUsers)
    {
        if (U.UserName == User.UserName)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
bool DeleteUserByUserName(string UserName,vector<sUser> vUsers,string UsersFileName)
{
    sUser User;
    char Sure;
    if (!FindUserByUserName(UserName, vUsers, User))
    {
        cout << "User with user name :" << UserName << " is not found!\n";
        return false;
    }
    cout << endl;
    PrintUserCard(User);
    cout << endl << endl;
    cout << "\n Are you sure you want to delete this user?";
    cin >> Sure;
    if (Sure == 'y' || Sure == 'Y')
    {
        MarkUserForDelete(User,vUsers);
        SaveUsersVectorToFile(vUsers,UsersFileName);
        vUsers = LoadUsersDataFromFile(UsersFileName);
        cout << "\nUser deleted successfully\n";
        return true;
    }
    return false;
}
void ShowDeleteUserScreen()
{
    cout << "-------------------------------------------------------------------------\n";
    cout << "\t\tDelete User Screen\n";
    cout << "-------------------------------------------------------------------------\n";
    vector<sUser>vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    DeleteUserByUserName(UserName,vUsers,UsersFileName);
}
void AddUserToFile(string UsersFileName,string Line)
{
    fstream File;
    File.open(UsersFileName,ios::out|ios::app);
    if (File.is_open())
    {
        File << Line << endl;
        File.close();
    }
}
bool UserExistsByUserName(string UserName)
{
    vector<sUser>vUsers = LoadUsersDataFromFile(UsersFileName);
    for (sUser& U : vUsers)
    {
        if (UserName == U.UserName)
            return true;
    }
    return false;
}
sUser ReadNewUser()
{
    sUser User;
    cout << "\nEnter User Name?";
    getline(cin >> ws, User.UserName);
    while (UserExistsByUserName(User.UserName))
    {
        cout << "\nThis user already exists.Enter another user name?";
        getline(cin >> ws, User.UserName);
    }
    cout << "\nEnter Password?";
    getline(cin , User.Password);
    User.Permissions = ReadUserPermissions();
    return User;
}
void AddNewUser()
{
    vector<sUser>vUsers = LoadUsersDataFromFile(UsersFileName);
    sUser User = ReadNewUser();
    AddUserToFile(UsersFileName,ConvertUserToLine(User));
}
void AddNewUsers()
{
    char AddMore;
    do
    {
        system("cls");
        cout << "\n\nAdding new user:\n";
        AddNewUser();
        cout << "User added successfully.Do you want to add more? Y/N ?";
        cin >> AddMore;
    } while (AddMore=='Y'||AddMore=='y');
}
void ShowAddNewUserScreen()
{
    cout << "-------------------------------------------------------------------------\n";
    cout << "\t\tAdd New User Screen\n";
    cout << "-------------------------------------------------------------------------\n";
    AddNewUsers();
}
void PerformManageUsersMenueOption(enManageUsersMenueOption option)
{
    switch (option)
    {
    case eListUsers:
        system("cls");
        ShowListUsers();
        GoBackToManageUsersScreen();
        break;
    case eAddNewUser:
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersScreen();
        break;
    case eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersScreen();
        break;
    case eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersScreen();
        break;
    case eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersScreen();
        break;
    case eMainMenue:
        ShowMainMenueScreen();
    }
}
void ShowManageUsersScreen()
{
    if (CheckAccessNotDenied(enUserPermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        return;
    }
    system("cls");
    cout << "========================================================================\n";
    cout << "\t\tManage Users Screen\n";
    cout << "========================================================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User Info.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    PerformManageUsersMenueOption((enManageUsersMenueOption)ReadManageUsersMenueOption());
}
void PerformMainMenueOption(enMainMenueOption option)
{
    switch (option)
    {
    case eShowClientList:
        system("cls");
        ShowClientListScreen();
        GoBackToMainMenueScreen();
        break;
    case eAddNewClient:
        system("cls");
        ShowAddNewClientScreen();
        GoBackToMainMenueScreen();
        break;
    case eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenueScreen();
        break;
    case eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenueScreen();
        break;
    case eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenueScreen();
        break;
    case eTransactions:
        system("cls");
        ShowTransactionsScreen();
        GoBackToMainMenueScreen();
        break;
    case eManageUsers:
        system("cls");
        ShowManageUsersScreen();
        break;
    case eLogout:
        Login();
        //ShowEndScreen();
    }
}
short ReadMainMenueOption()
{
    short option = 0;
    while (option > 8 || option < 1)
    {
        cout << "\nChoose what do you want to do? [1:8]?";
        cin >> option;
    }
    return option;
}
void ShowMainMenueScreen()
{
    system("cls");
    cout << "=================================================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "=================================================================\n";
    cout << "\t[1]Show Client List.\n";
    cout << "\t[2]Add New Client.\n";
    cout << "\t[3]Delete Client.\n";
    cout << "\t[4]Update Client Info.\n";
    cout << "\t[5]Find Client.\n";
    cout << "\t[6]Transactions.\n";
    cout << "\t[7]Manage Users.\n";
    cout << "\t[8]Logout.\n";
    cout << "=================================================================\n";
    PerformMainMenueOption((enMainMenueOption)ReadMainMenueOption());
}
bool FindUserByUserNameAndPassword(string UserName, string Password,sUser User)
{
    vector<sUser>vUsers = LoadUsersDataFromFile(UsersFileName);
    for (sUser& U : vUsers)
    {
        if (U.UserName == UserName && U.Password == Password)
        {
            User = U;
            return true;
        }
    }
    return false;
}
bool LoadUserInfoByNameAndPassword(string UserName,string Password)
{
    if (FindUserByUserNameAndPassword(UserName,Password,CurrentUser))
    {
        return true;
    }
    return false;
}
void Login()
{
    bool LoginFailed = false;
    string UserName, Password;
    do
    {
        
        system("cls");
        cout << "===================================================================\n";
        cout << "\t\tLogin Screen\n";
        cout << "===================================================================\n";
        if (LoginFailed)
        {
            cout << "\t\tInvalid User Name/Password!\n";
        }
        cout << "\nEnter User Name?";
        cin >> UserName;
        cout << "\nEnter Password?";
        cin >> Password;
        LoginFailed = !LoadUserInfoByNameAndPassword(UserName, Password);

    } while (LoginFailed);
    ShowMainMenueScreen();
}
int main()
{
    Login();
    //ShowMainMenueScreen();
    return 0;
}
