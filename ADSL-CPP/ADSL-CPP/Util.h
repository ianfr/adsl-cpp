#pragma once

#include <vector>

namespace adsl {

	template <class T>
	bool verifyDims(T& matRef) {
		if (matRef.size() == 0) {
			return false;
		}
		else {
			int theLen = matRef[0].size();
			for (auto col : matRef) {
				if (col.size() != theLen) {
					return false;
				}
			}
		}
		return true;
	}
}