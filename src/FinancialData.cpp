/*
 
 Copyright (C) 2017 Mauricio Bustos (m@bustos.org)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
*/

#include "FinancialData.hpp"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include <Poco/Net/HTTPCredentials.h>
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"
#define CSV_IO_NO_THREAD
#include "fast-cpp-csv-parser-master/csv.h"

using namespace Poco::Net;
using Poco::StreamCopier;
using Poco::StringTokenizer;
using Poco::cat;

FinancialData::FinancialData() {
}

FinancialData::FinancialData(string benchmark) {
	mBenchmark = benchmark;
	string file = "/Users/mauricio/Documents/development/git/AXAR_ParticleApp/data/index/";
	file = file + benchmark + ".csv";
	io::CSVReader<8, io::trim_chars<' '>, io::double_quote_escape<',', '"'>> in(file);
	in.read_header(io::ignore_extra_column, "Exchange", "Symbol", "Name", "Sector", "Industry", "Headquarters", "Latitude", "Longitude");
	std::string exchange;
	std::string symbol;
	std::string name;
	std::string sector;
	std::string industry;
	std::string headquarters;
	double latitude;
	double longitude;
	while(in.read_row(exchange, symbol, name, sector, industry, headquarters, latitude, longitude)){
		mEntities.insert({symbol, new Entity(symbol, name, sector, industry, headquarters, latitude, longitude)});
	}
}

void FinancialData::loadQuotes() {
	string quotes = "";
	for (unordered_map<string, Entity*>::iterator i = mEntities.begin(); i != mEntities.end(); ++i) {
		if (quotes.length() == 0) {
			quotes = quotes + i->first;
		} else {
			quotes = quotes + "+" + i->first;
		}
	}
	HTTPClientSession s("download.finance.yahoo.com");
	quotes = "/d/quotes.csv?s=" + quotes + "&f=snd1l1yr";
	HTTPRequest request(HTTPRequest::HTTP_GET, quotes);
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',', '"'>> in("yahoo", rs);
	std::string symbol;
	std::string name;
	std::string date;
	double lastTrade;
	double divYield;
	double peRatio;
	while(in.read_row(symbol, name, date, lastTrade, divYield, peRatio)){
		mEntities[symbol]->updateMarketData(date, lastTrade, divYield, peRatio);
	}
}
