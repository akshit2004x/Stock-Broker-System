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
        bool deductAmount(double amount) 
        {
            if (balance >= amount) 
            {
                balance = balance - amount;
                return true;
            }
            else
            {
                return false;
            }
        }
        void addAmount(double amount) 
        {  
            balance = balance + amount;
        }
        bool updateStock_holds(int stockId, double quantity, bool isBuy) 
        {
            if(isBuy) 
            {
                Stock_holds[stockId] = Stock_holds[stockId] + quantity;  
                return true;
            } 
            else 
            {   
                if(Stock_holds[stockId] >= quantity) 
                {
                    Stock_holds[stockId] = Stock_holds[stockId] - quantity;  
                    return true;
                }
                return false;  
            }
        }
        void addTransaction(Transaction trans)
        {
            transactions.push_back(trans);
        }
        vector<Transaction> getTransactions() 
        { 
            return transactions;
        }
        double getBalance() 
        {  
            return balance;
        }
        
};
class Stock 
{
    private:
        int stockId;
        double price;
        double brokerage;
        int quantity;

    public:
        Stock()
        {
            stockId = 0;
            price = 0.0;
            brokerage = 0.0;
            quantity = 0;
        }
        Stock(int id, double p,int broker,int qua)
        {
            stockId = id;
            price = p;
            brokerage = broker;
            quantity = qua;
        }
        double getPrice()  
        {   
            return price; 
        }

        double getbrokerage()
        {
            return brokerage;
        }

        bool deductstock(int preq)
        {
            if(quantity >= preq)
            {
                quantity = quantity - preq;
                return true;
            }
            else
            {
                return false;
            }
        }
        void addstock(int qa)
        {
            quantity = quantity + qa;
        }
        
};
class StockBroker 
{
    private:
        double broker_rate;
        map<int, User> users;  
        map<int, Stock> stocks; 

    public:

        StockBroker(double bob)
        {
            broker_rate = bob;
        }
    
        bool registerUser(int userId,string name, UserCategory type, double balance = 0.0) 
        {
            if(users.find(userId) != users.end()) 
                return false;
            else
            {
                users[userId] = User(userId,name,type,balance);
                return true; 
            }
        }
        bool unregisterUser(int userId) 
        {
            return users.erase(userId); 
        }
        bool registerStock(int stockId, double price,int brok,int quan) 
        {
            if(stocks.find(stockId) != stocks.end())
            {
                return false;
            }
            else
            {
                stocks[stockId] = Stock(stockId, price,brok,quan);
                return true;
            }
        }
        bool unregisterStock(int stockId) 
        {
            return stocks.erase(stockId);
        }
        bool buyStock(int userId, int stockId, double quantity) 
        {
            if (users.find(userId) == users.end() || stocks.find(stockId) == stocks.end() || quantity <= 0) 
                return false; 

            User& uobj = users[userId];      
            Stock& sobj = stocks[stockId];  

            
            double totalCost = sobj.getPrice() * quantity;
            double brokerage = (totalCost * sobj.getbrokerage()) / 100.0;
            double totalAmount = totalCost + brokerage;

           if(sobj.deductstock(quantity) && uobj.deductAmount(totalAmount)) {
            uobj.updateStock_holds(stockId, quantity, true);
            uobj.addTransaction(Transaction(stockId, true, quantity, sobj.getPrice(), brokerage));
            cout << "Transaction successful. Total cost: " << totalAmount << endl;
            return true;
            }
            return false;
        }

        
        
};


int main()
{
    return 0;
}