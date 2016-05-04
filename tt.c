#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>
using namespace std;
#define NETLINK_USER 31

#define MAX_PAYLOAD 1024 /* maximum payload size*/
struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;


void *get_addr_from_mod (char *ko, char *sym)
{
    string mod_base_file = "/sys/module/";
   string cmd = "nm ";
    FILE *fp;
   unsigned long  baseaddr, offset;

   mod_base_file += ko;
   mod_base_file += "/sections/.text";

   printf ("file base %s\r\n", mod_base_file.c_str());
   fp = fopen(mod_base_file.c_str(), "r");
   if (fp == 0)
   {
        printf ("fopen %s fail, reason %s\r\n",  
			mod_base_file.c_str(), strerror(errno));
   	return 0;
   }
   
   fscanf (fp, "%x", &baseaddr);

   printf ("baseaddr 0x%x\r\n", baseaddr);
   cmd += ko;
   cmd += ".ko";
   cmd += "|grep ";
   cmd += sym;
   cmd += " > 1";
   fp = fopen("1", "r");
   if (fp == 0)
   {
        printf ("fopen %s fail, reason %s\r\n",  
			"1", strerror(errno));
   	return 0;
   }
   printf ("cmd: %s\r\n", cmd.c_str());
   system(cmd.c_str());
   fscanf (fp, "%x", &offset);
   printf ("offset: 0x%x, addr 0x%x\r\n", offset, baseaddr + offset);
   return 0;
   
}

int main(int c, char *v[])
{
    char strp[1000] = "Hello\r\n";

   get_addr_from_mod ("dbg", "cccb");
   return 0;
    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
    if (sock_fd < 0)
        return -1;
  if (c > 1)
  	strcpy(strp, v[1]);
    strcat(strp, "\r\n");
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid(); /* self pid */

    bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

    memset(&dest_addr, 0, sizeof(dest_addr));
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; /* For Linux Kernel */
    dest_addr.nl_groups = 0; /* unicast */

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    strcpy((char *)(NLMSG_DATA(nlh)), strp);

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    printf("Sending message to kernel\n");
    sendmsg(sock_fd, &msg, 0);
    printf("Waiting for message from kernel\n");

    /* Read message from kernel */
    recvmsg(sock_fd, &msg, 0);
    printf("Received message payload: %s\n", NLMSG_DATA(nlh));
    close(sock_fd);
}
