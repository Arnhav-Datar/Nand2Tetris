#include<bits/stdc++.h>
using namespace std;
#define For(i,n) for(int i=0;i<(int)n;i++)
#define pb push_back
const string WHITESPACE = " \n\r\t\f\v";
fstream iF, oF, ferr;
vector<string> tok;
vector<string> names;
vector<string> xml;
int spc=0;
int ind=0;
void prspc(){ For(i,spc) oF<<"  "; }
void compileStatements();
void compileTerm();
int compileExpressionList();
inline string ltrim(const string& s){
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == string::npos) ? "" : s.substr(start);}
inline string rtrim(const string& s){
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == string::npos) ? "" : s.substr(0, end + 1);}
inline string trim(const string& s){ return rtrim(ltrim(s));}
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
bool isWhite(char c){
    For(i, WHITESPACE.length()) if(c==WHITESPACE[i]) return true;
    return false;
}
bool isKeyword(string s){
    if(s=="class" || s=="constructor" ||s=="method" ||s=="function" ||s=="field" ||s=="static" ||s=="var" ||s=="int" ||s=="char"||s=="boolean" ||s=="void" ||s=="true" ||s=="false"||s=="null" ||s=="let"||s=="this" ||s=="do"||s=="if" ||s=="else"||s=="while"||s=="return") return true;
    return false;
}
bool isKey(string s, string kw){
	trim(s);
	if(s.length()<20) return false;
	if(s.substr(0, 9)=="<keyword>" && s.substr(s.length()-10, 10)=="</keyword>")
		if(trim(s.substr(9, s.length()-20))==kw)
			return true;
	return false;
}
bool isSym(string s, string kw){
	trim(s);
	if(s.length()<18) return false;
	if(s.substr(0, 8)=="<symbol>" && s.substr(s.length()-9, 9)=="</symbol>")
		if(trim(s.substr(8, s.length()-18))==trim(kw))
			return true;
	return false;
}
bool isId(string s){
	trim(s);
	if(s.length()<26) return false;
	if(s.substr(0, 12)=="<identifier>" && s.substr(s.length()-13, 13)=="</identifier>")
		return true;
	return false;
}
void err1() {ferr<<"Syntax Error: ";}
void p(string s){
	prspc();	
	oF<<s<<endl; xml.pb(s);
}
bool isop(string tem)
{	return (isSym(tem, "+") || isSym(tem, "-") || isSym(tem, "*") || isSym(tem, "/")|| isSym(tem, "|")|| isSym(tem, "=")|| isSym(tem, "&amp;")|| isSym(tem, "&lt;")|| isSym(tem, "&gt;") );}
bool isUnaryOp(string tem)
{ return (isSym(tem, "~") || isSym(tem, "-") );}
bool isKeyCon(string tem)
{ return (isKey(tem, "true") || isKey(tem, "false") || isKey(tem, "null") || isKey(tem, "this"));}
bool isIC(string s)
{
	trim(s);
	if(s.length()<32) return false;
	if(s.substr(0, 17)=="<integerConstant>" && s.substr(s.length()-18, 18)=="</integerConstant>")
		return true;
	return false;
}
bool isSC(string s)
{
	trim(s);
	if(s.length()<32) return false;
	if(s.substr(0, 16)=="<stringConstant>" && s.substr(s.length()-17, 17)=="</stringConstant>")
		return true;
	return false;
}
void pr1()
{
	prspc();
	oF<<tok[ind]<<endl;xml.pb(tok[ind]);
	ind++;
}
void chId(){
	if(!isId(tok[ind])){
		err1();
		ferr<<"identifier expected"<<endl;
		//exit(0);
	}
	pr1();
}
void chSym(string s0){
	if(!isSym(tok[ind], s0))
	{
		err1();
		ferr<<s0<<" expected"<<endl;
		//exit(0);
	}
	pr1();
}
void compileSubRoutineCall(){
	chId();
	if(isSym(tok[ind], ".")){
		pr1();
		chId();
		chSym("(");
		if(!isSym(tok[ind], ")"))
			compileExpressionList();
		else
		{
			p("<expressionList>"); 
			p("</expressionList>"); 
		}
		chSym(")");
	}
	else if(isSym(tok[ind], "(")){
		pr1();
		if(!isSym(tok[ind], ")"))
			compileExpressionList();
		else
		{
			p("<expressionList>"); 
			p("</expressionList>"); 
		}chSym(")");
	}
	else{
		err1();
		ferr<<"either . or ( expected"<<endl;
		//exit(0);
	}
}
void compileClassVarDec(){
	p("<classVarDec>");
	spc++;
	pr1();
	pr1();  //to be modified to check type
	chId();
	while(isSym(tok[ind], ","))
	{
		pr1();
		if(isId(tok[ind]))
			pr1();
		else{
			err1();
			ferr<<"identifier expected"<<endl;//exit(0);
		}
	}
	chSym(";");
	spc--;
	p("</classVarDec>");
}
void compileVarDec(){
	p("<varDec>");
	spc++;
	pr1();
	pr1();  //to be modified to check type
	chId();
	while(isSym(tok[ind], ","))
	{
		pr1();
		if(isId(tok[ind]))
			pr1();
		else{
			err1();
			ferr<<"identifier expected"<<endl;//exit(0);
		}
	}
	chSym(";");
	spc--;
	p("</varDec>");
}
void compileParameterList(){
	p("<parameterList>");
	spc++;
	if(isSym(tok[ind], ")")){
		spc--;
		p("</parameterList>");
	}
	else
	{
		while(isSym(tok[ind+2], ",")){
			pr1();
			chId();
			chSym(",");
		}
		if(!isSym(tok[ind+2], ")")){
			err1();
			ferr<<"symbol ) expexted"<<endl;
			//exit(0);
		}
		pr1();
		chId();
		spc--;
		p("</parameterList>");	
	}
}
void compileExpression(){
	p("<expression>");
	spc++;
	compileTerm();
	while(isop(tok[ind])){
		pr1();
		compileTerm();
	}
	spc--;
	p("</expression>");
}
int compileExpressionList(){
	p("<expressionList>");
	spc++;
    int cnt=1;
	compileExpression();
	while(isSym(tok[ind], ",")){
		pr1();
		compileExpression();
        cnt++;
	}
	spc--;
	p("</expressionList>");
    return cnt;
}
void compileTerm(){
	p("<term>");
	spc++;

	if(isIC(tok[ind]) || isSC(tok[ind]) || isKeyCon(tok[ind]) )
		pr1();
	else if( isId(tok[ind]) )
	{
		pr1();
		if(isSym(tok[ind], "["))
		{
			pr1();
			compileExpression();
			chSym("]");
		}
		else if(isSym(tok[ind], ".")){
			pr1();
			chId();
			chSym("(");
			if(!isSym(tok[ind], ")"))
				compileExpressionList();
			else
			{
				p("<expressionList>"); 
				p("</expressionList>"); 
			}
			chSym(")");
		}
	}
	else if(isUnaryOp(tok[ind]))
	{
		pr1();
		compileTerm();
	}
	else if(isSym(tok[ind], "(")){
		pr1();
		compileExpression();
		chSym(")");
	}
	spc--;
	p("</term>");
}
void compileIfStat(){
	p("<ifStatement>");
	spc++;
	pr1();
	chSym("(");
	compileExpression();
	chSym(")");
	chSym("{");
	compileStatements();
	chSym("}");
	if(isKey(tok[ind], "else"))
	{
		pr1();
		chSym("{");
		compileStatements();
		chSym("}");
	}
	spc--;
	p("</ifStatement>");
}
void compileWhileStat(){
	p("<whileStatement>");
	spc++;
	pr1();
	chSym("(");
	compileExpression();
	chSym(")");
	chSym("{");
	compileStatements();
	chSym("}");
	spc--;
	p("</whileStatement>");
}
void compileReturnStat(){
	p("<returnStatement>");
	spc++;
	pr1();
	if(isSym(tok[ind], ";"))
		pr1();
	else
	{
		compileExpression();
		pr1();
	}
	spc--;
	p("</returnStatement>");	
}
void compileDoStat(){
	p("<doStatement>");
	spc++;
	pr1();
	compileSubRoutineCall();
	chSym(";");
	spc--;
	p("</doStatement>");
}
void compileLetStat(){
	p("<letStatement>");
	spc++;
	pr1();
	chId();
	if(isSym(tok[ind], "["))
	{
		pr1();
		compileExpression();
		chSym("]");
	}
	chSym("=");
	compileExpression();
	chSym(";");
	spc--;
	p("</letStatement>");
}
void compileStatements(){
	p("<statements>");
	spc++;
	while(isKey(tok[ind], "if") || isKey(tok[ind], "do") || isKey(tok[ind], "let") || isKey(tok[ind], "while") || isKey(tok[ind], "return"))
	{
		if(isKey(tok[ind], "if"))
			compileIfStat();
		else if(isKey(tok[ind], "do"))
			compileDoStat();
		else if(isKey(tok[ind], "let"))
			compileLetStat();
		else if(isKey(tok[ind], "while"))
			compileWhileStat();
		else if(isKey(tok[ind], "return"))
			compileReturnStat();
	}
	spc--;
	p("</statements>");
}
void compileSubRoutine(){
	p("<subroutineDec>");
	spc++;
	pr1();
	pr1();
	chId();
	chSym("(");
	compileParameterList();
	chSym(")");
	p("<subroutineBody>");
	spc++;
	chSym("{");
	while(isKey(tok[ind], "var"))
		compileVarDec();
	compileStatements();
	chSym("}");
	spc--;
	p("</subroutineBody>");
	spc--;
	p("</subroutineDec>");
}
void compileClass(){
	oF<<"<class>"<<endl;
	spc++;
	if(!isKey(tok[ind], "class"))
    {
		err1();
		ferr<<"Syntax error: class expected."<<endl;
		//exit(0);
	}
	pr1();
	chId();
	chSym("{");
	while(isKey(tok[ind], "static") || isKey(tok[ind], "field"))
		compileClassVarDec();
	while(isKey(tok[ind], "method") || isKey(tok[ind], "function") || isKey(tok[ind], "constructor"))
		compileSubRoutine();
	chSym("}");	 
    spc--; 
    oF<<"</class>"<<endl; 
}
void tokeniser(string x)
{ 
	iF.open(x);
    tok.pb("<tokens>");
    string s, stot="";
    while(getline(iF,s)) { if(s.length()) { stot+=s; stot+='\n';} }
    iF.close();
    string remd=rc(stot);
	s.clear();
    int i=0;
    bool isSC=false;
    while(i<remd.length())
    {
        if(i!=0 && remd[i-1]=='"' && isSC){
            s.clear();
            for(;i<remd.length() && remd[i]!='"';i++) s+=remd[i];
            string t="<stringConstant> ";t+=s;t+=" </stringConstant>";
        	tok.pb(t);
        }
        else if(isWhite(remd[i]))
            i++;
        else if(('a'<=remd[i] && remd[i]<='z') || ('A'<=remd[i] && remd[i]<='Z') || remd[i]=='_')
        {
            s.clear();
            s+=remd[i];
            i++;
            for(;i<remd.length() && ('a'<=remd[i] && remd[i]<='z') || ('A'<=remd[i] && remd[i]<='Z') || remd[i]=='_' || ('0'<=remd[i] && remd[i]<='9') ;i++)
                s+=remd[i];
            if(isKeyword(s)){
                string t="<keyword> ";t+=s;t+=" </keyword>";
                tok.pb(t);
            }
            else{
                string t="<identifier> ";t+=s;t+=" </identifier>";
                tok.pb(t);
            }
        }
        else if(remd[i]<='9' && remd[i]>='0')
        {
            s.clear();
            for(;remd[i]<='9' && remd[i]>='0' && i<remd.length();i++)
                s+=remd[i];
            string t="<integerConstant> ";t+=s;t+=" </integerConstant>";tok.pb(t);
        }
        else
        {
            if(remd[i]!='"')
            {
              if(remd[i]=='<') 
                tok.pb("<symbol> &lt; </symbol>");
              else if(remd[i]=='>') 
                tok.pb("<symbol> &gt; </symbol>");
              else if(remd[i]=='&')
                tok.pb("<symbol> &amp; </symbol>");
              else{
              	string t="<symbol> ";t+=remd[i];t+=" </symbol>";
                tok.pb(t);
               }
            }
            if(remd[i]=='"') isSC=!isSC;
              i++;
        }

    }
    tok.pb("</tokens>");
}
void analyser(string x)
{
	string x1=x.substr(0, x.length()-4);
    string x2=x1;
    x2+="err";
	x1+="xml";
	oF.open(x1);
    ferr.open(x2);
    ind=1;
    spc=0;
    compileClass();
    oF.close();
    ferr.close();
}
int main(int argc, char** argv) 
{ 
	int num=atoi(argv[1]);
	For(i,num)
	{
		string s;
		For(j, strlen(argv[i+2])) s+=argv[i+2][j];
		names.pb(s);
	}
	For(i, num)
	{
		tok.clear();
		xml.clear();
		tokeniser(names[i]);
		analyser(names[i]);
	}
	return 0;
}
