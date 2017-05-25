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

#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/SSLManager.h"
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
	std::string sector;//
	std::string industry;
	std::string headquarters;
	double latitude;
	double longitude;
	while(in.read_row(exchange, symbol, name, sector, industry, headquarters, latitude, longitude)){
		mEntities.insert({symbol, new Entity(symbol, name, sector, industry, headquarters, latitude, longitude)});
	}
}

void FinancialData::loadQuotes() {
	
	bool isServer = false;
	bool isHandleErrorsOnServer = false;
	Poco::SharedPtr<PrivateKeyPassphraseHandler> pConsoleHandler = new KeyConsoleHandler(isServer);
	Poco::SharedPtr<InvalidCertificateHandler> pInvalidCertHandler = new ConsoleCertificateHandler(isHandleErrorsOnServer);
	Context::Ptr pContext = new Context(Context::CLIENT_USE, "", "", "rootcert.pem", Context::VERIFY_RELAXED, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	SSLManager::instance().initializeClient(pConsoleHandler, pInvalidCertHandler, pContext);

	string quotes = "";
	for (unordered_map<string, Entity*>::iterator i = mEntities.begin(); i != mEntities.end(); ++i) {
		if (quotes.length() == 0) {
			quotes = quotes + i->first;
		} else {
			quotes = quotes + "," + i->first;
		}
	}
	Poco::URI uri("https://www.quandl.com/api/v3/datatables/WIKI/PRICES.csv?date=20160912&ticker=" + quotes + "&qopts.columns=date,close,ticker&api_key=oBZVP5YSmEmsFrHaXJ9s");
	
	Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());
	Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, uri.getPath() + "?" + uri.getRawQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
	session.sendRequest(req);
	HTTPResponse response;
	std::istream& rs = session.receiveResponse(response);
	io::CSVReader<3, io::trim_chars<' '>, io::double_quote_escape<',', '"'>> in("quandl", rs);
	in.read_header(io::ignore_extra_column, "date", "close", "ticker");
	std::string ticker;
	std::string date;
	double lastTrade;
	while(in.read_row(date, lastTrade, ticker)){
		printf("%s, %s, %6.2f\n", date.c_str(), ticker.c_str(), lastTrade);
		//mEntities[symbol]->updateMarketData(date, lastTrade, divYield, peRatio, capitalization);
	}
}
