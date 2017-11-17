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


void pthread_send_get()
{
	char buf[MAXLINE]={0};
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


	strcpy(sa.spkt_device,Physical_Port);
	memcpy(sa.s_color_addr.l_part_sid,l_part_sid,20);
	memcpy(sa.s_color_addr.nid_part_sid,nid_part_sid,16);
	memcpy(sa.s_color_addr.nid,nid,16);


	while(1)
	{
		sleep(1);
		len=sendto(sockfd,buf,0,0,(struct sockaddr *)&sa,sizeof(sa)); //buf size  0 no output
		printf("send get packet success!!!\n");

	}
	

	if(len <0)
	{
		printf("sendto error \n");
	}
	
	close(sockfd);
	
}


void pthread_recv_data()
{
	int recvdata_fd,DataLen,len;
	struct sockaddr_pkt sa_dataRecv;
	struct iovec iov;
	struct msghdr msg;
	char RecvDataBuf[MAXLINE]={0};
		
	if((recvdata_fd=socket(AF_COLOR,SOCK_DATA,htons(ETH_P_ALL)))==-1)
	{
		printf("create data recv socket error\n");
	}
	
	memset(&sa_dataRecv,0,sizeof(sa_dataRecv));
	sa_dataRecv.spkt_family=AF_COLOR;
	sa_dataRecv.spkt_protocol=htons(ETH_P_ALL);
	strcpy(sa_dataRecv.spkt_device,Physical_Port);
	
	
	
	DataLen = sizeof(sa_dataRecv);
	msg.msg_name = &sa_dataRecv;
	msg.msg_namelen=DataLen;
	msg.msg_iov=&iov;
	msg.msg_iovlen=1;
	msg.msg_iov->iov_base =	RecvDataBuf;
	msg.msg_iov->iov_len=MAXLINE;
	msg.msg_control= 0;
	msg.msg_controllen=0;
	msg.msg_flags=0;
	
	while(len = recvmsg(recvdata_fd,&msg,0))
	{

		char *temp_data=msg.msg_iov[0].iov_base;
		printf("finish receive data message! len %d\n", len);		

	}	
		
	close(recvdata_fd);
	return 0;	
}


int main(int argc, char** argv)
{

	pthread_t thread[2];  
	memset(&thread, 0, sizeof(thread));
	int temp;
	if((temp = pthread_create(&thread[0], NULL, pthread_send_get, NULL)) != 0)         
    	printf("线程pthread_send_get创建失败!\n");  
    else  
        printf("线程pthread_send_get被创建\n");  
    if((temp = pthread_create(&thread[1], NULL, pthread_recv_data, NULL)) != 0)    
        printf("线程pthread_recv_data创建失败");  
    else  
    	printf("线程pthread_recv_data被创建\n");  
    while(1)
	{}
    return 0;
}

