#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "json.h"
#include <iconv.h>
#include "iconv.h"
#include <string>

const char data[]="track=piraten";

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    json_value * tweet = json_parse((char *)ptr);
    if(size*nmemb > 0 && tweet != 0) {
        const char *text = ((*tweet)["text"]);
		const char *screenName = ((*tweet)["screenName"]);
		const char *created_at = ((*tweet)["created_at"]);
	    char *converted = convertUTF8ToCP437(text);
		FILE *file; 
		file = fopen("out","a+"); /* apend file (add text to 
		a file or create a file if it does not exist.*/ 
		fprintf(file,"%s %s: %s\n\n", created_at, screenName, converted);
		fclose(file); /*done!*/ 
    }
	
    return size*nmemb;
}

int main(int argc, char **argv)
{
    CURL *curl;
    CURLcode res;
	std::string user = argv[1];
	std::string password = argv[2];
	std::string login = user + ":" + password;
	std::string endpoint = "https://"+login+"@stream.twitter.com/1/statuses/filter.json";

    curl = curl_easy_init();
    if(curl) {
        /* First set the URL that is about to receive our POST. */
        curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());

        /* Now specify we want to POST data */
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        /* get verbose debug output please */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return 0;
}
