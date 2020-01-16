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
#define o(x) of<<(x)<<endl
const string WHITESPACE = " \n\r\t\f\v";
inline string ltrim(const string& s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == string::npos) ? "" : s.substr(start);
}
inline string rtrim(const string& s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == string::npos) ? "" : s.substr(0, end + 1);
}
inline string trim(const string& s)
{
  return rtrim(ltrim(s));
}
fstream of;
int counter=0;
string name;
string rc(string p) { 
    int n = p.length(); 
    string ans; 
    int s=0,m=0; 
    for (int i=0; i<n; i++) 
    { 
        if (s && p[i+1] == '\n')
			s=0;
        else if  (m && p[i] == '*' && p[i+1] == '/') 
            m=0,  i++; 
        else if (s || m) 
            continue; 
        else if (p[i] == '/' && p[i+1] == '/') 
            s=1, i++; 
        else if (p[i] == '/' && p[i+1] == '*') 
            m=1,  i++; 
        else 
        	ans += p[i]; 
    } 
    return ans; 
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
	of<<"A=M"<<endl;
	of<<"M=0"<<endl;
	of<<"@SP"<<endl;
	of<<"M=M+1"<<endl;
}
void invalid(){
	cout<<"Invalid Instruction"<<endl;
	exit(0);
}
void arithmetic(string s){	
	if(s.compare("add")==0){
		of<<"@SP"<<endl;
		of<<"AM=M-1"<<endl;
		of<<"D=M"<<endl;
		of<<"A=A-1"<<endl;
		of<<"M=D+M"<<endl;
	}else if(s.compare("sub")==0){
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
		of<<"@"<<name<<"J"<<counter<<endl;
		of<<"D;JEQ"<<endl;
		of<<"@SP"<<endl;
		of<<"A=M-1"<<endl;
		of<<"M=0"<<endl;
		of<<"("<<name<<"J"<<counter<<")"<<endl;
		counter++;
	}else if(s.compare("gt")==0){
		of<<"@SP"<<endl;
		of<<"AM=M-1"<<endl;
		of<<"D=M"<<endl;
		of<<"A=A-1"<<endl;
		of<<"D=M-D"<<endl;
		of<<"M=-1"<<endl;
		of<<"@"<<name<<"J"<<counter<<endl;
		of<<"D;JGT"<<endl;
		of<<"@SP"<<endl;
		of<<"A=M-1"<<endl;
		of<<"M=0"<<endl;
		of<<"("<<name<<"J"<<counter<<")"<<endl;
		counter++;
	}else if(s.compare("lt")==0){
		of<<"@SP"<<endl;
		of<<"AM=M-1"<<endl;
		of<<"D=M"<<endl;
		of<<"A=A-1"<<endl;
		of<<"D=M-D"<<endl;
		of<<"M=-1"<<endl;
		of<<"@"<<name<<"J"<<counter<<endl;
		of<<"D;JLT"<<endl;
		of<<"@SP"<<endl;
		of<<"A=M-1"<<endl;
		of<<"M=0"<<endl;
		of<<"("<<name<<"J"<<counter<<")"<<endl;
		counter++;
	}
	else if(s=="return"){
		o("@LCL");
		o("D=M");
		//o("@R13");
		//o("M=D");
		o("@5");
		o("A=D-A");
		o("D=M");
		o("@R14");
		o("M=D");
		
        o("@SP");
		o("AM=M-1");
		o("D=M");
		o("@ARG");
		o("A=M");
		o("M=D");
		
        o("@ARG");
		o("D=M");
		o("D=D+1");
		o("@SP");
		o("M=D");
		
        o("@LCL");
		o("AM=M-1");
		o("D=M");
		o("@THAT");
		o("M=D");
		
        o("@LCL");
		o("AM=M-1");
		o("D=M");
		o("@THIS");
		o("M=D");
		
        o("@LCL");
		o("AM=M-1");
		o("D=M");
		o("@ARG");
		o("M=D");
		
        o("@LCL");
		o("AM=M-1");
		o("D=M");
		o("@LCL");
		o("M=D");
		
        o("@R14");
		o("A=M");
		o("0;JMP");
	}
	else{
		cout<<"INVALID INSTRUCTION "<<s<<endl;
	}
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
			of<<'@'<<name<<val<<endl;
			of<<"D=M"<<endl;
			push();
		}else if(s2.compare("temp")==0){
			of<<"@"<<val+5<<endl;
			of<<"D=M"<<endl;
			push();
		}else
			invalid();
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
			of<<'@'<<name<<val<<endl;
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
		}else
			invalid();
	}
	else if(s1=="call"){
		of<<"@"<<s2<<"_"<<"ret"<<endl;
		of<<"D=A";
		push();

		of<<"@LCL"<<endl<<"D=M"<<endl;
		push();
		of<<"@ARG"<<endl<<"D=M"<<endl;
		push();
		of<<"@THIS"<<endl<<"D=M"<<endl;
		push();
		of<<"@THAT"<<endl<<"D=M"<<endl;
		push();

		of<<"@SP"<<endl;
		of<<"D=M"<<endl;
		of<<"@LCL"<<endl<<"M=D"<<endl;
		of<<"@"<<val+5<<endl;
		of<<"D=D-A"<<endl;
		of<<"@ARG"<<endl<<"M=D"<<endl;

		of<<"(f"<<name<<s2<<")"<<endl;
		of<<"0;JMP"<<endl;

		of<<"(f"<<s2<<"_"<<"ret)"<<endl;
	}
	else if(s1=="function"){
		of<<"(f"<<name<<s2<<")"<<endl;
		of<<"@SP"<<endl;
		For(i,val) push2();
	}
	else
		invalid();

}
void flow(string s){
	string s1="", s2="";
	int i=0;
	while(s[i]!=' '){
		s1+=s[i];
		i++;
	}
	i++;
	while(i<s.length()){
		s2+=s[i];
		i++;
	}
	if(s1=="label"){
		of<<"("<<name<<s2<<")"<<endl;
	}
	else if(s1=="goto"){
		of<<"@"<<name<<s2<<endl;
		of<<"0;JMP"<<endl;
	}
	else if(s1=="if-goto"){
		of<<"@SP"<<endl;
		of<<"AM=M-1"<<endl;
		of<<"D=M"<<endl;
		of<<"@"<<s2<<endl;
		of<<"D;JNE"<<endl;
	}
}

main(int argc, char *argv[])
{ 
 	fstream inFile, aFile, bFile;
 	inFile.open(argv[1]);
 	aFile.open("mid1.txt");
 	string stout;
 	For(i, strlen(argv[1])-2)
 		stout+=argv[1][i];
 	name=stout;	
 	stout+="asm";
 	of.open(stout);
 	string s, stot="";
 	while(getline(inFile,s))
 	{
 		if(s.length()) 
 		{
 			stot+=s;
 			stot+='\n';
 		}
 	}
 	string remd=rc(stot);
 	string remd1;
 	For(i, remd.length()-1){
 		if(!(remd[i]=='\n' && remd[i+1]=='\n'))
 			remd1+=remd[i];
 	}
 	aFile<<remd1;
 	aFile.close();
 	bFile.open("mid1.txt");
 	while(getline(bFile,s)){
 		int flg=0;
 		For(i, s.length()) if(64<s[i]<128) flg=1;
 		string s1=s;
 		s.clear();
 		For(i, s1.length()) if(!(s1[i]==' ' && s1[i+1]==' ')) s+=s1[i];
 		s=trim(s);
 		if(s.length()>1 && flg==1){
 			//s+='\n';
 			int cntspc=0;
 			For(i, s.length()){
 				if(s[i]==' ')
 					cntspc++;
 			}
// 			cout<<s<<endl<<endl;
// 			cout<<cntspc<<endl<<endl;
 			if(cntspc==0)
 				arithmetic(s);
 			else if(cntspc==2)
 				memory(s);
            else if(cntspc==1)
              	flow(s);
 		}
 	}
  return 0;
}
