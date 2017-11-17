#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/crypto.h>


typedef struct sid_node{
	unsigned char l_nid[16];
	unsigned char l_sid[20];
	unsigned char nid[16];
	struct sid_node *next;
}sid;


void mymd5(unsigned char *buff,unsigned char *md);
void mysha1(unsigned char *buff,unsigned char *md);
void print_hash(unsigned char *md,int len);
sid* createList(int item_num);
sid* insertList(sid *head, sid *node, int position);
sid* deleteElem(sid* head, sid* node);
void printList(sid* head);
sid* reverseList(sid* head);


void mymd5(unsigned char *buff,unsigned char *md)
{
	int len;
	len = strlen(buff);
	MD5_CTX c;
	MD5_Init(&c);
	MD5_Update(&c,buff,len);
	MD5_Final(md,&c);

}

void mysha1(unsigned char *buff,unsigned char *md)
{
	int len;
	len = strlen(buff);
	SHA_CTX c;	
	SHA1_Init(&c);
	SHA1_Update(&c,buff,len);
	SHA1_Final(md,&c);

}


void print_hash(unsigned char *md,int len)
{
	int i;
	for(i=0;i<len;i++)
	{	
		printf("%02x",md[i]);
	}
	printf("\n");

}


sid* createList(int item_num)
{
	sid *p, *head, *current;
	unsigned char content[50];
	if(item_num == 0)
		printf("item_num error!\n");
	printf("请输入内容:\n");
	scanf("%s", content);
	head = (sid*)malloc(sizeof(sid));
	mymd5(content, head->l_nid);
	mymd5(content, head->nid);
	mysha1(content,head->l_sid);
	p = head;
	item_num--;
	while(item_num > 0)
	{
		current = (sid*)malloc(sizeof(sid));
		printf("请输入内容:\n");
		scanf("%s", content);
		mymd5(content, current->l_nid);
		mymd5(content, current->nid);
		mysha1(content,current->l_sid);
		p->next = current;
		p = p->next;
		item_num--;

	}
	p->next = NULL;
	return head;
}

sid* insertList(sid *head, sid *node, int position)
{
	int num = position -1;
	sid *p, *insert_node;
	unsigned char content[50]; 
	p = head;
	while(num > 0 && p != NULL)
	{
		p = p->next;
		num--;
	}
	if(num > 0)
	{
		printf("要插入的位置不存在!\n");
		return head;
	}

	insert_node = (sid*)malloc(sizeof(sid));
	memcpy(insert_node->l_nid, node->l_nid, 16);
	memcpy(insert_node->l_sid, node->l_sid, 20);
	memcpy(insert_node->nid, node->nid, 16);
	insert_node->next = p->next;
	p->next = insert_node;
	return head;
	
}


sid* deleteElem(sid* head, sid* node)
{
	sid *p, *delete_node;
	p = head;
	if(memcmp(p->l_nid, node->l_nid, 16) == 0)
	{
		delete_node = p;
		p = p->next;
		free(delete_node);
		delete_node = NULL;
		head = p;
		return head;
	}

	while(p->next != NULL)
	{
		if(memcmp(p->next->l_nid, node->l_nid, 16) == 0)
		{
			if(p->next->next != NULL)
			{
				delete_node = p->next;
				p->next = p->next->next;
				free(delete_node);
				delete_node = NULL;
				break;

			}
			else
			{
				delete_node = p->next;
				p->next = NULL;
				free(delete_node);
				delete_node = NULL;
				break;
			}


		}
		p = p->next;
	}

	return head;
}


void printList(sid* head)
{
	sid *p = head;
	if(p == NULL)
	{
		printf("list is empty!\n");
		return;
	}

	while(p != NULL)
	{
		print_hash(p->l_nid, 16);
		print_hash(p->l_sid, 20);
		p = p->next;
	}

	printf("\n");
}

sid* reverseList(sid* head)
{
	sid *ptemp, *current, *temp;
	ptemp = head;
	current = head->next;
	head->next = NULL;
	while(current != NULL)
	{
		temp = current->next;
		current->next = ptemp;
		ptemp = current;
		current = temp;
	}

	return ptemp;
}


int main(int argc, char **argv)
{

	sid *head, p1;
	int num;
	unsigned char *content = "jiangjialishuaibi";
	printf("请输入初始化SID的数目:\n");
	scanf("%d", &num);
	head = createList(num);
	printList(head);

	mymd5(content, p1.l_nid);
	mysha1(content, p1.l_sid);
	mymd5(content, p1.nid);
	printf("插入一个元素:\n");
	head = insertList(head, &p1, 2);
	printList(head);

	
	printf("删除一个元素:\n");
	head = deleteElem(head, &p1);
	printList(head);

	head = reverseList(head);
	printList(head);
	return 0;


}







