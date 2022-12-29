#include<stdio.h>
#include<string.h>    // for strlen
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_addr
#include<unistd.h>    // for write
#include<time.h> //for show date
 
int main(int argc, char *argv[])
{
    //USE A SPACE AFTER "GET_DATE" WHEN USING
    //USE A SPACE AFTER "GET_DATE" WHEN USING
    //USE A SPACE AFTER "GET_DATE" WHEN USING
    //USE A SPACE AFTER "GET_DATE" WHEN USING
    //USE A SPACE AFTER "GET_DATE" WHEN USING
    //USE A SPACE AFTER "GET_DATE" WHEN USING
    //USE A SPACE AFTER "GET_DATE" WHEN USING
    //USE A SPACE AFTER "GET_DATE" WHEN USING
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    char message[1000];
	char received_message[1000];
    char showformatmessage[] ="Enter time format [with GET_DATE]: ";//to show message on telnet
    char showerrormessage[] ="INCORRECT REQUEST\n";//to warn about syntax
    time_t ticks;
     
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    //Address Family - AF_INET (this is IP version 4)
    //Type - SOCK_STREAM (this means connection oriented TCP protocol)
    //Protocol - 0 (Specifies a particular protocol to be used with the socket...
       //Specifying a protocol of 0 causes socket() to use an unspecified default protocol) 
       //[ or IPPROTO_IP This is IP protocol]
    if (socket_desc == -1)
    {
        puts("Could not create socket");
        return 1;
    }
     
    server.sin_family = AF_INET;  //IPv4 Internet protocols
    server.sin_addr.s_addr = INADDR_ANY; //IPv4 local host address
    server.sin_port = htons(8902); // server will listen to 8888 port
     
    // Bind
    if(bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        puts("Binding failed");
        return 1;
    }
    puts("Socket is binded");
     
    // Listen
    listen(socket_desc, 3);
     
    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    
    c = sizeof(struct sockaddr_in);
    new_socket= accept(socket_desc,(struct sockaddr *)&client,(socklen_t*)&c);
    if (new_socket<0)
    {
        puts("Accept failed");
        return 1;
    }
     
    puts("Connection accepted");
    while(1){
	    write(new_socket,showformatmessage,strlen(showformatmessage));//sended message to telnet
        char lastmessage[1000];//resets on each process 
        time_t time_1970;
        struct tm *time_detay;
        char cdizi[100];
        time_1970=time(NULL);
        time_detay = localtime(&time_1970);
        recv(new_socket,received_message,1000,0);//message from telnet
        char delim[]= " ";
        char *ptr = strtok(received_message,delim);//to split message from telnet
        int flag1=0;//to check does message from telnet start with GET_DATE
        int flag2=1;//to check date format
        while (ptr!=NULL&&flag2==1)
        {
            if(!strcmp(ptr,"GET_DATE"))//format start confirmed
            {
                flag1=1;
                strcat(lastmessage,">"); 
                strcat(lastmessage," ");               
            }
            else if(flag1==1){//if start format is confirmed
                strftime(cdizi,1000,ptr,time_detay);                
                if(!strcmp(cdizi,"-"))
                {
                   //do nothing   
                   strcat(lastmessage," ");                
                }
                else if(!strcmp(cdizi,ptr)&&strstr(cdizi,"%"))//check format 
                {
                    write(new_socket,showerrormessage,strlen(showerrormessage));
                    flag2=0;
                    break;
                }               
                else{//memorized if format is correct
                    strcat(lastmessage,cdizi);
                    strcat(lastmessage," ");
                }
                
            }
            else{//if format doesn't start GET_DATE
                write(new_socket,showerrormessage,strlen(showerrormessage));
                flag2=0;
                break;
            }
            ptr = strtok(NULL,delim);
            
        }
        strftime(cdizi,1000,lastmessage,time_detay);
        if(flag2==1&&!strstr(cdizi,"%")){//after all check returning dates based on telnet request
            
            write(new_socket,cdizi,strlen(cdizi));

        }
        else{
            write(new_socket,showerrormessage,strlen(showerrormessage));
        }
	}   

     close(socket_desc);
     close(new_socket); 

    return 0;
}

