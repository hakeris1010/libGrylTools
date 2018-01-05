#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <sstream>
#include <string>
#include <gryltools/stackreader.hpp>

const size_t SAMPLE_SIZE = 50001;
const size_t ITERATIONS = 10000;
const size_t BUFFSIZE = 2048;

const bool PRINT_SAMPLE = false;

const int CALLBACK_EVERY_ITERATION = 1;
const int CALLBACK_ONLY_AT_THE_END = 2;

template<typename... Args>
void functionExecTimeReturnCallback( auto&& func, size_t times, auto&& callback, 
                                     int callbackFlags, Args&&... functionArgs ){
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    if(callbackFlags & CALLBACK_EVERY_ITERATION){
        for(size_t i = 0; i < times; i++ ){
            callback( func( std::forward<Args>(functionArgs)... ) );
        }
    }
    else if(callbackFlags & CALLBACK_ONLY_AT_THE_END){
        for(size_t i = 0; i < times - 1; i++ ){
            func( std::forward<Args>(functionArgs)... );
        }
        callback( func( std::forward<Args>(functionArgs)... ) );
    }
    else{
        for(size_t i = 0; i < times; i++ ){
            func( std::forward<Args>(functionArgs)... );
        } 
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "f() took " << time_span.count() << " ms\n";
}

template<typename... Args>
void functionExecTimeRepeated( auto&& func, size_t times, Args&&... args ){
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    for(size_t i = 0; i < times; i++ ){
        func( std::forward<Args>(args)... );
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "f() took " << time_span.count() << " ms\n";
}

template<typename... Args>
void functionExecTime( auto&& func, Args&&... args ){
    functionExecTimeRepeated( func, 0, args... );
}

struct StreamStats{
    size_t lineCount = 0;
    size_t posInLine = 0;

    StreamStats(){}
    StreamStats( size_t lnc, size_t posc )
        : lineCount( lnc ), posInLine( posc ) 
    {}

    void print( std::ostream& out ) const {
        out<<"StreamStats:\n Lines: "<< lineCount <<"\n posInLine: "<< posInLine<<"\n";
    }
};

inline std::ostream& operator<<( std::ostream& os, const StreamStats& sts ){
    sts.print( os );
    return os;
}

StreamStats readCharByChar(std::istream& is){
    is.clear();
    is.seekg(0, is.beg);

	size_t posInLine = 0;
	size_t lineCount = 0;

	std::string buff;
	buff.reserve(256);

    //std::cout<<"Reading CHar by Char\n";
	while( 1 ){
		char c = is.get();

        if( is.eof() )
            break;

		if( c == '\n' ){
            //std::cout<<"["<< is.tellg() <<"]Found Endl!\n";
			posInLine = 0;
			lineCount++;
		}
        else
            posInLine++;

        buff.push_back( c );
	}

    return StreamStats( lineCount, posInLine );
}

StreamStats readBuffered(std::istream& is, size_t buffSize){
    is.clear();
    is.seekg(0, is.beg);

    size_t posInLine = 0;
	size_t lineCount = 0;

	std::string buff( buffSize, '\0' );

    while( !is.eof() ){
        is.read( &buff[0], buff.size() );
        size_t readct = is.gcount();

        for( size_t i = 0; i < readct; i++ ){
            if( buff[i] == '\n' ){
                posInLine = 0;
                lineCount++;
            }
            else
                posInLine++;
        }
    }

    return StreamStats( lineCount, posInLine );
}

StreamStats readUsingStackReaderCBC(std::istream& is){
    is.clear();
    is.seekg(0, is.beg);

    size_t posInLine = 0;
	size_t lineCount = 0;

    gtools::StackReader reader( is, 1024, 2048 );

    char c;
    while( reader.getChar(c, gtools::StackReader::SKIPMODE_SKIPWS, lineCount, posInLine) ){
        if( c == '\n' ){
            posInLine = 0;
            lineCount++;
        }
        else
            posInLine++;
    }

    return StreamStats( lineCount, posInLine );
}

StreamStats readUsingStackReaderBUFF(std::istream& is, size_t buffSize){
    is.clear();
    is.seekg(0, is.beg);

    size_t posInLine = 0;
	size_t lineCount = 0;

    gtools::StackReader reader( is, buffSize, buffSize );

	std::string buff( buffSize, '\0' );

    while( reader.isReadable() ){
        size_t readct = reader.getString( &buff[0], buff.size(), 
            gtools::StackReader::SKIPMODE_SKIPWS, lineCount, posInLine );

        for( size_t i = 0; i < readct; i++ ){
            if( buff[i] == '\n' ){
                posInLine = 0;
                lineCount++;
            }
            else
                posInLine++;
        } 
    }

    return StreamStats( lineCount, posInLine );
}


void generateSample( std::string& str, size_t sampleSize, size_t maxLineSize = 80 ){
    size_t nextLinePos = 0 + (rand() % maxLineSize);

    for( size_t i = 0; i < sampleSize; i++ ){
        char cc;
        if( i >= nextLinePos ){
            nextLinePos = i + (rand() % maxLineSize);
            cc = '\n';

            //std::cout<<" NextLinePos: "<< nextLinePos <<" (i + "<< nextLinePos - i <<")\n";
        }
        else
            cc = char(32) + rand() % 90;

        str.push_back( cc );
    }
}

int main(){
    srand( time(0) );

    std::string sample;
    sample.reserve( SAMPLE_SIZE );

    generateSample( sample, SAMPLE_SIZE );
    if( PRINT_SAMPLE)
        std::cout << sample <<"\n\n\n";

    std::istringstream sstr( sample, std::ios_base::in | std::ios_base::binary );

    std::cout<< "CharByChar:\n";
    functionExecTimeReturnCallback( readCharByChar, ITERATIONS, [](StreamStats&& st){
        std::cout<< st <<"\n"; }, CALLBACK_ONLY_AT_THE_END, sstr );
 
    std::cout<< "\n\nbuffXtimes:\n";
    functionExecTimeReturnCallback( readBuffered, ITERATIONS, [](StreamStats&& st){
        std::cout<< st <<"\n"; }, CALLBACK_ONLY_AT_THE_END, sstr, BUFFSIZE ); 

    std::cout<< "\n\nstackReaderCBC:\n";
    functionExecTimeReturnCallback( readUsingStackReaderCBC, ITERATIONS, [](StreamStats&& st){
        std::cout<< st <<"\n"; }, CALLBACK_ONLY_AT_THE_END, sstr ); 

    std::cout<< "\n\nstackReaderBUFF:\n";
    functionExecTimeReturnCallback( readUsingStackReaderBUFF, ITERATIONS, [](StreamStats&& st){
        std::cout<< st <<"\n"; }, CALLBACK_ONLY_AT_THE_END, sstr, BUFFSIZE ); 
    
    return 0;
}

