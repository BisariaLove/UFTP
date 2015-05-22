/*
*@author Love Bisaria
*date: September 8, 2014 1:15 AM
*/
#include<stdio.h>
#include<stdlib.h>
#include "my402list.h"

int main()
{
        My402List Q1;
	My402ListElem *elem = NULL;
	int rc;
	int *temp;
	int i,j,k,l;

	//List Initialization
	rc = My402ListInit(&Q1);

	if(0 == rc)
	{
		printf("Initialization of the list failed");
		return 1;
	}

	//Creating the List
	i=10;

	j=20;
	k=30;

	rc = My402ListAppend(&Q1 , &i);
	if(0 == rc)
	{
		printf("Append Failed");
		return 1;
	}
	printf("\nNode Value :%d \n",i);

	rc = My402ListAppend(&Q1 , &j);
	if(0 == rc)
	{
		printf("Append Failed");
		return 1;
	}
	printf("\nNode Value :%d \n",j);

	rc = My402ListAppend(&Q1 , &k);
	if(0 == rc)
	{
		printf("Append Failed");
		return 1;
	}
	printf("\nNode Value :%d \n",k);

	//Deleting the list
	for(l=0;l<3;l++)
	{
		elem = My402ListFirst(&Q1);
		temp = elem-> obj;
		printf("\nDeleted Value is %d", *temp);
		My402ListUnlink(&Q1,elem);

	}
	printf("\n\n-----------------Program Ends----------------------\n");
	return 0;
}

