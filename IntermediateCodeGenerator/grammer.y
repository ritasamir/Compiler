%{
#include <bits/stdc++.h>
#include <unistd.h>
#include <string>
using namespace std;

int pc=0;
int labelsCount=0;
int varaiblesNum = 1;
string codeFileName;
extern  FILE *yyin;
extern  int yylex();
void yyerror(const char * s);
ofstream fileOut("byteCode.j");	
extern int lineCounter;
vector<string> byteCode ;
typedef enum {INT_T, FLOAT_T, ERROR_T} type_enum;
map<string, pair<int,type_enum> > symbTable ;
map<string,string> inst_list = {
	/* arithmetic operations */
	{"+", "add"},
	{"-", "sub"},
	{"/", "div"},
	{"*", "mul"},
	{"|", "or"},
	{"&", "and"},
	{"%", "rem"},
	/* relational operations */
	{"==", "if_icmpeq"},
	{"<=", "if_icmple"},
	{">=", "if_icmpge"},
	{"!=", "if_icmpne"},
	{">",  "if_icmpgt"},
	{"<",  "if_icmplt"}
};

vector<int> * makeList(int pc);
vector<int> * merge(vector<int> *list1, vector<int> *list2);
void backPatch(vector<int> *list,int lAdd);
bool symbTableContains(string op);
void addVar(int type, string id);
void addLine(string line);
void begin();
void end();
void printCode(void);
void printLineNumber(int num);
%}

%code requires {
	#include <vector>
	#include <string>
	using namespace std;
}

%start Method_body

%union{
char * charsVal;
struct {
vector<int> *trueList,*falseList;
    }bexpr;
	struct {
vector<int> *nextList;
    }stmt;
	int ival;
	float fval;
	int sType;
}
%token OR
%token AND
%token NOT
%token TRUE
%token FALSE
%token LEFT_BRACKET
%token RIGHTBRACKET
%token SEMICOLON
%token INTWORD
%token FLOATWORD
%token EQUAL
%token IFWORD
%token WHILEWORD
%token CURLEFTBRACKET
%token CURRIGHTBRACKET
%token ELSEWORD
%token MINUS
%token <charsVal> RELOP
%token <charsVal> ID
%token <charsVal> ARITHOP
%token <ival> INT
%token <fval> FLOAT
%type <bexpr> Boolean_expr
%type <stmt> Statement
%type <ival> Mark
%type <sType> Expression
%type <stmt> If
%type <stmt> While
%type <ival> GoTo
%type <stmt> Statement_list
%type <sType>PrimitiveType

%% 
Method_body:
			{
				begin();
			}
		Statement_list Mark
			{
				backPatch($2.nextList,$3);
				end();
			}
;

Statement_list:
		Statement
		| Statement Mark Statement_list
			{
				backPatch($1.nextList,$2);
				$$.nextList=$3.nextList;
			}
;

Statement:
		Declaration
			{
				$$.nextList= makeList(-1);
			} 
		| If
			{
				$$.nextList= $1.nextList;
			} 
		| While
			{
				$$.nextList= $1.nextList;
			}
		|Assignment
			{
				$$.nextList= makeList(-1);
			} 
;

While:
		Mark WHILEWORD LEFT_BRACKET Boolean_expr RIGHTBRACKET CURLEFTBRACKET Mark Statement_list CURRIGHTBRACKET
			{
				addLine("goto L_"+to_string($1));
				backPatch($4.trueList,$7);
				backPatch($8.nextList,$1);
				$$.nextList=$4.falseList;
			}
;

If:
		IFWORD LEFT_BRACKET Boolean_expr RIGHTBRACKET CURLEFTBRACKET Mark Statement_list GoTo CURRIGHTBRACKET ELSEWORD CURLEFTBRACKET Mark Statement_list CURRIGHTBRACKET 
			{
				backPatch($3.trueList,$6);
				backPatch($3.falseList,$12);
				$$.nextList= merge($7.nextList,$13.nextList);
				$$.nextList->push_back($8);
				
			}
;

GoTo:
	{
		$$=pc;
		addLine("goto ");
	}
;

Assignment:
		ID EQUAL Expression SEMICOLON
			{
				string sId($1);
				if(symbTableContains(sId)){
					if(symbTable[sId].second==$3){
						if($3==INT_T){
							addLine("istore "+to_string(symbTable[sId].first));
						}else if ($3==FLOAT_T){
							addLine("f "+to_string(symbTable[sId].first));
						}
					}else {
						//casting
					}
				}else{
					string err = "identifier: "+sId+" isn't declared yet";
					yyerror(err.c_str());
				}
			}
;
		

Declaration:
		PrimitiveType ID SEMICOLON
			{
				string sId($2);
				if($1==INT_T){
					addVar(INT_T,sId);
				}else if($1==FLOAT_T){
					addVar(FLOAT_T,sId);
				}
				
			}
;
PrimitiveType:
		INTWORD
			{
				$$=INT_T;
			}
		| FLOATWORD
			{
				$$=FLOAT_T;
			}
;

Expression:
		INT
			{	
				$$=INT_T;
				addLine("ldc "+to_string($1));
			}
		|FLOAT
			{
				$$=FLOAT_T;
				addLine("ldc "+to_string($1));
			}
		|ID
			{
				string sId($1);
				if(symbTableContains(sId)){
					$$=symbTable[sId].second;
					if(symbTable[sId].second==INT_T){
						addLine("iload "+to_string(symbTable[sId].first));
					}else if (symbTable[sId].second==FLOAT_T){
						addLine("fload "+to_string(symbTable[sId].first));
					}
				}else{
					string err = "identifier: "+sId+" isn't declared yet";
					yyerror(err.c_str());
					$$ = ERROR_T;
				}
			}
		
		| Expression ARITHOP Expression
			{
				string op($2);
				if($1==$3){
					if($1==INT_T){
						addLine("i"+inst_list[op]);
						$$=INT_T;
					}else{
						addLine("f"+inst_list[op]);
						$$=FLOAT_T;
					}
				}else{
					
					$$=FLOAT_T;
					//casting
				}
			}
		|LEFT_BRACKET Expression RIGHTBRACKET
			{
				$$=$2;
			}
		| MINUS Expression
			{
				$$=$2;
				addLine("ldc "+to_string(-1));
				if($2==INT_T){
					addLine("imul");
				}else{
					addLine("fmul");						
				}				
			}
		;

Boolean_expr:
		Boolean_expr  OR  Mark Boolean_expr
			{
				backPatch($1.falseList,$3);
				$$.falseList=$4.falseList;
				$$.trueList=merge($1.trueList,$4.trueList);
			}
		
		|Boolean_expr AND  Mark Boolean_expr
			{
				backPatch($1.trueList,$3);
				$$.trueList=$4.trueList;
				$$.falseList=merge($1.falseList,$4.falseList);
			}
		| NOT Boolean_expr
			{
				$$.trueList=$2.falseList;
				$$.falseList=$2.trueList;
			}
		| TRUE
			{
				$$.trueList=makeList(pc);
				$$.falseList=makeList(-1);
				addLine("goto ");
			}
		|FALSE
			{
				$$.falseList=makeList(pc);
				$$.trueList=makeList(-1);
				addLine("goto ");
			}
		| Expression RELOP Expression
			{
				string op($2);
				$$.trueList=makeList(pc);
				$$.falseList= makeList(pc+1);
				addLine(inst_list[op]+" ");
				addLine("goto ");
			}
		;
Mark : 
		{
			$$=labelsCount;
			addLine("L_"+to_string(labelsCount)+":");
			labelsCount++;
		}
		;
			
			
			
			
			
			
			
%% 

main (int argv, char * argc[])
{
	FILE *myfile;
	if(argv == 1) 
	{
		myfile = fopen("code.txt", "r");
		codeFileName = "code.txt";
	}
	else 
	{
		myfile = fopen(argc[1], "r");
		codeFileName = string(argc[1]);
	}
	if (!myfile) {
		printf("Code file does not exist!\n");
		char path[200];
		if (!getcwd(path, sizeof(path)))
		     {
		     return -1;
		     }
		printf("%s\n",path);  
		getchar();
		return -1;
	}
	yyin = myfile;
	yyparse();
	printCode();
}
void yyerror(const char * s)
{
	printf("error@%d: %s\n",lineCounter, s);
}

vector<int> * makeList(int pc){
	vector<int> *list = new vector<int>();
	if(pc!= -1) 
		list->push_back(pc);
	return list;
}
vector<int> * merge(vector<int> *list1, vector<int> *list2)
{
	if(list1 && list2){
		vector<int> *list = new vector<int> (*list1);
		list->insert(list->end(), list2->begin(),list2->end());
		return list;
	}else if(list1)
	{
		return list1;
	}else if (list2)
	{
		return list2;
	}else
	{
		return new vector<int>();
	}
}
void backPatch(vector<int> *list,int lAdd){
	for(int i=0;i< list->size();i++){
		byteCode[(*list)[i]] += "L_"+to_string(lAdd);
	}
}
bool symbTableContains(string op)
{
	return (symbTable.find(op) != symbTable.end());
}

void addVar(int type, string id){
	if(symbTableContains(id)){
		string err = "variable: "+id+" declared before";
		yyerror(err.c_str());
	}else{
		if(type==INT_T){
			addLine("iconst_0\nistore " + to_string(varaiblesNum));
		}else if(type==FLOAT_T){
			addLine("fconst_0\nfstore " + to_string(varaiblesNum));
		}
		symbTable[id]=make_pair(varaiblesNum++,(type_enum)type);
	}

} 

void addLine(string line){
	byteCode.push_back(line);
	pc++;
}

void begin(){
	addLine(".source " + codeFileName);
	addLine(".class public test\n.super java/lang/Object\n"); //code for defining class
	addLine(".method public <init>()V");
	addLine("aload_0");
	addLine("invokenonvirtual java/lang/Object/<init>()V");
	addLine("return");
	addLine(".end method\n");
	addLine(".method public static main([Ljava/lang/String;)V");
	addLine(".limit locals 100\n.limit stack 100");
	/*generate line*/
	addLine(".line 1");
}	

void end(){
	addLine("return");
	addLine(".end method");
}	
			
void printCode(void)
{
	for ( int i = 0 ; i < byteCode.size() ; i++)
	{
		fileOut<<byteCode[i]<<endl;
	}
}
void printLineNumber(int num)
{
	addLine(".line "+ to_string(num));
}			
