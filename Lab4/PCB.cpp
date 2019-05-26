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
    LNode* next;
};
struct Link
{
    int num;//�����ϵĽ����
    LNode* next;
};

int stay = 6;
int p;//����������ʼλ��
int table[32];//�����ڴ棬ÿһ��Ԫ�ش���һ��ҳ��
int visit[32]; //��������
int memo[3] = { -1, -1, -1 };
int lost = 0;//û�ҵ���ҳ����
int index = 0;//ָʾ��ǰ�±�
LNode* nodes;//�Ľ���Clock�û��㷨�õ������ݽṹ
Link idle;
Link modified;

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

LNode* isinLinks (int n)//�ж��Ƿ��ڶ����� 
{
    LNode*p, *q;
    p = idle.next;
    q = NULL;
    
    while (p)
    {
        if (p->data == visit[n])//������ҳ��ͬ����ҳ��һ�� 
        {
            if (q != NULL)
            {
                q->next = p->next;
                p->next = NULL;
                idle.num--;
                break;
            }
            else
            {
                idle.next = NULL;
            }
        }
        q = p;
        p = p->next;
    } 
    if (p == NULL)
    {
        p = modified.next;
        
        while (p != NULL)
        {
            if (p->data == visit[n])
            {
                if (p == modified.next)
                { modified.next = p->next; }
                
                else
                {
                    q->next = p->next;
                    p->next = NULL;
                    modified.num--;
                }
                
                if (modified.num == 0)
                { modified.next = NULL; }
                
                break;
            }
            
            q = p;
            p = p->next;
        }
    }
    
    return p;
}
void emptyIdle ()
{
    LNode* p;
    p = idle.next;
    
    while (p)
 {
        idle.next = p->next;
        free (p);
        p = idle.next;
    }
    
    idle.num = 0;
}
void emptyModi()
{
    LNode* p;
    p = modified.next;
    
    while (p)
    {
        modified.next = p->next;
        free (p);
        p = modified.next;
    }
    modified.num = 0;
}
void addToLink (int data, int type)//������к��� 
{
    LNode* p;
    LNode* q;
    q = (LNode*) malloc (sizeof (LNode));
    q->data = data;
    q->flag = 1;
    if (type == 1)
    {
        q->modify = 1;
        p = modified.next;
    }
    else
    {
        q->modify = 0;
        p = idle.next;
    }
    q->next = NULL;
    if (p == NULL)
    {
        if (type == 0)
        {
            idle.next = q;
        }
        
        else
        {
            modified.next = q;
        }
    }
    else
    {
        while (p)
        {
            if (p->next == NULL)
            {
                p->next = q;
                break;
            }
            
            else
            {
                p = p->next;
            }
        }
    }
    if (type == 0)
    {
        idle.num += 1;
        
        if (idle.num == 10)
        {
            emptyIdle();
        }
    }
    else
    {
        modified.num += 1;
        
        if (modified.num == 10)
        {
            emptyModi();
        }
    }
}

void PBA (int n)
{
    if (!isInNodes (n))//�������ֱ�ӷ��ʣ���ȱҳ 
    {
        if (index == stay)//�жϵ�ǰ�±��Ƿ񵽴ﴰ�ڱ�Ե �����Ƿ�װ������ 
        {
            LNode *p;
            if ( (p = isinLinks (n)) != NULL)//���ڶ��������д��ڣ���ֱ��װ�� 
            {
                nodes = (LNode*) realloc (nodes, (stay + 1) * sizeof (LNode));
                nodes[stay] .data = p->data;
                nodes[stay].flag = p->flag;
                nodes[stay].modify = p->modify;
                nodes[stay].next = p->next;
                free (p);//�ͷŵ�ǰ��㣬������ 
                stay++;
                index++;
            }
            else//������������û�У���Ϊʵ��ȱҳ 
            {
                lost++;//ȱҳ����һ 
                if (nodes[n % 3].modify == 1)//���޸�λΪ1 
                {
                    addToLink (nodes[n % 3].data, 1);//����������޸�λΪ1 
                }
                else
                {
                    addToLink (nodes[n % 3].data, 0);//���Ϊ0 
                }
                nodes[n % 3].data = visit[n];//װ��ҳ�� 
                nodes[n % 3].flag = 1;//����λ��1 
                nodes[n % 3].next = NULL;
                if (rand() % 10 < 4)
                {
                    nodes[n % 3].modify = 0;
                }
                else
                {
                    nodes[n % 3].modify = 1;
                }
            }
        }
        else//��δװ����ֱ��װ�룬��ʱ������ȱҳ���� 
        {
            nodes[index].data = visit[n];
            nodes[index].flag = 1;
            nodes[index].next = NULL;
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

int main ()
{
    int i = 0, j = 0;
    int k; 
	int len;
	printf("����������Ҫ��������г��ȣ�\n");
	scanf("%d",&len); 
    //produce(len);
    printf("����������������£�\n");
     for (; k < len; k++)
    {
        printf ("%d, ", visit[k]);
    }
    printf("\n");
    
    printf ("ҳ�滺���û��㷨(PBA)\n");
    idle.num = 0;
    idle.next = NULL;
    modified.num = 0;
    modified.next = NULL;
    nodes = (LNode*) malloc (stay * sizeof (LNode));
    
    for (i = 0; i < stay; i++)
    {
        nodes[i].data = -1;
        nodes[i].flag = 0;
        nodes[i].modify = 0;
        nodes[i].next = NULL;
    }
    
    for (i = 0; i < len; i++)
    {
        PBA (i);
        
        for (j = 0; j < stay; j++)
        {
            printf ("%d ", nodes[j].data);
        }
        
        printf ("\n");
    }
    
    printf ("ҳ�滺���û��㷨ȱҳ�ʣ�%f   ȱҳ����Ϊ��%d\n", lost / (len*1.0), lost);
    getchar();
    getchar();
    return 0;
}

