#include"stdio.h"
#include"stdlib.h"
#include"time.h"

typedef struct node
{
    int num;//ҳ��
    node* next;//��һ�����ҳ��
} Node, *pNode;

typedef struct queue
{
    int n;//�ܵĽ����
    pNode front;//����ָ��
    pNode rear; //��βָ��
} Queue, *pQueue;

int visit[32];//��������
int stay = 6;//�����̷�����ڴ�Ĵ�С
int lost = 0;//ȱҳ��

void initQueue (pQueue q)
{
    q->rear = (pNode) malloc (sizeof (Node));
    
    if (q->rear == NULL)
    {
        printf ("failed\n");
    }
    
    else
    {
        q->front = q->rear;
        q->rear->next = NULL;
        q->front->next = NULL;
        q->n = 0;
    }
}

void push (pQueue q, int num)
{
    pNode p = (pNode) malloc (sizeof (Node));
    
    if (p == NULL)
    {
        printf ("failed");
    }
    
    else
    {
        p->next = NULL;
        p->num = num;
        
        if (q->front == q->rear)
        {
            q->front->next = p;
            q->rear = p;
        }
        
        else
        {
            q->rear->next = p;
            q->rear = p;
        }
        
        q->n++;
    }
}

void pop (pQueue q)
{
    pNode p;
    
    if (q->front != q->rear)
    {
        p = q->front->next;
        q->front->next = p->next;
        
        if (p == q->rear)
        {
            q->front = q->rear;
        }
        
        q->n--;
        free (p);
    }
}

void destroy (pQueue q)
{
    while (q->front != q->rear)
    {
        pop (q);
    }
}


bool findInQueue (pQueue q, int num)
{
    pNode p;
    
    if (q->front != q->rear)
    {
        p = q->front->next;
        
        while (p)
        {
            if (p->num == num)
            {
                return true;
            }
            
            else
            {
                p = p->next;
            }
        }
    }
    
    return false;
}

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

void fifo (pQueue q, int n)
{
    if (!findInQueue (q, n))//����q���޴�ҳ�� 
    {
		if (q->n == stay)//��װ������ 
        {
            pop (q);//��̭һ�� 
            push (q, n);//����һ�� 
            lost++;//ȱҳ����һ 
        }
        else//δװ����ֱ��װ�� 
        {
            push (q, n);
        }
    }
}

void fifoTest(int len)
{
    Queue q;
    pNode p;
    initQueue (&q);
    int i = 0;
    printf ("�Ƚ��ȳ��û��㷨\n");
    
    for (; i < len; i++)
    {
        fifo (&q, visit[i]);
        p = q.front->next;
        
        while (p)
        {
            printf ("%d ", p->num);
            p = p->next;
        }
        
        printf ("\n");
    }
    
    printf ("�Ƚ��ȳ��㷨ȱҳ�ʣ�%f   ȱҳ����Ϊ��%d\n", lost / (len*1.0), lost);
    destroy (&q);
}

int main ()
{
	int i;
	int len;
	printf("����������Ҫ��������г��ȣ�\n");
	scanf("%d",&len); 
    produce(len);
    printf("����������������£�\n");
     for (; i < len; i++)
    {
        printf ("%d, ", visit[i]);
    }
    printf("\n");
    fifoTest(len);
    getchar();
    getchar();
    return 0;
}

