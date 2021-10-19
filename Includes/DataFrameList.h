#pragma once

#include "DataFrame.h"
#include "Util.h"

namespace adsl {

    // Class for working with a group of DataFrames
	// If you need direct access to 'frames' see:
		// https://stackoverflow.com/questions/21646999/how-to-make-the-lambda-a-friend-of-a-class
	class DataFrameList {
	private:	
		std::vector<DataFrame> frames;

	public:
		// Getters
		int size() { return frames.size(); }
		DataFrame getFrame(int frameIndex);
		DataFrame getFrame(std::string frameDesc);

        // Setters
        void setFrames(std::vector<DataFrame> dfVec) {frames = dfVec;}

		// Other
		void addFrame(DataFrame &df);
		void loadFramesFromDir(std::string path, 
			std::string exclude, std::string delim, int dateFormat);
        std::vector<DataFrame> filter(std::function<bool(DataFrame)> predicate);
		std::vector<DataFrame> select(std::vector<std::string> names);

		// Operator overloads

		// DataFrameList <- DataFrameList
		DataFrameList operator+ (std::function<DataFrameList(DataFrameList&)> f) {
			return f(*this);
		}

	};

	DataFrame DataFrameList::getFrame(int frameIndex) {
		return frames.at(frameIndex);
	}

	DataFrame DataFrameList::getFrame(std::string frameDesc) {
		for(auto df : frames) {
			if(frameDesc.compare(df.getDesc()) == 0) {
				return df;
			}
		}
		std::cout << "getFrame() returning empty DF" << std::endl;
		DataFrame empty;
		return empty;
	}

	void DataFrameList::addFrame(DataFrame &df) {
		frames.push_back(df);
	}

	void DataFrameList::loadFramesFromDir(std::string path, 
			std::string exclude, std::string delim, int dateFormat)
	{
		// Get number of files in directory
		auto dirIter = std::filesystem::directory_iterator(path);
		int fileCount = std::count_if(
			begin(dirIter),
			end(dirIter),
			[](auto& entry) { return entry.is_regular_file(); }
		);
		std::cout << "Number of files to load in: " << fileCount << std::endl;

		// Load files into DFs one by one
		// Strip out a given suffix (like .us.txt, .csv, etc)
		dirIter = std::filesystem::directory_iterator(path);
		for (auto const &entry : dirIter) {
			if (entry.is_regular_file()) {
				std::cout << "Reading: " << entry << std::endl;
				std::stringstream stream;
				stream << entry;
				std::string nameStr = stream.str();

				// strip quotes, backslashes, the path, and the suffix (i.e. ".us.txt" or ".csv")
				nameStr = std::regex_replace( nameStr, std::regex("\""), "");
				nameStr = std::regex_replace( nameStr, std::regex(path), "");
				nameStr = std::regex_replace( nameStr, std::regex("/"), "");
				nameStr = std::regex_replace( nameStr, std::regex(exclude), "");

				// Read in DataFrame
				DataFrame df;
                df = loadFromCSV_wDate(std::regex_replace( 
                    stream.str(), std::regex("\""), ""), delim, true, 0, dateFormat);
                df.setDesc(nameStr);

                frames.push_back(df);
			}
		}
	}

    std::vector<DataFrame> DataFrameList::filter(
        std::function<bool(DataFrame)> predicate) 
    {
        std::vector<DataFrame> ret;
        for(auto df : frames) {
            if (predicate(df)) {
                ret.push_back(df);
            }
        }
        return ret;
    }

	std::vector<DataFrame> DataFrameList::select(std::vector<std::string> names) {
		std::vector<DataFrame> ret;
		for (auto df : frames) {
			if (std::find(names.begin(), names.end(), df.getDesc()) != names.end())
				ret.push_back(df);
		}
		return ret;
	}

}