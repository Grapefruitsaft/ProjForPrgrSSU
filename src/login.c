#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"menu.h"
#include"login.h"

static const char* ADMIN_ID = "admin";//log_in 함수에서 admin으로 로그인 받을경우를 위해
static const char* ADMIN_PWD = "lib_admin7";//마찬가지

extern int my_ID;//log_in함수 입력 버퍼
extern char my_password[50];//마찬가지

int Sign_down(void){
	if (sch_num2keys_on_borrow(NULL, my_ID) == 0)//sch_num2keys_on_borrow는 my_ID를 통해 my_ID와 일치하는 원소의 갯수를 리턴한다- NULL자리의 포인터 에는 해당하는 값을 넘겨주는데 null 이므로 1을 리턴 따라서 else로 가게된다 my_id는 log_in 함수에서 입력받는다
	{                                 
		remove_client(my_ID);//my_ID가 포함된 클라이언트 구조체 삭제
		printf("\n회원 탈퇴가 성공적으로 실행되었습니다.\n다시 로그인 해주십시오.\n");
		return -1;
	}
	else
	{
		printf("\n대여한 도서가 있어, 탈퇴가 불가능합니다.\n");
		return 1;
	}
}


void Sign_up(void)
{
	Client client,client_temp;//동적 할당을 받기위해 client 구조체와 temp 선언
	const Client *compare = &client_temp;// get_client 함수는 학번을 입력받아 이와 일치하는 원소가 있는 client 구조체의 포인터를 넘겨준다 이때 입력을 const로 받기때문이기도 하고 리스트에있는 내용을 변경하면 안되기에 const 형 선언.
	char temp[500]; //학번, 비밀번호 등 입력 버퍼  , \n입력 버그 막을려고                  

	printf("\n>>회원 가입<<\n");
	printf("학번, 비밀번호, 이름, 주소, 전화번호를 입력하세요\n");
	printf("학번: ");
	getchar();
	scanf("%[^\n]",temp);
	while(getchar() != '\n');//입력 버퍼의 \n 제거
	
	if (get_client(atoi(temp),&compare) == Success)// 앞서 말했다시피 getclient 함수를 써서 compare에 리스트포인터를 받아온다
	{
		printf("중복된 학번입니다");// 성공하면 중복된게 있다는 말이므로 false
		return;
	}
	else
	{
		client.sch_num = atoi(temp);// 아니면 sch_num이 리스트에서 int형이므로 atoi
	}
	
	printf("이름: ");
	scanf("%[^\n]",temp);// 공백 문자 또한 읽어들임
	getchar();// 입력버퍼에 \n 제거
	client.name = (char *)malloc(sizeof(char) * (strlen(temp) + 1));//동적 배열 할당
	strcpy(client.name,temp);// 할당된 포인터에 값을 넘겨준다
	
	printf("비밀번호: ");// 반복
	scanf("%[^\n]",temp);
	while(getchar() != '\n');
	client.password = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
	strcpy(client.password,temp);
	
	printf("주소: ");
	scanf("%[^\n]",temp);
	while(getchar() != '\n');
	client.address = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
	strcpy(client.address,temp);

	printf("전화번호: ");
	scanf("%[^\n]",temp);
	client.phone_num = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
	strcpy(client.phone_num,temp);
	if(client.phone_num == '\0' || client.address == '\0' || client.password == '\0' || client.name == '\0' || client.sch_num == '\0'){// \n 입력받았을 경우 버그해결
		printf("입력안된게 있습니다 다시 입력하세요");
		return ;
	}

	if((append_client(client) == Success))//append 함수는 구조체를 입력받아 리스트 에 연결해줌
	{
		printf("회원가입 되셨습니다.\n");
	}

	return ;
}


void Revise(void)
{
	int flag;
	printf("비밀번호, 주소, 전화번호 중 어떤것을 바꾸고 싶으십니까?\n");
	printf("비밀번호 = 1\n");
	printf("주소 = 2\n");
	printf("전화번호 = 3\n");
	scanf("%d",&flag);// switch 에 쓰일 값을 받음
	const Client *result = NULL;// 앞서 했다시피 get_client 쓸꺼임
	Client temp,temp2; // \n 입력받을시의 오류해결위해 temp를 두개나 선언
	char buff[500] = {0},sub_buff[500] = {0},nu_ll[2] = {0};// 마찬가지
	while(1){
	switch(flag){
		case 1 :
			if(get_client(my_ID,&result) == Success)//log_in에서 입력받은 my_id가 실제 리스트에 있는지 확인 
			{
				printf("바꿀 패스워드?");
				while(getchar() != '\n');// \n 입력받을시 오류 해결위해
				scanf("%[^\n]",buff);// 공백 문자 입력받음
				getchar();//마찬가지
				strcpy(sub_buff,result -> password);//링크드 리스트에 구조체가 없을 경우의 오류 해결을 위해 변경되기 전의 내용을 저장한다
				temp.sch_num = result->sch_num;// sch_num은 안바꿀꺼므로
				temp.name = (char *)malloc(sizeof(char) * (strlen(result->name) + 1));
				strcpy(temp.name ,result->name);
			
				temp.password = (char *)malloc(sizeof(char) * (strlen(buff) + 1));//바꿀 내용을 동적 배열로 할당받는다
				//replace 함수가 원래구조체를 변경된 구조체로 바꾸는 거므로 동적 할당을 각각 다시 해줘야됨
				strcpy(temp.password,buff);// 내용 변경
				temp.address = (char *)malloc(sizeof(char) * (strlen(result->address) + 1));
				strcpy(temp.address ,result->address);

				temp.phone_num = (char *)malloc(sizeof(char) * (strlen(result->phone_num) + 1));
				strcpy(temp.phone_num , result->phone_num);
				if(!strncmp(buff,nu_ll,1))//입력받은 게 \n 일경우
				{
					printf("입력은 한개이상입니다\n");
					return; 
				}
				if((flag = replace_client(result,temp)) == Success)// 정상적으로 변경 됬을경우
				{
					printf("변경 되었습니다.\n");
					return ;
				}
				else if (flag == Fail_No_Element)// client 구조체에 해당하는 구조체가 없을 경우 즉 result와 일치하는 구조체가 링크드 리스트에 없을경우 
				{
					printf("해당하는 구성요소가 없습니다\n");//변경된 요소 복원 
					temp2.address = (char *)malloc(sizeof(char) * (strlen(temp.address)+1));
					strcpy(temp2.address,temp.address);
					temp2.password = (char *)malloc(sizeof(char) * (strlen(sub_buff)+1));
					strcpy(temp2.password,sub_buff);
					temp2.sch_num = temp.sch_num;
					temp2.name = (char *)malloc(sizeof(char) * (strlen(temp.name)+1));
					strcpy(temp2.name,temp.name);
					temp2.phone_num = (char *)malloc(sizeof(char) * (strlen(temp.phone_num)+1));
					strcpy(temp2.phone_num,temp.phone_num);
					flag = replace_client(&temp,temp2);
					return;
				}
				
			}
			else printf("잘못된 학번입니다.\n");// get_client 함수로 학번과 일치하는 원소를 가진 client 구조체를 못받았을 경우
			break;
		case 2 :
			if(get_client(my_ID,&result) == Success)// case 1과 작동원리가 같음 변경되는 값만 다름
			{
				printf("바꿀 주소?");
				while(getchar() != '\n');
				scanf("%[^\n]",buff);
				getchar();
				strcpy(sub_buff,result -> address);
				temp.sch_num = result->sch_num;
				temp.password = (char *)malloc(sizeof(char) * (strlen(result->password) + 1));
				strcpy(temp.password,result->password);
				temp.address = (char *)malloc(sizeof(char) * (strlen(buff)+1));
				strcpy(temp.address,buff);
				temp.phone_num = (char *)malloc(sizeof(char) * (strlen(result->phone_num) + 1));
				strcpy(temp.phone_num ,result->phone_num);
				temp.name = (char *)malloc(sizeof(char) * (strlen(result->name) + 1));
				strcpy(temp.name, result->name);
				if(!strncmp(buff,nu_ll,1))
				{
					printf("입력은 한개이상입니다\n");
					return; 
				}
				if((flag = replace_client(result,temp)) == Success)
				{
					printf("변경 되었습니다.\n");
					return ;
				}
				else if (flag == Fail_No_Element)
				{
					printf("해당하는 구성요소가 없습니다.\n");
					temp2.address = (char *)malloc(sizeof(char) * (strlen(sub_buff)+1));
					strcpy(temp2.address,sub_buff);
					temp2.password = (char *)malloc(sizeof(char) * (strlen(temp.password)+1));
					strcpy(temp2.password,temp.password);
					temp2.sch_num = temp.sch_num;
					temp2.name = (char *)malloc(sizeof(char) * (strlen(temp.name)+1));
					strcpy(temp2.name,temp.name);
					temp2.phone_num = (char *)malloc(sizeof(char) * (strlen(temp.phone_num)+1));
					strcpy(temp2.phone_num,temp.phone_num);
					flag = replace_client(&temp,temp2);
					return ;
				}
			}
			else printf("잘못된 학번입니다.\n");
			break;
		case 3 :
			if(get_client(my_ID,&result) == Success)// 마찬가지
			{
				printf("바꿀 전화번호?");
				while(getchar() != '\n');
				scanf("%[^\n]",buff);
				getchar();
				strcpy(sub_buff,result -> phone_num);
				temp.sch_num = result->sch_num;
				temp.password = (char *)malloc(sizeof(char) * (strlen(result->password) + 1));
				strcpy(temp.password , result->password);
				temp.address = (char *)malloc(sizeof(char) * (strlen(result->address) + 1));
				strcpy(temp.address, result->address);
				temp.phone_num = (char *)malloc(sizeof(char) * (strlen(buff)+1));
				strcpy(temp.phone_num,buff);
				strcpy(sub_buff,result -> phone_num);
				temp.name = (char *)malloc(sizeof(char) * (strlen(result->name)+1));
				strcpy(temp.name ,result->name);
				if(!strncmp(buff,nu_ll,1))
				{
					printf("입력은 한개이상입니다\n");
					return; 
				}
				if((flag = replace_client(result,temp)) ==Success)
				{
					file_write_client();
					printf("변경되었습니다.\n");
					return ;
				}
				else if (flag == Fail_Two_Same_Value)
				{
					printf("같은 번호입니다\n");
					return ;
				}
				else if (flag == Fail_No_Element)
				{
					printf("해당하는 구성요소가 없습니다.");
					temp2.address = (char *)malloc(sizeof(char) * (strlen(temp.address)+1));
					strcpy(temp2.address,buff);
					temp2.password = (char *)malloc(sizeof(char) * (strlen(temp.password)+1));
					strcpy(temp2.password,temp.password);
					temp2.sch_num = temp.sch_num;
					temp2.name = (char *)malloc(sizeof(char) * (strlen(temp.name)+1));
					strcpy(temp2.name,temp.name);
					temp2.phone_num = (char *)malloc(sizeof(char) * (strlen(sub_buff)+1));
					strcpy(temp2.phone_num,sub_buff);
					flag = replace_client(&temp,temp2);
					return ;
				}
			}
			else printf("잘못된 학번입니다.\n");
			break;
		default : 
			printf("1,2,3 만 입력하세요 !!");//1,2,3이아닌 다른걸 입력받았을 경우 
			return;
	}

		}	 
			
			

return; 

}

int Log_in()
{
	const Client * client = NULL;//get _client 함수 쓸꺼임 ㅇㅇ
	char buff[50];// 버퍼

	
	printf(">> 로그인 <<\n");
	while(getchar() != '\n');// 공백문자 대비
	printf("학번: ");
	scanf("%[^\n]", buff);// 공백문자 받을려고

	if (!strcmp(buff, ADMIN_ID))// 입력받은 id가 admin 인지 확인
		my_ID = -1;
	else
		my_ID = atoi(buff);// 아니므로 int로 전환(클라이언트 리스트에 sch_num은 인트형 임)
	while(getchar() != '\n');// \n 제거

	printf("비밀번호: ");
	scanf("%[^\n]", my_password);// 공백문자 받음

	if (my_ID == -1 && !strcmp(my_password, ADMIN_PWD))// 내 아디가 admin일때 비밀번호도 일치하는가?
	{
		return Admin_menu();// 일치함 ㅇㅇ admin_menu 소환
	}
	else
		if (get_client(my_ID, &client) == Success && password2keys_on_client(NULL,my_password) != 0)//아이디가 일치하는 지 그리고 비밀번호가 일치하는지 한번에 ~
		{                                                                                           // get_client 는 앞서 말했고 password2keys_on_client 는 패스워드로 이와 일치하는 원소 갯수를 리스트에서 찾아서 리턴하는 거임 따라서 일치하는 원소가 1개이상 있으면 1을리턴 함 1 != 0 이 참이므로 로그인 됨 
			printf("로그인 되었습니다.");
			return Member_menu();
		}
		else	 
		{
			printf("로그인에 실패하였습니다.\n");
			return 0;
		}
}


