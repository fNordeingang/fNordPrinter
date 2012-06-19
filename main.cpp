#include <cstdio>
#include <string.h>
#include <curl/curl.h>
#include "json.h"
#include <iconv.h>
#include "iconv.h"
#include <getopt.h>
#include <string>
#include <boost/program_options.hpp>
#include <vector>

std::string trackPhrase = "track=";
std::string format = "%s %s:\n%s\n\n";
std::string searchText;
std::string filename;
std::string encoding;
std::string username;
std::string password;
bool verbose = false;
int clear = 20;
int currentTweet = 0;

namespace po = boost::program_options;

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
	json_value * tweet = json_parse((char *)ptr);
	if(size*nmemb > 0 && tweet != 0) {
		const char *text = ((*tweet)["text"]);
		const char *screenName = ((*tweet)["user"])["screen_name"];
		const char *created_at = ((*tweet)["created_at"]);
		char *converted;
		if(encoding != "UTF-8") {
			converted = convertUTF8ToCP437(text);
		}
		else {
			converted = (char *)text;
		}
		if(filename != "stdout") {
			FILE *file; 
			file = fopen(filename.c_str(), "a+"); 
			fprintf(file,format.c_str(), created_at, screenName, converted);
			fclose(file);	
		}
		else {
			currentTweet++;
			printf(format.c_str(), created_at, screenName, converted);
			if(currentTweet >= clear) {
				system("clear");
				currentTweet = 0;
			}
		}
	}

	return size*nmemb;
}

int main(int argc, char **argv)
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("file,f", po::value<std::string>(&filename)->default_value("stdout"), "set filename")
		("encoding,e", po::value<std::string>(&encoding)->default_value("UTF-8"), "output encodig")
		("username,u", po::value<std::string>(&username)->required()->composing(), "twitter username")
		("password,p", po::value<std::string>(&password)->required()->composing(), "twitter password")
		("track,t", po::value<std::string>(&searchText)->required()->composing(), "search text")
		("verbose,v", "verbose output")
		;
	
	po::variables_map vm;
	po::positional_options_description pd;
	pd.add("username", -1);	

	try {		
		po::store(po::command_line_parser(argc, argv).options(desc).positional(pd).run(), vm);    

		if (vm.count("help")) {
			std::cout << desc << std::endl;
			return false;
		}

		po::notify(vm);
		
		verbose = vm.count("verbose");

		CURL *curl;
		CURLcode res;

		std::string login = username + ":" + password;
		std::string endpoint = "https://"+login+"@stream.twitter.com/1/statuses/filter.json";
		std::string postData = trackPhrase+searchText;

		curl = curl_easy_init();
		if(curl) {
			/* First set the URL that is about to receive our POST. */
			curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());

			/* Now specify we want to POST data */
			curl_easy_setopt(curl, CURLOPT_POST, 1L);

			/* we want to use our own read function */
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

			if(verbose) {
				/* get verbose debug output please */
				curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			}

			/* Perform the request, res will get the return code */
			res = curl_easy_perform(curl);

			/* always cleanup */
			curl_easy_cleanup(curl);
		}
	}
	catch(std::exception& e)
	{
		std::cerr << "Error: " << e.what() << "\n";
		std::cout << desc << std::endl;
		return false;
	}
	catch(...)
	{
		std::cerr << "Unknown error!" << "\n";
		return false;
	}	

	return 0;
}
