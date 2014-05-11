//Mike Orr, Luke Segaran, Tom sugarev - May 14

#include "ISAAccount.h"

ISAAccount::ISAAccount( const Date, const double minimumBalance,
					   const double currentYearlyDeposit, const double maximumYearlyDeposit )
					   : SavingsAccount( minimumBalance ), currentYearlyDeposit_( currentYearlyDeposit ),
					   maximumYearlyDeposit_( maximumYearlyDeposit )
{

}

ISAAccount::ISAAccount( const string &typ, const string& acctNum, const string& sCode,
                          const Date& cD, double b, const TransactionList& trList, const double minimumBalance,
						  const Date endDepositPeriod, const double currentYearlyDeposit,
						  const double maximumYearlyDeposit )
						  : SavingsAccount( typ, acctNum, sCode, cD, b, trList, minimumBalance ),
						  endDepositPeriod_( endDepositPeriod ),
						  currentYearlyDeposit_( currentYearlyDeposit ), maximumYearlyDeposit_( maximumYearlyDeposit )
{
}

//Public member methods

//Accessor methods
double ISAAccount::getCurrentYearlyDeposit( void ) const
{
	return currentYearlyDeposit_;
}

double ISAAccount::getMaximumYearlyDeposit( void ) const
{
	return maximumYearlyDeposit_;
}

Date ISAAccount::getEndDepositPeriod( void ) const
{
	return endDepositPeriod_;
}

//Private member methods

//Setter methods
void ISAAccount::updateCurrentYearlyDeposit( const double deposit )
{
	currentYearlyDeposit_ += deposit;
}

ostream& ISAAccount::putDataInStream( ostream& os) const
{
	//put (unformatted) BankAccount details in stream
    os << accountType_ << "\n";				//put account type
    os << accountNumber_ << "\n";			//put account number
	os << sortCode_ << "\n";				//put sort code
    os << creationDate_ << "\n";			//put creation date
	os << balance_ << "\n";					//put balance
	os << minimumBalance_ << "\n";
	os << currentYearlyDeposit_ << "\n";
	os << maximumYearlyDeposit_ << "\n";
	os << endDepositPeriod_ << "\n";

	if (  ! transactions_.size() == 0)
		os << transactions_;				//put all transactions, one per line

	return os;
}

istream& ISAAccount::getDataFromStream( istream& is)
{
		//get BankAccount details from stream
    is >> accountType_;						//get account type
    is >> accountNumber_;					//get account number
	is >> sortCode_;						//get sort code
 	is >> creationDate_;					//get creation date
	is >> balance_;							//get balance_
	is >> minimumBalance_;
	is >> currentYearlyDeposit_;
	is >> maximumYearlyDeposit_;
	is >> endDepositPeriod_;
	is >> transactions_;					//get all transactions (if any)

	return is;
}

const bool ISAAccount::canTransferIn( const double amount ) const
{
	return (((currentYearlyDeposit_ + amount ) <= maximumYearlyDeposit_) &&
		(Date::currentDate() < endDepositPeriod_) && (amount >= 0.0));
}

void ISAAccount::recordTransferIn( const double amount, const string aAN, const string aSC )
{
	const string transaction = ( "Transfer_from_" + aAN + "_" + aSC ); 
	transactions_.addNewTransaction( Transaction( transaction, amount ) ); 

	updateCurrentYearlyDeposit( amount );
	updateBalance( amount ); //Change account balance
}

const string ISAAccount::prepareFormattedAccountDetails() const
{
	return "Function yet to be implemented";
	//collect account details in string
	ostringstream os;
	//account details
	os << "\nACCOUNT TYPE:    " << accountType_ << " ACCOUNT";						//display account type
	os << "\nACCOUNT NUMBER:  " << accountNumber_;									//display account number
	os << "\nSORT CODE:       " << sortCode_;										//display sort code
	os << "\nCREATION DATE:   " << creationDate_.toFormattedString();				//display creation date
	os << fixed << setprecision(2) << setfill(' ');
	os << "\nBALANCE:         \234" << setw(10) << balance_;						//display balance
	os << "\nCURRENT YEARS DEPOSIT: \234" << setw(10) << currentYearlyDeposit_;		//display current year deposit
	os << "\nMAX YEARS DEPOSIT: \234" << setw(10) << maximumYearlyDeposit_;			//display max year deposit
	os << "\nDEPOSIT RENEWAL DATE: " << endDepositPeriod_.toFormattedString();		//display deposit renewal date

	return os.str();
}