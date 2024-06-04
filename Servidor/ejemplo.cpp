#include <iostream>
#include <chrono>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility::conversions;
using namespace Concurrency;

const string BASE_API_URL = "https://api.openweathermap.org/data/2.5";
const string API_KEY = "c218b7feba435cf266490fc57e2c7c12";

void main()
{
	string cityName;
	cout << " Enter the name of the city"<<endl;

	getline(cin, cityName);

	http_client httpClient = http_client(to_string_t(BASE_API_URL));
	
	http_request request =	http_request (methods::GET);	

	uri_builder uriBuilder = uri_builder(to_string_t("/weather"));

	uriBuilder.append_query(to_string_t("q"), to_string_t(cityName));
	uriBuilder.append_query(to_string_t("appid"), to_string_t(API_KEY));

	request.set_request_uri(uriBuilder.to_string());

	httpClient.request(request);

	task<http_response>requestTask = httpClient.request(request);
	bool isRunning = true;
	while (isRunning)
	{
		if (requestTask.is_done())
		{

			http_response response = requestTask.get();
			json::value json=response.extract_json().get();

			string city = to_utf8string (json[to_string_t("name")].as_string());
			string weather = to_utf8string (json[to_string_t("weather")][0][to_string_t("main")].as_string());
			float temperature = json[to_string_t("main")][to_string_t("temp")].as_double();
			float feelsLike = json[to_string_t("main")][to_string_t("feels_like")].as_double();
			float humidity = json[to_string_t("main")][to_string_t("humidity")].as_double();

			cout << "Weather in: " << city << endl;

			cout << " Weather: " << weather << endl;
			cout << " Temperature: " << temperature <<"K"<< endl;
			cout << " FeelsLike: " << feelsLike <<"K"<< endl;
			cout << " Humidity: " << humidity << "%"<<endl;

			isRunning = false;
		}
		else
		{
			cout << "Loading weather..." << endl;
		}
		this_thread::sleep_for(chrono::milliseconds(1000));

	}

	cin.get();
}