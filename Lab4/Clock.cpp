#include "stdio.h"
#include"stdlib.h"
#include"time.h"
#define P 32    //�����ڴ����
#define V 64   //�����ڴ����

struct LNode
{
    int data;
    int flag;//����λ
    int modify;//�޸�λ
};

int stay = 6;//�������� 
int visit[32]; //��������
int* work;
int lost = 0;//û�ҵ���ҳ����
int index = 0;//ָʾ��ǰ�±�
LNode* nodes;//�Ľ���Clock�û��㷨�õ������ݽṹ

void produce(int len)
{
    srand ( (unsigned) time (NULL)); 
    int p = rand() % 64;
    int m = 8, e = 8;
    int i, j;
    double t;
    t = rand() % 10 / 10.0;
    
    for (i = 0; i < 4; i++)
    {
        for (j = i * m; j < (i + 1) *m; j++)
        {
        	if (j > len)
			break;
            visit[j] = (p + rand() % e) % 64;
        }
        
        double r = (rand() % 10) / 10.0;
        
        if (r < t)
        {
            p = rand() % 64;
        }
        
        else
        {
            p = (p + 1) % 64;
        }
    }
}


bool isInNodes (int n)
{
    int i;
    
    for (i = 0; i < stay; i++)
    {
        if (nodes[i].data == visit[n])
        {
            return true;
        }
    }
    
    return false;
}

void updated_Clock (int n)
{
    if (!isInNodes (n))//�������ֱ�ӷ��ʣ���ȱҳ
    {
        if (index == stay)//���ڿռ���װ�� 
        {
            lost++;//ȱҳ������ 
            int i = 0, flag = -1;
            while (true)
            {
                if ( (i / stay) % 2 == 0)
                {
                    if (nodes[i % stay].flag == 0 && nodes[i % stay].modify == 0)//ѡ��flag=0��modify=0�ĵ�һ��ҳ����̭
                    {
                        flag = i % stay;
                        break;
                    }
                }
                if ( (i / stay) % 2 == 1)
                {
                    if (nodes[i % stay].flag == 0 && nodes[i % stay].modify == 1)//�ڶ���ɨ�裬ѡ��flag=0��modify=1�ĵ�һ��ҳ����̭
                    {
                        flag = i % stay;
                        break;
                    }
                    else
                    {
                        nodes[i % stay].flag = 0;//����λ��0 
                    }
                }
                i++;
            }
            nodes[flag].data = visit[n];//�滻ҳ�� 
            nodes[flag].flag = 1;//����λ��1 
            if (rand() % 10 < 4)
            {
                nodes[flag].modify = 1;
            }
            
            else
            {
                nodes[flag].modify = 0;
            }
        }
        else//δ����ֱ��װ�� 
        {
            nodes[index].data = visit[n];
            nodes[index].flag = 1;//����λ��1 
            if (rand() % 10 < 4)
            {
                nodes[index].modify = 1;
            }
            else
            {
                nodes[index].modify = 0;
            }
            index++;
        }
    } 
}
 
void Clock(int len)
{
    int i = 0, j = 0;
    printf ("�Ľ���Clock�û��㷨\n");
    nodes = (LNode*) malloc (stay * sizeof (LNode));
    
    for (i = 0; i < stay; i++)
    {
        nodes[i].data = -1;
        nodes[i].flag = -1;
        nodes[i].modify = -1;
    }
    
    for (i = 0; i < len; i++)
    {
        updated_Clock (i);
        for (j = 0; j < stay; j++)
        {
            printf ("%d ", nodes[j].data);
        }
        printf ("\n");
    }
    printf ("�Ľ���Clock�û��㷨ȱҳ�ʣ� %2f ȱҳ����Ϊ��%d \n", lost / (len*1.0), lost);
    lost = 0;
    index = 0;
}
int main ()
{
	int i = 0;
	int len;
	
	printf("����������Ҫ��������г��ȣ�\n");
	scanf("%d",&len); 
    //produce(len);
    printf("����������������£�\n");
     for (; i < len; i++)
    {
        printf ("%d, ", visit[i]);
    }
    printf("\n");

    Clock(len);
	printf("\n");
	
    getchar();
    getchar();
    return 0;
}
