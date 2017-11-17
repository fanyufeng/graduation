//gcc send_data.c -o send_data -ldl -lcrypto


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <resolv.h>
#include <signal.h>
#include <getopt.h>
#include <linux/if_packet.h>
//#include <config.h>
#include <string.h>

#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/crypto.h>
#define Physical_Port "lo"
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

int main(int argc, char **argv)
{
	File *fp;
	fp=fopen("send_fan.txt","r");
	
	int sockfd_data,len, pid_num;
	char buf_data_send[MAXLINE]=NULL;
	struct sockaddr_pkt sa_data;

	if((sockfd_data=socket(AF_COLOR,SOCK_DATA,htons(ETH_P_ALL)))==-1)
	{
		printf("create data socket error\n");
	}

	memset(&sa_data,0,sizeof(sa_data));
	sa_data.spkt_family=AF_COLOR;
	sa_data.spkt_protocol=htons(ETH_P_ALL);
	
	
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

	/*pid_num = 2;
	sa_data.s_color_addr.pid_number = pid_num;
	strcpy(sa_data.spkt_device,Physical_Port);
	memcpy(sa_data.s_color_addr.l_part_sid,l_part_sid,20);
	memcpy(sa_data.s_color_addr.nid_part_sid,nid_part_sid,16);
	memcpy(sa_data.s_color_addr.nid,nid,16);
	
//sa_data.s_color_addr.pid_1 =  0x1af8cd01;
	sa_data.s_color_addr.pid_1[0] ='1';
	sa_data.s_color_addr.pid_1[1] ='1';
	sa_data.s_color_addr.pid_1[2] ='1';
	sa_data.s_color_addr.pid_1[3] ='1';

//sa_data.s_color_addr.pid_2 =  0x37a37a02;
	sa_data.s_color_addr.pid_2[0] ='2';
	sa_data.s_color_addr.pid_2[1] ='2';
	sa_data.s_color_addr.pid_2[2] ='2';
	sa_data.s_color_addr.pid_2[3] ='2';


	//memcpy(sa_data.s_color_addr.pid_1,"1111",4);
	//memcpy(sa_data.s_color_addr.pid_2,"2222",4);
//	memcpy(sa_data.s_color_addr.pid_3,"3333",4);
//	memcpy(sa_data.s_color_addr.pid_4,"4444",4);
*/
	
	while(fread(buf_data_send,sizeof(buf_data_send),1,fp))
	{
		sleep(1);
		len= sendto(sockfd_data,buf_data_send,32,0,(struct sockaddr *)&sa_data,sizeof(sa_data));
		printf("data send success!\n");

	}
	close(sockfd_data);

	return 0;
}



