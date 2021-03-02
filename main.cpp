// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRT_SECURE_NO_WARNINGS
// error 统一，树的收敛
# include <iostream>
# include <stdio.h>
# include <string>
# include <string.h>
# include <stdlib.h>
# include <iostream>
# include <iomanip>
# include <vector>
# include <sstream>
# include <stdlib.h>
enum TYPE
{
  LP,
  RP,
  INT,
  FLOAT,
  DOT,
  NIL,
  T,
  QUOTE,
  STRING,
  SYMBOL,
  ERROR,
  ERR_STR,
  EXIT
} ;
using namespace std ;

int gline = 1 ;
int grow = 0 ;
int gtest;
struct Token_data
{
	string token ;
	TYPE token_type ;
	int start_line ;
	int start_row ;
	bool if_setpos ;
	bool if_first;
} ;

struct Err_msg 
{
	Token_data err_token ;
	string err_type ;
} ;

struct Tree
{
	string data ;
	Tree * left ;
	Tree * right ;
} ;

struct Symbol_table
{
	string name ;
	int level ;
	int isfunction ;
	int lambda ;
	bool first_define ;
	vector<string>param ;
	Tree * link ;
} ;

void Peektoken( char &nextchar ) ;
void Gettoken( Token_data &now_token ) ;
char Setbash() ;
bool Isinteger( string token ) ;
bool Isfloat( string token ) ;
TYPE Tokentype( string token ) ;
bool Issymbol( string token ) ;
void Set_postion( Token_data &now_token ) ;
void Readexp( Tree * data, bool &errbit ) ;
void Check_grammer( Tree * head, bool &errbit, Err_msg &now_error, Token_data token ) ;
void Print_tree( Tree * data, int &height ) ;
int Count_str( string token, char target ) ;
Tree * Eval_tree( Tree * input_tree, bool &errbit, int level, vector<Symbol_table>let_table ) ;
Tree * If_atom( Tree * input_tree ) ;
Tree * If_pair( Tree * input_tree ) ;
Tree * If_integer( Tree * input_tree ) ;
Tree * If_null( Tree * input_tree ) ;
Tree * If_list( Tree * input_tree ) ;
Tree * If_real( Tree * input_tree ) ;
Tree * If_string( Tree * input_tree ) ;
Tree * If_boolean( Tree * input_tree ) ;
Tree * If_symbol( Tree * input_tree ) ;
Tree * Eval_num( Tree * input_tree, bool do_float, string instr ) ;
Tree * Judge_num( Tree * input_tree, string instr ) ;
Tree * Add_string( Tree * input_tree ) ;
Tree * Judge_string( Tree * input_tree, string instr ) ;
Tree * Set_eqv( Tree * input_tree ) ;
Tree * Eval_not( Tree * input_tree ) ;
void Equal( Tree * first, Tree * compair, bool &false_bit ) ;
int Get_param( Tree * input_tree ) ;
Symbol_table Find_symbol( string name ) ;
void Set_non_list() ;
void Set_sys_symbol() ;
bool Judge_list( Tree * input ) ;
void Unbound_symbol( string symbol ) ;
void Non_function( Tree * err_data ) ;
bool Chece_level( int level, string instr ) ;
void Define_format() ;
bool Find_undefine_symbol( string now_define ) ;
void Argu_count_error( string instr ) ;
void If_no_return( Tree * err_data ) ;
void Argu_type( string instr, Tree * data ) ;
void Divide_zero() ;
void Cond_format() ;
void Copy_tree( Tree * input_, Tree * copy_ ) ;
void Lambda_format() ;
vector<Symbol_table>gsymbol_list ;
vector<Symbol_table>grecord_table ;
vector<Symbol_table>gtransform_table ;



Tree * g_op = NULL ;
bool gfunction = false ;
bool gsymbolreturn = false ;
bool g_errstr = false ;
string gnowname = "" ;

int main()
{
	cin >> gtest ;
	cout << "You are GAY\n";
	Set_sys_symbol();
	// cout << gsymbol_list.size() ;
	printf( "Welcome to OurScheme!\n\n" ) ;
	printf( "> " ) ; 
	char err_temp ;
	bool errbit ;
	Token_data now_token ;
	Tree * head = new Tree ;
	Tree * g_origin = NULL ;
	Tree * g_mid = new Tree ;
	int height = 0 ;
	int level = 0 ;
	vector<Symbol_table>let_table ;

	while ( true ) {
		Readexp( head, errbit ) ;

		if ( errbit != 1 ) {	
			Tree * temp = head;
			g_origin = head ;
			g_op = g_mid ;
			if ( g_origin != NULL ) {
				g_mid->data = g_origin->data ;
				g_mid->left = NULL ;
				g_mid->right = NULL ;
				Copy_tree( g_origin, g_mid ) ;

			} // if

			head = Eval_tree( temp, errbit, level, let_table ) ;


		} // if

		else if ( errbit == 1 ) {
			Peektoken( err_temp ) ;
			while ( err_temp != '\n' ) {
				Peektoken( err_temp ) ;
				if ( err_temp == EOF ) {
					printf( "> ERROR (no more input) : END-OF-FILE encountered\n" ) ;
					printf( "Thanks for using OurScheme!" ) ;
					exit( 0 );
				} // if
				
				
				scanf( "%c", &err_temp ) ;
			} // while

		} // else if

		if ( errbit != 1 ) {
			Print_tree( head, height ) ;
			cout << "\n" ;
		} // if

		Peektoken( err_temp ) ;
		if ( err_temp == EOF ) {
			printf( "> ERROR (no more input) : END-OF-FILE encountered\n" ) ;
			printf( "Thanks for using OurScheme!" ) ;
			exit( 0 );
		} // if

		gfunction = 0 ;
		gsymbolreturn = 0 ;
		gnowname = "" ;
		gline = 1 ;
		grow = 0 ;
		head = NULL ;
		head = new Tree ;
		errbit = 0 ;
		printf( "> " ) ;
		g_errstr = false ;
		g_origin = NULL ;
		g_mid = new Tree ;
		gtransform_table.clear() ;
		grecord_table.clear() ;
		/*
		vector<Symbol_table>::iterator it = gsymbol_list.begin() ;
		int i = 0 ;
		while ( it != gsymbol_list.end() && i < gsymbol_list.size() ) {
			if ( ( *it ).name == "#<procedure lambda>" ) gsymbol_list.erase( it );
			it = it + 1 ;
			i = i + 1 ;
		} // while
		*/
	} // while()


} // main()


bool Ifstop( string nowtoken, char nextchar ) {
	if ( nowtoken == "" ) return false ;
	else if ( nowtoken == "(" || nowtoken == ")" || nowtoken == "'" || nowtoken == ";" ) {
		return true ;
	} // if()

	else if ( nowtoken == "." && ( nextchar == '(' || nextchar == ')' ) ) return true ;
	else if ( nowtoken != "" && ( nextchar == '\n' || nextchar == ' ' || nextchar == '\t' 
																|| nextchar == '(' || nextchar == ')' || nextchar == '\'' 
																|| nextchar == '"' || nextchar == ';'  ) ) {
		return true ;
	} // else if

	

	else {
		return false ;
	} // else
} // Ifstop()

void Gettoken( Token_data &set_token ) {
	set_token.token = "" ;
	set_token.if_setpos = false ;
	char nextchar ;
	bool first_n = false ;

	if ( set_token.if_first == true ) first_n = true ;

	Peektoken( nextchar ) ;
	if ( nextchar == EOF ) {
		set_token.token = "-999999" ;
		return ;
	} // if
	
	if ( nextchar == ';' ) {
		Peektoken( nextchar ) ;
		while ( nextchar != '\n' ) {
			if ( nextchar == EOF ) {
				set_token.token = "-999999" ;
				return ;
			} // if

			scanf( "%c", &nextchar ) ;
			Peektoken( nextchar );
		} // while

		Peektoken( nextchar ) ;
		if ( nextchar == EOF ) {
			set_token.token = "-999999" ;
			return ;
		} // if

	} // if

	if ( nextchar == EOF ) {
		set_token.token = "-999999" ;
		return ;
	} // if

	while ( nextchar != EOF && Ifstop( set_token.token, nextchar ) == false ) {
		scanf( "%c", &nextchar ) ;
		if ( nextchar == '\n' && first_n == false ) {
			gline = gline + 1 ;
			grow = 0 ;
		} // if
		else if ( nextchar == '\n' && first_n == true && set_token.token == "" ) {
			gline = 1 ;
			grow = 0 ;
			first_n = false ;
		} // else if
		else grow = grow + 1 ;

	

		if ( nextchar == '"' ) {
			Set_postion( set_token ) ;
			set_token.token.push_back( nextchar ) ;
			
			Peektoken( nextchar ) ;
			if ( nextchar == EOF ) {
				set_token.token = "-999999" ;
				return ;
			} // if

			if ( nextchar == '\n' ) {
				g_errstr = true ;
				grow = grow + 1 ;
				return ;
			} // if
			else grow = grow + 1 ;
			scanf( "%c", &nextchar ) ;
			while ( nextchar != '"' ) {
				if ( nextchar == EOF ) {
					set_token.token = "-999999" ;
					return ;
				} // if

				if ( nextchar == '\\' && Setbash() != '@' ) {
					set_token.token.push_back( Setbash() ) ;
					scanf( "%c", &nextchar ) ;
					grow = grow + 1;
				} // if
				else
					set_token.token.push_back( nextchar ) ;
				
				
				
				Peektoken( nextchar ) ;
				if ( nextchar == EOF ) {
					set_token.token = "-999999" ;
					return ;
				} // if
				else if ( nextchar == '\n' ) {
					g_errstr = true ;
					grow = grow + 1 ;
					return ;
				} // else if
				else
					grow = grow + 1 ;
				scanf( "%c", &nextchar ) ;
			} // while

			set_token.token.push_back( nextchar ) ;
			return ;
			
		} // if
		else if ( nextchar == '(' ) {
			Peektoken( nextchar ) ;
			while ( nextchar == '\n' || nextchar == '\t' || nextchar == ' ' ) {
				scanf( "%c", &nextchar ) ;
				grow = grow + 1 ;
				Peektoken( nextchar ) ;
				if ( nextchar == EOF ) {
					set_token.token = "-999999" ;
					return ;
				} // if

			} // while

			Set_postion( set_token ) ;
			if ( nextchar == ')' ) {
				set_token.token = "()" ;
				scanf( "%c", &nextchar ) ;
				grow = grow + 1 ;
				return ;
			} // if
			else set_token.token.push_back( '(' ) ;
		} // else if 
		
		else if ( nextchar != ' ' && nextchar != '\n' && nextchar != '\t' ) {
			Set_postion( set_token ) ;
			set_token.token.push_back( nextchar ) ;
		} // else if

		Peektoken( nextchar ) ;
		if ( nextchar == EOF && set_token.token != "" ) return ;
		if ( nextchar == EOF ) {
			set_token.token = "-999999" ;
			return ;
		} // if

		if ( nextchar == ';' ) {
			Peektoken( nextchar ) ;
			while ( nextchar != '\n' ) {
				if ( nextchar == EOF ) {
					set_token.token = "-999999" ;
					return ;
				} // if

				scanf( "%c", &nextchar ) ;
				Peektoken( nextchar ) ;
				if ( nextchar == EOF ) {
					set_token.token = "-999999" ;
					return ;
				} // if

			} // while

			Peektoken( nextchar ) ;
			if ( nextchar == EOF ) {
				set_token.token = "-999999" ;
				return ;
			} // if
		} // if

	} // while


} // Gettoken()

char Setbash() {
	char temp ;
	Peektoken( temp ) ;
	if ( temp == 'n' ) return '\n' ;
	else if ( temp == 't' ) return '\t' ;
	else if ( temp == '"' ) return '\"' ;
	else if ( temp == '\\' ) return '\\' ;
	else return '@' ;
	return '@' ;
} // Setbash()

void Peektoken( char &nextchar ) {
	nextchar = cin.peek();
	if ( nextchar != EOF && nextchar != '\x1a' ) {
		return ;
	} // if
	else nextchar = EOF ;

} // Peektoken()



TYPE Tokentype( string token ) {
	if ( g_errstr == true ) return ERR_STR ;
	if ( token == "" ) return ERROR ;
	else if ( token == "(" ) return LP ;
	else if ( token == ")" ) return RP ;
	else if ( token == "." ) return DOT ;
	else if ( token == "t" || token == "#t" ) return T ;
	else if ( token == "nil" || token == "#f" || token == "()" ) return NIL ;
	else if ( token == "'" ) return QUOTE ;
	// else if ( token == "exit" ) return EXIT;
	else if ( *(token.begin() ) == '"' && token[token.size() - 1] == '"' 
						&& ( token.begin() ) != ( token.end() - 1 ) ) 
		return STRING ;
	else if ( token.find( '"' ) != string::npos ) return ERR_STR ;
	else if ( Isinteger( token ) == true ) return INT ;
	else if ( Isfloat( token ) == true ) return FLOAT ;
	else if ( Issymbol( token ) == true ) return SYMBOL ;
	else return ERROR ;
} // Tokentype()

bool Isinteger( string token ) {
	if ( Count_str( token, '.' ) > 0 ) return false ;
	else if ( Count_str( token, '+' ) + Count_str( token, '-' ) > 1 ) return false ;
	else if ( token.find( '+' ) != string::npos && token.find( '+' ) > 0 ) return false ;
	else if ( token.find( '-' ) != string::npos && token.find( '-' ) > 0 ) return false ;

	const char * TEMP = token.c_str() ;
	int i = 0 ;
	while ( i < strlen( TEMP ) ) {
		if ( ( TEMP[i] < 48 && TEMP[i] != 43 && TEMP[i] != 45 )
         || TEMP[i] > 57 ) return false ;
		i = i + 1 ;
	} // while

	i = 0;
	bool have_num = false ;
	while ( i < strlen( TEMP ) ) {
		if ( TEMP[i] >= 48 && TEMP[i] <= 57 ) have_num = true ;
		i = i + 1;

	} // while

	if ( have_num == false ) return false ;
	return true ;
} // Isinteger()

bool Isfloat( string token ) {
	if ( Count_str( token, '.' ) > 1 ) return false ;
	else if ( Count_str( token, '+' ) + Count_str( token, '-' ) > 1 ) return false ;
	else if ( token.find( '+' ) != string::npos && token.find( '+' ) > 0 ) return false ;
	else if ( token.find( '-' ) != string::npos && token.find( '-' ) > 0 ) return false ;

	const char * TEMP = token.c_str() ;
	int i = 0 ;
	while ( i < strlen( TEMP ) ) {
		if ( ( TEMP[i] < 48 && TEMP[i] != 43 && TEMP[i] != 45 
           && TEMP[i] != 46 ) || TEMP[i] > 57 ) return false ;
		i = i + 1 ;
	} // while

	i = 0 ;
	bool have_num = false ;
	while ( i < strlen( TEMP ) ) {
		if ( TEMP[i] >= 48 && TEMP[i] <= 57 ) have_num = true ;
		i = i + 1 ;

	} // while

	if ( have_num == false ) return false ;
	return true ;
} // Isfloat()

bool Issymbol( string token ) {
	const char * TEMP = token.c_str() ;
	int i = 0 ;
	if ( token.find( '"' ) != string::npos ) return false ;
	else if ( token.find( '\'' ) != string::npos ) return false ;
	else if ( token.find( ';' ) != string::npos ) return false ;
	else if ( token.find( '(' ) != string::npos ) return false;
	else if ( token.find( ')' ) != string::npos ) return false;


	return true ;
} // Issymbol()

void Set_postion( Token_data &now_token ) {
	if ( now_token.if_setpos == false ) {
		now_token.start_line = gline ;
		now_token.start_row = grow ;
		now_token.if_setpos = true ;
	} // if

	return ;
} // Set_postion()

void Check_grammer( Tree * data, bool &errbit, Err_msg &now_error, Token_data token ) {
	if ( errbit == 1 ) return ;

	

	if ( token.token_type == SYMBOL || token.token_type == INT || token.token_type == FLOAT 
       || token.token_type == STRING || token.token_type == NIL || token.token_type == T ) {
		if ( data == NULL ) {
			data = new Tree ;
			data->left = NULL ;
			data->right = NULL ;
			data->data = token.token ;
		} // if
		else if ( data != NULL ) {
			data->data = token.token ;
		} // else if

	} // if
	else if ( token.token_type == LP ) {

		if ( data == NULL ) {
			data = new Tree ;
			data->left = new Tree ;
			data->right = NULL ;
			data->left->left = NULL ;
			data->left->right = NULL ;
			Gettoken( token ) ;
			if ( token.token == "-999999" ) {
				now_error.err_token.token = "" ;
				now_error.err_type = "4" ;
				errbit = 1 ;

				return ;
			} // if

			token.token_type = Tokentype( token.token ) ;
			Check_grammer( data->left, errbit, now_error, token ) ;
		} // if

		else if ( data != NULL ) {
			data->left = new Tree ;
			data->left->left = NULL ;
			data->left->right = NULL ;
			Gettoken( token ) ;
			if ( token.token == "-999999" ) {
				now_error.err_token.token = "" ;
				now_error.err_type = "4" ;
				errbit = 1 ;

				return ;
			} // if

			token.token_type = Tokentype( token.token ) ;

			Check_grammer( data->left, errbit, now_error, token ) ;
		} // else if 

		if ( errbit == 1 ) return ;



		Gettoken( token ) ;

		if ( token.token == "-999999" ) {
			now_error.err_token.token = "" ;
			now_error.err_type = "4" ;
			errbit = 1 ;
			return ;
		} // if

		token.token_type = Tokentype( token.token ) ;

		while ( token.token_type != RP && token.token_type != DOT ) {
			if ( data->right == NULL ) {
				data->right = new Tree ;
				data->right->left = new Tree ;
				data->right->right = NULL ;
				data = data->right ;
				data->left->left = NULL ;
				data->left->right = NULL ;
				Check_grammer( data->left, errbit, now_error, token ) ;
			} // if

			if ( errbit == 1 ) return ;
			Gettoken( token ) ;
			if ( token.token == "-999999" ) {
				now_error.err_token.token = "" ;
				now_error.err_type = "4" ;
				errbit = 1 ;
				return ;
			} // if
			else if ( token.token_type == ERROR ) {
				now_error.err_token.token = token.token ;
				now_error.err_token.start_line = token.start_line ;
				now_error.err_token.start_row = token.start_row ;
				now_error.err_type = "1" ;
				errbit = 1 ;
				return ;
			} // else if
			else if ( token.token_type == ERR_STR ) {
				now_error.err_token.token = token.token ;
				now_error.err_token.start_line = token.start_line ;
				now_error.err_token.start_row = token.start_row ;
				now_error.err_type = "3" ;
				errbit = 1 ;
				return ;
			} // else if


			token.token_type = Tokentype( token.token ) ;
		} // while


		if ( token.token_type == DOT ) {
			Gettoken( token ) ;
			if ( token.token == "-999999" ) {
				now_error.err_token.token = "" ;
				now_error.err_type = "4" ;
				errbit = 1 ;
				return ;
			} // if

			token.token_type = Tokentype( token.token ) ;
			data->right = new Tree ;
			data->right->left = NULL ;
			data->right->right = NULL ;
			Check_grammer( data->right, errbit, now_error, token ) ;
			if ( errbit == 1 ) return ;
			Gettoken( token ) ;
			token.token_type = Tokentype( token.token ) ;
			if ( token.token == "-999999" ) {
				now_error.err_token.token = "" ;
				now_error.err_type = "4" ;
				errbit = 1 ;
				return ;
			} // if
			else if ( token.token_type != RP ) {
				now_error.err_token.token = token.token ;
				now_error.err_token.start_line = token.start_line ;
				now_error.err_token.start_row = token.start_row ;
				now_error.err_type = "2" ;
				errbit = 1 ;
				return ;
			} // else if
			else if ( token.token_type == RP ) {

				return;
			} // else if

		} // if

		else if ( token.token_type == RP ) {

			return;
		} // else if
		
		else {
			cout << "unkhnow error\n" ;
		} // else
	} // else if

	else if ( token.token_type == QUOTE ) {
		data->left = new Tree ;
		data->left->left = NULL ;
		data->left->right = NULL ;
		data->left->data = "quote" ;

		data->right = new Tree ;
		data->right->right = NULL ;
		data->right->left = new Tree ;
		data->right->left->left = NULL ;
		data->right->left->right = NULL ;

		Gettoken( token ) ;
		if ( token.token == "-999999" ) {
			now_error.err_token.token = "" ;
			now_error.err_type = "4" ;
			errbit = 1 ;
			return ;
		} // if


		token.token_type = Tokentype( token.token ) ;
		Check_grammer( data->right->left, errbit, now_error, token ) ;
		if ( errbit == 1 ) return ;
	} // else if

	else {
		if ( token.token_type == ERR_STR ) {
			now_error.err_token.token = token.token ;
			now_error.err_token.start_line = token.start_line ;
			now_error.err_token.start_row = token.start_row ;
			now_error.err_type = "3" ;
			errbit = 1 ;
			return ;
		} // if

		else if ( token.token == "-999999" ) {
			now_error.err_token.token = "";
			now_error.err_type = "4";
			errbit = 1;
			return;
		} // if

		else {
			now_error.err_token.token = token.token ;
			now_error.err_token.start_line = token.start_line ;
			now_error.err_token.start_row = token.start_row ;
			now_error.err_type = "1" ;
			errbit = 1 ;
			return ;
		} // else
	} // else


} // Check_grammer()


void Readexp( Tree * head, bool &errbit ) {
	Tree * data = head ;
	data->left = NULL ;
	data->right = NULL ;
	Err_msg err ;
	Token_data token ;
	token.if_first = true ;
	Gettoken( token ) ;
	token.if_first = false ;
	if ( token.token == "-999999" ) {

		err.err_type = "4";
		errbit = 1;

	} // if
	else {
		token.token_type = Tokentype( token.token ) ;
		Check_grammer( data, errbit, err, token ) ;
	} // else

	if ( err.err_type == "1" ) cout << 
		"ERROR (unexpected token) : atom or '(' expected when token at Line "
		<< err.err_token.start_line 
		<< " Column " << err.err_token.start_row << " is >>" << err.err_token.token << "<<\n\n" ;

	else if ( err.err_type == "2" ) cout << 
		"ERROR (unexpected token) : ')' expected when token at Line " 
		<< err.err_token.start_line 
		<< " Column " << err.err_token.start_row << " is >>" << err.err_token.token << "<<\n\n" ;

	else if ( err.err_type == "3" ) cout << 
		"ERROR (no closing quote) : END-OF-LINE encountered at Line " 
		<< err.err_token.start_line // + err.err_token.token.size()
		<< " Column " << grow << "\n\n" ;
	
	else if ( err.err_type == "4" ) {
		cout << "ERROR (no more input) : END-OF-FILE encountered\nThanks for using OurScheme!\n\n" ;
		exit( 0 ) ;
	} // else if

} // Readexp()

void Print_tree( Tree * data, int &height ) {
	if ( data->data == "" ) {

		cout << "( " ;
		height = height + 2 ;
		Print_tree( data->left, height ) ;

		while ( data->right != NULL ) {
			int i = 0 ;


			if ( data->right->data != "" && 
           ( data->right->data == "#f" || data->right->data == "nil" || data->right->data == "()" ) )
				data->right = NULL ;
			else if ( data->right->data != "" ) {
				while ( i < height ) {
					cout << "  " ;
					i = i + 2 ;
				} // while

				cout << ".\n" ;

				i = 0 ;
				while ( i < height ) {
					cout << "  " ;
					i = i + 2 ;
				} // while

				Print_tree( data->right, height ) ;
				data = data->right ;
			} // if

			else {
				while ( i < height ) {
					cout << "  " ;
					i = i + 2 ;
				} // while

				data = data->right ;
				Print_tree( data->left, height ) ;
			} // else


		} // while

		height = height - 2 ;
		int j = 0 ;
		while ( j < height ) {
			cout << "  ";
			j = j + 2;
		} // while

		cout << ")\n";
		
		
	} // if
	else {
		int type = Tokentype( data->data ) ;
		int temp = 0 ;
		double dtemp = 0.0;
		if ( type == INT ) {
			temp = atoi( data->data.c_str() ) ;
			
			cout << temp << "\n" ;
		} // if

		else if ( type == FLOAT ) {
			dtemp = atof( data->data.c_str() ) ; 
			printf( "%.3f\n", dtemp ) ;

		} // else if

		else if ( type == NIL ) {
			cout << "nil\n" ;

		} // else if

		else if ( type == T ) {
			cout << "#t\n" ;

		} // else if

		else cout << data->data << "\n" ;
		return ;
	} // else

} // Print_tree()


void Set_sys_symbol() {
	Symbol_table in_symbol ;
	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;

	in_symbol.name = "car";
	in_symbol.link->data = "#<procedure car>" ;
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "cdr";
	in_symbol.link->data = "#<procedure cdr>" ;
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "define";
	in_symbol.link->data = "#<procedure define>" ;
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "cons";
	in_symbol.link->data = "#<procedure cons>" ;
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "list";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure list>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "atom?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure atom?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "pair?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure pair?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "list?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure list?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "null?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure null?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "integer?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure integer?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "real?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure real?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "number?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure number?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "string?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure string?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "boolean?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure boolean?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "symbol?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure symbol?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "+";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure +>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "-";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure ->" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "*";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure *>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "/";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure />" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "not";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure not>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "and";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure and>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "or";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure or>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = ">";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure >>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = ">=";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure >=>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "<";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure <>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "<=";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure <=>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "=";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure =>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "string-append";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure string-append>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "string>?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure string>?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "string<?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure string<?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "string=?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure string=?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "eqv?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure eqv?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "equal?";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure equal?>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "begin";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure begin>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "if";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure if>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "quote";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure quote>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "cond";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure cond>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "clean-environment";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure clean-environment>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "exit";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure exit>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "let";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure let>" ;
	gsymbol_list.push_back( in_symbol ) ;

	in_symbol.link = new Tree ;
	in_symbol.link->left = NULL ;
	in_symbol.link->right = NULL ;
	in_symbol.name = "lambda";
	in_symbol.isfunction = false ;
	in_symbol.first_define = false ;
	in_symbol.link->data = "#<procedure lambda>" ;
	gsymbol_list.push_back( in_symbol ) ;
} // Set_sys_symbol()


Tree * Eval_tree( Tree * input_tree, bool &errbit, int level, vector<Symbol_table>let_table ) {
	if ( errbit == 1 ) return NULL ;
	vector<Symbol_table>::iterator it ;
	vector<Symbol_table>::iterator bk = gsymbol_list.end() ;
	

	if ( input_tree->data != "" && Tokentype( input_tree->data ) != SYMBOL )
		return input_tree ;

	else if ( input_tree->data != "" && Tokentype( input_tree->data ) == SYMBOL ) {
		int startlevel = level ;

		it = gtransform_table.begin() ;
		vector<Symbol_table>::iterator comp = gtransform_table.end() ;

		while ( it != gtransform_table.end() ) {
			if ( ( *it ).name == input_tree->data ) comp = it ;
			it = it + 1 ;
		} // while

		it = let_table.begin() ;
		bk = let_table.end() ;

		while ( startlevel >= 0 ) {
			it = let_table.begin() ;
			while ( it != let_table.end() ) {
				if ( ( *it ).name == input_tree->data && ( *it ).level == startlevel ) {
					bk = it ;
					startlevel = -1 ;
				} // if

				it = it + 1 ;
			} // while

			startlevel = startlevel - 1 ;
		} // while


		if ( bk != let_table.end() && comp != gtransform_table.end() ) {
			if ( ( *bk ).level >= ( *comp ).level ) return ( *bk ).link ;
			else return ( *comp ).link ;
		} // if

		else if ( bk != let_table.end() ) return ( *bk ).link ;
		else if ( comp != gtransform_table.end() ) return ( *comp ).link ;
		

		if ( Find_symbol( input_tree->data ).name != "-999999" ) {
			if ( Find_symbol( input_tree->data ).isfunction == 1 ) {
				gnowname = Find_symbol( input_tree->data ).name ;
				gfunction = true ;
				gsymbolreturn = true ;
				Tree * name = new Tree ;
				name->left = NULL ;
				name->right = NULL ;


				if ( Find_symbol( input_tree->data ).lambda == 1 ) name->data = "#<procedure lambda>" ;
				else {
					it = gsymbol_list.begin() ;
					bk = gsymbol_list.end() ;
					if ( Find_symbol( input_tree->data ).first_define == false ) {
						while ( it != gsymbol_list.end() ) {
							if ( Find_symbol( input_tree->data ).link == ( *it ).link 
                   && ( *it ).first_define == true ) bk = it;
							it = it + 1;
						} // while

						if ( bk != gsymbol_list.end() ) input_tree->data = ( *bk ).name ;
					} // if

					name->data = "#<procedure " + input_tree->data + ">";
				} // else

				return name ;
			} // if

			return Find_symbol( input_tree->data ).link ;
		} // if

		else {
			Unbound_symbol( input_tree->data ) ;
			errbit = 1 ;
			return NULL ;
		} // else

	} // else if

	else if ( input_tree->data == "" ) {
		Symbol_table commandline;
		vector<Symbol_table>copylet ;
		commandline.name = "test_line-----" ;
		if ( Judge_list( input_tree->right ) == false ) {
			Set_non_list();
			errbit = 1 ;
			return NULL ;
		} // if non-list

		input_tree->left = Eval_tree( input_tree->left, errbit, level + 1, let_table ) ;
		if ( errbit == 1 ) return NULL ;

		if ( gfunction == 1 ) {
			gfunction = 0 ;
			Symbol_table getlist ;
			Symbol_table binding ;
			if ( gsymbolreturn == false && gnowname == "" ) getlist = gsymbol_list.back() ;
			else {
				gsymbolreturn = 0 ;
				getlist = Find_symbol( gnowname ) ;
				gnowname = "" ;
			} // else

			Tree * bind = input_tree->right ;
			vector<string>::iterator getparam = getlist.param.begin() ;
			if ( Get_param( bind ) != getlist.param.size() ) {
				Argu_count_error( getlist.name ) ;
				errbit = 1 ;
				return NULL ;
			} // if

			vector<Symbol_table>cachetable ;
			while ( bind != NULL && Tokentype( bind->data ) != NIL ) {
				bind->left = Eval_tree( bind->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;
				binding.name = *getparam ;
				binding.level = level ;
				binding.lambda = false ;
				binding.isfunction = false ;
				binding.link = bind->left ;
				getparam = getparam + 1 ;
				cachetable.push_back( binding ) ;

				bind = bind->right ;
			} // while

			grecord_table.push_back( commandline ) ; 

			while ( !gtransform_table.empty() ) {
				grecord_table.push_back( *gtransform_table.begin() ) ;
				gtransform_table.erase( gtransform_table.begin() ) ;
				
			} // while

			while ( !cachetable.empty() ) {
				gtransform_table.push_back( *cachetable.begin() ) ;
				cachetable.erase( cachetable.begin() ) ;
			} // while

			copylet = let_table ;
			it = gtransform_table.begin() ;
			bk = gtransform_table.end() ;
			vector<Symbol_table>::iterator setlet = let_table.begin() ;
			vector<Symbol_table>::iterator runlet = let_table.end() ;
			while ( setlet != let_table.end() ) {
				it = gtransform_table.begin() ;
				bk = gtransform_table.end() ;
				while ( it != gtransform_table.end() ) {
					if ( ( *setlet ).name == ( *it ).name ) bk = it ;
					it = it + 1 ;
				} // while

				if ( bk == gtransform_table.end() ) {
					let_table.erase( setlet ) ;
					setlet = setlet - 1 ;
				} // if
				
				setlet = setlet + 1 ;
			} // while

			Tree * mid = new Tree ;
			mid->left = NULL ;
			mid->right = NULL ;
			Tree * run_link = getlist.link ;

			Tree * record = mid ;
			Copy_tree( run_link, mid ) ;
			Tree * get_return = record ;

			while ( record != NULL && Tokentype( record->data ) != NIL ) {
				get_return = record ;
				record->left = Eval_tree( record->left, errbit, level + 1, let_table ) ;
				record = record->right ;
			} // while

			let_table = copylet ;


			gtransform_table.clear() ;
			while ( grecord_table.back().name != "test_line-----" ) {
				gtransform_table.push_back( grecord_table.back() ) ;
				grecord_table.pop_back() ;
			} // while

			grecord_table.pop_back() ;
			return get_return->left ;
		} // if

		if ( If_symbol( input_tree )->data != "#t" ) {
			Non_function( input_tree->left ) ;
			errbit = 1 ;
			return NULL ;
		} // if attempt to apply non-function

		string instr = input_tree->left->data ;

		if ( Chece_level( level, instr ) == false ) {
			errbit = 1 ;
			return NULL ;
		} // if  define等参数等级



		if ( instr == "#<procedure quote>" ) {
			return input_tree->right->left ; 

		} // if

		else if ( instr == "#<procedure define>" ) {
			Tree * define_return = new Tree ;
			vector<Symbol_table>::iterator get_symbol = gsymbol_list.begin() ;
			vector<Symbol_table>::iterator record = gsymbol_list.end() ;
			Symbol_table temp_table ;
			bool function_define = false ;
			if ( Get_param( input_tree->right ) < 2 ) {
				Define_format() ;
				errbit = 1 ;
				return NULL ;
			} // if  define 参数数量

			if ( Tokentype( input_tree->right->left->data ) == NIL ) {
				Define_format() ;
				errbit = 1 ;
				return NULL ;
			} // if

			if ( If_pair( input_tree->right )->data == "#t" ) function_define = true ;
			if ( Get_param( input_tree->right ) > 2 ) function_define = true ;



			Symbol_table define_tb ;
			if ( function_define == true ) {
				string name = "" ;
				if ( If_pair( input_tree->right )->data == "#t" ) {

					define_tb.name = input_tree->right->left->left->data ;

					if ( Find_undefine_symbol( define_tb.name ) == false ) {
						Define_format() ;
						errbit = 1 ;
						return NULL ;
					} // if 定义系统变数，atom变数

					name = define_tb.name ;
					Tree * param_start = input_tree->right->left->right ;

					while ( param_start != NULL && Tokentype( param_start->data ) != NIL ) {
						define_tb.param.push_back( param_start->left->data ) ;
						param_start = param_start->right ;
					} // while

				} // if

				else {

					define_tb.name = input_tree->right->left->data ;
					name = define_tb.name ;
				} // else


				it = gsymbol_list.begin() ;
				bk = gsymbol_list.end() ;
				while ( it != gsymbol_list.end() ) {
					if ( ( *it ).name == name ) bk = it ;
					it = it + 1 ;
				} // while

				define_tb.isfunction = true ;
				define_tb.level = level ;
				define_tb.link = input_tree->right->right ;
				define_tb.lambda = 0 ;
				define_tb.first_define = true ;

				if ( bk != gsymbol_list.end() ) {
					( *bk ).isfunction = true ;
					( *bk ).level = define_tb.level ;
					( *bk ).lambda = 0 ;
					( *bk ).link = input_tree->right->right ;
					( *bk ).first_define = true ;
					( *bk ).param = define_tb.param ;
					
				} // if

				else gsymbol_list.push_back( define_tb ) ;

				define_return->data = name + " defined" ;
			} // if

			else if ( function_define == false ) {
				Tree * nowdefine = input_tree->right->left ;
				Tree * data = input_tree->right->right->left ;
				if ( Find_undefine_symbol( input_tree->right->left->data ) == false ) {
					Define_format() ;
					errbit = 1 ;
					return NULL ;
				} // if 定义系统变数，atom变数

				if ( data->data != "" && Tokentype( data->data ) == SYMBOL ) {
					get_symbol = gsymbol_list.begin() ;
					record = gsymbol_list.end() ;
					while ( get_symbol != gsymbol_list.end() ) {
						if ( ( *get_symbol ).name == data->data ) record = get_symbol ;
						get_symbol = get_symbol + 1 ;
					} // while

					if ( record != gsymbol_list.end() ) {
						temp_table.first_define = false ;
						temp_table.link = ( *record ).link;
						temp_table.param = ( *record ).param ;
						if ( ( *record ).isfunction == 1 ) {
							temp_table.isfunction = 1 ;

						} // if

						temp_table.name = nowdefine->data ;

						it = gsymbol_list.begin() ;
						bk = gsymbol_list.end() ;
						while ( it != gsymbol_list.end() ) {
							if ( ( *it ).name == input_tree->right->left->data ) bk = it ;
							it = it + 1 ;
						} // while

						if ( bk == gsymbol_list.end() ) gsymbol_list.push_back( temp_table ) ;
						else {
							( *bk ) = temp_table ;
						} // else

					} // if

					else {
						Unbound_symbol( data->data ) ;
						errbit = 1 ;
						return NULL ;
					} // else

				} // if

				else {

					input_tree->right->right->left = Eval_tree( data, errbit, level + 1, let_table ) ;

					if ( errbit == 1 ) return NULL ;
					if ( gfunction == 1 ) {

						it = gsymbol_list.begin() ;
						bk = gsymbol_list.end() ;
						while ( it != gsymbol_list.end() ) {
							if ( ( *it ).name == input_tree->right->left->data ) bk = it ;
							it = it + 1 ;
						} // while

						if ( bk != gsymbol_list.end() ) {
							( *bk ).lambda = true ;
							( *bk ).first_define = true ;
							( *bk ).isfunction = true ;
							( *bk ).link = gsymbol_list.back().link ;
							( *bk ).param = gsymbol_list.back().param ;
							( *bk ).name = input_tree->right->left->data ;
						} // if
						
						else {
							gsymbol_list.back().name = nowdefine->data ;
							gsymbol_list.back().first_define = true ; 
							gfunction = 0 ;
						} // else


					} // if

					else {

						it = gsymbol_list.begin() ;
						bk = gsymbol_list.end() ;
						while ( it != gsymbol_list.end() ) {
							if ( ( *it ).name == input_tree->right->left->data ) bk = it ;
							it = it + 1 ;
						} // while

						temp_table.name = nowdefine->data ;
						temp_table.level = 0 ;
						temp_table.link = input_tree->right->right->left ;
						temp_table.lambda = 0 ;
						temp_table.isfunction = false ;
						temp_table.first_define = true ;

						if ( bk == gsymbol_list.end() ) gsymbol_list.push_back( temp_table ) ;
						else {
							( *bk ) = temp_table ;
						} // else

					} // else

				} // else


				define_return->data = input_tree->right->left->data + " defined" ;
			} // else if
	
			define_return->left = NULL ;
			define_return->right = NULL ;
			
			return define_return ;
		} // else if

		else if ( instr == "#<procedure clean-environment>" ) {
			if ( Get_param( input_tree->right ) > 0 ) {
				Argu_count_error( "clean-environment" ) ;
				errbit = 1 ;
				return NULL ;
			} // if  c-e 参数数量

			gsymbol_list.clear() ;
			Set_sys_symbol() ;
			Tree * clean = new Tree ;
			clean->left = NULL ;
			clean->right = NULL ;
			clean->data = "environment cleaned" ;
			return clean ;
		} // else if

		else if ( instr == "#<procedure exit>" ) {
			if ( Get_param( input_tree->right ) > 0 ) {
				Argu_count_error( "exit" ) ;
				errbit = 1 ;
				return NULL ;
			} // if  exit 参数数量

			printf( "\nThanks for using OurScheme!\n\n" ) ;
			exit( 0 );
		} // else if

		else if ( instr == "#<procedure lambda>" ) {
			if ( Get_param( input_tree->right ) < 2 ) {
				Lambda_format() ;
				errbit = 1 ;
				return NULL ;
			} // if

			if ( Tokentype( input_tree->right->left->data ) != NIL 
           && If_atom( input_tree->right )->data == "#t" ) {
				Lambda_format() ;
				errbit = 1 ;
				return NULL ;
			} // if

			gfunction = true ;
			Symbol_table result ;
			result.isfunction = true ;
			if ( Tokentype( input_tree->right->left->data ) != NIL ) {
				Tree * param = input_tree->right->left ;
				result.param.push_back( param->left->data ) ;
				while ( param->right != NULL && Tokentype( param->right->data ) != NIL ) {
					param = param->right ;
					result.param.push_back( param->left->data ) ;
					
				} // while

				result.level = level ;

			} // if

			result.name = "#<procedure lambda>" ;
			result.link = input_tree->right->right ;
			result.lambda = 1 ;
			gsymbol_list.push_back( result ) ;
			Tree * user_define = new Tree ;
			user_define->left = NULL ;
			user_define->right = NULL ;
			user_define->data = "#<procedure lambda>" ;
			return user_define ;

		} // else if

		else if ( instr == "#<procedure let>" ) {
			// 检查参数数量，至少两个
			vector<Symbol_table>temprecord ;
			Tree * param_start = input_tree->right->left ;
			if ( Tokentype( param_start->data ) != NIL ) {
				Symbol_table temp ;
				temp.name = param_start->left->left->data ;
				Tree * link = param_start->left->right->left ;
				param_start->left->right->left = Eval_tree( link, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;

				temp.level = level ;
				temp.link = param_start->left->right->left ;

				it = let_table.begin() ;
				bk = let_table.end() ;
				while ( it != let_table.end() ) {
					if ( ( *it ).name == temp.name && ( *it ).level == temp.level ) bk = it ;
					it = it + 1 ;
				} // while

				if ( bk != let_table.end() ) {
					( *bk ).link = temp.link ;
				} // if

				else temprecord.push_back( temp ) ;

				while ( param_start->right != NULL && Tokentype( param_start->right->data ) != NIL ) {
					param_start = param_start->right ;
					temp.name = param_start->left->left->data ;
					Tree * link2 = param_start->left->right->left ;
					param_start->left->right->left = Eval_tree( link2, errbit, level + 1, let_table ) ;
					if ( errbit == 1 ) return NULL ;

					temp.level = level ;
					temp.link = param_start->left->right->left ;
					temp.isfunction = 0 ;
					/*
					it = let_table.begin() ;
					bk = let_table.end() ;
					while ( it != let_table.end() ) {
						if ( ( *it ).name == temp.name && ( *it ).level == temp.level ) bk = it ;
						it = it + 1 ;
					} // while

					if ( bk != let_table.end() ) {
						( *bk ).link = temp.link ;
					} // if

					else 
					*/
					temprecord.push_back( temp ) ;
				} // while

			} // if
			
			while ( !temprecord.empty() ) {
				let_table.push_back( *temprecord.begin() ) ;
				temprecord.erase( temprecord.begin() );
			} // while
			
			Tree * run = input_tree->right->right ;
			Tree * record = run ;
			while ( run != NULL && Tokentype( run->data ) != NIL ) {
				record = run ;
				run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;
				run = run->right ;
			} // while

			return record->left ;
		} // else if

		else if ( instr == "#<procedure eqv?>" ) {
			if ( Get_param( input_tree->right ) != 2 ) {
				Argu_count_error( "eqv?" ) ;
				errbit = 1 ;
				return NULL ;
			} // if  eqv参数数量


			Tree * first_data = input_tree->right->left ;
			Tree * second_data = input_tree->right->right->left ;

			first_data = Eval_tree( first_data, errbit, level + 1, let_table ) ;
			if ( errbit == 1 ) return NULL ;
			second_data = Eval_tree( second_data, errbit, level + 1, let_table ) ;
			if ( errbit == 1 ) return NULL ;

			if ( first_data == second_data ) {
				Tree * temp = new Tree ;
				temp->left = NULL ;
				temp->right = NULL ;
				temp->data = "#t" ;
				return temp ;
			} // if

			else return Set_eqv( input_tree->right ) ;
			

		} // else if

		else if ( instr == "#<procedure and>" || instr == "#<procedure or>" ) {
			Tree * result_tree = new Tree ;
			result_tree->left = NULL ;
			result_tree->right = NULL ;

			if ( Get_param( input_tree->right ) < 2  ) {
				if ( instr == "#<procedure and>" ) Argu_count_error( "and" ) ;
				else if ( instr == "#<procedure or>" ) Argu_count_error( "or" ) ;
				errbit = 1 ;
				return NULL ;
			} // if and or参数数量

			Tree * condition = input_tree->right ;
			Tree * check = input_tree->right ;

			
			while ( condition != NULL && Tokentype( condition->data ) != NIL ) {
				condition->left = Eval_tree( condition->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;
				check = condition ;
				if ( instr == "#<procedure and>" ) {
					if ( If_null( condition )->data == "#t" ) {
						result_tree = condition->left ;
						return result_tree ;
					} // if
				} // if

				else if ( instr == "#<procedure or>" ) {
					if ( If_null( condition )->data != "#t" ) {
						result_tree = condition->left ;
						return result_tree ;
					} // if
				} // else if

				
				condition = condition->right ;

			} // while

			return check->left ;

		} // else if

		else if ( instr == "#<procedure if>" ) {

			if ( Get_param( input_tree->right ) != 2 &&  Get_param( input_tree->right ) != 3 ) {
				if ( instr == "#<procedure if>" ) Argu_count_error( "if" ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查if参数数量

			Tree * noreturn = NULL ;
			Tree * copy_ins = input_tree ;
			Tree * copy_run = NULL ;
			
			copy_run = new Tree ;
			noreturn = copy_run ;

			copy_run->data = copy_ins->data ;
			copy_run->left = NULL ;
			copy_run->right = NULL ;
			Copy_tree( copy_ins, copy_run ) ;
			noreturn->left->data = "if" ;

			Tree * cond = input_tree->right ;
			Tree * true_param = input_tree->right->right ;
			Tree * false_param = NULL ;

			if ( true_param->right != NULL && Tokentype( true_param->right->data ) != NIL ) 
        false_param = true_param->right ;

			cond->left = Eval_tree( cond->left, errbit, level + 1, let_table ) ;
			if ( errbit == 1 ) return NULL ;
			if ( If_null( cond )->data == "#t" ) {
				if ( false_param == NULL ) {
					If_no_return( noreturn ) ;
					errbit = 1 ;
					return NULL ;
				} // if

				false_param->left = Eval_tree( false_param->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;
				return false_param->left ;
			} // if

			else {
				true_param->left = Eval_tree( true_param->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;
				return true_param->left ;
			} // else

		} // else if

		else if ( instr == "#<procedure cond>" ) {
			if ( Get_param( input_tree->right ) < 1 ) {
				Cond_format() ;
				errbit = 1 ;
				return NULL ;
			} // if 检查cond参数数量

			Tree * noreturn = NULL ;
			Tree * copy_ins = input_tree ;
			Tree * copy_run = NULL ;

			copy_run = new Tree ;
			noreturn = copy_run ;

			copy_run->data = copy_ins->data ;
			copy_run->left = NULL ;
			copy_run->right = NULL ;
			Copy_tree( copy_ins, copy_run ) ;
			noreturn->left->data = "cond" ;

			Tree * cond_tree = input_tree->right ;
			Tree * run = cond_tree->left ;
			bool get_else = false ;
			Tree * check = cond_tree ;
			while ( check != NULL && Tokentype( check->data ) != NIL ) {
				if ( If_atom( check )->data == "#t" || check->left->right == NULL ) { // 检查条件
					Cond_format() ;
					errbit = 1 ;
					return NULL ;
				} // if
				
				check = check->right ;
			} // while


			while ( cond_tree != NULL && Tokentype( cond_tree->data ) != NIL ) {
				if ( cond_tree->right == NULL || Tokentype( cond_tree->right->data ) == NIL ) get_else = true ;

				if ( get_else == false ) {
					run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ; // 计算条件
					if ( errbit == 1 ) return NULL ; 
					if ( If_null( run )->data != "#t" ) {

						if ( Judge_list( run->right ) == false ) { // 检查结果树non-list
							Cond_format() ;
							errbit = 1 ;
							return NULL ;
						} // if non-list

						if ( run->right == NULL ) {
							Cond_format() ;
							errbit = 1 ;
							return NULL ;
						} // if

						while ( run->right != NULL && Tokentype( run->right->data ) != NIL ) {
							run = run->right;
							run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ;
							if ( errbit == 1 ) return NULL ;
						} // while

						return run->left ;
					} // if

				} // if

				else if ( get_else == true ) {
					if ( run->left->data == "else" ) {
						if ( Judge_list( run->right ) == false ) { // 检查结果树non-list
							Cond_format() ;
							errbit = 1 ;
							return NULL ;
						} // if non-list

						if ( run->right == NULL ) {
							Cond_format() ;
							errbit = 1 ;
							return NULL ;
						} // if

						while ( run->right != NULL && Tokentype( run->right->data ) != NIL ) {
							run = run->right;
							run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ;
							if ( errbit == 1 ) return NULL ;
						} // while

						return run->left ;
					} // if

					else {
						run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ;
						if ( errbit == 1 ) return NULL ; 

						if ( If_null( run )->data != "#t" ) {
							if ( Judge_list( run->right ) == false ) { // 检查结果树non-list
								Cond_format() ;
								errbit = 1 ;
								return NULL ;
							} // if non-list

							if ( run->right == NULL ) {
								Cond_format() ;
								errbit = 1 ;
								return NULL ;
							} // if

							while ( run->right != NULL && Tokentype( run->right->data ) != NIL ) {
								run = run->right;
								run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ;
								if ( errbit == 1 ) return NULL ;
							} // while

							return run->left ;
						} // if

						else {
							If_no_return( noreturn ) ;
							errbit = 1 ;
							return NULL ;
						} // else

					} // else

				} // else if

				cond_tree = cond_tree->right ;
				run = cond_tree->left ;
				if ( cond_tree != NULL && Tokentype( cond_tree->data ) != NIL ) run = cond_tree->left ;
				get_else = false ;
			} // while

			return NULL ;
		} // else if

		else if ( instr == "#<procedure cdr>" ) {

			if ( Get_param( input_tree->right ) != 1 ) {
				Argu_count_error( "cdr" ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查cdr参数数量

			input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;

			if ( errbit == 1 ) return NULL ;
			if ( If_pair( input_tree->right )->data == "nil" ) {
				Argu_type( "cdr", input_tree->right->left ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查cdr参数类型

			if ( input_tree->right->left->right != NULL ) return input_tree->right->left->right ;
			else {
				input_tree->right->left->right = new Tree ;
				input_tree->right->left->right->left = NULL ;
				input_tree->right->left->right->right = NULL ;
				input_tree->right->left->right->data = "nil" ;

				return input_tree->right->left->right ;
			} // else

		} // else if

		else if ( instr == "#<procedure car>" ) {
			if ( Get_param( input_tree->right ) != 1 ) {
				Argu_count_error( "car" ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查car参数数量


			input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;

			if ( errbit == 1 ) return NULL ;
			if ( If_pair( input_tree->right )->data == "nil" ) {
				Argu_type( "car", input_tree->right->left ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查car参数类型

			return input_tree->right->left->left ;



		} // else if

		else if ( instr == "#<procedure cons>" ) {

			if ( Get_param( input_tree->right ) != 2 ) {
				Argu_count_error( "cons" ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查cons参数数量

			Tree * eval_tree = new Tree ;
			eval_tree->left = NULL ;
			eval_tree->right = NULL ;

			eval_tree->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;
			if ( errbit == 1 ) return NULL ;
			eval_tree->right = Eval_tree( input_tree->right->right->left, errbit, level + 1, let_table ) ;
			if ( errbit == 1 ) return NULL ;

			return eval_tree ;


		} // else if

		else if ( instr == "#<procedure atom?>" || instr == "#<procedure pair?>" ||
              instr == "#<procedure list?>" || instr == "#<procedure null?>" ||
              instr == "#<procedure integer?>" || instr == "#<procedure real?>" ||
              instr == "#<procedure number?>" || instr == "#<procedure string?>" ||
              instr == "#<procedure boolean?>" || instr == "#<procedure symbol?>" ) {

			if ( instr == "#<procedure atom?>" ) {
				if ( Get_param( input_tree->right ) != 1 ) {
					Argu_count_error( "atom?" ) ;
					errbit = 1 ;
					return NULL ;
				} // if 检查atom参数数量

				input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;

				return If_atom( input_tree->right ) ;
			} // if

			else if ( instr == "#<procedure pair?>" ) {
				if ( Get_param( input_tree->right ) != 1 ) {
					Argu_count_error( "pair?" ) ;
					errbit = 1 ;
					return NULL ;
				} // if 检查pair参数数量

				input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;

				return If_pair( input_tree->right );
			} // else if

			else if ( instr == "#<procedure list?>" ) {
				if ( Get_param( input_tree->right ) != 1 ) {
					Argu_count_error( "list?" ) ;
					errbit = 1 ;
					return NULL ;
				} // if 检查list参数数量

				input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;

				return If_list( input_tree->right ) ;
			} // else if

			else if ( instr == "#<procedure null?>" ) {
				if ( Get_param( input_tree->right ) != 1 ) {
					Argu_count_error( "null?" ) ;
					errbit = 1 ;
					return NULL ;
				} // if 检查null参数数量

				input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;

				return If_null( input_tree->right ) ;
			} // else if

			else if ( instr == "#<procedure integer?>" ) {
				if ( Get_param( input_tree->right ) != 1 ) {
					Argu_count_error( "integer?" ) ;
					errbit = 1 ;
					return NULL ;
				} // if 检查integer参数数量

				input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;

				return If_integer( input_tree->right ) ;
			} // else if

			else if ( instr == "#<procedure real?>" || instr == "#<procedure number?>" ) {
				if ( Get_param( input_tree->right ) != 1 ) {
					if ( instr == "#<procedure real?>" ) Argu_count_error( "real?" ) ;
					else Argu_count_error( "number?" ) ;
					errbit = 1 ;
					return NULL ;
				} // if 检查real/number参数数量

				input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;

				return If_real( input_tree->right ) ;
			} // else if

			else if ( instr == "#<procedure string?>" ) {
				if ( Get_param( input_tree->right ) != 1 ) {
					Argu_count_error( "string?" ) ;
					errbit = 1 ;
					return NULL ;
				} // if 检查string参数数量

				input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;

				return If_string( input_tree->right ) ;
			} // else if

			else if ( instr == "#<procedure boolean?>" ) {
				if ( Get_param( input_tree->right ) != 1 ) {
					Argu_count_error( "boolean?" ) ;
					errbit = 1 ;
					return NULL ;
				} // if 检查boolean参数数量

				input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;

				return If_boolean( input_tree->right ) ;
			} // else if

			else if ( instr == "#<procedure symbol?>" ) {
				if ( Get_param( input_tree->right ) != 1 ) {
					Argu_count_error( "symbol?" ) ;
					errbit = 1 ;
					return NULL ;
				} // if 检查symbol参数数量

				input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;

				return If_symbol( input_tree->right ) ;
			} // else if 

		} // else if

		else if ( instr == "#<procedure begin>" ) {
			if ( Get_param( input_tree->right ) < 1 ) {
				Argu_count_error( "begin" ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查begin参数数量

			Tree * run = input_tree->right ;
			Tree * record = run ;
			while ( run != NULL && Tokentype( run->data ) != NIL ) {
				record = run ;
				run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;
				run = run->right ;
			} // while

			return record->left ;
		} // else if

		else if ( instr == "#<procedure >>" || instr == "#<procedure >=>" 
              || instr == "#<procedure <>" || instr == "#<procedure <=>" || instr == "#<procedure =>" ) {

			if ( Get_param( input_tree->right ) < 2 ) {
				if ( instr == "#<procedure >>" ) Argu_count_error( ">" ) ;
				else if ( instr == "#<procedure >=>" ) Argu_count_error( ">=" ) ;
				else if ( instr == "#<procedure <>" ) Argu_count_error( "<" ) ;
				else if ( instr == "#<procedure <=>" ) Argu_count_error( "<=" ) ;
				else if ( instr == "#<procedure =>" ) Argu_count_error( "=" ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查每个符号参数数量

			Tree * run = input_tree->right ;
			while ( run != NULL && Tokentype( run->data ) != NIL ) {
				run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;
				if ( If_real( run )->data == "nil" ) {
					if ( instr == "#<procedure >>" ) Argu_type( ">", run->left ) ;
					else if ( instr == "#<procedure >=>" ) Argu_type( ">=", run->left ) ;
					else if ( instr == "#<procedure <>" ) Argu_type( "<", run->left ) ;
					else if ( instr == "#<procedure <=>" ) Argu_type( "<=", run->left ) ;
					else if ( instr == "#<procedure =>" ) Argu_type( "=", run->left ) ;
					errbit = 1 ;
					return NULL ;
				} // if

				run = run->right ;
			} // while

			return Judge_num( input_tree->right, instr ) ;
		} // else if		


		else if ( instr == "#<procedure string-append>" ) {
			if ( Get_param( input_tree->right ) < 2 ) {
				Argu_count_error( "string-append" ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查string-append参数数量

			Tree * run = input_tree->right ;
			while ( run != NULL && Tokentype( run->data ) != NIL ) {
				run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;
				if ( If_string( run )->data == "nil" ) {
					Argu_type( "string-append", run->left ) ;
					errbit = 1 ;
					return NULL ;
				} // if

				run = run->right ;
			} // while

			return Add_string( input_tree->right ) ;
		} // else if

		else if ( instr == "#<procedure string>?>" 
              || instr == "#<procedure string<?>" || instr == "#<procedure string=?>" ) {

			if ( Get_param( input_tree->right ) < 2 ) {
				if ( instr == "#<procedure string<?>" ) Argu_count_error( "string<?" ) ;
				else if ( instr == "#<procedure string>?>" ) Argu_count_error( "string>?" ) ;
				else if ( instr == "#<procedure string=?>" ) Argu_count_error( "string=?" ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查string符号参数数量

			Tree * run = input_tree->right ;
			while ( run != NULL && Tokentype( run->data ) != NIL ) {
				run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;
				if ( If_string( run )->data == "nil" ) {
					if ( instr == "#<procedure string>?>" ) Argu_type( "string>?", run->left ) ;
					else if ( instr == "#<procedure string<?>" ) Argu_type( "string<?", run->left ) ;
					else if ( instr == "#<procedure string=?>" ) Argu_type( "string=?", run->left ) ;
					errbit = 1 ;
					return NULL ;
				} // if

				run = run->right ;
			} // while

			return Judge_string( input_tree->right, instr ) ;
		} // else if


		else if ( instr == "#<procedure list>" ) {
			if ( input_tree->right == NULL ) {
				Tree * temp = new Tree ;
				temp->left = NULL ;
				temp->right = NULL ;
				temp->data = "nil" ;
				return temp ;
			} // if

			Tree * run = input_tree->right ;
			while ( run != NULL && Tokentype( run->data ) != NIL ) {
				run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;
				run = run->right ;
			} // while

			return input_tree->right ;
		} // else if

		else if ( instr == "#<procedure not>" ) {
			if ( Get_param( input_tree->right ) != 1 ) {
				Argu_count_error( "not" ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查not参数数量

			input_tree->right->left = Eval_tree( input_tree->right->left, errbit, level + 1, let_table ) ;
			if ( errbit == 1 ) return NULL ;
			return Eval_not( input_tree->right ) ;
		} // else if

		else if ( instr == "#<procedure +>" || instr == "#<procedure ->" 
              || instr == "#<procedure *>" || instr == "#<procedure />" ) {
			bool do_float = false ;

			if ( Get_param( input_tree->right ) < 2 ) {
				if ( instr == "#<procedure +>" ) Argu_count_error( "+" ) ;
				else if ( instr == "#<procedure ->" ) Argu_count_error( "-" ) ;
				else if ( instr == "#<procedure *>" ) Argu_count_error( "*" ) ;
				else if ( instr == "#<procedure />" ) Argu_count_error( "/" ) ;

				errbit = 1 ;
				return NULL ;
			} // if 检查每个符号参数数量


			Tree * run = input_tree->right ;
			while ( run != NULL && Tokentype( run->data ) != NIL ) {
				run->left = Eval_tree( run->left, errbit, level + 1, let_table ) ;
				if ( errbit == 1 ) return NULL ;
				if ( If_real( run )->data == "nil" ) {
					if ( instr == "#<procedure +>" ) Argu_type( "+", run->left ) ;
					else if ( instr == "#<procedure ->" ) Argu_type( "-", run->left ) ;
					else if ( instr == "#<procedure *>" ) Argu_type( "*", run->left ) ;
					else if ( instr == "#<procedure />" ) Argu_type( "/", run->left ) ;

					errbit = 1 ;
					return NULL ;
				} // if
				else if ( instr == "#<procedure />" && atof( run->left->data.c_str() ) == 0 
                  && run != input_tree->right ) {
					Divide_zero() ;
					errbit = 1 ;
					return NULL ;
				} // else if

				if ( Tokentype( run->left->data ) == FLOAT ) do_float = true ;
				run = run->right ;
			} // while

			  // 检查参数数量，包括 .
			  // 检查参数类型
			  // 检查浮点数

			return Eval_num( input_tree->right, do_float, instr );
		} // else if

		else if ( instr == "#<procedure equal?>" ) {
			if ( Get_param( input_tree->right ) != 2 ) {
				Argu_count_error( "equal?" ) ;
				errbit = 1 ;
				return NULL ;
			} // if 检查equal参数数量

			Tree * first = input_tree->right->left ;
			Tree * compare = input_tree->right->right->left ;

			first = Eval_tree( first, errbit, level + 1, let_table ) ; 
			if ( errbit == 1 ) return NULL ;
			compare = Eval_tree( compare, errbit, level + 1, let_table ) ; 
			if ( errbit == 1 ) return NULL ;

			bool false_bit = 0 ;
			Equal( first, compare, false_bit ) ;
			Tree * result = new Tree ;
			result->left = NULL ;
			result->right = NULL ;
			if ( false_bit == 1 ) result->data = "#f" ;
			else if ( false_bit == 0 ) result->data = "#t" ;
			return result ;
		} // else if

	} // else if


	errbit = 1 ;
	cout << "unkhown error\n\n" ;
	return NULL ;
} // Eval_tree()

void Copy_tree( Tree * input_, Tree * copy_ ) {
	if ( input_->left != NULL ) {
		copy_->left = new Tree ;
		copy_->left->left = NULL ;
		copy_->left->right = NULL ;
		copy_->left->data = input_->left->data ;
		Copy_tree( input_->left, copy_->left ) ;

	} // if

	if ( input_->right != NULL ) {
		copy_->right = new Tree ;
		copy_->right->left = NULL ;
		copy_->right->right = NULL ;
		copy_->right->data = input_->right->data ;
		Copy_tree( input_->right, copy_->right ) ;

	} // if
} // Copy_tree()

int Count_str( string token, char target )
{
	int i = 0 ;
	int time = 0 ;
	while ( i < token.size() ) {
		if ( token[i] == target ) time = time + 1 ;
		i = i + 1 ;
	} // while

	return time ;
} // Count_str()

Tree * If_atom( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;


	if ( input_tree->left->data == "" ) {
		temp->data = "nil" ;
		return temp ;
	} // if

	if ( Tokentype( input_tree->left->data ) != ERROR 
       || Tokentype( input_tree->left->data ) != ERR_STR ) {
		temp->data = "#t" ;
		return temp ;
	} // if

	else {
		temp->data = "nil" ;
		return temp ;
	} // else

} // If_atom()

Tree * If_pair( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;
	if ( If_atom( input_tree )->data == "nil" ) {
		temp->data = "#t" ;
		return temp ;
	} // if

	else if ( If_atom( input_tree )->data == "#t" ) {
		temp->data = "nil" ;
		return temp ;
	} // else if

	temp->data = "nil" ;
	return temp ;

} // If_pair()

Tree * If_list( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;

	if ( input_tree == NULL ) {
		temp->data = "#t" ;
		return temp ;
	} // if

	if ( input_tree->left->data != "" ) {
		temp->data = "nil" ;
		return temp ;
	} // if

	Tree * run = input_tree->left ;
	while ( run->right != NULL ) {
		run = run->right ;
		if ( run->data != "" && Tokentype( run->data ) != NIL ) {
			temp->data = "nil" ;
			return temp ;
		} // if

	} // while

	temp->data = "#t" ;
	return temp ;
} // If_list()

Tree * If_null( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;

	if ( input_tree->left->data == "" ) {
		temp->data = "nil" ;
		return temp ;
	} // if

	else if ( Tokentype( input_tree->left->data ) == NIL ) {
		temp->data = "#t" ;
		return temp ;
	} // else if

	else {
		temp->data = "nil" ;
		return temp ;
	} // else

} // If_null()

Tree * If_integer( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;
	if ( input_tree->left->data == "" ) {
		temp->data = "nil" ;
		return temp ;
	} // if

	else if ( Tokentype( input_tree->left->data ) == INT ) {
		temp->data = "#t" ;
		return temp ;
	} // else if
	
	else {
		temp->data = "nil" ;
		return temp ;
	} // else

} // If_integer()

Tree * If_real( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;
	if ( input_tree->left->data == "" ) {
		temp->data = "nil" ;
		return temp ;
	} // if

	else if ( Tokentype( input_tree->left->data ) == INT 
            || Tokentype( input_tree->left->data ) == FLOAT ) {
		temp->data = "#t" ;
		return temp ;
	} // else if

	else {
		temp->data = "nil" ;
		return temp ;
	} // else

} // If_real()

Tree * If_string( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;

	if ( input_tree->left->data == "" ) {
		temp->data = "nil" ;
		return temp ;
	} // if

	else if ( Tokentype( input_tree->left->data ) == STRING ) {
		temp->data = "#t" ;
		return temp ;
	} // else if

	else {
		temp->data = "nil" ;
		return temp ;
	} // else

} // If_string()

Tree * If_boolean( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;

	if ( input_tree->left->data == "" ) {
		temp->data = "nil" ;
		return temp ;
	} // if

	else if ( Tokentype( input_tree->left->data ) == T || Tokentype( input_tree->left->data ) == NIL ) {
		temp->data = "#t" ;
		return temp ;
	} // else if

	else {
		temp->data = "nil" ;
		return temp ;
	} // else

} // If_boolean()

Tree * If_symbol( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;
	vector<Symbol_table>::iterator it ;
	vector<Symbol_table>::iterator bk = gsymbol_list.end() ;
	it = gsymbol_list.begin() ;

	if ( input_tree->left->data == "" ) {
		temp->data = "nil" ;
		return temp ;
	} // if

	while ( it != gsymbol_list.end() ) {
		if ( ( *it ).name == input_tree->left->data ) bk = it ;
		it = it + 1 ;
	} // while

	if ( Tokentype( input_tree->left->data ) == SYMBOL || bk != gsymbol_list.end() ) { // symbol bug
		temp->data = "#t" ;
		return temp ;
	} // if

	else {
		temp->data = "nil" ;
		return temp ;
	} // else

} // If_symbol()

Tree * Eval_num( Tree * input_tree, bool do_float, string instr ) {
	double sum_f = 0.0 ;
	int sum = 0 ;
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;

	if ( instr == "#<procedure +>" ) {
		while ( input_tree != NULL && Tokentype( input_tree->data ) != NIL ) {
			sum = sum + atoi( input_tree->left->data.c_str() ) ;
			sum_f = sum_f + atof( input_tree->left->data.c_str() ) ;
			input_tree = input_tree->right ;
		} // while

	} // if

	else if ( instr == "#<procedure ->" ) {
		sum = atoi( input_tree->left->data.c_str() ) ;
		sum_f = atof( input_tree->left->data.c_str() ) ;
		input_tree = input_tree->right ;
		while ( input_tree != NULL && Tokentype( input_tree->data ) != NIL ) {
			sum = sum - atoi( input_tree->left->data.c_str() ) ;
			sum_f = sum_f - atof( input_tree->left->data.c_str() ) ;
			input_tree = input_tree->right ;
		} // while

	} // else if

	else if ( instr == "#<procedure *>" ) {
		sum = atoi( input_tree->left->data.c_str() ) ;
		sum_f = atof( input_tree->left->data.c_str() ) ;
		input_tree = input_tree->right ;
		while ( input_tree != NULL && Tokentype( input_tree->data ) != NIL ) {
			sum = sum * atoi( input_tree->left->data.c_str() ) ;
			sum_f = sum_f * atof( input_tree->left->data.c_str() ) ;
			input_tree = input_tree->right ;
		} // while

	} // else if

	else if ( instr == "#<procedure />" ) {
		sum = atoi( input_tree->left->data.c_str() ) ;
		sum_f = atof( input_tree->left->data.c_str() ) ;
		input_tree = input_tree->right ;
		while ( input_tree != NULL && Tokentype( input_tree->data ) != NIL ) {
			if ( do_float == false ) sum = sum / atoi( input_tree->left->data.c_str() ) ;
			sum_f = sum_f / atof( input_tree->left->data.c_str() ) ;
			input_tree = input_tree->right ;
		} // while

	} // else if

	else return NULL ;
	stringstream ss ;

	if ( do_float == true ) {
		ss << sum_f ;
		
		temp->data = ss.str() ;
		if ( temp->data.find( '.' ) == string::npos ) temp->data = temp->data + ".0" ;
		return temp ;
	} // if

	else {
		ss << sum ;
		temp->data = ss.str() ;
		return temp ;
	} // else
} // Eval_num()

Tree * Judge_num( Tree * input_tree, string instr ) {

	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;
	double data1 = 0 ;
	double data2 = 0 ;
	
	data1 = atof( input_tree->left->data.c_str() ) ;
	input_tree = input_tree->right ;
	data2 = atof( input_tree->left->data.c_str() ) ;


	if ( ( instr == "#<procedure >>" && data1 <= data2 ) ||
       ( instr == "#<procedure >=>" && data1 < data2 ) ||
       ( instr == "#<procedure <>" && data1 >= data2 ) ||
       ( instr == "#<procedure <=>" && data1 > data2 ) ||
       ( instr == "#<procedure =>" && data1 != data2 ) ) {
		temp->data = "nil" ;
		return temp ;
	} // if

	while ( input_tree->right != NULL && Tokentype( input_tree->right->data ) != NIL ) {
		input_tree = input_tree->right ;
		data1 = data2 ;
		data2 = atof( input_tree->left->data.c_str() ) ;
		if ( ( instr == "#<procedure >>" && data1 <= data2 ) ||
         ( instr == "#<procedure >=>" && data1 < data2 ) ||
         ( instr == "#<procedure <>" && data1 >= data2 ) ||
         ( instr == "#<procedure <=>" && data1 > data2 ) ||
         ( instr == "#<procedure =>" && data1 != data2 ) ) {
			temp->data = "nil" ;
			return temp ;
		} // if


	} // while

	temp->data = "#t" ;
	return temp ;

} // Judge_num()

Tree * Add_string( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;
	string total = "" ;
	while ( input_tree != NULL && Tokentype( input_tree->data ) != NIL ) {
		input_tree->left->data = input_tree->left->data.substr( 1, input_tree->left->data.size() - 2 ) ;
		total = total + input_tree->left->data ;
		input_tree = input_tree->right ;
	} // while

	total = "\"" + total + "\"" ;
	temp->data = total ;
	return temp ;
} // Add_string()

Tree * Judge_string( Tree * input_tree, string instr ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;

	string data1 ;
	string data2 ;

	data1 = input_tree->left->data.substr( 1, input_tree->left->data.size() - 2 ) ;
	input_tree = input_tree->right ;
	data2 = input_tree->left->data.substr( 1, input_tree->left->data.size() - 2 ) ;
	if ( ( instr == "#<procedure string>?>" && data1 <= data2 ) || 
       ( instr == "#<procedure string<?>" && data1 >= data2 ) || 
       ( instr == "#<procedure string=?>" && data1 != data2 ) ) {
		temp->data = "nil" ;
		return temp ;
	} // if

	while ( input_tree->right != NULL && Tokentype( input_tree->right->data ) != NIL ) {
		input_tree = input_tree->right ;
		data1 = data2 ;
		data2 = input_tree->left->data.substr( 1, input_tree->left->data.size() - 2 ) ;
		if ( ( instr == "#<procedure string>?>" && data1 <= data2 ) || 
         ( instr == "#<procedure string<?>" && data1 >= data2 ) || 
         ( instr == "#<procedure string=?>" && data1 != data2 ) ) {
			temp->data = "nil" ;
			return temp ;
		} // if

	} // while

	temp->data = "#t" ;
	return temp ;
} // Judge_string()

Tree * Set_eqv( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;

	Tree * run = input_tree->right ;

	if ( If_atom( input_tree )->data == "nil" || If_atom( run )->data == "nil" || 
       Tokentype( input_tree->left->data ) == STRING || Tokentype( run->left->data ) == STRING ) {
		temp->data = "nil" ;
		return temp ;
	} // if

	else if ( input_tree->left->data != "" && Tokentype( input_tree->left->data ) == FLOAT &&
            run->left->data != "" && Tokentype( run->left->data ) == FLOAT ) {
		double first_f = atof( input_tree->left->data.c_str() ) ;
		double compair_f = atof( run->left->data.c_str() ) ;
		first_f = first_f * 1000 ;
		compair_f = compair_f * 1000 ;

		int first_i = first_f ;
		int compair_i = compair_f ;
		if ( first_i != compair_i ) {
			temp->data = "nil" ;
			return temp ;
		} // if

		else if ( first_i == compair_i ) {
			temp->data = "#t" ;
			return temp ;
		} // else if

	} // else if

	else if ( ( Tokentype( input_tree->left->data ) == T && Tokentype( run->left->data ) == T )
            || ( Tokentype( input_tree->left->data ) == NIL && Tokentype( run->left->data ) == NIL ) ) {
		temp->data = "#t" ;
		return temp ;
	} // else if

	else if ( input_tree->left->data == run->left->data ) {
		temp->data = "#t" ;
		return temp ;
	} // else if

	temp->data = "nil" ;
	return temp ;
} // Set_eqv()

void Equal( Tree * first, Tree * compair, bool &false_bit ) {
	if ( false_bit == 1 ) return ;

	if ( ( first == NULL && compair != NULL && Tokentype( compair->data ) == NIL ) ) return ;
	else if ( ( compair == NULL && first != NULL && Tokentype( first->data ) == NIL ) ) return ;

	if ( ( first == NULL && compair != NULL ) || ( first != NULL && compair == NULL ) ) {
		false_bit = 1 ;
		return ;
	} // if

	else if ( first == NULL && compair == NULL ) {
		return ;
	} // else if

	else {
		if ( ( first->data == "" && compair->data != "" ) || ( first->data != "" && compair->data == "" ) ) {
			false_bit = 1 ;
			return ;
		} // if

		else if ( first->data == "" && compair->data == "" ) ;
		else if ( ( Tokentype( first->data ) == T && Tokentype( compair->data ) == T ) 
              || ( Tokentype( first->data ) == NIL && Tokentype( compair->data ) == NIL ) ) ; // 浮点数??
		else if ( Tokentype( first->data ) == FLOAT && Tokentype( compair->data ) == FLOAT ) {
			double first_f = atof( first->data.c_str() ) ;
			double compair_f = atof( compair->data.c_str() ) ;
			first_f = first_f * 1000 ;
			compair_f = compair_f * 1000 ;

			int first_i = first_f ;
			int compair_i = compair_f ;
			if ( first_i != compair_i ) {
				false_bit = 1 ;
				return ;
			} // if

		} // else if

		else if ( first->data != compair->data ) {
			false_bit = 1 ;
			return ;
		} // else if

		Equal( first->left, compair->left, false_bit ) ;
		if ( false_bit == 1 ) return ;
		Equal( first->right, compair->right, false_bit ) ;

	} // else

} // Equal()

Symbol_table Find_symbol( string name )
{
	Symbol_table empty_table ;
	empty_table.name = "-999999" ;
	vector<Symbol_table>::iterator pt = gsymbol_list.begin() ;
	while ( pt != gsymbol_list.end() ) {
		if ( ( *pt ).name == name ) return *pt ;
		pt = pt + 1 ;
	} // while

	return empty_table ;
} // Find_symbol()

Tree * Eval_not( Tree * input_tree ) {
	Tree * temp = new Tree ;
	temp->left = NULL ;
	temp->right = NULL ;

	if ( input_tree->left->data != "" && Tokentype( input_tree->left->data ) == NIL ) {
		temp->data = "#t" ;
		return temp ;
	} // if

	else {
		temp->data = "nil" ;
		return temp ;
	} // else

} // Eval_not()


int Get_param( Tree * input_tree ) {
	Tree * run = input_tree ;
	int param = 0 ;
	if ( run == NULL ) return param ;
	while ( run != NULL ) {
		if ( run->data == "" ) param = param + 1 ;
		run = run->right ;
	} // while

	return param ;

} // Get_param()

bool Judge_list( Tree * input ) {
	if ( input == NULL ) return true ;
	else if ( input->data != "" && Tokentype( input->data ) != NIL ) return false ;
	Tree * run = input ;
	while ( run->right != NULL ) {
		run = run->right ;
		if ( run->data != "" && Tokentype( run->data ) != NIL ) {
			return false ;
		} // if

	} // while

	return true ;
} // Judge_list()

void Set_non_list() {
	cout << "ERROR (non-list) : " ;
	int height = 0 ;
	Print_tree( g_op, height ) ;
	cout << "\n" ;
} // Set_non_list()

void Unbound_symbol( string symbol ) {
	cout << "ERROR (unbound symbol) : " << symbol << "\n\n" ;
} // Unbound_symbol()

void Non_function( Tree * err_data ) {
	cout << "ERROR (attempt to apply non-function) : " ;
	int height = 0 ;
	Print_tree( err_data, height ) ;
	cout << "\n" ;
} // Non_function()

bool Chece_level( int level, string instr ) {
	if ( level > 0 && instr == "#<procedure define>" ) {
		cout << "ERROR (level of DEFINE)\n\n" ;
		return false ;
	} // if

	else if ( level > 0 && instr == "#<procedure clean-environment>" ) {
		cout << "ERROR (level of CLEAN-ENVIRONMENT)\n\n" ;
		return false ;
	} // else if

	else if ( level > 0 && instr == "#<procedure exit>" ) {
		cout << "ERROR (level of EXIT)\n\n" ;
		return false ;
	} // else if

	else return true ;

} // Chece_level()

void Define_format() {
	cout << "ERROR (DEFINE format) : " ;
	int height = 0 ;
	Print_tree( g_op, height ) ;
	cout << "\n" ;
} // Define_format()

bool Find_undefine_symbol( string now_define ) {
	int i = 0 ;
	while ( i < gsymbol_list.size() ) {
		if ( i < 41 && gsymbol_list[i].name == now_define ) return false ;
		i = i + 1 ;
	} // while
	
	if ( Tokentype( now_define ) != SYMBOL ) return false ;
	return true ;
} // Find_undefine_symbol()

void Argu_count_error( string instr ) {
	cout << "ERROR (incorrect number of arguments) : " << instr << "\n\n" ;
} // Argu_count_error()

void If_no_return( Tree * err_data ) {
	cout << "ERROR (no return value) : " ;
	int height = 0 ;
	Print_tree( err_data, height ) ;
	cout << "\n" ;
} // If_no_return()

void Argu_type( string instr, Tree * data ) {
	cout << "ERROR (" << instr << " with incorrect argument type) : " ;
	int height = 0 ;
	Print_tree( data, height ) ;
	cout << "\n" ;
} // Argu_type()

void Divide_zero() {
	cout << "ERROR (division by zero) : /\n\n" ;
} // Divide_zero()

void Cond_format() {
	cout << "ERROR (COND format) : " ;
	int height = 0 ;
	Print_tree( g_op, height ) ;
	cout << "\n" ;
} // Cond_format()

void Lambda_format() {
	cout << "ERROR (lambda format)\n";
} // Lambda_format()




// 2019-6-7 2:20 






// 有0 的除法都不过
// eqv？（+ 1 1）
