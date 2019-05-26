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

void initwork()//��ʼ������ 
{
    work = (int*) malloc (stay * sizeof (int));
    int i = 0;
    
    for (; i < stay; i++)
    {
        work[i] = -1;//��δ����ҳ�����趨Ϊ-1 
    }
    
    return;
}

bool  isInwork (int n)//�ж��Ƿ��ֱ�ӷ��� 
{
    int i = 0;
    
    for (; i < stay; i++)
    {
        if (visit[n] == work[i])
        {
            return true;
        }
    }
    
    return false;
}

void LRU (int n)
{
    int i, j;
    
    if (!isInwork (n))//�������ֱ�ӷ��ʣ���ȱҳ
    {
        if (index == stay)//�жϵ�ǰ�±��Ƿ񵽴ﴰ�ڱ�Ե �����Ƿ�װ������ 
        {
            int max = n, pos = -1, flag;
            
            for (i = 0; i < stay; i++)//����פ���������ڵ�ҳ�� 
            {
                for (j = n - 1; j >= 0; j--)//�ӵ�ǰ����ҳ����ǰ�� 
                {
                    if (visit[j] == work[i])//��ǰ���Ĺ����е�һ���ڴ����ڵ�ҳ�� 
                    {
                        flag = j;//��Ǹ�ҳ�沢��ѭ�� 
                        break;
                    }
                }
                
                if (flag < max)//���ҳ���ڷ���ҳ��֮ǰ���˴���֤ "���" 
                {
                    max = flag;
                    pos = i;
                    
                    if (max == 0)
                    {
                        break;
                    }
                }
            }
            
            work[pos] = visit[n];//�滻ҳ�� 
            lost++;    //ȱҳ������ 
        }
        
        else//��δװ����ֱ��װ�룬��ʱ������ȱҳ���� 
        {
            work[index] = visit[n];
            index++;
        
		}
	}
}

void run_LRU(int len)
{
    int i;
    initwork();
    printf ("������δʹ���㷨\n");
    
    for (i = 0; i < len; i++)
    {
        LRU (i);
        printf ("[ %d %d %d]\n", work[0], work[1], work[2]);
    }
    
    printf ("������δʹ��ȱҳ�ʣ�%2f ȱҳ����Ϊ��%d \n", lost / (len*1.0), lost);
    lost = 0;
    index = 0;
    free (work);
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

    
    run_LRU(len);
    printf("\n");
  
    getchar();
    getchar();
    return 0;
}
