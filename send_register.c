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



typedef struct NID{
    uint8_t nid[16]; 
} nid_t;

typedef struct sid{
        uint8_t nid_sid[16];
        uint8_t l_sid[20];      
} sid_t;

typedef struct sid_route {
       sid_t key;
       nid_t nid_out;
       uint8_t flag_out;
       uint32_t pid_out;
       uint32_t ipv4_out;
 
} sid_header;



#define Physocal_Port "eth0"
#define ETH_P_ALL 0x0003
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

	char buf[MAXLINE];
	int sockfd,len, sid_len;
	struct sockaddr_pkt sa;

	if((sockfd =socket(AF_COLOR,SOCK_REGISTER,htons(ETH_P_ALL)))==-1)
	{
		printf("socket error");
	}
    
    unsigned char *l_part_sid=(unsigned char *)malloc(20);
	unsigned char *nid_part_sid=(unsigned char *)malloc(16);
	unsigned char *nid=(unsigned char *)malloc(16);
	
	memset(&sa,0,sizeof(sa));
	sa.spkt_family= AF_COLOR;
	sa.spkt_protocol = htons(ETH_P_ALL);


	unsigned char *content = "jiangjialishuaibi";
	unsigned char *content1 = "jiangjialishuaibi*2";

	mymd5(content, nid);
	mysha1(content1, l_part_sid);
	memcpy(nid_part_sid, nid, 16);
	print_hash(nid, 16);
	print_hash(nid_part_sid, 16);
	print_hash(l_part_sid, 20);

	strcpy(sa.spkt_device,Physocal_Port);
	memcpy(sa.s_color_addr.l_part_sid,l_part_sid,20);
	memcpy(sa.s_color_addr.nid_part_sid,nid_part_sid,16);
	memcpy(sa.s_color_addr.nid,nid,16);

	sid_header sid_pkg;
	memcpy(sid_pkg.key.nid_sid, nid, 16);
	memcpy(sid_pkg.key.l_sid, l_part_sid, 20);
	memcpy(sid_pkg.nid_out.nid, nid, 16);
	sid_pkg.flag_out = 0;
	sid_pkg.pid_out = 0x0808;
	sid_pkg.ipv4_out = 0x0808;

	sid_len = sizeof(sid_pkg);
	memcpy(buf, &sid_pkg, sid_len);
	printf("sid_len:%d\n", sid_len);


	while(1)
	{

		sleep(1);
		len=sendto(sockfd,buf,sid_len,0,(struct sockaddr *)&sa,sizeof(sa));
		printf("REGISTER request packet sending success,waiting for the data back:\n");

	}

	close(sockfd);
	return 0;

}

