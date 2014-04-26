package Parser;

use Carp;
use strict;
use warnings;
use Data::Dumper;
use Pascal::Constants;
use Hash::Util qw( lock_keys unlock_keys );
use fields qw( filename line col curtok tokenlist estr symtable emcode );

sub new {
    my $self = shift;

    $self = fields::new($self) if( !ref($self) );
    $self->_init(@_);
    return $self;
}

sub _init{
    my $self = shift;
    my $scanner = shift;

    if( !$scanner->isa("Scanner") ){
        confess("\nERROR: expected type Pascal::Scanner \n");
    }

    $self->{filename}  = $scanner->{filename};
    $self->{tokenlist} = $scanner->{lextoks}; 
    # put first token in curtoken
    $self->{curtok}   = shift( @{$self->{tokenlist}} );
    $self->{emcode}   = {code =>[], data=>[]}; 
}



sub start_descent{
    my $self = shift;

    $self->program_header(); 
    $self->declarations();

    if( $self->{curtok}->{type} ne 'TK_BEGIN'){
        $self->_get_errorstring(); 
        confess( $self->{estr} . "missing keyword BEGIN \n");
    }

    $self->pL();    

    $self->get_next_token();
    if( $self->{curtok}->{type} ne 'TK_EOF'){
        confess("\nERROR: missing internal EOF\n");
    }

    #insert halt instruction
    push( @{$self->{emcode}->{code}}, Constants::OP_HALT );   

}

sub program_header{
    my $self = shift;

    if( $self->{curtok}->{type} ne 'TK_PROG'){
        confess( $self->{estr} . "missing identifier program \n" );
    }

    $self->get_next_token();
    if($self->{curtok}->{type} ne 'TK_UNKNOWN'){
        confess($self->{estr} . "missing user defined name\n");
    }

    $self->get_next_token();
    if( $self->{curtok}->{type} ne 'TK_SEMICOL'){
        confess($self->{estr} . "missing semicolon\n");
    }
}

sub declarations{
    my $self = shift;

    my $user_decls = { integer=>[], real=>[], string=>[] };
    lock_keys( %$user_decls );

    while(1){
    # iterate over list of variable declarations
        my @decls; 
        $self->get_next_token();
        last if( $self->{curtok}->{type} ne 'TK_VAR'); 

        $self->get_next_token();
        while( $self->{curtok}->{type} eq 'TK_UNKNOWN' ){
        # iterate over variable names
            push(@decls, $self->{curtok});         
            $self->get_next_token();
            # multiple variable declaration
            $self->get_next_token()
                if( $self->{curtok}->{type} eq 'TK_COMA');
        } 
 
        if( $self->{curtok}->{type} ne 'TK_COLON' ){
            confess($self->{estr} . "variable declaration missing colon\n");
        } 
        
        $self->get_next_token();
        my $t = $self->{curtok}->{type};
        if( ($t eq 'TK_INT') || ( $t eq 'TK_REAL') || ($t eq 'TK_STR') ){
            $t = $self->{curtok}->{value};
            # temporary saving variable names   
            push( @{$user_decls->{$t}}, @decls ); 
        }
        else{
            confess($self->{estr} . "undefined type: [ $t ]\n");
       }

        $self->get_next_token();
        if( $self->{curtok}->{type} ne 'TK_SEMICOL' ){
            confess($self->{estr}. "variable declaration undefined type\n");
        }
    }

    foreach my $type (keys %$user_decls ){
    # determine if there are any variable sharing same names 
        my %seen;

        # increment each time variable name seen
        map( ++$seen{$_->{value}}, @{$user_decls->{$type}} );

        while( my($key, $value) = each %seen){
        # variable name seen, no more than onece per type
            confess("error: redeclaration of variable $key\n")if($value != 1);
        }
    }

}

sub pL{
    my $self = shift;
    
    $self->pE();
    $self->p_L();

    $self->get_next_token();
    if( $self->{curtok}->{type} ne 'TK_PERIOD'){
        confess($self->{estr} . "missing period\n");
    }
}

sub p_L{
    my $self = shift;

    $self->get_next_token();
    return if( $self->{curtok}->{type} eq 'TK_END');

    $self->pE();
    $self->p_L();
}

sub pE{
    my $self = shift;
    $self->pT();
    $self->p_E();
}

sub p_E{
    my $self = shift;
  
    if( $self->{curtok}->{type} eq 'TK_PLUS'){
        $self->get_next_token();
        $self->pT(); 
        print "\n ADD \n"; 
        $self->p_E();
    }
    elsif( $self->{curtok}->{type} eq 'TK_MINUS' ){

        $self->get_next_token();
        $self->pT(); 
        print "\n SUBTRACT\n ";
        $self->p_E();

    }
}

sub pT{
    my $self = shift;
    $self->pF();
    $self->p_T();
}

sub p_T{
    my $self = shift;

    if( $self->{curtok}->{type} eq 'TK_MULTIPLY' ){
        print "\n MULTIPLY ";
    }
    elsif( $self->{curtok}->{type} eq 'TK_DIVIDE' ){
        print "\n SUBTRACT\n ";
    }
    
}

sub pF{
    my $self = shift;


    if( $self->{curtok}->{type} eq $Constants::TK_LITRL_INT ){
        print "\n PUSH ";
        $self->get_next_token();
    }
    elsif( $self->{curtok}->{type} eq $Constants::TK_LITRL_FLOT ){
        print "\n PUSH ";
        $self->get_next_token();
    }
    elsif( $self->{curtok}->{type} eq $Constants::TK_LITRL_STR ){ 
        $self->get_next_token();
    }
    elsif( $self->{curtok}->{type} eq 'TK_LEFT_PAREN'){

         $self->pL();

         $self->get_token();
         if( $self->{curtok}->{type} ne 'TK_RIGHT_PAREN'){ 
             confess($self->{estr}."missing closing parenthesis\n");
         }
    } 
    elsif( $self->{curtok}->{type} eq 'TK_WRITE'){
       
         $self->get_next_token();
         if( $self->{curtok}->{type} ne 'TK_LEFT_PAREN'){
             confess($self->{estr}."missing opening parenthesis\n");
         }

         $self->get_next_token();
         if( $self->{curtok}->{type} eq 'TK_DBQUOTES'){
         # process string literals

             my @string;
             $self->get_next_token();
             my $toktype = $self->{curtok}->{type}; 
             my $tokvalue = $self->{curtok}->{value}; 

             while( $toktype eq 'TK_UNKNOWN' || ($toktype ne 'TK_DBQUOTES')
                    &&  ( ($Constants::ASCII_MIN <= ord($tokvalue))
                    &&    (ord($tokvalue) <= $Constants::ASCII_MAX) )
             ){
                 push(@string, ( $tokvalue, " " ) );

                 $self->get_next_token();
                 $toktype  = $self->{curtok}->{type}; 
                 $tokvalue = $self->{curtok}->{value}; 
             }
            
             push(@{$self->{emcode}->{data}}, join('', @string));   
             push(@{$self->{emcode}->{code}}, Constants::OP_PRINTSTR );

             if($self->{curtok}->{type} ne 'TK_DBQUOTES'){
                 confess($self->{estr}."missing closing double quotes\n");
             }
         }
         elsif( $self->{curtok}->{type} eq 'TK_UNKNOWN' ){
         # process user defined variables 
             confess( "\nERROR: IMPLEMENT ME!\n"); 
         }
         else {
             confess($self->{estr} . "\n");
         }

         $self->get_next_token();
         if( $self->{curtok}->{type} ne 'TK_RIGHT_PAREN'){
             confess($self->{estr}."missing closing parenthesis\n");
         }
         
         $self->get_next_token();
         if( $self->{curtok}->{type} ne 'TK_SEMICOL' ){
             confess( $self->{estr} . "semicolon terminator missing\n" );
         }
    }
    elsif( $self->{curtok}->{type} eq 'TK_WRITELN')
    {
        $self->get_next_token();
        if( $self->{curtok}->{type} ne 'TK_LEFT_PAREN'){
            confess($self->{estr}."missing opening parenthesis\n");
        }   

        $self->get_next_token();
        if( $self->{curtok}->{type} eq 'TK_DBQUOTES'){
        # process string literals

            my @string;
            $self->get_next_token();
            my $toktype = $self->{curtok}->{type};
            my $tokvalue= $self->{curtok}->{value}; 

            while( $toktype eq 'TK_UNKNOWN' || ($toktype ne 'TK_DBQUOTES')
                 &&  ( ($Constants::ASCII_MIN <= ord($tokvalue))  
                 &&    (ord($tokvalue) <= $Constants::ASCII_MAX) ) 
            ){
                push(@string, ( $tokvalue, ' ' ) );
                $self->get_next_token();
                $toktype =  $self->{curtok}->{type};
                $tokvalue= $self->{curtok}->{value}; 
            }

            push( @{$self->{emcode}->{data}}, join('', @string) );   
            push( @{$self->{emcode}->{code}}, Constants::OP_PRINTSTRLN );

            if($self->{curtok}->{type} ne 'TK_DBQUOTES'){
                confess($self->{estr}."missing closing double quotes\n");
            }   
        }   
        elsif( $self->{curtok}->{type} eq 'TK_UNKNOWN' ){
        # process user defined variables 
            confess( "\nERROR: IMPLEMENT ME!\n"); 
        }   
        else {
            confess($self->{estr} . "\n");
        }   

        $self->get_next_token();
        if( $self->{curtok}->{type} ne 'TK_RIGHT_PAREN'){
            confess($self->{estr}."missing closing parenthesis\n");
        }   
   
        $self->get_next_token();
        if( $self->{curtok}->{type} ne 'TK_SEMICOL' ){
            confess( $self->{estr} . "semicolon terminator missing\n" );
        }   
    }
}

sub match {
    my $self = shift;
    my $exptok = shift;

    if( $self->{curtok}->{value} != $exptok){  
      confess( $self->{estr} . "expected token $exptok found\n" );  
    }
}

sub get_next_token {
    my $self = shift;
    $self->{curtok} = shift( @{$self->{tokenlist}} );
    $self->_get_errorstring();
}

sub _get_errorstring{
    my $self = shift;
    my $file = $self->{filename};
    my $line = $self->{curtok}->{line};
    my $col = $self->{curtok}->{col};
    $self->{estr} = 
     "\nsyntax error in file: $file at line: $line near column: $col ";
}


=pod

=head1 SYNOPSYS
 
 Parser for pascal implementation, uses recursive descent to parse

=head1 AUTHOR

Javier Fernandez <jfernan16@ccny.cuny.edu>

CCNY-CSC-I0700 Compiler Construction

Pascal Implementation

=cut


1;
__END__
