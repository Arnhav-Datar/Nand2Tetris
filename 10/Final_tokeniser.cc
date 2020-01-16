#include <bits/stdc++.h>
using namespace std;
#define fio ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL)
#define For(i,n) for(int i=0;i<(int)n;i++)
const string WHITESPACE = " \n\r\t\f\v";
string rc(string p) 
{ 
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
bool isWhite(char c){
    For(i, WHITESPACE.length()) if(c==WHITESPACE[i]) return true;
    return false;
}
bool isKeyword(string s){
    if(s=="class" || s=="constructor" ||s=="method" ||s=="function" ||s=="field" ||s=="static" ||s=="var" ||s=="int" ||s=="char"
    ||s=="boolean" ||s=="void" ||s=="true" ||s=="false"||s=="null" ||s=="let"||s=="this" ||s=="do"||s=="if" ||s=="else"||s=="while"
    ||s=="return") return true;
    return false;
}
main(int argc, char** argv)
{ 
    string inj;
    For(i,strlen(argv[1])) inj+=argv[1][i];

    string outt;
    outt=inj.substr(0, inj.size()-4);
    outt+="tok";

    fstream iF, oF;
    iF.open(inj);
    oF.open(outt);
    oF<<"<tokens>"<<endl;

    string s, stot="";
    while(getline(iF,s)) { if(s.length()) { stot+=s; stot+='\n';} }
    string remd=rc(stot);

    s.clear();
    int i=0;
    bool isSC=false;
    while(i<remd.length())
    {
        if(i!=0 && remd[i-1]=='"' && isSC){
            s.clear();
            for(;i<remd.length() && remd[i]!='"';i++)
                s+=remd[i];
            oF<<"<stringConstant> "<<s<<" </stringConstant>"<<endl;
        }
        else if(isWhite(remd[i]))
            i++;
        else if(('a'<=remd[i] && remd[i]<='z') || ('A'<=remd[i] && remd[i]<='Z') || remd[i]=='_')
        {
            s.clear();
            for(;i<remd.length() && ('a'<=remd[i] && remd[i]<='z') || ('A'<=remd[i] && remd[i]<='Z') || remd[i]=='_' ;i++)
                s+=remd[i];
            if(isKeyword(s))
                oF<<"<keyword> "<<s<<" </keyword>"<<endl;
            else
                oF<<"<identifier> "<<s<<" </identifier>"<<endl;
        }
        else if(remd[i]<='9' && remd[i]>='0')
        {
            s.clear();
            for(;remd[i]<='9' && remd[i]>='0' && i<remd.length();i++)
                s+=remd[i];
            oF<<"<integerConstant> "<<s<<" </integerConstant>"<<endl;
        }
        else
        {
            if(remd[i]!='"')
            {
              if(remd[i]=='<') 
                oF<<"<symbol> &lt; </symbol>"<<endl;
              else if(remd[i]=='>') 
                oF<<"<symbol> &gt; </symbol>"<<endl;
              else if(remd[i]=='&')
                oF<<"<symbol> &amp; </symbol>"<<endl;
              else
                oF<<"<symbol> "<<remd[i]<<" </symbol>"<<endl;
            }
            if(remd[i]=='"') isSC=!isSC;
              i++;
        }

    }
    oF<<"</tokens>"<<endl;
    return 0;
        
}
