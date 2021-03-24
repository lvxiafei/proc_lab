/**********************************************
    author:xiafei_xupt
	website:https://xiafei-xupt.top/
	function:collect process data form proc filesystem
/***********************************************/

#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>   
#include <fcntl.h> 

#include <time.h>
int current_time()
{
  time_t t;
  struct tm * lt;
  time (&t);
  lt = localtime(&t);
  printf ("%d:%d:%d",lt->tm_hour,lt->tm_min,lt->tm_sec);
  return 0;

}

//文件指针，用于读取proc文件系统中的一些文件
FILE *thisProcFile;  

//获取CPU类型
void CPUinfo()
{
    char s[1000];
    char gu[10] = "model name";

    //打开文件并进行读取
    if((thisProcFile = fopen("/proc/cpuinfo","r")) == NULL)
    {
        printf("%s\n","Open file failed!!");
        exit(0);    
    }

    while(fgets(s,100,thisProcFile) != NULL)
    {
        char *temp = strtok(s,":");

        if(strncmp(s,gu,10) == 0)
        {
            //以":"为分割符将字符串分割开来
            temp = strtok(NULL,":");
            printf("CPU类型: %s\n",temp);

            fclose(thisProcFile);
            return;
        }
    }
}

//获取内核版本
void Kernel_version()
{
    char s[1000];

    if((thisProcFile = fopen("/proc/version","r")) == NULL)
    {
        printf("%s\n","Open file failed!!");
        exit(0);    
    }

    printf("内核版本信息为: \n");
    while(!feof(thisProcFile))
    {
        if(fgets(s,100,thisProcFile) != NULL)
            printf("%s",s);
    }

    fclose(thisProcFile);

}

//获取系统运行时间
void time_run()
{
    char s[100];

    if((thisProcFile = fopen("/proc/uptime","r")) == NULL)
    {
        printf("%s\n","Open file failed!!");
        exit(0);    
    }

    if(fgets(s,100,thisProcFile) != NULL)
    {
        char *temp = strtok(s," ");
        int times = atoi(temp);

        //进行时间的转换
        int run_days = times / 86400;
        int run_hours = (times % 86400) / 3600;
        int run_minutes = (times % 3600) / 60;
        int run_seconds = (times % 60);
        printf("系统运行时间为: %d天%d时%d分%d秒\n",run_days,run_hours,run_minutes,run_seconds);
    }

    fclose(thisProcFile);
}

void sampleLoadAvg(){ //观察系统负荷
    char s[100];
    if((thisProcFile = fopen("/proc/loadavg","r")) == NULL)
    {
        printf("%s\n","Open file failed!!");
        exit(0);    
    }
    fgets(s,100,thisProcFile);
    fclose(thisProcFile);
    char *temp = strtok(s," ");
    char *array[5];
    int m = 0;
    while(temp != NULL){
        array[m] = temp;
        temp = strtok(NULL," ");
        m++;
    }
    printf("一分钟之内的平均进程数为: %s\n",array[0]);
    printf("五分钟之内的平均进程数为: %s\n",array[1]);
    printf("十五分钟之内的平均进程数为: %s\n",array[2]);
    printf("正在运行进程数/进程总数: %s\n",array[3]);
    printf("最近运行的进程ID: %s\n",array[4]);

    fclose(thisProcFile);
}

//获取CPU状态
void cpu_state()
{
    char s[1000];

    if((thisProcFile = fopen("/proc/stat","r")) == NULL)
    {
        printf("%s\n","Open file failed!!");
        exit(0);    
    }

    if(fgets(s,1000,thisProcFile) != NULL)
    {
        char *temp = strtok(s," ");

        char *array[11];
        int m = 0;
        while(temp != NULL){
            array[m] = temp;
            temp = strtok(NULL," ");
            m++;
        }

        printf("CPU执行用户态所用的时间: %s jiffies\n",array[1]);
        printf("CPU执行系统态所用的时间: %s jiffies\n",array[3]);
        printf("CPU执行空闲态所用的时间: %s jiffies\n",array[4]);
    }

    fclose(thisProcFile);
}

//获取上下文切换次数
void ctxt()
{
    char s[1000];
    char gu[4] = "ctxt";

    if((thisProcFile = fopen("/proc/stat","r")) == NULL)
    {
        printf("%s\n","Open file failed!!");
        exit(0);    
    }
	
    while(fgets(s,100,thisProcFile) != NULL)
    {
        char *temp = strtok(s," ");

        if(strncmp(s,gu,4) == 0)
        {
            temp = strtok(NULL," ");
            printf("上下文切换的次数为: %s\n",temp);

            fclose(thisProcFile);
            return;
        }
    }
}

//获取创建的进程数
void processes()
{
    char s[1000];
    char gu[9] = "processes";

    if((thisProcFile = fopen("/proc/stat","r")) == NULL)
    {
        printf("%s\n","Open file failed!!");
        exit(0);    
    }
 
    while(fgets(s,100,thisProcFile) != NULL)
    {
        char *temp = strtok(s," ");

        if(strncmp(s,gu,9) == 0)
        {
            temp = strtok(NULL," ");
            printf("创建的进程数为: %s\n",temp);

            fclose(thisProcFile);
            return;
        }
    }
}

//获取内存总量
void MemTotal()
{
    char s[100];
    char gu[8] = "MemTotal";

    if((thisProcFile = fopen("/proc/meminfo","r")) == NULL)
    {
        printf("%s\n","Open file failed!!");
        exit(0);    
    }

    while(fgets(s,100,thisProcFile) != NULL)
    {
        char *temp = strtok(s,":");

        if(strncmp(s,gu,8) == 0)
        {
            temp = strtok(NULL,":");
            printf("内存总量为: %s\n",temp);

            fclose(thisProcFile);
            return;
        }
    }
}

//获取活动内存
void MemFree()
{
    char s[100];
    char gu[7] = "MemFree";

    if((thisProcFile = fopen("/proc/meminfo","r")) == NULL)
    {
        printf("%s\n","Open file failed!!");
        exit(0);    
    }

    while(fgets(s,100,thisProcFile) != NULL)
    {
        char *temp = strtok(s,":");

        if(strncmp(s,gu,7) == 0)
        {
            temp = strtok(NULL,":");
            printf("可用内存为: %s\n",temp);

            fclose(thisProcFile);
            return;
        }
    }
}


//获取进程信息
void process_info(int i)
{	
	//printf("%d",i);
	char number[10];            
	sprintf(number,"%d",i);

	//char number[]="0";
	char s[1000];

	//获取1号进程stat信息
		
	//char filename[]="/proc/1/stat";
	char filename[]="/proc/";
	char stat[]="/stat";
	strcat(filename,number); 
	strcat(filename,stat); 
				
	//判断文件是否存在
	//if((access("filename",F_OK))!=-1)   
    //{   
    //    printf("文件存在.\n");   
    //}   
   // else	
		
    if((thisProcFile = fopen(filename,"r")) == NULL)
    {
        //printf("%s\n","Open file failed!!");
		//i++;
		//for(;i<1000;i++){
		//process_info(i);
	//}
        //exit(0); 
		return;
    }
	
    if(fgets(s,1000,thisProcFile) != NULL)
    {
        char *temp = strtok(s," ");

        char *array[1000];
        int m = 0;
        while(temp != NULL){
            array[m] = temp;
            temp = strtok(NULL," ");
            m++;
        }
		//100 * (process jiffies)/ (float)Hertz / (float)et  / cpuCoresCount
		//(process jiffies) = (current process jiffies) - (last process jiffies)
		
		
		//int array_13= atof(array[13]);
		//int array_14= atof(array[14]);
		//int array_15= atof(array[15]);
		//int array_16= atof(array[16]);
		//float cpu_100=100.0f*(array_13+array_14+array_15+array_16)/100/3/1;
		
		//printf("时间：");
		current_time();
		//printf("\n");	
		float cpu_100=0.0;
		printf("	%s	%s	%8s	 %0.2f	%s	%s	%s	%s	%s	%s	",array[0],array[18],array[22],cpu_100,array[2],array[13],array[14],array[15],array[16],array[1]);
					
		/*
		int i;
		printf("stat: ");
		for(i=0;i<30;i++)
		printf("%s  ",array[i]);
		printf("\n");
		*/
		//printf("\npid	   name		%%cpu 	irq		contextswitch 	current_time\n");
		
        //printf("%s	%s\n",array[0],array[1]);
    }
    fclose(thisProcFile);	  		  
	//获取io信息，输出多个文件
	char rchar[8] = "rchar";
	char wchar[8] = "wchar";
	char syscr[8] = "syscr";
	char syscw[8] = "syscw";
	char read_bytes[16] = "read_bytes";
	char write_bytes[16] = "write_bytes";
	
	if((thisProcFile = fopen("/proc/1/io","r")) == NULL)
    {
        //printf("%s\n","Open file failed!!");
        //exit(0); 
		return;		
    }
	   
    while(fgets(s,100,thisProcFile) != NULL)
    {
        char *temp = strtok(s,": ");

        if(strncmp(s,rchar,8) == 0)
        {
            temp = strtok(NULL,":");
			int array_rchar= atof(temp);
			printf("%d",array_rchar);

            //fclose(thisProcFile);
            //return;
        }
		
		if(strncmp(s,wchar,8) == 0)
        {
            temp = strtok(NULL,":");
			int array_wchar= atof(temp);
			printf("	%d",array_wchar);
			//printf("   %s",temp);

            //fclose(thisProcFile);
            //return;
        }
		if(strncmp(s,syscr,8) == 0)
        {
            temp = strtok(NULL,":");
			int array_syscr= atof(temp);
			printf("	%d",array_syscr);

            //fclose(thisProcFile);
            //return;
        }
		if(strncmp(s,syscw,8) == 0)
        {
            temp = strtok(NULL,":");
			int array_syscw= atof(temp);
			printf("	%d",array_syscw);
			//printf("   %s",temp);

            //fclose(thisProcFile);
            //return;
        }
		if(strncmp(s,read_bytes,16) == 0)
        {
            temp = strtok(NULL,":");
			int array_read_bytes= atof(temp);
			printf("	%d",array_read_bytes);
			//printf("   %s",temp);

            //fclose(thisProcFile);
            //return;
        }
		
		if(strncmp(s,write_bytes,16) == 0)
        {
            temp = strtok(NULL,":");
			int array_write_bytes= atof(temp);
			printf("	%d",array_write_bytes);
			
           // printf("  %s\n",temp);
			printf("\n");
            fclose(thisProcFile);
            return;
        }
    }
    fclose(thisProcFile);	  	
}

//定时器
void time_cycle()
{
	int i;
	for(i=0;i<100000;i++){
		process_info(i);
	}
	sleep(3);
	time_cycle();
}
int main(int argc,char *argv[])
{
	//格式：
	//printf("time	 PID		 name		  NI		start_time	    cpu%	     state			utime			stime				cutime					cstime					rchar			wchar		   syscr  		   syscw		  read_bytes	  write_bytes	 ");//英文
	//printf("时间	进程ID		进程名		nice值		 启动时间	  CPU%	    进程状态	    用户态运行时间	    核心态运行的时间  	waited-for用户态时间 	waited-for核心态时间	读的总字节数	写的总字节数	read调用次数	write调用次数	读出磁盘字节数 	写入磁盘字节数	");//中文
	
	printf("   时间	进程ID	nice值	启动时间	CPU百分比 	进程状态 	用户态运行时间	 核心态运行的时间 	waited-for用户态时间	 waited-for核心态时间   	进程名  	读的总字节数	写的总字节数	read调用次数	write调用次数	读出磁盘字节数 	写入磁盘字节数\n");
	time_cycle();
}
