#!/usr/bin/perl -w

use Carp;
use strict;
use warnings;
use Data::Dumper;
use Pascal::Scanner;
use Pascal::Parser;
use Pascal::Pvm;

&main();

sub main{

    my $pasfile = $ARGV[0];

    if( !defined $pasfile ){ 
        usage();
    }
    elsif(! -e $pasfile ){
        usage("ERROR: file $pasfile not found"); 
    }

    my $DEBUG = 1 if( (defined $ARGV[1]) && ($ARGV[1] eq '-dump') );

    # single pass tokenization
    my $scanner = Scanner->new($pasfile);
    print Dumper($scanner) if( $DEBUG );

    my $parser = Parser->new( $scanner ); 
    $parser->start_descent($scanner); 
    print Dumper($parser) if( $DEBUG );

    my $pascalvm = Pvm->new($parser);
    $pascalvm->run(); 
    
}

sub usage{
    my $estring = shift;

    print $estring if( defined $estring );
    print "\n\tUsage: ./jpcc <yourfile.pas>\n"; 
    exit;
}


=pod

=head1 SYNOPSYS

 Running this script will beging Lexical analysis, parsing, and execution
 of the virtual machine. 

=head1 AUTHOR

 Javier Fernandez <jfernan16@ccny.cuny.edu>
 CCNY-CSC-I0700 Compiler Construction
 Pascal Implementation
=cut
