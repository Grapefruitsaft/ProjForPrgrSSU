#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manageData.h"
#include "menu.h"
#include "login.h"


int my_ID;
char my_password[50];



void Library_service()
{
	int n;
	int rtnvalue = 0;
	while(1)
	{
		printf("\n>>도서관 서비스<<\n");
		printf("1. 회원 가입\t2. 로그인\t3. 프로그램 종료\n");
		printf("번호를 선택하세요: ");

		scanf("%d", &n);
		switch (n)
		{
			case 1 : Sign_up(); break;
			case 2 : rtnvalue = Log_in(); break;
			case 3 : return;

			default : printf("잘못 입력했습니다. 다시 입력해 주세요.");
		}

		if (rtnvalue < 0)//리턴값이 음수라면 함수종료하여 프로그램 종료.
			return;

	}
}


int Member_menu()
{
	int m;
	int rtnvalue = 0;
	while(1)
	{
		printf("\n>> 회원 메뉴 <<\n");
		printf("1. 도서 검색\t\t2. 내 대여 목록\n3. 개인정보 수정\t4. 회원 탈퇴\n5. 로그아웃\t\t6. 프로그램 종료\n");
		printf("번호를 선택하세요: ");
		scanf("%d", &m);
		switch (m)
		{
			case 1 : Search_books(); break;
			case 2 : My_BB_list(); break;
			case 3 : Revise(); break;
			case 4 : rtnvalue = Sign_down(); break;
			case 5 : return 0;
			case 6 : return -2;
			default : printf("잘못 입력했습니다. 다시 입력해 주세요.");
		}

		if (rtnvalue < 0)
			return rtnvalue + 1;
	}
}






void Search_books()
{
	int o;
	while(1)
	{
		printf("\n>> 도서 검색 <<\n");
		printf("1. 도서명 검색\t\t2. 출판사 검색\n3. ISBN 검색\t\t4. 저자명 검색\n5. 전체 검색\t\t6. 이전메뉴\n");
		printf("번호를 선택하세요: ");
		scanf("%d", &o);
		while(getchar() != '\n');
		switch(o)
		{
			case 1 : S_by_title(); break;
			case 2 : S_by_publisher(); break;
			case 3 : S_by_ISBN(); break;
			case 4 : S_by_author(); break;
			case 5 : S_total_Search(); break;
			case 6 : break;

			default : printf("잘못 입력했습니다. 다시 입력해 주세요.\n\n");
		}
		if(o == 6)
		{
			break;
		}
	}
}

void S_by_title()
{
	char s_temp_c[300];//사용자가 입력하는 도서명 입력받는 배열
	printf("\n>> 도서명 검색 <<\n");
	printf("도서명을 입력하세요 : ");
	scanf("%[^\n]s", s_temp_c);
	while(getchar() != '\n');
	int keys[100];//입력받는 도서명과 일치하는 도서번호를 입력받는 배열
	int cnt, i, cy=0;
	char yn = 'Y';
	if((cnt = name2keys_on_book(keys, s_temp_c)) != 0)//입력받는 도서명과 일치하는 도서들의 갯수를 리턴하고 그 도서들의 번호를 keys배열에 저장한다.
	{
		const Book * result = NULL;
		if(get_book(keys[0], &result) == Success)//책정보를 받아옴
		{
			for(i = 0; i < cnt; i++)
			{
				get_book(keys[i], &result);
				if(result -> borrow_Y_N == 'Y')
				{
					cy++;
				}
			}

			if(cy == 0)
			{
				yn = 'N';
			}

			printf("\n>> 검색 결과 <<\n");
			printf("도서명: %s\n출판사: %s\n저자명:%s\nISBN: %ld\n소장처: %s\n",\
					s_temp_c, result -> publisher, result -> author, result -> ISBN, result -> owner);//책정보 출력
			printf("대여가능 여부 : %c(%d/%d)\n", yn, cy,cnt);
		}
		else
		{
			printf("도서 찾기 실패. \n");
		}
	}
	else
	{
		printf("해당 도서는 존재하지않습니다.\n");
	}


}

void S_by_publisher()
{
	char s_temp_c[300];//사용자가 입력하는 출판사명을 입력받는 배열
	printf("\n>> 출판사 검색 <<\n");
	printf("출판사명을 입력하세요 : ");
	scanf("%[^\n]s", s_temp_c);
	while(getchar() != '\n');

	int keys[100];//입력받은 출판사명과 일치하는 도서들의 번호를 입력하는 배열
	int cnt_r,cnt_g, i;

	long  last_ISBN = 0;
	int ratio[100][2];//대여가능한 책의 수와 소유한 도서의 갯수를 받아오는 배열
	if((cnt_r = get_ratio_books_from_publisher(ratio, s_temp_c)) != 0)//대여가능여부의 정보를 받아온다
	{
		if((cnt_g = publisher2keys_on_book(keys, s_temp_c)) != 0)//출판사명과 일치하는 도서의 책번호를 받아온다
		{
			printf("\n>> 검색 결과 <<\n");
			const Book * result = NULL;
			for(i=0; i<cnt_g; i++)
			{
				if(get_book(keys[i], &result) == Success)//책정보를 받아온다
				{
					if (result->ISBN == last_ISBN)//이미 이전에 똑같은 책이 출력되었다면 출력하지않는다.
						continue;
					printf("도서명: %s\n출판사: %s\n저자명: %s\nISBN: %ld\n소장처: %s\n대여가능 여부: %c(%d/%d)\n\n", result -> name, result -> publisher, result -> author, result -> ISBN, result -> owner, result -> borrow_Y_N, ratio[i][0], ratio[i][1]);
					last_ISBN = result->ISBN;//중복출력을 방지하기위한 변수
				}
				else
				{
					printf("Failed to get book info\n");
				}
			}
		}
		else
		{
			printf("Failed to get book list\n");
		}
	}
	else
	{
		printf("해당 출판사의 도서는 존재하지 않습니다.\n\n");
	}

}


void S_by_ISBN()
{
	int long s_temp_l;//사용자가 입력하는 ISBN을입력받는 변수
	printf("\n>> ISBN 검색 <<\n");
	printf("ISBN을 입력하세요 : ");
	scanf("%ld", &s_temp_l);
	int keys[100];//입력받은 ISBN과 일치하는 도서의 번호를 입력받는 배열
	int cnt, i, cy=0;
	long  last_ISBN = 0;
	char yn = 'Y';

	if((cnt = ISBN2keys_on_book(keys, s_temp_l)) != 0)//ISBN과 일치하는 도서의 번호를 받아온다
	{
		const Book * result = NULL;
		if(get_book(keys[0], &result) == Success)//책정보를 받아온다
		{
			for(i = 0; i< cnt; i++)
			{
				if(get_book(keys[i], &result) == Success)
				{
					if (result->ISBN == last_ISBN)//이미 이전에 똑같은 책이 출력되었다면 출력하지않는다.
						continue;

					if((result -> borrow_Y_N) == 'Y')
					{
						cy++;
					}

					if(cy == 0)
					{
						yn = 'N';
					}

					printf("\n>> 검색 결과 <<\n");
					printf("도서명: %s\n출판사: %s\n저자명:%s\nISBN: %ld\n소장처: %s\n",\
							result -> name, result -> publisher, result -> author, result -> ISBN, result -> owner);//도서정보 출력
					printf("대여가능 여부: %c(%d/%d)\n", yn, cy, cnt);
					last_ISBN = result->ISBN;//중복출력을 방지하기위한 변수
				}
				else
				{
					printf("Failed to read book info\n");
				}
			}
		}
		else
		{
			printf("일치하는 ISBN을 가진 도서가 존재하지않습니다.\n");
		}
	}

}

void S_by_author()
{
	char s_temp_c[300];//사용자가 입력하는 저자명을  저장하는 변수
	printf("\n>> 저자명 검색 <<\n");
	printf("저자명을 입력하세요 : ");
	scanf("%[^\n]s", s_temp_c);
	while(getchar() != '\n');
	int keys[100];//입력받은 저자명과 일치하는 도서의 번호를 입력받는 변수
	int cnt_r,cnt_g, i;

	long  last_ISBN = 0;
	int ratio[50][2];//대여가능여부 정보를 저장하는 배열
	if((cnt_r = get_ratio_books_from_author(ratio, s_temp_c)) != 0)//대여가능여부 정보를 저장하는 배열
	{
		if((cnt_g = author2keys_on_book(keys, s_temp_c)) != 0)
		{
			printf("\n>> 검색 결과 <<\n");
			const Book * result = NULL;
			for(i=0; i<cnt_g; i++)
			{
				if(get_book(keys[i], &result) == Success)
				{
					if (result->ISBN == last_ISBN)//이미 이전에 똑같은 책이 출력되었다면 출력하지않는다.
						continue;

					printf("도서명: %s\n출판사: %s\n저자명: %s\nISBN: %ld\n소장처: %s\n대여가능 여부: %c(%d/%d)\n\n", result -> name, result -> publisher, result -> author, result -> ISBN, result -> owner, result -> borrow_Y_N, ratio[i][0], ratio[i][1]);
					last_ISBN = result->ISBN;//중복출력을 방지하기위한 변수
				}
				else
				{
					printf("Failed to get book info\n");
				}
			}
		}
		else
		{
			printf("Failed to get book list\n");
		}
	}
	else
	{
		printf("해당 저자의 도서는 존재하지 않습니다.\n\n");
	}

}


void S_total_Search()
{
	printf("\n>> 전체 검색 <<\n");
	list_book -> current = list_book -> head;//리스트 위치를 맨 처음으로 초기화
	while(list_book -> current)//현재 주소가 NULL이 아닐때까지 반복문 실행
	{
		printf("책번호: %d\nISBN: %ld\n책이름: %s\n출판사: %s\n저자: %s\n소장처:%s\n대여가능여부: %c\n\n", list_book -> current -> book_num, list_book -> current -> ISBN, list_book -> current -> name, list_book -> current -> publisher, list_book -> current -> author, list_book -> current -> owner, list_book -> current -> borrow_Y_N);

		list_book -> current = list_book -> current -> next;//다음 리스트 주소를 현재 리스트에 대입

	}

}


void My_BB_list()
{
	int my_sch_num = my_ID;//사용자의 학번을 저장하는 변수
	int keys[100];//대여한 책의 번호를 저장하는 배열
	int cnt, i;
	const Book *  Binfo = NULL;//받아온 책정보의 주소값을 저장하는 포인터 변수
	const Borrow * result = NULL;//받아온 대여정보의 주소값을 저장하는 포인터 변수
	struct tm *bt;//리턴된 구조체로 변환된 대여일자값을 저장하는 tm구조체 변수
	struct tm *rt;//리턴된 구조체로 변환된 반납일자값을 저장하는 tm구조체 변수
	char day[7][15] = {"일요일", "월요일", "화요일", "수요일", "목요일", "금요일", "토요일"};

	printf("\n>>내 대여 목록 <<\n");

	if((cnt = (sch_num2keys_on_borrow(keys, my_sch_num))) != 0)//사용자가 대여한 도서들의 번호를 저장한다
	{
		printf("\n>> 회원의 대여 목록 <<\n");

		for(i = 0; i < cnt; i++)
		{
			if(get_borrow(keys[i], &result) == Success)
			{
				const time_t BT = result -> borrow_day;//대여일자의 값을 받는 변수
				const time_t RT = result -> return_day;//반납일자의 값을 받는 변수
				bt = localtime(&BT);//time_t 변수를 tm구조체로 변환한다
				if(get_book(keys[i], &Binfo) == Success)//책 정보를 받아온다
				{
					printf("도서번호: %d\n도서명: %s\n", result  -> book_num,Binfo -> name);
					printf("대여일자: %d년 %d월 %d일 %s\n", bt -> tm_year  + 1900, bt -> tm_mon + 1, bt -> tm_mday, day[bt -> tm_wday]);
					rt = localtime(&RT);
					printf("반납일자: %d년 %d월 %d일 %s\n\n",rt -> tm_year + 1900, rt -> tm_mon + 1, rt -> tm_mday, day[rt -> tm_wday]);//대여정보 출력
				}
				else
				{
					printf("Failed to read book info\n");
				}
			}


			else
			{
				printf("Failed to read borrow list\n");

			}
		}
	}
	else
	{
		printf("대여 목록이 존재하지 않습니다.\n");
	}
}

int Admin_menu()
{
	int o;
	while(1)
	{
		printf(">> 관리자  메뉴 <<\n");
		printf("1. 도서 등록\t2. 도서 삭제\n3. 도서 대여\t4. 도서 반납\n5. 도서 검색\t6. 회원 목록\n7. 로그아웃\t8. 프로그램 종료\n");
		printf("번호를 선택하세요: ");
		scanf("%d", &o);

		switch(o)
		{
			case 1 : Register_book(); break;
			case 2 : Delete_book(); break;
			case 3 : Lend_book(); break;
			case 4 : Return_book(); break;
			case 5 : Search_books(); break;
			case 6 : Member_list(); break;
			case 7 : return 0;
			case 8 : return -2;
			default : printf("잘못 입력했습니다. 다시 입력해 주세요.\n");
		}

	}
}

void Register_book()
{
	Book *Btemp = NULL;//저장될 책의 정보를 받는 구조체 변수를 가르킬 포인터 변수
	Book add;//저장될 책의 정보를 받는 구조체 변수
	Btemp = &add;
	int Y_N = '0';

	char input[500];
	getchar();
	printf("도서명: ");
	scanf("%[^\n]s", input);
	Btemp -> name = (char *)malloc(sizeof(char) * (strlen(input) + 1));
	strcpy(Btemp -> name, input);
	while(getchar() != '\n');

	printf("출판사: ");
	scanf("%[^\n]s", input);
	Btemp -> publisher = (char *)malloc(sizeof(char) * (strlen(input) +1));
	strcpy(Btemp -> publisher, input);
	while(getchar() != '\n');

	printf("저자명: ");
	scanf("%[^\n]s", input);
	Btemp -> author = (char *)malloc(sizeof(char) * (strlen(input)+1));
	strcpy(Btemp -> author, input);
	while(getchar() != '\n');

	printf("ISBN: ");
	scanf("%ld", &add.ISBN);
	while(getchar() != '\n');

	printf("소장처: ");
	scanf("%[^\n]s", input);
	Btemp -> owner = (char *)malloc(sizeof(char) * (strlen(input) +1));
	strcpy(Btemp -> owner, input);
	while(getchar() != '\n');

	add.borrow_Y_N = 'Y';


	printf("등록하시겠습니까? (등록하려면 Y입력) ");
	Y_N = getchar();
	if(Y_N == 'Y')
	{
		add.book_num = ++(list_book -> last_book_num);//마지막에 저장된 도서번호+1 값을 대입
		add.borrow_Y_N = 'Y';
		if(append_book(add) == Success)//책정보를 링크드리스트에 추가 
		{
			file_write_book();
			printf("도서등록이 완료되었습니다.\n");

		}
		else
		{
			printf("도서등록을 실패하였습니다.\n");
			add.book_num = --(list_book -> last_book_num);//실패하였으므로 마지막에 저장된 도서번호를 초기화
		}
	}
	else
	{
		printf("Failed to accept\n");
	}
}




void Delete_book()
{
	int p;
	printf("\n>> 도서 삭제 <<\n");
	printf("1. 도서명 검색\t\t2. ISBN 검색\n");
	printf("검색 번호를 입력하세요: ");
	scanf("%d", &p);

	switch(p)
	{
		case 1 : D_by_title(); break;
		case 2 : D_by_ISBN(); break;

		default : printf("잘못 입력했습니다. 다시 입력해 주세요.");
	}

}

void D_by_title()
{
	char d_temp_c[300];//삭제할 도서명을 입력받는 변수
	printf("도서명을 입력하세요 : ");
	while(getchar() != '\n');
	scanf("%[^\n]s", d_temp_c);
	int keys[100];//입력받은 도서명과 일치하는 도서들의 번호를 입력받는 배열
	int cnt;
	if((cnt = name2keys_on_book(keys, d_temp_c)) != 0)
	{
		delete_book(keys, cnt);//도서를 삭제하는 함수
		file_write_book();
	}
	else
	{
		printf("존재하지 않는 도서명입니다.\n");

	}	
}

void D_by_ISBN()
{
	int long d_temp_l;//삭제할 도서의 ISBN값을 입력받는 변수
	printf("ISBN 값을 입력하세요 : ");
	scanf("%ld", &d_temp_l);
	int keys[100];//입력받은 ISBN값과 일치하는 도서들의 번호를 입력받는 배열
	int cnt;

	if((cnt = ISBN2keys_on_book(keys, d_temp_l)) != 0)//입력받은 ISBN값과 일치하는 도서들의 번호를 입력받는다
	{
		delete_book(keys, cnt);//도서를 삭제하는 함수
		file_write_book();
	}
	else
	{
		printf("존재하지 않는 도서명입니다.\n");
	}
}	



void delete_book(int *keys, int cnt)
{
	const Book * result = NULL;//받아온 도서의 주소를 가르키는 포인터 변수
	int innum;//삭제할 도서의 번호를 입력받는 변수
	int i;
	if(get_book(keys[0], &result) == Success)//도서정보를 받아온다
	{
		printf("\n>> 검색 결과 <<\n");
		printf("도서번호: ");
		printf("%d(삭제 가능 여부 : %c)", result -> book_num, result -> borrow_Y_N);
		for(i = 1; i < cnt; i++)
		{
			if(get_book(keys[i], &result) == Success)//도서정보를 받아온다
			{
				{
					printf(", %d(삭제 가능 여부 : %c)", result -> book_num, result -> borrow_Y_N);
				}
			}
			else
			{
				printf("책정보를 받아오는데 실패하였습니다.\n");
			}
		}
	}
	printf("도서명: %s\n출판사: %s\n저자명: %s\nISBN: %ld\n소장처: %s\n", result  -> name, result -> publisher, result -> author, result -> ISBN, result -> owner);

	printf("삭제할 도서의 번호를 입력하세요: ");
	scanf("%d", &innum);
	if(get_book(innum, &result) == Success)//도서정보를 받아온다
	{
		if((result -> borrow_Y_N) == 'Y')
		{
			remove_book(innum);
			printf("도서가 삭제되었습니다.\n");
		}
		else if((result -> borrow_Y_N) == 'N')
		{
			printf("이 도서는 삭제할 수 없습니다.\n");
		}
	}
	else
	{
		printf("책정보를 받아오는데 실패하였습니다.\n");
	}
}


void Lend_book()
{
	int p;
	printf("\n>> 도서 대여 <<\n");
	printf("1. 도서명 검색\t\t2. ISBN 검색\n");
	printf("번호를 선택하세요: ");
	while(getchar() != '\n');
	scanf("%d", &p);

	switch (p)
	{
		case 1 : L_by_title(); break;
		case 2 : L_by_ISBN(); break;
		default : printf("잘못 입력했습니다. 다시 입력해 주세요.");
	}
}

void L_by_title()
{
	char l_temp_c[300];//대여할 도서명을 입력받는 변수
	int sch_id, book_n;
	char YorN;
	while(getchar() != '\n');
	printf("도서명을 입력하세요: ");
	scanf("%[^\n]s", l_temp_c);
	while(getchar() != '\n');
	int keys[100];//대여할 도서명과 일치하는 도서들의 번호를 입력받는 배열
	int cnt, i;
	if((cnt = name2keys_on_book(keys, l_temp_c)) != 0)//대여할 도서명과 일치하는 도서들의 번호를 입력받는다
	{
		const Book * result = NULL;
		Borrow btemp;//대여정보를  저장할 구조체 변수
		Borrow * binput = &btemp;
		if(get_book(keys[0], &result) == Success)//도서정보를 받아온다
		{
			printf("\n>>검색 결과 <<\n");
			for(i = 0; i<cnt; i++)
			{
				if(get_book(keys[i], &result) == Success)
				{
					printf("도서번호: %d(대여가능 여부 : %c)\n도서명: %s\n출판사: %s\n저자명: %s\nISBN: %ld\n소장처: %s\n\n", \
							result -> book_num, result -> borrow_Y_N, result -> name, \
							result -> publisher, result -> author, result -> ISBN, result -> owner);
				}
			}
			printf("학번을 입력하세요: ");
			scanf("%d", &sch_id);
			binput -> sch_num = sch_id;

			printf("도서번호를 입력하세요: ");
			scanf("%d", &book_n);
			binput -> book_num = book_n;

			while(getchar() != '\n');

			printf("이 도서를 대여합니까? ");
			scanf("%c", &YorN);

			if(YorN == 'Y')
			{
				struct tm *rt;//반납일자가 일요일인지 체크하기위한 포인터 변수
				time_t bn;//대여일자값을 저장하는 변수
				time_t rn;//반납일자값을 저장하는 변수

				bn = time(NULL);//현재시간을 받아온다

				rn = bn + (30*24*60*60);//현재시간 +30일 값을 저장
				rt = localtime(&rn);

				binput -> borrow_day = bn;
				binput -> return_day = rn;

				if((rt -> tm_wday) == 0)//반납일자가 일요일인 경우
				{
					rn = rn + (1 * 24 * 60 * 60);
					binput -> return_day = rn;
				}

				if(get_book(book_n, &result) == Success)
				{

					Book modi;//대여 후 대여여부(Y -> N)를 수정하기 위해 대여할 도서의 정보를 받는 구조체 변수


					modi.book_num = result -> book_num;

					modi.name = (char *)malloc(sizeof(char) * (strlen(result -> name) +1));
					strcpy(modi.name, result -> name);

					modi.publisher = (char *)malloc(sizeof(char) * (strlen(result -> publisher) +1));
					strcpy(modi.publisher, result -> publisher);

					modi.author = (char *)malloc(sizeof(char) * (strlen(result -> author) +1));
					strcpy(modi.author, result -> author);

					modi.ISBN = result  -> ISBN;
					modi.owner = (char *)malloc(sizeof(char) * (strlen(result -> owner) +1));
					strcpy(modi.owner, result -> owner);

					modi.borrow_Y_N = 'N';//대여상태를 N으로 변경
					Return_Flags flag;

					if((flag = replace_book(result, modi)) == Success)
					{
						if(append_borrow(btemp) == Success)
						{
							printf("도서가 대여 되었습니다.\n");
							file_write_book();
							file_write_borrow();
						}
						else
						{
							printf("도서 대여를 실패하였습니다.(Same_Two_Same_Value)\n");	
						}

					}
					else if(flag == Fail_Two_Same_Value)
					{
						printf("Fail. there is same value\n");
					}
					else  if(flag == Fail_No_Element)
					{
						printf("There is no element\n");
					}
				}
				else
				{
					printf("failed finding book.\n");
				}
			}
			else
			{
				printf("도서대여가 취소되었습니다.\n");
			}
		}
	}
	else
	{
		printf("해당 도서는 존재하지 않습니다.\n");
	}
}

void L_by_ISBN()
{
	int sch_id, book_n;
	int YorN;
	long int l_temp_l;//대여할 도서의 ISBN값을 입력받는 변수
	int keys[100];//입력받은 ISBN값과 일치하는 도서들의 번호를 저장하는 배열
	int cnt, i;
	Borrow btemp;//대여정보를  저장할 구조체 변수
	Borrow *binput = &btemp;
	const Book * result = NULL;

	printf("ISBN을 입력하세요: ");
	scanf("%ld", &l_temp_l);
	if((cnt = ISBN2keys_on_book(keys, l_temp_l)) == Success)//입력받은 ISBN값과 일치하는 도서들의 정보를 저장한다
	{
		printf("\n>> 검색 결과 <<\n");
		for(i = 0; i < cnt; i++)
		{
			if(get_book(keys[i], &result) == Success)//도서 정보를 받아온다
			{
				printf("도서번호: %d(대여가능 여부 : %c)\n도서명: %s\n출판사: %s\n저자명: %s\nISBN: %ld\n소장처: %s\n\n", \
						result -> book_num, result -> borrow_Y_N, result -> name, \
						result -> publisher, result -> author, result -> ISBN, result -> owner);
			}
			else
			{
				printf("Failed to read book list\n");

			}
		}
		printf("학번을 입력하세요: ");
		scanf("%d", &sch_id);
		binput -> sch_num = sch_id;

		printf("도서번호를 입력하세요: ");
		scanf("%d", &book_n);
		binput -> book_num = book_n;

		while(getchar() != '\n');
		printf("이 도서를 대여합니까? ");
		YorN = getchar();
		if(YorN == 'Y')
		{
			struct tm *rt;//반납일자가 일요일인지 체크하기위한 포인터 변수
			time_t bn;//대여일자값을 저장하는 변수
			time_t rn;//반납일자값을 저장하는 변수


			bn = time(NULL);//현재시간을 저장한다

			rn = bn + (30*24*60*60);//현재시간 + 30일 값으로 저장
			rt = localtime(&rn);

			binput -> borrow_day = bn;
			binput -> return_day = rn;

			if((rt -> tm_wday) == 0)//반납일자가 일요일인경우
			{
				rn = rn + (1 * 24 * 60 * 60);
				binput -> return_day = rn;
			}

			if(get_book(book_n, &result) == Success)//도서정보를 불러온다
			{

				Book modi;

				modi.book_num = result -> book_num;

				modi.name = (char *)malloc(sizeof(char) * (strlen(result -> name) +1));
				strcpy(modi.name, result -> name);

				modi.publisher = (char *)malloc(sizeof(char) * (strlen(result -> publisher) +1));
				strcpy(modi.publisher, result -> publisher);

				modi.author = (char *)malloc(sizeof(char) * (strlen(result -> author) +1));
				strcpy(modi.author, result -> author);

				modi.ISBN = result  -> ISBN;
				modi.owner = (char *)malloc(sizeof(char) * (strlen(result -> owner) +1));
				strcpy(modi.owner, result -> owner);

				modi.borrow_Y_N = 'N';
				Return_Flags flag;

				if((flag = replace_book(result, modi)) == Success)
				{
					if(append_borrow(btemp) == Success)
					{
						printf("도서가 대여 되었습니다.\n");
						file_write_book();
						file_write_borrow();
					}
					else
					{
						printf("도서 대여를 실패하였습니다.(Same_Two_Same_Value)\n");	
					}

				}
				else if(flag == Fail_Two_Same_Value)
				{
					printf("Fail. there is same value\n");
				}
				else  if(flag == Fail_No_Element)
				{
					printf("There is no element\n");
				}
			}
			else
			{
				printf("등록되지 않은 도서입니다.\n");
			}
		}
		else
		{
			printf("도서대여가 취소되었습니다.\n");
		}
	}
	else
	{
		printf("failed to convert\n");
	}
}



void Return_book()
{
	int r_temp_i;//도서를 반납하는 사용자의 학번을 입력받는 변수
	int keys[100];//입력받은 학번과 일치하는 대여정보의 도서번호 저장하는 벼을
	int cnt, i;
	char day[7][15] = {"일요일", "월요일", "화요일", "수요일", "목요일", "금요일", "토요일"};
	const Borrow *result = NULL;
	const Book * Binfo = NULL;
	struct tm *bt;
	struct tm *rt;
	int book_n;
	char YorN;

	printf("\n>> 도서 반납 <<\n");
	while(getchar() != '\n');
	printf("학번을 입력하세요 : ");
	scanf("%d", &r_temp_i);

	if((cnt = sch_num2keys_on_borrow(keys, r_temp_i)) != 0)//입력받은  학번과 일치하는 도서번호를 저장한다.
	{
		printf("\n>> 회원의 대여 목록 <<\n");
		for(i = 0; i < cnt; i++)
		{
			if(get_borrow(keys[i], &result) == Success)//대여정보를 받아온다.
			{
				const time_t BT = result -> borrow_day;
				const time_t RT = result -> return_day;
				bt = localtime(&BT);

				if(get_book(keys[i], &Binfo) == Success)//대여정보에 해당하는 책정보를 받는다.
				{
					printf("도서번호: %d\n도서명: %s\n", result  -> book_num,Binfo -> name);
					printf("대여일자: %d년 %d월 %d일 %s\n", bt -> tm_year  + 1900, bt -> tm_mon + 1, bt -> tm_mday, day[bt -> tm_wday]);
					rt = localtime(&RT);
					printf("반납일자: %d년 %d월 %d일 %s\n\n",rt -> tm_year + 1900, rt -> tm_mon + 1, rt -> tm_mday, day[rt -> tm_wday]);

				}
				else
				{
					printf("failed to read  book info");
				}
			}
			else
			{
				printf("failed to read book list");
			}
		}

		printf("반납할 도서번호를 입력하세요: ");
		scanf("%d", &book_n);

		getchar();
		printf("도서 반납처리를 할까요? ");
		scanf("%c", &YorN);


		if(YorN == 'Y')
		{

			const Book *itemp = NULL;
			if(get_book(book_n, &itemp) == Success)//대여정보에 일치하는 책 정보를 불러온다.
			{
				Book pick;//대여정보를 변경하기위해 구조체에 책정보를 옮겨쓴다.

				pick.book_num = itemp -> book_num;

				pick.ISBN = itemp -> ISBN;

				pick.name = (char *)malloc(sizeof(char) * (strlen(itemp -> name) +1));
				strcpy(pick.name, itemp -> name);

				pick.publisher = (char *)malloc(sizeof(char) * (strlen(itemp -> publisher) +1));
				strcpy(pick.publisher, itemp -> publisher);

				pick.author = (char *)malloc(sizeof(char) * (strlen(itemp -> author) +1));
				strcpy(pick.author, itemp -> author);

				pick.owner = (char *)malloc(sizeof(char) * (strlen(itemp -> owner) +1));
				strcpy(pick.owner, itemp -> owner);

				pick.borrow_Y_N = 'Y';//대여정보 수정

				Return_Flags flag;

				if((flag = replace_book(itemp, pick)) == Success)//책정보를 수정한다
				{
					if(remove_borrow(book_n) == Success)
					{
						printf("반납이 완료되었습니다.\n");
						file_write_book();
						file_write_borrow();

					}
					else
					{
						printf("반납 실패 (Fail_No_element)\n");
					}

				}
				else if(flag == Fail_Two_Same_Value)
				{
					printf("Fail. there is same value\n");
				}
				else  if(flag == Fail_No_Element)
				{
					printf("There is no element\n");
				}
			}
		}
		else
		{
			printf("반납을 취소하였습니다.\n");
		}
	}
	else
	{
		printf("failed to convert\n");
	}
}

void Member_list()
{
	int escape = 0;
	while(1)
	{
		int  p;
		printf("\n>> 회원 목록<<\n");
		printf("1. 이름 검색\t2. 학번 검색\n3. 전체 검색\t4. 이전 메뉴\n");
		printf("번호를 선택하세요: ");
		scanf("%d", &p);

		switch (p)
		{
			case 1 : Search_name(); break;
			case 2 : Search_ID(); break;
			case 3 : M_total_search(); break;
			case 4 : escape = 1; break; 

			default : printf("잘못 입력했습니다. 다시 입력해 주세요.\n\n");
		}
		if(escape == 1)
		{
			break;
		}
	}
}

void Search_name()
{
	char s_temp_c[500];
	while(getchar() != '\n');
	printf(">> 이름 검색 <<\n");
	printf("이름을 입력하세요 : ");
	scanf("%[^\n]s", s_temp_c);
	while(getchar() != '\n');
	int keys[20];
	int cnt, i;
	if((cnt = name2keys_on_client(keys, s_temp_c)) != 0)//입력받은 이름과 일치하는 회원의 학번을 저장한다
	{
		const Client * result = NULL;
		printf("\n>> 회원 목록 <<\n\n");
		for(i = 0; i < cnt; i++)
			if(get_client(keys[i], &result) == Success)//일치하는 회원들의 정보를 출력한다.
			{
				printf("학번: %d\n이름: %s\n주소: %s\n전화번호: %s\n\n", result -> sch_num, result -> name, result -> address, result -> phone_num);
			}
			else
			{
				printf("Failed to get client info\n");
			}
	}
	else
	{
		printf("존재하지 않는 이름입니다.\n\n");
	}

}



void Search_ID()
{
	int s_temp_i;
	while(getchar() != '\n');
	printf("학번을 입력하세요 : ");
	scanf("%d", &s_temp_i);
	const Client * picker = NULL;
	if(get_client(s_temp_i, &picker) == Success)//학번과 일치하는 회원정보를 받는다.
	{
		printf("\n학번: %d\n이름: %s\n주소: %s\n전화번호: %s\n\n", picker -> sch_num, picker -> name, picker -> address, picker -> phone_num);

	}	
	else
	{
		printf("회원이 존재하지 않습니다.\n");
	}
}


void M_total_search()
{
	list_client -> current = list_client -> head;//리스트 현재 위치를 head로 변경
	while(list_client->current)//current -> next가 NULL일때까지 반복문을 실행한다.
	{
		printf("학번: %d\n이름: %s\n주소: %s\n전화번호: %s\n\n", list_client -> current -> sch_num, list_client -> current -> name, list_client -> current -> address, list_client -> current -> phone_num);

		list_client -> current = list_client -> current -> next;//현재 위치를 current -> next로 변경
	}
}




int start_proc()
{
	Library_service();
	return 0;
}
