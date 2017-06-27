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
#include "SystemConfig.h"

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
#include "Poco/FileStream.h"
#include "Poco/File.h"
#include "Poco/JSON/Parser.h"
#define CSV_IO_NO_THREAD
#include "fast-cpp-csv-parser-master/csv.h"

using namespace Poco::Net;
using Poco::StreamCopier;
using Poco::StringTokenizer;
using Poco::cat;

FinancialData::FinancialData() {
}

FinancialData::FinancialData(string benchmark, string date) {
	mBenchmark = benchmark;
	mDate = date;
	
	Poco::JSON::Parser parser;
	
	Poco::File json(processedFile());
	Poco::Dynamic::Var result;
	
	if (json.exists()) {
		Poco::FileInputStream x(processedFile());
		result = parser.parse(x);
	} else {
		Poco::FileInputStream x(unprocessedFile());
		result = parser.parse(x);
	}
	
	double totalCap = 0.0;
	Poco::JSON::Array::Ptr children = result.extract<Poco::JSON::Array::Ptr>();
	for (int i = 0; i < children->size(); i++) {
		Poco::JSON::Object::Ptr object = children->getObject(i);
		string name = object->getValue<string>("NAME");
		string headquarters = object->getValue<string>("HQ_COUNTRY");
		string sector = object->getValue<string>("SECTOR");
		string similarsGroup = object->getValue<string>("SIMILARS_GROUP");
		
		Entity *entity = new Entity(name, name, sector, similarsGroup, headquarters, i);

		entity->mCompanyXrf = object->getValue<string>("COMPANY_XRF");
		entity->mCompanyZone = object->getValue<string>("COMPANY_ZONE");
		entity->mListingXrf = object->getValue<string>("LISTING_XRF");
		entity->mListingZone = object->getValue<string>("LISTING_ZONE");
		
		entity->mUsdCapitalization = object->getValue<double>("USD_CAP");
		entity->mShares = object->getValue<double>("SHARES");
		
		if (object->has("LATITUDE") && object->has("LONGITUDE")) {
			entity->updateLatitudeLongitude(object->getValue<double>("LATITUDE"), object->getValue<double>("LONGITUDE"));
		} else {
			updateLatLon(entity);
		}
		
		auto sectorIndex = mSectorWeights.find (SectorIndices.at(sector));
		totalCap += entity->mUsdCapitalization;
		if (sectorIndex != mSectorWeights.end()) {
			sectorIndex->second = mSectorWeights.at(SectorIndices.at(sector)) + entity->mUsdCapitalization;
		} else {
			mSectorWeights.insert({SectorIndices.at(sector), entity->mUsdCapitalization});
		}
		mEntities.insert({name, entity});
	}
	
	for (unordered_map<int,double>::iterator i = mSectorWeights.begin(); i != mSectorWeights.end(); ++i) {
		i->second = i->second / totalCap;
	}
	writeJson();
}

string FinancialData::processedFile() {
	return FileRoot + mBenchmark + "_" + mDate + "_processed.json";
}

string FinancialData::unprocessedFile() {
	return FileRoot + mBenchmark + "_" + mDate + ".json";
}

void FinancialData::writeJson() {
	
	Poco::JSON::Array::Ptr entityArray = new Poco::JSON::Array();
	
	int index = 0;
	for (unordered_map<string, Entity*>::iterator entity = mEntities.begin(); entity != mEntities.end(); ++entity) {
		Poco::JSON::Object::Ptr entityObject = new Poco::JSON::Object();
		
		entityObject->set("NAME", entity->second->mName);
		entityObject->set("SECTOR", entity->second->mSector);

		entityObject->set("SIMILARS_GROUP", entity->second->mSimilarsGroup);
		entityObject->set("COMPANY_XRF", entity->second->mCompanyXrf);
		entityObject->set("COMPANY_ZONE", entity->second->mCompanyZone);
		entityObject->set("LISTING_XRF", entity->second->mListingXrf);
		entityObject->set("LISTING_ZONE", entity->second->mListingZone);
		entityObject->set("USD_CAP", entity->second->mUsdCapitalization);
		entityObject->set("SHARES", entity->second->mShares);
		entityObject->set("HQ_COUNTRY", entity->second->mHeadquarterCountry);
		entityObject->set("LATITUDE", entity->second->mLatitude);
		entityObject->set("LONGITUDE", entity->second->mLongitude);
		
		entityArray->set(index, entityObject);
		
		index++;
	}
	Poco::FileOutputStream json(processedFile());
	entityArray->stringify(json);
}

Poco::Dynamic::Var FinancialData::retrieveQuery(string query) {
	
	bool isServer = false;
	bool isHandleErrorsOnServer = false;
	Poco::SharedPtr<PrivateKeyPassphraseHandler> pConsoleHandler = new KeyConsoleHandler(isServer);
	Poco::SharedPtr<InvalidCertificateHandler> pInvalidCertHandler = new ConsoleCertificateHandler(isHandleErrorsOnServer);
	Context::Ptr pContext = new Context(Context::CLIENT_USE, "", "", "rootcert.pem", Context::VERIFY_RELAXED, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	SSLManager::instance().initializeClient(pConsoleHandler, pInvalidCertHandler, pContext);
	
	Poco::URI uri("https://maps.googleapis.com/maps/api/place/textsearch/json?query=" + query + "&key=" + GooglePlacesApiKey);
	
	Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());
	Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, uri.getPath() + "?" + uri.getRawQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
	session.sendRequest(req);
	HTTPResponse response;
	std::istream& rs = session.receiveResponse(response);
	
	Poco::JSON::Parser parser;
	std::istream& json(rs);
	return parser.parse(json);
}

void FinancialData::updateLatLon(Entity *entity) {
	
	try {
		
		string query = entity->mName;
		replace(query.begin(), query.end(), ' ', '+' );
		query += "+headquarters";
		
		Poco::Dynamic::Var result = retrieveQuery(query);
		Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
		
		if (object->getValue<std::string>("status") == "ZERO_RESULTS") {
			result = retrieveQuery(entity->mHeadquarterCountry);
			object = result.extract<Poco::JSON::Object::Ptr>();
		}
		
		//std::string name = object->getObject("results")->getObject("geometry")->getObject("location")->getValue<std::string>("lat");
		std::string lat = object->getArray("results")->getObject(0)->getObject("geometry")->getObject("location")->getValue<std::string>("lat");
		std::string lon = object->getArray("results")->getObject(0)->getObject("geometry")->getObject("location")->getValue<std::string>("lng");
		//Poco::JSON::Array::Ptr children = name->getArray("");
		
		entity->updateLatitudeLongitude(std::stod(lat), std::stod(lon));

	} catch (...) {
		cout << entity->mName << endl;
	}
}
