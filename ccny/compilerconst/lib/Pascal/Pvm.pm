package Pvm;

use Carp;
use strict;
use warnings;
use Data::Dumper;
use Pascal::Constants;
use fields qw( codelist datalist);

sub new {
    my $self = shift;

    $self = fields::new($self) if( !ref($self) );
    $self->_init(@_);
    return $self;
}


sub _init{
    my $self = shift;
    my $parser = shift;

    if( !$parser->isa("Parser") ){
        confess("\nERROR: expected type Pascal::Parser\n"); 
    }
    
    $self->{codelist} = $parser->{emcode}->{code};
    $self->{datalist} = $parser->{emcode}->{data};
}

sub run {
    my $self = shift;

    foreach my $op( @{$self->{codelist}} ){

        if( $op == Constants::OP_HALT ){
            exit();
        }
        elsif( $op == Constants::OP_PRINTSTR ){
            my $string = shift( @{$self->{datalist}} ); 
            print $string;
        }
        elsif( $op == Constants::OP_PRINTSTRLN ){
            my $string = shift( @{$self->{datalist}} );  
            print $string . "\n";
        }
        elsif( $op == Constants::OP_PUSH ){
            carp("ERROR: IMPLEMENT ME!");
        }
        elsif( $op == Constants::OP_POP ){
            carp("ERROR: IMPLEMENT ME!");
        }
        else{
            carp("ERROR: unable to resolve OP instruction");
        }
    }
}

=pod

=head1 SYNOPSYS

 Pascal Virtual machine that runs instructions given by parser

=head1 AUTHOR

Javier Fernandez <jfernan16@ccny.cuny.edu>

CCNY-CSC-I0700 Compiler Construction

Pascal Implementation

=cut

1;
__END__
