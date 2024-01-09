#include <cassert>
#include <iomanip>
#include <iostream>
#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//


Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,  
        unsigned int month_date,  unsigned year_date, 
        bool buy_sell_trans,  unsigned int number_shares,  
        double trans_amount ){
//
// initializing the member variables of the transaction object with the values passed through the constructors parameters
    symbol = ticker_symbol;
    day = day_date;
    month = month_date;
    year = year_date;
    shares = number_shares;
    amount = trans_amount;
    acb = 0.0;
    cgl = 0.0;
    acb_per_share = 0.0;
    share_balance = 0;
    p_next = nullptr;

// if buy_sell_trans is true then it's a "buy" transaction, then trans_type is set to "buy"
  if (buy_sell_trans){
    trans_type = "Buy";
  }
  // if buy_sell_trans is false then its a "sell" transaction, then trans_type is set to "sell"
  else {
  
    trans_type = "Sell"; 
  }
// increments the assigned_trans_id static member variable of the transaction class and assigns the
// resulting value to the trans_id member variable of the current transaction object
   trans_id = assigned_trans_id++;
    

 
//
        }


Transaction::~Transaction(){

p_next = nullptr;
    

}

 bool Transaction:: operator<( Transaction const &other ){

// object's year is less than the other transaction object's year, it implies that the current transaction occurred earlier in time
if (year < other.year) { 
    return true; 

// if years of both transactions equal, it checks whether the current transaction object's month is less than the other Transaction 
// object's month. If so, it returns true, implies the current transaction happened earlier
} else if (year == other.year && month < other.month) { 
    return true; 

// if true, it returns true, implies an earlier occurrence of the current transaction
} else if (year == other.year && month == other.month && day < other.day) { 
    return true; 

//  if the current transaction object's trans_id is greater than the other transaction object's trans_id, it returns true
} else if (year == other.year && month == other.month && day == other.day && trans_id > other.trans_id) { 
    return false;

} 

else if (year == other.year && month == other.month && day == other.day && trans_id < other.trans_id){
return true;
}

else { 
    return false; 


 } 


}



// GIVEN
// Member functions to get values.
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
History::History (){
p_head = 0;
  
}
// Destructor
History::~History (){

Transaction *current_trans = p_head; 
// continues as long as p_head points to a valid node 
while (p_head != nullptr) {
  // creates a temporary pointer temp and sets it to point to the same node that p_head is currently pointing to
        Transaction *temp = p_head;
        //  moves the p_head pointer to the next node in the linked list
        p_head = p_head->get_next();
        delete temp;

    }
  
}

// insert(...): Insert transaction into linked list.
void History::insert(Transaction *p_new_trans ){
Transaction *current_trans = p_head; 
  
 if (p_head == nullptr) {
        p_head = p_new_trans;
    } else {
        Transaction *current_trans = p_head;
        while (current_trans->get_next() != nullptr) {
            current_trans = current_trans->get_next();
        }
        current_trans->set_next(p_new_trans);
    }
}

// read_history(...): Read the transaction history from file.
void History::read_history(){
ece150::open_file(); 

    while (ece150::next_trans_entry()) {
        
        Transaction *new_transaction = new Transaction(ece150::get_trans_symbol(), 
                                                      ece150::get_trans_day(),
                                                    ece150::get_trans_month(),
                                                       ece150::get_trans_year(),
                                                       ece150::get_trans_type(),
                                                       ece150::get_trans_shares(),
                                                       ece150::get_trans_amount());

        insert(new_transaction);
    }
// close
    ece150::close_file(); 
}

// print() Print the transaction history.
void History::print(){
Transaction *current_trans = p_head; 
  
  std::cout << "BEGIN TRANSACTION HISTORY" 
  << std::endl;

  while (current_trans != nullptr) {
    current_trans -> print(); 
    current_trans = current_trans -> get_next(); 
  }

  std::cout << "END TRANSACTION HISTORY"
  << std::endl;
  
}

// third submission

// sort_by_date(): Sort the linked list by trade date.
void History::sort_by_date(){
Transaction *old_head = get_p_head();
    Transaction *new_head = nullptr;

    while (old_head != nullptr) {
      //performs the sorting operation
        Transaction *current = old_head;
        old_head = old_head->get_next();
        current->set_next(nullptr);

        if (new_head == nullptr || *current < *new_head) {
            current->set_next(new_head);
            new_head = current;
        } else {
            Transaction *temp = new_head;
            while (temp->get_next() != nullptr && !(*current < *(temp->get_next()))) {
                temp = temp->get_next();
            }
            current->set_next(temp->get_next());
            temp->set_next(current);
        }
    }
//Updates the head pointer of the main linked list to the newly sorted list
    p_head = new_head;


}

// update_acb_cgl(): Updates the ACB and CGL values.
void History::update_acb_cgl(){
Transaction *current = get_p_head();
    double acb = 0.0;
    unsigned int shares = 0;
    double acb_per_share = 0.0;
    double cgl = 0.0;
    //keep track of the previous transaction while iterating through the linked list
    Transaction *previous = nullptr;

    for (; current != nullptr; previous = current, current = current->get_next()) {
      //checks if the current transaction is a buy 
      //If sell, it calcs the adjustments in acb, shares, and cgl based on the previous transaction's details
        if (current->get_trans_type() == true) {
            acb += current->get_amount();
            shares += current->get_shares();
            acb_per_share = acb / shares;
            cgl = 0.0;
        } else {
            acb = previous->get_acb() - (current->get_shares() * previous->get_acb_per_share());
            shares = previous->get_share_balance() - current->get_shares();
            acb_per_share = previous->get_acb_per_share();
            cgl = current->get_amount() - (current->get_shares() * acb_per_share);
        }

        current->set_acb(acb);
        current->set_share_balance(shares);
        current->set_acb_per_share(acb_per_share);
        current->set_cgl(cgl);
    }

}

// compute_cgl(): )Compute the ACB, and CGL.
double History::compute_cgl ( unsigned int year ){
  // initializes a pointer 
Transaction *current = get_p_head();
//store the cumulative gain/loss (CGL) for a specific year
//starts with a value of 0.0
    double cgl_of_year = 0.0;
// updates current to its next node in the list by calling 
//current.get_next() at the end of each iteration
    for (; current != nullptr; current = current->get_next()) {
        cgl_of_year += (current->get_year() == year) ? current->get_cgl() : 0.0;
    }

    return cgl_of_year;

}


// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
