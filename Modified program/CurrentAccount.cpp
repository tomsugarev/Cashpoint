#include "CurrentAccount.h"

//Constructor/Destructor implementations

CurrentAccount::CurrentAccount( const double overdraftLimit ) : overdraftLimit_( overdraftLimit )
{
}

CurrentAccount::CurrentAccount( const string& acctNum, const string& sCode,
                          const Date& cD, double b,
                          const TransactionList& trList, const double overdraftLimit ) :
BankAccount(  "CURRENT", acctNum, sCode, cD, b, trList ), overdraftLimit_( overdraftLimit )
{

}

//Public member methods

//Accessor methods
double CurrentAccount::getOverdraftLimit( void ) const
{
	return overdraftLimit_;
}

void CurrentAccount::transferMoney( const double amount, BankAccount *toAccount )
{
	char answer = ' '; //For use with checking if the user wishes to continue
	double balance = getBalance(); //Store the current balance of the account

	string toAccNo = toAccount->getAccountNumber();
	string toSrtCode = toAccount->getSortCode();

	if( *this != *toAccount ) //Check again that the two accounts are not the same
	{
		printf( "ACCOUNT BALANCE: \x9C%.2f\n", balance ); //Let the customer know their balance.
		printf( "ACCOUNT OVERDRAFT: \x9C%.2f\n", overdraftLimit_ ); //Let the customer know their overdraft limit.
		printf( "AVAILABLE: \x9C%.2f\n", (balance + overdraftLimit_) ); //Let the customer know the amount available

		if( (balance - amount) >= -overdraftLimit_ ) //If the transfer does not leave us beyond overdraft
		{
			printf( "The transfer can be granted.\n" );
			printf( "Are you sure you wish to transfer %.2f to %s %s (Y/N): ",
				amount, toAccNo.c_str(), toSrtCode.c_str() );
			
			cin >> answer;

			if( answer == 'Y' || answer == 'y' ) //Check if the user still wishes to proceed
			{
				updateBalance( -amount ); //Change each account's balance
				toAccount->updateBalance( amount );
				
				recordTransfer( -amount, "Transfer_to_" + toAccNo + "_" + toSrtCode ); //Record the transfer
				toAccount->recordTransfer( amount, "Transfer_from_" + accountNumber_ + "_" + sortCode_ );
				
				//storeBankAccountInFile( FILEPATH + "account_" + accountNumber_ + "_" + sortCode_ + ".txt" ); //Save the accounts' current state
				//toAccount.storeBankAccountInFile( FILEPATH + "account_" + toAccount.getAccountNumber() + "_" + toAccount.getSortCode() + ".txt" );
				
				cout << "Transfer success" << endl;
			}
			else
				cout << "The transfer did not take place" << endl;
		}
		else
			printf( "INSUFFICIENT FUNDS TO TRANSFER \x9C%.2f\n", amount );
	}
}

ostream& CurrentAccount::putDataInStream( ostream& os ) const
{
	//put (unformatted) BankAccount details in stream
    os << accountType_ << "\n";				//put account type
    os << accountNumber_ << "\n";			//put account number
	os << sortCode_ << "\n";				//put sort code
    os << creationDate_ << "\n";			//put creation date
	os << balance_ << "\n";					//put balance
	os << overdraftLimit_ << "\n";

	if (  ! transactions_.size() == 0)
		os << transactions_;				//put all transactions, one per line

	return os;
}

istream& CurrentAccount::getDataFromStream( istream& is )
{
	//get BankAccount details from stream
    is >> accountType_;						//get account type
    is >> accountNumber_;					//get account number
	is >> sortCode_;						//get sort code
 	is >> creationDate_;					//get creation date
	is >> balance_;							//get balance_
	is >> overdraftLimit_;					//get overdraftLimit
	is >> transactions_;					//get all transactions (if any)

	return is;
}

const string CurrentAccount::prepareFormattedAccountDetails() const
{
		//collect account details in string
	ostringstream os;
	//account details
	os << "\nACCOUNT TYPE:    " << accountType_ << " ACCOUNT";						//display account type
	os << "\nACCOUNT NUMBER:  " << accountNumber_;									//display account number
	os << "\nSORT CODE:       " << sortCode_;										//display sort code
	os << "\nCREATION DATE:   " << creationDate_.toFormattedString();				//display creation date
	os << fixed << setprecision(2) << setfill(' ');
	os << "\nBALANCE:         \234" << setw(10) << balance_;						//display balance
	os << "\nOVERDRAFT:       \234" << setw(10) << overdraftLimit_;						//display overdraft
	os << "\nAVAILABLE:       \234" << setw(10) << (balance_ + overdraftLimit_);			//Available balance

	return os.str();
}