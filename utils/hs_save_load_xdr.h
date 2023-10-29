#pragma once

/*! \file save_load_xdr.h

Utilities for saving and loading xdr objects to disk 
*/

#include <fcntl.h>
#include <sys/stat.h>

#include <xdrpp/marshal.h>

#include <utils/cleanup.h>

namespace utils {

/*! Load an xdr object from disk, dynamically allocating buffer for deserialization
  Can be somewhat slower than  load_xdr_from_file_fast(), but easier to use.
*/
template<typename xdr_type>
int __attribute__((warn_unused_result)) 
load_xdr_from_file(xdr_type& output, const char* filename)  {
	FILE* f = std::fopen(filename, "r");

	if (f == nullptr) {
		return -1;
	}

	std::vector<char> contents;
	const int BUF_SIZE = 65536;
	char buf[BUF_SIZE];

	int count = -1;
	while (count != 0) {
		count = std::fread(buf, sizeof(char), BUF_SIZE, f);
		if (count > 0) {
			contents.insert(contents.end(), buf, buf+count);
		}
	}

	xdr::xdr_from_opaque(contents, output);
	std::fclose(f);
	return 0;
}

/*! Save xdr object to disk.
    Slower than save_xdr_to_file_fast() because it 
    of serialization buffer allocation, but easier
    to use.
*/
template<typename xdr_type>
int __attribute__((warn_unused_result))
save_xdr_to_file(const xdr_type& value, const char* filename) {

	FILE* f = std::fopen(filename, "w");

	if (f == nullptr) {
		return -1;
	}

	auto buf = xdr::xdr_to_opaque(value);


	std::fwrite(buf.data(), sizeof(buf.data()[0]), buf.size(), f);
	std::fflush(f);

	fsync(fileno(f));

	std::fclose(f);
	return 0;
}

} /* utils */
