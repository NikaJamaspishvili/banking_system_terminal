//create the account (id,name,age,national_id,email,password), if the user is registered already let him/her login with email and password.
    //during the login check if the email and password are correct

//after the succesfull registration, user will be granted witht the bank account number and will be able to:
    //deposit money, or withdraw the money, or send it to someone.


#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

class Account{
    protected:
    string name;
    int age;
    string nationalID;
    string email;
    string password;
    double balance = 0;

    public:
    bool Signup(){
        cout<<endl<<"---SignUp---"<<endl;
        cout<<"Enter Your name: "<<endl;
        cin>>name;
        cout<<"Enter Your age: "<<endl;
        cin>>age;
        cout<<"Enter Your nationalID: "<<endl;
        cin>>nationalID;
        cin.ignore();
        cout<<"Enter Your email: "<<endl;
        cin>>email;
        cout<<"Enter Your password (no spaces): "<<endl;
        cin>>password;

        //now we insert these info in the accounts.txt file.
        ofstream writeFile("accounts.txt",ios::app);

        if(writeFile.is_open()){
            writeFile <<"nationalID:"<<nationalID<<" balance:"<<balance<<" name:"<<name<<" age:"<<age<<" email:"<<email<<" password:"<<password<<endl;
            writeFile.close();
            cout<<"✅You have signedup succesfully"<<endl<<endl;
            return true;
        }else{
            cout<<"💥something went wrong, please refresh and try again💥"<<endl;
            return false;
        }
    }
    
    bool Login(){
            cout<<"---Login---"<<endl;
            cout<<"insert email: "<<endl;
            cin>>email;
            cout<<"insert password: "<<endl;
            cin>>password;

            ifstream readFile("accounts.txt");
            string line;

            while(getline(readFile,line)){
                //extract the email and password from each of them and see if they are equal
                int emailPos = line.find("email:");
                int passwordPos = line.find("password:");

                if(emailPos != string::npos && passwordPos != string::npos){
                    string email = line.substr(emailPos + 6,passwordPos - (emailPos+6) - 1);
                    string password = line.substr(passwordPos+9);

                    // cout<<"Email: "<<email<<endl;
                    // cout<<"Password: "<<password<<endl;
                    if(this->email == email && this->password == password){
                        nationalID = line.substr(11,line.find("balance") - 12);
                        name = line.substr(line.find("name")+5,line.find("age")-(line.find("name")+5) -1);
                        cout<<"✅ you have logged in succesfully"<<endl<<endl;
                        return true;
                        // cout<<"name is: "<<name<<endl;
                        // cout<<"id is: "<<nationalID<<endl;
                    }
                }else{
                    cout<<"something went wrong, please contact support"<<endl;
                    return false;
                }
            }
            cout << "❌ Credentials are not correct" << endl;
            return false;
    }

    bool showActions(){
    int action;
    bool isRegistered;

    cout<<"---BANK SYSTEM---"<<endl;
    cout<<"1. Signup"<<endl;
    cout<<"2. Login"<<endl;
    cin>>action;

    if(action == 2){
        isRegistered = Login();
    }else if(action == 1){
        isRegistered = Signup();
    }

    return isRegistered;
    };

};


class AccountActions : public Account{
    public:
    void afterRegisterActions(){
        int action;
        cout<<"---Take Action---"<<endl;
        cout<<"1. Withdraw Money"<<endl;
        cout<<"2. Deposit Money"<<endl;
        cout<<"3. View Balance"<<endl;
        cout<<"4. Transfer Money"<<endl;
        cin>>action;

        if(action == 1){
            Deposit_Withdraw("Withdraw");
        }else if(action == 2){
            Deposit_Withdraw("Deposit");
        }else if(action == 3){
            ViewBalance();
        }else if(action == 4){
            TransferMoney();
        }
    };


    void Deposit_Withdraw(string action){
        ifstream readFile("accounts.txt");
        ofstream writeFile("temp.txt");

        int money = 0;
        cout<<"How Much Money To "<<action<<"?"<<endl;
        cin>>money;

        if(readFile.is_open()){
            int balancePos;
            string line;
            while(getline(readFile,line)){
                //first of all find the line we want to modify with nationalID
                //modify that line and update the balance, use the exact string but with updated balance
                //put this new line in different file
                //put all lines in different file
                //delete the current file

                string nationalID = line.substr(line.find("nationalID:")+11,line.find("balance:") - 12);
               // cout<<this->nationalID<<" woow "<<nationalID<<endl;
                if(this->nationalID == nationalID){
                    string balance = line.substr((line.find("balance:")+8),(line.find("name:")-1)-(line.find("balance:")+8));
                  //  cout<<"balance is: "<<balance<<endl;
                  if(action == "Deposit"){
                    line.replace((line.find("balance:")+8),(line.find("name:")-1)-(line.find("balance:")+8),to_string(stoi(balance) + money));
                  }else if(action == "Withdraw"){
                    if(stoi(balance) < money){
                        cout<<"❌ Balance is not enough - "<<balance<<" in total"<<endl<<endl;
                        return;
                    }
                    line.replace((line.find("balance:")+8),(line.find("name:")-1)-(line.find("balance:")+8),to_string(stoi(balance) - money));
                  }
                }
                writeFile<<line<<endl;
            }

           int status = remove("accounts.txt");
           if(status == 1){
            cout<<"Something went wrong, please try again."<<endl;
           }else{
            rename("temp.txt","accounts.txt");
            if(action == "Deposit"){
                cout<<"✅ Succesfully deposited "<<to_string(money)<<" Gel to your bank account"<<endl<<endl;
            }else if(action == "Withdraw"){
                cout<<"✅ Succesfully withrawed "<<to_string(money)<<" Gel from your bank account"<<endl<<endl;
            }
           }

        }else{
            cout<<"💥something went wrong, please refresh and try again💥"<<endl;
        }
    }

    void TransferMoney(){
        int balance = ViewBalance();
        string recipient;
        int amount;
        bool foundRecipient = false;

        cout<<"Who Do You Want To Transfer Money? (NationalID)"<<endl;
        cin>>recipient;
        cout<<"How Much?"<<endl;
        cin>>amount;

        if(amount > balance){
            cout<<"❌Not enought money on account"<<endl<<endl;
            return;
        }

        ifstream readFile("accounts.txt");
        ofstream writeFile("temp.txt");

        string line;
        while(getline(readFile,line)){
            string nationalID = line.substr(line.find("nationalID:")+11,line.find("balance:") - 12);
            if(recipient == nationalID){
                foundRecipient = true;
                string recipient_balance = line.substr((line.find("balance:")+8),(line.find("name:")-1)-(line.find("balance:")+8));
                line.replace((line.find("balance:")+8),(line.find("name:")-1)-(line.find("balance:")+8),to_string(stoi(recipient_balance) + amount));
                writeFile <<line<<endl;
            }
        }
        readFile.close();

        readFile.open("accounts.txt");

        if(!foundRecipient){
            cout<<"❌Couldn't find the recipient with same number"<<endl<<endl;
        }else{

        if(readFile.is_open()){
        while(getline(readFile,line)){
            string nationalID = line.substr(line.find("nationalID:")+11,line.find("balance:") - 12);
            if(this->nationalID == nationalID){
                line.replace((line.find("balance:")+8),(line.find("name:")-1)-(line.find("balance:")+8),to_string(balance - amount));
            }
            if(nationalID != recipient) writeFile<<line<<endl;
        }
        }

        int status = remove("accounts.txt");
        if(status == 1){
            cout<<"Something went wrong."<<endl<<endl;
        }else{
            rename("temp.txt","accounts.txt");
            cout<<"✅Transfer completed succesfully"<<endl<<endl;
        }

        }

        }

    int ViewBalance(){
        ifstream readFile("accounts.txt");
        string line;
        while(getline(readFile,line)){
            string nationalID = line.substr(line.find("nationalID:")+11,line.find("balance:") - 12);
            // cout<<this->nationalID<<" woow "<<nationalID<<endl;
            if(this->nationalID == nationalID){
            string balance = line.substr((line.find("balance:")+8),(line.find("name:")-1)-(line.find("balance:")+8));
            cout<<"✅ Your Balance Is: "<<balance<<" Gel"<<endl<<endl;
            return stoi(balance);
            }
        }
        return 0;

        cout<<"We couldn't find information about you, please try again."<<endl<<endl;

    }
};


int main(){
    AccountActions user;

    //the start
    bool isRegistered = user.showActions();

    while (isRegistered == 0){
        isRegistered = user.showActions();
    }

    while(isRegistered == 1){
        user.afterRegisterActions();
    }

    // if(isRegistered == 1){
    //     user.afterRegisterActions();
    // }else if(isRegistered == 0){
    //     isRegistered = user.showActions();
    // }

    return 0;
}