
#include "app.h"
#include "../Server/server.h"
#pragma warning (disable : 4996)


int main() {
	char A =0;
	int N = 0;
	ST_transaction_t gettrans;
	while (1) {
		printf("get a previous Transaction Data?(Y/N): ");                             //get transaction
		scanf(" %c", &A);												              
		int c;
		while ((c = getchar()) != '\n' && c != EOF);                                   //clears input buffer (Modified)
		if (A == 'Y') {

			printf("Enter transaction Number: ");
			scanf(" %d", &N);
			N=getTransaction(N, &gettrans);
			if (N == SERVER_OK)
			{
				printf("\n********************transaction********************\n");
				printf("Date:                   %s\n", gettrans.terminalData.transactionDate);
				printf("Card holder name:       %s\n", gettrans.cardHolderData.cardHolderName);
				printf("Primary account Number: %s\n", gettrans.cardHolderData.primaryAccountNumber);
				printf("transaction amount:     %f\n", gettrans.terminalData.transAmount);
				printf("state:                  ");
				if (gettrans.transState == FRAUD_CARD)printf("Declined No acc.\n");
				if (gettrans.transState == APPROVED) printf("Approved\n");
				if (gettrans.transState == DECLINED_STOLEN_CARD)printf("Declined acc. is Blocked\n");
				if (gettrans.transState == DECLINED_INSUFFECIENT_FUND)printf("Declined Insuffecient fund\n");
				if (gettrans.transState == INTERNAL_SERVER_ERROR)printf("Declined Internal error\n");
				printf("Transaction number:     %d\n", gettrans.transactionSequenceNumber);
				printf("***************************************************\n");
			}
			else printf("TRANSACTION NOT FOUND\n");
			
		}

		printf("Make a transaction?(Y/N): ");                                //make transaction
		scanf(" %c", &A);
		while ((c = getchar()) != '\n' && c != EOF);                         //clears input buffer (Modified)
		if (A == 'Y') {
			appStart();
		}

	}
}

void appStart(void) {
	ST_cardData_t Card1;
	ST_terminalData_t term1;
	int x = 0;
	while (1) {
		x = getCardHolderName(&Card1);
		if (x == 0) break;
		else printf("Wrong card holder name!\n");
	}
	while (1) {
		x = getCardExpiryDate(&Card1);
		if (x == 0) break;
		else printf("Wrong Expiry Date!\n");
	}
	while (1) {
		x = getCardPAN(&Card1);
		if (x == 0) break;
		else printf("Wrong PAN!\n");
	}

	getTransactionDate(&term1);

	x=isCardExpired(Card1, term1);
	if (x != 0) {
		printf("Card is Expired!\n");
		return;}

	while (1) {
		x = setMaxAmount(&term1);
		if (x == 0) break;
		else printf("Wrong max amount!\n");
	}
	while (1) {
		x = getTransactionAmount(&term1);
		if (x == 0) break;
		else printf("Invalid amount!\n");
	}
	while (1) {
		x = isBelowMaxAmount(&term1);
		if (x == 0) break;
		else printf("Exceed max amount!\n");
		return;
	}
	
	ST_transaction_t trans1 = { Card1,term1 };
	EN_transState_t error;
	error = recieveTransactionData(&trans1);
	printf("\n********************transaction********************\n");
	printf("Date:                   %s\n", term1.transactionDate);
	printf("Card holder name:       %s\n", Card1.cardHolderName);
	printf("Primary account Number: %s\n", Card1.primaryAccountNumber);
	printf("transaction amount:     %f\n", term1.transAmount);
	printf("state:                  ");
	if (error == FRAUD_CARD)printf("Declined No acc.\n");
	if (error == APPROVED) printf("Approved\n");     
	if (error == DECLINED_STOLEN_CARD)printf("Declined acc. is Blocked\n");
	if (error == DECLINED_INSUFFECIENT_FUND)printf("Declined Insuffecient fund\n");
	if (error == INTERNAL_SERVER_ERROR)printf("Declined Internal error\n");
	printf("Transaction number:     %d\n", trans1.transactionSequenceNumber);                   //MODIFIED
	printf("***************************************************\n");
	printf("%s  %s %f\n", transDataBase[0].cardHolderData.cardHolderName, transDataBase[0].cardHolderData.primaryAccountNumber,accData[accIndex].balance);
}