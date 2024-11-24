#include<bits/stdc++.h>
#include<optional>
using namespace std;
enum class UserCategory   // created a user define data type that can take only limited values
{
    INDIVIDUAL,
    INSTITUTION
};
class Transaction   // created a class to represent a single transaction.
{
    public:  
        // store stockid , whether it is sell or purchase , no of stocks user has purchased or sold , price of stock , brokerage charge of that transaction  
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

class Stock_observer   
{
    // create a stock_observer class that will notify users when price changes
    public :
        virtual void update_stock_Price(int sid,double oprice,double nprice) = 0;   // i am implementing a pure virtual function , so that i have to define it later in derived class 
};

class User : public Stock_observer
{
    private:
        int userId;   // so that we can uniquely identify data 
        string username;  
        double balance;  
        UserCategory usercat;
    public:
        vector<Transaction> transactions;  // we can store no of transactions performed by user
        map<int,double> Stock_holds; // we can create a list of no of stocks purchased by user 

        User()  
        {
            userId = 0;
            username = "";
            usercat = UserCategory::INDIVIDUAL;
            balance = 0.0;
        }
        User(int id,string name,UserCategory ucat, double bal)  // created a parametrized constructor to assign value to variables
        {
            userId = id;
            username = name;
            usercat = ucat;
            balance = bal;
        }


        // created some getter and setter
        string getName()
        {
            return username;
        }
        double getBalance() 
        {  
            return balance;
        }
        UserCategory getUserCategory()  
        {
            return usercat;
        }

        
        // created a deduct amount function so that at time of buy stock we ccan refer to this function for deducting amount
        bool deductAmount(double amount) 
        {
            //i have created a balance notification , whenever the user holds balnce less than 100 then user will be notified
            if(amount <= 100)
            {
                cout<<"DEAR USER YOUR BALANCE IS QUITE LOW PLEASE DEPOSIT"<<endl;
            }

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
        bool updateStock_holds(int stockId, double quantity, bool isBuy)  // if i am buying a stock or selling a stock i have to keep updating the stocks user is holding simultaneously 
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
            }
            return false;
        }

        void update_stock_Price(int sid,double oprice,double nprice) override
        {
            // i am sending notification to the user that the particular stock price is changed
            if(Stock_holds.find(sid)!=Stock_holds.end()  && Stock_holds[sid] >=0 )
            {

                double shares = Stock_holds[sid];
                double total = nprice * shares;

                cout<<endl<<endl;
                cout<<"Notification"<<endl; 
                cout<<"stock updated for user id = "<<userId<<" username = "<<username<<endl;
                cout<<"Price Changes from "<<oprice<<" this to "<<nprice<<endl;
                cout<<"No of shares user is holding is "<<shares<<endl;
                cout<<"Total Previous charge is "<<oprice*shares<<endl; 
                cout<<"Total value is now "<<total<<endl<<endl;

            } 
        }


        void addTransaction(const Transaction &trans) // storing a transaction into a vector of transactions 
        {
            transactions.push_back(trans);
        }
        const vector<Transaction> getTransactions()   //getter
        {   
            return transactions;
        }

        
};

class stock_subject
{
    public:
        virtual void add_stock_user(int id,int quan,User& u) = 0; // if a new user buys stocks it should be added in stock users
        virtual void erase_stock_user(int id,int quan) = 0; // if someone sells stocks its quantity should be reduced 
        virtual void notify_user(double oprice, double nprice) = 0; // send notification to all users
};

class Stock : public stock_subject       // class to represent a stock
{
    private:
        int stockId; // uniquely identify stock
        double price; // price of stock
        optional<double> brokerage;  // brokerage charge associated per stock
        int quantity;  // quantity of stock

        vector<pair<int,int>> stock_users; // pair -> user id , pair - > quantity user is holding
        map<int,User> users;

    public:
        Stock()
        {
            stockId = 0;
            price = 0.0;
            brokerage = 0.0;
            quantity = 0;
        }
        Stock(int id, double p,optional<double> broker,int qua)  // created a parameterized constructor that initializes stock with variables
        {
            stockId = id;
            price = p;
            brokerage = broker; 
            quantity = qua;
        }


        // getter and setter
        double getPrice()  
        {   
            return price; 
        }
        void setPrice(double np)
        {
            price = np;
        }
        double getquantity()
        {
            return quantity;
        }
        void setquantity(int a)
        {
            quantity = a; 
        }
        double getbrokerage(double d=1.0)  // we are geeting brokerage and if the brokerage value is null we assign 1% as brokerage value;
        {
            return brokerage.value_or(d);
        }

        bool deductstock(int preq) // whenever i buy a stock i have to use this function to deduct the no of stocks present
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
        void addstock(int qa) // whenever i sell a stock i use this function to add no of stocks present 
        {
            quantity = quantity + qa;
        }

        void add_stock_user(int userid, int quan,User& uobj) override 
        {
            // i will first find the userid in the pair 
            auto i = find_if(stock_users.begin(), stock_users.end(),[userid](const pair<int,int>& user) { return user.first == userid; });
            // if i found the user not registerd with that stock then i will create a pair and push otherwise i will just increase the quantity
            if(i==stock_users.end()) 
            {
                stock_users.push_back({userid, quan});
                users[userid] = uobj;
            }
            else
            {
                i->second = i->second + quan; 
            }
        }

        void erase_stock_user(int userid,int quan) override 
        {
            // i will first find the userid in the pair 
            auto i = find_if(stock_users.begin(), stock_users.end(),[userid](const pair<int, int>& user) { return user.first == userid; });
            // if i found the userid then only i will perform
            if(i != stock_users.end()) 
            {
                // if my quantity of stock becomes 0 then i will erase otherwise i will just decrease the quantity
                int q = i->second;
                if(q - quan == 0)
                {
                    stock_users.erase(i);
                    users.erase(userid);
                }
                else
                {
                    i->second = q - quan;  
                }
                
            }
        }   

        void  notify_user(double oprice, double nprice) override 
        {
            // i will notify all the users who are associated withthat stock
            for(auto i : stock_users) 
            {
                int userid = i.first;
                if(users.find(userid) != users.end())
                {
                    User& uobj = users[userid];
                    uobj.update_stock_Price(stockId,oprice,nprice);
                }
            }
        }
        
};
class StockBroker  // this is the main class that handles all the fucntions 
{ 
    private:
        map<int, User> users;   // it stores all the users 
        map<int, Stock> stocks; // it stores all the stocks

    public:
        bool registerUser(int userId,string name, UserCategory type, double balance = 0.0) // creating a new user 
        {
            if(users.find(userId) != users.end())  // finding user in the list of users if already present returning false;
                return false;
            else
            {
                users[userId] = User(userId,name,type,balance);  // storing a new user in the list of users , with primary key of userid
                return true; 
            }
        }
        bool unregisterUser(int userId)  // deleting a user from the list of users
        {
            return users.erase(userId); 
        }
        bool registerStock(int stockId, double price,optional<double> brok,int quan) // creating a new stock
        {
            if(stocks.find(stockId) != stocks.end())  // finding stock int the list of stocks  if it's already there return false
            {
                return false;
            }
            else
            {
                stocks[stockId] = Stock(stockId, price,brok,quan); // storing a new stock in the list of stocks , with primary key as stockid
                return true;
            }
        }
        bool unregisterStock(int stockId) // deleting a stock from the list of stocks
        {
            return stocks.erase(stockId);
        }
        bool buyStock(int userId, int stockId, double quantity) // For buying a stock
        {
            // i have made certian base condition 1 . if userid is not present
            //                                    2.  if stockid is not present
            //                                    3.  if quantity is less than or equal to zero
            if(users.find(userId) == users.end() || stocks.find(stockId) == stocks.end() || quantity <= 0)  
                return false; 

            User& uobj = users[userId];        // creating a object of user
            Stock& sobj = stocks[stockId];     // creating a object of stock
            
            double totalCost = sobj.getPrice() * quantity;    // calculating a cost my multiplying stock price and quantity demanded by user      
            double brokerage = (totalCost * sobj.getbrokerage()) / 100.0;  // calculating brokerge charge
            double totalAmount = totalCost + brokerage; 

            // deductstock - basically it checks whether the stocks are available or not , if yes then it will also deduct the quantity
            // deductamount - basically it checks whether the user is having the balance to buy the stock or not , if yes the totalamount of stocks with brokerage will be deducted 

            if(sobj.deductstock(quantity) && uobj.deductAmount(totalAmount)) 
            {
                uobj.updateStock_holds(stockId, quantity, true);  // updating the list of stocks the user is holding
                uobj.addTransaction(Transaction(stockId, true, quantity, sobj.getPrice(), brokerage));  // adding transactions to vector per user
                sobj.add_stock_user(userId,quantity,uobj);  // update the list of users who are holding that stock
                cout<<"PURCHASING STOCK IS SUCCESSFUL"<<endl;
                cout << "Transaction successful , Total cost: " << totalAmount << endl;
                return true;
            }
            return false;
        }
        bool sellStock(int userId, int stockId, double quantity) 
        {
            // i have made certian base condition 1 . if userid is not present
            //                                    2.  if stockid is not present
            //                                    3.  if quantity is less than or equal to zero
            if (users.find(userId) == users.end() || stocks.find(stockId) == stocks.end() || quantity <= 0)
                return false;

            User& uobj = users[userId];             // creating a object of user
            Stock& sobj = stocks[stockId];          // creating a object of stock

            if (uobj.updateStock_holds(stockId, quantity, false)) 
            {
                double totalIncome = sobj.getPrice() * quantity;   // calculating a cost my multiplying stock price and quantity demanded by user 
                double brokerage = (totalIncome * sobj.getbrokerage()) / 100.0;  // calculating brokerage
                double netIncome = totalIncome - brokerage;

                // addstock - basically it add the quantity of stocks
                // addamount - basically it add the amount to the user account
                // addTran - it is used to add transactions to vector per user
                // eraseTran - it is used to erase or decrease the quantity of stocks from list of user who are holding that stock 
                sobj.addstock(quantity);
                uobj.addAmount(netIncome);
                uobj.addTransaction(Transaction(stockId, false, quantity,sobj.getPrice(), brokerage));
                sobj.erase_stock_user(userId,quantity);
                cout<<"SELLING STOCK IS SUCCESSFUL"<<endl;
                cout <<"Transaction successful , Net income: " << netIncome << endl;
                return true;
            }
            return false;
        }
        void update_stock_price(int stockId,int new_price)
        {
            Stock& sobj = stocks[stockId];   
            double ab = sobj.getPrice();
            // storing the old price in a variable
            sobj.setPrice(new_price);
            // updating stock price by setter
            sobj.notify_user(ab, new_price);
            // notify the users who are wholding these stocks

            cout<<endl<<"UPDATION IS SUCCESSFULL"<<endl;
        }
        double balanceleft(int userId)
        {
            return users[userId].getBalance(); // it is used to get the remaining balance 
        }
        double getBrokerageCharges(int userId) 
        {   
            // it is used to the get the brokerage charge given by user till now
            double total = 0.0;
            for (const auto& trans : users[userId].getTransactions()) 
            {
                total = total + trans.brokerCharge; 
            }
            return total;
        }
        void Display_stock_holds(int userId)
        {
            // it is used to display the stocks the user is holding
            cout<<"STOCKS THE USER IS HOLDING"<<endl;
            for(const auto& i:users[userId].Stock_holds)
            {
                cout<<i.first<<" - "<<i.second<<endl;
            }
        }
        void Transaction_history_retrieval(int userId)
        {
            // it is used to print all the transactions done by user whether it is buy or sell
            cout<<"TRANSACTIONS DONE TILL NOW"<<endl<<endl;
            for(const auto& transaction : users[userId].getTransactions())
            {       
                cout<<"Stock ID:"<<transaction.stockId<<endl;
                cout<<"Transaction Type:"<<(transaction.isBuy ? "Buy" : "Sell")<<endl;
                cout<<"Quantity:"<<transaction.quantity<<endl;
                cout<<"Price:"<<transaction.price<<endl;
                cout<<"Broker Charge:"<<transaction.brokerCharge<<endl;
                cout<<endl<<endl;
            }

        }
        void display_all_stock()
        {
            // it is used to display all the stocks in which user can invest 
            for(auto i:stocks)
            {
                cout<<"Stock No = "<<i.first<<" , Price - "<<i.second.getPrice()<<" ,Quantity = "<<i.second.getquantity()<<endl;
            }
        }

        void display_all_user()
        {
            // it is used to display all the registered users
            for(auto i:users)
            {
                cout<<"User No = "<<i.first<<" , User Name ="<<i.second.getName()<<" , User Balance = "<<i.second.getBalance()<<endl;
            }
        }

        void display_particular_stock(int stockId)
        {
            // it is used to display the particular stock details
            if (stocks.find(stockId) == stocks.end()) // if stock is not present it will return  
            {
                cout<<"Stock ID "<<stockId<<" not found."<<endl;
                return;
            }

            Stock& stock = stocks[stockId];
            cout<<"Stock ID: "<<stockId<<", Price: "<<stock.getPrice()<<", Quantity: "<<stock.getquantity()<<", Brokerage: "<<stock.getbrokerage()<<"%"<< endl;
        }

        void display_particular_user(int userId)
        {
            // it is used to display particular user details
            if(users.find(userId) == users.end()) // if user is not present it will return 
            {
                cout<<"User ID "<<userId<<" not found."<<endl;
                return;
            }

            User& user = users[userId];
            cout<<"User ID: "<<userId<< ", Name: "<<user.getName()<<", Balance: "<<user.getBalance()<<", User Category: "<<(user.getUserCategory() == UserCategory::INDIVIDUAL ? "INDIVIDUAL" : "INSTITUTIONAL")<<" , User Balance = "<<user.getBalance()<<endl; 
        }   

};
int main()
{
    StockBroker Broker_1;
    
    //  I have already registered some stocks and users  

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

            // I have created user input based program 

            while (true) 
            {
                cout<<endl;
                cout<<"Stock Management System"<<endl;
                cout<<"1.Register User"<<endl;
                cout<<"2.Register Stock"<<endl;
                cout<<"3.Unregister User"<<endl;
                cout<<"4.Unregister Stock"<<endl;
                cout<<"5.Buy Stock"<<endl;
                cout<<"6.Sell Stock"<<endl;
                cout<<"7.Update Stock"<<endl;
                cout<<"8.Transaction history of a particular user"<<endl;
                cout<<"9.Remaining balance of a particular user"<<endl;
                cout<<"10.Total Brokerage charges given by a particular user"<<endl;
                cout<<"11.Display all Stocks Listed"<<endl;
                cout<<"12.Display all Users Listed"<<endl;
                cout<<"13.Display a particular stock"<<endl;
                cout<<"14.Display a particular user"<<endl;
                cout<<"15.Exit"<<endl;

                int ch;
                cout<<"Enter a No to choose: ";
                cin>>ch;

                switch(ch) 
                {
                    case 1:
                    {
                        cout<<"Registering User"<<endl;

                        int userid;
                        cout<<"Enter User ID:";
                        cin>>userid;

                        string uname;
                        cout<<"Enter Firstname:";
                        cin>>uname;

                        int category;
                        cout<<"Enter User Category (0 for INDIVIDUAL, 1 for INSTITUTION):";
                        cin>>category;

                        double balance;
                        cout<<"Enter Initial Balance:";
                        cin>>balance;

                        UserCategory usercategory = (category == 1) ? UserCategory::INSTITUTION : UserCategory::INDIVIDUAL;

                        if(Broker_1.registerUser(userid,uname,usercategory,balance)) 
                        {
                            cout<<"User registered successfully"<<endl;
                        } 
                        else 
                        {
                            cout<<"User registration failed"<<endl;
                        }
                        break;

                    }
                    case 2:
                    {
                        cout<<"Registering Stock"<<endl;

                        int stockid, quantity;
                        double price;
                        optional<double> brokerage;
                        double rate;

                        cout<<"Enter Stock ID: ";
                        cin>>stockid;
                        cout<<"Enter Stock Price: ";
                        cin>>price;
                        cout<<"Enter Stock Quantity: ";
                        cin>>quantity;

                        cout<<"Enter Brokerage Percentage( enter -1 for no brokerage charge ):";
                        cin>>rate;

                        if(rate == -1) 
                            brokerage = nullopt;
                        else 
                            brokerage = rate;


                        if(Broker_1.registerStock(stockid,price,brokerage,quantity)) 
                        {
                            cout<<"Stock registered successfully"<<endl;
                        }
                        else 
                        {
                            cout<<"Failed to register stock"<<endl;
                        }

                        break;

                    }
                    case 3:
                    {
                        cout<<"Unregistering User"<<endl;

                        int userId;
                        cout<<"Enter User ID to unregister:";
                        cin>>userId;

                        if(Broker_1.unregisterUser(userId)) 
                        {
                            cout<<"User unregistered successfully"<<endl;
                        } 
                        else 
                        {
                            cout<<"Failed to unregister user"<<endl;
                        }

                        break;

                    }       
                    case 4:
                    {
                        cout<< "Unregistering Stock"<<endl;

                        int stockId;
                        cout<<"Enter Stock ID to unregister:";
                        cin>>stockId;

                        if(Broker_1.unregisterStock(stockId)) 
                        {
                            cout<<"Stock unregistered successfully"<<endl;
                        } 
                        else 
                        {
                            cout<<"Failed to unregister stock"<<endl;
                        }

                        break;
                    }
                    case 5:
                    {

                        cout<<"Displaying Stocks"<<endl;
                        Broker_1.display_all_stock();
                        cout<<endl<<"Buying Stock"<<endl;

                        int stockid, userid, quantity;

                        cout<<"Enter UserID:";
                        cin>>userid;
                        cout<<"Enter StockID:";
                        cin>>stockid;
                        cout<<"Enter Quantity: ";
                        cin>>quantity;

                        Broker_1.buyStock(userid,stockid,quantity);

                        break;
                    }
                    case 6:
                    {
                        cout<<"Displaying Stocks"<<endl;
                        Broker_1.display_all_stock();
                        cout<<endl<<"Selling Stock"<<endl;

                        int stockid,userid,quantity;

                        cout<<"Enter UserID:";
                        cin>>userid;
                        cout<<"Enter StockID:";
                        cin>>stockid;
                        cout<<"Enter Quantity:";
                        cin>>quantity;

                        Broker_1.sellStock(userid, stockid, quantity);

                        break;
                    }
                    case 7:
                    {
                        cout<<"Update Stock"<<endl;

                        int stockid,nprice,quantity;

                        cout<<"Enter StockID:";
                        cin>>stockid;
                        cout<<"Enter new Stock Price:";
                        cin>>nprice;

                        Broker_1.update_stock_price(stockid, nprice);
                
                        break;
                    }
                    case 8:
                    {
                        cout<<"Transaction history retrieval"<<endl;

                        int userid;
                        cout<<"Enter User ID: ";    
                        cin>>userid;

                        Broker_1.Transaction_history_retrieval(userid);

                        break;
                    }
                    case 9:
                    {
                        cout<<"Remaining Balance of a particular User"<<endl;

                        int userid;
                        cout<<"Enter User ID:";
                        cin>>userid;

                        cout<<"Remaining Balance :"<<Broker_1.balanceleft(userid)<<endl;

                        break;
                    }
                    case 10:
                    {
                        cout<<"Total Brokerage Charge"<<endl;

                        int userid;
                        cout<<"Enter User ID:";
                        cin>>userid;

                        cout<<"Total Brokerage Charges : "<<Broker_1.getBrokerageCharges(userid)<<endl;

                        break;
                    }
                    case 11:
                    {
                        cout<<"Display All Stocks Listed"<<endl;
                        Broker_1.display_all_stock();
                        break;
                    }
                    case 12:
                    {
                        cout<<"Display All Users Listed"<<endl;
                        Broker_1.display_all_user();
                        break;
                    }
                    case 13:
                    {
                        int sid;
                        cout<<"Enter stock id";
                        cin>>sid;
                        Broker_1.display_particular_stock(sid);
                        break;
                    }
                    case 14:
                    {
                        int uid;
                        cout<<"Enter user id";
                        cin>>uid;
                        Broker_1.display_particular_user(uid);
                        break;
                    }
                    case 15:
                    {
                        cout<<"Thank You"<<endl;
                        return 0;
                    }
                    default:
                    {
                        cout<<"Invalid option"<<endl;
                        break;
                    }
                }
            }

    return 0;
}