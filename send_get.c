#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <time.h>
#include <pthread.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/crypto.h>

			
#define Physocal_Port "lo"
#define MAXLINE 256


void mymd5(unsigned char *buff,unsigned char *md);
void mysha1(unsigned char *buff,unsigned char *md);
void print_hash(unsigned char *md,int len);


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



int main(int argc,char  **argv)
{

	//char buf[MAXLINE]={0};
	char buf[MAXLINE]= "1111222233334444";
	int sockfd,len;
	struct sockaddr_pkt sa;
	if((sockfd =socket(AF_COLOR,SOCK_GET,htons(ETH_P_ALL)))==-1)
	{
		printf("socket error");
	}
    char *l_part_sid=(char *)malloc(20);
	char *nid_part_sid=(char *)malloc(16);
	char *nid=(char *)malloc(16);

	unsigned char *content = "jiangjialishuaibi";
	unsigned char *content1 = "jiangjialishuaibi*2";

	mymd5(content, nid);
	mysha1(content1, l_part_sid);
	memcpy(nid_part_sid, nid, 16);
	print_hash(nid, 16);
	print_hash(nid_part_sid, 16);
	print_hash(l_part_sid, 20);

	memset(&sa,0,sizeof(sa));
	sa.spkt_family= AF_COLOR;
	sa.spkt_protocol = htons(ETH_P_ALL);


	strcpy(sa.spkt_device,Physocal_Port);
	memcpy(sa.s_color_addr.l_part_sid,l_part_sid,20);
	memcpy(sa.s_color_addr.nid_part_sid,nid_part_sid,16);
	memcpy(sa.s_color_addr.nid,nid,16);


	while(1)
	{
		sleep(1);
		len=sendto(sockfd,buf,MAXLINE,0,(struct sockaddr *)&sa,sizeof(sa)); //buf size  0 no output
		printf("send get packet success!!!\n");

	}
	

	if(len <0)
	{
		printf("sendto error \n");
	}
	
	close(sockfd);
	return 0;

}


