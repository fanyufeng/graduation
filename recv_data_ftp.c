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
#define BUFSIZE 256



int main(int argc, char **argv)
{
	File *fp=NULL;
	fp=fopen("fan.txt","w");
	
	int recvdata_fd,DataLen,len;
	struct sockaddr_pkt sa_dataRecv;
	struct iovec iov;
	struct msghdr msg;
	char RecvDataBuf[BUFSIZE]={0};
		
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
	msg.msg_iov->iov_len=BUFSIZE;
	msg.msg_control= 0;
	msg.msg_controllen=0;
	msg.msg_flags=0;
	
	while(len = recvmsg(recvdata_fd,&msg,0))
	{
		char *temp_data=msg.msg_iov[0].iov_base;
		fwrite(&msg.msg_iov[0].iov_base,sizeof(msg.msg_iov[0].iov_base),1,fp);
		printf("finish receive data message! len %d   %s\n", len, temp_data);		

	}	
		
	close(recvdata_fd);
	return 0;	
	
}


