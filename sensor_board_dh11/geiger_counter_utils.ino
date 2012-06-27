void countInterruptFunc()
{
    count ++ ;
}

unsigned long get_geiger_count()
{
    cli();                // disable global interrupts
    unsigned long tmp = count ;
    count = 0;
    sei();
    return tmp ;
}


int count_to_sievert( long counts, long coefficient )
{
    long answer = ( counts * coefficient * 360 ) / (sampling_time/1000) ;
    answer /= 10000 ;
    return ( int )answer ;
}

char* to_string( int counter )
{
    static char buf[ 14 ] ;
    char* str = ItoA5( counter ) ;
    buf[ 0 ] = str[ 1 ] ;
    buf[ 1 ] = str[ 2 ] ;
    buf[ 2 ] = '.' ;
    buf[ 3 ] = str[ 3 ] ;
    buf[ 4 ] = str[ 4 ] ;
    strcpy( &buf[ 5 ], " uSv/h" ) ;
    return buf ;
}
char* ItoA5( unsigned int a )
{
	static char Buffer[ 6 ] ;
	static const unsigned int Div[] = { 10000U, 1000U, 100U, 10U, 1U } ;
	int i ;

	for( i = 0 ; i < 5 ; i ++ ) {
		Buffer[ i ] = ( a / Div[ i ] ) + '0' ;
		a %= Div[ i ] ;
	}
	Buffer[ i ] = 0 ;
	return( Buffer ) ;
}

