#include "server.h"

ST_accountsDB_t accRef;
ST_accountsDB_t accData[255] = { {30000,RUNNING,"01234567891234561"},{30000,RUNNING,"01234567891234562"},{30000,RUNNING,"01234567891234563"},
{30000,BLOCKED,"01234567891234564"},{30000,RUNNING,"01234567891234565"},{30000,RUNNING,"01234567891234566"} };
ST_transaction_t transDataBase[255] = { 0 };
int accIndex = 0;

EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	transData->transState = APPROVED;
	if (isValidAccount(transData->cardHolderData, &accRef) == ACCOUNT_NOT_FOUND) transData->transState = FRAUD_CARD;
	else if (isBlockedAccount(&accRef) == BLOCKED_ACCOUNT) transData->transState = DECLINED_STOLEN_CARD;
	else if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE) transData->transState = DECLINED_INSUFFECIENT_FUND;
	if (saveTransaction(transData) == SAVING_FAILED)transData->transState = INTERNAL_SERVER_ERROR;
	return transData->transState;
}

EN_serverError_t isValidAccount(ST_cardData_t cardData, ST_accountsDB_t* accountRefrence) {
	while (accIndex < 255) {
		if (strcmp(cardData.primaryAccountNumber, accData[accIndex].primaryAccountNumber) == 0) break;
		accIndex++;
	}
	if (accIndex == 255) return ACCOUNT_NOT_FOUND;
	*accountRefrence = accData[accIndex];
	return SERVER_OK;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {

	if (accountRefrence->state == BLOCKED) return BLOCKED_ACCOUNT;

	return SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData) {
	if (termData->transAmount > accRef.balance) return LOW_BALANCE;

	return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	int i = 0;
	while (i < 255) {													//searching for empty place
		if (transDataBase[i].transactionSequenceNumber == 0) break;
		i++;
	}
	if (i == 255) return SAVING_FAILED;
	transData->transactionSequenceNumber= 1010+i*10;                  //arbitary sequence number               (MODIFIED)
	transDataBase[i] = *transData;                                    // check if it actually copy a structure (checked)
	if(transData->transState == 0) accData[accIndex].balance -= transDataBase[i].terminalData.transAmount;      // updating balance
	return SERVER_OK;

}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData) {
	int i = 0;
	while (i < 255) {
		if (transDataBase[i].transactionSequenceNumber == transactionSequenceNumber) break;
		else if (transDataBase[i].transactionSequenceNumber == 0) return TRANSACTION_NOT_FOUND;
		i++;
	}
	if (i == 255) return TRANSACTION_NOT_FOUND;
	*transData = transDataBase[i];
	return SERVER_OK;
}
