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

/// 유지보수의 편의성을 위해서 매크로를 이용합니다.
/// 제너릭 인터페이스가 쓰고싶었던 자의 몸부림입니다.
/// set tapstop=4 에 최적화 된 코드입니다.
#define CODE(Sth, sth, key) 									\
/* 이 c 파일에서만 사용되는 get함수입니다. 성공을 확인한 후 리스트의 current*/\
/* 가져다 이용하시면 됩니다.												*/\
int private_get_##sth(int key)									\
{																\
	/* 리스트의 current 초기화 */								\
	list_##sth->current = list_##sth->head;						\
	/* 리스트가 오름차순으로 정렬 되어 있으므로 자신보다 작은 값을 가진 원소까지 */\
	/* 검사하면 충분합니다. key값이 같거나 클 경우 반복문에서 빠져나옵니다. */\
	while(list_##sth->current->key < key && list_##sth->current->next != NULL)\
		list_##sth->current = list_##sth->current->next;		\
																\
	/* 열심히 찾았는데 없을경우 실패를 반환합니다. */			\
	if (list_##sth->current->key != key)						\
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
	}/* 현재 리스트의 current 는 삽입하기 적당한 위치 */		\
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
	free(list_##sth->current);									\
																\
	file_write_##sth();											\
																\
	return Success;												\
}																\
																\
int get_##sth(int key, const Sth** result)						\
{																\
	/* 접근성을 제한하기 위해 const에 할당 후 그 주소를 이용합니다 */\
	if (private_get_##sth(key) == Success)						\
	{															\
		*result = list_##sth->current;							\
		return Success;											\
	}															\
	else														\
		return Fail_No_Element;									\
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

/// 구조체의 특정 값을 검색하여 해당하는 값의 개수를 반환합니다.
/// sth, key는 CODE 매크로와 같이 넣어주면 됩니다.
/// T는 thg의 자료형을 의미합니다. thg는 비교할 값
/// same는 T 자료형 변수 둘이 일치하는지 확인하는 함수이름이 필요합니다.
/// 둘이 일치하면 1을 반환해야 합니다.
#define GET_KEY_FROM_THING(sth, key, T, thg, compare)			\
int thg##2##keys_on_##sth(int ** keys, T thg)					\
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
			*keys[cnt] = list_##sth->current->key;				\
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

/// 순서대로 Client, Book, Borrow 구조체를 대상으로 하는 코드입니다.
CODE(Client, client, sch_num)
CODE(Book, book, book_num)
CODE(Borrow, borrow, book_num)

GET_KEY_FROM_THING(book, book_num, long, ISBN, longcomp)
GET_KEY_FROM_THING(book, book_num, char*, name, strcomp)
GET_KEY_FROM_THING(book, book_num, char*, author, strcomp)
GET_KEY_FROM_THING(book, book_num, char*, publisher, strcomp)
GET_KEY_FROM_THING(borrow, book_num, int, sch_num, intcomp)

#ifdef DEBUG
int main(void)
{
	int i = 0;
	int j = 0;
	int key = 0;
	const Client* t = NULL;

	init_all_list();
	get_all_file_data();

	while(1)
	{
		printf("\n디버깅 모드입니다.\n");
		printf("1. 리스트 추가\n2. 리스트 제거\n3. 리스트 얻기\n4. 리스트 대체\n5. 현재 리스트 출력\n");
		scanf("%d",&i);

		switch(i)
		{
			case 1:
				printf("추가할 key 입력 : ");
				scanf("%d", &key);
				Client c = {key,0};
				if (append_client(c) == Success)
					printf("%d 추가 완료\n",key);
				else
					printf("추가 실패\n");
				break;
			case 2:
				printf("제거할 key 입력 : ");
				scanf("%d", &key);
				if (remove_client(key) == Success)
					printf("%d 제거 완료\n", key);
				else
					printf("제거 실패\n");
				break;
			case 3:
				printf("얻을 key 입력 : ");
				scanf("%d", &key);
				if (get_client(key,&t) == Success)
				{
					printf("%d\n",t->sch_num);
				}
				else
					printf("얻기 실패\n");
				break;
			case 4:
				printf("원래 key 바꿀 key : ");
				scanf("%d %d",&key,&j);
				Client f = {j,0};
				if (get_client(key, &t) == Success)
				{
					if(replace_client(t,f) == Success)
						printf("바꾸기 성공\n");
					else
						printf("바꾸기 실패\n");
				}
				else
					printf("얻기 실패\n");
				break;
			case 5:
				printf("리스트 출력\n");
				list_client->current = list_client->head;
				while(1)
				{
					printf("%d\n",list_client->current->sch_num);
					if (list_client->current->next == NULL)
						break;
					list_client->current = list_client->current->next;
				}
				break;
		}
	}

	return 0;
}
#endif
