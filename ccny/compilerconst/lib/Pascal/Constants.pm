package Constants;
use strict;
use warnings;
use Hash::Util qw ( lock_hash unlock_hash );



# OPCodes for VM
use constant{
    OP_HALT       => 0,
    OP_PUSH       => 1,
    OP_POP	  => 2,
    OP_PRINTSTR   => 3,
    OP_PRINTSTRLN => 4,
};


our $SPACE = " ";
our $EOF   = "~";

our $TK_U_PROC  = "TK_U_PROC";
our $TK_UNKNOWN = "TK_UNKNOWN";

#literals
our $TK_LITRL_STR   = "TK_LITRL_STR";
our $TK_LITRL_INT   = "TK_LITRL_INT";
our $TK_LITRL_FLOT  = "TK_LITRL_FLOT";
our $TK_LITRL_BOOL  = "TK_LITRL_BOOL";
our $TK_LITRL_CHAR  = "TK_LITRL_CHAR";


our %KEYWORDS = ( 
 "integer"   => "TK_INT", 
 "real"      => "TK_REAL",
 "string"    => "TK_STR",
 "and"       => "TK_AND",   
 "array"     => "TK_ARRAY",
 "begin"     => "TK_BEGIN",
 "case"      => "TK_CASE",
 "const"     => "TK_CONST",
 "div"       => "TK_DIV",
 "do"        => "TK_DO",
 "downto"    => "TK_DOWNTO",
 "else"      => "TK_ELSE",
 "end"       => "TK_END",
 "file"      => "TK_FILE",
 "for"       => "TK_FOR",
 "function"  => "TK_FUNC",
 "gin"       => "TK_GIN",
 "goto"      => "TK_GOTO",
 "if"        => "TK_IF",
 "in"        => "TK_IN",
 "label"     => "TK_LABEL",
 "mod"       => "TK_MOD",
 "nil"       => "TK_NIL",
 "not"       => "TK_NOT",
 "of"        => "TK_OF",
 "or"        => "TK_OR",
 "pragma"    => "TK_PRAG",
 "procedure" => "TK_PROC",
 "program"   => "TK_PROG",
 "repeat"    => "TK_REPEAT",
 "set"       => "TK_SET",
 "switch"    => "TK_SWITCH",
 "to"        => "TK_TO",
 "then"      => "TK_THEN",
 "type"      => "TK_TYPE",
 "until"     => "TK_UNTIL",
 "var"       => "TK_VAR",
 "while"     => "TK_WHILE",
 "with"      => "TK_WITH",
 "write"     => "TK_WRITE",
 "writeln"   => "TK_WRITELN", 
 "and"       => "TK_AND_OP",       
 "mod"       => "TK_MOD_OP",       
);

lock_hash(%KEYWORDS);



our %OPERATORS = ( 
   "+"    => "TK_PLUS",             
   "-"    => "TK_MINUS",           
   "*"    => "TK_MULTIPLY",         
   "/"    => "TK_DIVIDE",            
   "="    => "TK_EQUAL",              
   ">"    => "TK_GREATER",            
   "<"    => "TK_LESS",               
   ">="   => "TK_GREATER_EQUAL", 
   "<="   => "TK_LESS_EQUAL",         
   "<>"   => "TK_NOT_EQUAL",    
);

lock_hash( %OPERATORS );

our %SPECIAL_CHARS = ( 
   "~"    => "TK_EOF",
   ":="   => "TK_ASSIGN",             
   "("    => "TK_LEFT_PAREN",        
   ")"    => "TK_RIGHT_PAREN",        
   "["    => "TK_LEFT_BRKT",          
   "]"    => "TK_RIGHT_BRKT",         
   ";"    => "TK_SEMICOL",            
   ":"    => "TK_COLON",              
   ","    => "TK_COMA",               
   "."    => "TK_PERIOD",            
   "\'"    => "TK_SQUOTE",             
   "\""   => "TK_DBQUOTES",     
   "#"    => "TK_POUND",        
);

lock_hash( %SPECIAL_CHARS );

our $ASCII_MIN = 32;
our $ASCII_MAX = 127;

=pod

=head1 SYNOPSYS
    Holds constants use by other modules

=head1 AUTHOR

Javier Fernandez <jfernan16@ccny.cuny.edu>
CCNY-CSC-I0700 Compiler Construction
Pascal Implementation

=cut



1;
__END__
