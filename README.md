# STOCK BROKER SYSTEM

- A C++ program that models a stock brokerage system where users can perform buy/sell transactions.
- I am Attaching a PDF FILE for Functionalities.

## Functionalities

I am outlining some functionalities that will be implemented, divided into two types:

- **Must-have Functionalities:** Essential features needed for core operations.
- **Nice-to-have Functionalities:** Additional features that enhance usability and provide extra value.

### Must-have Functionalities
- **Register User:** Allows the broker to add new users.
- **Unregister User:** Allows the broker to delete a user.
- **Stock Holds:** Displays the number of stocks a single user is holding.
- **Register Stock:** Allows the broker to add new stocks.
- **Unregister Stock:** Allows the broker to delete a stock.
- **Buy Stock:** Enables users to purchase stocks with the broker's assistance, ensuring sufficient balance and updating stock holdings.
  - Deducts amount
  - Updates stocks the user is holding
- **Sell Stock:** Allows users to sell stocks with the broker's assistance, ensuring they have sufficient stock to sell and updating their balance accordingly.
  - Adds amount
  - Updates stocks the user is holding

### Nice-to-have Functionalities
- **Transaction History Retrieval:** Stores each user’s transactions, whether buying or selling.
- **Balance Inquiry:** Allows users to check their remaining balance with the broker's assistance.
- **Brokerage Charges Display:** Shows users the total brokerage charges they have incurred.
- **Display Stock Information:** Provides users with stock details, including brokerage charges.
- **Display User Information:** Allows users to view their account information.
- **Balance Notification:** Alerts users when their balance drops below a specified limit.

## Flowchart

![Flowchart](FlowChart.jpg)

## How to Run

You can run this C++ program using the following methods:

### 1. Running Locally in VS Code

- Open the `.c++` file in **Visual Studio Code (VS Code)**.
- Make sure you have the **C++ extension** installed. If not, you can install it from the Extensions Marketplace.
- **Compile and Run**:
  - Open a terminal in VS Code.
  - Run the following command:
    ```
    g++ filename.cpp -o anyname
    ./anyname 
    ```

### 2. Running Online

You can also run this program in an online compiler like [OnlineGDB](https://www.onlinegdb.com/online_c++_compiler). Just paste the code into the editor and click "Run" to execute the program without needing to install anything locally.
