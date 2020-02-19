#include <iostream>
#include <fstream>
#include <direct.h> 
#include <windows.h> 
#include <shlwapi.h>
#include "dirczong.cpp"
#include <stdio.h> 


using namespace std;
#define max  100 
void zhuce();
void denglu();

  
struct UOF{
	char txtname[20];  //文件名称 这里的 大小 一定要弄好 
	char attribute[10];  //文件属性 
    int  loginlen;   // 记录长度 
	int  state;  // 文件状态  （打开/建立） 1为建立 0为打开 
	int read;     //读指针
	int write;    //写指针  
};    
// 每个 用户文件对应一个     已打开文件                               
struct UFD{  //二级文件目录  
	 char txtname[20];
	 char attribute[10];    //文件属性 只读（是否）  //只能读 ，就不能写入数据 ， 
	 int  loginlen;    //记录长度 
	 int  txtlocation;  // 文件地址  
  
};
// w 代表可写，r 代表只读  
int cipan[10000];    //磁盘块号！ 

struct  userMFD{     // 一级目录  
	string username;   //用户名    可以使用 strcpy(char ,string.c_str())来进行操作 将string 赋值到其中  
	int  userlocation;   //一级目录地址   //为其分配地址	
	UFD ufdfile[10];    // 使用 MFD 包含了 UFD   每个用户 包含 10个文件 
	UOF uofopen[6];    //每个用户只能打开 6个 文件 ，判断文件的状态，如果为建立则不能打开，如果为打开则可以打开 
	                  // 如果为新用户的话，只能新建文件，那么文件的状态则都为 建立 ，则都不能打开 ，
					  //故而 该实验 便需要进行充分的初始化 操作                      // 为已打开文件名表   0表示状态为 打开 ，1表示为建立 不能打开  
}u[10]={ {"Tom",12,{{"f1","w",12,122},{"f2","r",122,33},{"f3","r",32,125} },{{"f1","w",12,0,10,20},{"f2","r",13,1,22,20}} },
       
        {"jane",14,{{"test","w",13,122 },{"English复习资料","r",12,33},{"BBC英语听力","w",123,30}},{{"BBC英语听力","r",123,0,22,30}} },
        {"user1",20,{{"f1","r",45,23},{"f2","w",66,88},{"f3","r",55,90}},{{"f1","r",45,1,23,90}}}
		

       };   //比如说 jane 这个用户 它有 f1 f2 f3 f4 f5 f6 f7 f8 f9 f10 10个文件 
	   //// 然后有 f1~f4文件登记在 UOF中 ，然后jane 这时候需要打开 f10文件 ，先在 jane中找文件 f10发现存在 
	   /// 然后将f10登记 在 uof中 ，且 判断 文件属性（f10 只读），因为是打开故而 文件属性与打开是符合的
	   /// 可以打开文件， 

// UFD   // Tom，文件名，文件属性，记录长度，文件地址 




//UFD*ufd=new UFD[10];  //最多十个文件  ------>每个用户 

//userMFD*u=new userMFD[10];

      // 最多十个用户 
//UOF*uof=new UOF[6];            //用户打开的文件名，最多为6个
 
 
void zhishi(){
	cout<<"\t*"<<"***************************************"<<endl;
	cout<<"\t*"<<"  欢迎来到文件管理！                  *"<<endl; 
	cout<<"\t*"<<"------------------------------------  *"<<endl; 
	cout<<"\t*"<<"  指令               功能             *"<<endl; 
	cout<<"\t*"<<"--------------------------------------*"<<endl;
	cout<<"\t*"<<"  ls                列文件目录        *"<<endl; 
	cout<<"\t*"<<"  create            建立文件          *"<<endl;
	cout<<"\t*"<<"  open              打开文件          * "<<endl;
	cout<<"\t*"<<"  write             写文件            * "<<endl;
	cout<<"\t*"<<"  read              读文件            * "<<endl;
	cout<<"\t*"<<"  close             关闭文件          * "<<endl;
	cout<<"\t*"<<"  del               删除文件          * "<<endl;
	cout<<"\t*"<<"  end               程序结束          * "<<endl;
	cout<<"\t*"<<"  zhuce             注册用户          * "<<endl;
	cout<<"\t*"<<"                                      *"<<endl;
	 cout<<"\t*"<<"-------------------------------------*"<<endl; 
	
}

void deletefile(string n) {       //删除文件  
      cout<<endl;
	  cout<<"请输入需要删除的文件名称：";
	  string txtname;
	  cin>>txtname;
	  bool ufpan=false;
	  bool uofpan=false;
	  for(int i=0;i<10;i++){
	  	    if(u[i].username==n){
	  	    	 for(int j=0;j<10;j++){
	  	    	 	if(u[i].ufdfile[j].txtname==txtname){
	  	    	 		ufpan=true;
						   for(int k=0;k<6;k++){
						   	    if(u[i].uofopen[k].txtname==txtname){
						   	       // 
						   	       uofpan=true;
						   	         //消除在UOF中的登记栏
								     strcpy(u[i].uofopen[k].txtname,"0");
								     strcpy(u[i].uofopen[k].attribute,"0");
								      u[i].uofopen[k].loginlen=0;
								      u[i].uofopen[k].read=0;
								      u[i].uofopen[k].state=0;
								      u[i].uofopen[k].write=0;
								      
								      //消除在UFD中的登记栏 
									  strcpy(u[i].ufdfile[j].txtname,"0");
									  strcpy(u[i].ufdfile[j].attribute,"0"); 
								      u[i].ufdfile[j].loginlen=0;
								      u[i].ufdfile[j].txtlocation=0;
								      
								      cout<<"删除文件成功！"<<endl;
								   }
						   }
						   if(!uofpan){
						   	cout<<"UOF中没有该文件！"<<endl;
						   	//消除在UFD中的登记栏 
									  strcpy(u[i].ufdfile[j].txtname,"0");
									  strcpy(u[i].ufdfile[j].attribute,"0"); 
								      u[i].ufdfile[j].loginlen=0;
								      u[i].ufdfile[j].txtlocation=0;
								      cout<<"在UFD中删除成功！"<<endl;
								      
						   } 
					   }
				   }
				   if(!ufpan){
				   	cout<<"ufd没有该文件！"<<endl;
				   	
				   }
			  }
	  } 
}


void closefile(string n){     //关闭文件 
      cout<<endl;
	  cout<<"请输入需要关闭文件的名称：";
	  string txtname;
	  cin>>txtname;
	  bool uofpan=false; 
	  for(int i=0;i<10;i++){
	  	if(u[i].username==n){
	  		for(int j=0;j<6;j++){
	  			if(u[i].uofopen[j].txtname==txtname){
	  				uofpan=true;
	  				 //如果存在 
					 if(u[i].uofopen[j].state==1)   //0为打开 ，1为建立  
					 {
					 	cout<<endl;
					 	cout<<"文件为建立状态！"<<endl; 
					 	cout<<"正在结束该文件..."<<endl;
						//将该文件在uof中的登记栏置为0
						strcpy(u[i].uofopen[j].txtname,"0");
						strcpy(u[i].uofopen[j].attribute,"0");
						u[i].uofopen[j].loginlen=0;
						u[i].uofopen[j].read=0;
						u[i].uofopen[j].state=0;
						u[i].uofopen[j].write=0;
					 
						cout<<"关闭文件成功！"<<endl;
					 }
					 else{
					 	  //如果不是建立状态  
						   if(u[i].uofopen[j].state==0){  //等于 0为打开状态 
						     cout<<"正在结束该文件..."<<endl;
							  strcpy(u[i].uofopen[j].txtname,"0");
						strcpy(u[i].uofopen[j].attribute,"0");
						u[i].uofopen[j].loginlen=0;
						u[i].uofopen[j].read=0;
						u[i].uofopen[j].state=0;
						u[i].uofopen[j].write=0;
						   	cout<<"关闭文件成功！"<<endl;
						   }
						   else{
						   	 cout<<"文件已打开！"<<endl; 
						   }
					 } 
				  }
				  
			  }
			  if(!uofpan){
			  	
			  	cout<<"该文件未在UOF中注册！"<<endl;
			  }
			  
			  
		  }
      
  }
      
      
          

} 


void readfile(string n){
	   // 输入需要的文件名 ，查找uof，
	   cout<<endl;
	   cout<<"输入读文件的名称：";
	   string txtname;
	   cin>>txtname;
	   bool uofpan=false;
	   for(int i=0;i<10;i++){
	   	if(u[i].username==n){
	   		 for(int j=0;j<6;j++){
	   		 	 if(u[i].uofopen[j].txtname==txtname){
	   		 	 	uofpan=true;  //
	   		 	 	// 从读指针得到，当前地址，
					 //按读长度，把记录信息读出送给用户 
					 cout<<endl;
	   		 	 	 cout<<"输出记录长度："<<u[i].uofopen[j].loginlen<<endl;
					 // 修改读指针 
					 u[i].uofopen[j].read=u[i].uofopen[j].loginlen;  //
					 cout<<endl;
					 cout<<"读文件成功！"<<endl; 
					 }
				}
				if(!uofpan){
					cout<<"文件未打开，不能读"<<endl; 
				}
		   }
	   } 
	
	
	
	
	
	
	
	 
}

void writefile(string n){   //写文件 
       // 输入文件名 ，
       cout<<"输入要写的文件名称：";
       string txtname;
       cin>>txtname;
       
       bool uofpan=false;
       string lin="1";
       string shxin="w";  //为w 可以写 ， 
	   for(int i=0;i<10;i++){
	   	if(u[i].username==n){
	   	       for(int j=0;j<6;j++){
	   	       	if(u[i].uofopen[j].txtname==txtname){
	   	       		uofpan=true;
	   	       		
	   	       		     if(u[i].uofopen[j].state==1){
	   	       		           //把记录信息写到写指针指向的磁盘块
							u[i].uofopen[j].write=u[i].uofopen[j].loginlen;  //
							cout<<"磁盘块号为："<<u[i].uofopen[j].write<<endl; 
							cout<<"写文件成功！"<<endl;
						  }
						  else{
						  	cout<<"该文件状态不是建立状态！"<<endl;
						  	if(strcmp(u[i].uofopen[j].attribute,shxin.c_str())==0){  //等于 w则可以 
						  		cout<<"是否顺序修改(y/n)：";
								  string xiu;
								  cin>>xiu;
								  if(xiu=="y"){
								  
								  	cipan[u[i].uofopen[j].loginlen]=u[i].uofopen[j].loginlen+10;
								  	cout<<"磁盘号为："<<cipan[u[i].uofopen[j].loginlen]<<endl;
									  cout<<"写文件成功！"<<endl; 
								  } 
								  else{
								  	 //将写指针取出 
								  	cipan[u[i].uofopen[j].write]=u[i].uofopen[j].loginlen;
								  	cout<<"磁盘号为："<<cipan[u[i].uofopen[j].write]<<endl;
								  	cout<<"写文件成功！"<<endl;
								  }
							  }
							  else{
							  	cout<<"操作不合法，不能写！"<<endl;
							  } 
						  }
					  }
				  }
				  if(!uofpan){
				  	cout<<"文件不存在，不能写！"<<endl; 
				  }
		   }
	   } 
	  	    
	
} 


void open(string n){   //打开文件 
        // 从UFD中查询是否存在该文件，查UOF中是否存在该文件 ，且查看文件属性是否与操作类型相符，与文件状态如何 
		cout<<"请输入需要打开文件的名称：";
		string opentxt;
		cin>>opentxt;
		string sxin="r"; 
		bool ufdpan=false;
		bool uofpan=false;
		 
	//	w 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
		
		for(int i=0;i<10;i++){
			if(u[i].username==n){
				for(int j=0;j<10;j++){
					if(u[i].ufdfile[j].txtname==opentxt){     //////
						cout<<"UFD中存在该文件！"<<endl;
						ufdpan=true;
						for(int k=0;k<6;k++){
							if(u[i].uofopen[k].txtname==opentxt){
								uofpan=true;
							cout<<"UOF中存在该文件!"<<endl;   
							//且操作属性open都为 r //都为读，且判断文件的状态，如为 建立 1， 打开为 0 
							//建立则不能打开，
					       if(u[i].uofopen[k].state==0){  //等于只读 ， 
								cout<<"文件已打开！"<<endl;   //0 为打开 
							}
							else{
								cout<<"正在建立，不能打开！"<<endl;
							} 
						}
							
						}
					
						if(!uofpan){
							cout<<endl;
							cout<<"UOF中不存在该文件！"<<endl;
							if(strcmp(u[i].ufdfile[j].attribute,sxin.c_str())==0){
								cout<<"文件属性与操作类型相符！"<<endl;
								//在 uof中进行登记 
								//这里应该来个判断uof中是否存在空间  
								strcpy(u[i].uofopen[j].txtname,opentxt.c_str());  //登记名称 
							    u[i].uofopen[j].loginlen=u[i].ufdfile[j].loginlen;
							    strcpy(u[i].uofopen[j].attribute,u[i].ufdfile[j].attribute);
							    u[i].uofopen[j].write=u[i].ufdfile[j].txtlocation;
							    u[i].uofopen[j].read=u[i].ufdfile[j].txtlocation;
							    //应该将当前文件状态设置为 0 打开
								u[i].uofopen[j].state=0; 
							    cout<<"文件打开成功！"<<endl;
							    
							    
							}
							else{
								cout<<"操作不合法，不能打开！"<<endl;
							}
							
							
							
						}
							
					
						
					} //
					
					
				}
				if(!ufdpan){
					cout<<"文件不存在，不能打开"<<endl;
				}
			}
		} 
	
}



void  lsmulu(string n){   //列文件目录 
       //显示文件名，文件属性，记录长度，文件首地址 
	  //显示“用户名为***的用户拥有文件
	  //显示所有文件 
	  for(int i=0;i<10;i++){
	  	     if(u[i].username==n){  //唯一用户名 
	  	     	  cout<<"用户名为"<<u[i].username<<"的用户拥有文件"<<endl;
					 cout<<"文件名        文件属性       记录长度      文件首地址"<<endl;
					 for(int j=0;j<10;j++){
					 	cout<<u[i].ufdfile[j].txtname<<"\t\t"<<u[i].ufdfile[j].attribute<<"\t\t"<<u[i].ufdfile[j].loginlen;
					 	cout<<"\t\t"<<u[i].ufdfile[j].txtlocation<<endl;
					 } 
	  	     	  
			   }
	  } 
	
	
	
} 
void create(string n){   //传递用户名过来  
     //cout<<"这里根据书上所写的算法来进行描述"<<endl;
     cout<<endl; 
     cout<<"输入文件名：";
	 // 首先创建文件 ，判断 UFD 是否存在 文件 
	 // 按照书中 所写进行即可  //需要在 MFD中得到指定的用户，然后在那个用户下面建立文件 即可
	 bool panduan=false; 
    string txtname; 
    cin>>txtname; // 
    cout<<"输入记录长度：";
    int log;
    cin>>log;
    cout<<"输入文件属性（w/r）：";   //只读或者能写能读 
	string shuxing;
	cin>>shuxing;
	
	bool puof=true;
	bool pufd=false; 
	bool puofcunzai=false;    //  uof是否存在空间  
	 
    for(int i=0;i<10;i++){
    	 if(u[i].username==n){  //设定 得到唯一的用户 
		    for(int j=0;j<10;j++){   //ufd中进行循环查看是否存在 一样的，
		   
			    if(u[i].ufdfile[j].txtname==txtname){
			    	panduan=true;   //如果存在就不执行 
			    	cout<<"有同名文件，不能建立"<<endl;
				} 
			}
    	 	if(!panduan){
    	 		 // 可以建立  ，查询是否存在空间  i固定 ，而
    	 		 
    	 		 int k;
    	 		 string lin="0";
				  for(int k=0;k<10;k++){
				   
				  	if(strcmp(u[i].ufdfile[k].txtname,lin.c_str())==0){
				  	// 如果存在空闲 的则进行插入 这里应该要进行初始化  
				    	pufd=true;   //ufd有空闲  
				  	 
		             cout<<"UFD中存在空闲登记栏！"<<endl;
		             cout<<endl;
		                    // 在 uof中找 出空闲块  总共有六个 
							for(int f=0;f<6;f++){
								if(strcmp(u[i].uofopen[f].txtname,"0")==0)
								{   
								     puofcunzai=true;  
								     puof=false;  //
									 cout<<"UOF中存在空登记栏！"<<endl;
									 cout<<endl;
								    	strcpy(u[i].ufdfile[k].txtname,txtname.c_str()); 
									 strcpy(u[i].uofopen[f].txtname,txtname.c_str());  //文件名 
									 u[i].uofopen[f].loginlen=log;
								   	strcpy(u[i].uofopen[f].attribute,shuxing.c_str());  //文件 属性 
								    u[i].ufdfile[k].loginlen=log;
								    strcpy(u[i].ufdfile[k].attribute,shuxing.c_str());  //文件属性  
								    // 登记地址，登记状态，写指针 
									u[i].ufdfile[k].txtlocation=log;  //ufd 地址
									u[i].uofopen[f].state=1;    //登记状态为 建立 1， 打开为 0  
									u[i].uofopen[f].write=log;  //写指针为 i 
								      //每次 只建立一个即可  
								     
								}
								 if(!puof){
								 	cout<<"建立成功！"<<endl;
								 	break; 
								 }
								 
							}
							if(!puofcunzai){
								cout<<"磁盘无空闲块，不能建立"<<endl;  // 
							} 
							break;  //仅是执行一次  
					  }
				  	        
				  }
				  if(!pufd){
				  	cout<<"UFD中无空登记栏不能建立"<<endl; 
				  }
			
				  	
				  } 
			 } 
		 
	} 
	
	
	 
	  
} 

void xuanze(string n){
	


    cout<<"E:\\USER\\testczxt>"<<n<<"文件夹>"; 
	 string s1;
     cin>>s1;
     if(s1=="zhuce"){
     	zhuce();
     	
     	denglu();
	 }
     if(s1=="end"){
     	
     	cout<<"程序退出成功！"<<endl;
		 exit(0);
	 }
     if(s1=="del"){
     	deletefile(n);
     	xuanze(n);
	 }
     if(s1=="close"){
     	closefile(n);
     	xuanze(n);
	 }
     if(s1=="read"){
     	readfile(n);
     	xuanze(n);
	 }
     if(s1=="write"){
     	writefile(n);
     	xuanze(n);
	 }
     if(s1=="ls"){
     	lsmulu(n);
     	xuanze(n);
	 }
     if(s1=="dir"){
     	 sRoot = "E:\\USER\\testczxt\\";  //根目录
     	 string n1="文件夹"; 
         szhongjian =n; //中间路径
         string n2=szhongjian+n1;
         sEnd = "\\"; 
         sPath = sRoot + n2;
     	Move(sPath);     // 查看文件目录  
     	xuanze(n); 
	 }
	 
     if(s1=="create"){  //
     
      // cout<<"只是模拟即可 ，不需要实际的创建文件或者文件夹"<<endl; 
          create(n);  //
          xuanze(n);
	   }
	   if(s1=="open"){
	   	 open(n);
	   	 xuanze(n);
	   	 
	 }
	 
	
}
 
void zhuce(){    //注册函数  
    	 cout<<"请输入用户名：";
         for(int i=0;i<10;i++){
         if(u[i].username=="0"){
         cin>>u[i].username;
         cout<<"注册成功！"<<endl;
		 //这里应该为 其分配地址  用户名 文件的地址  从中 选择一个地址给其 即可  
		  
         denglu();
         break;  //只注册一个  
	 }
  } 
}

void initMFD(){
	// 之前已经有两个  
	for(int i=2;i<10;i++){  //初始化了 mfd  
		u[i].username="0";  //初始化为0  
		
	}
	cout<<"初始化MFD目录成功！"<<endl;
	// 初始化 UFD   现在为其他的几个预留了一些位置，之后还要初始化数据，现在先按照有的进行初始化  
     string lin="0";
	for(int i=0;i<10;i++){  //第一个也要赋值 txtname ... 
		for(int j=3;j<10;j++){
			
          strcpy(u[i].ufdfile[j].txtname,lin.c_str());//初始化为 0 
		}
	}
	cout<<"初始化UFD目录成功！"<<endl;
	//接下来便是初始化 UOF 已经打开文件名表  目前规定 都打开两个 其余四个为空位  
	for(int i=0;i<10;i++){
		for(int j=2;j<6;j++){
			strcpy(u[i].uofopen[j].txtname,"0");
 	}
	} 
	cout<<"初始化UOF文件成功！"<<endl;
	cout<<endl; 
	
} 


/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void denglu(){

	bool panduan=false;

	cout<<"请输入用户名：";
	string n1;
	cin>>n1;
	   
		int j=0;
		while(u[j].username!="0"){
			if(u[j].username==n1){
				
			cout<<"欢迎用户："<<u[j].username<<endl; 
			panduan=true; 
			zhishi();   //将指令只显示一次即可  
	    	string n=u[j].username;  //得到 username 
	    
			xuanze(n);
			
           }
		     j++;
		}
		
	
		
		if(!panduan){
			cout<<"无此用户！"<<endl;
			cout<<"是否选择注册（y/n)?"<<endl;
			char x;
			cout<<":";
			cin>>x;
			if(x=='y'||x=='Y'){
			zhuce();	
			} 
			else{
				cout<<"成功退出！"<<endl;
				exit(0);
			}
		}
	
	
} 


int main(int argc, char** argv) {

    
	        initMFD();
        	denglu();
    		
    		
    
		
		
		

	    

	
     return 0;
}