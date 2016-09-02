#include <iostream>
#include <curl/curl.h>

using namespace std;
int main() {
    CURL * curl = curl_easy_init();
    if(!curl){
        cout << "error"<<endl;
    }
    curl_easy_setopt(curl,CURLOPT_URL,"https://www.baidu.com");
    curl_easy_setopt(curl,CURLOPT_TIMEOUT,5L);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return 0;
}