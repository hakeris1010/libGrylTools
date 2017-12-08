#include <ostream>
#include <string>

namespace gtools{

namespace PrintTools{

    struct ListOutputParams{
		size_t tabLeaderSize = 4;
		size_t oneLineElements = 1;
		bool bracesInNewLines = true;
		bool spacesBetweenElements = true;
	};

	template <typename InputIterator, typename Callback>
	inline void outputInitializerList( std::ostream& output, 
			InputIterator first, InputIterator last, 
			Callback callback,
			const ListOutputParams& props = ListOutputParams() )
	{
		output<<"{ ";
		std::string leader(props.tabLeaderSize, ' ');

		if(props.bracesInNewLines)
			output << "\n" << leader;

		//size_t curpos = output.tellg();
		register bool firstElem = true;
		size_t lineElemCnt = 0;

		for( InputIterator it = first; it != last; it++ ){
			// Put commas
			if(!firstElem)
				if(props.spacesBetweenElements)
					output<<" , ";
				else
					output<<",";
			else
				firstElem = false;

			// Put newline if needed.
			if( lineElemCnt >= props.oneLineElements ){
				output << "\n" << leader;
				lineElemCnt = 0;
			} 

			// Print element inside callback.
			callback( output, *it, props );

			lineElemCnt++;
		}

		if(props.bracesInNewLines)
			output << "\n";
		output<<"}";
	}


}

}

