#include <ostream>
#include <string>

namespace gtools{

namespace PrintTools{

    struct ListOutputParams{
		size_t tabLeaderSize = 4;
		size_t oneLineElements = 1;
		bool bracesInNewLines = false;
        
        ListOutputParams(){}
        ListOutputParams( size_t tabsize, size_t lineElems = 1, bool bracesInNL = false )
            : tabLeaderSize( tabsize ), oneLineElements( lineElems ), 
              bracesInNewLines( bracesInNL ) {}
	};

	template <typename InputIterator, typename Callback>
	inline void outputInitializerList( std::ostream& output, 
			InputIterator first, InputIterator last, 
			Callback callback,
			const ListOutputParams& props = ListOutputParams() )
	{
		std::string leader(props.tabLeaderSize, ' ');

		if(props.bracesInNewLines){
			output<< "\n";
            output<< std::string((props.tabLeaderSize-4 >=0 ? props.tabLeaderSize-4 : 0), ' ' );
            output<< "{ ";
        }
        else
            output<<"{ ";

		//size_t curpos = output.tellg();
		register bool firstElem = true;
		size_t lineElemCnt = props.oneLineElements;

		for( InputIterator it = first; it != last; it++ ){
			// Put commas
			if(!firstElem)
                output<<", ";
			else
				firstElem = false;

			// Put newline if needed.
			lineElemCnt++;
			if( lineElemCnt >= props.oneLineElements ){
				output << "\n" << leader;
				lineElemCnt = 0;
			} 

			// Print element inside callback.
			callback( output, *it, props );
		}

		if(lineElemCnt != props.oneLineElements){
            output<<"\n";
            output<< std::string((props.tabLeaderSize-4 >=0 ? props.tabLeaderSize-4 : 0), ' ' );
        }
        output<< "}"; 
	}

}

}

