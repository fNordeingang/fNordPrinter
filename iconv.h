#include <cstdlib>
#include <string>
#include <iconv.h>

char *convert(const char *from, const char *to, char *src) {

        iconv_t cd;
	cd = iconv_open(to, from);
	if((long)cd == -1)
		return (0);

	size_t len;
	size_t target_len;
	char *target;
	char *target_start;
	const char *src_start;
	int len_start;
	int target_len_start;

	len = strlen(src) + 1;
	if(!len)
		return (0);

	target_len = len+1;
	target = (char *)calloc(target_len, 1);
	len_start = len;
	target_len_start = target_len;
	target_start = target;
	src_start = src;

	size_t iconv_value;
	iconv_value = iconv(cd, &src, &len, &target, &target_len);
	if(iconv_value == (size_t)-1)
		return (0);
	return target_start;
}

char *convertUTF8ToCP437(std::string str) {
	const char *src = str.c_str();
	char *res = convert("UTF-8", "CP437", (char *)src);
	return res;
}
