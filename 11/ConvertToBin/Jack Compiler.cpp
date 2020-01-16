/** JACK COMPILER */
/**Author : Sarthak Kapoor*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <fstream>

#define MAXLENGTH 1000
#define INPUT "Main" // name of the input filename by default is named "MAIN.jack"

using namespace std;

class keywords {
public:
	set<string> keywordSet;
	keywords() {
		keywordSet = { "class", "constructor", "method", "function", "do", "var", "let", "if", "while", "static", "field", "int", "bool", "char", 
		"null", "this", "true", "false", "boolean", "return", "void", "else" }; // 22 keywords in JACK programming language 
	}
	bool find(string key) {
		if (keywordSet.find(key) != keywordSet.end()) {
			return true;
		}
		return false;
	}
};

class symbols {
public:
	set<char> symbolSet;
	symbols() {
		symbolSet = { '(', ')', '{', '}', '[', ']', ';', '~', '=', '+', '-', '*', '/', ',', '<', '>', '&', '|', '.' }; // set of all symbols in JACK
	}
	bool find(char key) {
		if (symbolSet.find(key) != symbolSet.end()) {
			return true;
		}
		return false;
	}
};

class tokenizer {
private :
	ifstream code;
	ofstream tokens;
	string filename;
	unordered_map<int, int> lineMap; // maps lines in original code to lines in code with removed comments
	keywords keywords;
	symbols symbols;
	int lineCounter;
	int tokenCounter;

public :
	tokenizer(const char* filename) {
		this->filename = filename;
		code.open(filename); // code reads the jack program
		tokens.open(this->filename + ".tok"); // final tokenized file is present in in tokens
		lineCounter = 0;
		tokenCounter = 1;
	}

	void removeComments() {
		ofstream o;
		o.open(filename + ".removeComments"); // temporary file after removing comments
		int lineCount = 0;
		bool flag = false;
		int skippedLines = 0;
		while (!code.eof()) {
			char* buffer = new char[MAXLENGTH];
			code.getline(buffer, MAXLENGTH, '\n');
			string s = buffer;
			string t = "";
			long long i;
			for (i = 0; i < s.length(); i++) {
				if (i + 1 < s.length() && s[i] == '/' && s[i + 1] == '/') { // comment format 1
					if (flag == false) {
						break;
					}
				}
				else if (i + 2 < s.length() && s[i] == '/' && s[i + 1] == '*' && s[i + 2] == '*') { // comment format 3
					flag = true;
				}
				else if (i + 1 < s.length() && s[i] == '/' && s[i + 1] == '*') { // comment format 2 
					flag = true;
				}
				else if (i + 1 < s.length() && s[i] == '*' && s[i + 1] == '/') {
					if (flag == true) {
						flag = false;
						i++;
					}
					else {
						cout << "Unidentified Symbol at line " << lineCount << endl;
					}
				}
				else {
					if (flag == false) {
						t += s[i];
					}
				}
			}
			if (t != "") { /** if current line is not part of a multiline comment, write it */
				o << t << '\n';
			}
			else {
				o << '\n';
			}
			lineCount++;
			if (t == "") { /** If a line is a part of a multiline comment or the whole line is a single line comment then */
					skippedLines++;		  /** increment skipped lines */
			}
			lineMap[lineCount - skippedLines] = lineCount;
		}
		code.close();
		o.close();
	}

	
	bool checkNumber(string currLiteral) { // checks if current literal is an integer constant
		int i;
		for (i = 0; i < currLiteral.length(); i++) {
			if (currLiteral[i] - '0' > 9 || currLiteral[i] - '0' < 0) {
				return false;
			}
		}
		if (currLiteral.length() == 0) {
			return false;
		}
		return true;
	}

	void assignLiteral(string currLiteral) {
		if (currLiteral == "" || currLiteral == "	") {
			tokenCounter--;
			return;
		}
		if (keywords.find(currLiteral)) { // current literal is a keyword
			tokens << "<keyword> " << currLiteral << " </keyword>"<<'\n';
		}
		else if (currLiteral[0] == '"' && currLiteral[currLiteral.length() - 1] == '"') {
			if (currLiteral.length() - 1 == 0) {
				tokens << "<identifier> " << currLiteral << " </identifier>"<<'\n';
			}
			else {
				tokens << "<stringConstant> " << currLiteral << " </stringConstant>"<<'\n';
			}
		}
		else if (checkNumber(currLiteral)) {
			tokens << "<integerConstant> " << currLiteral << " </integerConstant>"<<'\n';
		}
		else {
			tokens << "<identifier> " << currLiteral << " </identifier>"<<'\n';
		}
	}

	void createTokens() {
		code.open(filename + ".removeComments");
		tokens << "<tokens>" << '\n';
		while (!code.eof()) {
			char* buffer = new char[MAXLENGTH];
			code.getline(buffer, MAXLENGTH, '\n');
			lineCounter++;
			string s = buffer;
			string currLiteral = ""; // holds the current lexical element and is null if there is a space or no character is read
			int i;
			for (i = 0; i < s.length(); i++) {
				if (s[i] == ' ' || s[i] == '	') {
					assignLiteral(currLiteral);
					tokenCounter++;
					lineMap[tokenCounter] = lineCounter;
					currLiteral = ""; //space or tab is encountered hence current literal is set to null to read next literal
				}
				else if(symbols.find(s[i])){
					assignLiteral(currLiteral);
					tokenCounter++;
					lineMap[tokenCounter] = lineCounter;
					if (s[i] == '<') {
						tokens << "<symbol> " << "&lt;" << " </symbol>" << '\n';
						tokenCounter++;
						lineMap[tokenCounter] = lineCounter;
					}
					else if (s[i] == '&') {
						tokens << "<symbol> " << "&amp;" << " </symbol>" << '\n';
						tokenCounter++;
						lineMap[tokenCounter] = lineCounter;
					}
					else {
						tokens << "<symbol> " << s[i] << " </symbol>" << '\n';
						tokenCounter++;
						lineMap[tokenCounter] = lineCounter;
					}
					currLiteral = ""; //symbol is encountered hence current literal is set to null to read next literal
				}
				else if (s[i] == '"') {
					assignLiteral(currLiteral);
					tokenCounter++;
					lineMap[tokenCounter] = lineCounter;
					string t;
					i++;
					while (i < s.length() && s[i] != '"') {
						t += s[i];
						i++;
					}
					if (i == s.length()) {
						tokens << "<identifier> " << t << " </identifier>" << '\n';
						tokenCounter++;
						lineMap[tokenCounter] = lineCounter;
					}
					else {
						tokens << "<stringConstant> " << t << " </stringConstant>" << '\n';
						tokenCounter++;
						lineMap[tokenCounter] = lineCounter;
					}
					currLiteral = "";
				}
				else {
					currLiteral += s[i];
				}
			}
		}
		tokens << "</tokens>" << '\n';
		tokens.close();
	}

	unordered_map<int, int> getLineMap() {
		return lineMap;
	}
};

class lexemal {
public :
	set<string> lexemalSet;
	lexemal() {
		lexemalSet = { "<keyword>", "<stringConstant>", "<integerConstant>", "<symbol>", "<identifier>", "<class>", "</class>", "<classVarDec>", "</classVarDec>",
		"<subroutineDec>", "</subroutineDec>", "<varDec>", "</varDec>", "<statements>", "</statements>", "<subroutineBody>", "</subroutineBody>", "<parameterList>",
		"</parameterList>", "<ifStatement>", "</ifStatement>", "<doStatement>", "</doStatement>", "<letStatement>", "</letStatement>", "<whileStatement>",
		"</whileStatement>", "<returnStatement>", "</returnStatement>", "<expression>", "</expression>", "<expressionList>", "</expressionList>", "<term>",
		"</term>"};
	}
	bool find(string key) {
		if (lexemalSet.find(key) != lexemalSet.end()) {
			return true;
		}
		return false;
	}
};

class identifier {
public:
	set<int> ascii;
	identifier() {
		ascii.insert((int)('_'));
		int i;
		for (i = 0; i <= 9; i++) {
			ascii.insert(48 + i);
		}
		for (i = 0; i < 26; i++) {
			ascii.insert(65 + i);
		}
		for (i = 0; i < 26; i++) {
			ascii.insert(97 + i);
		}
	}

	bool find(char ch) {
		int x = (int)ch;
		if (ascii.find(x) != ascii.end()) {
			return true;
		}
		return false;
	}

	bool checkDigit(char ch) {
		int x = (int)ch;
		if (ascii.find(x) != ascii.end()) {
			auto it = ascii.find(x);
			if (48 <= *it && *it <= 57) {
				return true;
			}
			return false;
		}
		return false;
	}
};

class operations {
public :
	set<string> operators;
	operations() {
		operators = { "+", "-", "*", "/", "&amp;", "|", "&lt;", ">", "="};
	}
	bool find(string key) {
		if (operators.find(key) != operators.end()) {
			return true;
		}
		return false;
	}
};

class syntaxAnalyser {
private:
	ifstream tokenCode;
	ofstream xmlCode;
	unordered_map<int, int> lineMap;
	lexemal lexemal;
	string filename;
	identifier identifier;
	operations operations;

	string getLiteral(string currToken) {
		int i;
		string currLiteral = "";
		bool flag = false;
		for (i = 0; i < currToken.size(); i++) {
			if (flag == true && currToken[i] == '<') {
				return currLiteral;
			}
			if (currToken[i] != ' ' &&  currToken[i] != '	') {
				currLiteral += currToken[i];
			}
			if (lexemal.find(currLiteral)) {
				currLiteral = "";
				flag = true;
			}
		}
		return currLiteral;
	}

	string getLexemalItem(string currToken) {  // returns the lexemal item from the current token
		int i;
		string currLexemal = "";
		for (i = 0; i < currToken.size(); i++) {
			currLexemal += currToken[i];
			if (lexemal.find(currLexemal)) {
				return currLexemal.substr(1, currLexemal.length() - 2);
			}
		}
		return currLexemal;
	}

	bool checkIdentifier(string word) {  // checks if the current identifier is legal
		if (identifier.checkDigit(word[0] == true)) {
			return false;
		}
		int i;
		for (i = 1; i < word.length(); i++) {
			if (!identifier.find(word[i])) {
				return false;
			}
		}
		return true;
	}

	void compileClass(){ // compiles the class in the JACK program
		while (!tokenCode.eof()) {
			char* read = new char[MAXLENGTH];
			tokenCode.getline(read, MAXLENGTH, '\n');
			string u = read;
			if (u == "<tokens>") {
				break;
			}
		}
		if (tokenCode.eof()) {
			return;
		}
		char* buffer1 = new char[MAXLENGTH];
		tokenCode.getline(buffer1, MAXLENGTH, '\n');
		string s;
		s = getLiteral(buffer1);
		if (s != "class") {
			cout << "error at line " << lineMap[0] << " ,all jack programs must begin with a class" << endl;
			return;
		}
		if (tokenCode.eof()) {
			return;
		}
		char* buffer2 = new char[MAXLENGTH];
		tokenCode.getline(buffer2, MAXLENGTH, '\n');
		s = getLiteral(buffer2);
		string t = getLexemalItem(buffer2);
		if (t != "identifier" || s == "") {
			cout << "error at line " << lineMap[1] << " , className expected" << endl;
			return;
		}
		if (tokenCode.eof()) {
			return;
		}
		char* buffer3 = new char[MAXLENGTH];
		tokenCode.getline(buffer3, MAXLENGTH, '\n');
		s = getLiteral(buffer3);
		if (s != "{") {
			cout << "error at line " << lineMap[2] << " ,expected {" << endl;
			return;
		}
		xmlCode << "<class>" << '\n';
		string tab = "  ";
		xmlCode << tab << buffer1 << '\n';
		xmlCode << tab << buffer2 << '\n';
		xmlCode << tab << buffer3 << '\n';
		bool flag = false;
		int count = 3;
		while (!tokenCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			tokenCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			if (s == "}") {
				xmlCode << tab << buffer << '\n';
				count++;
				break;
			}
			s = getLexemalItem(buffer);
			if (s != "keyword") {
				cout << "error at line " << lineMap[count] << " expected class field, static or subroutine declaration" << endl;
			}
			else {
				s = getLiteral(buffer);
				if (s == "field" || s == "static") {
					if (flag == true) {
						cout << "Error at line " << lineMap[count] << " all variable declarations must occur at the beginning of the class" << endl;
					}
					else {
						xmlCode << tab << "<classVarDec>" << '\n';
						compileClassVarDec(buffer, count, tab + "  ");
						xmlCode << tab << "</classVarDec>" << '\n';
					}
				}
				else {
					if (s == "constructor" || s == "method" || s == "function") {
						flag = true;  // now no variable declarations are allowed as we have our first function declaration
						xmlCode << tab << "<subroutineDec>" << '\n';
						compileSubRoutine(buffer, count, tab + "  ");
						xmlCode << tab << "</subroutineDec>" << '\n';
					}
					else {
						cout << "Error at line " << lineMap[count] << " Expected a function declaration" << endl;
					}
				}
			}
			count++;
		}
		if (s != "}") {
			cout << "Error at line " << lineMap[count] << " expected }" << '\n';
		}
		xmlCode << "</class>" << '\n';
		xmlCode.close();
	}
	void compileClassVarDec(string currToken, int & count, string tab){
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " missing variable type and identifier" << endl;
			return;
		}
		char* buffer1 = new char[MAXLENGTH];
		tokenCode.getline(buffer1, MAXLENGTH, '\n');
		count++;
		string s = getLexemalItem(buffer1);
		if (s != "keyword" && s != "identifier") {
			cout << "Error at line " << lineMap[count] << " variable type missing" << endl;
		}
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " identifier missing" << endl;
			return;
		}
		char* buffer2 = new char[MAXLENGTH];
		tokenCode.getline(buffer2, MAXLENGTH, '\n');
		s = getLexemalItem(buffer2);
		count++;
		if (s != "identifier") {
			cout << "Error at line " << lineMap[count] << " identifier missing" << endl;
		}
		else {
			s = getLiteral(buffer2);
			bool check = checkIdentifier(s);
			if (check == false) {
				cout << "Error at line " << lineMap[count] << " illegal identifier name" << endl;
			}
		}
		xmlCode << tab << currToken << '\n';
		xmlCode << tab << buffer1 << '\n';
		xmlCode << tab << buffer2 << '\n';
		while (!tokenCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			tokenCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			if (s == ";") {
				xmlCode << tab << buffer << '\n';
				count++;
				break;
			}
			else if (s == ",") {
				xmlCode << tab << buffer << '\n';
				count++;
			}
			else {
				cout << "Error at line " << lineMap[count] << " expected ',' or ';' " << endl;
			}
			if (tokenCode.eof()) {
				cout << "Error at line " << lineMap[count] << " illegal statement termination expected ';' " << endl;
				return;
			}
			tokenCode.getline(buffer, MAXLENGTH, '\n');
			count++;
			s = getLexemalItem(buffer);
			if (s != "identifier") {
				cout << "Error at line " << lineMap[count] << " expected an identifier" << endl;
			}
			xmlCode << tab << buffer << '\n';
		}
		if (s != ";") {
			cout << "Error at line " << lineMap[count] << " missing ';'" << endl;
		}
	}

	void compileSubRoutine(string currToken, int & count, string tab) {
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " incomplete function description" << endl;
		}
		char* buffer1 = new char[MAXLENGTH];
		tokenCode.getline(buffer1, MAXLENGTH, '\n');
		count++;
		string s = getLexemalItem(buffer1);
		if (s != "identifier" && s != "keyword") {
			cout << "Error at line " << lineMap[count] << " illegal return type" << endl;
		}
		if (s == "keyword") {
			s = getLiteral(buffer1);
			if (s != "void" && s != "int" && s != "boolean" && s != "char") {
				cout << "Error at line " << lineMap[count] << " illegal return type" << endl;
			}
		}
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " function name missing" << endl;
		}
		char* buffer2 = new char[MAXLENGTH];
		tokenCode.getline(buffer2, MAXLENGTH, '\n');
		s = getLexemalItem(buffer2);
		if (s != "identifier") {
			cout << "Error at line " << lineMap[count] << " illegal identifier" << endl;
		}
		else {
			s = getLiteral(buffer2);
			bool check = checkIdentifier(s);
			if (check == false) {
				cout << "Error at line " << lineMap[count] << " illegal identifier name" << endl;
			}
		}
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " expected '(' " << endl;
		}
		char* buffer3 = new char[MAXLENGTH];
		tokenCode.getline(buffer3, MAXLENGTH, '\n');
		count++;
		s = getLiteral(buffer3);
		if (s != "(") {
			cout << "Error at line " << lineMap[count] << " expected '(' " << endl;
		}
		xmlCode << tab << currToken << '\n';
		xmlCode << tab << buffer1 << '\n';
		xmlCode << tab << buffer2 << '\n';
		xmlCode << tab << buffer3 << '\n';
		xmlCode << tab << "<parameterList>" << '\n';
		compileParameterList(count, tab + " ");
		xmlCode << tab << "</parameterList>" << '\n';
		xmlCode << tab << "<symbol> ) </symbol>" << '\n';
		
		char* buffer4 = new char[MAXLENGTH];
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " expected '{' " << endl;
		}
		tokenCode.getline(buffer4, MAXLENGTH, '\n');
		s = getLiteral(buffer4);
		if (s != "{") {
			cout << "Error at line " << lineMap[count] << " expected '{' " << endl;
		}
		xmlCode << tab << "<subroutineBody>" << '\n';
		string orgTab = tab;
		tab += "  ";
		xmlCode << tab << buffer4 << '\n';
		bool check = false;
		int flag = 0;
		while (!tokenCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			tokenCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			if (s == "}") {
				count++;
				if (flag == 0) {
					xmlCode << tab << "<statements>" << '\n';
				}
				xmlCode << tab << "</statements>" << '\n';
				xmlCode << tab << buffer << '\n';
				break;
			}
			if (s == "var" && check == false) {
				xmlCode << tab << "<varDec>" << '\n';
				compileVarDec(buffer, count, tab + "  ");
				xmlCode << tab << "</varDec>" << '\n';
			}
			else {
				check = true;
				if (flag == 0) {
					xmlCode << tab << "<statements>" << '\n';
				}
				compileStatements(buffer, count, tab + "  ");
				flag++;
			}	
		}
		if (s != "}") {
			cout << "Error at line " << lineMap[count] << " missing '}' at the end of function" <<'\n';
		}
		tab = orgTab;
		xmlCode << tab << "</subroutineBody>" << '\n';
	}

	void compileParameterList(int & count, string tab) {
		string s;
		while (!tokenCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			tokenCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			if (s == ")") {
				count++;
				break;
			}
			else if (s == ",") {
				xmlCode << tab << buffer << '\n';
				continue;
			}
			s = getLexemalItem(buffer);
			if (s != "identifier" && s != "keyword") {
				cout << "Error at line " << lineMap[count] << " expected a variable type" << endl;
			}
			if (tokenCode.eof()) {
				cout << "Error at line " << lineMap[count] << " expected an identifier " << endl;
				return;
			}
			char* buffer1 = new char[1000];
			tokenCode.getline(buffer1, MAXLENGTH, '\n');
			xmlCode << tab << buffer << '\n';
			xmlCode << tab << buffer1 << '\n';
		}
		if (s != ")") {
			cout << "Error at line " << lineMap[count] << " missing ')'" << endl;
		}
	}

	void compileVarDec(string currToken, int & count, string tab) {  // same as compileClassVarDec
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " missing variable type and identifier" << endl;
			return;
		}
		char* buffer1 = new char[MAXLENGTH];
		tokenCode.getline(buffer1, MAXLENGTH, '\n');
		count++;
		string s = getLexemalItem(buffer1);
		if (s != "keyword" && s != "identifier") {
			cout << "Error at line " << lineMap[count] << " variable type missing" << endl;
		}
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " identifier missing" << endl;
			return;
		}
		char* buffer2 = new char[MAXLENGTH];
		tokenCode.getline(buffer2, MAXLENGTH, '\n');
		s = getLexemalItem(buffer2);
		count++;
		if (s != "identifier") {
			cout << "Error at line " << lineMap[count] << " identifier missing" << endl;
		}
		else {
			s = getLiteral(buffer2);
			bool check = checkIdentifier(s);
			if (check == false) {
				cout << "Error at line " << lineMap[count] << " illegal identifier name" << endl;
			}
		}
		xmlCode << tab << currToken << '\n';
		xmlCode << tab << buffer1 << '\n';
		xmlCode << tab << buffer2 << '\n';
		while (!tokenCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			tokenCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			count++;
			if (s == ";") {
				xmlCode << tab << buffer << '\n';
				break;
			}
			else if (s == ",") {
				xmlCode << tab << buffer << '\n';
				continue;
			}
			s = getLexemalItem(buffer);
			if (s != "identifier") {
				cout << "Error at line " << lineMap[count] << " expected an identifier" << endl;
			}
			xmlCode << tab << buffer << '\n';
		}
		if (s != ";") {
			cout << "Error at line " << lineMap[count] << " missing ';'" << endl;
		}
	}

	void compileStatements(string currToken, int & count, string tab) { // compiles the 5 different types of statements
		string s = getLiteral(currToken);
		if (s == "do") {
			xmlCode << tab << "<doStatement>" << '\n';
			compileDo(currToken, count, tab + "  ");
			xmlCode << tab << "</doStatement>" << '\n';
		}
		else if (s == "let") {
			xmlCode << tab << "<letStatement>" << '\n';
			compileLet(currToken, count, tab + "  ");
			xmlCode << tab << "</letStatement>" << '\n';
		}
		else if (s == "while") {
			xmlCode << tab << "<whileStatement>" << '\n';
			compileWhile(currToken, count, tab + "  ");
			xmlCode << tab << "</whileStatement>" << '\n';
		}
		else if (s == "return") {
			xmlCode << tab << "<returnStatement>" << '\n';
			compileReturn(currToken, count, tab + "  ");
			xmlCode << tab << "</returnStatement>" << '\n';
		}
		else if (s == "if") {
			xmlCode << tab << "<ifStatement>" << '\n';
			compileIf(currToken, count, tab + "  ");
			xmlCode << tab << "</ifStatement>" << '\n';
		}
		else {
			cout << "Error at line " << lineMap[count] << " unidentified statement" << endl;
		}
	}

	void compileDo(string currToken, int & count, string tab) {  // compiles the do statement
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " expected a subroutine call " << endl;
		}
		char* buffer1 = new char[MAXLENGTH];
		tokenCode.getline(buffer1, MAXLENGTH, '\n');
		count++;
		string s = getLexemalItem(buffer1);
		if (s != "identifier") {
			cout << "Error at line " << lineMap[count] << " expected subroutine name" << endl;
		}
		else {
			s = getLiteral(buffer1);
			bool check = checkIdentifier(s);
			if (check == false) {
				cout << "Error at line " << lineMap[count] << " illegal identifier name" << endl;
			}
		}
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " expected ( or ." << endl;
		}
		char* buffer2 = new char[MAXLENGTH];
		tokenCode.getline(buffer2, MAXLENGTH, '\n');
		count++;
		s = getLiteral(buffer2);
		xmlCode << tab << currToken << '\n';
		xmlCode << tab << buffer1 << '\n';
		if (s != "(" && s != ".") {
			cout << "Error at line " << lineMap[count] << " expected ( or ." << endl;
		}
		else if (s == "(") {
			xmlCode << tab << buffer2 << '\n';
			xmlCode << tab << "<expressionList>" << '\n';
			compileExpressionList(count, tab + "  ");
			xmlCode << tab << "</expressionList>" << '\n';
			xmlCode << tab << "<symbol> ) </symbol>" << '\n';
		}
		else {
			if (tokenCode.eof()) {
				cout << "Error at line " << lineMap[count] << " expected a subroutine call " << endl;
			}
			char* buffer3 = new char[MAXLENGTH];
			tokenCode.getline(buffer3, MAXLENGTH, '\n');
			count++;
			string s = getLexemalItem(buffer3);
			if (s != "identifier") {
				cout << "Error at line " << lineMap[count] << " expected subroutine name" << endl;
			}
			else {
				s = getLiteral(buffer3);
				bool check = checkIdentifier(s);
				if (check == false) {
					cout << "Error at line " << lineMap[count] << " illegal identifier name" << endl;
				}
				if (tokenCode.eof()) {
					cout << "Error at line " << lineMap[count] << " expected ( " << endl;
				}
				char* buffer4 = new char[MAXLENGTH];
				count++;
				tokenCode.getline(buffer4, MAXLENGTH, '\n');
				s = getLiteral(buffer4);
				if (s != "(") {
					cout << "Error at line " << lineMap[count] << " expected ( " << endl;
				}
				xmlCode << tab << buffer2 << '\n';
				xmlCode << tab << buffer3 << '\n';
				xmlCode << tab << buffer4 << '\n';
				xmlCode << tab << "<expressionList>" << '\n';
				compileExpressionList(count, tab + "  ");
				xmlCode << tab << "</expressionList>" << '\n';
				xmlCode << tab << "<symbol> ) </symbol>" << '\n';
			}
		}
		char* buffer5 = new char[MAXLENGTH];
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " missing ';' " << endl;
		}
		tokenCode.getline(buffer5, MAXLENGTH, '\n');
		s = getLiteral(buffer5);
		if (s != ";") {
			cout << "Error at line " << lineMap[count] << " missing ';' " << endl;
		}
		xmlCode << tab << buffer5 << '\n';
	}

	void compileLet(string currToken, int & count, string tab) { // compiles the let statement
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " expected identifier name" << endl;
		}
		char* buffer1 = new char[MAXLENGTH];
		tokenCode.getline(buffer1, MAXLENGTH, '\n');
		count++;
		string s = getLexemalItem(buffer1);
		if (s != "identifier") {
			cout << "Error at line " << lineMap[count] << " expected identifier" << endl;
		}
		else {
			s = getLiteral(buffer1);
			bool check = checkIdentifier(s);
			if (check == false) {
				cout << "Error at line " << lineMap[count] << " illegal identifier name" << endl;
			}
		}
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " expected '=' " << endl;
		}
		char* buffer2 = new char[MAXLENGTH];
		tokenCode.getline(buffer2, MAXLENGTH, '\n');
		count++;
		s = getLiteral(buffer2);
		if (s == "[") {
			xmlCode << tab << currToken << '\n';
			xmlCode << tab << buffer1 << '\n';
			xmlCode << tab << buffer2 << '\n';
			xmlCode << tab << "<expression>" << '\n';
			compileExpression(count, tab + "  ");
			xmlCode << tab << "</expression>" << '\n';
			char* buffer7 = new char[MAXLENGTH];
			tokenCode.getline(buffer7, MAXLENGTH, '\n');
			string t = getLiteral(buffer7);
			if (t != "]") {
				cout << "Expected ] at line " << lineMap[count] << endl;
			}
			xmlCode << tab << "<symbol> ] </symbol>" << '\n';
			if (tokenCode.eof()) {
				cout << "Error at line " << lineMap[count] << " expected '=' " << endl;
			}
			char* buffer3 = new char[MAXLENGTH];
			tokenCode.getline(buffer3, MAXLENGTH, '\n');
			count++;
			s = getLiteral(buffer3);
			if (s != "=") {
				cout << "Error at line " << lineMap[count] << " expected '=' " << endl;
			}
			xmlCode << tab << buffer3 << '\n';
			xmlCode << tab << "<expression>" << '\n';
			compileExpression(count, tab + "  ");
			xmlCode << tab << "</expression>" << '\n';
			char* buffer6 = new char[MAXLENGTH];
			tokenCode.getline(buffer6, MAXLENGTH, '\n');
			string u = getLiteral(buffer6);
			if (u != ";") {
				cout << "Expected ; at line " << lineMap[count] << endl;
			}
			xmlCode << tab << "<symbol> ; </symbol>" << '\n';
		}
		else if (s == "=") { 
			xmlCode << tab << currToken << '\n';
			xmlCode << tab << buffer1 << '\n';
			xmlCode << tab << buffer2 << '\n';
			xmlCode << tab << "<expression>" << '\n';
			compileExpression(count, tab + "  ");
			xmlCode << tab << "</expression>" << '\n';
			char* buffer6 = new char[MAXLENGTH];
			tokenCode.getline(buffer6, MAXLENGTH, '\n');
			string t = getLiteral(buffer6);
			if (t != ";") {
				cout << "Expected ; at line " << lineMap[count] << endl;
			}
			xmlCode << tab << "<symbol> ; </symbol>" << '\n';
		}
		else {
			cout << "Error at line " << lineMap[count] << " expected '=' " << endl;
		}
	}

	void compileWhile(string currToken, int & count, string tab) { // compiles the while statement
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " missing '(' " << endl;
		}
		char* buffer1 = new char[MAXLENGTH];
		tokenCode.getline(buffer1, MAXLENGTH, '\n');
		string s = getLiteral(buffer1);
		count++;
		if (s != "(") {
			cout << "Error at line " << lineMap[count] << " missing '(' " << endl;
		}
		xmlCode << tab << currToken << '\n';
		xmlCode << tab << buffer1 << '\n';
		xmlCode << tab << "<expression>" << '\n';
		compileExpression(count, tab + "  ");
		xmlCode << tab << "</expression>" << '\n';
		char* buffer3 = new char[MAXLENGTH];
		tokenCode.getline(buffer3, MAXLENGTH, '\n');
		xmlCode << tab << "<symbol> ) </symbol>" << '\n';
		char* buffer2 = new char[MAXLENGTH];
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " missing '{' " << endl;
		}
		tokenCode.getline(buffer2, MAXLENGTH, '\n');
		s = getLiteral(buffer2);
		if (s != "{") {
			cout << "Error at line " << lineMap[count] << " missing '{' " << endl;
		}
		xmlCode << tab << buffer2 << '\n';
		int flag = 0;
		while (!tokenCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			tokenCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			count++;
			if (s == "}") {
				if (flag == 0) {
					xmlCode << tab << "<statements>" << '\n';
				}
				xmlCode << tab << "</statements>" << '\n';
				xmlCode << tab << buffer << '\n';
				break;
			}
			else {
				if (flag == 0) {
					xmlCode << tab << "<statements>" << '\n';
				}
				compileStatements(buffer, count, tab + "  ");
				flag++;
			}
		}
		if (s != "}") {
			cout << "Error at line " << lineMap[count] << " missing '}' " << '\n';
		}
	}

	void compileIf(string currToken, int & count, string tab) { // compiles the if statement
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " missing '(' " << endl;
		}
		char* buffer1 = new char[MAXLENGTH];
		tokenCode.getline(buffer1, MAXLENGTH, '\n');
		string s = getLiteral(buffer1);
		count++;
		if (s != "(") {
			cout << "Error at line " << lineMap[count] << " missing '(' " << endl;
		}
		xmlCode << tab << currToken << '\n';
		xmlCode << tab << buffer1 << '\n';
		xmlCode << tab << "<expression>" << '\n';
		compileExpression(count, tab + "  ");
		xmlCode << tab << "</expression>" << '\n';
		char* buffer6 = new char[MAXLENGTH];
		tokenCode.getline(buffer6, MAXLENGTH, '\n');
		xmlCode << tab << "<symbol> ) </symbol>" << '\n';
		char* buffer2 = new char[MAXLENGTH];
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " missing '{' " << endl;
		}
		tokenCode.getline(buffer2, MAXLENGTH, '\n');
		s = getLiteral(buffer2);
		if (s != "{") {
			cout << "Error at line " << lineMap[count] << " missing '{' " << endl;
		}
		xmlCode << tab << buffer2 << '\n';
		int flag = 0;
		while (!tokenCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			tokenCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			count++;
			if (s == "}") {
				if (flag == 0) {
					xmlCode << tab << "<statements>" << '\n';
				}
				xmlCode << tab << "</statements>" << '\n';
				xmlCode << tab << buffer << '\n';
				break;
			}
			else {
				if (flag == 0) {
					xmlCode << tab << "<statements>" << '\n';
				}
				compileStatements(buffer, count, tab + "  ");
				flag++;
			}
		}
		if (s != "}") {
			cout << "Error at line " << lineMap[count] << " missing '}' " << '\n';
		}
		char* buffer3 = new char[MAXLENGTH];
		if (tokenCode.eof()) {
			return;
		}
		size_t pos = tokenCode.tellg();
		tokenCode.getline(buffer3, MAXLENGTH, '\n');
		s = getLiteral(buffer3);
		flag = 0;
		if (s != "else") {
			tokenCode.seekg(pos);
			return;
		}
		else {
			char* buffer7 = new char[MAXLENGTH];
			tokenCode.getline(buffer7, MAXLENGTH, '\n');
			xmlCode << tab << buffer3 << '\n';
			xmlCode << tab << buffer7 << '\n';
			while (!tokenCode.eof()) {
				char* buffer = new char[MAXLENGTH];
				tokenCode.getline(buffer, MAXLENGTH, '\n');
				s = getLiteral(buffer);
				count++;
				if (s == "}") {
					if (flag == 0) {
						xmlCode << tab << "<statements>" << '\n';
					}
					xmlCode << tab << "</statements>" << '\n';
					xmlCode << tab << buffer << '\n';
					break;
				}
				else {
					if (flag == 0) {
						xmlCode << tab << "<statements>" << '\n';
					}
					compileStatements(buffer, count, tab + "  ");
					flag++;
				}
			}
			if (s != "}") {
				cout << "Error at line " << lineMap[count] << " missing '}' " << '\n';
			}
		}
	}

	void compileReturn(string currToken, int & count, string tab) { // compiles the return statement
		if (tokenCode.eof()) {
			cout << "Error at line " << lineMap[count] << " expected ';' " << endl;
		}
		size_t pos = tokenCode.tellg();
		xmlCode << tab << currToken << '\n';
		char* buffer1 = new char[MAXLENGTH];
		tokenCode.getline(buffer1, MAXLENGTH, '\n');
		string s = getLiteral(buffer1);
		if (s == ";") {
			xmlCode << tab << buffer1 << '\n';
		}
		else {
			tokenCode.seekg(pos);
			xmlCode << tab << "<expression>" << '\n';
			compileExpression(count, tab + "  ");
			xmlCode << tab << "</expression>" << '\n';
			char* buffer6 = new char[MAXLENGTH];
			tokenCode.getline(buffer6, MAXLENGTH, '\n');
			string t = getLiteral(buffer6);
			if (t != ";") {
				cout << "Expected ; at line " << lineMap[count] << endl;
			}
			xmlCode << tab << "<symbol> ; </symbol>" << '\n';
		}
	}

	void compileExpression(int & count, string tab) {
		string s;
		int check = 0;
		while (!tokenCode.eof()) {
			size_t pos = tokenCode.tellg();
			char* buffer = new char[MAXLENGTH];
			tokenCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			count++;
			if (s == ";" || s == "]" || s == ")" || s == ",") {
				tokenCode.seekg(pos);
				break;
			}
			else {
				if (check == 0 && s == "-") {
					xmlCode << tab << "<term>" << '\n';
					compileTerm(buffer, count, tab + "  ");
					xmlCode << tab << "</term>" << '\n';
				}
				else if (operations.find(s)) {
					xmlCode << tab << buffer << '\n';
				}
				else {
					xmlCode << tab << "<term>" << '\n';
					compileTerm(buffer, count, tab + "  ");
					xmlCode << tab << "</term>" << '\n';
				}
			}
			check++;
		}
	}

	void compileExpressionList(int & count, string tab) {
		string s;
		bool check = false;
		while (!tokenCode.eof()) {
			size_t pos = tokenCode.tellg();
			char* buffer = new char[MAXLENGTH];
			count++;
			tokenCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			if (s == ")") {
				count++;
				break;
			}
			else if (s == ",") {
				xmlCode << tab << buffer << '\n';
				continue;
			}
			tokenCode.seekg(pos);
			xmlCode << tab << "<expression>" << '\n';
			compileExpression(count, tab + "  ");
			xmlCode << tab << "</expression>" << '\n';
		}
		if (s != ")") {
			cout << "Error at line " << lineMap[count] << " missing ')'" << endl;
		}
	}

	void compileTerm(string currTerm, int & count, string tab) {
		string s = getLiteral(currTerm);
		string t = getLexemalItem(currTerm);
		if (t == "keyword" && ((s != "true") && (s != "false") && (s != "null") && (s != "this"))) {
			cout << "Error at line " << lineMap[count] << " invalid expression term" << endl;
		}
		else if (t == "keyword" || t == "integerConstant" || t == "stringConstant") {
			xmlCode << tab << currTerm << '\n';
		}
		else if (t == "symbol") {
			if (s == "~" || s == "-") {
				xmlCode << tab << currTerm << '\n';
				if (tokenCode.eof()) {
					cout << "Expected a term at line " << lineMap[count] << endl;
				}
				char* buffer = new char[MAXLENGTH];
				tokenCode.getline(buffer, MAXLENGTH, '\n');
				count++;
				xmlCode << tab << "<term>" << '\n';
				compileTerm(buffer, count, tab + "  ");
				xmlCode << tab << "</term>" << '\n';
			}
			else if (s == "(") {
				xmlCode << tab << currTerm << '\n';
				xmlCode << tab << "<expression>" << '\n';
				compileExpression(count, tab + "  ");
				xmlCode << tab << "</expression>" << '\n';
				char* buffer5 = new char[MAXLENGTH];
				tokenCode.getline(buffer5, MAXLENGTH, '\n');
				xmlCode << tab << "<symbol> ) </symbol>" << '\n';
			}
			else {
				cout << "Error at line " << lineMap[count] << " invalid expression term" << endl;
			}
		}
		else if (t == "identifier") {
			bool check = checkIdentifier(s);
			if (check == false) {
				cout<< "Error at line " << lineMap[count] << " invalid identifier" << endl;
			}
			if (tokenCode.eof()) {
				xmlCode << tab << currTerm << '\n';
				return;
			}
			size_t pos = tokenCode.tellg();
			char* buffer = new char[MAXLENGTH];
			tokenCode.getline(buffer, MAXLENGTH, '\n');
			string u = getLiteral(buffer);
			if (u != "[" && u != "(" && u != ".") {
				tokenCode.seekg(pos);
				xmlCode << tab << currTerm << '\n';
			}
			else if (u == "[") {
				xmlCode << tab << currTerm << '\n';
				xmlCode << tab << buffer << '\n';
				xmlCode << tab << "<expression>" << '\n';
				compileExpression(count, tab + "  ");
				xmlCode << tab << "</expression>" << '\n';
				char* buffer6 = new char[MAXLENGTH];
				tokenCode.getline(buffer6, MAXLENGTH, '\n');
				string t = getLiteral(buffer6);
				if (t != "]") {
					cout << "Expected ] at line " << lineMap[count] << endl;
				}
				xmlCode << tab << "<symbol> ] </symbol>" << '\n';
			}
			else {
				if (u == "(") {
					xmlCode << tab << currTerm << '\n';
					xmlCode << tab << buffer << '\n';
					xmlCode << tab << "<expressionList>" << '\n';
					compileExpressionList(count, tab + "  ");
					xmlCode << tab << "</expressionList>" << '\n';
					xmlCode << tab << "<symbol> ) </symbol>" << '\n';
				}
				else {
					if (tokenCode.eof()) {
						cout << "Error at line " << lineMap[count] << " expected a subroutine call " << endl;
					}
					char* buffer3 = new char[MAXLENGTH];
					tokenCode.getline(buffer3, MAXLENGTH, '\n');
					count++;
					string s = getLexemalItem(buffer3);
					if (s != "identifier") {
						cout << "Error at line " << lineMap[count] << " expected subroutine name" << endl;
					}
					else {
						s = getLiteral(buffer3);
						bool check = checkIdentifier(s);
						if (check == false) {
							cout << "Error at line " << lineMap[count] << " illegal identifier name" << endl;
						}
						if (tokenCode.eof()) {
							cout << "Error at line " << lineMap[count] << " expected ( " << endl;
						}
						char* buffer4 = new char[MAXLENGTH];
						count++;
						tokenCode.getline(buffer4, MAXLENGTH, '\n');
						s = getLiteral(buffer4);
						if (s != "(") {
							cout << "Error at line " << lineMap[count] << " expected ( " << endl;
						}
						xmlCode << tab << currTerm << '\n';
						xmlCode << tab << buffer << '\n';
						xmlCode << tab << buffer3 << '\n';
						xmlCode << tab << buffer4 << '\n';
						xmlCode << tab << "<expressionList>" << '\n';
						compileExpressionList(count, tab + "  ");
						xmlCode << tab << "</expressionList>" << '\n';
						xmlCode << tab << "<symbol> ) </symbol>" << '\n';
					}
				}
			}
		}
		
	}

public :
	syntaxAnalyser(const char* filename, unordered_map<int, int> lineMap) {
		this->filename = filename;
		tokenCode.open(this->filename + ".tok");
		xmlCode.open(this->filename + ".xml");
		this->lineMap = lineMap;
		compileClass();
	}
};

class classSymbolTable {
private:
	unordered_map<string, int> table; // maps variable name to its index in the symbol table
	vector<string> kind;
	vector<string> type;
	vector<string> name;
	vector<int> index;
	int fieldCount;
	int staticCount;
	int totalCount;
public:
	classSymbolTable() {
		fieldCount = 0;
		staticCount = 0;
		totalCount = 0;
	}
	bool add(string kindName, string typeName, string varName) {
		for (auto str : name) {
			if (str == varName) {
				return false;  // the given variable name already exists as a class variable
			}
		}
		if (kindName == "field") {
			kindName = "this";
			index.push_back(fieldCount);
			fieldCount++;
			totalCount++;
		}
		else if (kindName == "static") {
			index.push_back(staticCount);
			staticCount++;
			totalCount++;
		}
		kind.push_back(kindName);
		type.push_back(typeName);
		name.push_back(varName);
		table[varName] = totalCount - 1;
		return true; // addition is successful
	}
	string getKind(string varName) {
		int i = table[varName];
		return kind[i];
	}
	string getType(string varName) {
		int i = table[varName];
		return type[i];
	}
	bool find(string varName) {
		for (auto str : name) {
			if (varName == str) {
				return true;
			}
		}
		return false;
	}
	int getIndex(string varName) {
		int i = table[varName];
		return index[i];
	}
	int getFieldCount() {
		return fieldCount;
	}
	int getStaticCount() {
		return staticCount;
	}
	int getTotalCount() {
		return totalCount;
	}
	void print() {
		int i;
		for (i = 0; i < kind.size(); i++) {
			cout << kind[i] << " " << type[i] << " " << name[i] << " " << index[i] << endl;
		}
	}
};

class subRoutineSymbolTable {
private:
	unordered_map<string, int> table; // maps variable name to its index in the symbol table
	vector<string> kind;
	vector<string> type;
	vector<string> name;
	vector<int> index;
	int localCount;
	int argCount;
	int totalCount;
public:
	subRoutineSymbolTable() {
		localCount = 0;
		argCount = 0;
		totalCount = 0;
	}
	bool add(string kindName, string typeName, string varName) {
		for (auto str : name) {
			if (str == varName) {
				return false;  // the given variable name already exists as a subRoutine variable
			}
		}
		if (kindName == "local") {
			index.push_back(localCount);
			localCount++;
			totalCount++;
		}
		else if (kindName == "argument") {
			index.push_back(argCount);
			argCount++;
			totalCount++;
		}
		kind.push_back(kindName);
		type.push_back(typeName);
		name.push_back(varName);
		table[varName] = totalCount - 1;
		return true; // addition is successful
	}
	string getKind(string varName) {
		int i = table[varName];
		return kind[i];
	}
	string getType(string varName) {
		int i = table[varName];
		return type[i];
	}
	bool find(string varName) {
		for (auto str : name) {
			if (varName == str) {
				return true;
			}
		}
		return false;
	}
	int getIndex(string varName) {
		int i = table[varName];
		return index[i];
	}
	int getLocalCount() {
		return localCount;
	}
	int getArgCount() {
		return argCount;
	}
	int getTotalCount() {
		return totalCount;
	}
	void refresh() { // after execution of one subroutine the symbol table is cleared
		table.clear();
		kind.clear();
		type.clear();
		name.clear();
		index.clear();
	}
	void print() {
		int i;
		for (i = 0; i < kind.size(); i++) {
			cout << kind[i] << " " << type[i] << " " << name[i] << " " << index[i] << endl;
		}
	}
};

class codeGenerator {
private :	
	ifstream xmlCode;
	ofstream vmCode;
	classSymbolTable cst;
	subRoutineSymbolTable sst;
	string filename;
	string className;
	string currSubRoutineName;
	string currSubRoutineType;
	lexemal lexemal;
	int labelCount;

	string getLiteral(string currToken) {
		int i;
		string currLiteral = "";
		bool flag = false;
		for (i = 0; i < currToken.size(); i++) {
			if (flag == true && currToken[i] == '<') {
				return currLiteral;
			}
			if (currToken[i] != ' ') {
				currLiteral += currToken[i];
			}
			if (lexemal.find(currLiteral)) {
				currLiteral = "";
				flag = true;
			}
		}
		return currLiteral;
	}

	string getString(string currToken) {
		int i;
		string currLiteral = "";
		bool flag = false;
		for (i = 0; i < currToken.size(); i++) {
			if (flag == true && currToken[i] == '<') {
				return currLiteral;
			}
			if (currToken[i] != ' ' || flag == true) {
				currLiteral += currToken[i];
			}
			if (lexemal.find(currLiteral)) {
				currLiteral = "";
				flag = true;
				i++;
			}
		}
		return currLiteral.substr(0, currLiteral.length() - 1);
	}

	string getLexemalItem(string currToken) {  // returns the lexemal item from the current token
		int i;
		string currLexemal = "";
		for (i = 0; i < currToken.size(); i++) {
			if (currToken[i] != ' ') {
				currLexemal += currToken[i];
			}
			if (lexemal.find(currLexemal)) {
				return currLexemal.substr(1, currLexemal.length() - 2);
			}
		}
		return currLexemal;
	}

	void compileClass() {
		char* buffer1 = new char[MAXLENGTH];
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <class>
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // class
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // className
		string s = getLiteral(buffer1);
		className = s;
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // {

		while (!xmlCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			xmlCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			if (s == "}") {
				break;
			}
			s = getLexemalItem(buffer);
			if (s == "classVarDec") {
				compileClassVarDec();
				xmlCode.getline(buffer, MAXLENGTH, '\n'); // buffer = </classVarDec>
			}
			else if (s == "subroutineDec") {
				compileSubRoutine();
				xmlCode.getline(buffer, MAXLENGTH, '\n'); // buffer = </subRoutineDec>
			}
		}
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // }
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // </class>
		// compilation completed
	}

	void compileClassVarDec() {
		string s;
		char* buffer1 = new char[MAXLENGTH];
		xmlCode.getline(buffer1, MAXLENGTH, '\n');
		string kind = getLiteral(buffer1); // field or static
		xmlCode.getline(buffer1, MAXLENGTH, '\n');
		string type = getLiteral(buffer1); // variable type
		while (!xmlCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			xmlCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			if (s == ";") {
				break;
			}
			if (s == ",") {
				continue;
			}
			string varName = s;
			bool check = cst.add(kind, type, varName); // adding to class symbol table
			if (check == false) {
				cout << "Error redeclaration of variable " << varName << " at line " << endl;
			}
		}
	}

	void compileSubRoutine() {
		sst = *new subRoutineSymbolTable();
		char* buffer1 = new char[MAXLENGTH];
		xmlCode.getline(buffer1, MAXLENGTH, '\n');
		string s = getLiteral(buffer1);
		currSubRoutineType = s;
		if (s == "method") {
			sst.add("argument", className, "this");
		}
		xmlCode.getline(buffer1, MAXLENGTH, '\n');
		xmlCode.getline(buffer1, MAXLENGTH, '\n');
		currSubRoutineName = getLiteral(buffer1);
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // (
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <parameterList>
		int numParameters = compileParameterList();
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // )
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <subroutineBody>
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // {
		while (!xmlCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			xmlCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			if (s == "}") {
				break;
			}
			s = getLexemalItem(buffer);
			if (s == "varDec") {
				compileVarDec();
				xmlCode.getline(buffer, MAXLENGTH, '\n'); // </varDec>
			}
			else if (s == "statements") {
				vmCode << "function " << className << "." << currSubRoutineName << " " << sst.getLocalCount() << '\n'; // VM function declaration
				if (currSubRoutineType == "constructor") {
					vmCode << "push constant " << cst.getFieldCount() << '\n';
					vmCode << "call Memory.alloc 1" << '\n'; // call to allocate memory to field variables of a class
					vmCode << "pop pointer 0" << '\n'; // assign the memory address to this
				}
				else if (currSubRoutineType == "method") {
					vmCode << "push argument 0" << '\n';
					vmCode << "pop pointer 0" << '\n';
				}
				compileStatements();
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // }
				break;
			}
		}
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // </subroutineBody>
	}

	int compileParameterList() {
		string kind = "argument";
		int numParameters = 0;
		while (!xmlCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			xmlCode.getline(buffer, MAXLENGTH, '\n');
			string s = getLexemalItem(buffer);
			if (s == "/parameterList") {
				break;
			}
			s = getLiteral(buffer);
			if (s == ",") {
				continue;
			}
			s = getLiteral(buffer);
			string type = s;  // variable type
			xmlCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			string varName = s;  // variable name
			bool check = cst.find(varName);
			if (check == true) {
				cout << "Error redeclaration of variable " << varName << " at line " << endl;
			}
			check = sst.find(varName);  // add the arguments to the subroutine symbol table;
			if (check == true) {
				cout << "Error redeclaration of variable " << varName << " at line " << endl;
			}
			sst.add(kind, type, varName);
			numParameters++;
		}
		return numParameters;
	}

	void compileVarDec() {
		string s;
		char* buffer1 = new char[MAXLENGTH];
		xmlCode.getline(buffer1, MAXLENGTH, '\n');
		string kind = getLiteral(buffer1); // var
		kind = "local";
		xmlCode.getline(buffer1, MAXLENGTH, '\n');
		string type = getLiteral(buffer1); // variable type
		while (!xmlCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			xmlCode.getline(buffer, MAXLENGTH, '\n');
			s = getLiteral(buffer);
			if (s == ";") {
				break;
			}
			if (s == ",") {
				continue;
			}
			string varName = s;
			bool check = cst.find(varName);
			if (check == true) {
				cout << "Error redeclaration of variable " << varName << " at line " << endl;
			}
			check = sst.add(kind, type, varName); // adding to subroutine symbol table
			if (check == false) {
				cout << "Error redeclaration of variable " << varName << " at line " << endl;
			}
		}
	}

	void compileStatements() {
		while (!xmlCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			xmlCode.getline(buffer, MAXLENGTH, '\n');
			string s = getLexemalItem(buffer);
			if (s == "/statements") {
				break;
			}
			else if (s == "ifStatement") {
				compileIf();
			}
			else if (s == "letStatement") {
				compileLet();
				xmlCode.getline(buffer, MAXLENGTH, '\n'); // </letStatement>
			}
			else if (s == "doStatement") {
				compileDo();
				xmlCode.getline(buffer, MAXLENGTH, '\n'); // </doStatement>
			}
			else if (s == "whileStatement") {
				compileWhile();
				xmlCode.getline(buffer, MAXLENGTH, '\n'); // </whileStatement>
			}
			else if (s == "returnStatement") {
				compileReturn();
				xmlCode.getline(buffer, MAXLENGTH, '\n'); // </returnStatement>
			}
		}
	}

	void compileIf() {
		char* buffer1 = new char[MAXLENGTH];
		int tempLabelCount = labelCount;
		labelCount += 2;
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // if
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // (
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <expression>
		compileExpression();
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // )
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // {
		vmCode << "not" << '\n';
		vmCode << "if-goto " << className << "." << tempLabelCount + 1 << '\n';
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <statements>
		compileStatements();
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // }
		vmCode << "goto " << className << "." << tempLabelCount << '\n';
		vmCode << "label " << className << "." << tempLabelCount + 1 << '\n';
		xmlCode.getline(buffer1, MAXLENGTH, '\n');
		string s = getLiteral(buffer1);
		if (s == "else") {
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // {
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <statements>
			compileStatements();
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // }
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // </ifStatement>
		}
		else {
			// we have already read </ifStatement>
		}
		vmCode << "label " << className << "." << tempLabelCount<<'\n';
	}

	void compileLet() {
		char* buffer = new char[MAXLENGTH];
		xmlCode.getline(buffer, MAXLENGTH, '\n'); // let
		xmlCode.getline(buffer, MAXLENGTH, '\n'); 
		string varName = getLiteral(buffer); // variable name
		int index = 0;
		string kind;
		if (sst.find(varName) == true) {
			index = sst.getIndex(varName);
			kind = sst.getKind(varName);
		}
		else if (cst.find(varName) == true) {
			index = cst.getIndex(varName);
			kind = cst.getKind(varName);
		}
		else {
			cout << "Undefined variable at line " << endl;  // line number to be updated
		}
		xmlCode.getline(buffer, MAXLENGTH, '\n');
		string s = getLiteral(buffer);
		if (s == "[") {
			xmlCode.getline(buffer, MAXLENGTH, '\n'); // expression
			compileExpression();
			vmCode << "push " << kind << " " << index << '\n';
			vmCode << "add" << '\n';
			xmlCode.getline(buffer, MAXLENGTH, '\n'); // ]
			xmlCode.getline(buffer, MAXLENGTH, '\n'); // =
			xmlCode.getline(buffer, MAXLENGTH, '\n'); // expression
			compileExpression();
			vmCode << "pop temp 0" << '\n';
			vmCode << "pop pointer 1" << '\n';
			vmCode << "push temp 0" << '\n';
			vmCode << "pop that 0" << '\n';
			xmlCode.getline(buffer, MAXLENGTH, '\n'); // ;
		}
		else {
			xmlCode.getline(buffer, MAXLENGTH, '\n'); // expression
			compileExpression();
			vmCode << "pop " << kind << " " << index << '\n'; //  VM Code
			xmlCode.getline(buffer, MAXLENGTH, '\n'); // ;
		}
	}

	void compileDo() {
		char* buffer1 = new char[MAXLENGTH];
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // do
		xmlCode.getline(buffer1, MAXLENGTH, '\n');
		string id1 = getLiteral(buffer1);
		string id2;
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); 
		string s = getLiteral(buffer1);
		if (s == "(") {
			vmCode << "push pointer 0" << '\n';
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <expressionList>
			int numArgs = compileExpressionList();
 			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // )
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // ;
			vmCode << "call " << className << "." << id1 << " " << numArgs + 1 << '\n';
			vmCode << "pop temp 0" << '\n'; // void function call
		}
		else { // s == .
			string kind = "";
			string type = "";
			int index = 0;
			if (sst.find(id1) == true) {
				kind = sst.getKind(id1);
				type = sst.getType(id1);
				index = sst.getIndex(id1);
			}
			else if (cst.find(id1) == true) {
				kind = cst.getKind(id1);
				type = cst.getType(id1);
				index = cst.getIndex(id1);
			}
			else {
				// either undefined variable or OS class (check required)
			}
			if (type != "") {
				vmCode << "push " << kind << " " << index << '\n';
			}
			xmlCode.getline(buffer1, MAXLENGTH, '\n');
			id2 = getLiteral(buffer1);
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // (
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <expressionList>
			int numArgs = compileExpressionList();
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // )
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // ;
			if (type == "") {
				vmCode << "call " << id1 << "." << id2 << " " << numArgs << '\n';
			}
			else {
				vmCode << "call " << type << "." << id2 << " " << numArgs + 1 << '\n';
			}
			vmCode << "pop temp 0" << '\n';
		}
	}

	void compileWhile() {
		char* buffer1 = new char[MAXLENGTH];
		int tempLabelCount = labelCount;
		labelCount += 2;
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // while
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // (
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <expression>
		vmCode << "label " << className << "." << tempLabelCount << '\n';
		compileExpression();
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // )
		vmCode << "not" << '\n';
		vmCode << "if-goto " << className << "." << tempLabelCount + 1 << '\n';
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // {
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <statements>
		compileStatements();
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // }
		vmCode << "goto " << className << "." << tempLabelCount << '\n';
		vmCode << "label " << className << "." << tempLabelCount + 1 << '\n';
	}

	void compileReturn() {
		char* buffer1 = new char[MAXLENGTH];
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); // return
		xmlCode.getline(buffer1, MAXLENGTH, '\n');
		string s = getLexemalItem(buffer1);
		if (s == "expression") {
			compileExpression();
			vmCode << "return" << '\n';
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); //;
		}
		else {
			vmCode << "push constant 0" << '\n';
			vmCode << "return" << '\n';
			// s is ;
		}
	}

	int compileExpressionList() {
		int numArgs = 0;
		while (!xmlCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			xmlCode.getline(buffer, MAXLENGTH, '\n');
			string s = getLexemalItem(buffer);
			if (s == "/expressionList") {
				break;
			}
			s = getLiteral(buffer);
			if (s == ",") {
				continue;
			}
			// otherwise we have <expression>
			compileExpression();
			numArgs++;
		}
		return numArgs;
	}

	void compileExpression() {
		char* buffer1 = new char[MAXLENGTH]; 
		xmlCode.getline(buffer1, MAXLENGTH, '\n'); 
		string s;
		string op;
		s = getLexemalItem(buffer1);
		if (s == "/expression") {
			return;
		}
		if (s == "symbol") {
			cout << "Expected a term found " << s << endl;
		}
		else { // s is term
			compileTerm();
		}
		while (!xmlCode.eof()) {
			char* buffer = new char[MAXLENGTH];
			xmlCode.getline(buffer, MAXLENGTH, '\n');
			s = getLexemalItem(buffer);
			if (s == "/expression") {
				break;
			}
			if (s == "term") {
				cout << "Expected an operator " << endl;
			}
			else { // s is symbol
				op = getLiteral(buffer);
			}
			xmlCode.getline(buffer, MAXLENGTH, '\n');
			s = getLexemalItem(buffer);
			if (s == "/expression") {
				cout << "Expected a term" << endl;
				break;
			}
			if (s == "symbol") {
				cout << "Expected a term" << endl;
			}
			else { // s is term
				compileTerm();
			}
			if (op == "/") {
				vmCode << "call Math.divide 2"<<'\n';
			}
			else if (op == "*") {
				vmCode << "call Math.multiply 2" << '\n';
			}
			else if (op == "&amp;") {
				vmCode << "and" << '\n';
			}
			else if (op == "&lt;") {
				vmCode << "lt" << '\n';
			}
			else if (op == ">") {
				vmCode << "gt" << '\n';
			}
			else if (op == "+") {
				vmCode << "add" << '\n';
			}
			else if (op == "-") {
				vmCode << "sub" << '\n';
			}
			else if (op == "=") {
				vmCode << "eq" << '\n';
			}
			else if (op == "|") {
				vmCode << "or" << '\n';
			}
		}
	}

	void compileTerm() {
		char* buffer1 = new char[MAXLENGTH];
		xmlCode.getline(buffer1, MAXLENGTH, '\n');
		string termType = getLexemalItem(buffer1);
		if (termType == "integerConstant") {
			string s = getLiteral(buffer1);
			vmCode << "push constant " << s << '\n';
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // </term>
		}
		else if (termType == "stringConstant") {
			string s = getString(buffer1);
			int len = s.length();
			vmCode << "push constant " << len << '\n';
			vmCode << "call String.new 1" << '\n';
			for (auto i = 0; i < len; i++) {
				vmCode << "push constant " << (int)s[i] << '\n';
				vmCode << "call String.appendChar 2" << '\n';
			}
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // </term>
		}
		else if (termType == "keyword") {
			string s = getLiteral(buffer1);
			if (s == "true") {
				vmCode << "push constant 0" << '\n';
				vmCode << "not" << '\n';
			}
			else if (s == "false") {
				vmCode << "push constant 0" << '\n';
			}
			else if (s == "null") {
				vmCode << "push constant 0" << '\n';
			}
			else if(s == "this"){
				vmCode << "push pointer 0" << '\n';
			}
			xmlCode.getline(buffer1, MAXLENGTH, '\n'); // </term>
		}
		else if (termType == "symbol") {
			string s = getLiteral(buffer1);
			if (s == "(") {
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <expression>
				compileExpression();
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // )
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // </term>
			}
			else { // unary op term
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <term>
				compileTerm();
				if (s == "~") {
					vmCode << "not" << '\n';
				}
				else if (s == "-") {
					vmCode << "neg" << '\n';
				}
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // </term>
			}
		}
		else { 
			string varName = getLiteral(buffer1);
			xmlCode.getline(buffer1, MAXLENGTH, '\n');
			string s = getLiteral(buffer1);
			if (s == "[") {
				string kind;
				int index = 0;
				if (sst.find(varName) == true) {
					kind = sst.getKind(varName);
					index = sst.getIndex(varName);
				}
				else if (cst.find(varName) == true) {
					kind = cst.getKind(varName);
					index = cst.getIndex(varName);
				}
				else {
					cout << "Undefined identifier " << varName << endl;
				}
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <expression>
				compileExpression();
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // ]
				vmCode << "push " << kind << " " << index << '\n';
				vmCode << "add" << '\n';
				vmCode << "pop pointer 1" << '\n';
				vmCode << "push that 0" << '\n';
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // </term>
			}
			else if(s == "("){
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <expressionList>
				int numArgs = compileExpressionList();
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // )
				vmCode << "call " << className << "." << varName << " " << numArgs + 1 << '\n';
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // </term>
			}
			else if (s == ".") {
				string kind = "";
				string type = "";
				int index = 0;
				if (sst.find(varName) == true) {
					kind = sst.getKind(varName);
					type = sst.getType(varName);
					index = sst.getIndex(varName);
					vmCode << "push " << kind << " " << index <<'\n';
				}
				else if (cst.find(varName) == true) {
					kind = cst.getKind(varName);
					type = cst.getType(varName);
					index = cst.getIndex(varName);
					vmCode << "push " << kind << " " << index << '\n';
				}
				else {
					// undefined identifier or OS class
				}
				xmlCode.getline(buffer1, MAXLENGTH, '\n');
				string id = getLiteral(buffer1);
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // (
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // <expressionList>
				int numArgs = compileExpressionList();
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // )
				if (type != "") {
					vmCode << "call " << type << "." << id << " " << numArgs + 1 << '\n';
				}
				else {
					vmCode << "call " << varName << "." << id << " " << numArgs << '\n';
				}
				xmlCode.getline(buffer1, MAXLENGTH, '\n'); // </term>
			}
			else {
				// s is </term> and we have a non array variable
				string kind;
				int index = 0;
				if (sst.find(varName) == true) {
					kind = sst.getKind(varName);
					index = sst.getIndex(varName);
				}
				else if (cst.find(varName) == true) {
					kind = cst.getKind(varName);
					index = cst.getIndex(varName);
				}
				else {
					cout << s << endl;
					cout << "Undefined identifier " << varName << endl;
				}
				vmCode << "push " << kind << " " << index << '\n';
			}
		}
	}


public:
	codeGenerator(const char* filename) {
		this->filename = filename;
		xmlCode.open(this->filename + ".xml");
		vmCode.open(this->filename + ".vm");
		compileClass();
		labelCount = 0;
	}

};

int main(void) {
	tokenizer tokenizer(INPUT); // will tokenize the current JACK file and create filename.tok 
	tokenizer.removeComments();
	tokenizer.createTokens();
	unordered_map<int, int> lineMap = tokenizer.getLineMap();
	syntaxAnalyser syntaxAnalyser(INPUT, lineMap); // analyses the syntax and generates the vm code
	codeGenerator codeGenerator(INPUT);
	return 0;
}
