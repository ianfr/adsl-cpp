#include "DataFrame.h"
#include <sstream>

using namespace adsl;

// DataList

// Ostream
std::string DataList::str() {
	using namespace std;
	stringstream os;
	os << "[BEGIN DataList]" << "\n" << "Name: " << name << endl;
	os << "Data:" << endl;
	for (auto val : vals) {
		os << val << " ";
	}
	os << "\n" << "[END DataList]" << endl;;
	return os.str();
}




// DataFrame

// Ostream
std::string DataFrame::str() {
	using namespace std;
	stringstream os;
	os << "[[BEGIN DataFrame]]" << "\n" << "Description: " << m_description << endl;
	os << "Data:" << endl;
	for (auto list : m_data) {
		os << list.str();
	}
	os << "[[END DataFrame]]" << endl;;
	return os.str();
}


// Getters
int DataFrame::getRows() {
	return m_rows;
}

int DataFrame::getCols() {
	return m_columns;
}

std::string DataFrame::getDesc() {
	return m_description;
}

vDL DataFrame::getData() {
	return m_data;
}


// Setters
void DataFrame::setDesc(std::string s) {
	m_description = s;
}

void DataFrame::setData(vDL& matRef) {
	m_data = matRef;
	m_rows = matRef[0].vals.size();
	m_columns = matRef.size();
}

// Other
void DataFrame::addCol(DataList& refCol) {
	m_data.emplace_back(refCol);
}

//void DataFrame::addCol(vd vec&, std::string vecName = "") {
//	m_data.emplace_back(DataList(vec, vecName));
//}


















