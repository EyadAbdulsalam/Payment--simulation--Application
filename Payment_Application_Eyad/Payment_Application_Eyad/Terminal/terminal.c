#include "terminal.h"


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	time_t t = time(NULL);              
	struct tm date;
	localtime_s(&date, &t);            
	if (date.tm_mday < 10 && date.tm_mon < 9) {               //insure the date will be written in the right format DD/MM/YYYY
		sprintf_s(termData->transactionDate, 11, "0%d/0%d/%d", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900);
	}
	else if (date.tm_mon < 9) {
		sprintf_s(termData->transactionDate, 11, "%d/0%d/%d", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900);
	}
	else if (date.tm_mday < 10) {
		sprintf_s(termData->transactionDate, 11, "%0d/%d/%d", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900);
	}
	else {
		sprintf_s(termData->transactionDate, 11, "%d/%d/%d", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900);
	}

	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {

	short int cardExpMM = (cardData.cardExpirationDate[0] - '0') * 10 + cardData.cardExpirationDate[1] - '0';    //month
	short int dateMM = (termData.transactionDate[3] - '0') * 10 + termData.transactionDate[4] - '0';             //month 

	short int cardExpYY = (cardData.cardExpirationDate[3] - '0') * 10 + cardData.cardExpirationDate[4] - '0';              //year
	short int dateYY = (termData.transactionDate[8] - '0') * 10 + termData.transactionDate[9] - '0';                       //year

	if (cardExpYY == dateYY && cardExpMM < dateMM) return EXPIRED_CARD;    //if the Exp date within the current year

	if (cardExpYY < dateYY) return EXPIRED_CARD;                       //if the Expiration year has past 

	return TERMINAL_OK;

}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	printf("Enter transaction amount: ");
	scanf_s("%f", &termData->transAmount);
	if (termData->transAmount <= 0) return INVALID_AMOUNT;
	return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount) return EXCEED_MAX_AMOUNT;
	return TERMINAL_OK;

}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	printf("Enter max amount: ");
	scanf_s("%f", &termData->maxTransAmount);
	if (termData->maxTransAmount <= 0) return INVALID_MAX_AMOUNT;
	return TERMINAL_OK;
}

