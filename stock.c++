#include<bits/stdc++.h>
#include<optional>

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
    public:
        vector<Transaction> transactions;
        map<int,double> Stock_holds;

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
        const vector<Transaction>& getTransactions() const 
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
        optional<double> brokerage;
        int quantity;

    public:
        Stock()
        {
            stockId = 0;
            price = 0.0;
            brokerage = 0.0;
            quantity = 0;
        }
        Stock(int id, double p,optional<double> broker,int qua)
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

        double getbrokerage(double d=1.0)
        {
            return brokerage.value_or(d);;
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
        double broker_rate = 1;
        map<int, User> users;  
        map<int, Stock> stocks; 

    public:
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
        bool registerStock(int stockId, double price,optional<double> brok,int quan) 
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
            if(users.find(userId) == users.end() || stocks.find(stockId) == stocks.end() || quantity <= 0) 
                return false; 

            User& uobj = users[userId];      
            Stock& sobj = stocks[stockId];  
            
            double totalCost = sobj.getPrice() * quantity;
            double brokerage = (totalCost * sobj.getbrokerage()) / 100.0;
            double totalAmount = totalCost + brokerage;

            if(sobj.deductstock(quantity) && uobj.deductAmount(totalAmount)) 
            {
                uobj.updateStock_holds(stockId, quantity, true);
                uobj.addTransaction(Transaction(stockId, true, quantity, sobj.getPrice(), brokerage));
                cout<<"PURCHASING STOCK IS SUCCESSFUL"<<endl;
                cout << "Transaction successful , Total cost: " << totalAmount << endl;
                return true;
            }
            return false;
        }
        bool sellStock(int userId, int stockId, double quantity) 
        {
            if (users.find(userId) == users.end() || stocks.find(stockId) == stocks.end() || quantity <= 0)
            return false;

            User& uobj = users[userId];
            Stock& sobj = stocks[stockId];

            if (uobj.updateStock_holds(stockId, quantity, false)) 
            {
                double totalIncome = sobj.getPrice() * quantity;
                double brokerage = (totalIncome * sobj.getbrokerage()) / 100.0;
                double netIncome = totalIncome - brokerage;

                sobj.addstock(quantity);
                uobj.addAmount(netIncome);
                uobj.addTransaction(Transaction(stockId, false, quantity,sobj.getPrice(), brokerage));
                cout<<"SELLING STOCK IS SUCCESSFUL"<<endl;
                cout <<"Transaction successful , Net income: " << netIncome << endl;
                return true;
            }
            return false;
        }
        double balanceleft(int userId)
        {
            if(users.find(userId) == users.end()) return 0.0; 

            return users[userId].getBalance();
        }
        double getBrokerageCharges(int userId) 
        {   
            double total = 0.0;
            for (const auto& trans : users[userId].getTransactions()) 
            {
                total = total + trans.brokerCharge; 
            }
            return total;
        }
        void Display_stock_holds(int userId)
        {
            cout<<"STOCKS THE USER IS HOLDING"<<endl;
            for(const auto& i:users[userId].Stock_holds)
            {
                cout<<i.first<<" - "<<i.second<<endl;
            }
        }
        void Transaction_history_retrieval(int userId)
        {
            cout<<"TRANSACTIONS DONE TILL NOW"<<endl<<endl;
            for(const auto& transaction : users[userId].getTransactions())
            {
                
                cout << "Stock ID :" << transaction.stockId << endl;
                cout << "Transaction Type : " << (transaction.isBuy ? "Buy" : "Sell") << endl;
                cout << "Quantity : " << transaction.quantity << endl;
                cout << "Price : " << transaction.price << endl;
                cout << "Broker Charge : " << transaction.brokerCharge << endl;
                cout<<endl<<endl;
            }

        }
};
int main()
{
    StockBroker Broker_1;
    
    // Must have Functionalities 

        // REGISTER USER
            Broker_1.registerUser(1,"Akshit",UserCategory::INDIVIDUAL,10000.0);
            Broker_1.registerUser(2,"Pratham",UserCategory::INDIVIDUAL,10000.0);
            Broker_1.registerUser(3,"Nishkarsh",UserCategory::INDIVIDUAL,10000.0);
            Broker_1.registerUser(4,"Shlok",UserCategory::INDIVIDUAL,10000.0);

        // REGISTER STOCK
            Broker_1.registerStock(101,100.0,nullopt,50);
            Broker_1.registerStock(102,200.0,1,70);
            Broker_1.registerStock(103,1000.0,5,30);
            Broker_1.registerStock(104,400.0,2,100);

        //  UNREGISTER USER
            Broker_1.unregisterUser(2);

        //  UNREGISTER STOCK
            Broker_1.unregisterStock(103);
            
        //  BUY STOCK
            Broker_1.buyStock(1,101,8);
            Broker_1.buyStock(1,102,10);
            Broker_1.buyStock(1,104,2);

        // SELL STOCK
            Broker_1.sellStock(1, 101, 5);
        
        //  STOCK UPHOLDS
            cout<<endl;
            Broker_1.Display_stock_holds(1);


        // NICE TO HAVE FUNCTIONALITIES
    
        // Transaction History Retrieval
        Broker_1.Transaction_history_retrieval(1);

        // Balance Inquiry
        cout<<Broker_1.getBrokerageCharges(1);

        //
        
    return 0;
}