#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#define pb push_back
#define endl "\n"
#define all(v) (v).begin(),(v).end()
#define For(i,n) for(int i=0;i<(int)n;i++)
#define Rev(i,n) for(int i=n-1;i>=0;i--)
#define ZERO(a) memset(a,0,sizeof(a))
fstream of;
int counter=0;
string name;
string rc(string p) { 
    int n = p.length(); 
    string ans; 
    int s=0,m=0; 
    for (int i=0; i<n; i++) 
    { 
        if (s && p[i] == '\n') 
            s=0; 
        else if  (m && p[i] == '*' && p[i+1] == '/') 
            m=0,  i++; 
        else if (s || m) 
            continue; 
        else if (p[i] == '/' && p[i+1] == '/') 
            s=1, i++; 
        else if (p[i] == '/' && p[i+1] == '*') 
            m=1,  i++; 
        else ans += p[i]; 
    } 
    return ans; 
} 
void arithmetic(string s){
	string s1=s;
	s=s1.substr(0, s.size()-1);
	if(s.compare("add")==0){
		of<<"@SP"<<endl;
		of<<"AM=M-1"<<endl;
		of<<"D=M"<<endl;
		of<<"A=A-1"<<endl;
		of<<"M=D+M"<<endl;
	}
	else if(s.compare("sub")==0){
		of<<"@SP"<<endl;
		of<<"AM=M-1"<<endl;
		of<<"D=M"<<endl;
		of<<"A=A-1"<<endl;
		of<<"M=M-D"<<endl;
	}else if(s.compare("and")==0){
		of<<"@SP"<<endl;
		of<<"AM=M-1"<<endl;
		of<<"D=M"<<endl;
		of<<"A=A-1"<<endl;
		of<<"M=D&M"<<endl;
	}else if(s.compare("or")==0){
		of<<"@SP"<<endl;
		of<<"AM=M-1"<<endl;
		of<<"D=M"<<endl;
		of<<"A=A-1"<<endl;
		of<<"M=D|M"<<endl;
	}else if(s.compare("not")==0){
		of<<"@SP"<<endl;
		of<<"A=M-1"<<endl;
		of<<"M=!M"<<endl;
	}else if(s.compare("neg")==0){
		of<<"@SP"<<endl;
		of<<"A=M-1"<<endl;
		of<<"M=-M"<<endl;
	}else if(s.compare("eq")==0){
		of<<"@SP"<<endl;
		of<<"AM=M-1"<<endl;
		of<<"D=M"<<endl;
		of<<"A=A-1"<<endl;
		of<<"D=M-D"<<endl;
		of<<"M=-1"<<endl;
		of<<"@J"<<counter<<endl;
		of<<"D;JEQ"<<endl;
		of<<"@SP"<<endl;
		of<<"A=M-1"<<endl;
		of<<"M=0"<<endl;
		of<<"(J"<<counter<<")"<<endl;
		counter++;
	}else if(s.compare("gt")==0){
		of<<"@SP"<<endl;
		of<<"AM=M-1"<<endl;
		of<<"D=M"<<endl;
		of<<"A=A-1"<<endl;
		of<<"D=M-D"<<endl;
		of<<"M=-1"<<endl;
		of<<"@J"<<counter<<endl;
		of<<"D;JGT"<<endl;
		of<<"@SP"<<endl;
		of<<"A=M-1"<<endl;
		of<<"M=0"<<endl;
		of<<"(J"<<counter<<")"<<endl;
		counter++;
	}else if(s.compare("lt")==0){
		of<<"@SP"<<endl;
		of<<"AM=M-1"<<endl;
		of<<"D=M"<<endl;
		of<<"A=A-1"<<endl;
		of<<"D=M-D"<<endl;
		of<<"M=-1"<<endl;
		of<<"@J"<<counter<<endl;
		of<<"D;JLT"<<endl;
		of<<"@SP"<<endl;
		of<<"A=M-1"<<endl;
		of<<"M=0"<<endl;
		of<<"(J"<<counter<<")"<<endl;
		counter++;
	}else{
		cout<<"INVALID INSTRUCTION "<<s<<endl;
	}
}
void pop(){
	of<<"@SP"<<endl;
	of<<"AM=M-1"<<endl;
	of<<"D=M"<<endl;
	of<<"@R13"<<endl;
	of<<"A=M"<<endl;
	of<<"M=D"<<endl;
}
void push(){
	of<<"@SP"<<endl;
	of<<"A=M"<<endl;
	of<<"M=D"<<endl;
	of<<"@SP"<<endl;
	of<<"M=M+1"<<endl;
}
void push1(string s3){
	of<<"D=M"<<endl;
	of<<"@"<<s3<<endl;
	of<<"A=A+D"<<endl;
	of<<"D=M"<<endl;
}
void push2(){

}
void memory(string s){
	int n=s.length();
	string s1="";
	int nex;
	For(i,n){
		if(s[i]==' '){
			nex=i+1;
			break;
		}
		s1+=s[i];
	}
	string s2="";
	int nex2;
	for(int i=nex; i<n;i++){
		if(s[i]==' '){
			nex2=i;
			break;
		}
		s2+=s[i];
	}
	string s3=s.substr(nex2);


	int val=stoi(s3);

	if(s1.compare("push")==0){
		int x=-1;
		if(s2.compare("argument")==0){
			of<<"@ARG"<<endl;
			push1(s3);
			push();
		}else if(s2.compare("local")==0){
			of<<"@LCL"<<endl;
			push1(s3);
			push();
		}else if(s2.compare("this")==0){
			of<<"@THIS"<<endl;
			push1(s3);
			push();
		}else if(s2.compare("that")==0){
			of<<"@THAT"<<endl;
			push1(s3);
			push();
		}else if(s2.compare("constant")==0){
			of<<"@"<<val<<endl;
			of<<"D=A"<<endl;
			push();
		}else if(s2.compare("pointer")==0){
			of<<"@"<<val+3<<endl;
			of<<"D=M"<<endl;
			push();
		}else if(s2.compare("static")==0){
			of<<'@'<<name<<'.'<<val<<endl;
			of<<"D=M"<<endl;
			push();
		}else if(s2.compare("temp")==0){
			of<<"@"<<val+5<<endl;
			of<<"D=M"<<endl;
			push();
		}
	}
	else if(s1.compare("pop")==0){
		if(s2.compare("argument")==0){
			of<<"@ARG"<<endl;
			of<<"D=M"<<endl;
			of<<"@"<<val<<endl;
			of<<"D=A+D"<<endl;
			of<<"@R13"<<endl;
			of<<"M=D"<<endl;
			pop();
		}else if(s2.compare("local")==0){
			of<<"@LCL"<<endl;
			of<<"D=M"<<endl;
			of<<"@"<<val<<endl;
			of<<"D=A+D"<<endl;
			of<<"@R13"<<endl;
			of<<"M=D"<<endl;
			pop();
		}else if(s2.compare("this")==0){
			of<<"@THIS"<<endl;
			of<<"D=M"<<endl;
			of<<"@"<<val<<endl;
			of<<"D=A+D"<<endl;
			of<<"@R13"<<endl;
			of<<"M=D"<<endl;

			pop();
		}else if(s2.compare("that")==0){
			of<<"@THAT"<<endl;
			of<<"D=M"<<endl;
			of<<"@"<<val<<endl;
			of<<"D=A+D"<<endl;
			of<<"@R13"<<endl;
			of<<"M=D"<<endl;

			pop();
		}else if(s2.compare("pointer")==0){
			of<<"@3"<<endl;
			of<<"D=A"<<endl;
			of<<"@"<<val<<endl;
			of<<"D=A+D"<<endl;
			of<<"@R13"<<endl;
			of<<"M=D"<<endl;

			pop();
		}else if(s2.compare("static")==0){
			of<<'@'<<name<<'.'<<val<<endl;
			of<<"D=A"<<endl;
			of<<"@R13"<<endl;
			of<<"M=D"<<endl;
			pop();
		}else if(s2.compare("temp")==0){
			of<<"@5"<<endl;
			of<<"D=A"<<endl;
			of<<"@"<<val<<endl;
			of<<"D=A+D"<<endl;
			of<<"@R13"<<endl;
			of<<"M=D"<<endl;

			pop();
		}
	}


}
main()
{ 
 	string invm;
 	cin>>invm;
 	fstream inFile, aFile, bFile;
 	inFile.open(invm);
 	aFile.open("mid1.txt");
 	string stout;
 	stout=invm.substr(0, invm.length()-3);
 	name=stout;
 	stout+=".asm";
 	of.open(stout);
 	string s, stot="";
 	while(getline(inFile,s)){
 		if(s.length()) {stot+=s;
 		stot+='\n';}
 	}
 	string remd=rc(stot);
 	aFile<<remd;
 	aFile.close();
 	bFile.open("mid1.txt");
 	while(getline(bFile,s)){
 		int flg=0;
 		For(i, s.length()) if(64<s[i]<128) flg=1;
 		string s1=s;
 		s.clear();
 		For(i, s1.length()) if(!(s1[i]==' ' && s1[i+1]==' ')) s+=s1[i];
 		if(s.length()>1 && flg==1){
 			//s+='\n';
 			int cntspc=0;
 			For(i, s.length()){
 				if(s[i]==' ')
 					cntspc++;
 			}
 			if(cntspc==0){
 				arithmetic(s);
 			}
 			else if(cntspc==2){
 				memory(s);
 			}
 		}
 	}
  return 0;
}
