#include<bits/stdc++.h>
using namespace std;
#define For(i,n) for(int i=0;i<(int)n;i++)
#define Rev(i,n) for(int i=n-1;i>=0;i--)
#define pb push_back
#define mp make_pair
#define S second
#define F first
const string WHITESPACE = " \n\r\t\f\v";
fstream iF, oF, ferr, ff;
vector<string> tok;
vector<string> names;
vector<string> xml;
int spc=0;
int ind=0;
int flag=0;
void prspc(){ For(i,spc) oF<<"  "; }
void compileStatements();
void compileTerm();
int compileExpressionList();
void comStatements();
void comTerm();
int comExpressionList();
inline string ltrim(const string& s){
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == string::npos) ? "" : s.substr(start);}
inline string rtrim(const string& s){
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == string::npos) ? "" : s.substr(0, end + 1);}
inline string trim(const string& s){ return rtrim(ltrim(s));}
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
    return false;}
bool isKeyword(string s){return (s=="class" || s=="constructor" ||s=="method" ||s=="function" ||s=="field" ||s=="static" ||s=="var" ||s=="int" ||s=="char"||s=="boolean" ||s=="void" ||s=="true" ||s=="false"||s=="null" ||s=="let"||s=="this" ||s=="do"||s=="if" ||s=="else"||s=="while"||s=="return");}
bool isKey(string s, string kw){
	trim(s);
	if(s.length()<20) return false;
	if(s.substr(0, 9)=="<keyword>" && s.substr(s.length()-10, 10)=="</keyword>")
		if(trim(s.substr(9, s.length()-20))==kw)
			return true;
	return false;}
bool isSym(string s, string kw){
	trim(s);
	if(s.length()<18) return false;
	if(s.substr(0, 8)=="<symbol>" && s.substr(s.length()-9, 9)=="</symbol>")
		if(trim(s.substr(8, s.length()-18))==trim(kw))
			return true;
	return false;}
bool isId(string s){
	trim(s);
	if(s.length()<26) return false;
	if(s.substr(0, 12)=="<identifier>" && s.substr(s.length()-13, 13)=="</identifier>")
		return true;
	return false;}
void err1() {ferr<<"Syntax Error: ";}
void p(string s){
	prspc();	
	oF<<s<<endl; xml.pb(s);}
bool isop(string tem){return (isSym(tem, "+") || isSym(tem, "-") || isSym(tem, "*") || isSym(tem, "/")|| isSym(tem, "|")|| isSym(tem, "=")|| isSym(tem, "&amp;")|| isSym(tem, "&lt;")|| isSym(tem, "&gt;") );}
bool isUnaryOp(string tem){return (isSym(tem, "~") || isSym(tem, "-") );}
bool isKeyCon(string tem){return (isKey(tem, "true") || isKey(tem, "false") || isKey(tem, "null") || isKey(tem, "this"));}
bool isIC(string s){
	trim(s);
	if(s.length()<32) return false;
	if(s.substr(0, 17)=="<integerConstant>" && s.substr(s.length()-18, 18)=="</integerConstant>")
		return true;
	return false;}
bool isSC(string s){
	trim(s);
	if(s.length()<32) return false;
	if(s.substr(0, 16)=="<stringConstant>" && s.substr(s.length()-17, 17)=="</stringConstant>")
		return true;
	return false;}
void pr1(){
	prspc();
	oF<<tok[ind]<<endl;
	xml.pb(tok[ind]);
	ind++;
	if(ind==tok.size())
		tok.pb("");
}
void chId(){
	if(!isId(tok[ind])){
		err1();
		ferr<<"identifier expected"<<endl;
		flag=1;//exit(0);
	}
	pr1();
}
void chSym(string s0){
	if(!isSym(tok[ind], s0))
	{
		err1();
		ferr<<s0<<" expected"<<endl;
		flag=1;//exit(0);
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
		flag=1;//exit(0);
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
			ferr<<"identifier expected"<<endl;flag=1;//exit(0);
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
			ferr<<"identifier expected"<<endl;flag=1;//exit(0);
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
			flag=1;//exit(0);
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
	xml.pb("<class>");
	spc++;
	if(!isKey(tok[ind], "class"))
    {
		err1();
		ferr<<"Syntax error: class expected."<<endl;
		flag=1;//exit(0);
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
    xml.pb("</class>");
}
void tokeniser(string x){
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
void analyser(string x){
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
}


string currClassName, currSubroutineName, currSubroutineType;
string xtrim(string x){
	int st, en;
	For(i,x.length())
		if(x[i]=='>')
		{
			st=i;
			break;
		}
	Rev(i, x.length())
		if(x[i]=='<')
		{
			en=i;
			break;
		}
	string temp=x.substr(st+1, en-st-1);
	temp=trim(temp);
	return temp;
}
void inc(){
	if(ind<xml.size()) ind++;
}
map<string, pair<string, int>> fv, sv, av, lv;
int fc, sc, ac, lc, counter;
void comSubRoutineCall()
{
	string x1=xtrim(xml[ind]); inc();
	if(isSym(xml[ind], "("))
	{
		ff<<"push pointer 0"<<endl;
		inc();
		int n=comExpressionList();
		inc();
		ff<<"call currClassName."<<x1<<" "<<n+1;
	}
	else
	{
		inc();
		string x2=xtrim(xml[ind]); inc();

		int fl=-1;

		if(sv.count(x1)!=0) ff<<"push static "<<sv[x1].S<<endl;
		else if(fv.count(x1)!=0) ff<<"push static "<<fv[x1].S<<endl;
		else if(av.count(x1)!=0) ff<<"push argument "<<av[x1].S<<endl;
		else if(lv.count(x1)!=0) ff<<"push local "<<lv[x1].S<<endl;
		else fl=1;

		inc();
		int n=comExpressionList();
		inc();

		if(fl!=1)
		{
			if(sv.count(x1)!=0) ff<<"call "<<sv[x1].F<<"."<<x2<<" "<<n+1<<endl;
			else if(fv.count(x1)!=0) ff<<"call "<<fv[x1].F<<"."<<x2<<" "<<n+1<<endl;
			else if(av.count(x1)!=0) ff<<"call "<<av[x1].F<<"."<<x2<<" "<<n+1<<endl;
			else if(lv.count(x1)!=0) ff<<"call "<<lv[x1].F<<"."<<x2<<" "<<n+1<<endl;
		}
		else{
			ff<<"call "<<x1<<"."<<x2<<" "<<n<<endl;
		}
	}
}
void comClassVarDec()
{
	inc();
	if(isKey(xml[ind], "field"))
	{
		inc();
		string typ=xml[ind]; 
		do
		{
			inc();
			if(isId(xml[ind]))
			{
				fv[xtrim(xml[ind])]=mp(typ,fc);
				fc++;
			}
			inc();
		}
		while(!isSym(xml[ind], ";"));
		inc();
	}
	
	else if(isKey(xml[ind], "static"))
	{
		inc();
		string typ=xml[ind]; 
		do
		{
			inc();
			if(isId(xml[ind]))
			{
				sv[xtrim(xml[ind])]=mp(typ,sc);
				sc++;
			}
			inc();
		}
		while(!isSym(xml[ind], ";"));
		inc();
	}
	while(!(xml[ind]=="</classVarDec>")) inc(); inc();
}
void comVarDec()
{
	ind+=2;
	string tp=xtrim(xml[ind]);
	do
	{
		inc();
		if(isId(xml[ind]))
		{
			lv[xtrim(xml[ind])]=mp(tp,lc);
			cout<<tp<<" "<<lc<<endl;
			lc++;
		}
		inc();
	}
	while(!isSym(xml[ind], ";"));
	while(!(xml[ind]=="</varDec>")) inc(); inc();
}
void comParameterList()
{
	
	while(xml[ind]!="</parameterList>"){
		string typ=xtrim(xml[ind]);
		inc();
		string n=xtrim(xml[ind]);
		inc();
		av[n]=mp(typ, ac);
		ac++;
		if(isSym(xml[ind], ",")) inc();
	}
	inc();
}
void comExpression()
{
	inc();
	while(xml[ind]=="<term>")
	{
		comTerm();
		while(isop(xml[ind]))
		{
			string sm=xtrim(xml[ind]);
			cout<<sm<<endl;
			inc();
			comTerm();
			if(sm=="+") ff<<"add"<<endl;
			else if(sm=="-") ff<<"sub"<<endl;
			else if(sm=="&amp;") ff<<"and"<<endl;
			else if(sm=="|") ff<<"or"<<endl;
			else if(sm=="&gt;") ff<<"gt"<<endl;
			else if(sm=="&lt;") ff<<"lt"<<endl;
			else if(sm=="=") ff<<"eq"<<endl;
			else if(sm=="*") ff<<"call Math.multiply 2"<<endl;
			else if(sm=="/") ff<<"call Math.divide 2"<<endl;
		}
	}
	while(xml[ind]!="</expression>") inc(); inc();
}
int comExpressionList()
{
	int np=0;
	inc();
	while(xml[ind]!="</expressionList>")
	{
		if(xml[ind]=="<expression>")
			comExpression(), np++;
		inc();
	}
	inc();
	return np;
}    
void comTerm()
{
	inc();
	if(isUnaryOp(xml[ind]))
	{
		string un=xtrim(xml[ind]);
		inc();
		comTerm();
		if(un=="-") ff<<"neg";
		else ff<<"not";
		ff<<endl;
	}
	else if(isSym(xml[ind], "(")){
		inc();
		comExpression();
		inc();
	}
	else if(isIC(xml[ind]))
	{
		string temp=xml[ind];
		temp=trim(temp.substr(17, xml[ind].size()-36));
		ff<<"push constant "<<temp<<endl;
		inc();
	}
	else if(isKeyCon((xml[ind])))
	{
		string x1=xtrim(xml[ind]);
		if(x1=="true") ff<<"push constant 0\nnot"<<endl;
		else if(x1=="this") ff<<"push pointer 0"<<endl;
		else ff<<"push constant 0"<<endl;
		inc();
	}
	else if(!( isSym(xml[ind+1], "(") ||  isSym(xml[ind+1], "[") ||  isSym(xml[ind+1], ".")   ))
	{
		string x1=xtrim(xml[ind]);
		if(sv.count(x1)!=0) ff<<"push static "<<sv[x1].S<<endl;
		else if(fv.count(x1)!=0) ff<<"push static "<<fv[x1].S<<endl;
		else if(av.count(x1)!=0) ff<<"push argument "<<av[x1].S<<endl;
		else if(lv.count(x1)!=0) ff<<"push local "<<lv[x1].S<<endl;
		else{
			ferr<<"Declaration Error"<<endl;
			exit(0);
		}
		inc();
	}
	else if(isSym(xml[ind+1], "["))
	{
		string x1=xtrim(xml[ind]);
		if(sv.count(x1)!=0) ff<<"push static "<<sv[x1].S<<endl;
		else if(fv.count(x1)!=0) ff<<"push static "<<fv[x1].S<<endl;
		else if(av.count(x1)!=0) ff<<"push argument "<<av[x1].S<<endl;
		else if(lv.count(x1)!=0) ff<<"push local "<<lv[x1].S<<endl;
		else{
			ferr<<"Declaration Error"<<endl;
			exit(0);
		}
		ind+=2;
		comExpression();
		inc();
		ff<<"add\npop pointer 1\npush that 0\n";
	}
	else if(isSC(xml[ind]))
	{
		string x1=xtrim(xml[ind]);
		ff<<"push constant "<<x1.length()<<"\ncall String.new 1";
		For(i, x1.length())
			ff<<"push constant "<<(int)x1[i]<<"\n call String.appendChar 2";
		inc();
	}
	else if(isSym(xml[ind+1], "(") ||  isSym(xml[ind+1], "."))
	{
		comSubRoutineCall();
	}
	while(xml[ind]!="</term>") inc(); inc();
}
void comIfStat()
{
	while(!(xml[ind]=="<ifStatement>")) inc(); inc();
	inc();inc();
	cout<<xml[ind]<<endl;
	comExpression();
	ind+=2;
	ff<<"not\nif-goto "<<currClassName<<counter<<endl;
	comStatements();
	inc();
	ff<<"goto "<<currClassName<<counter+1<<"\nlabel "<<currClassName<<counter<<endl;
	if(isKey(xml[ind], "else")){
		inc();
		comStatements();
		while(xml[ind]!="</ifStatement>") inc(); inc();
	}
	ff<<"label "<<currClassName<<counter;
	counter+=2;
	while(!(xml[ind]=="</ifStatement>")) inc(); inc();
}
void comWhileStat()
{
	while(!(xml[ind]=="<whileStatement>")) inc(); inc();
	inc();inc();
	ff<<"label "<<currClassName<<counter<<endl;
	comExpression();
	ind+=2;
	ff<<"not\nif-goto "<<currClassName<<counter+1<<endl;
	comStatements();
	inc();
	ff<<"goto "<<currClassName<<counter<<"\nlabel "<<currClassName<<counter+1<<endl;
	counter+=2;
	while(!(xml[ind]=="</whileStatement>")) inc(); inc();
}	
void comReturnStat()
{
	if(xml[ind+2]=="<expression>")
		ind+=2, comExpression();
	else
		ff<<"push constant 0"<<endl;
	ff<<"return"<<endl;
	while(!(xml[ind]=="</returnStatement>")) inc(); inc();
}
void comDoStat()
{
	inc(); inc();
	comSubRoutineCall();
	ff<<"pop temp 0"<<endl;	
	while(xml[ind]!="</doStatement>") inc(); inc();
}
void comLetStat()
{
	inc(); inc();
	string x1=xtrim(xml[ind]); inc();
	int flg=1;
	if(isSym(xml[ind], "[")){
		inc();
		comExpression();
		inc();
		if(sv.count(x1)!=0) ff<<"push static "<<sv[x1].S<<endl;
		else if(fv.count(x1)!=0) ff<<"push static "<<fv[x1].S<<endl;
		else if(av.count(x1)!=0) ff<<"push argument "<<av[x1].S<<endl;
		else if(lv.count(x1)!=0) ff<<"push local "<<lv[x1].S<<endl;
		else{
			ferr<<"Declaration Error"<<endl;
			exit(0);
		}
		ff<<"add"<<endl;
		inc();
		comExpression();
		ff<<"pop temp 0 \npop pointer 1 \npush temp 0\n pop that 0\n";
	}
	else{
		inc();
		comExpression();
		if(sv.count(x1)!=0) ff<<"push static "<<sv[x1].S<<endl;
		else if(fv.count(x1)!=0) ff<<"push static "<<fv[x1].S<<endl;
		else if(av.count(x1)!=0) ff<<"push argument "<<av[x1].S<<endl;
		else if(lv.count(x1)!=0) ff<<"push local "<<lv[x1].S<<endl;
		else{
			ferr<<"Declaration Error"<<endl;
			exit(0);
		}
	}

	while(xml[ind]!="</letStatement>") inc(); inc();
}
void comStatements(){
	if(xml[ind]=="<statements>") inc();
	while(xml[ind]!="</statements>")
		if(xml[ind]=="<letStatement>") comLetStat();
		else if(xml[ind]=="<ifStatement>") comIfStat();
		else if(xml[ind]=="<doStatement>") comDoStat();
		else if(xml[ind]=="<whileStatement>") comWhileStat();
		else if(xml[ind]=="<returnStatement>") comReturnStat();
	while(xml[ind]!="</statements>") inc(); inc();
}
void comSubRoutineBody()
{
	while(!isSym(xml[ind], "{")) inc(); inc();
	while(xml[ind]=="<varDec>")
		comVarDec();
	
	ff<<"function "<<currClassName<<"."<<currSubroutineName<<" "<<lv.size()<<endl;
	if(currSubroutineType=="constructor")
		ff<<"push constant "<<fv.size()<<"\n call Memory.alloc 1 \n pop pointer 0\n";	
	else if(currSubroutineType=="method")
		ff<<"push argument 0\n pop pointer 0\n";
	
	comStatements();
	while(!isSym(xml[ind], "}")) inc(); inc(); 
}
void comSubRoutine()
{
	lc=ac=0;
	inc();
	currSubroutineType=xtrim(xml[ind]);
	inc();
	inc();
	currSubroutineName=xtrim(xml[ind]);
	
	av["this"]=mp(currClassName, 0);
	ac++;

	while(xml[ind]!="<parameterList>") inc();inc();
	comParameterList();
	while(xml[ind]!="<subroutineBody>") inc();inc();
	comSubRoutineBody();
	while(xml[ind]!="</subroutineDec>") inc();inc();
	
	av.clear();ac=0;
	lv.clear();lc=0;
}
void comClass()
{
	counter=sc=fc=ac=lc=0;
	currClassName=trim(xml[2]);
	currClassName=trim(currClassName.substr(12,currClassName.size()-26));
	
	while(xml[ind]!="<classVarDec>" && xml[ind]!="<subroutineDec>" && xml[ind]!="</class>") inc();
	while(xml[ind]=="<classVarDec>") comClassVarDec();
	while(xml[ind]=="<subroutineDec>") comSubRoutine();
	if(xml[ind]!="</class>") err1();
	fv.clear();
	sv.clear();
}
void compiler(string x)
{
	string op=x.substr(0, x.length()-4);
	op+="vm";
	ff.open(op);
	ind=0;
	if(xml[ind]=="<class>")
		comClass();
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
		flag=0;
		tokeniser(names[i]);
		analyser(names[i]);
		if(flag==0) compiler(names[i]);
	}
	return 0;
}