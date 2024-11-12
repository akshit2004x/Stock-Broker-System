#include<bits/stdc++.h>
using namespace std;
enum class UserCategory
{
    INDIVIDUAL,
    INSTITUTION
};
class Transaction 
{
    public:  
        int stockId;         
        bool isBuy;             
        double quantity;        
        double price;           
        double brokerCharge; 

        Transaction(int sid, bool buy, double qty, double p, double charge) 
        {
            stockId = sid;
            isBuy = buy;
            quantity = qty;
            price = p;
            brokerCharge = charge;
        }
};
class User 
{
    private:

        int userId;
        string username;
        UserCategory usercat;
        double balance;
        map<int,double> Stock_holds;
        vector<Transaction> transactions;

    public:

        User()
        {
            userId = 0;
            username = "";
            usercat = UserCategory::INDIVIDUAL;
            balance = 0.0;
        }
        User(int id,string name,UserCategory ucat, double bal)
        {
            userId = id;
            username = name;
            usercat = ucat;
            balance = bal;
            
        }
        
};

int main()
{
    return 0;
}