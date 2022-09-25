#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	char name[26];
	printf("Enter card holder name: ");
	if (!fgets(name, 26, stdin) || strlen(name) < 21) {
		return WRONG_NAME;
	}
	else if (!strchr(name, '\n')) {
		fgets(name, 26, stdin);
		return WRONG_NAME;
	}
	else {
		name[strcspn(name, "\n")] = '\0';
		strcpy_s(cardData->cardHolderName, 25, name);
		return CARD_OK;
	}
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	char date[7];
	printf("Enter Expiry date(MM/YY): ");
	if (!fgets(date, 7, stdin) || date[6] != '\0' || date[2] != '/') {
		return WRONG_EXP_DATE;
	}
	else if (!strchr(date, '\n')) {
		fgets(date, 7, stdin);
		return WRONG_EXP_DATE;
	}
	else if ((date[0] - 48) * 10 + date[1] - 48 > 12 || (date[0] - 48) * 10 + date[1] - 48 <= 0) { //cheks if month is vaild (01->12) 
		return WRONG_EXP_DATE;
	}
	else if ((date[3] - 48) * 10 + date[4] - 48 > 99 || (date[3] - 48) * 10 + date[4] - 48 < 0) {  //cheks if year is vaild (00->99)
		return WRONG_EXP_DATE;
	}
	else {
		date[strcspn(date, "\n")] = '\0';
		strcpy_s(cardData->cardExpirationDate, 6, date);
		return CARD_OK;
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	printf("Enter PAN: ");
	char PAN[21];
	if (!fgets(PAN, 21, stdin) || strlen(PAN) < 17) {
		return WRONG_PAN;
	}
	else if (!strchr(PAN, '\n')) {
		fgets(PAN, 21, stdin);
		return WRONG_PAN;
	}
	else {
		PAN[strcspn(PAN, "\n")] = '\0';
		strcpy_s(cardData->primaryAccountNumber, 20, PAN);
		return CARD_OK;
	}

}
