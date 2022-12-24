#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <float.h>
#include <cmath>
#include <numeric>
#include <time.h>
#include <math.h>


/* g++ -o output.exe main.cpp
 * ./output.exe
 */


/* Create a structure of our data, read from Yahoo Finance */
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
	
	// Function to write out all the data
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
	result = sqrt(result/(data.size()-1));

	return result;
}


double least_square(std::vector<double> x_val, std::vector<double> y_val) {
	if (x_val.size() != y_val.size()) {
		return 0;
	}

	double x_mean = std::accumulate(x_val.begin(), x_val.end(), 0.0) / x_val.size();
	double y_mean = std::accumulate(y_val.begin(), y_val.end(), 0.0) / y_val.size();
	double numerator = 0;
	double denominator = 0;

	for (int i = 0; i < x_val.size(); i++) {
		numerator = numerator +  (x_val[i] - x_mean) * (y_val[i] - y_mean);
		denominator = denominator + pow(x_val[i] - x_mean, 2);
	}
	return numerator / denominator;
}	



double hurst_exponent(std::vector<double> my_data, int size_lag) {
	
	std::vector<double> lags(size_lag);

	for (int i = 0; i < size_lag; i++) {
    		lags[i] = i+2;
	}


	std::vector<double> tau(size_lag);
	std::vector<double> final_term;
	std::vector<double> term2;
	double x;

	for (int i = 0; i < size_lag; i++) {
		final_term.resize(my_data.size() - lags[i], 1);
		term2.resize(my_data.size() - lags[i], 1);
		for (int j = lags[i]; j < my_data.size(); j++) {
			term2[j] = my_data[j];
		}

		for (int k = 0; k < my_data.size()-lags[i]; k++) {
			final_term[k] = my_data[k] - term2[k];
		}

		x = stdev(term2);
		tau[i] = sqrt(x);
		final_term.clear();
		term2.clear();
	}
	std::vector<double> lags_log(size_lag);
	std::vector<double> tau_log(size_lag);

	for (int i = 0; i < lags.size(); i++) {
		lags_log[i] = log(lags[i]);
		tau_log[i] = log(tau[i]);
	}


	double hurst = 2*least_square(lags_log, tau_log);
	
	return hurst;

}


std::vector<double> hurst_values(std::vector<double> my_data) {
	std::vector<double> hurst_values;
	for (int i = 2; i < 200; i++) {
		hurst_values.push_back(hurst_exponent(my_data, i));
	}

	return hurst_values;
	
}

void write_to_file(std::vector<double> hurst_vector) {
	std::vector<double> lag_vector(hurst_vector.size());
	for (double i = 0; i < hurst_vector.size(); i++) {
		lag_vector[i] = i+2;
	}
	std::ofstream ofs;
	ofs.open("hurst.txt", std::ofstream::out | std::ofstream::trunc);
	for (int i = 1; i < hurst_vector.size(); i++) {
		ofs << lag_vector[i] << ", " << hurst_vector[i] << std::endl;
	}
	ofs.close();
}


int main() {
	
	std::string path = "Data/GOOG.csv";
	
	std::vector<Data> my_data = create_vector(path);
	std::vector<double> closing = closing_price(my_data);
	double value = hurst_exponent(closing, 98);
	std::cout << "GOOG:" << value << std::endl;
	
 	srand( (unsigned)time( NULL ) );
	std::vector<double> rand_numbers;
	double rand_value = 0;

	for (int i = 0; i < 10000; i++) {
		rand_value = (double) rand()/RAND_MAX + 1;

		rand_numbers.push_back(log(rand_value));
		rand_value = 0;
	}

	std::vector<double> trending(10000);
	double rand_val = 0;
	for (int i = 1; i < trending.size(); i++) {
		trending[i] = log(10 + trending[i-1]);
		
	}



	double v2 = hurst_exponent(rand_numbers, 98);
	std::cout << "Mean reverting series" << v2 << std::endl;	

	double v3 = hurst_exponent(trending, 98);
	std::cout << "Trending" << v3 << std::endl;
	
	std::vector<double> hurst_vector = hurst_values(closing);
	write_to_file(hurst_vector);
	return 0;

};
