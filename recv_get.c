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

#define Physical_Port "eth0"
#define BUFSIZE 256



int main(int argc, char **argv)
{
	

	int recv_size, recv_len, pid_num,sockfd,color_addr_len;
	char RecvBuf[BUFSIZE]={0};
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
	msg.msg_iov->iov_len = BUFSIZE;
	msg.msg_control=0;
	msg.msg_controllen =0;
	msg.msg_flags =0;

	while(recv_size = recvmsg(sockfd,&msg,0))
	{
		char *temp =msg.msg_iov->iov_base;
		recv_len = strlen(temp);
		pid_num = recv_len/4;
		printf("pid_num is %d, recv_len is %d  %s\n",pid_num, recv_len, temp);	

	}
	
	close(sockfd);
	printf("FINISH RECV_GET!!!!\n");
	return 0;

}


