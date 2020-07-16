#include <string> 
#include <cstring>
#include <stdlib.h>
#include <iostream> 
using namespace std;
#define max 12   //规定了输入的字符串大小，可改 

struct Stable{
	char lei; //类型
	string value; //值 
	int pro;   //算符优先级
	 
}; 

struct Op{
	string  value;
	int pro;    //算符优先级 
}; 

Op op[max];

Stable stable[100];
char s[max];
char ei[max];
int x;
int i;

float  value;

int total[max];

//string op[max];

//声明函数 
void lexicalERROR();
void printsuc();
void syntaxfx();
void syntaxerror(); //语法错误！ 
void semanticAnalysis();

int cifa(){
	cout<< endl; 
  cout <<"************************语义分析*******************************"<<endl;
  cout <<"          规定：输入字符为12个（可改max值）" <<endl; 
  cout<<"input:"<<endl;
	
  for(int i=0;i<max;i++){
    cin>>s[i];
   }
   cout<<endl;
   cout<<"---------------------------------------------------------------------------------------------"<<endl;
   cout<<"                               输入的字符串                              "<<endl;
   cout<<"---------------------------------------------------------------------------------------------"<<endl;
   cout<<"下标: "; 
   for(int i=0;i<max;i++){
   	cout<<i;
   	cout<<"      ";
   }
   cout<<endl;
   cout<<"值:   ";
   for(int i=0;i<max;i++){
   	cout<<s[i];
   	cout<<"      "; 
   }
   
   cout<<endl;
   //判断类型是否 为数字或其他
    char ch[10];
    int index = 0;//Stable下标
    int flag = 1;//默认无错误
    int num = 0;
    for(int i = 0; i < max;i++){
        if(s[i] >= '0' && s[i] <= '9'){
            stable[index].lei='n'; //数字 
            while(s[i] >= '0' && s[i] <= '9'){
                num += s[i] - '0';
                num *= 10;
                i++;
            }
            int x=num/10;
            sprintf(ch,"%d",x);
    	    string str(ch,ch+strlen(ch));
            stable[index].value=str; 
            num = 0;
            i--;
            index++;
        }else if(s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/'){
            stable[index].lei='o';  //算符 
            stable[index].value=s[i];
			//cout<<"输出stable算符："<<stable[index].value<<endl;
            index ++;
        }else{
            ei[i] = 1;  //其他的则错误 
            flag = 0;//出错的话则将flag=0 然后调用下方的else return -1 
        }
    }
    if(flag){
    	return index;
    }else{
    	return -1; // 
    }
	
	
	
}

void pipeisuanfu(string ch);
void E();
void T();
void A();
void F();
void B();
     string jia="+";
     string jian="-";
     string jing="#";
     string ji="*";
     string chu="/";
int main(){
  
     
	 x=cifa();
	cout<<"------------------------------------------------------------------------------------------------"<<endl;
	cout<<"                                     lexical分析表                "<<endl;
	cout<<"-------------------------------------------------------------------------------------------------"<<endl;
	cout<<"下标: "; 
	for(int i=0;i<max;i++){
		cout<<i;
		cout<<"       ";
		
	} cout<<endl;
	cout<<"类型: ";
   	for(int i=0;i<max;i++){
   		cout<<stable[i].lei;
   		cout<<"       ";
	   }
	   cout<<endl;
	cout<<"值：  "; 
   	for(int i=0;i<max;i++){
   		cout<<stable[i].value;
   		cout<<"       ";
	   }
   	
   	//test
   	//
   	if(x==-1){
   		cout<<endl;
   		cout<<endl;
   		lexicalERROR();
	   }
	else{
	    stable[x].lei='o';//为index赋值 
	    stable[x].value='#';
		x++;
		cout<<endl;
		cout<<endl; 
		printsuc();
		cout<<endl;
		cout<<endl;   
		syntaxfx();   //调用语法分析 
		
		value = 0;
		semanticAnalysis();  //调用语义分析 
	}
   	
   	
     
}
//根据 消除左递归的的文法进行编写语法分析函数

void syntaxfx(){
	i=0;
	E();
}

void E(){     //E->TE'    E'可以看作A ,且通过文法可知其需为数字 
     if(stable[i].lei=='n'){
     	T();
		A();
	 }
	 else{
	 	syntaxerror();
	 } 
}
void T(){ //根据T->FT'  T'可以用B来表示，根F->i故而这里为数字 
    if(stable[i].lei=='n'){ 
    	F();
		B();
	} 
	else{
	syntaxerror();	
	}
} 
void F(){
	if(stable[i].lei=='n'){   //因为 F->i 故而这里就可以进行返回，且开始扫描下一个stable表中的元素 
		i++;
		return ;
	}
	else{
	syntaxerror();	
	} 
}
void A(){   
     
     if(stable[i].lei=='o'&&stable[i].value==jia){//根据 A->+TA   文法进行编写
     	 pipeisuanfu(jia);//匹配算符+
		 T();                
		 A();  
	 }
	 else if(stable[i].lei=='o'&&stable[i].value==jian){//根据 A->-TA 
	 	pipeisuanfu(jian);        
	 	T();
	 	A();
	 }
	 else if(stable[i].lei== 'o' && stable[i].value==jing){ //根据   A->E     等于空的时候则i加一，扫描下一个 
		i++; 
		return ;
		}
	else{
	     syntaxerror();	
	}
		
}
void B(){
	 if(stable[i].lei=='o'&&stable[i].value==ji)  //根据 B->*FB 
	 {
	 	pipeisuanfu(ji);
		F();
		B(); 
	  } 
	  else if(stable[i].lei=='o'&&stable[i].value==chu) //根据 B->/FB
	  {
	  	pipeisuanfu(chu);
	  	F();
	  	B();
	   }
	   //根据算符后面不能跟随其他符号（如+-） 
	 else if(stable[i].lei == 'o' && (stable[i].value==jing||stable[i].value==jia||stable[i].value==jian)){ 
		return ; 
	}
	else{
		syntaxerror();
	}
	
}
void pipeisuanfu(string ch){
	if (i < x && stable[i].value==ch)
	 	i++; //匹配成功，开始分析下一个元素 
	else
	   syntaxerror();
}
void syntaxerror(){
	cout<<"语法错误"<<endl;;
	cout<<"坐标："<<i<<"--------->"; 
	if(stable[i].lei=='o'){
		cout<<stable[i].value;
	}else{
		cout<<stable[i].value;
	}
	exit(0);
}
void printsuc(){
	cout<<"句子："<<endl;

	for(int i=0;i<x;i++)
	{
		if(stable[i].lei=='o')
		{
			if(stable[i].value==jia)
			{
				stable[i].pro=1;            
			}
			if(stable[i].value==jian)
			{
				stable[i].pro=1;
			}
			if(stable[i].value==ji)
			{
				stable[i].pro=3;
			}
			if(stable[i].value==chu)
			{
				stable[i].pro=3;
			}
			if(stable[i].value==jing)
			{
				stable[i].pro=-2;   //且如果出现它则结束 ,
			}
		}
	    
	}
	
	
    for(int i = 0; i < x;i++){
        if(stable[i].lei == 'o'){
            cout<<stable[i].value;
            
        }else{
            cout<<stable[i].value;
            
        }
    }
}


void  semanticAnalysis()
{ 

	for(int i = 0; i < x;i++)
	{
		
	 if(total[0] == 0)
	 {
	 string s = stable[i].value;
	 char * end;
	 float y = static_cast<float>(strtol(s.c_str(),&end,10));
	 total[0]=y;
	 }
	 if(total[1] == 0)
	 {
	 string t =stable[i+2].value;
	 char * tx;
	 float z = static_cast<float>(strtol(t.c_str(),&tx,10));
	 total[1]=z;
	 
	 }
	 
     if(!op[0].value.empty())
     {
     	
     	if((op[0].pro-stable[i+2].pro)<0)   // +   , /  (1-3)=-2   ,
     	{  
		  
		        string s4 = stable[i+3].value;
     	   	    char *t4;
     	   	    float a = static_cast<float>(strtol(s4.c_str(),&t4,10));
					 
     	   if(stable[i+2].value == ji)
     	    {
     	   	   value = total[1]*a;    //
				    
			}
			if(stable[i+2].value==chu)
			{
				value = total[1]/a;    // 12/4
			}
			total[1] = value;   //置入栈顶
			i = i+1;  //将指针指向后面 ,现在的  i = 1+1 = 2  
			
		 } 
		 else if((op[0].pro - stable[i+2].pro) == 2)      //当 3 - 1 =2 ，3-（-2）=5，都大于0，所以不能这样，应该直接 
        {
             	if(op[0].value == ji)
             	{
             		value = total[0]*total[1];
             		total[0]=value; 
             		total[1] = 0;
             		op[0].value = op[1].value;  //置空
			 
				 }
		       if(op[0].value == chu)
		       {
		       	value = total[0] / total[1];
		       	total[0]=value;
				total[1]=0;
				op[0].value = op[1].value;  
			   }
		 }
	
		 else if(op[0].pro == stable[i+2].pro)   //+ , -  ,1 = 1
		 {
		 	
		 	 if(op[0].value == jia)
		 	 {
		 	 	value = total[0] + total[1];  // 
		 	 	total[0] = value;   //将值置入栈0
				op[0].value = op[1].value;   //将算符数组置空
				total[1] = 0;  //将其置为 0 新添加值
				
			  }
			  if(op[0].value == jian)  // -
			  {
			  	value = total[0]-total[1];    
			  	total[0] = value;
			  	op[0].value = op[1].value;
			  	total[1] = 0;  //置入 0 ，重新赋值 
			   
			   } 
			   if(op[0].value == ji)
			   {
			   	value = total[0]*total[1];
			   	total[0] = value;
			   	total[1] = 0;
			   	op[0].value = op[1].value;
			   	
			   }
			   if(op[0].value == chu)
			   {
			   	value = total[0]/total[1];
			   	total[0] = value;
			   	total[1]=0;
			   	op[0].value = op[1].value;
			   }
		 }
		
		 
		 else if(stable[i+2].value == jing)  //出现# 
		 {
		 	if(op[0].value == jia)
		 	 {
		 		value = total[0]+total[1];
		 		
			 }
			 if(op[0].value == jian)
			 {
			 	value = total[0] - total[1];
			 }
		 	if(op[0].value== ji)
		 	{
		 		value = total[0]*total[1];
			 }
			 if(op[0].value == chu)
			 {
			 	value = total[0] / total[1]; 
			 }
		 	break;  //出现# 则结束 
		 }
		  
	 }
     else
     {
     	
     	op[0].value=stable[i+1].value;     // 4+1 = 5 
     	
     	if(stable[i+1].value==jia)
     	{
     		op[0].pro=1;
		 }
		 if(stable[i+1].value==jian)
		 {
		 	op[0].pro=1;
		 }
		 if(stable[i+1].value==ji)
		 {
		 	op[0].pro=3;
		 }
		 if(stable[i+1].value==chu)
		 {
		 	op[0].pro=3;
		 }
		 
	    }
	}
	
	cout << "语义分析结果:" << "  " <<  value;
    cout << endl;

}


void lexicalERROR(){

    cout<<"词法错误！"<<endl;
    for(int i = 0 ; i < max; ++i){
        if(ei[i] == 1){
            cout<<i<<"----->"<<s[i]<<endl;
        }
        
    }

}
