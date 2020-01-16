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
struct entry
{
    string symbol;
    int address;
};
std::vector<struct entry> v;
void initialise(){
    v.pb({"SP", 0});
    v.pb({"LCL", 1});
    v.pb({"ARG", 2});
    v.pb({"THIS", 3});
    v.pb({"THAT", 4});
    v.pb({"R0",0});
    v.pb({"R1",1});
    v.pb({"R2",2});
    v.pb({"R3",3});
    v.pb({"R4",4});
    v.pb({"R5",5});
    v.pb({"R6",6});
    v.pb({"R7",7});
    v.pb({"R8",8});
    v.pb({"R9",9});
    v.pb({"R10",10});
    v.pb({"R11",11});
    v.pb({"R12",12});
    v.pb({"R13",13});
    v.pb({"R14",14});
    v.pb({"R15",15});
    v.pb({"SCREEN", 16384});
    v.pb({"KBD", 24576});   
}
void add(struct entry item){
    int i;
    for (i = 0; i <v.size(); ++i)
        if(v[i].symbol.compare(item.symbol)==0)
            break;
    if(i==v.size())
        v.pb(item);
    else
        v[i].address=item.address;
}
int getAddress(string s){
    For(i, v.size())
        if(!s.compare(v[i].symbol))
            return(v[i].address);
    return(-1);
}
void convert(int k, bool ans[]){
  For(i, 16){
    ans[i]=k%2;
    k/=2;
  }
}
string jump(string i){
  if(!i.compare("JGT")) return("001");
  if(!i.compare("JEQ")) return("010");
  if(!i.compare("JGE")) return("011");
  if(!i.compare("JLT")) return("100");
  if(!i.compare("JNE")) return("101");
  if(!i.compare("JLE")) return("110");
  if(!i.compare("JMP")) return("111");
}
string dest(string i){
  string a="000";
  For(j,i.length())
  {
    if(i[j]=='A')
      a[0]='1';
    else if(i[j]=='D')
      a[1]='1';
    else
      a[2]='1';
  }
  return a;
}
string comp(string i){
  if(!i.compare("0")) return("0101010");
  if(!i.compare("1")) return("0111111");
  if(!i.compare("-1")) return("0111010");
  if(!i.compare("D")) return("0001100");
  if(!i.compare("A")) return("0110000");
  if(!i.compare("M")) return("1110000");
  if(!i.compare("!D")) return("0001101");
  if(!i.compare("!A")) return("0110001");
  if(!i.compare("!M")) return("1110001");
  if(!i.compare("-D")) return("0001111");
  if(!i.compare("-A")) return("0110011");
  if(!i.compare("-M")) return("1110011");
  if(!i.compare("D+1")) return("0011111");
  if(!i.compare("1+D")) return("0011111");
  if(!i.compare("1+A")) return("0110111");
  if(!i.compare("A+1")) return("0110111");
  if(!i.compare("1+M")) return("1110111");
  if(!i.compare("M+1")) return("1110111");
  if(!i.compare("D-1")) return("0001110");
  if(!i.compare("-1+D")) return("0001110");
  if(!i.compare("A-1")) return("0110010");
  if(!i.compare("-1+A")) return("0110010");
  if(!i.compare("M-1")) return("1110010");
  if(!i.compare("-1+M")) return("1110010");
  if(!i.compare("D+A")) return("0000010");
  if(!i.compare("A+D")) return("0000010");
  if(!i.compare("D+M")) return("1000010");
  if(!i.compare("M+D")) return("1000010");
  if(!i.compare("D-A")) return("0010011");
  if(!i.compare("-A+D")) return("0010011");
  if(!i.compare("D-M")) return("1010011");
  if(!i.compare("-M+D")) return("1010011");
  if(!i.compare("A-D")) return("0000111");
  if(!i.compare("-D+A")) return("0000111");
  if(!i.compare("M-D")) return("1000111");
  if(!i.compare("-D+M")) return("1000111");
  if(!i.compare("D&A")) return("0000000");
  if(!i.compare("A&D")) return("0000000");
  if(!i.compare("D&M")) return("1000000");
  if(!i.compare("M&D")) return("1000000");
  if(!i.compare("D|A")) return("0010101");
  if(!i.compare("A|D")) return("0010101");
  if(!i.compare("D|M")) return("1010101");
  if(!i.compare("M|D")) return("1010101");
}
string Type1 (string co){
    string a="0000000000000111";
    int i=0;
    string s1, s2;
    while(co[i]!=';'){
      s1+=co[i];
      i++;
    }
    i++;
    while(i<co.size()){
      s2+=co[i];
      i++;
    }
    string b1="0000000", b2="000";
    b1=comp(s1);
    reverse(all(b1));
    b2=jump(s2);
    reverse(all(b2));
    For(i, 3)
      a[i]=b2[i];
    For(i, 3)
      a[i+3]='0';
    For(i,7)
      a[i+6]=b1[i];
     return(a);
}
string Type2(string co){
    string a="0000000000000111";
    int i=0;
    string s1, s2;
    while(co[i]!='=')
    {
      s1+=co[i];
      i++;
    }
    i++;
    while(i<co.size()){
      s2+=co[i];
      i++;
    }
    string b1="000", b2="0000000";
    b1=dest(s1);
    b2=comp(s2);
    reverse(all(b1));
    reverse(all(b2));
    For(i, 3)
      a[i]='0';
    For(i, 3)
      a[i+3]=b1[i];
    For(i,7)
      a[i+6]=b2[i];
     return(a);
}
string removeComments(string prgm) { 
    int n = prgm.length(); 
    string res; 
    bool s_cmt = false; 
    bool m_cmt = false; 
    for (int i=0; i<n; i++) 
    { 
        if (s_cmt == true && prgm[i] == '\n') 
            s_cmt = false; 
        else if  (m_cmt == true && prgm[i] == '*' && prgm[i+1] == '/') 
            m_cmt = false,  i++; 
        else if (s_cmt || m_cmt) 
            continue; 
        else if (prgm[i] == '/' && prgm[i+1] == '/') 
            s_cmt = true, i++; 
        else if (prgm[i] == '/' && prgm[i+1] == '*') 
            m_cmt = true,  i++; 
        else  res += prgm[i]; 
    } 
    return res; 
} 
main()
{ 
    initialise();
    string stin, stout;
    cin>>stin;
    fstream inFile, intFile, outFile, lfile, mfile;
    int i=0,j=16;
    int ino=0;
    string nexco,pco, remcom, com;
    inFile.open(stin);
    intFile.open("mid.txt");
    mfile.open("mid1.txt");
    while(getline(inFile, com))
      remcom+=com;
    string remdcom=removeComments(remcom);
    mfile<<remdcom;  
    while(getline(mfile, nexco))
    {
        pco.clear();
        For(i, nexco.size())
        {
           if(nexco[i]!=' ' && nexco[i]!='\t')
              pco+=nexco[i];
        }
        if(pco.size() || pco[0]!='\n' || pco.size()-1)
        {
          int st,fl=0;
          For(i,pco.length())
            if(pco[i]=='(')
              fl=1, st=i;
          if(fl==1)
          {
            int e;
            For(i, pco.length())
              if(pco[i]==')')
                e=i;
            string t=pco.substr(st+1, e-st-1);
            add({t,ino});
            pco.erase(st, e-st+1);

          }
          if(pco.size() || pco[0]!='\n' || pco.size()-1)
            if((64<=pco[0] && pco[0]<=122) ||(pco[0]>=48 && pco[0]<58))
              intFile<<pco<<endl;ino++;
        }
    }
    inFile.close();
    intFile.close();
    mfile.close();
    lfile.open("mid.txt");
    stout=stin.substr(0, stin.length()-3);
    stout+="hack";
    outFile.open(stout);
    bool assin[16];
    string co;
    while(getline(lfile, co))
    {
      if(co[0]==64)
      {
        ZERO(assin);
        co.erase(co.begin());
        co.erase(co.end()-1);
        assin[15]=0;
        if(co[0]<48 || co[0]>57)
        {
          int t=getAddress(co); 
          if(t==-1){
            add((entry){co, j}); 
            t=j;j++;
          }
          convert(t, assin);
        }
        else
        {
          int t=stoi(co);
          convert(t,assin);
        }
        Rev(i,16)
          outFile<<assin[i];
        outFile<<endl;
        ZERO(assin);
      }
      else
      {
        string fin;
        int f1=1, f2=1;
        co.erase(co.end()-1);
        For(i, co.length()){
          if(co[i]=='=')
            f1=0;
          if(co[i]==';')
            f2=0;
        }
        if(f1==1 && f2==0)
          fin=Type1(co);
        else if (f1==0 && f2==1)
          fin=Type2(co);
        else{
          cout<<"Incorrect Instruction"<<endl;
          exit(0);
        }
        Rev(i,16)
          outFile<<fin[i];
        outFile<<endl;
        fin.clear();
      }
      co.clear();
    }
    return 0;
}
