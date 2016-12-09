#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"fileIO.h"
#include<string.h>

char temp[500] = {0};
List_Client *list_client = NULL;
List_Book *list_book = NULL;
List_Borrow *list_borrow = NULL;

/*********************
  이거슨 제가 오류 잡기위해 만든함수
  쓰고싶으시다면 써도 상관은 없습니다만.
  암튼 리스트 개수를 알려주는 함수이랍니다.
   ********************/
int say_Client_list_num()
{
	int count = 0;
	list_client -> current = list_client -> head;
	while(list_client -> current)
	{
		count ++;
		list_client -> current = list_client -> current -> next;
	}
	list_client -> current = list_client -> head;
	return count;
}

int say_Borrow_list_num()
{
	int count = 0;
	list_borrow -> current = list_borrow -> head;
	while(list_borrow -> current)
	{
		count ++;
		list_borrow -> current = list_borrow -> current -> next;
	}
	list_borrow -> current = list_borrow -> head;
	return count;
}

int say_Book_list_num()
{
	int count= 0;
	list_book -> current = list_book -> head;
	while(list_book -> current)
	{
		count ++;
		list_book -> current = list_book -> current -> next;
	}
	list_book -> current = list_book -> head;
	return count;
}

/*****************************************************************
	파일 입력함수
  *************************************************************/

void file_write()
{
	file_write_client();
	file_write_book();
	file_write_borrow();
}

void file_write_client()
{
	FILE *fp = fopen("data/client.txt", "w");
	list_client -> current = list_client -> head;
	while(list_client -> current)
	{
		fprintf(fp, "|%d",list_client ->current -> sch_num);	
		fprintf(fp, "|%s",list_client -> current -> name);	
		fprintf(fp, "|%s",list_client -> current -> password);	
		fprintf(fp, "|%s",list_client -> current -> address);	
		fprintf(fp, "|%s|\n",list_client -> current -> phone_num);	

		list_client -> current = list_client -> current ->next;
	}

	fclose(fp);
}

void file_write_book()
{
	FILE *fp =fopen("data/book.txt", "w");

	list_book -> current = list_book -> head;

	while(list_book -> current)
	{
		fprintf(fp, "|%ld", list_book -> current -> ISBN);
		
		fprintf(fp, "|%d", list_book -> current -> book_num);
			
		fprintf(fp, "|%s", list_book -> current -> name);
		
		fprintf(fp, "|%s", list_book -> current -> publisher);
		
		fprintf(fp, "|%s", list_book -> current -> author);
		
		fprintf(fp, "|%s", list_book -> current -> owner);
		
		fprintf(fp, "|%c|\n", list_book -> current -> borrow_Y_N);
	
		list_book ->current = list_book -> current -> next;
	}
	fclose(fp);
}

void file_write_borrow()
{
	FILE *fp = fopen("data/borrow.txt", "w");

	list_borrow ->current = list_borrow -> head;
	
	while(list_borrow -> current)
	{
		fprintf(fp, "|%d", list_borrow -> current -> sch_num);
		fprintf(fp, "|%d", list_borrow -> current -> book_num);
		fprintf(fp, "|%ld", list_borrow -> current -> borrow_day);
		fprintf(fp, "|%ld|\n", list_borrow -> current -> return_day);
		
		list_borrow -> current = list_borrow -> current ->next;
	}
	fclose(fp);
}




/**********************
  리스트 초기화 함수들
   ********************/
void init_all_list()
{
	if(list_client == NULL)
		list_client = (List_Client *)malloc(sizeof(List_Client));
	if(list_book == NULL)
		list_book = (List_Book *)malloc(sizeof(List_Book));
	if(list_borrow == NULL)
		list_borrow = (List_Borrow *)malloc(sizeof(List_Borrow));

	init_client_list();
	init_borrow_list();
	init_book_list();
}

void init_client_list()
{
	list_client -> head = NULL;
	list_client -> current = NULL;
	list_client -> tail = NULL;
}

void init_borrow_list()
{
	list_borrow -> head = NULL;
	list_borrow -> current = NULL;
	list_borrow -> tail = NULL;
}

void init_book_list()
{
	list_book -> head = NULL;
	list_book -> current = NULL;
	list_book -> tail = NULL;
}

/********************************************************************************
	파일 입력 소스코드 부분입니다!
  ******************************************************************************/

char *get_oneWord(FILE **fp)
{
	memset(temp,0,sizeof(temp));
	fgetc(*fp);
	fscanf(*fp ,"%[^|\n]",temp);
	return temp;
}

void get_all_file_data()
{
	FILE *client_fp, *book_fp, *borrow_fp;
	client_fp = fopen("data/client.txt","r");
	book_fp = fopen("data/book.txt","r");
	borrow_fp = fopen("data/borrow.txt","r");
	
	if(list_client -> head == NULL)
		make_client_node();
	if(list_book -> head == NULL)
		make_book_node();
	if(list_borrow -> head == NULL)
		make_borrow_node();
	
	while(get_client_file_data(client_fp))
	{
		make_client_node();
	}
	while(get_book_file_data(book_fp))
	{
		make_book_node();
	}
	while(get_borrow_file_data(borrow_fp))
	{
		make_borrow_node();
	}
	

	fclose(client_fp);
	fclose(book_fp);
	fclose(borrow_fp);
}

int get_client_file_data(FILE *fp)
{
	Client *client;
	client = list_client -> current;

	client -> sch_num = atoi(get_oneWord(&fp));

	get_oneWord(&fp);
	client -> name = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
	strcpy(client -> name, temp);
	
	get_oneWord(&fp);
	client -> password = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
	strcpy(client -> password, temp);

	get_oneWord(&fp);
	client -> address = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
	strcpy(client -> address, temp);

	get_oneWord(&fp);
	client -> phone_num = (char *)malloc(sizeof(char) *(strlen(temp) + 1));
	strcpy(client -> phone_num, temp);
	fseek(fp,1,SEEK_CUR);	
	char a = fgetc(fp);
	while(a != '|')
	{
		if(a == EOF)
			return 0;
		a = fgetc(fp);
	}
	fseek(fp,-1,SEEK_CUR);	

	return 1;
}

int get_book_file_data(FILE *fp)
{
	Book *book;
	book = list_book -> current;
	
	book -> ISBN = atol(get_oneWord(&fp));
	book -> book_num = atoi(get_oneWord(&fp));
	if(list_book -> last_book_num < book -> book_num)
		list_book -> last_book_num = book -> book_num;
	
	get_oneWord(&fp);
	book -> name = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
	strcpy(book -> name,temp);

	get_oneWord(&fp);
	book -> publisher = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
	strcpy(book -> publisher,temp);
	
	
	get_oneWord(&fp);
	book -> author = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
	strcpy(book -> author,temp);

	get_oneWord(&fp);
	book -> owner = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
	strcpy(book -> owner,temp);

	get_oneWord(&fp);
	book -> borrow_Y_N = temp[0];
	fseek(fp,1,SEEK_CUR);	
	
	char a = fgetc(fp);
	while(a != '|')
	{
		if(a == EOF)
			return 0;
		a = fgetc(fp);
	}
	fseek(fp,-1,SEEK_CUR);	
	
	
	return 1;
}
	
int get_borrow_file_data(FILE *fp)
{
	Borrow *borrow;
	borrow = list_borrow -> current;
	borrow -> sch_num = atoi(get_oneWord(&fp));
	borrow -> book_num = atoi(get_oneWord(&fp));
	borrow -> borrow_day = atol(get_oneWord(&fp));
	borrow -> return_day = atol(get_oneWord(&fp));
	fseek(fp,1,SEEK_CUR);	
	char a = fgetc(fp);
	while(a != '|')
	{
		if(a == EOF)
			return 0;
		a = fgetc(fp);
	}

	return 1;
}

/*********************************************************
	node free 소스코드	
 *************************************************************/


void free_all_node()
{
	free_client_node();
	free_book_node();
	free_borrow_node();
}

void free_client_node()
{
	list_client -> current = list_client -> head;
	
	while(list_client -> current)
	{
		free(list_client -> current -> name);
		free(list_client -> current -> password);
		free(list_client -> current -> address);
		free(list_client -> current -> phone_num);
		if(list_client -> current -> next)
		{
			list_client -> current = list_client -> current -> next;
			free(list_client -> current -> last);
		}
		else
		{
			free(list_client -> current);
			list_client -> current = NULL;
		}
	}
}

void free_book_node()
{
	list_book -> current = list_book -> head;
	while(list_book -> current)
	{
		free(list_book -> current -> name);
		free(list_book -> current -> publisher);
		free(list_book -> current -> author);
		free(list_book -> current -> owner);
	
		if(list_book -> current -> next)
		{
			list_book -> current = list_book -> current -> next;
			free(list_book -> current -> last);
		}
		else
		{
			free(list_book -> current);
			list_book -> current = NULL;
		}
	}
}

void free_borrow_node()
{
	list_borrow -> current = list_borrow -> head;
	while(list_borrow -> current)
	{
		if(list_borrow -> current -> next)
		{
			list_borrow -> current = list_borrow -> current -> next;
			free(list_borrow -> current -> last);
		}
		else
		{
			free(list_borrow -> current);
			list_borrow -> current = NULL;
		}
	}
}




/********************************************************************************
	node생성 소스코드 부분
  ******************************************************************************/
void make_client_node()
{
	if(list_client -> head == NULL)
	{
		list_client -> tail = (Client *)malloc(sizeof(Client));
		list_client -> head = list_client -> tail;
		list_client -> current = list_client -> tail;
		list_client -> tail -> next = NULL;
		list_client -> tail -> last = NULL;
	}
	else
	{
		list_client -> tail = (Client *)malloc(sizeof(Client));
		list_client -> current -> next = list_client -> tail;
		list_client -> tail -> last = list_client -> current;
		list_client -> current = list_client -> tail;
		list_client -> tail -> next = NULL;
	}
}

void make_borrow_node()
{
	if(list_borrow -> head == NULL)
	{
		list_borrow -> tail = (Borrow *)malloc(sizeof(Borrow));
		list_borrow -> head = list_borrow -> tail;
		list_borrow -> current = list_borrow ->tail;
		list_borrow -> tail -> next = NULL;
		list_borrow -> tail -> last = NULL;
	}
	else
	{
		list_borrow -> tail = (Borrow *)malloc(sizeof(Borrow));
		list_borrow -> current -> next = list_borrow -> tail;
		list_borrow -> tail -> last = list_borrow -> current;
		list_borrow -> current = list_borrow ->tail;
		list_borrow -> tail -> next = NULL;
	}
}

void make_book_node()
{
	if(list_book -> head == NULL)
	{
		list_book -> tail = (Book *)malloc(sizeof(Book));
		list_book -> head = list_book -> tail;
		list_book -> current = list_book -> tail;
		list_book -> tail -> next = NULL;
		list_book -> tail -> last = NULL;
	}
	else
	{
		list_book -> tail = (Book *)malloc(sizeof(Book));
		list_book -> tail -> last = list_book -> current;
		list_book -> current -> next = list_book -> tail;
		list_book -> current = list_book -> tail;
		list_book -> tail -> next = NULL;
	}
}
#ifdef DEGBUG
int main(void)
{
	init_all_list();
	get_all_file_data();
	free_all_node();
	printf("%d\n", say_Book_list_num());
	return 0;
}
#endif
