#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <float.h>
#include <cmath>
#include <numeric>

/* g++ -o output.exe main.cpp
 * ./output.exe
 */




/* Create a structure of our data */
struct Data {
public:
	Data(
		std::string date,
		double open,
		double high,
		double close,
		double adjclose,
		int volume
	) {
		Date = date;
		Open = open;
		High = high;
		Close = close;
		AdjClose = adjclose;
		Volume = volume;
	}

	void display() {
		std::cout << "Close: " << Close << std::endl;
	
	}

	std::string Date;
	double Open;
	double High;
	double Close;
	double AdjClose;
	int Volume;
};


/*
 * Function to display all the data
 */
void display_data(std::vector<Data> my_data) {
	for (auto data : my_data) {
		data.display();
	}
}


/*
 * Function to read the data from a csv file, and create a vector of type "Data"
 */

std::vector<Data> create_vector(std::string path) {
	std::fstream inputFile;
	inputFile.open(path);


	std::string line = "";
	std::getline(inputFile, line);
	line = "";
 

	std::vector<Data> my_data;


	while (std::getline(inputFile, line)) {
		std::string Date;
		double Open;
		double High;
		double Close;
		double AdjClose;
		int Volume;

		std::string tempString = "";

		std::stringstream inputString(line);
		
		std::getline(inputString, Date, ',');

		std::getline(inputString, tempString, ',');
		Open = std::atof(tempString.c_str());
		tempString = "";

		std::getline(inputString, tempString, ',');
		High = std::atof(tempString.c_str());
		tempString = "";

		std::getline(inputString, tempString, ',');
		Close = std::atof(tempString.c_str());
		tempString = "";

		std::getline(inputString, tempString, ',');
 		AdjClose = std::atof(tempString.c_str());
		tempString = "";

		std::getline(inputString, tempString, ',');
		Volume = std::atoi(tempString.c_str());
		

		Data current_data(Date, Open, High, Close, AdjClose, Volume);

		my_data.push_back(current_data);
	
		line = "";
	} 
	return my_data;	
}

std::vector<double> closing_price(std::vector<Data> my_data) {
	std::vector<double> closing(my_data.size());
	for (auto data : my_data) {
		closing.push_back(data.Close);
	}
	return closing;

}

double stdev(std::vector<double> data) {
	double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();

	double result = 0;

	for (auto point2 : data) {
		result  = result + pow(point2-mean, 2);	
		
	}
	std::cout << mean << std::endl;

	return 0;
}



int hurst_exponent(std::vector<double> my_data) {
	
	int size_lag = 98;
	std::vector<int> lags(size_lag);

	for (int i = 0; i < size_lag; i++) {
    		lags[i] = i;
	}


	std::vector<double> tau(size_lag);
	
	for (int i = 0; i < size_lag; i++) {
		std::vector<double> final_term;
		std::vector<double> term2;
		for (int j = lags[i]; j< my_data.size(); j++) {
			term2[j] = my_data[j];
		}

		for (int k = 0; k < my_data.size()-lags[i]; k++) {
			final_term[k] = my_data[k] - term2[k];
		}
		double x = stdev(my_data);
	}
	
	return 0;

}


int main() {

	
	std::string path = "Data/AMZN.csv";
	
	std::vector<Data> my_data = create_vector(path);
	std::vector<double> closing = closing_price(my_data);
	//display_data(my_data);
	hurst_exponent(closing);


	return 0;
};
