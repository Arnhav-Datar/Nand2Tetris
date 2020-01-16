#include<bits/stdc++.h>
using namespace std;
#define For(i,n) for(int i=0;i<(int)n;i++)
#define Debug(i,n,a) For(i,n) cout<<a[i]<<endl;
#define pb push_back
const string WHITESPACE = " \n\r\t\f\v";
fstream iF, oF;
vector<string> tok;
int spc=0;
int ind=0;
void prspc(){ For(i,spc) oF<<"  "; }
void compileStatements();
void compileTerm();
void compileExpressionList();
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
inline string trim(const string& s){ return rtrim(ltrim(s));}
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
void err1()
{cerr<<"On line "<<ind+1<<": Token regarding ";}
void p(string s){
	prspc();	
	oF<<s<<endl;
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
	oF<<tok[ind]<<endl;
	ind++;
}
void chId()
{
	if(!isId(tok[ind])){
		err1();
		cerr<<"identifier expected"<<endl;
		exit(0);
	}
	pr1();
}
void chSym(string s0)
{
	if(!isSym(tok[ind], s0))
	{
		err1();
		cerr<<"symbol "<<s0<<" expected"<<endl;
		exit(0);
	}
	pr1();
}
void compileSubRoutineCall()
{
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
		cerr<<"either . or ( expected"<<endl;
		exit(0);
	}
}
void compileClassVarDec()
{
	p("<classVarDec>");
	spc++;
	pr1();
	pr1();
	chId();
	while(isSym(tok[ind], ","))
	{
		pr1();
		if(isId(tok[ind]))
			pr1();
		else{
			err1();
			cerr<<"identifier expected"<<endl;exit(0);
		}
	}
	pr1();
	spc--;
	p("</classVarDec>");
}
void compileVarDec()
{
	p("<varDec>");
	spc++;
	pr1();
	pr1();
	chId();
	while(isSym(tok[ind], ","))
	{
		pr1();
		if(isId(tok[ind]))
			pr1();
		else{
			err1();
			cerr<<"identifier expected"<<endl;exit(0);
		}
	}
	pr1();
	spc--;
	p("</varDec>");
}
void compileParameterList()
{
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
			oF<<"symbol ) expexted"<<endl;
			exit(0);
		}
		pr1();
		chId();
		spc--;
		p("</parameterList>");	
	}
}
void compileExpression()
{
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
void compileExpressionList()
{
	p("<expressionList>");
	spc++;
	compileExpression();
	while(isSym(tok[ind], ",")){
		pr1();
		compileExpression();
	}
	spc--;
	p("</expressionList>");
}
void compileTerm()
{
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
	else if( isUnaryOp(tok[ind]))
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
void compileIfStat()
{
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
void compileWhileStat()
{
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
void compileReturnStat()
{
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
void compileDoStat()
{
	p("<doStatement>");
	spc++;
	pr1();
	compileSubRoutineCall();
	chSym(";");
	spc--;
	p("</doStatement>");
}
void compileLetStat()
{
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
void compileStatements()
{
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
void compileSubRoutine()
{
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
void compileClass()
{
	oF<<"<class>"<<endl;
	spc++;
	if(!isKey(tok[ind], "class")){
		err1();
		cerr<<"keyword class expected"<<endl;
		exit(0);
	}
	pr1();
	chId();

	chSym("{");

	
	while(isKey(tok[ind], "static") || isKey(tok[ind], "field"))
		compileClassVarDec();
		
		
	while(isKey(tok[ind], "method") || isKey(tok[ind], "function") || isKey(tok[ind], "constructor"))
		compileSubRoutine();

	chSym("}");	 spc--; oF<<"</class>"<<endl; } int main() { string s; cin>>s;

    string inputFile=s;
    string outputFile=s.substr(0, s.size()-3);
    outputFile+="xmlo";

    iF.open(inputFile);
    oF.open(outputFile);

    while(getline(iF,s)) tok.pb(s);
    if(tok[0]!="<tokens>"){
    	cerr<<"On line 1: the file must start with <tokens>"<<endl;
    	return 0;
    }
    ind=1;
    spc=0;
    compileClass();
   // if(tok[tok.size()-1]!="</tokens>")
    //{
    //	cerr<<"On line "<<tok.size()<<" : the file must end with </tokens>"<<endl;
    //	return 0;
    //}
    iF.close();
    oF.close();
}