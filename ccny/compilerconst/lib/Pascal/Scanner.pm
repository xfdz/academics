package Scanner;
use warnings;
use strict;

use Carp;
use IO::File;
use Pascal::Constants;
use Data::Dumper;
use Hash::Util qw(lock_hash unlock_hash);
use fields qw( filename linecnt filestrings lextoks );

our $ERROR = "\nERROR: ";
our $TK_BEGCOMNT_1 = "\{";
our $TK_ENDCOMNT_1 = "\}";

# escape twice, double interpolation
our $TK_BEGCOMNT_2 = "\\(\\*";
our $TK_ENDCOMNT_2 = "\\*\\)";

sub new {
    my $self = shift;
    $self = fields::new($self) if( ! ref($self) );
    $self->_init(@_);
    return $self;
}

sub _init(){ 
    my $self = shift;
  
    $self->{filename} = shift;  
    $self->_read_file();     
    $self->_tokenize();
}


sub _read_file {
    my $self = shift;

    my $fh = IO::File->new( $self->{filename} ,"r") 
             or confess( $ERROR . $! ); 

    my $lines;  
    my $linenum =0;
    my $string_line;

    while(<$fh>){
    # read file, one line at a time
        ++$linenum;
     
        # all string lowered-cased 
        $string_line = lc($_);
        chomp($string_line); 
  
        my $tk_line ={ lineno => $linenum, tokens => $string_line }; 
        push( @$lines, $tk_line );
    }
    
    # end of file marker
    push(@$lines,{ lineno => ++$linenum ,tokens=> $Constants::EOF } );

    $self->{filestrings} = $lines; 
    $self->{linecnt} = $linenum;
}

sub _tokenize{
    my $self = shift;

    my $ETOK = $ERROR . "unrecognized token ";
    my @string_line = @{ $self->{filestrings} };

    foreach my $line ( @string_line ){
    # string line 
        my  @chars = split(//,$line->{tokens});
        next if(! defined $chars[0] );

        my @toks; 
        my $scanp = 0;
        my $end = $#chars +1;
        my $line = $line->{lineno};

        while(  $scanp < $end  ){
        # read till end of token

            if((!defined $chars[$scanp]) || ($chars[$scanp] =~ m/(\s)/)){
               ++$scanp; next;
            }
 
            my $type;
            my $word; 
            my $tok;
            my $lookahead_1;
            if( $chars[$scanp] =~ m/ (\d) /x ){
            # is digit
                $word .= $chars[$scanp]; 
                while((++$scanp < $end ) && ($chars[$scanp] =~ m/(\d)/x)){
                    $word .= $chars[$scanp];
                }

                if( $chars[$scanp] eq '.' ){
                # real
                    $word .= $chars[$scanp];
                    while((++$scanp < $end)&&($chars[$scanp]=~ m/(\d)/x)){ 
                        $word .= $chars[$scanp];
                    }
                    if( $chars[$scanp] =~ m/(\w)|(_)/x ){
                    # illegal charactsr after number
                        confess($ETOK. "line $line column $scanp \n"); 
                    }

                    $type = $Constants::TK_LITRL_FLOT;
                    $tok = { 
                        value  => $word, 
                        line   => $line, 
                        col    => $scanp,
                        length => +length($word),  
                        type   => $type
     
                     };
                     ++$scanp;
                     push( @toks, $tok ); next;
                }
                elsif( $chars[$scanp] =~ m/(\w)|(_)/x  ){
                # illegal charactsr after number
                    confess($ETOK. "line $line column $scanp \n"); 
                }
              
                $type = $Constants::TK_LITRL_INT;
                $tok = {
                    value  => $word, 
                    line   => $line, 
                    col    => $scanp,
                    length => +length($word),  
                    type   => $type
                };
               push( @toks, $tok ); 
               ++$scanp;
               next;
           }   
           elsif( $chars[$scanp] =~ m/ (\w) /x ){
           # is an alphabet char 

                $word = $chars[$scanp];
                while((++$scanp < $end) && 
                      ($chars[$scanp] =~ m/(\w)|(\d)|(\_)/x))
                {
                    $word .= $chars[$scanp];
                }

               my $type = $Constants::TK_UNKNOWN;
               if( exists $Constants::KEYWORDS{$word} ){
                   $type = $Constants::KEYWORDS{$word};
                   if(!$type ){
                       confess($ETOK . "line $line column $scanp\n" );
                   }
               }

               $tok = {
                    value  => $word,  
                    line   => $line,  
                    col    => $scanp,
                    length => +length( $word ),
                    type   => $type,
               }; 
               push( @toks, $tok ); 
               next;

           }   
           elsif( $chars[$scanp] =~ m/ (\W) /x ){
           # non alphanumeric
             
               next
               if(($chars[$scanp]=~m/(\s)/) || (!defined $chars[$scanp]));
               
               $word = $chars[$scanp];
               if( $word eq "\'" ){

                   $lookahead_1 = $chars[$scanp+1];
                   if( $lookahead_1 =~ m/(\w)|(\W)|(\d)|(\D)/x ){
                   # literal string
                       while((++$scanp<$end) && ($chars[$scanp] ne "\'" )){
                           #$word .= $chars[$scanp];
                           join("",$word,$chars[$scanp]);
                       }
                       
                       if( $chars[$scanp] ne "\'" ){
                           confess( $ETOK . "line $line column $scanp" 
                                        .  " missing single quote \n" );
                       }
                       #zap the single quotes
                       $word =~ s/\'//g;    
                       
                       my $len = length($word) - $scanp; 
                       $type='TK_SQUOTE';
                       $tok = {
                          value  => "'",
                          line   => $line,
                          col    => $len, 
                          length => +length( $word ),
                          type   => $type
                       }; 
                       push( @toks, $tok );

                       $len = length($word) - 1;
                       $type = $Constants::TK_LITRL_STR;
                       $tok = {
                           value  => $word,
                           line   => $line,
                           col    => $len,
                           length => +length( $word ),
                           type   => $type
                       };
                       push( @toks, $tok ); 

                       $type='TK_SQUOTE';
                       $tok = {
                           value  => "'",
                           line   => $line,
                           col    => $scanp,
                           length => +length( $word ),
                           type   => $type
                       };
                       push( @toks, $tok ); 

                       ++$scanp; 
                       next;
                   }
               }
               elsif( $word eq ':' ){
                   $lookahead_1 = $chars[$scanp+1];

                   if( $lookahead_1 eq '=' ){
                   # assignment operator
                       $word .= $chars[++$scanp];
                       $type = $Constants::SPECIAL_CHARS{$word}; 

                       if(!$type){
                           my $e = $ETOK. "line $line column $scanp"; 
                           confess( $e ." character [$word] \n" );
                       }

		       $tok = {
                           value  => $word,
                           line   => $line,
                           col    => $scanp,
                           length => +length( $word ),
                           type   => $type
                       };
                       push( @toks, $tok ); 
                       ++$scanp; 
                       next;
                   }
                   else{
                   # colon delimiter 
                       $type = $Constants::SPECIAL_CHARS{$word};
                       if(!$type){
                           confess($ETOK. "line $line column $scanp\n" );
                       }

                       $tok = {
                           value  => $word,
                           line   => $line,
                           col    => $scanp,
                           length => +length( $word ),
                           type   => $type
                       };

                       push( @toks, $tok ); 
                       ++$scanp; 
                       next;
                   }  
              }
              else{
              # some character delimiter

                  if( exists $Constants::SPECIAL_CHARS{$word} ){
                      $type = $Constants::SPECIAL_CHARS{$word};
                  }
                  elsif( exists $Constants::OPERATORS{$word} ){
                      $type = $Constants::OPERATORS{$word};

                      if( !$type ){
                          my $e = $ETOK. "line $line column $scanp"; 
                          confess($e ." character $word \n" );
                      }
                  }

                  $tok = { 
                      value  => $word,
                      line   => $line,
                      col    => $scanp,
                      length => +length( $word ),
                      type   => $type
                  };  
                  push( @toks, $tok ); 
                  ++$scanp;
                  next;
              }
          }
        }
        push ( @{$self->{lextoks}} , @toks  );
    }
}


sub ToString{
    my $self = shift;
    print "\nToString:\n";
    print "\nFile name: "    . $self->{filename};
    print "\nLine numbers: " . $self->{linecnt};
    print "\n\n **** NonTokenized Pascal file ****\n\n";
    print Dumper( $self->{filestrings});
    print "\n\n **** Tokenized Pascal ****\n\n";
    print Dumper( $self->{lextoks});
}


=pod

=head1 SYNOPSYS

    One pass scanner for pascal syntax

=head1 AUTHOR

Javier Fernandez <jfernan16@ccny.cuny.edu>

CCNY-CSC-I0700 Compiler Construction

Pascal Implementation

=cut


1;
__END__
