#include <iostream>
#include "function.h"

int main()
{
	Init();
	
	char func[20];
	int index,count,pos;
	Init_block(func,20);
	while(gets(func))
	{
		int fun;
		char filename[FILE_NAME_LENGTH];
		//��ʼ��
		fun = -1;
		Init_block(filename,FILE_NAME_LENGTH);
		if(strncmp("dictionary",func,10) == 0)			//���Ŀ¼dir(non)
		{
			printf("---------------------�ļ��������-------------------------\n");
			directory();	
			printf("----------------------------------------------\n\n");
		}
		if(strncmp("create",func,6) == 0)			//�����ļ�����create(filename)
		{
			strcat(filename,func+7);
			if(create(filename) == 1)
				printf("�Ѵ����ļ�%s\n\n",filename);
			directory();
		}
		if(strncmp("delete",func,6) == 0)			//ɾ���ļ�����delete(filename)
		{
			strcat(filename,func+7);
			if(destroy(filename) == 1)
				printf("��ɾ���ļ�%s\n\n",filename);
			directory();
		}
		if(strncmp("open",func,4) == 0)			//���ļ�����open(filename)
		{
			strcat(filename,func+5);
			if(open(filename) == 1)
				printf("�Ѵ��ļ�%s���������\n",filename);
		}
		if(strncmp("close",func,5) == 0)			//�ر��ļ�����close(index)
		{
			if(show_openlist()==0)
			{
				printf("��ǰû���ļ�����\n\n");
				break;
			}
			printf("������Ҫ�ر��ļ��������ţ�\n");
			scanf("%d",&index);
			if(close(index) == 1)
				printf("�ѹر�%d\n\n",index);
			directory();
		}
		if(strncmp("read",func,4) == 0)			//���ļ�����read(index)
		{
			if(show_openlist()==0)
			{
				printf("��ǰû���ļ�����\n\n");
				break;
			}
			printf("������Ҫ��ȡ�ļ��������ţ�\n");
			scanf("%d",&index);
			printf("�������ȡ�ļ����ȣ�\n");
			scanf("%d",&count);
			if(read(index,0,count) == 1)
				printf("���ļ��ɹ�\n\n");
		}
		if(strncmp("write",func,5) == 0)			//д�ļ�����write(index)
		{
			if(show_openlist()==0)
			{
				printf("��ǰû���ļ�����\n\n");
				break;
			}
			printf("������Ҫд���ļ���������\n");
			scanf("%d",&index);
			printf("������д�볤�ȣ�\n");
			scanf("%d",&count);
			if(write(index,0,count) == 1)
				printf("д������ɹ�\n\n");
		}
		if(strncmp("lseek",func,5) == 0)			//ָ������lseek(index,pos)
		{
			if(show_openlist()==0)
			{
				printf("��ǰû���ļ�����\n\n");
				break;
			}
			printf("������Ҫд���ļ��������ţ�\n");
			scanf("%d",&index);
			printf("�������ļ����λ��\n");
			scanf("%d",&pos);
			lseek(index,pos);
			printf("\n"); 
		}
		if(strncmp("openlist",func,6) == 0)			// �鿴���ļ���
		{
			if(show_openlist()==0)
			{
				printf("û���ļ�����\n\n");
				break;
			}
			show_openlist();
		}
		if(strncmp("lblock",func,6) == 0)			//�鿴���� 
		{
			printf("����������£�\n");
			show_lblock();
		}

		if(strncmp("exit",func,4) == 0)			// �˳�����exit
		{
			exit(0); 
		}
		fflush(stdin);
		Init_block(func,20);
	}
return 0;
}

