#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

enum enAtmMainMenueOption
{
    eQuickWithdraw = 1,
    eNormalWithdraw = 2,
    eDeposit = 3,
    eCheckBalance = 4,
    eLogout = 5
};

const string ClientsFileName = "Clients.txt";

sClient CurrentClient;

void Login();
void ShowAtmMainMenue();
void ShowQuickWithdrawMenue();
void ShowNormalWithdrawScreen();

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable
                  // use find() function to get the position of the delimiters
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length()); // erase() until positon and move to next word.
    }
    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }
    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]); // cast string to double
    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in); // read Mode
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

vector<sClient> LoadCleintsDataFromFile(string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient &Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

vector<sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); // overwrite
    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                // we only write records that are not marked for delete.
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

bool DepostAmountByAccountNumber(string AccountNumber, double Amount, vector<sClient> &vClients)
{

    char Answer = 'n';

    cout << "\n\nAre you sure you want perform this transation? y/n ? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        for (sClient &C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully, The New Balance is " << C.AccountBalance << endl;

                return true;
            }
        }
    }
    return false;
}

void GoBackToAtmMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowAtmMainMenue();
}

short ReadAtmMainMenueOption()
{
    short Choice = 0;
    do
    {
        cout << "Choose what do you want to do? [1 to 5]? ";
        cin >> Choice;
    } while (Choice < 0 || Choice > 5);

    return Choice;
}

short ReadQuickWithdrawOption()
{
    short Choice = 0;
    do
    {
        cout << "Choose what to Do from [1] to [9]? ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 9);

    return Choice;
}

int ReadNormalWithdrawAmount()
{
    int Amount = 0;

    do
    {
        cout << "\nEnter An Amount Multiple of 5's? ";
        cin >> Amount;
    } while ((Amount % 5) != 0);

    return Amount;
}

double ReadDepositAmount()
{
    double Amount = 0;

    do
    {
        cout << "\nEnter a Positive Deposit Amount? ";
        cin >> Amount;

    } while (Amount < 1);

    return Amount;
}

void ShowCheckBalanceScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tCheck Balance Screen";
    cout << "\n-----------------------------------\n";
    cout << "\nYour Balance is " << CurrentClient.AccountBalance << endl;
}

short GetQuickWithdrawAmount(short Option)
{

    switch (Option)
    {
    case 1:
        return 20;
        break;
    case 2:
        return 50;
        break;
    case 3:
        return 100;
        break;
    case 4:
        return 200;
        break;
    case 5:
        return 400;
        break;
    case 6:
        return 600;
        break;
    case 7:
        return 800;
        break;
    case 8:
        return 1000;
        break;
    }

    return 0;
}

void PerformQuickWithdrawOption(short Option)
{
    if (Option == 9)
        return;

    short QuickWithdrawAmount = GetQuickWithdrawAmount(Option);

    if (QuickWithdrawAmount > CurrentClient.AccountBalance)
    {
        cout << "\nThe Amount Exceeds Your balance, Make Another Choice " << endl;
        cout << "\nPress Any Key To Continue...\n";
        system("pause>0");
        ShowQuickWithdrawMenue();
        return;
    }

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    if (DepostAmountByAccountNumber(CurrentClient.AccountNumber, QuickWithdrawAmount * -1, vClients))
        CurrentClient.AccountBalance -= QuickWithdrawAmount;
}

void ShowQuickWithdrawMenue()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tQuick Withdraw";
    cout << "\n-----------------------------------\n";

    cout << "\t[1] 20\t[2] 50\n";
    cout << "\t[3] 100\t[4] 200\n";
    cout << "\t[5] 400\t[6] 600\n";
    cout << "\t[7] 800\t[8] 1000\n";
    cout << "\t[9] Exit";
    cout << "\n-----------------------------------\n";
    cout << "\nYour Balance is " << CurrentClient.AccountBalance << endl;

    PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

void PerformNormalWithdrawOption()
{
    int WithdrawAmount = ReadNormalWithdrawAmount();

    if (WithdrawAmount > CurrentClient.AccountBalance)
    {
        cout << "\nThe Amount Exceeds Your balance, Make Another Choice " << endl;
        cout << "\nPress Any Key To Continue...\n";
        system("pause>0");
        ShowNormalWithdrawScreen();
        return;
    }

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    if (DepostAmountByAccountNumber(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients))
        CurrentClient.AccountBalance -= WithdrawAmount;
}

void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tNormal Withdraw Screen";
    cout << "\n-----------------------------------\n";

    PerformNormalWithdrawOption();
}

void PerformDepositOption()
{
    double DepositAmount = ReadDepositAmount();

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    if (DepostAmountByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients))
        CurrentClient.AccountBalance += DepositAmount;
}

void ShowDepositScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    PerformDepositOption();
}

void PerfromAtmMainMenueOption(enAtmMainMenueOption MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enAtmMainMenueOption::eQuickWithdraw:
    {
        ShowQuickWithdrawMenue();
        GoBackToAtmMainMenue();
        break;
    }
    case enAtmMainMenueOption::eNormalWithdraw:
    {
        ShowNormalWithdrawScreen();
        GoBackToAtmMainMenue();
        break;
    }
    case enAtmMainMenueOption::eDeposit:
    {
        ShowDepositScreen();
        GoBackToAtmMainMenue();
        break;
    }
    case enAtmMainMenueOption::eCheckBalance:
    {
        ShowCheckBalanceScreen();
        GoBackToAtmMainMenue();
        break;
    }
    case enAtmMainMenueOption::eLogout:
    {
        Login();
        break;
    }
    }
}

void ShowAtmMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerfromAtmMainMenueOption((enAtmMainMenueOption)ReadAtmMainMenueOption());
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient &Client)
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    return FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient) ? true : false;
}

void Login()
{

    bool LoginFailed = false;
    string AccountNumber, PinCode;

    do
    {
        system("cls");
        cout << "\n-----------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n-----------------------------------\n";

        if (LoginFailed)
        {
            cout << "\nInvalid Account Number/PinCode!\n";
        }

        cout << "\nEnter Account Number? ";
        cin >> AccountNumber;
        cout << "Enter Pin Code? ";
        cin >> PinCode;

        LoginFailed = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFailed);

    ShowAtmMainMenue();
}

int main()
{
    Login();
    system("pause>0");
    return 0;
}