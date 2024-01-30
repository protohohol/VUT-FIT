/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	if (list->firstElement != NULL) { //checks if list is not empty
		DLLElementPtr currentElement = list->firstElement; //pointer to first element of list
		do { //free every element of list one by one
			DLLElementPtr nextElement = currentElement->nextElement;
			free(currentElement);
			currentElement = nextElement;
		} while (currentElement != NULL);
	}

	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr newElement = malloc(sizeof(struct DLLElement)); //Initialization of new element
	if (newElement == NULL) {
		DLL_Error();
	} else {
		newElement->nextElement = NULL;
		newElement->previousElement = NULL;
		if (list->firstElement == NULL) { //if first element is NULL, list is empty and we can just insert
			list->firstElement = newElement;
			list->firstElement->data = data;
			list->lastElement = newElement;
		} else { //if the list is not empty, push first element by one and replace with a new one
			list->firstElement->previousElement = newElement;
			newElement->nextElement = list->firstElement;
			list->firstElement = newElement;
			list->firstElement->data = data;
		}
	}
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	DLLElementPtr newElement = malloc(sizeof(struct DLLElement)); //Initialization of new element
	if (newElement == NULL) {
		DLL_Error();
	} else {
		newElement->nextElement = NULL;
		newElement->previousElement = NULL;
		if (list->lastElement == NULL) { //if last element is NULL, list is empty and we can just insert
			list->lastElement = newElement;
			list->lastElement->data = data;
			list->firstElement = list->lastElement;
		} else { //if the list is not empty, push back last element by one and replace with a new one
			list->lastElement->nextElement = newElement;
			newElement->previousElement = list->lastElement;
			list->lastElement = newElement;
			list->lastElement->data = data;
		}
	}
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if (list->firstElement == NULL) {
		DLL_Error();
	} else {
		*dataPtr = list->firstElement->data; //changing value of dataPtr
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	if (list->lastElement == NULL) {
		DLL_Error();
	} else {
		*dataPtr = list->lastElement->data; //changing value of dataPtr
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (list->firstElement != NULL) { //checking if list is not empty
		//saving pointer to second element
		DLLElementPtr nextElement = list->firstElement->nextElement;
		//if first element was active, now it's NULL
		if (list->activeElement == list->firstElement) {
			list->activeElement = NULL;
		}
		//if there was one element in list, assigning last element to NULL
		if (list->lastElement == list->firstElement) {
			list->lastElement = NULL;
		}
		free(list->firstElement);
		list->firstElement = nextElement;
		if (list->firstElement != NULL) { //assigning previous element for new first element to NULL, if exists
			list->firstElement->previousElement = NULL;
		}
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if (list->lastElement != NULL) { //checking if list is not empty
		//saving pointer to penultimate element
		DLLElementPtr previousElement = list->lastElement->previousElement;
		//if last element was active, now it's NULL
		if (list->activeElement == list->lastElement) {
			list->activeElement = NULL;
		}
		//if there was one element in list, assigning first element to NULL
		if (list->lastElement == list->firstElement) {
			list->firstElement = NULL;
		}
		free(list->lastElement);
		list->lastElement = previousElement;
		if (list->lastElement != NULL) { //assigning next element for new last element to NULL, if exists
			list->lastElement->nextElement = NULL;
		}
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	//checking if list has active element and if active element is not last element
	if (list->activeElement != NULL && list->activeElement->nextElement != NULL) { 
		DLLElementPtr deletableElement = list->activeElement->nextElement; //saving pointer to deletable element
		//reassigning next element of active element to next element of deletable element
		list->activeElement->nextElement = deletableElement->nextElement;
		//reassigning previous element of new next element if exists
		if (list->activeElement->nextElement != NULL) {
			list->activeElement->nextElement->previousElement = list->activeElement;
		} else {
			list->lastElement = list->activeElement;
		}
		free(deletableElement);
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	//checking if list has active element and if active element is not first element
	if (list->activeElement != NULL && list->activeElement->previousElement != NULL) { 
		DLLElementPtr deletableElement = list->activeElement->previousElement; //saving pointer to deletable element
		//reassigning previous element of active element to previous element of deletable element
		list->activeElement->previousElement = deletableElement->previousElement;
		//reassigning next element of new previous element if exists
		if (list->activeElement->previousElement != NULL) {
			list->activeElement->previousElement->nextElement = list->activeElement;
		}  else {
			list->firstElement = list->activeElement;
		}
		free(deletableElement);
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	if (list->activeElement != NULL) {
		DLLElementPtr newElement = malloc(sizeof(struct DLLElement));	//Initialization of new element
		if (newElement == NULL) {
			DLL_Error();
		} else {
			if (list->lastElement == list->activeElement) {
				list->lastElement = newElement;
			}
			//assigning next element of new element to next element of active element
			newElement->nextElement = list->activeElement->nextElement;
			newElement->previousElement = list->activeElement;
			//assigning new previous element for next element of new element if it's not NULL
			if (newElement->nextElement != NULL) {
				newElement->nextElement->previousElement = newElement;
			}
			newElement->data = data;
			list->activeElement->nextElement = newElement; //now next element of active element is new element
		}
	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	if (list->activeElement != NULL) {
		DLLElementPtr newElement = malloc(sizeof(struct DLLElement));	//Initialization of new element
		if (newElement == NULL) {
			DLL_Error();
		} else {
			if (list->firstElement == list->activeElement) {
				list->firstElement = newElement;
			}
			//assigning previous element of new element to previous element of active element
			newElement->previousElement = list->activeElement->previousElement;
			newElement->nextElement = list->activeElement;
			//assigning new next element for previous element of new element if it's not NULL
			if (newElement->previousElement != NULL) {
				newElement->previousElement->nextElement = newElement;
			}
			newElement->data = data;
			list->activeElement->previousElement = newElement; //now previous element of active element is new element
		}
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if (list->activeElement == NULL) {
		DLL_Error();
	} else {
		*dataPtr = list->activeElement->data; //changing value of dataPtr
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if (list->activeElement != NULL) {
		list->activeElement->data = data;
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement != NULL) {
		list->activeElement = list->activeElement->nextElement;
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (list->activeElement != NULL) {
		list->activeElement = list->activeElement->previousElement;
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return list->activeElement != NULL ? 1 : 0;
}

/* Konec c206.c */
