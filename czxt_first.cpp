#include <stdio.h>
#include <ctype.h> 
#include <string.h>
#include <time.h>

int main(){
   //实现一个 输入 数据 然后转换大小写，之后存入 文件中，且显示时间  
   // 且计算程序运行时间  
   
   printf("请输入一串字母：");
   printf("\n"); 
   char newstr[100]="\0";
   char str[100]="\0";
   scanf("%s",str);
   printf("转换后字母串:\n");
   for(int i=0;i<100;i++){
     if(str[i]>='A'&&str[i]<='Z'){
     	//则为大写字母  
     	printf("%c",tolower(str[i])); //转换为小写 函数 
     	newstr[i]=tolower(str[i]);     
	 }
	 else if(str[i]>='a'&&str[i]<='z'){
	 	 //则为小写字母
		   printf("%c",toupper(str[i])); 
		   newstr[i]=toupper(str[i]);    //转换大写函数 
	 }
    }
    //存入文件中  
    printf("\n");
    
    printf("请输入需要创建的文件名称：");
    
    char filename[100]={0};
    
    strcpy(filename,"E:\\");   //调用字符串复制函数 
    
    scanf("%s",filename+3);
    
    FILE *fp=fopen(filename,"w+");   //调用 打开文件函数  
    
    int length=strlen(str);   //调用 strlen 计算长度  
    
    
    printf("创建文件成功！"); 
    for(int i=0;i<length;i++){
    	fprintf(fp,"%c",str[i]);   //写入文件函数 
    }
    fprintf(fp,"\n");   //给字母后面添加了一个换行 
    
	for(int i=0;i<length;i++){
		fprintf(fp,"%c",newstr[i]);
	}
	
	printf("\n"); 
    printf("将原先字母串与转换后的字母串写入文件成功！");
    
	//这里应该读文件查看效果
	printf("\n"); 
	fclose(fp);  //首先要先关闭文件  
	             //才能读取 
	char sss[100];
	FILE*fpread=fopen(filename,"r");  //
	printf("从文件中读取信息\n"); 
	fscanf(fpread,"%s",sss);
	printf("1：%s\n",sss); 
	
	fgets(sss,100,(FILE*)fpread);
	
    fgets(sss,100, (FILE*)fpread);
     printf("2: %s\n", sss );
    
    time_t timep;
    time (&timep);
    printf("现在的时间：%s",ctime(&timep));

	
	 
	 
	 
	 
  return 0;

	
} 
