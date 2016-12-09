#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"manageData.h"

#ifdef DEBUG_ALONE
extern List_Client *list_client;
extern List_Book * list_book;
extern List_Borrow * list_borrow;

void file_write_client(void){return;}
void file_write_book(void){return;}
void file_write_borrow(void){return;}
#endif 

#define MAXBUFF 50

/// 유지보수의 편의성을 위해서 매크로를 이용합니다.
/// 제너릭 인터페이스가 쓰고싶었던 자의 몸부림입니다.
/// set tapstop=4 에 최적화 된 코드입니다.
#define CODE(Sth, sth, key, sortT, sortKey) 					\
/* 이 c 파일에서만 사용되는 get함수입니다. 성공을 확인한 후 리스트의 current*/\
/* 가져다 이용하시면 됩니다.*/									\
int private_get_##sth(int key)									\
{																\
	/* 리스트의 current 초기화 */								\
	list_##sth->current = list_##sth->head;						\
	if (strcmp(#key, #sortKey))/* 정렬키와 키가 다르다면*/		\
	{															\
		while(list_##sth->current->key != key && list_##sth->current->next != NULL)\
			list_##sth->current = list_##sth->current->next;	\
	}															\
	else														\
	{															\
		/* 리스트가 오름차순으로 정렬 되어 있으므로 자신보다 작은 값을 가진 원소까지 */\
		/* 검사하면 충분합니다. key값이 같거나 클 경우 반복문에서 빠져나옵니다. */\
		while(list_##sth->current->key < key && list_##sth->current->next != NULL)\
			list_##sth->current = list_##sth->current->next;	\
	}															\
	/* 열심히 찾았는데 없을경우 실패를 반환합니다. */			\
	if (list_##sth->current->key != key)						\
		return Fail_No_Element;									\
																\
	return Success;												\
}																\
																\
/* key와 sortkey가 다를경우 사용하는 함수입니다. */				\
/* private_get_##sth의 #key = #sortKey와 작동 과정이 같습니다.*/\
int private_sort_get_##sth(sortT sortKey)						\
{																\
	list_##sth->current = list_##sth->head;						\
	while(list_##sth->current->sortKey < sortKey && list_##sth->current->next != NULL)\
		list_##sth->current = list_##sth->current->next;		\
																\
	if (list_##sth->current->sortKey != sortKey)				\
		return Fail_No_Element;									\
																\
	return Success;												\
}																\
																\
int append_##sth(Sth sth)										\
{																\
	/* 리스트에 추가할 원소를 동적할당하여 공간을 만듭니다.*/	\
	Sth * element = (Sth *)malloc(sizeof(Sth));					\
	*element = sth; 											\
																\
	if (list_##sth->head == NULL)								\
	{															\
		list_##sth->head = element;								\
		list_##sth->tail = element;								\
		return Success;											\
	}															\
    															\
	if (private_get_##sth(element->key) == Success)				\
	{/* 중복이라면 오류 반환 */									\
		return Fail_Two_Same_Value;								\
	}															\
	if (strcmp(#key, #sortKey))									\
		private_sort_get_##sth(element->sortKey);				\
	/* 현재 리스트의 current 는 삽입하기 적당한 위치 */			\
																\
	if (list_##sth->current->next == NULL && list_##sth->current->key < element->key)\
	{/* 마지막 원소일 때 */										\
		list_##sth->tail = element;								\
																\
		list_##sth->current->next = element;					\
		element->last = list_##sth->current;					\
		element->next = NULL;									\
	}															\
	else if (list_##sth->current->last == NULL)					\
	{/* 첫번째 원소일 때 */										\
		list_##sth->head = element;								\
																\
		list_##sth->current->last = element;					\
		element->next = list_##sth->current;					\
		element->last = NULL;									\
	}															\
	else														\
	{/* 중간 원소일 때 */										\
		Sth *last = list_##sth->current->last, *current = list_##sth->current;\
		/* 요소 연결 */											\
		last->next = element;									\
		element->last = last;									\
																\
		current->last = element;								\
		element->next = current;								\
	}															\
																\
    file_write_##sth();											\
																\
	return Success;												\
}																\
																\
int remove_##sth(int key)										\
{																\
	/* 원하는 원소를 얻어내지 못했다면, 해당하는 원소는 없다. 에러반환 */\
	if (private_get_##sth(key) != Success)						\
		return Fail_No_Element;									\
	/* 현재 리스트의 current는 지워야할 원소이다. */			\
																\
	if (list_##sth->current->next == NULL)						\
	{/* 마지막 원소 */											\
		list_##sth->tail = list_##sth->current->last;			\
		list_##sth->tail->next = NULL;							\
	}															\
	else if (list_##sth->current->last == NULL)					\
	{/* 첫번째 원소 */											\
		list_##sth->head = list_##sth->current->next;			\
		list_##sth->head->last = NULL;							\
	}															\
	else														\
	{/* 중간 원소 */											\
		Sth *last = list_##sth->current->last, *next = list_##sth->current->next;\
																\
		last->next = next;										\
		next->last = last;										\
	}															\
	free_all_##sth(list_##sth->current);						\
																\
	file_write_##sth();											\
																\
	return Success;												\
}																\
																\
int get_##sth(int key, const Sth** result)						\
{																\
	/* 접근성을 제한하기 위해 const를 이용합니다 */				\
	if (private_get_##sth(key) == Success)						\
	{															\
		*result = list_##sth->current;							\
		return Success;											\
	}															\
	else														\
	{															\
		*result = list_##sth->current;							\
		return Fail_No_Element;									\
	}															\
}																\
																\
int replace_##sth(const Sth* p_origin, Sth sth)					\
{																\
	/* 예외 검사 */												\
	/* 미리 p_origin 요소가 있는지, sth 자리가 있는지 */		\
	/* 확인한 후 기존의 함수를 이용해 처리합니다. */			\
	if(private_get_##sth(p_origin->key) != Success)				\
		return Fail_No_Element;									\
	if(private_get_##sth(sth.key) == Success && p_origin->key != sth.key)\
		return Fail_Two_Same_Value;								\
																\
	remove_##sth(p_origin->key);								\
	append_##sth(sth);											\
	/* 파일저장은 위 함수에서 수행한다. */						\
																\
	return Success;												\
}																

void free_all_client(Client *obj)
{
	free(obj->name);
	free(obj->password);
	free(obj->address);
	free(obj->phone_num);
	free(obj);
	return;
}

void free_all_book(Book *obj)
{
	free(obj->name);
	free(obj->publisher);
	free(obj->author);
	free(obj->owner);
	free(obj);
	return;
}

void free_all_borrow(Borrow *obj)
{
	free(obj);
	return;
}

/// 구조체의 특정 값을 검색하여 해당하는 값의 개수를 반환합니다.
/// sth, key는 CODE 매크로와 같이 넣어주면 됩니다.
/// T는 thg의 자료형을 의미합니다. thg는 비교할 값
/// compare는 T 자료형 변수 둘이 일치하는지 확인하는 함수이름이 필요합니다.
/// 둘이 일치하면 1을 반환해야 합니다.
#define GET_KEY_FROM_THING(sth, key, T, thg, compare)			\
int thg##2keys_on_##sth(int * keys, T thg)						\
{																\
	/* 반환할 값, key값의 개수 */								\
	int cnt = 0;												\
	/* 순차 탐색을 위한 current 초기화 */						\
	list_##sth->current = list_##sth->head;						\
																\
	do															\
	{															\
		if (compare(list_##sth->current->thg, thg) == 1)		\
		{														\
			if (keys != NULL)									\
				keys[cnt] = list_##sth->current->key;			\
			cnt++;												\
		}														\
	}															\
	while((list_##sth->current = list_##sth->current->next) != NULL);\
																\
	return cnt;													\
}


int strcomp(char* a, char* b){
	return (strcmp(a,b) == 0)?1:0;
}

int longcomp(long a, long b){
	return a == b;
}

int intcomp(int a, int b){
	return a == b;
}

/// 직접 접근 없이 책의 비율을 알아내는 일이 너무 어려웠기 때문에
/// 따로 함수를 만듭니다.
#define GET_RATIO_BOOKS_FROM_THG(T, thg) 								\
int get_ratio_books_from_##thg(int ratio[][2], T thg)					\
{																		\
	int keys_book[MAXBUFF], keys_ISBN[MAXBUFF];							\
	int cnt_book, i, j;													\
	long last_ISBN = 0;													\
	int ratio_cnt = 0;													\
	if ((cnt_book = thg##2keys_on_book(keys_book, thg)) != 0)			\
	{																	\
		const Book* temp = NULL;										\
																		\
		for (i = 0; i < cnt_book; i ++)									\
		{																\
			if (get_book(keys_book[i], &temp) == Success)				\
			{															\
				if (last_ISBN == temp->ISBN)							\
				   continue;											\
				ratio[ratio_cnt][1] = ISBN2keys_on_book(keys_ISBN, temp->ISBN);\
																		\
				for (j = 0, ratio[ratio_cnt][0] = 0; j < ratio[ratio_cnt][1]; j++)\
				{														\
					if (get_book(keys_book[j], &temp) == Success)		\
					{													\
						if (temp->borrow_Y_N == 'Y')					\
							ratio[ratio_cnt][0]++;						\
					}													\
				}														\
																		\
				ratio_cnt++;											\
				last_ISBN = temp->ISBN;									\
			}															\
		}																\
	}																	\
	return ratio_cnt;													\
}

/// 순서대로 Client, Book, Borrow 구조체를 대상으로 하는 코드입니다.
CODE(Client, client, sch_num, int, sch_num)
CODE(Book, book, book_num, long, ISBN)
CODE(Borrow, borrow, book_num, int, book_num)

// 필요에 따라 손쉽게 추가됩니다.
GET_KEY_FROM_THING(client, sch_num, char*, name, strcomp)
GET_KEY_FROM_THING(client, sch_num, char*, password, strcomp)

GET_KEY_FROM_THING(book, book_num, long, ISBN, longcomp)
GET_KEY_FROM_THING(book, book_num, char*, name, strcomp)
GET_KEY_FROM_THING(book, book_num, char*, author, strcomp)
GET_KEY_FROM_THING(book, book_num, char*, publisher, strcomp)

GET_KEY_FROM_THING(borrow, book_num, int, sch_num, intcomp)

GET_RATIO_BOOKS_FROM_THG(char*, author)
GET_RATIO_BOOKS_FROM_THG(char*, publisher)

