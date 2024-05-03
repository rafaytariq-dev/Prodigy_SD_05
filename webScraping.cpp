#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>

// Function to write HTTP response to string
size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *data) {
    size_t totalSize = size * nmemb;
    data->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    CURL *curl;
    CURLcode res;
    std::string url = "https://www.daraz.pk/buy-fresh-produce/?spm=a2a0e.8553159.cate_1.1.7458332dBZpoLJ";
    std::string htmlContent;

    curl = curl_easy_init();
    if (curl) {
        // Set URL and callback function
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlContent);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "Failed to download: " << curl_easy_strerror(res) << std::endl;
            return 1;
        }

        // Cleanup
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return 1;
    }

    // Basic HTML parsing (not recommended for real-world use)
    size_t pos = 0;
    while ((pos = htmlContent.find("<div class=\"product-listing\"", pos)) != std::string::npos) {
        // Find title
        size_t titleStart = htmlContent.find("<h3 class=\"product-title\"", pos);
        size_t titleEnd = htmlContent.find("</h3>", titleStart);
        std::string title = htmlContent.substr(titleStart + 27, titleEnd - titleStart - 27);

        // Find price
        size_t priceStart = htmlContent.find("<span class=\"product-price\"", titleEnd);
        size_t priceEnd = htmlContent.find("</span>", priceStart);
        std::string price = htmlContent.substr(priceStart + 29, priceEnd - priceStart - 29);

        // Find rating
        size_t ratingStart = htmlContent.find("<span class=\"product-rating\"", priceEnd);
        size_t ratingEnd = htmlContent.find("</span>", ratingStart);
        std::string rating = htmlContent.substr(ratingStart + 29, ratingEnd - ratingStart - 29);

        // Move position for next iteration
        pos = ratingEnd;

        // Output the extracted data
        std::cout << "Title: " << title << std::endl;
        std::cout << "Price: " << price << std::endl;
        std::cout << "Rating: " << rating << std::endl;
        std::cout << std::endl;
    }

    return 0;
}