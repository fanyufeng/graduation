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


void pthread_recv_get()
{

	int recv_size, recv_len, pid_num,sockfd,color_addr_len;
	char RecvBuf[MAXLINE]={0};
	struct sockaddr_pkt sa;
	struct msghdr msg;
	struct iovec iov;  //void__user *iov_base __kernel_size_t iov_len

	
	strcpy(sa.spkt_device,Physical_Port);
	

	if((sockfd = socket(AF_COLOR,SOCK_GET,htons(ETH_P_ALL)))==-1)
	{
		printf("socket error\n");
	}

	color_addr_len = sizeof(sa);
	msg.msg_name = &sa;
	msg.msg_namelen = color_addr_len;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1; // buff size num
	msg.msg_iov->iov_base = RecvBuf;
	msg.msg_iov->iov_len = MAXLINE;
	msg.msg_control=0;
	msg.msg_controllen =0;
	msg.msg_flags =0;

	while((recv_size = recvmsg(sockfd,&msg,0))>= 0)
	{
		char *temp =msg.msg_iov[0].iov_base;
		printf("recv get len is %d\n",recv_size);	

	}
	
	close(sockfd);
	printf("FINISH RECV_GET!!!!\n");
	return 0;
}


void pthread_send_data()
{
	int sockfd_data,len, pid_num;
	char buf_data_send[MAXLINE]={0};
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

	pid_num = 4;
	sa_data.s_color_addr.pid_number = pid_num;
	strcpy(sa_data.spkt_device,Physical_Port);
	memcpy(sa_data.s_color_addr.l_part_sid,l_part_sid,20);
	memcpy(sa_data.s_color_addr.nid_part_sid,nid_part_sid,16);
	memcpy(sa_data.s_color_addr.nid,nid,16);
	

	memcpy(sa_data.s_color_addr.pid_1,"1111",4);
	memcpy(sa_data.s_color_addr.pid_2,"2222",4);
	memcpy(sa_data.s_color_addr.pid_3,"3333",4);
	memcpy(sa_data.s_color_addr.pid_4,"4444",4);

	while(1)
	{
		sleep(1);
		len= sendto(sockfd_data,buf_data_send,MAXLINE,0,(struct sockaddr *)&sa_data,sizeof(sa_data));
		printf("data send success!\n");

	}
	close(sockfd_data);


}


int main(int argc, char** argv)
{
	pthread_t thread[2];  
	memset(&thread, 0, sizeof(thread));
	int temp;
	if((temp = pthread_create(&thread[0], NULL, pthread_recv_get, NULL)) != 0)         
    	printf("线程pthread_recv_get创建失败!\n");  
    else  
        printf("线程pthread_recv_get被创建\n");  
    if((temp = pthread_create(&thread[1], NULL, pthread_send_data, NULL)) != 0)    
        printf("线程pthread_send_data创建失败");  
    else  
    	printf("线程pthread_send_data被创建\n");  
    while(1)
	{}
    return 0;
}



