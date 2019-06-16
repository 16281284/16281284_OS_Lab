#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#define C		10			//����� 
#define H		10			//��ͷ�� 
#define B1		10			//������ 
#define L		500	    //�洢������
#define B 		10			//�洢�鳤��
#define K		100			//��������С
#define FILE_BLOCK_LENGTH		7				//�ļ�������̿�����鳤��
#define FILE_NAME_LENGTH		9				//��ļ�������
#define FILE_SIGN_AREA			((L-1-K)/B+1)		//���������ļ���ʶ����ʼ���(λͼ֮��)
#define FILE_NUM				FILE_BLOCK_LENGTH	//Ŀ¼������ļ���Ŀ
#define BUFFER_LENGTH			25					//���ļ���Ŀ�еĻ���������

struct BLock
{
    int Block_num; //�洢���
    int c; // �����
    int h; //��ͷ��
    int b; //������
}Block; 

struct filesign{							//�ļ�������
	int file_length;						//�ļ�����
	int filesign_flag;						//ռ�ñ�ʶλ
	int file_block;							//�ļ�������̿������ʵ�ʳ���
	int file_block_ary[FILE_BLOCK_LENGTH];	//�ļ�������̿������
};

struct con{							//Ŀ¼��
	char filename[FILE_NAME_LENGTH];		//�ļ���
	int	 filesignnum;						//�ļ����������
};

struct openfilelist{						//���ļ����Ŀ
	char buffer[BUFFER_LENGTH];				//��д������
	int pointer[2];							//��дָ��
	int filesignnum;						//�ļ�������
	int flag;								//ռ�÷�
};

BLock ldisk[C][H][B1];						//ģ�����
char lblock[L][B]; 

openfilelist open_list[FILE_NUM];		//���ļ���

int Disk_Block(int c,int h,int b);
void InitDisk();
int show_openlist();				
void directory();	
void read_block(int,char *);	
void write_block(int,char *);		
void Init();						
int create(char *);					
int destroy(char *);				
int open(char *);					
int close(int);					
int read(int,int,int);			
int write(int,int,int);			
int write_buffer(int,int);		
int lseek(int,int);				
void Init_block(char,int);		

int Disk_Block(int c,int h,int b)
{
    return b+b*h+b*h*c;
}

void InitDisk()
{
    for(int i=0;i<C;i++)
        for(int j=0;j<H;j++)
            for(int k=0;k<B1;k++)
            {
                ldisk[i][j][k].c=i;
                ldisk[i][j][k].h=j;
                ldisk[i][j][k].b=k;
                ldisk[i][j][k].Block_num=Disk_Block(i,j,k);//�����Ӧ���߼����
            }
}

void read_block(int i,char *p)
{
	char * temp = (char *)malloc(sizeof(char));
	temp = p;
	for(int a = 0; a < B;)
	{
		*temp = lblock[i][a];
		a++;
		temp++;
	}
}

void write_block(int i,char *p)
{
	char * temp = (char *)malloc(sizeof(char));
	temp = p;
	for(int a = 0; a < B;)
	{
		lblock[i][a] = *temp;
		a++;
		temp++;
	}
}

void Init_block(char *temp,int length) 
{
	int i;
	for(i = 0; i < length; i++)
	{
		temp[i] = '\0';
	}
}

int write_buffer(int index,int list)
{

	int i;
	int j;
	int freed;
	char temp[B];

	int buffer_length = BUFFER_LENGTH;
	for(i = 0; i < BUFFER_LENGTH ; i ++)
	{
		if(open_list[list].buffer[i] == '\0')
		{
			buffer_length = i;				
			break;
		}
	}
	
	int x = open_list[list].pointer[0];
	int y = open_list[list].pointer[1];
	int z = B-y;									

	if( buffer_length < z )				
	{	
		read_block(x,temp);
		strncat(temp+y,open_list[list].buffer,buffer_length);	
		write_block(x,temp);
		
		read_block(index+FILE_SIGN_AREA,temp);	
		temp[1] += buffer_length;	
		write_block(index+FILE_SIGN_AREA,temp);
		
		open_list[list].pointer[0] = x;
		open_list[list].pointer[1] = y + buffer_length ;				
	}
	else								
	{
		read_block(index+FILE_SIGN_AREA,temp);
		if(temp[2]+(buffer_length - z)/B+1 > FILE_BLOCK_LENGTH)
		{
			printf("�ļ��������鲻������\n");
			return 0;
		}
		read_block(x,temp);
		strncat(temp+y,open_list[list].buffer,z);			
		write_block(x,temp);
		for(i = 0; i < (buffer_length - z)/B ; i ++ )
		{
			for(j = K + FILE_NUM; j < L ; j++)
			{
				read_block((j-K)/B,temp);
				if(temp[(j-K)%B] == 0)
				{
					freed = j;
					break;
				}
			}
			if(j == L)
			{
				printf("��������,����ʧ��\n");
				return 0;
			}
			
			Init_block(temp,B);						
			strncpy(temp,(open_list[list].buffer+z+(i*B)),B);
			write_block(freed,temp);				

			read_block((freed-K)/B,temp);			
			temp[(freed-K)%B] = 1;
			write_block((freed-K)/B,temp);			

			read_block(index+FILE_SIGN_AREA,temp);
			temp[2] ++;								
			temp[2+temp[2]] = freed;
			write_block(index+FILE_SIGN_AREA,temp);	
		}
		for(j = K + FILE_NUM; j < L ; j++)
		{
			read_block((j-K)/B,temp);
			if(temp[(j-K)%B] == 0)
			{
				freed = j;
				break;
			}
		}
		if(j == L)
		{
			printf("��������,����ʧ��\n");
			return 0;
		}
		Init_block(temp,B);
		strncpy(temp,(open_list[list].buffer+z+(i*B)),(buffer_length - z)%B);
		write_block(freed,temp);			

		read_block((freed-K)/B,temp);		
		temp[(freed-K)%B] = 1;
		write_block((freed-K)/B,temp);			

		read_block(index+FILE_SIGN_AREA,temp);
		temp[2] ++;							
		temp[2+temp[2]] = freed;
		write_block(index+FILE_SIGN_AREA,temp);


		read_block(index+FILE_SIGN_AREA,temp);	
		temp[1] += buffer_length;			
		write_block(index+FILE_SIGN_AREA,temp);

		open_list[list].pointer[0] = freed;
		open_list[list].pointer[1] = (buffer_length - z)%B ;
	}	
}
int lseek(int index,int pos)
{
	int i;
	int list = -1;
	char temp[B];
	int block_pos = pos / B; 	
	int block_in = pos % B;			
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)//�ڴ�Ŀ¼���ҵ���Ӧ�ļ� 
		{
			list = i;
			break;
		}
	}
	read_block(open_list[list].filesignnum + FILE_SIGN_AREA,temp);//ȷ���������� 
	if(block_pos > temp[2]-1)										
	{
		printf("Խ��\n");
		return 0;
	}
	open_list[list].pointer[0] = temp[3+block_pos];		
	open_list[list].pointer[1] = block_in; //�ҵ���λλ�� 
	return 1;

}


void Init()
{
	
	int i;
	char temp[B];
	

	for(i = 0; i < L; i++)					
	{
		Init_block(temp,B);
		write_block(i,temp);			
	}

	for(i = K; i < L; i ++)					
	{
		read_block((i-K)/B,temp);
		temp[(i-K)%B] = 0;
		write_block((i-K)%B,temp);
	}


	filesign temp_cnt_sign;
	temp_cnt_sign.filesign_flag =1;
	temp_cnt_sign.file_length = 0;							
	temp_cnt_sign.file_block = FILE_BLOCK_LENGTH;
	 
	Init_block(temp,B);
	temp[0] = temp_cnt_sign.filesign_flag;
	temp[1] = temp_cnt_sign.file_length;
	temp[2] = temp_cnt_sign.file_block;

	for(i = 0; i < FILE_BLOCK_LENGTH ; i++ )
	{
		temp[i+3] = K+i;								
	}
	write_block(FILE_SIGN_AREA,temp);


	read_block(0,temp);
	for(i = 0; i < FILE_NUM ; i++ )
	{
		temp[i] = 0;					
	}
	write_block(0,temp);
}


int create(char filename[])
{
	int i;
	int sign_local;						
	int	content_local;						
	int content_locall;								
	char temps[B];
	char tempc[B];
	char temp[B];	

	for(i = FILE_SIGN_AREA ; i < K; i ++)
	{	
		read_block(i,temp);
		if(temp[0] == 0)
		{
			sign_local = i;			//�ҵ��������п��е��ļ�������	
			break;
		}
	}
	for(i = K ; i < K+FILE_NUM ; i++)
	{	
		read_block((i-K)/B,temp);						
		if(temp[(i-K)%B] == 0)
		{
			content_local = i;     //�ҵ���������ָ���Ŀ��п� 
			break;
		}
	}
	if(i == K+FILE_NUM)
	{
		printf("�ļ��Ѵ�����\n");
		return 0;
	}

	for(i = K + FILE_NUM; i < L ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == 0)
		{
			content_locall = i;   //�ҵ��ļ����Ŀ��п� 
			break;
		}
	}
	//��ʱ���� 
	filesign temp_filesign;				
	contents temp_contents;				
	temp_filesign.filesign_flag = 1;
	temp_filesign.file_length = 0;
	temp_filesign.file_block = 1;
	
	Init_block(temps,B);
	temps[0] = temp_filesign.filesign_flag;
	temps[1] = temp_filesign.file_length;
	temps[2] = temp_filesign.file_block;
	temps[3] = content_locall;
	for(i = 4 ; i < FILE_BLOCK_LENGTH ; i++)
	{
		temps[i] = '\0';
	}
	write_block(sign_local,temps);		//�������ļ�������д�����	
	temp_contents.filesignnum = sign_local - FILE_SIGN_AREA;
	strncpy(temp_contents.filename,filename,FILE_NAME_LENGTH);
	
	Init_block(tempc,B);
	tempc[0] = temp_contents.filesignnum;   
	tempc[1] = '\0';
	strcat(tempc,temp_contents.filename);
	write_block(content_local,tempc);	//д������������		

	read_block((content_local-K)/B,temp);     //�޸�λͼ 
	temp[(content_local-K)%B] = 1;
	write_block((content_local-K)/B,temp);		

	read_block((content_locall-K)/B,temp);		
	temp[(content_locall-K)%B] = 1;
	write_block((content_locall-K)/B,temp);		

	read_block(FILE_SIGN_AREA,temp); //Ŀ¼���ȼ�1 
	temp[1]++;
	write_block(FILE_SIGN_AREA,temp);		
	return 1;	
}

int destroy(char * filename)
{
	int i;
	int sign_dlocal;										
	int content_dlocal;									
	int use_block;								
	int index;
	char temp[B];
	char tempd[B];
	
	for(i = K ; i < K+FILE_NUM ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == 1)
		{
			read_block(i,temp);
			if(strncmp(temp+1,filename,FILE_NAME_LENGTH) == 0)//�Ҷ�Ӧ�ļ���
			{
				content_dlocal = i;			//�ҵ�Ŀ¼��λ�� 
				sign_dlocal = temp[0] + FILE_SIGN_AREA;	 //�ҵ��ļ�������λ�� 
				index = temp[0];
				break;
			}
		}
	}
	int list = -1;
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)//ȷ���ļ��Ƿ�� �����޷�ɾ�� 
		{
			list = i;
			break;
		}
	}
	if(open_list[list].flag == 1 && list != -1)
	{
		printf("���ļ�δ�ر�\n");
		return 0;
	}
	
	read_block(sign_dlocal,temp);
	use_block = temp[2];
	for(i = 0 ; i < use_block ; i++)
	{
		read_block((temp[i+3]-K)/B,tempd);				
		tempd[(temp[i+3]-K)%B] = 0;
		write_block((temp[i+3]-K)/B,tempd);
	}
	//ɾ���ļ� 
	Init_block(temp,B);//ɾ��Ŀ¼�� 
	write_block(sign_dlocal,temp);
	Init_block(temp,B);//ɾ���ļ������� 
	write_block(content_dlocal,temp);
	//����λͼ 
	read_block((content_dlocal-K)/B,temp);
	temp[(content_dlocal-K)%B] = 0;
	write_block((content_dlocal-K)/B,temp);
	read_block(FILE_SIGN_AREA,temp);
	temp[1]--;
	write_block(FILE_SIGN_AREA,temp);
	return 1;
}
int open(char * filename)
{
	int i;
	int oppos;
	int opcontent;
	int list;
	char temp[B];
	int index;       
	for(i = K ; i < K+FILE_NUM ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == 1)
		{
			read_block(i,temp);
			if(strncmp(temp+1,filename,FILE_NAME_LENGTH) == 0)//�ҵ���Ӧ�ļ��� 
			{
				oppos = i;				//�ҵ��ļ�Ŀ¼��λ��
				opcontent = temp[0] ;	//�ҵ��ļ������� 
				break;
			}
		}
	}
	for(i = 0 ; i < FILE_NUM ; i++)  //�ҵ��ձ�Ŀ 
	{
		if(open_list[i].flag != 1)
		{	
			list = i;
			break;
		}
	}
	open_list[list].filesignnum = opcontent;   //д���ļ�������		
	open_list[list].flag = 1;							
	index = open_list[list].filesignnum;	//����������			
	lseek(index,0);			//ָ���趨Ϊ�ڳ�ʼλ��		
	Init_block(open_list[list].buffer,BUFFER_LENGTH);			
	read_block(open_list[list].pointer[0],temp);				
	strncpy(open_list[list].buffer,temp,BUFFER_LENGTH);	  //д�뻺���� 
	return 1;
}

int close(int index) 
{
	int i;
	int list = -1;
	char temp[B];
	for(i = 0; i < FILE_NUM ; i++)   
	{
		if(open_list[i].filesignnum == index)    //�ҵ�Ŀ¼ 
		{
			list = i;
			break;
		}
	}
	write_buffer(index,list);//�����ļ���Ļ���д�� 
	Init_block(open_list[list].buffer,BUFFER_LENGTH);//���				
	open_list[list].filesignnum = 0;								
	open_list[list].flag = 0;									
	open_list[list].pointer[0] = NULL;							
	open_list[list].pointer[1] = NULL;
	return 1;
}

int read(int index, int mem_area, int count) 
{
	int i;
	int list = -1;
	char temp[B];
	for(i = 0; i < FILE_NUM ; i++)//�ҵ���Ӧ�� 
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}

	char temp_output[100];//ȷ���ļ����� 
	Init_block(temp_output,100);
	char output[100];
	Init_block(output,100);
	read_block(FILE_SIGN_AREA+index,temp);
	int file_length = temp[1];				
	int file_block = temp[2];				
	int file_area;
	
	for(i = 0; i < file_block - 1 ; i++)
	{
		read_block(FILE_SIGN_AREA+index,temp);
		read_block(temp[3+i],temp);
		strncpy(temp_output+i*B,temp,B);			
	}
	read_block(FILE_SIGN_AREA+index,temp);
	read_block(temp[3+i],temp);
	strncpy(temp_output+i*B,temp,B);
	int x = open_list[list].pointer[0];   //��λ�ļ� 
	int y = open_list[list].pointer[1];
	for(i = 0 ; i < file_block ; i++)
	{
		read_block(FILE_SIGN_AREA+index,temp);
		if(temp[3+i] == x)
		{
			break;
		}
	}
	file_area = i * B + y;		//λ��ת��						
	for(i = 0 ;  i < count ; i++)//�ҵ���λ�ĵط�������� 
	{
		output[i+mem_area] = temp_output[i+file_area]; 
	}
	printf("%s\n",output+mem_area);
	return 1;
}
 
int write(int index,int mem_area,int count)
{
	int i;
	int list = -1;
	int input_length;
	char temp[B];
	for(i = 0; i < FILE_NUM ; i++)//�ҵ��ļ��򿪱� 
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}
	char input[100];
	Init_block(input,100);
	i = 0;
	fflush(stdin);
	while(scanf("%c",&input[i]))   //������� 
	{
		if(input[i] == '\n')											
		{
			input[i] == '\0';			
			break;
		}
		i++;
	}
	input_length = i;		
	if(count <= BUFFER_LENGTH )		//������������д��			
	{
		strncat(open_list[list].buffer,input+mem_area,count);		
	}
	else												
	{
		int rest;					
		for(i = 0; i < BUFFER_LENGTH ; i++)
		{
			if(open_list[list].buffer[i] == 0)
			{
				rest = BUFFER_LENGTH - i;    //�ó��������� 
				break;
			}
		}		
		strncat(open_list[list].buffer+BUFFER_LENGTH-rest,input + mem_area,rest);   //�п��е�д���ļ������ 
		write_buffer(index,list);
		Init_block(open_list[list].buffer,BUFFER_LENGTH);
		for(i = 0; i < (count/BUFFER_LENGTH)-1 ; i++)    //ѭ��д�� 
		{
			strncpy(open_list[list].buffer,(input+mem_area)+rest+i*BUFFER_LENGTH,BUFFER_LENGTH);

			write_buffer(index,list);
			Init_block(open_list[list].buffer,BUFFER_LENGTH);
		}
		Init_block(open_list[list].buffer,BUFFER_LENGTH);
		strncpy(open_list[list].buffer,(input+mem_area)+rest+i*BUFFER_LENGTH,count%BUFFER_LENGTH);//ʣ���д�뻺���� 
		int buffer_start;
	}
	return 1;
}

void directory()
{
	int i;
	int filenum;
	int filelength;
	char filename[FILE_NAME_LENGTH];
	char temp[B];
	char tempd[B];
	char temps[B];
	read_block(FILE_SIGN_AREA,temp);
	filenum = temp[1];						
	printf("\n");
	if(filenum == 0 )//�ļ���Ϊ��ʱ����� 
	{
		printf("��Ŀ¼��û���ļ�\n");
	}
	
	for(i = 0; i < FILE_NUM; i++)//��ʾĿ¼���ļ���Ϣ 
	{
		read_block(temp[3+i],tempd);					
		if(tempd[0] != 0)
		{
			read_block(tempd[0]+FILE_SIGN_AREA,temps);		
			if(temps[0] == 1 && tempd[0] != 0)
			{	
				filelength = temps[1];
				strcpy(filename,tempd+1);
				printf("%s\t\t���ļ��Ĵ�СΪ%d�ֽ�\n",filename,filelength);
			}
		}
	}
	if(filenum != 0 )
	{
		printf("\t\t\t\t��ǰ����%d���ļ�\n",filenum);//���ͳ����Ϣ 
	}
}

int show_openlist()
{

	int i,j;
	int openfile = 0;
	char temp[B];
	int index;
	printf("\n������\t\t���ļ��Ĵ�С\t\t���ļ���\n");
	for(i = 0 ; i < FILE_NUM ; i ++) 
	{
		if(open_list[i].flag == 1)//���ļ����б���Ѱ�� 
		{
			index = open_list[i].filesignnum;//�ҵ���Ӧ������ 
			printf("  %d",index);
			openfile++;
			read_block(FILE_SIGN_AREA+index,temp);//�ҵ���Ӧ�ļ���С 
			printf("\t\t %d",temp[1]);	
			for(j = K; j < K+FILE_NUM ; j++)
			{
				read_block(j,temp);
				if(temp[0] == index)//�ҵ���Ӧ�ļ��� 
				{
					printf("\t\t%s\n",temp+1);
				}
			}	
		}
	}
	return openfile;

}

